/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //图像地址

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PIT0_IRQHandler();
void PORTE_IRQHandler();

uint8  var1 = 20, var2;
uint16 var3, var4;
uint32 var5, var6;


/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外 无线调试 测试实验
 */
uint32 number  = 0;
void main()
{
  gpio_init(PTD15,GPO,LOW);
  gpio_init(PTC0,GPO,HIGH);
  Oled_Menu_init();
  ftm_pwm_init(FTM0,FTM_CH0,10*1000,5000);
  ftm_pwm_init(FTM0,FTM_CH1,10*1000,0);
  ftm_pwm_init(FTM0,FTM_CH2,10*1000,5000);
  ftm_pwm_init(FTM0,FTM_CH3,10*1000,0);
   
  while(1)
  {
    float num[7] ={10.0,20.0,15.0,15.5,12.2,16.0,12.0};
    Display_Page1(num);
   
    
    ftm_pwm_duty(FTM0,FTM_CH0,2500);
    ftm_pwm_duty(FTM0,FTM_CH1,0);
    ftm_pwm_duty(FTM0,FTM_CH2,2500);
    ftm_pwm_duty(FTM0,FTM_CH3,0);
  }
}
void  main1(void)
{
    Site_t site     = {0, 0};                           //显示图像左上角位置
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小
    Size_t size     = {80, 60}; //{LCD_W, LCD_H};       //显示区域图像大小
    uint32 i;
    com_e     com;
    nrf_result_e nrf_result;
    uint8 imgtxflag = 0;    //0表示发送图像

    /************************ 配置 K60 的优先级  ***********************/
    //K60 的默认优先级 都为 0
    //参考帖子：急求中断嵌套的例程 - 智能车讨论区 - 山外论坛
    //          http://www.vcan123.com/forum.php?mod=viewthread&tid=499&page=1#pid3270
    NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级

    NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
    NVIC_SetPriority(PORTE_IRQn,1);         //配置优先级
    NVIC_SetPriority(DMA0_IRQn,2);          //配置优先级
    NVIC_SetPriority(PIT0_IRQn,3);          //配置优先级

    /************************** LED 初始化  ***************************/
    led_init(LED_MAX);


    /************************ LCD 液晶屏 初始化  ***********************/
    LCD_init();

    /************************ 无线模块 NRF 初始化  ***********************/
    while(!nrf_init());
    //配置中断服务函数
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTE_IRQn);
    nrf_msg_init();                                                     //无线模块消息初始化

    //摄像头模块，需要 发送 空命令给 调试板模块，这样可以清掉 接收图像失败而产生多余数据
    i = 20;
    while(i--)
    {
        nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //发送多个 空 命令过去，用于清空 接收端的缓存数据
    }

    /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                                   //按键消息初始化
    pit_init_ms(PIT0,10);                                               //pit 定时中断(用于按键定时扫描)
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
    enable_irq(PIT0_IRQn);

    /************************ 摄像头 初始化  ***********************/
    camera_init(imgbuff);                                   //摄像头初始化，把图像采集到 imgbuff 地址
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler


    while(1)
    {
        /************************ 图像采集和显示  ***********************/
        camera_get_img();                           //摄像头获取图像

        /************************ 无线发送和接收数据  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
            if(nrf_result == NRF_RESULT_RX_VALID)
            {
                switch(com)
                {
                    case COM_IMG:
                        //显示 到 屏幕上
#if     ( CAMERA_COLOR == 1 )                                       //灰度摄像头
                        LCD_Img_gray_Z(site,size, imgbuff,imgsize);
#elif   ( CAMERA_COLOR == 0 )                                       //黑白摄像头
                        LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
#endif
                        break;

                    case COM_VAR:
                        if(last_tab == CAR_CTRL)                    //如果是 控制 小车 命令
                        {
                            switch(car_ctrl_get())                  //获取小车控制命令，并进行处理
                            {
                            case CAR_START:                         //开车
                                //此处加入 开车 代码
                                led(LED0,LED_ON);
                                break;

                            case CAR_STOP:                          //停车
                                //此处加入 停车 代码 ，例如：关闭电机驱动（关闭使能）
                                led(LED0,LED_OFF);
                                break;

                            case CAR_IMG_OPEN:
                                imgtxflag = 0;
                                break;

                            case CAR_IMG_CLOSE:
                                imgtxflag = 1;
                                break;


                            default:
                                //无效，不进行任何处理
                                break;
                            }
                        }

                        break;
                    default:
                        break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);         //接收不到数据 才退出

        if(!imgtxflag)
        {
            //把图像 发送出去
            nrf_msg_tx(COM_IMG,nrf_tx_buff);
        }

#if     ( CAMERA_COLOR == 1 )                                       //灰度摄像头
        LCD_Img_gray_Z       (site,size, imgbuff,imgsize);
#elif   ( CAMERA_COLOR == 0 )                                       //黑白摄像头
        LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
#endif

        if(!imgtxflag)
        {
            while(nrf_tx_state() == NRF_TXING);             //等待发送完成
        }

        /*********************** 按键消息 处理  ***********************/
        deal_key_event();
    }
}

/*

知识点 提醒;

1.鹰眼 摄像头 采集回来的图像是 压缩图像，需要进行解压，参考如下 文章:
        鹰眼图像解压，转为 二维数组 - 智能车资料区 - 山外论坛
        http://www.vcan123.com/forum.php?mod=viewthread&tid=17


2.图像 地址 转换为 二维数组。
        如果是 鹰眼，参考 第1个知识点说明

        如果是其他 灰度图像摄像头，修改如下代码:
        uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_buff) + COM_LEN);     //图像地址
        改为
        uint8 (*imgbuff)[CAMERA_H][CAMERA_W] =   (uint8 (*)[CAMERA_H][CAMERA_W])(((uint8 *)&nrf_buff) + COM_LEN);     //图像地址
        实际上 就是强制 类型转换。

3.摄像头的采集地址 是在 摄像头初始化是传递进去的，此后的图像都采集到该地址上。
        camera_init(imgbuff);                                   //摄像头初始化，把图像采集到 imgbuff 地址

4.无线模块，检测数据接收后，才进行 发送，这样可以尽量避免 无线模块同时出于 发送状态。

5.嵌套中断的问题，参考如下帖子:
        急求中断嵌套的例程 - 智能车讨论区 - 山外论坛
        http://www.vcan123.com/forum.php?mod=viewthread&tid=499&page=1#pid3270

6.按键扫描，是采用定时扫描方式进行扫描的


*/



/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    //while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif

}

/*!
 *  @brief      PORTE中断服务函数
 *  @since      v5.0
 */
void PORTE_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //清中断标志位

    n = 27;
    if(flag & (1 << n))                                 //PTE27触发中断
    {
        nrf_handler();
    }
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/*!
 *  @brief      PIT0中断服务函数
 *  @since      v5.0
 */
void PIT0_IRQHandler()
{
    key_IRQHandler();

    PIT_Flag_Clear(PIT0);
}

