#ifndef __zx_top2__
#define __zx_top2__

#include "zx_top1.h"
#include "type.h"




/*       此开发包用于       山景 BP1032A1  
-------------------------------------------------------------------------------------
    TOP2设计规则:尽量保证该文件内的选项为直接选项，其他类型分类放置到top1和top3
-------------------------------------------------------------------------------------
*/



/*****************      writer: 博益友    BOEU   ************************/



/*****************************************************************
 * Note: 蓝牙部分参数已移植到system_config/parameter.ini文件内进行配置,具体如下:
 * 1. 蓝牙名称  BT_NAME/BLE_NAME
 * 2. 蓝牙发射功率  TxPowerLevel/PagePowerLevel
 * 3. 默认的频偏参数  BtTrim
 * 4. TWS主从之间音量控制同步标志  TwsVolSyncEnable

  更多蓝牙相关参数在 bt_config.h
 *****************************************************************/
//////#define _BT_NAME						"BP1032A1"


//--------------------------------------
#define Special_option_3                0
#define MCU_boost_pin                   Port_A29
/*
单独的io口控制主控升压脚
*/





#define Z__UseDesignBoard               0                   //是否使用开发板
//0x198000  //提示音烧录地址           不要超过0x1F0000

#define fun_bt_en                       1
#define fun_udisk_en                    0
#define fun_sd_en                       0
#define fun_linein_en                  	0
#define fun_fm_en                       0
#define fun_pc_en                       0
#define fun_OPTICAL_en                  0  //光纤
#define fun_COAXIAL_en                  0   //同轴
#define fun_HDMI_en                     0   //HDMI IN
#define fun_IIS_in_en                   0   //IIS IN
#define fun_IIS_mix_en                  0   //IIS mix
#define fun_IIS_out_en                  0   //IIS out

#define fun_rec_en                      0  //录音
#define fun_idle_en                     1  // 待机模式; 关机充电逻辑设置




//***************************************************************************** 【外接设备】


//-------------------------------------------------------------------------------linein1
//要做line常开 在 app_config.h  打开 CFG_FUNC_LINE_MIX_MODE
#if fun_linein_en==1
#define Z__linein1_ch                   ANA_INPUT_CH_LINEIN5           //通道选择
#define  Z__CFG_LINEIN_DET_EN				1
#if Z__CFG_LINEIN_DET_EN==1
	#define 	Z__LINEIN_DET_GPIO       GPIOB1
	#define 	Z__LINEIN_DET_GPIO_IN    GPIO_B_IN
	#define     Z__LINEIN_DET_BIT_MASK   GPIO_INDEX1
	#define 	Z__LINEIN_DET_GPIO_IE    GPIO_B_IE
	#define 	Z__LINEIN_DET_GPIO_OE    GPIO_B_OE
	#define 	Z__LINEIN_DET_GPIO_PU    GPIO_B_PU
	#define 	Z__LINEIN_DET_GPIO_PD    GPIO_B_PD

//Line 检测口方式 :  1  电平检测        0  电压检测(AD口)
#define Z__LINEIN_DET_MODE                  1
	
#endif

#endif
// GPIOA30  8
// GPIOA29  9
// GPIOA28  10
#if fun_OPTICAL_en==1
 // #define PORT_B_INPUT_DIGATAL
#ifndef PORT_B_INPUT_DIGATAL 
#define Z__SPDIF_OPTICAL_INDEX              GPIOA29//        GPIOA28
#define Z__SPDIF_OPTICAL_PORT_MODE			10     //                10//

#define Z__SPDIF_OPTICAL_PORT_ANA_INPUT  SPDIF_ANA_INPUT_A29
#else
#define Z__SPDIF_OPTICAL_INDEX          GPIOB1 //  固定端口
#define Z__SPDIF_OPTICAL_PORT_MODE			1

#endif

#endif


#if fun_COAXIAL_en==1


#define Z__SPDIF_COAXIAL_INDEX 		GPIOA31	///GPIOA28
//#define SPDIF_PORT_OE 				GPIO_A_OE
//#define SPDIF_PORT_IE 				GPIO_A_IE
#define Z__SPDIF_COAXIAL_PORT_MODE 		8//10
#define Z__SPDIF_COAXIAL_PORT_ANA_INPUT	SPDIF_ANA_INPUT_A31

#endif





#define Z__DEEPSLEEP_EN                 0 //深度睡眠      app模式运行时 可反复睡眠/唤醒。





#define A1_DEBUG
//***************************************************************************** 【 DEBUG 】


#define Z__DEBUG_custom                 0          //开启串口调试,生产的软件务必关闭   具体打印选项到TOP3开启
#define Z__printf_RATE                  512000              //波特率


#define Z__printf_port                  SWUART_GPIO_PORT_A//打印端口选择 //SWUART_GPIO_PORT_A //SWUART_GPIO_PORT_B
#define Z__printf_bit                  	0                //位



//***************************************************************************** 【 解码 】

#define Z_decode_MP3_EN                 1
#define Z_decode_WAV_EN                 1
#define Z_decode_WMA_EN                 1
#define Z_decode_FLAC_EN                0
#define Z_decode_APE_EN                 0
#define Z_decode_AAC_EN                 0
#define Z_decode_AIF_EN                 0
#define Z_decode_AMR_EN                 0



//AdcLevelKeyProcess //滑块扫描
//AdcLevelMsgProcess   //滑块响应
          
//SysVarInit
//HunXiang
//Music
//RemindSoundServiceItemRequest(SOUND_REMIND_WAWAYIN,FALSE);
//AppModeBtAudioPlay
//ResourcesAppMask  任务列表
//GPIO_PortBModeSet(GPIOB1, 0);




#define A1_PWOER
//***************************************************************************** 【 开关机 】


#define Power_on_off_plan               8                 //开关机方案选择

//开关机方案1--硬开关机 没有io需要操作
//-------------------------------------------
#define P1_valid                        0               //    //1-高电平开机       0-低电平开机       2-按键开关机


//开关机方案2--软开关机 带mos
//-------------------------------------------
#define P2_Pin_POWER_MOS                Port_A0             //MOS控制脚(维持脚)


//开关机方案3--假硬开关机 外观与硬开关机一样 但是有io需要操作
//-------------------------------------------
#define P3_Pin_POWER_MOS                Port_A1             //MOS控制脚
#define Pin_Poweroff_DET                Port_A1             //开关检测脚
#define _INIT_Pin_Poweroff_DET          IO_contral_init(Pin_Poweroff_DET,1,0,1,0);
#define Toggle_Switch_valid             1                   //有效值


//开关机方案4--无mos的软开关机 关机时进行待机操作 开机使用按键进行唤醒 需要设置唤醒脚
//-------------------------------------------
#if Power_on_off_plan==4
#define Pin_wakeup_IO                   Port_A1
#define Pin_wakeup_EDGE                 0                   // 选择唤醒边沿   0下降沿  1上升沿
#endif

//开关机方案5--硬开关机 开机后也可以使用按键进行待机和唤醒
//-------------------------------------------
#if Power_on_off_plan==5
#define Pin_wakeup_IO                   Port_A1
#define Pin_wakeup_EDGE                 0                   // 选择唤醒边沿   0下降沿  1上升沿
#endif


//开关机方案6--带开关的旋钮开关机  主控不掉电 旋钮打开时唤醒 关闭时播放提示音后关机
//-------------------------------------------
#if Power_on_off_plan==6
#define Pin_wakeup_IO                   Port_A1
#define _INIT_Power_plan6               IO_contral_init(Pin_wakeup_IO,1,0,0,0);
#define Pin_wakeup_EDGE                 0                   // 选择唤醒边沿   0下降沿  1上升沿
#endif
// 
//开关机方案7-- IO 型 软开关机 带mos
#define _Pin_POWER_KEY_PORT                A  //开机 IO 按键
#define _Pin_POWER_KEY_PIN                GPIO_INDEX0
         
#define P7_Pin_POWER_MOS                 Port_A0 //MOS控制脚
#define P7_POWER_MOS_ON()                  IO_contral_init(P7_Pin_POWER_MOS,0,0,0,1)
#define P7_POWER_MOS_OFF()                 IO_contral_init(P7_Pin_POWER_MOS,0,0,0,0)



//开关机方案8-- ADkey 软开关机 带mos,按键上电直接开机,不判断按键按下,关机关mos
#define P8_Pin_POWER_MOS                Port_A30            //MOS控制脚(维持脚)
#define POWER_MOS_ON()                  IO_contral_init(P8_Pin_POWER_MOS,0,0,0,1)
#define POWER_MOS_OFF()                 IO_contral_init(P8_Pin_POWER_MOS,0,0,0,0)



#if Power_on_off_plan==7
#define Z__CFG_SOFT_POWER_KEY_EN      

#undef  P1_valid 
#define P1_valid   3
#endif


#if Power_on_off_plan==2
#undef  P1_valid 
#define P1_valid   2
#endif

#if Power_on_off_plan==8

#undef  P1_valid 
#define P1_valid   3
#endif



#define A1_BAT
//*****************************************************************************【电池】
////充电检测可用 CFG_FUNC_OPTION_CHARGER_DETECT

#define BAT_CHECK_EN                    1                   //电池检测使能
#define Pin_BAT_check                   Port_A31
#define Voltage_multiple                2                    //分压系数 3.7V为2, 7.4V为4
#define Adjust_Voltage                  60///微调电压

//电量检测相关参数在 power_monitor.c  LDOIN_POWER_OFF_CYCLE


#define BAT_protect_plan                2                   //电池保护方案选择

//电池保护方案1--电池电压低于报警电压开始报低电提示音 设定多长时间一次  4次后关机
//-------------------------------------------
#if BAT_protect_plan==1
#define BAT_warning_VTG                 6800                //报警电压   示例:345   3.45V 开始报低电提示音
#define BAT_warning_time                30                  //报警间隔时间 2XN单位s
#define BAT_protect_VTG                 6600                 //保护电压

#endif


//电池保护方案2--电池电压低于报警电压开始报低电提示音 间隔时间由参数决定 低于保护电压关机
//-------------------------------------------
#if BAT_protect_plan==2
#define BAT_warning_VTG                 3500                 //报警电压
#define BAT_warning_time                15                  //报警间隔时间 2XN单位s
#define BAT_protect_VTG                 3300                //保护电压
#endif


//--------------------------[电源升压控制]
// 电源升压控制
#define  CHAGER_DCDC_CTRL_EN                         0
#define  CHAGER_DCDC_EN_PIN				       Port_B4	
#define  CHAGER_DCDC_EN_init()            	IO_contral_init(CHAGER_DCDC_EN_PIN,0,0,0,1);
#define  CHAGER_DCDC_EN()            		IO_contral_init(CHAGER_DCDC_EN_PIN,0,0,0,1);
#define  CHAGER_DCDC_DIS()            		IO_contral_init(CHAGER_DCDC_EN_PIN,0,0,0,0);

//--------------------------[RGB电源控制]
// RGB电源控制
#define  RGB_DCDC_CTRL_EN                         0
#define  RGB_DCDC_EN_PIN				       Port_A23
#define  RGB_DCDC_EN_init()            	IO_contral_init(RGB_DCDC_EN_PIN,0,0,0,1);
#define  RGB_DCDC_EN()            		IO_contral_init(RGB_DCDC_EN_PIN,0,0,0,1);
#define  RGB_DCDC_DIS()            		IO_contral_init(RGB_DCDC_EN_PIN,0,0,0,0);


//-------------------------------[无线充控制]
#define WIRELESS_CHAGER_CTRL_EN                         0
#define WIRELESS_CHAGER_PIN				            Port_A20	
#define WIRELESS_CHAGER_init()            	        GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(20));
#define WIRELESS_CHAGER_EN()            			GPIO_RegOneBitSet (GPIO_A_OUT,Z_IO_INDEX(20));
#define WIRELESS_CHAGER_DIS()            			GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(20));






#define Z__SYS_MAX_VOL                        32   //32      开启苹果音量同步后改为15



/** 音量+-设置规则:
      主音量,MIC音量,混响,按键+-分多少级,都是在32级里跳级选择    

      gSysVolArr[] 
**/


#define Z__SYS_GAMUT_VOL                  16 //按键调节主音量级数,分6级,10级,16级
#define Z__SYS_DEFAULT_VOL                10 //开机时系统音量级数,<= Z__SYS_GAMUT_VOL, 0为记忆音量 


#define Z__SYS_MIC_VOL                    10 //按键调节MIC音量级数,分6级,10级,16级
#define Z__SYS_DEFAULT_MIC_VOL            10 //开机时MIC音量级数,<= Z__SYS_MIC_VOL, 0为记忆音量 


#define Z__MIC_ECHO_VOL                   10 //按键调节混响音量级数,分6级,10级,16级
#define Z__SYS_DEFAULT_EOHO_VOL           10 //开机时混响音量级数,<= Z__MIC_ECHO_VOL, 0为记忆音量 

#define Z__MIC_ECHO_ADJ_PALN            1   //混响调节方案:   1 :增益调节      0 :干湿度、延时调节




#define	Z__PROMPT_TONE_VOL			    18 //提示音音量大小,0~32,为0时跟随系统音量



#define    Z__BT_AVRCP_VOLUME_SYNC        1  //开启苹果手机音量同步


#if Z__BT_AVRCP_VOLUME_SYNC
#undef   Z__SYS_MAX_VOL
#define  Z__SYS_MAX_VOL             16//最大音量

#undef   Z__SYS_DEFAULT_VOL
#define  Z__SYS_DEFAULT_VOL         8//开机默认音量

#undef   Z__SYS_GAMUT_VOL
#define  Z__SYS_GAMUT_VOL           0//按键调节等级, 0 =  0~15,  分16级 

#undef   Z__SYS_MIC_VOL
#define  Z__SYS_MIC_VOL             0//按键MIC等级, 0 =  0~15,  分16级 

#undef   Z__MIC_ECHO_VOL
#define  Z__MIC_ECHO_VOL            0//按键混响等级, 0 =  0~15,  分16级 

#undef   Z__PROMPT_TONE_VOL
#define  Z__PROMPT_TONE_VOL         8//提示音音量
#endif

#define Phone_function                  1       //通话功能使能   ENABLE , DISABLE
#define Phone_MIC_NUM                	0 //由音效文件选择       	//通话麦 0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
#define Z__TonePhoneNumber              0                   //来电报号使能

//通话相关配置在  BT_HFP_AEC_ENABLE







//***************************************************************************** 【 音效 】


#define _ALL_EFFECT_EN					1					//总音效使能
#define _MUSIC_EQ_EN					0					//音乐EQ使能
#define ECHO_MIC_EFFECT_EN               	1					//内置混响音效使能

#define Z__EFFECT_usb_DEBUG      		1                   //usb调音效     
#define Z__EFFECT_usb_DEBUG_PASSWORD_EN         1      ////调音通讯加密
#define Z__EFFECT_usb_DEBUG_PASSWORD            0x20211001 //////四字节的长度密码





#define A1_TONE
//***************************************************************************** 【 提示音 】

//#define Z__ToneEN                		1                   //提示音使能


#define Z__PowerOnToneEN                1                   //是否有开机提示音

#define Z__Tone_effect_sw       		1                   //音效切换提示音使能
#define Z__Tone_scene_EN          		1                   //场景音效使能
#define Z__Tone_moyin_share             0                   //合并--魔音提示音  使用 .mp3




#define Z__Max_vol_tone_en              1                   //最大音量提示音使能
#define Z__Min_vol_tone_en              1                   //最小音量提示音使能


#define Z__update_ok_tone_en         	1                   //升级成功提示音使能


//#define Chapter_tone_EN               0                   //曲目报号



#define DSP110P_MAX_VOL         30      //110P最大音量
#define DSP110P_DEF_VOL         30      //110p默认音量





#define A1_PA
//***************************************************************************** 【功放控制】
#if 1



#define PA1_EN              1               //功放1使能
#define PA1_type            PA_type_1_1     //功放1控制方案

#define PA1_mute_pin        Port_A5         //功放1 MUTE脚 //Port_B3



#define PA1_AB_D_pin        Port_A0 //Port_invalid    //功放1 AB类控制脚


#define PA1_power_EN        0               //功放1的电源控制脚使能
#define PA1_power_pin       Port_A23        //功放1的电源控制脚


//-------------------------------------------------

#define PA2_EN              0               //功放2使能
#define PA2_type            PA_type_1_1     //功放2控制方案

#define PA2_mute_pin        Port_A4         //功放2 MUTE脚
#define PA2_AB_D_pin        Port_invalid    //功放2 AB类控制脚


#endif


#define A1_LED
//***************************************************************************** 【LED控制】
/*
单独的io口控制主控升压脚
*/

// 蓝色灯
#define LED1_EN                         1           //指示灯LED1控制io
#define Pin_LED1                        Port_B0
#define LED1_ON_E                       1           //点亮电平  1-高点亮

#define LED2_EN                         0           //指示灯LED1控制io
#define Pin_LED2                        Port_A16
#define LED2_ON_E                       1           //点亮电平  1-高点亮

#define LED3_EN                         0           //指示灯LED1控制io
#define Pin_LED3                        Port_A16
#define LED3_ON_E                       1           //点亮电平  1-高点亮

#define LED4_EN                         0           //指示灯LED1控制io
#define Pin_LED4                        Port_A16
#define LED4_ON_E                       1           //点亮电平  1-高点亮


#define LED1_custom_CTR                 1           //自定义led控制 0使用默认的配置
#if LED1_custom_CTR

/*开机状态                  */    #define Z_led_poweron                   (iLED1|iType_only|LED_plan_breathe)
/*蓝灯熄灭状态                  */  #define Z_led_poweroff                   (iLED1|iType_only|LED_plan_off)

/*配对状态            */ 		#define Z_led_master                    (iLED1|iType_only|LED_plan_slow_flicker4)
/*蓝牙连接成功 未播放状态*/ #define Z_led_bt_connected                         (iLED1|iType_only|LED_plan_on)
/*蓝牙断开连接*/					#define Z_led_bt_disconnected           (iLED1|iType_only|LED_plan_fast_flicker1)
/*TWS配对状态            */ 	#define Z_led_tws_master                (iLED1|iType_only|LED_plan_slow_flicker3)

/*蓝牙播放状态*/				   #define Z_led_bt_play                    (iLED1|iType_only|LED_plan_on)
/*蓝牙暂停状态*/                 #define Z_led_bt_pause                  (iLED1|iType_only|LED_plan_on)
/*作为对箱主机的播放状态    */ #define Z_led_play_with_slave                  (iLED1|iType_only|LED_plan_slow_flicker4)
/*作为对箱主机的暂停状态    */ #define Z_led_pause_with_slave                 (iLED1|iType_only|LED_plan_on)
/*没有对箱的播放状态        */ 	   #define Z_led_play                      (iLED1|iType_only|LED_plan_slow_flicker1)
/*没有对箱的暂停状态        */      #define Z_led_pause                     (iLED1|iType_only|LED_plan_on)

/*music 播放状态            */ #define Z_led_TF_play                   (iLED1|iType_only|LED_plan_slow_flicker1)
/*music 播放暂停            */ #define Z_led_TF_pause                  (iLED1|iType_only|LED_plan_on)
/*AUX 播放状态              */ #define Z_led_AUX_play                  (iLED1|iType_only|LED_plan_slow_flicker4)
/*AUX 播放暂停              */ #define Z_led_AUX_pause                 (iLED1|iType_only|LED_plan_on)
/*FM 播放状态               */ #define Z_led_FM_play                   (iLED1|iType_only|LED_plan_slow_flicker1)
/*FM 播放暂停               */ #define Z_led_FM_pause                  (iLED1|iType_only|LED_plan_on)
/*PC 播放状态               */ #define Z_led_PC_play                   (iLED1|iType_only|LED_plan_slow_flicker1)
/*PC 播放暂停               */ #define Z_led_PC_pause                  (iLED1|iType_only|LED_plan_on)
/*HDMI 播放状态             */ #define Z_led_HDMI_play                 (iLED1|iType_only|LED_plan_slow_flicker1)
/*HDMI 播放暂停             */ #define Z_led_HDMI_pause                (iLED1|iType_only|LED_plan_on)
/*光纤 播放状态             */   #define Z_led_OPTICAL_play                (iLED1|iType_only|LED_plan_slow_flicker1)
/*光纤 播放暂停             */   #define Z_led_OPTICAL_pause               (iLED1|iType_only|LED_plan_on)
/*同轴 播放状态             */   #define Z_led_COAXIAL_play                (iLED1|iType_only|LED_plan_slow_flicker1)
/*同轴 播放暂停             */   #define Z_led_COAXIAL_pause               (iLED1|iType_only|LED_plan_on)


/*电话拨入状态              */ #define Z_led_Call_I                    (iLED1|iType_only|LED_plan_fast_flicker1)
/*电话拨出状态              */ #define Z_led_Call_O                    (iLED1|iType_only|LED_plan_fast_flicker1)
/*电话建立状态              */ #define Z_led_Call_A                    (iLED1|iType_only|LED_plan_slow_flicker1)
/*电话建立挂起              */ #define Z_led_Call_S                    (iLED1|iType_only|LED_plan_on)

/*fm收音状态                */ #define Z_led_FM_scan                 (iLED1|iType_only|LED_plan_fast_flicker1)
/*作为从机状态                */ #define Z_led_slave                   (iLED1|iType_only|LED_plan_on)
/*低电状态                  */ #define Z_led_lowpower                (iLED3|iType_only|LED_plan_warning1)


/*开机状态    绿灯状态          */ #define Z_led2_poweron                (iLED2|iType_only|LED_plan_on)




/*录音过程中，红灯慢闪*/                 #define Z_led3_rec_dealing                    (iLED3|iType_only|LED_plan_slow_flicker1)
/*录音结束，红灯熄灭*/                  #define Z_led3_rec_finish                     (iLED3|iType_only|LED_plan_off)
/*录音文件播放   绿灯慢闪*/              #define Z_led2_recfile_play                   (iLED2|iType_only|LED_plan_slow_flicker1)
/*录音文件播放结束   绿灯熄灭*/            #define Z_led2_recfile_stop                   (iLED2|iType_only|LED_plan_off)


/*音效按键触发 白灯闪烁*/                #define Z_led4_press_key 				(iLED4|iType_only|LED_plan_fast_flicker1)
/*音效按键为触发 白停止灯闪烁*/             #define Z_led4_Nopress_key 				(iLED4|iType_only|LED_plan_off)
#define  Press_key_flashtime  150// *10ms 为单位 白灯闪烁时长 
#endif


//电量灯
#define POWER_LED_EN                 0

#define Power_LED1_pin              Port_A5
#define Power_LED1_ON_status         1    //电量灯开状态
#define Power_LED1_IO_INIT()    IO_contral_init(Power_LED1_pin,0,0,0,!Power_LED1_ON_status)
#define Power_LED1_ON           IO_contral_init(Power_LED1_pin,0,0,0,Power_LED1_ON_status)
#define Power_LED1_OFF          IO_contral_init(Power_LED1_pin,0,0,0,!Power_LED1_ON_status)

#define Power_LED2_pin              Port_A6
#define Power_LED2_ON_status         1    //电量灯开状态
#define Power_LED2_IO_INIT()    IO_contral_init(Power_LED2_pin,0,0,0,!Power_LED2_ON_status)
#define Power_LED2_ON           IO_contral_init(Power_LED2_pin,0,0,0,Power_LED2_ON_status)
#define Power_LED2_OFF          IO_contral_init(Power_LED2_pin,0,0,0,!Power_LED2_ON_status)

#define Power_LED3_pin              Port_A8
#define Power_LED3_ON_status         1    //电量灯开状态
#define Power_LED3_IO_INIT()    IO_contral_init(Power_LED3_pin,0,0,0,!Power_LED3_ON_status)
#define Power_LED3_ON           IO_contral_init(Power_LED3_pin,0,0,0,Power_LED3_ON_status)
#define Power_LED3_OFF          IO_contral_init(Power_LED3_pin,0,0,0,!Power_LED3_ON_status)



//充电检测
#define CHARGE_EN                      1
#define Charge_det_pin               Port_B6
//#define Charge_in_status               1 //充电插入状态
#define Charge_IO_INIT()        IO_contral_init(Charge_det_pin,1,0,0,0)
/*
   如需要插充电上电,逻辑和 fun_idle_en 一起设置
   充电检测定义 CFG_FUNC_OPTION_CHARGER_DETECT
   在sys_gpio.h 具体配置IO选项
*/


//满电检测PIN  (检测充电IC PIN)
#define  Full_Voltage_Pin_EN           0
#define  Full_voltage_det_pin           Port_A1
#define  Full_voltage_pin_init()        IO_contral_init(Full_voltage_det_pin,1,1,0,0) 
#define  Full_voltage_status            1 //充满状态 (充电IC给的状态)
#define  Get_Full_voltage_status()     IO_in_check(Full_voltage_det_pin)



//--------------------------------------------------------------------------混色RGB
//RGB灯开关
#define RGB_Since_the_flash_EN                     0

#define RGB_Since_the_flash_pin                  Port_A6
#define RGB_Since_the_flash_pin_init    IO_contral_init(RGB_Since_the_flash_pin,0,0,0,1)
#define RGB_Since_the_flash_ON          IO_contral_init(RGB_Since_the_flash_pin,0,0,0,1)
#define RGB_Since_the_flash_OFF         IO_contral_init(RGB_Since_the_flash_pin,0,0,0,0)



//RGB DMA
#define CFG_DMA_RGB_LED_EN		         //DMA模式驱动RGB灯
#define RGB_effect_SW_ctr_pin            GPIOA6 // Port_A31
#define RGB_CTR_GPIO_INDEX               GPIO_INDEX6

#define USER_RGB_Lighting_Effects_EN     0 //闪灯方案





#define LEDS_mix_RGB_EN                     1         //混色RGB灯 使能


//-------------------------R
#define LEDS_RGB_R_OPEN                 1     //点亮灯的电平 1高 0低

#define LEDS_RGB_R_IO_pin               Port_A15
#define LEDS_RGB_R_IO_H                 GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(15))
#define LEDS_RGB_R_IO_L                 GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(15))
#define CLOSE_RGB_R_IO                  GPIO_RegOneBitClear(GPIO_A_OUT, Z_IO_INDEX(15));\
GPIO_RegOneBitClear(GPIO_A_OE , Z_IO_INDEX(15));\
GPIO_RegOneBitSet  (GPIO_A_IE , Z_IO_INDEX(15));\
GPIO_RegOneBitClear(GPIO_A_PU,  Z_IO_INDEX(15));

#define OPEN_RGB_R_IO                    GPIO_RegOneBitClear(GPIO_A_IE , Z_IO_INDEX(15));\
GPIO_RegOneBitSet  (GPIO_A_OE , Z_IO_INDEX(15));\
GPIO_RegOneBitSet  (GPIO_A_PU,  Z_IO_INDEX(15));\
GPIO_PortAOutDsSet( Z_IO_INDEX(15),  GPIO_PortA_OUTDS_8MA);

		
//-------------------------G
#define LEDS_RGB_G_OPEN                 1     //点亮灯的电平 1高 0低

#define LEDS_RGB_G_IO_pin               Port_A16
#define LEDS_RGB_G_IO_H                 GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(16))
#define LEDS_RGB_G_IO_L                 GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(16))

#define CLOSE_RGB_G_IO                  GPIO_RegOneBitClear(GPIO_A_OUT, Z_IO_INDEX(16));\
GPIO_RegOneBitClear(GPIO_A_OE , Z_IO_INDEX(16));\
GPIO_RegOneBitSet  (GPIO_A_IE , Z_IO_INDEX(16));\
GPIO_RegOneBitClear(GPIO_A_PU,  Z_IO_INDEX(16));

#define OPEN_RGB_G_IO                    GPIO_RegOneBitClear(GPIO_A_IE , Z_IO_INDEX(16));\
GPIO_RegOneBitSet  (GPIO_A_OE , Z_IO_INDEX(16));\
GPIO_RegOneBitSet  (GPIO_A_PU,  Z_IO_INDEX(16));\
GPIO_PortAOutDsSet( Z_IO_INDEX(16),  GPIO_PortA_OUTDS_8MA);

//-------------------------B
#define LEDS_RGB_B_OPEN                 1     //点亮灯的电平 1高 0低

#define LEDS_RGB_B_IO_pin               Port_A17
#define LEDS_RGB_B_IO_H                 GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(17))
#define LEDS_RGB_B_IO_L                 GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(17))

#define CLOSE_RGB_B_IO                  GPIO_RegOneBitClear(GPIO_A_OUT, Z_IO_INDEX(17));\
GPIO_RegOneBitClear(GPIO_A_OE , Z_IO_INDEX(17));\
GPIO_RegOneBitSet  (GPIO_A_IE , Z_IO_INDEX(17));\
GPIO_RegOneBitClear(GPIO_A_PU,  Z_IO_INDEX(17));

#define OPEN_RGB_B_IO                    GPIO_RegOneBitClear(GPIO_A_IE , Z_IO_INDEX(17));\
GPIO_RegOneBitSet  (GPIO_A_OE , Z_IO_INDEX(17));\
GPIO_RegOneBitSet  (GPIO_A_PU,  Z_IO_INDEX(17));\
GPIO_PortAOutDsSet( Z_IO_INDEX(17),  GPIO_PortA_OUTDS_8MA);






//*******


//-------------------------R
#define RGB_POWER_1_EN                 	 0     
#define RGB_POWER_1_IO_pin               Port_A1
#define RGB_POWER_1_IO_H                 GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(1));
#define RGB_POWER_1_IO_L                 GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(1));

//-------------------------G
#define RGB_POWER_2_EN                 	 0     

#define RGB_POWER_2_IO_pin               Port_A1
#define RGB_POWER_2_IO_H                 GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(1));
#define RGB_POWER_2_IO_L                 GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(1));

//-------------------------B
#define RGB_POWER_3_EN                 	 0     

#define RGB_POWER_3_IO_pin               Port_A1
#define RGB_POWER_3_IO_H                 GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(1));
#define RGB_POWER_3_IO_L                 GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(1));




#define RGB_effect_plan             	1   // 方案选择


//RGB灯 方案1 - 播放音乐时效果为节奏 暂停时或音量很小时为呼吸 具体呼吸和节奏类型可选择
//----------------------------------
#define RGB_effect_breathe_type       	RGB_Effect_breathe1   	// 呼吸类型选择
#define RGB_effect_rhythm_type       	RGB_Effect_rhythm1    	// 节奏类型选择

#define RGB_effect_breathe1_speed  		8       				// 呼吸变化速度


//RGB灯 方案2 - 按键切换效果 (8种) 节奏、呼吸、纯色
//----------------------------------



/*
可供使用的消息:
_Key_fun1_RGB_SW,     开关RGB
_Key_fun1_RGB_next,   下一个,包含关闭
_Key_fun1_RGB_next2,  下一个,不包含关闭
*/



#define A1_KEY
//***************************************************************************** 【 按键控制 】

#define Remote_433_EN                       0           //433 遥控器使能 
#define Remote_433_pin                      Port_B4          
#define IR_Receive_IO_INI()                 IO_contral_init(Remote_433_pin,1,0,0,0)
#define IR_Receive_IO()		               GPIO_RegOneBitGet(GPIO_B_IN, GPIO_INDEX4)// IO_in_check(Remote_433_pin)	


#define Remote_EN                       0           //遥控器使能 
#define Remote_UserCode                 0xfc03//0x03fc      //红外遥控用户码  
#define Pin_IR                          Port_B6     //Port_B6,Port_B7,Port_A29 只能使用其中一个
#define ir_default_en                   0           //使用默认遥控
#if Z__DEEPSLEEP_EN
#define Z__IR_KEY_POWER					0x1d  //  实际遥控器power键码值  
#endif


#define Z_IO_ADC_key_Det_EN         1//如果IO按键和AD按键同时触发,AD按键无效

//长按n秒应用
#define   Z_key_long_ns_EN                 1



#define Z_pwoerkey_EN                  1           //按键powerkey识别 使能 固定powerkey脚
#define Z_pwoerkey_11_EN               0           //开启后powerkey支持11个按键 前提不能开启内部mos


#define Z_ADkey_1_EN                    0          //按键ADKEY1识别 使能
#define Pin_ADK1                        Port_A23                     

#define Z_ADkey_2_EN                    0           //按键ADKEY2识别 使能
#define Pin_ADK2                        Port_A24


#define IO_KEY_10_EN                    1           //编号为10的按键
#define Pin_KEY_10_key                  Port_B1
#define _INIT_IO_KEY_10                 IO_contral_init(Pin_KEY_10_key,1,0,1,0);
#define _Active_IO_KEY_10               1           //有效电平

#define IO_KEY_11_EN                    0           //编号为11的按键
#define Pin_KEY_11_key                  Port_A2
#define _INIT_IO_KEY_11                 IO_contral_init(Pin_KEY_11_key,1,1,0,0);
#define _Active_IO_KEY_11               0           //有效电平

#define IO_KEY_12_EN                    0           //编号为12的按键
#define Pin_KEY_12_key                  Port_A2
#define _INIT_IO_KEY_12                 IO_contral_init(Pin_KEY_12_key,1,0,0,0);
#define _Active_IO_KEY_12               1           //有效电平

#define IO_KEY_13_EN                    0           //编号为13的按键
#define Pin_KEY_13_key                  Port_B0
#define _INIT_IO_KEY_13                 IO_contral_init(Pin_KEY_13_key,1,0,0,0);
#define _Active_IO_KEY_13               1           //有效电平




//***************************************************************************** 【自定义输出】

#define Custom_out1_EN                  0           //自定义控制io
#define Pin_Custom_out1                 Port_B5
#define Custom_out1_Default_state       0           //开机默认状态
#define _INIT_IO_Custom_out1            IO_contral_init(Pin_Custom_out1,1,1,1,0);  //初始化函数


#define Custom_out2_EN                  0           //自定义控制io
#define Pin_Custom_out2                 Port_A23
#define Custom_out2_Default_state       0          //开机默认状态
#define _INIT_IO_Custom_out2            IO_contral_init(Pin_Custom_out2,0,0,0,Custom_out2_Default_state);  //初始化函数


#define Custom_out3_EN                  0           //自定义控制io
#define Pin_Custom_out3                 Port_B5
#define Custom_out3_Default_state       0           //开机默认状态
#define _INIT_IO_Custom_out3            IO_contral_init(Pin_Custom_out3,0,0,0,Custom_out3_Default_state);  //初始化函数

#define LED_YELLOW_EN					0
#define Pin_LED_YELLOW					Port_A9
#define LED_YELLOW_DEFULAT_STATE		0			//开机默认状态
#define _INIT_IO_LED_YELLOW				IO_contral_init(Pin_LED_YELLOW,0,0,0,LED_YELLOW_DEFULAT_STATE);  //初始化函数
#define LED_YELLOW_OPEN					IO_contral_init(Pin_LED_YELLOW,0,0,0,1);  //初始化函数
#define LED_YELLOW_CLOSE				IO_contral_init(Pin_LED_YELLOW,0,0,0,0);  //初始化函数

//***************************************************************************** 【 X1协议 】
//X1_send_msg(0);

#define X1_protocol_EN                      	0                   //X1协议使能


#define X1_tx_EN                            	0
#define X1_tx_pin                           	Port_A6

#define X1_rx_EN                            	0
#define X1_rx_pin                           	Port_B5

#define _X1_SEND_MSG_POWER_OFF          		28
#define _X1_SEND_MSG_POWER_ON 		        	27

#define _X1_SEND_MSG_LED_MODE_HDMI_ON          	17
#define _X1_SEND_MSG_LED_MODE_HDMI_OFF         	18
#define _X1_SEND_MSG_LED_MODE_SPIDIF_ON      	19
#define _X1_SEND_MSG_LED_MODE_SPIDIF_OFF      	20
#define _X1_SEND_MSG_LED_MODE_OPTICAL_ON		21
#define _X1_SEND_MSG_LED_MODE_OPTICAL_OFF      	22
#define _X1_SEND_MSG_LED_MODE_BT_ON      		23
#define _X1_SEND_MSG_LED_MODE_BT_OFF      		24
#define _X1_SEND_MSG_LED_MODE_AUX_ON      		25
#define _X1_SEND_MSG_LED_MODE_AUX_OFF      		26

#define _X1_SEND_MSG_LED_EQ_MODE_GAME_ON		13//7
#define _X1_SEND_MSG_LED_EQ_MODE_GAME_OFF		14//8
#define _X1_SEND_MSG_LED_EQ_MODE_NEWS_ON		7//9
#define _X1_SEND_MSG_LED_EQ_MODE_NEWS_OFF		8//10
#define _X1_SEND_MSG_LED_EQ_MODE_MUSIC_ON		9//13
#define _X1_SEND_MSG_LED_EQ_MODE_MUSIC_OFF		10//14
#define _X1_SEND_MSG_LED_EQ_MODE_MOVIE_ON		11
#define _X1_SEND_MSG_LED_EQ_MODE_MOVIE_OFF		12


#define Set_MIC_Tmr_EN            0  //音量计时回到默认MIC音量


//***************************************************************************** 【 IIC 】


#define IIC_protocol_EN                     0                   //IIC协议使能



#define IIC_DAT_pin                         Port_B4
#define IIC_DAT_IO_IN                       Port_I__B4
#define IIC_DAT_IO_OUT                      Port_O__B4
#define IIC_DAT_IO_OUT1                     Port_OH_B4
#define IIC_DAT_IO_OUT0                     Port_OL_B4
#define IIC_DAT_IO_PU1                      Port_UH_B4
#define IIC_DAT_IO_PU0                      Port_UL_B4
#define IIC_DAT_IO_R                        Port_R__B4

//--------------------

#define IIC_CLK_pin                         Port_B5
#define IIC_CLK_IO_O                        Port_O__B5
#define IIC_CLK_IO_UH                       Port_UH_B5
#define IIC_CLK_IO_DL                       Port_DL_B5
#define IIC_CLK_IO_OH                       Port_OH_B5
#define IIC_CLK_IO_OL                       Port_OL_B5


//***************************************************************************** 【 扩展芯片 】

//--------------------------------------------------------------------------FM发射

#define QN8027_EN               0

#define QN8027_pre1             875      //7600 ~ 10800 MHz
#define QN8027_pre2             1080      

#define QN8027_pre_MAX          2           //总频点个数   

/*
可供使用的消息:
_Key_fun1_QN8027_sw,      开关 QN8027  
_Key_fun1_QN8027_next,    循环切换下一个频率  不包含关闭
_Key_fun1_QN8027_next2,    循环切换下一个频率   包含关闭
*/



//--------------------------------------------------------------------------u段发射

#define KT0641_EN               0
#define KT0641_power_pin		Port_A8     

#define KT0641_pre1             662000      //470000 ~ 960000
#define KT0641_pre2             800000      
#define KT0641_pre3             900000      

#define KT0641_pre_MAX          3           //总频点个数   

/*
可供使用的消息:
_Key_fun1_KT0641_sw,      开关KT0641  关闭电源方式
_Key_fun1_KT0641_next,    循环切换下一个频率
*/


//按键开关消息  _Key_fun1_BTMIC_Power_sw
#define BT_MIC_POWER_EN                   0
#define MIC_Pin_Custom_out1               Port_B5
#define _INIT_IO_MIC_Pin_out1             IO_contral_init(MIC_Pin_Custom_out1,1,0,0,0);  //初始化函数
#define BTMIC_POWER_KEY_HOLD_TIME         2//  蓝牙麦 软按键开关机 长按持续时间 单位秒

/*
    如果蓝牙唛芯片配置为power脚拉低1秒开机,用以上方式的基础上(BT_MIC_POWER_EN 打开),
    开机自动打开MIC,不需要按键开关(如果不用按键开关,按键就不定义消息)
*/
#define  CFG_POWER_ON_MICON_EN            0



// 数码管
#define DIGITAL_TUBE_EN                               0

////  数码管IO口配置在 seg_led_disp.h,  填真值在 seg_led_disp.c  的 LedFlushDisp() 函数
/*
    DispIcon(ICON__H1, LIGHT_OFF);  //点亮其中一段
    DispString(" OFF");   //显示字符
*/



#if DIGITAL_TUBE_EN
//#define DISP_DEV_AIP1668_LED
#endif

////  数码管IO口配置在 seg_led_disp.h,  填真值在 seg_led_disp.c  的 LedFlushDisp() 函数
/*
    DispIcon(ICON__H1, LIGHT_OFF);  //点亮其中一段
    DispString(" OFF");   //显示字符
*/

#if  DIGITAL_TUBE_EN 
#define DISPLAY_LINE_LED_EN        0  //LED布局为行列形式, 非数码管(数字) //项目显示方案
#define Rhythm_Spectrum_EN         0  //频谱律动显示
#endif

//----------------------------------------------AIP1668 驱动控制-----------------------------

#ifdef DISP_DEV_AIP1668_LED
// 片选脚
#define AIP1668_STB_pin                         Port_A28
#define AIP1668_STB_IN                          Port_I__A28
#define AIP1668_STB_OUT                       Port_O__A28
#define AIP1668_STB_OUT1                     Port_OH_A28
#define AIP1668_STB_OUT0                     Port_OL_A28
#define AIP1668_STB_PU1                       Port_UH_A28
#define AIP1668_STB_PU0                       Port_UL_A28
#define AIP1668_STB_DW1                       Port_DH_A28
#define AIP1668_STB_DW0                       Port_DL_A28
#define AIP1668_STB_R                           Port_R__A28

//--------------------
// 时钟脚
#define AIP1668_CLK_pin                        Port_A29
#define AIP1668_CLK_O                          Port_O__A29
#define AIP1668_CLK_UH                        Port_UH_A29
#define AIP1668_CLK_UL                        Port_UL_A29
#define AIP1668_CLK_DL                        Port_DL_A29
#define AIP1668_CLK_OH                       Port_OH_A29
#define AIP1668_CLK_OL                        Port_OL_A29
// 数据脚
#define AIP1668_DAT_pin                         Port_A0
#define AIP1668_DAT_IN                          Port_I__A0
#define AIP1668_DAT_OUT                       Port_O__A0
#define AIP1668_DAT_OUT1                     Port_OH_A0
#define AIP1668_DAT_OUT0                     Port_OL_A0
#define AIP1668_DAT_PU1                       Port_UH_A0
#define AIP1668_DAT_PU0                       Port_UL_A0
#define AIP1668_DAT_DW1                       Port_DH_A0
#define AIP1668_DAT_DW0                       Port_DL_A0
#define AIP1668_DAT_R                           Port_R__A0



#define AIP1668_IO_INIT()	  do{\
								AIP1668_STB_OUT;AIP1668_STB_PU1;AIP1668_STB_DW0;AIP1668_STB_OUT1;\
								AIP1668_CLK_O;AIP1668_CLK_UH;AIP1668_CLK_DL;AIP1668_CLK_OH;\
							       AIP1668_DAT_OUT;AIP1668_DAT_PU1;AIP1668_DAT_DW0;AIP1668_DAT_OUT1;\
								}while(0)
#define AIP1668_STB(x)			(x)?(AIP1668_STB_OUT1):(AIP1668_STB_OUT0);
#define AIP1668_CLK(x)			(x)?(AIP1668_CLK_OH):(AIP1668_CLK_OL);
#define AIP1668_DIO_IN()		do{AIP1668_DAT_IN;}while(0)
#define AIP1668_DIO_OUT(x)	do{\
								AIP1668_DAT_OUT;\
								AIP1668_DAT_PU0;\
								AIP1668_DAT_DW0;\
								(x)?(AIP1668_DAT_OUT1):(AIP1668_DAT_OUT0);\
							}while(0)
#define AIP1668_DIO(x)			(x)?(AIP1668_DAT_OUT1):(AIP1668_DAT_OUT0);



#endif

//--------------------------------------------------------------------------DSP-110P

#define DSP110P_EN                0
#define DSP110P_Reset_Pin         Port_A7                 //
#define DSP110P_address	 	      0xe6                      //从机地址


#define DSP110P_ch_tone         0
#define DSP_110P_CH1   1
#define DSP_110P_CH2   2
#define DSP_110P_CH3   3

#define  DSP110P_ch1_to_ac6921      DSP_110P_CH1   // AC6921 主通道，直接进入NAP110P  ch1
#define  DSP110P_ch1_to_line  		DSP_110P_CH2   //  line 直接进入NAP110p ch2
#define  DSP110P_ch1_to_fm    		DSP_110P_CH3   //  line 直接进入NAP110p ch3

#define   DSP_LINEIN_CHANNEL_DIFFERENT   0   // line 直接进NAP110P
#define   DSP_FM_CHANNEL_DIFFERENT       0   // FM 直接进NAP110P
#if DSP110P_EN
#define DSP110P_MAIN_VOL_EN           0
#define Pin_MainVolSwitch                    Port_A4

#define DSP110P_110p_TREBLE_EN             0
#define Pin_DSP110PTREBLEVolSwitch                    Port_A5


#define DSP110P_110p_BASS_EN             0
#define Pin_DSP110PBASSVolSwitch                    Port_A6



#endif

//***************************************************************************** 【 旋钮电位器 】



#define Z__CFG_FUNC_BT_VOL_EN             0                  //音乐滑块使能
#define Z_BTVOL_AdcLevelCh                2//GPIO29
#define BT_VOL_block_reverse              0                   //0 为正序； 1为反序 

//  music 高音
#define Z__CFG_FUNC_MUSIC_TREB_EN            0
#define Z_Music_TREB_AdcLevelCh              2//GPIO21
#define Music_TREB_block_reverse             0// 0 为正序； 1为反序 


// music 低音
#define Z__CFG_FUNC_MUSIC_BASS_EN             0
#define Z_Music_BASS_AdcLevelCh               3//GPIO22
#define Music_BASS_block_reverse              0// 0 为正序； 1为反序


#if  (Z__CFG_FUNC_MUSIC_TREB_EN==1)||(Z__CFG_FUNC_MUSIC_BASS_EN==1)
#define Z__CFG_FUNC_MUSIC_TREB_BASS_EN     1 //  音乐高低音控制使能开关
#else
#define Z__CFG_FUNC_MUSIC_TREB_BASS_EN     0 //  音乐高低音控制使能开关

#endif




//***************************************************************************** 【内置混响】
//一般K歌唛初始化音效用HunXiang.c, 纯蓝牙音箱初始化音效用 Music.c
#define Built_in_echo_EN                1 // 1: 混响使能(应用K歌宝,K歌音箱) 0: 关闭混响(应用纯蓝牙音箱)
#define Built_in_MIC_NUM                3 // 1      //混响mic选择       0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2

// Built_in_echo_EN 打开 初始化音效会用HunXiang.c
// Built_in_echo_EN 关闭 初始化音效会用 Music.c

#define Z__CFG_FUNC_MIC_ECHO_REVERB_GAIN_KEY_EN   0


//mic echo reverb gain 调节   
#define Z__CFG_FUNC_MIC_ECHO_REVERB_GAIN_EN          0                //混响滑块使能
#define Z_MIC_ECHO_AdcLevelCh               2// GPIO28
#define MIC_ECHO_block_reverse              1                   //滑块是否反向


#define Z__CFG_FUNC_MIC_CFG_FUNC_MIC_VOLUME_EN           0           
#define Z_MIC_VOL_AdcLevelCh                1 // GPIO20
#define MIC_VOL_block_reverse               1                   //滑块是否反向

//  mic  高音
#define Z__CFG_FUNC_MIC_TREB_EN            0
#define Z_MIC_TREB_AdcLevelCh              2//GPIO21
#define MIC_TREB_block_reverse             0// 0 为正序； 1为反序 


// mic 低音
#define Z__CFG_FUNC_MIC_BASS_EN             0
#define Z_MIC_BASS_AdcLevelCh               3//GPIO22
#define MIC_BASS_block_reverse              0// 0 为正序； 1为反序


//BT MIC A 电源控制
#define  BT_MICA_CTRL_EN                         0
#define  BT_MICA_POWER_PIN				       Port_A6	
#define  BT_MICA_POWER_init()            	IO_contral_init(BT_MICA_POWER_PIN,0,0,0,1)
#define  BT_MICA_POWER_EN()            		IO_contral_init(BT_MICA_POWER_PIN,0,0,0,1)
#define  BT_MICA_POWER_DIS()            	IO_contral_init(BT_MICA_POWER_PIN,0,0,0,0)

//BT MIC B 电源控制
#define  BT_MICB_CTRL_EN                         0
#define  BT_MICB_POWER_PIN				       Port_A24	
#define  BT_MICB_POWER_init()            	IO_contral_init(BT_MICB_POWER_PIN,0,0,0,1)
#define  BT_MICB_POWER_EN()            		IO_contral_init(BT_MICB_POWER_PIN,0,0,0,1)
#define  BT_MICB_POWER_DIS()            	IO_contral_init(BT_MICB_POWER_PIN,0,0,0,0)


#define Echo_MIC_DET_EN                 0                   //内置混响MIC检测使能
#define Pin_Echo_MIC_DET                Port_A29
#define _INIT_IO_MIC                    IO_contral_init(Pin_Echo_MIC_DET,1,1,0,0);
#define Effective_Pin_Echo_MIC          0                   //有效电平


#define BT_MIC_A_DET_EN                 0                   //BT MICA 检测使能
#define Pin_BT_MIC_A_DET                Port_A0
#define _INIT_IO_BT_MIC_A               IO_contral_init(Pin_BT_MIC_A_DET,1,1,0,0);
#define Effective_Pin_BT_MIC_A          0                   //有效电平


#define BT_MIC_B_DET_EN                 0                   //BT MICB 检测使能
#define Pin_BT_MIC_B_DET                Port_A0
#define _INIT_IO_BT_MIC_B               IO_contral_init(Pin_BT_MIC_B_DET,1,1,0,0);
#define Effective_Pin_BT_MIC_B          0                   //有效电平


//BT MIC  A唛 B唛 共用一个检测口
#if BT_MIC_A_DET_EN==0 && BT_MIC_A_DET_EN==0

#define BT_MIC_DET_EN                  0                   //BT MICA 检测使能
#define Pin_BT_MIC_DET                 Port_A9
#define _INIT_IO_BT_MIC                IO_contral_init(Pin_BT_MIC_DET,1,1,0,0);
#define Effective_Pin_BT_MIC           0                   //有效电平

#endif





#if  (Z__CFG_FUNC_MIC_TREB_EN==1)||(Z__CFG_FUNC_MIC_BASS_EN==1)
#define Z__CFG_FUNC_MIC_TREB_BASS_EN     1 //  音乐高低音控制使能开关
#else
#define Z__CFG_FUNC_MIC_TREB_BASS_EN     0 //  音乐高低音控制使能开关

#endif


// zsh A2
#if Z__CFG_FUNC_MIC_ECHO_REVERB_GAIN_EN|| Z__CFG_FUNC_MIC_ECHO_REVERB_GAIN_KEY_EN //mic echo reverb gain 调节   
#define CFG_FUNC_MIC_ECHO_REVERB_GAIN_EN
#endif
		

#define Z_KEY_MIC_CTRL_EN                 0//按键控制MIC 1



#define Wireless_MIC_power_EN           0                   //无线麦电源脚使能
#define Pin_Wireless_MIC                Port_A24




//------------------------------------------------------------------------------- 耳机


#define PH_DET_EN                       0                //耳机检测使能
#define Pin_PH_DET                      Port_A20
#define _INIT_PH_DET_pin            IO_contral_init(Pin_PH_DET,1,1,0,0);
#define EV_PH                               0                   //耳机检测电平

#define  PH_EN                            0 //耳放使能
#define  PH_EN_pin                        Port_A0
#define  PH_EN_pin_init           IO_contral_init(PH_EN_pin,0,0,0,0)
#define  PH_EN_pin_ON             IO_contral_init(PH_EN_pin,0,0,0,1)
#define  PH_EN_pin_OFF            IO_contral_init(PH_EN_pin,0,0,0,0)


#if PH_DET_EN  
/* 耳机音效,一般默认关闭,除非客户需求  
   PH_off_line.c  耳机拔出,注意 与 HunXiang.c 同步更新
   PH_on_line.c   耳机插入  
*/
    #define PH_DET_AUDIO_EFFECT_EN        0//耳机拔插刷音效

#endif





//对箱连接时,音效做调整,关闭一些,避免爆资源(一般应用K歌唛会用较多资源)   
//如果TWS播放正常,或不做TWS,则不需要开启
#if Built_in_echo_EN
#define Z_TWS_connection_Sound_effects_Adjust_EN               0
#endif


//对箱连接时,不可以消原音;开启消原音对箱配对后也会取消消原音
#define Z_TWS_Connection_Clean_Voice_OFF_EN          0



// 编码器
//  IO选择在  app_config.h     // CFG_RES_CODE_KEY_USE
// 编码器触发消息在 key.c 的 CODEKEY_TAB[] 定义
#define  Z_CFG_RES_CODE_KEY_USE                            0

#define  CODE_KEY_MODE_IO_SCAN             0  //IO扫描方式  (消息在 Use_Encoder_fun() 设置)
#define  Encoder_A_pin         Port_A10
#define  Encoder_B_pin         Port_A9
//#define  Z_CFG_CODE_KEY1P_PIN_A         GPIOA0
//#define  Z_CFG_CODE_KEY1P_PIN_B         GPIOA1



//蓝牙无连接自动关机
//多少分钟无连接自动关机, 0 不开启此功能
#define  Z_No_Connection_Timing_Power_Off_EN               30

//蓝牙暂停播放多少分钟自动关,0 不开启此功能
#if fun_bt_en
#define   Z_BT_Pause_Timing_Power_Off_EN                    0
#endif

//T卡\U盘暂停播放多少分钟自动关,0 不开启此功能
#if fun_sd_en || fun_udisk_en
#define   Z_Media_Pause_Timing_Power_Off_EN                    0
#endif



//用于防止音量+-最大(小)提示音不松键播很多次
// 与 变量 max_min_tone_msg_send_en 关联
#define  Z_MAX_MIN_TONE_MSG_SEND_At_a_time_EN                   1

//按键双击功能
/////////////#define  Z_key_double_EN                   0




/* 蓝牙和TWS相关在bt_config.h  
*  蓝牙发射功率在 BT_TX_POWER_LEVEL
*  TWS相关配置  BT_TWS_SUPPORT
*  TWS配对条件 TWS_FILTER_NAME
*/

// TWS主从播放状态同步设置
#define USER_TWS_PLAY_State_Sync_EN            1





//***************************************************************************** 【 音频输出 】
//    如下输出源可同时输出；

#define Z_AUDIO_out_Ch_DAC0        		1     //选择 DAC0 通道
#if Z_AUDIO_out_Ch_DAC0
#define CFG_DAC0_EN                     1   // 是否要用软mute
#endif

#define Z_AUDIO_out_Ch_DACX        		1     //选择 DACX 通道
#define Z_DACX_NUM						AUDIO_DACX_SINK_NUM + AUDIO_CORE_SOURCE_MAX_MUN
#if Z_AUDIO_out_Ch_DACX
#define CFG_DAC1_EN                     1   // 是否要用软mute
#define CFG_FUNC_REMIND_MIX_AUDIO_EN    1  //是否做直播提示音复合输出DACX

#endif
#define Z_AUDIO_out_Ch_IIS        		0     //选择 IIS 通道


#define Z_All_Tone_Out_DACx_EN          0 //所有提示音输出DACX


#define BOEU_DACX_OUT_PAM_EN        	0  //DACX 通道 输出喇叭




//***************************************************************************** 【 按键功能 】

#define Poweron_key_check_EN        0       //开机按键检测  不合要求不能开机
#define Poweron_key_number          PowK1_Res_100R   //指定开机键的按键编号 错误会导致无法开机 只有开机方案2,4有效

//ZX_key_info

//433 遥控
#if Remote_433_EN
#define  Z__Remote_433_KEY \
          {\
            Remote_01,{{_Key_click__ ,_Key_fun1_bianyin},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_MIC_SW}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_02,{{_Key_click__ ,_Key_fun1_bianyin},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_03,{{_Key_click__ ,_Key_fun1_sw_vol_up},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_04,{{_Key_click__ ,_Key_fun1_sw_vol_down},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_05,{{_Key_click__ ,_Key_fun1_bianyin},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_06,{{_Key_click__ ,_Key_fun1_MIC_SW},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_07,{{_Key_click__ ,_Key_fun1_prev},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_08,{{_Key_click__ ,_Key_fun1_next},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_09,{{_Key_click__ ,_Key_fun1_bianyin},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	       {\
            Remote_0a,{{_Key_click__ ,_Key_fun1_tone_bishi},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_0b,{{_Key_click__ ,_Key_fun1_PAM_SW},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_0c,{{_Key_click__ ,_Key_fun1_effect_SW},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_pp}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_0d,{{_Key_click__ ,_Key_fun1_bianyin},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_0e,{{_Key_click__ ,_Key_fun1_tone_hongxiao},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_0f,{{_Key_click__ ,_Key_fun1_bianyin},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_10,{{_Key_click__ ,_Key_fun1_tone_huanhu},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },\
	      {\
            Remote_11,{{_Key_click__ ,_Key_fun1_tone_zhangsheng},\
                          {_Key_double_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	      },
	  
	      
#endif





#if IO_KEY_10_EN
#define Z__IOKEY \
          {\
            IOKey_num__1,{{_Key_click__ ,_Key_fun1_volume_up},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_next}, \
        		          {_Key_long_2s ,}, \
        		          {_Key_long_3s ,}, \
        		          {_Key_long_4s ,}, \
        		          {_Key_long_5s ,}, \
        		          {_Key_long_6s ,}, \
        		          {_Key_hold___ ,}},\
	    },\

#endif

#if IO_KEY_11_EN
#define Z__IOKEY \
          {\
            IOKey_num__2,{{_Key_click__ ,_Key_fun1_effect_SW},\
                          {_Key_double_ ,_Key_fun1_pp}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_powerdown}, \
        		          {_Key_long_2s ,}, \
        		          {_Key_long_3s ,}, \
        		          {_Key_long_4s ,}, \
        		          {_Key_long_5s ,}, \
        		          {_Key_long_6s ,}, \
        		          {_Key_hold___ ,}},\
	    },\

#endif


#if Z_pwoerkey_EN
 #if Z_pwoerkey_11_EN
 #define Z__POWERKEY \
         {\
            PowK2_Res_100R,{{_Key_click__ ,_Key_fun1_MIC_vol_down},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_clean_voice}, \
        		          {_Key_hold___ ,}},\
	    },\
        {\
            PowK2_Res_2_2,{{_Key_click__ ,  _Key_fun1_volume_down},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s , _Key_fun1_prev}, \
        		          {_Key_hold___ ,}},\
	    },\
        {\
            PowK2_Res_4_7,{{_Key_click__ ,_Key_fun1_MIC_vol_up},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_TWS}, \
        		          {_Key_hold___ ,}},\
	    },\
        {\
            PowK2_Res_8_2,{{_Key_click__ , },\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s , }, \
        		          {_Key_hold___ ,}},\
	    },\
        {\
            PowK2_Res__12,{{_Key_click__ ,_Key_fun1_MIC_ECHO_UP},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,}},\
	    },

 
 #else
#define Z__POWERKEY \
        {\
            PowK1_Res_100R,{{_Key_click__ ,_Key_fun1_pp},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_powerdown}, \
        		          {_Key_long_2s ,}, \
        		          {_Key_long_3s ,}, \
        		          {_Key_long_4s ,}, \
        		          {_Key_long_5s ,}, \
        		          {_Key_long_6s ,}, \
        		          {_Key_hold___ ,}},\
	    },\
	    {\
            PowK1_Res__16,{{_Key_click__ ,},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun2_TWS_bt_dis}, \
        		          {_Key_long_2s ,}, \
        		          {_Key_long_3s ,}, \
        		          {_Key_long_4s ,}, \
        		          {_Key_long_5s ,_Key_fun1_TWS}, \
        		          {_Key_long_6s ,}, \
        		          {_Key_hold___ ,}},\
	    },\
         {\
            PowK1_Res__82,{{_Key_click__ ,_Key_fun1_volume_down},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_prev}, \
        		          {_Key_long_2s ,}, \
        		          {_Key_long_3s ,}, \
        		          {_Key_long_4s ,}, \
        		          {_Key_long_5s ,}, \
        		          {_Key_long_6s ,}, \
        		          {_Key_hold___ ,}},\
	    },

#endif

#endif


#if Z_ADkey_1_EN
#define Z__ADK1 \
         {\
            ADK1_Res___0,{{_Key_click__ ,_Key_fun1_clean_voice},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_powerdown}, \
        		          {_Key_hold___ ,}},\
	    },\
	    {\
            ADK1_Res_8_2,{{_Key_click__ ,_Key_fun1_pp},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_mode}, \
        		          {_Key_hold___ ,}},\
	    },\
	    {\
            ADK1_Res__12,{{_Key_click__ ,_Key_fun1_prev},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_volume_down}, \
        		          {_Key_hold___ ,_Key_fun1_volume_down}},\
	    },\
        {\
            ADK1_Res__18,{{_Key_click__ ,_Key_fun1_next},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,}, \
        		          {_Key_hold___ ,_Key_fun1_volume_up}},\
	    },\
	     {\
            ADK1_Res__27,{{_Key_click__ ,_Key_fun1_DMA_RGB_MODE_effect_SW},\
                          {_Key_double_ ,}, \
                          {_Key_CLICK3_ ,}, \
        		          {_Key_long_1s ,_Key_fun1_DMA_RGB_SW}, \
        		          {_Key_hold___ ,}},\
	    },
  

#endif

		
#if Z_ADkey_2_EN
#define Z__ADK2 \
				{\
					ADK2_Res___0,{{_Key_click__ ,_Key_fun1_scene_ktv},\
								  {_Key_double_ ,}, \
								  {_Key_CLICK3_ ,}, \
								  {_Key_long_1s ,}, \
								  {_Key_hold___ ,}},\
				},\
				{\
					ADK2_Res_2_2,{{_Key_click__ ,_Key_fun1_scene_micba},\
								  {_Key_double_ ,}, \
								  {_Key_CLICK3_ ,}, \
								  {_Key_long_1s ,_Key_fun1_volume_up}, \
								  {_Key_hold___ ,_Key_fun1_volume_up}},\
				},\
				{\
					ADK2_Res_4_7,{{_Key_click__ ,_Key_fun1_scene_zhuanye},\
								  {_Key_double_ ,}, \
								  {_Key_CLICK3_ ,}, \
								  {_Key_long_1s ,}, \
								  {_Key_hold___ ,}},\
				},\
				{\
					ADK2_Res_8_2,{{_Key_click__ ,_Key_fun1_volume_down},\
								  {_Key_double_ ,_Key_fun1_prev}, \
								  {_Key_CLICK3_ ,}, \
								  {_Key_long_1s ,}, \
								  {_Key_hold___ ,}},\
				},\
				{\
					ADK2_Res__12,{{_Key_click__ ,_Key_fun1_volume_up},\
								  {_Key_double_ ,_Key_fun1_next}, \
								  {_Key_CLICK3_ ,}, \
								  {_Key_long_1s ,}, \
								  {_Key_hold___ ,}},\
				},\
				{\
					ADK2_Res__18,{{_Key_click__ ,_Key_fun1_MIC_SW},\
								  {_Key_double_ ,}, \
								  {_Key_CLICK3_ ,}, \
								  {_Key_long_1s ,}, \
								  {_Key_hold___ ,}},\
				},
				
#endif

#if  0
#define Z__IRKEY \
        {\
            IR_1d,{{_Key_click__ ,_Key_fun1_mode_idle},\
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_18,{{_Key_click__ ,_Key_fun1_mute}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_09,{{_Key_click__ ,_Key_fun1_volume_up}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,_Key_fun1_volume_up}},\
	    },\
        {\
            IR_01,{{_Key_click__ ,_Key_fun1_volume_down}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,_Key_fun1_volume_down}},\
	    },\
        {\
            IR_0b,{{_Key_click__ ,_Key_fun1_pp}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
	    {\
            IR_0a,{{_Key_click__ ,_Key_fun1_dacx_vol_up}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,_Key_fun1_prev}, \
        		   {_Key_hold___ ,}},\
	    },\
	    {\
            IR_08,{{_Key_click__ ,_Key_fun1_dacx_vol_down}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,_Key_fun1_next}, \
        		   {_Key_hold___ ,}},\
	    },\
	    {\
            IR_0d,{{_Key_click__ ,_Key_fun1_mode_hdmi}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
	    {\
            IR_1e,{{_Key_click__ ,_Key_fun1_mode_spidif}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
		{\
			IR_1f,{{_Key_click__ ,_Key_fun1_mode_optical}, \
				   {_Key_double_ ,}, \
				   {_Key_CLICK3_ ,}, \
				   {_Key_long_1s ,}, \
				   {_Key_hold___ ,}},\
		},\
		{\
			IR_02,{{_Key_click__ , _Key_fun1_mode_aux}, \
				   {_Key_double_ ,}, \
				   {_Key_CLICK3_ ,}, \
				   {_Key_long_1s ,}, \
				   {_Key_hold___ ,}},\
		},\
		{\
			IR_05,{{_Key_click__ ,_Key_fun1_mode_bt}, \
				   {_Key_double_ ,}, \
				   {_Key_CLICK3_ ,}, \
				   {_Key_long_1s ,}, \
				   {_Key_hold___ ,}},\
		},\
	 	{\
            IR_15,{{_Key_click__ ,_Key_fun1_110p_eq_movie}, \
                   {_Key_double_ ,}, \
                   {_Key_CLICK3_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
		{\
			IR_4b,{{_Key_click__ ,_Key_fun1_110p_eq_music}, \
				   {_Key_double_ ,}, \
				   {_Key_CLICK3_ ,}, \
				   {_Key_long_1s ,}, \
				   {_Key_hold___ ,}},\
		},\
		{\
			IR_4a,{{_Key_click__ ,_Key_fun1_110p_eq_game}, \
				   {_Key_double_ ,}, \
				   {_Key_CLICK3_ ,}, \
				   {_Key_long_1s ,}, \
				   {_Key_hold___ ,}},\
		},\
		{\
			IR_11,{{_Key_click__ ,_Key_fun1_110p_eq_news}, \
				   {_Key_double_ ,}, \
				   {_Key_CLICK3_ ,}, \
				   {_Key_long_1s ,}, \
				   {_Key_hold___ ,}},\
		},
#endif



#if  X1_rx_EN
#define Z__X1_PT \
				{\
					X1_MSG1,\
					{\
						{_Key_click__ ,  },\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG2,\
					{\
						{_Key_click__ , _Key_fun1_pp },\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG3,\
					{\
						{_Key_click__ ,_Key_fun1_prev },\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG4,\
					{\
						{_Key_click__ ,_Key_fun1_next},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG5,\
					{\
						{_Key_click__ ,_Key_fun1_bianyin},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG6,\
					{\
						{_Key_click__ ,_Key_fun1_clean_voice},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG7,\
					{\
						{_Key_click__ ,},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG8,\
					{\
						{_Key_click__ ,_Key_fun1_tone_max},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG9,\
					{\
						{_Key_click__ ,},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG10,\
					{\
						{_Key_click__ ,_Key_fun3_sw_vol_down},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG11,\
					{\
						{_Key_click__ ,_Key_fun3_sw_vol_up},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},\
				{\
					X1_MSG12,\
					{\
						{_Key_click__ ,_Key_fun1_set_vol_sw},\
						{_Key_long_1s ,},\
						{_Key_hold___ ,}\
					},\
				},
			
		
#endif




#endif




