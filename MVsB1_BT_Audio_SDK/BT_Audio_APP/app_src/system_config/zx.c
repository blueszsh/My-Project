#include "zx_top3.h"
#include "debug.h"
#include "gpio.h"
#include "app_message.h"
#include "ir_key.h"
#include "adc_key.h"
#include "adc.h"
#include "irqn.h"
#include "ctrlvars.h"
#include "timer.h"
//#include "rtos_api.h"
#include "rtos_api.h"
#include "main_task.h"

#include "dac.h"
#include "bt_tws_api.h"

#include "display.h"
#include "seg_panel.h"
#include "qn8027.h"
#include "iic.h"
#include "bt_manager.h"
#include "power_monitor.h"
#include "bt_play_api.h"
#include "media_play_api.h"
#include "bt_play_mode.h"


/*
#define u8          uint8_t
#define u16         uint16_t
#define u32         uint32_t
*/

#define NO_MSG         0xffff

//extern ControlVariablesContext gCtrlVars;
//MessageContext		Z_msgSend;
S_MY_DEV_INF g_t_dev_inf = {
	.appMode		= 0,
	.fakeOffFlag	= 1,
	.is_init		= 1,
	.dacxVol		= 31,
	.fristSend		= 1,
};



idle_sw Idle_sw = 
{
    0,
	0,
	0,
};

bool Led_Flicker_falg = 0;


u16 Save_key_number;
u16 Save_key_number2=0;
u16 Save_KeyMsg_ype=0;
u8 inquire_key_i;
u8 inquire_key_j;


u8 BtPairTonePlay=0;

u32 Get_Curr_Music_energy;

bool BT_MIC_state=FALSE;
bool BT_MIC2_state=FALSE;
bool PowerOn_Key_Press=FALSE;
bool PowerOff_Key_Press=FALSE;
bool Single_Press_EN=TRUE;
bool BT_PowerKey_Ctr_EN=FALSE;
u16 btmic_power_cnt=2;//BTMIC_POWER_KEY_HOLD_TIME;

u8 Vol_Sw_Flag = Set_Mic_Vol;
u8 SetMicTmrFlag;


/*
蓝牙状态flag, 和 BtAccessModeNotAccessible 几个状态配合使用.
  MSG_BT_MID_STACK_INIT
*/
bool use_bt_mode_flag = 1; //  0  蓝牙不可搜索不可连接   1 蓝牙可搜索连接  

u8 g_tws_mic_echo;
u8 g_tws_mic_rever;
u8 g_tws_mic_vol;
u8 power_on_mute_po_flag = 0;    //解决开机音和蓝牙音之间的po声
u8 power_on_tone_flag =0;  //mute 开机前PO声
u8 pam_en;
u8 BT_state_flag = 0; //蓝牙与手机连接 flag    1  连接     0 未连接
bool is_bt_mode=0;//是否蓝牙模式, 1 在蓝牙模式    0  非蓝牙
u8 g_led_power = 1;
u8 frist_set_eq = 0;
u8 yellow_light = 0;
u8 Tws_Slave_PA_state;// 从机同步主机  1: 解mute   0: mute   只在TWS下发送、判断
u8 PowerOnInitAudioEffect_flag=0; //开机刷初始化音效 flag
u8 moyin_in_out_flag = moyin_out;
u8 Tws_play_flag=0; //TWS播放状态flag 设置同步   0 暂停    1 播放  ( 主要用于从机同步)

#if Z__SYS_GAMUT_VOL != 0
u8 Custom_MusicVolume = Z__SYS_DEFAULT_VOL;
#else
u8 Custom_MusicVolume = 0;
#endif

#if Z__SYS_MIC_VOL != 0
u8 MIC_VOL_Adjust_flag=Z__SYS_DEFAULT_MIC_VOL;
#else
u8 MIC_VOL_Adjust_flag=0;
#endif

#if Z__MIC_ECHO_VOL != 0
u8 Custom_MicECHO_Volume = Z__SYS_DEFAULT_EOHO_VOL;
#else
u8 Custom_MicECHO_Volume=0;
#endif

u8 MicECHO_Volume;
u8 Music_Volume;
u8 Mic_Volume;
bool APP_Volume_UP_DW_Flag; //音量+- flag    1 : 加   0 : 减

bool max_min_tone_msg_send_en=TRUE;  //TRUE : 可以发送  FALSE  不可以发送

u8 LOW_voltage_flag = 0;//低电flag  =1 低电 (在报提示音后)

u8 PAM_ctrl_en=0;   // == 0 喇叭打开   == 1开始播喇叭关闭提示音  == 2  提示音播完,喇叭关

u8 FM_TX_FALG=0;

u16 led_effect_num;
u8 update_ok;


u8 power_on_key_none = 1;

u8 Curr_key_number=0xff;
u8 ADkey_press_num=0xff;//提供全局查询adkey按键当前被按下的按键编号 开机标志未打开时也可以查询
u8 ADkey2_press_num=0xff;
u8 IOkey_press_num=0xff;
u8	Press_Key_cnt=0;//  200ms时长
bool Power_On_Low_Power_Flag = FALSE; //上电检测低电flag  
u16 _1s_con;
u8 phone_state;
bool music_mute_flag=FALSE;
u8 ZX_key_info_length;
u8 Flag_turnon=0;
u8 Flag_key_clock = 1;
u8 Global_playing_flag;
u8  Global_rec_playing_flag=0;
u8 RGB_effect_num;
u8 Global_pause_flag;
bool Power_Key_Releasd=0;
u8 Machine_state=Machine_Poweroff;
bool wireless_charger_switch=0;
u8 power_down_zx_flag = 0; //进关机函数FALG
u8 Global_PA_work;
u8 PA_work_mode;
u8 Last_PA_workmode;
u8 Task_state;
u8 BT_state;
u8 Tone_play_state;
u8 Tws_play_state;
u8 Tws_Slave_state=0;
u8 Tws_state=0;

u8 Tws_master_bt_con=0;//tws连接,主机回连手机

u8 TWS_DIS_tone = 0;

u8 Custom_Event1_number;
u8 Custom_Event1_get;
u8 Z_init_ok;
bool mic1_ctrl_en=TRUE;
bool mic2_ctrl_en=TRUE;
bool mic_switch=FALSE;
u8 temp_bianyi_state=EFFECT_MODE_YuanSheng;

int16_t *mic_pcm_temp;  

//u32 _Curr_Music_energy;
//u32 _Curr_Music_energy_2;
u8 A2DP_Play_State=0;
bool TWS_Slave_A2DP_Play_State=0;//为从机时同步主机 的A2DP 状态
u32 _Curr_MIC_energy;

u8 KTV_MIC_state;
u8 Com_mute_contral;
u8 PH_online_state;

u8 RGB_SW;
u8 RGB_R_Grade;
u8 RGB_G_Grade;
u8 RGB_B_Grade;

u8 LED_Grade;


u32 Curr_DAC_energy;
u8 RGB_curr_effect = 0xff;//RGB_effect_breathe_type;
u8 Temp_RGB_curr_effect = 0xff;

u8 RGB_back_effect = RGB_effect_breathe_type;//RGB_effect_breathe_type;

bool DMA_RGB_POWER_SW = 1; // 1 开   0 关

u8 DAC_energy_top_flag=0;


//u8 linein_det_status=0xff;
u8 Flag_fm_scan;
u8 Flag_low_power;
u8 Flag_low_power_tone;
u8 Flag_poweroff=0;
u8 Flag_IR_poweroff=0;


u8 PA_keep_on;
u8 PA_keep_off;
u8 update_ok;
u8 rgb_timer_state;
u8 Flag_max_vol = 0;
u8 Fm_sel_mode=Fm_sel_next_station;
u8 bianyi_state=EFFECT_MODE_YuanSheng;
u8 effect_state=EFFECT_MODE_KTV;

u8 Slave_effect_bianyin_state;

bool mode_switch_flag=FALSE;
bool rest_mode_flag=FALSE;
bool tone_play_flag = FALSE;




Z_Task_inf *Curr_task_inf;

Z_Task_inf T_bt_inf=
{
    Task_bt,
    Task_online,
    0,
};
Z_Task_inf T_music_inf;
Z_Task_inf T_sd0_inf=
{
    Task_sd_for_sd0,
    Task_offline,
    0,
};
Z_Task_inf T_sd1_inf=
{
    Task_sd_for_sd1,
    Task_offline,
    0,
};
Z_Task_inf T_usb_inf=
{
    Task_sd_for_usb,
    Task_offline,
    0,
};
Z_Task_inf T_FM_inf=
{
    Task_fm,
    Task_online,
    0,
};
Z_Task_inf T_OPTICAL_inf=
{
    Task_OPTICAL,
    Task_online,
    0,
};
Z_Task_inf T_COAXIAL_inf=
{
    Task_COAXIAL,
    Task_online,
    0,
};
Z_Task_inf T_HDMI_in_inf=
{
    Task_HDMI_in,
    Task_online,
    0,
};
Z_Task_inf T_IIS_in_inf=
{
    Task_IIS_in,
    Task_online,
    0,
};

Z_Task_inf T_linein1_inf=
{
    Task_AUX_for_linein1,
    Task_offline,
    0,
};
Z_Task_inf T_TwsSlave_inf=
{
    Task_TwsSlave,
    Task_offline,
    0,
};
/*Z_Task_inf T_linein3_inf=
{
    Task_AUX_for_linein3,
    Task_offline,
    0,
};*/
	
Z_Task_inf T_pc_inf=
{
    Task_pc,
    Task_offline,
    0,
};
Z_Task_Rec_inf T_rec_inf=
{
    Task_rec,
    Task_online,
    0,
};


Z_Task_inf T_fm_inf;
Z_Task_inf T_idle_inf;

//Z_Task_inf T_EX_linein1_inf;
//Z_Task_inf T_EX_linein2_inf;
//Z_Task_inf T_EX_linein3_inf;







/*const u16 powerkey_adTab[]=
{
	(0+ADC_200V)/2,
	ADC_300V,
	ADC_300V,
	ADC_300V,
	ADC_300V,
};*/

struct _X1_PT X1_PT_inf=
{
    X1_wait,
    0,
    0,

    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,
    
    0,0,0,4,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,15
};


const _Key_fun_info ZX_key_info[]=
{
#ifdef Z__IOKEY
    Z__IOKEY
#endif

#ifdef Z__POWERKEY
    Z__POWERKEY
#endif

#ifdef Z__ADK1
    Z__ADK1
#endif

#ifdef Z__ADK2
    Z__ADK2
#endif

#ifdef Z__IRKEY
    Z__IRKEY
#endif

#ifdef Z__Remote_433_KEY
    Z__Remote_433_KEY
#endif


#ifdef Z__X1_PT
    Z__X1_PT
#endif

};

void Update_presskey_number()
{
    if(ADkey_press_num!=0xff)
    {
        Curr_key_number = ADkey_press_num;
        //DEBUG_ZX(DEBUG_other, "Update_presskey_number AD1", Curr_key_number);
    }
    else if(ADkey2_press_num!=0xff)
    {
        Curr_key_number = ADkey2_press_num;
        //DEBUG_ZX(DEBUG_other, "Update_presskey_number AD2", Curr_key_number);
    }
    else if(IOkey_press_num!=0xff)
    {
        Curr_key_number = IOkey_press_num;
        //DEBUG_ZX(DEBUG_other, "Update_presskey_number IO", Curr_key_number);
    }
    else
    {
        Curr_key_number = 0xff;
        //DEBUG_ZX(DEBUG_other, "Update_presskey_number ", Curr_key_number);
    }


}

void _1s_fun()
{
      //	MessageContext		msgSend;


  /* printf("Curr_DAC_energy == %d\n",Curr_DAC_energy); 
   printf("Global_playing_flag == %d\n",Global_playing_flag);
   printf("T_bt_inf.play_state == %d\n",T_bt_inf.play_state);
    printf("\n",); */
}

void _10ms_fun()
{
    Input_check();
    LED_scan_II();


#if BT_MIC_POWER_EN
   if(Machine_state>=Machine_run&&PowerOn_Key_Press==TRUE)
   	{
		if(btmic_power_cnt>0&&BT_PowerKey_Ctr_EN)
			{
			 IO_contral_init(MIC_Pin_Custom_out1,0,0,0,0);
			btmic_power_cnt--;
			}
		if(btmic_power_cnt==0)
	       {
	       //DBG("wiless mic open doing\n");
	       btmic_power_cnt=BTMIC_POWER_KEY_HOLD_TIME;
	       BT_PowerKey_Ctr_EN=FALSE;
		   PowerOn_Key_Press=FALSE;
	       IO_contral_init(MIC_Pin_Custom_out1,1,0,0,0);
		   
		 
		}
		//else
		  //IO_contral_init(MIC_Pin_Custom_out1,0,0,0,1); 	
	
   }

      if(Machine_state>=Machine_run&&PowerOff_Key_Press==TRUE)
     	{
		if(btmic_power_cnt>0&&BT_PowerKey_Ctr_EN)
			{
			 IO_contral_init(MIC_Pin_Custom_out1,0,0,0,0);
			btmic_power_cnt--;
			}
		if(btmic_power_cnt==0)
	    {
	      // DBG("wiless mic close doing\n");
	       btmic_power_cnt=BTMIC_POWER_KEY_HOLD_TIME;
	       BT_PowerKey_Ctr_EN=FALSE;
		   PowerOff_Key_Press=FALSE;
	       IO_contral_init(MIC_Pin_Custom_out1,1,0,0,0);
		 
		}
		//else
		  //IO_contral_init(MIC_Pin_Custom_out1,0,0,0,1); 	
	
   }
#endif
	


}

void _100ms_fun()
{
	static u8 cnt = 0;


	if (power_on_key_none == 1){
		if (cnt++ == 40){
			power_on_key_none = 0;
			cnt = 0;
		}

	}

    #if 0//Built_in_echo_EN
	  if(power_on_tone_flag != 0xff)
	  {
		    if(power_on_tone_flag == 0)
		    {
		        IO_contral_init(PA1_mute_pin,0,0,0,0);
			}
			else if(power_on_tone_flag == 1)//播开机音时
			{
                // power_on_tone_flag = 2;         
				 
				 #if PH_DET_EN
				 if(IO_in_check(Pin_PH_DET) != EV_PH)//没插耳机
				 #endif
				 {
                   //  IO_contral_init(PA1_mute_pin,0,0,0,1);
				 }
			}
           // AudioCoreSourceMute2(MIC_SOURCE_NUM,TRUE,TRUE);//关MIC,防止开机时刷音效前共震
			
	  }
    #endif
   

	
}

void _1ms_fun()
{
    static u8 _100ms_con=0;
	static u8 _2ms_con_m=0;
	static u8 _3ms_con_m=0;
	static u16 _1000ms_con=0;

   #if Built_in_echo_EN
	static u16 cnt = 0;
   #endif 


   #if Z_No_Connection_Timing_Power_Off_EN > 0
        BOEU_BT_No_Connection_Timing_Power_Off();
   #endif
   #if Z_BT_Pause_Timing_Power_Off_EN > 0
        BOEU_BT_Pause_Timing_Power_Off();
   #endif
   #if Z_Media_Pause_Timing_Power_Off_EN > 0
        BOEU_Media_Pause_Timing_Power_Off();
   #endif

/*
	if (Task_state==Task_fm) {
		//DBG("Task_state : %x\n", Task_state);
		if((PA_work_mode==PA_D_unmute)||(PA_work_mode==PA_AB_unmute)){
			fm_cnt++;
			if (fm_cnt <= 1){
				IO_contral_init(PA1_mute_pin,0,0,0,0);
			}
			else if (fm_cnt <= 11){
				IO_contral_init(PA1_mute_pin,0,0,0,1);
				fm_cnt = 0;
			}

		} else {
			fm_cnt = 0;
		}	
		
	}*/
    _2ms_con_m++;
	if(_2ms_con_m==2)
	{
        _2ms_fun();
        _2ms_con_m=0;
	}
    _3ms_con_m++;
	if(_3ms_con_m==2)
	{
	   #if LEDS_mix_RGB_EN
		if(Z_init_ok)
		{
		   //RGB_Contral();
		   Blue_LED_PowerOn_Show();
		   RGB_Mode_Contral();
		}
       #endif
	   
        _3ms_con_m=0;
		
	}
	
    _100ms_con++;
	if(_100ms_con==100)
	{
        _100ms_fun();
        _100ms_con=0;
	}

	_1000ms_con++;
	if(_1000ms_con==1000)
	{
        _1s_fun();
        _1000ms_con=0;
	}

 //应用K歌唛,开机前几秒(刷初始化音效前),MIC关闭,防啸叫、共震
#if 0//Built_in_echo_EN 
    /* if(cnt != 0xffff)
	  {
	      cnt++;
		  AudioCoreSourceMute2(MIC_SOURCE_NUM, 1, 1);
		  if(cnt >= 6830)//开机到刷初始化音效的时间会受开机提示音的长短影响
		  {
             cnt = 0xffff;
			 AudioCoreSourceUnmute(MIC_SOURCE_NUM, 1, 1);
		  }
          
	  }*/
	  if(PowerOnInitAudioEffect_flag != 0xff)
	  {
          if(PowerOnInitAudioEffect_flag == 0)//开机初始化音效前
		  {	     
			  AudioCoreSourceMute(MIC_SOURCE_NUM, 1, 1);		  
		  }   
		  else if(PowerOnInitAudioEffect_flag == 1)
		  {
		      PowerOnInitAudioEffect_flag = 0xff;
	          AudioCoreSourceUnmute(MIC_SOURCE_NUM, 1, 1);
		  }
	  }
	  
	  
#endif


}
void _2ms_fun()
{
    static u8 _10ms_con=0;
    _1s_con++;
    if(_1s_con==500)
    {
        _1s_con=0;
        //DBG(" _1s_: %d \n", 1);
    }
    _10ms_con++;
    if(_10ms_con==5)
    {
        _10ms_con=0;
        _10ms_fun();
    }
    /*
    if(IO_in_check(X1_rx_pin))
    {
        IO_contral_init(Port_B2,0,0,0,1);
    }
    else
    {
        IO_contral_init(Port_B2,0,0,0,0);
    }*/
    
    X1_PT_deal();
}
void Io_init()
{

   
    IO_contral_init(Port_A7,1,0,0,0);
    //IO_contral_init(Port_A8,1,0,0,0);
    //IO_contral_init(Port_A9,1,0,0,0);
    //IO_contral_init(Port_A10,1,0,0,0);
    //IO_contral_init(Port_A23,1,0,0,0);
    //IO_contral_init(Port_A17,1,0,0,0);
    //IO_contral_init(Port_A5,0,1,0,1);
    //IO_contral_init(Port_A6,0,1,0,1);

    //IO_contral_init(Port_B0,1,0,0,0);
    //IO_contral_init(Port_B1,1,0,0,0);

   #if CHAGER_DCDC_CTRL_EN
	 CHAGER_DCDC_EN_init();
   #endif

      #if RGB_DCDC_CTRL_EN
	 RGB_DCDC_EN_init();
   #endif
   
    #if WIRELESS_CHAGER_CTRL_EN
	WIRELESS_CHAGER_init();
	#endif
	
    #if IIC_protocol_EN
    IO_contral_init(IIC_DAT_pin,0,0,0,1);
    IO_contral_init(IIC_CLK_pin,0,0,0,1);
    #endif



    #if BAT_CHECK_EN&&(Pin_BAT_check!=Port_LDOIN)
    IO_ANA_init(Pin_BAT_check);
    #endif


    #if Wireless_MIC_power_EN
    IO_contral_init(Pin_Wireless_MIC,0,0,0,1);
    #endif
    
    #if X1_protocol_EN
    #if X1_tx_EN
    IO_contral_init(X1_tx_pin,0,0,0,1);
    #endif
    #if X1_rx_EN
    IO_contral_init(X1_rx_pin,1,1,0,0);
    #endif
    #endif

    #if Custom_out1_EN
   // _INIT_IO_Custom_out1;
   
	GPIO_RegOneBitSet  (GPIO_B_IE, Z_IO_INDEX(5));
	GPIO_RegOneBitClear(GPIO_B_OE, Z_IO_INDEX(5));
	GPIO_RegOneBitSet  (GPIO_B_PU, Z_IO_INDEX(5));
	GPIO_RegOneBitSet  (GPIO_B_PU, Z_IO_INDEX(5));
	#endif
    
    #if Custom_out2_EN
        _INIT_IO_Custom_out2;
    #endif
    
    #if Custom_out3_EN
        _INIT_IO_Custom_out3;
    #endif

	  #if LED_YELLOW_EN
        _INIT_IO_LED_YELLOW;
    #endif


    if(IO_KEY_10_EN==1)
    {
        _INIT_IO_KEY_10;
    }

    if(IO_KEY_11_EN==1)
    {
        _INIT_IO_KEY_11;
    }
    
    if(IO_KEY_12_EN==1)
    {
        _INIT_IO_KEY_12;
    }
    
    if(IO_KEY_13_EN==1)
    {
        _INIT_IO_KEY_13;
    }

	#if BT_MICA_CTRL_EN
        BT_MICA_POWER_init();
	#endif
    #if BT_MICB_CTRL_EN
        BT_MICB_POWER_init();
	#endif
    #if Echo_MIC_DET_EN
        _INIT_IO_MIC;        
    #endif
    #if BT_MIC_A_DET_EN
        _INIT_IO_BT_MIC_A;        
    #endif
	#if BT_MIC_B_DET_EN
        _INIT_IO_BT_MIC_B;        
    #endif
	#if BT_MIC_DET_EN
        _INIT_IO_BT_MIC;        
    #endif
	
    
    #if PH_DET_EN
        _INIT_PH_DET_pin;        
    #endif
    
    #if LEDS_mix_RGB_EN
        IO_contral_init(LEDS_RGB_R_IO_pin,0,0,0,!LEDS_RGB_R_OPEN);;        
        IO_contral_init(LEDS_RGB_G_IO_pin,0,0,0,!LEDS_RGB_G_OPEN);;        
        IO_contral_init(LEDS_RGB_B_IO_pin,0,0,0,!LEDS_RGB_B_OPEN);;    
		
           
    #endif
    #if RGB_Since_the_flash_EN
	  RGB_Since_the_flash_pin_init;
	#endif

	
    #ifdef DISP_DEV_AIP1668_LED
        IO_contral_init(AIP1668_STB_pin,0,1,0,1);       
	    IO_contral_init(AIP1668_CLK_pin,0,1,0,1); 
	    IO_contral_init(AIP1668_DAT_pin,0,1,0,1); 
    #endif
    #if Z_CFG_RES_CODE_KEY_USE &&  CODE_KEY_MODE_IO_SCAN
	 IO_contral_init(Encoder_A_pin,1,1,0,0);
	 IO_contral_init(Encoder_B_pin,1,1,0,0);
    #endif

    #if QN8027_EN
    // fmtx_init();
    qn8027_init(QN8027_TX_POWER_MAX,QN8027_pre1);
    //fmtx_set_state(RADIO_STATUS_IDLE);
	//extern u8 fmtx_power;
	//fmtx_power = 1;
	 #endif
	 
    #if KT0641_EN
	  KT0641_init();
	#endif
    #if BT_MIC_POWER_EN
     _INIT_IO_MIC_Pin_out1;
    #endif
	
	 #if PH_EN
          PH_EN_pin_init;
	  #endif 

	 #if POWER_LED_EN
         Power_LED1_IO_INIT();
	     Power_LED2_IO_INIT();
	 	 Power_LED3_IO_INIT();
	  #endif
	  #if CHARGE_EN
           Charge_IO_INIT();
	  #endif
	  #if Full_Voltage_Pin_EN
           Full_voltage_pin_init();
	  #endif

	  
    
}

u8 Scan_IO_key()
{
    u8 Key1_state=0xff;
    u8 Key2_state=0xff;
    u8 Key3_state=0xff;
    u8 Key4_state=0xff;
    u8 key_number=0xff;
    
    //u8 key_num = NO_KEY;
    //
    #if IO_KEY_10_EN
    Key1_state = IO_in_check(Pin_KEY_10_key);
    #endif

    #if IO_KEY_11_EN
    Key2_state = IO_in_check(Pin_KEY_11_key);
    #endif

    #if IO_KEY_12_EN
    Key3_state = IO_in_check(Pin_KEY_12_key);
    #endif
    
    #if IO_KEY_13_EN
    Key3_state = IO_in_check(Pin_KEY_13_key);
    #endif


    
    
    

    if(Key1_state==_Active_IO_KEY_10)
    {
        key_number = IOKey_num__1;
    }
    else if(Key2_state==_Active_IO_KEY_11)
    {
        key_number = IOKey_num__2;
    }
    else if(Key3_state==_Active_IO_KEY_12)
    {
        key_number = IOKey_num__3;
    }
    else if(Key4_state==_Active_IO_KEY_13)
    {
        key_number = IOKey_num__4;
    }
    
    

    #if 0
    static u8 de_x=0;
    de_x++;
    if(de_x%40==0)
    {
        DEBUG_ZX(0, "Key1_state=", Key1_state);
        DEBUG_ZX(0, "Key2_state=", Key2_state);
        DEBUG_ZX(0, "Key3_state=", Key3_state);
        DEBUG_ZX(0, "Key_state io=", key_number);
    }
    #endif
    
    return key_number;
}
u16 GET_AD_sampling(u16 Pin_io)  //GET_AD_sampling(Port_A0);
{
    u16 lv_sampling=0;

    switch(Pin_io)
    {
        case Port_A20:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX20);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA20_A23); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX20);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX23);
                break;
        case Port_A21:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX21);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA21_A24); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX21);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX24);
                break;
        case Port_A22:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX22);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA22_A25); 
               GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX22);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX25);
                break;
        case Port_A23:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX23);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA20_A23); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX23);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX20);
                break;
        case Port_A24:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX24);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA21_A24); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX21);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX24);
                break;
        case Port_A25:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX25);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA22_A25); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX22);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX25);
                break;
        case Port_A26:  
               // GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX25);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA26);
                break;
		case Port_A30:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX30);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA30); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX30);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX30);
                break;
		case Port_A31:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX31);
                lv_sampling = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA31); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX31);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX31);
                break;

    }
    return lv_sampling;
}
void IO_ANA_init(u16 Pin_io)  //IO_ANA_init(Port_A0);
{
    u16 Pin_io_H=0;
    u16 Pin_io_L=0;

    Pin_io_H = Pin_io & 0xff00;
    Pin_io_L = Pin_io & 0x00ff;

    switch(Pin_io_H)
    {
        case Port_A:  GPIO_RegOneBitSet(GPIO_A_ANA_EN, Z_IO_INDEX(Pin_io_L)); break;
        case Port_B:  GPIO_RegOneBitSet(GPIO_B_ANA_EN, Z_IO_INDEX(Pin_io_L)); break;

    }
    //GPIO_RegOneBitSet(CFG_RES_ADC_KEY_CH1_ANA_EN, CFG_RES_ADC_KEY_CH1_ANA_MASK);
    
}

//                   -引脚编号- -输入输出-  -上拉-      -下拉-       -输出值-
void IO_contral_init(u16 Pin_io,u8 IOdir,u8 IO_up_en,u8 IO_down_en,u8 Value_out)  //IO_contral_init(Port_A0,1,0,0,0);
{
    u16 Pin_io_H=0;
    u16 Pin_io_L=0;

    Pin_io_H = Pin_io & 0xff00;
    Pin_io_L = Pin_io & 0x00ff;


	
    
    switch(Pin_io_H)
    {
        case Port_A: if(IOdir==1)
                   {
                        GPIO_RegOneBitSet  (GPIO_A_IE, Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitClear(GPIO_A_OE, Z_IO_INDEX(Pin_io_L));
                   }

                   if(IO_up_en==0)  { GPIO_RegOneBitClear(GPIO_A_PU, Z_IO_INDEX(Pin_io_L)); }
                   if(IO_up_en==1)  { GPIO_RegOneBitSet  (GPIO_A_PU, Z_IO_INDEX(Pin_io_L)); }

                   if(IO_down_en==0){ GPIO_RegOneBitClear(GPIO_A_PD, Z_IO_INDEX(Pin_io_L)); }
                   if(IO_down_en==1){ GPIO_RegOneBitSet  (GPIO_A_PD, Z_IO_INDEX(Pin_io_L)); }

                   if((IOdir==0)&&(Value_out==1))
                   {
                        GPIO_RegOneBitClear(GPIO_A_IE , Z_IO_INDEX(Pin_io_L));
                        
                        GPIO_RegOneBitSet  (GPIO_A_OE , Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitSet  (GPIO_A_OUT, Z_IO_INDEX(Pin_io_L));
                        GPIO_PortAOutDsSet(Z_IO_INDEX(Pin_io_L), GPIO_PortA_OUTDS_8MA);
                   }
                   if((IOdir==0)&&(Value_out==0))
                   {
                        GPIO_RegOneBitClear(GPIO_A_OUT, Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitClear(GPIO_A_IE , Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitSet  (GPIO_A_OE , Z_IO_INDEX(Pin_io_L));
                        //GPIO_RegOneBitSet  (GPIO_A_OUT, Z_IO_INDEX(Pin_io_L));
                        
                   }
                   
                   break;

                   
        case Port_B: if(IOdir==1)
                   {
                        GPIO_RegOneBitSet  (GPIO_B_IE, Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitClear(GPIO_B_OE, Z_IO_INDEX(Pin_io_L));
                   }

                   if(IO_up_en==0)  { GPIO_RegOneBitClear(GPIO_B_PU, Z_IO_INDEX(Pin_io_L)); }
                   if(IO_up_en==1)  { GPIO_RegOneBitSet  (GPIO_B_PU, Z_IO_INDEX(Pin_io_L)); }

                   if(IO_down_en==0){ GPIO_RegOneBitClear(GPIO_B_PD, Z_IO_INDEX(Pin_io_L)); }
                   if(IO_down_en==1){ GPIO_RegOneBitSet  (GPIO_B_PD, Z_IO_INDEX(Pin_io_L)); }

                   if((IOdir==0)&&(Value_out==1))
                   {
                        GPIO_RegOneBitClear(GPIO_B_IE , Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitSet  (GPIO_B_OUT, Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitSet  (GPIO_B_OE , Z_IO_INDEX(Pin_io_L));
                        GPIO_PortBOutDsSet(Z_IO_INDEX(Pin_io_L), GPIO_PortB_OUTDS_8MA);
                   }
                   if((IOdir==0)&&(Value_out==0))
                   {
                        GPIO_RegOneBitClear(GPIO_B_OUT, Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitClear(GPIO_B_IE , Z_IO_INDEX(Pin_io_L));
                        GPIO_RegOneBitSet  (GPIO_B_OE , Z_IO_INDEX(Pin_io_L));
                   }

                   break;

        

    }
}


//********************************************************【功放控制方案】
void _send_L_wave(u16 lv_pin,u8 lv_num)
{
    u8 for_x;
    for(for_x=0;for_x<lv_num;for_x++)
    {
        IO_contral_init(lv_pin,0,0,0,0);
        //delay(800);
        IO_contral_init(lv_pin,0,0,0,1);
    }
}

void PA_contral_solution_1_x(u16 LV_pin_mute)
{
    if(PA_work_mode==PA_D_mute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    if(PA_work_mode==PA_D_unmute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
        DelayMs(50);
        IO_contral_init(LV_pin_mute,0,0,0,1);
        DelayMs(10);
        
        //irq_global_disable();
        _send_L_wave(LV_pin_mute,2);
        //irq_global_enable();
    }
    if(PA_work_mode==PA_AB_mute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    if(PA_work_mode==PA_AB_unmute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
        DelayMs(50);
        IO_contral_init(LV_pin_mute,0,0,0,1);
        DelayMs(10);
        
        //irq_global_disable();
        _send_L_wave(LV_pin_mute,4);
        //irq_global_enable();
    }
}
void PA_contral_solution_1_1(u16 LV_pin_mute)
{
    if((PA_work_mode==PA_D_mute)||(PA_work_mode==PA_AB_mute))
    {
        //IO_contral_out(LV_pin,0);
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    if(PA_work_mode==PA_D_unmute)
    {
     //   IO_contral_init(LV_pin_mute,0,0,0,1);
     
	 IO_contral_init(LV_pin_mute,0,0,0,1);
    }
	if (PA_work_mode==PA_AB_unmute) {
		IO_contral_init(LV_pin_mute,0,0,0,1);

	}
}

void PA_contral_solution_1_2(u16 LV_pin_mute)
{
    if((PA_work_mode==PA_D_mute)||(PA_work_mode==PA_AB_mute))
    {
        IO_contral_init(LV_pin_mute,0,0,0,1);
    }
    if((PA_work_mode==PA_D_unmute)||(PA_work_mode==PA_AB_unmute))
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
}
void PA_contral_solution_1_3(u16 LV_pin)
{
    if(PA_work_mode==PA_D_mute)
    {
        IO_contral_init(LV_pin,0,0,0,0);
    }
    if(PA_work_mode==PA_AB_mute)
    {
        IO_contral_init(LV_pin,0,0,0,0);
    }
    if(PA_work_mode==PA_D_unmute)
    {
        IO_contral_init(LV_pin,0,0,0,1);
    }
    if(PA_work_mode==PA_AB_unmute)
    {
        IO_contral_init(LV_pin,1,1,0,0);
    }
}
void PA_contral_solution_1_4(u16 LV_pin_mute)
{
    if(PA_work_mode==PA_D_mute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    else if(PA_work_mode==PA_D_unmute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,1);
    }
    else if(PA_work_mode==PA_AB_mute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    else if(PA_work_mode==PA_AB_unmute)
    {
        IO_contral_init(LV_pin_mute,1,1,0,0);
    }
}
void PA_contral_solution_1_5(u16 LV_pin_mute)
{
    if(PA_work_mode==PA_D_mute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    else if(PA_work_mode==PA_D_unmute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
        DelayMs(10);
        IO_contral_init(LV_pin_mute,0,0,0,1);
        DelayMs(20); 
        IO_contral_init(LV_pin_mute,0,0,0,0);
        DelayUs(70);//第一个低脉冲
        IO_contral_init(LV_pin_mute,0,0,0,1);
		DelayUs(70);//第一个高脉冲
		IO_contral_init(LV_pin_mute,0,0,0,0);
		DelayUs(70);//第二个低脉冲
		IO_contral_init(LV_pin_mute,0,0,0,1);
		DelayUs(70);//第二个高脉冲
		IO_contral_init(LV_pin_mute,0,0,0,0);
		DelayUs(70);//第三个低脉冲
		IO_contral_init(LV_pin_mute,0,0,0,1);
	
		
    }
    else if(PA_work_mode==PA_AB_mute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,0);
    }
    else if(PA_work_mode==PA_AB_unmute)
    {
        IO_contral_init(LV_pin_mute,0,0,0,1);
    }
}


void PA_contral_solution_2_1(u16 LV_pin_mute,u16 LV_pin_AB_D)
{
    if(PA_work_mode==PA_D_mute)
    {
        //IO_contral_out(LV_pin_mute,0);
        //IO_contral_out(LV_pin_AB_D,1);
        IO_contral_init(LV_pin_mute,0,0,0,0);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);

    }
    else if(PA_work_mode==PA_D_unmute)
    {
        //IO_contral_out(LV_pin_mute,1);
        //IO_contral_out(LV_pin_AB_D,1);
        IO_contral_init(LV_pin_mute,0,0,0,1);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);
    }
    else if(PA_work_mode==PA_AB_mute)
    {
        //IO_contral_out(LV_pin_mute,0);
        //IO_contral_out(LV_pin_AB_D,0);

        IO_contral_init(LV_pin_mute,0,0,0,0);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);
    }
    else if(PA_work_mode==PA_AB_unmute)
    {
        //IO_contral_out(LV_pin_mute,1);
        //IO_contral_out(LV_pin_AB_D,0);
        //IO_contral_init(LV_pin_mute,0,0,0,0);
        //DelayMs(25);
        IO_contral_init(LV_pin_AB_D,0,0,0,1);
        IO_contral_init(LV_pin_mute,1,0,0,0);
    }
}

void PA_contral_solution_2_2(u16 LV_pin_mute,u16 LV_pin_AB_D)
{
    if(PA_work_mode==PA_D_mute)
    {
        //IO_contral_out(LV_pin_mute,0);
        //IO_contral_out(LV_pin_AB_D,1);
        IO_contral_init(LV_pin_mute,0,0,0,0);
        IO_contral_init(LV_pin_AB_D,0,0,0,1);

    }
    else if(PA_work_mode==PA_D_unmute)
    {
        //IO_contral_out(LV_pin_mute,1);
        //IO_contral_out(LV_pin_AB_D,1);
        IO_contral_init(LV_pin_mute,0,0,0,1);
        IO_contral_init(LV_pin_AB_D,0,0,0,1);
    }
    else if(PA_work_mode==PA_AB_mute)
    {
        //IO_contral_out(LV_pin_mute,0);
        //IO_contral_out(LV_pin_AB_D,0);
        IO_contral_init(LV_pin_mute,0,0,0,0);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);
    }
    else if(PA_work_mode==PA_AB_unmute)
    {
        //IO_contral_out(LV_pin_mute,1);
        //IO_contral_out(LV_pin_AB_D,0);
        IO_contral_init(LV_pin_mute,0,0,0,0);
        DelayMs(25);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);
        IO_contral_init(LV_pin_mute,0,0,0,1);
    }
}


void PA_contral_solution_2_3(u16 LV_pin_mute,u16 LV_pin_AB_D)
{

   

    if(PA_work_mode==PA_AB_unmute)//AB模式
    {
        printf("**************  PA_AB_unmute *****************\n");
        IO_contral_init(LV_pin_mute,0,0,0,1);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);
    }
    else if(PA_work_mode==PA_D_unmute)//D模式
    {
       printf("**************  PA_D unmute *****************\n");
        IO_contral_init(LV_pin_mute,0,0,0,0);
        IO_contral_init(LV_pin_AB_D,0,0,0,1);
    }
    else if(PA_work_mode==PA_D_mute)//mute
    {
      printf("**************  PA_D_mute *****************\n");
        IO_contral_init(LV_pin_mute,0,0,0,0);
        IO_contral_init(LV_pin_AB_D,0,0,0,0);
    }
   
}

u16 key_num_fit(u16 num)
{
    u16 ret_num=0;
    static u16 save_num=0;
    static u8 save_count=0;

    if(save_num==num)
    {
        
        if(save_count<4)
        {
            save_count++;
        }
        else
        {
            ret_num = save_num;
        }
    }
    else
    {
        save_count=0;
        save_num=num;
    }
    return ret_num;
}
u16 get_powerkey_num()
{
    u16 adc_sampling=0;
    u16 key_number=0;
    u8 i=0;
   /* adc_sampling= ADC_SingleModeDataGet(ADC_CHANNEL_POWERKEY);
    //AdcChannelKeyGet(ADC_CHANNEL_POWERKEY);
    for(i = 0; i < sizeof(powerkey_adTab)/sizeof(powerkey_adTab[0]); i++)
	{
		if(adc_sampling < powerkey_adTab[i])
		{
		    key_number=i+1;
			break;
		}
	}*/


    if((Machine_state == Machine_run)||(Machine_state == Machine_FakePoweroff))
    {
        //if((Poweron_key_number>=Res_0)&&(Poweron_key_number<=Res_220))
        {
            if(key_number == 0)
            {
                Flag_key_clock = 0;
            }
        }
        if(Flag_key_clock==1)
        {
            key_number = 0;
        }
    }
    else
    {
        key_number = 0;
    }





	
	if(key_number==0)
	{
        return 0;
	}
	else
	{
        return (key_number+99);
	}
}
u16 KeyScan2()
{

    static u8 ___1s=0;
    
    u32 _IR_msg=0;
    u32 _IR_msg_con=0;
    u16 _key_msg=0;
    u16 key_num=0;
    u8 key_status=0;
    static u16 key_num_last=0;
    static u16 key_num_save=0;
    static u16 key_press_con=0;
    static u16 key_press_con_long=0;
    static u8 key_hold_press_con = 0;
    //IRKeyMsg IRKeyMsg;

    /**
    ___1s++;
    if(___1s<=100)
    {
        //___1s=0;
        IO_contral_init(P2_Pin_POWER_MOS,0,0,0,1);
        //DEBUG_ZX(0, "KeyScan  ", 0);
    }
    else if(___1s<=200)
    {
        IO_contral_init(P2_Pin_POWER_MOS,0,0,0,0);
    }
    else ___1s=0;
    */
    
    #if Z_IOKEY
        if(key_num==0)key_num = Scan_IO_key();
    #endif
    #if Z_pwoerkey_EN
        if(key_num==0)key_num = get_powerkey_num();
    #endif
    #if Remote_EN
        if(key_num==0)key_num = GetIRNecKeyIndex();
    #endif
    
    if(key_num!=0)DEBUG_ZX(0," key_num=",key_num);
    
    key_num = key_num_fit(key_num);

    //if(key_num!=0)DEBUG_ZX(0," key_num=",key_num);
    
    key_num_save = key_num_last;
    if(key_num==key_num_last)
    {
        if(key_num!=0)
        {
            key_press_con++;
            if (key_press_con == KEY_LONG_CNT) 
            {        
                key_status = _Key_long_1s;
            } 
            else if(key_press_con == (KEY_LONG_CNT + KEY_HOLD_CNT)) 
            {    
                key_status = _Key_hold___;
                key_press_con = KEY_LONG_CNT;
                
                if(key_hold_press_con<255)
                {
                    key_hold_press_con ++;
                }
                
                if(key_hold_press_con==KEY_LONG_2S_CNT)
                {
                    key_status = _Key_long_2s;
                }
                else if(key_hold_press_con==KEY_LONG_3S_CNT)
                {
                    key_status = _Key_long_3s;
                }
                else if(key_hold_press_con==KEY_LONG_4S_CNT)
                {
                    key_status = _Key_long_4s;
                }
                else if(key_hold_press_con==KEY_LONG_5S_CNT)
                {
                    key_status = _Key_long_5s;
                }
            } 
            else 
            {
                //return;
            }
        }
    }
    else
    { //cur_key = NO_KEY, 抬键
        key_num_last = key_num;
        key_hold_press_con=0;
		//key_press_flag=0;
        if(key_press_con < KEY_LONG_CNT) 
        {   //短按抬起
            key_status = _Key_click__;
        } 
        else if(key_press_con >= KEY_LONG_CNT) 
        { //长按抬起
            key_status = _Key_long_up;
            //puts("[UP]");
        } 
        key_press_con = 0;
    }


    
    /*
    if(_IR_msg!=0)
    {
        //DEBUG_ZX(0, "IRKeyMsg.index=", IRKeyMsg.index);
        DEBUG_ZX(0, "_IR_msg=", _IR_msg);
        //DEBUG_ZX(0, "_IR_msg_con=", _IR_msg_con);
    }*/
    
    if(key_status!=0)
    {
        //printf("op_state=%d  \r\n",op_state);
        //key_msg_send(key_num_save,key_status);
        _key_msg=key_operation_to_fun(key_num_save,key_status);
    }
    else
    {
        _key_msg = Special_msg_deal();
    }
    if(_key_msg!=0)
    {
        DEBUG_ZX(0, "_key_msg=", _key_msg);
    }

    return _key_msg;
}



u8 IO_in_check(u16 Pin_io)
{
    u16 Pin_io_H=0;
    u16 Pin_io_L=0;
    u8 io_in_state=0;

    Pin_io_H = Pin_io & 0xff00;
    Pin_io_L = Pin_io & 0x00ff;
    

    
    switch(Pin_io_H)
    {
        case Port_A: io_in_state = GPIO_RegOneBitGet(GPIO_A_IN, Z_IO_INDEX(Pin_io_L));   break;
        case Port_B: io_in_state = GPIO_RegOneBitGet(GPIO_B_IN, Z_IO_INDEX(Pin_io_L));   break;
    }
    //DEBUG_ZX(0, "io_in_state ", io_in_state);
    
    if(io_in_state>0)
    {
        return 1;
    }
    else return 0;
}









void X1_PT_deal()   //在2ms中断运行
{

#if X1_tx_EN
    X1_PT_send();
#endif

#if X1_rx_EN
    //if(X1_PT_inf.RX_en)
    X1_PT_receive();
#endif

}




void X1_PT_receive()    //接收消息模块
{
    //IO_contral_init(X1_rx_pin,1,0,0,0);

  
	  
    X1_PT_inf.RX_level = IO_in_check(X1_rx_pin);
    if(X1_PT_inf.RX_level)
    {
        if(X1_PT_inf.RX_H_level_flag==0)
        {
            X1_PT_inf.RX_H_level_flag=1;
            X1_PT_inf.RX_H__EDGE=1;
        }
        
        X1_PT_inf.RX_L_level_count=0;
        X1_PT_inf.RX_L_level_flag=0;
        //if()
        X1_PT_inf.RX_H_level_count++;
    }
    else
    {
        if(X1_PT_inf.RX_L_level_flag==0)
        {
            X1_PT_inf.RX_L_level_flag=1;
            X1_PT_inf.RX_L__EDGE=1;
        }
        X1_PT_inf.RX_L_level_count++;
        X1_PT_inf.RX_H_level_count=0;
        X1_PT_inf.RX_H_level_flag=0;
    }
    
    if(X1_PT_inf.state == X1_wait)
    {
        if(X1_PT_inf.RX_H_level_count==4)
        {
            X1_PT_inf.state = X1_idle;
            //DBG(" X1_PT_inf- X1_idle ", X1_PT_inf.state);
        }
    }
    
    if(X1_PT_inf.state == X1_idle)
    {
        if(X1_PT_inf.RX_L__EDGE)
        {
            X1_PT_inf.state = X1_start;
            //DEBUG_ZX(DEBUG_other, " X1_PT_inf- X1_start ", X1_PT_inf.state);
        }
    }
    if(X1_PT_inf.state == X1_start)
    {
        X1_PT_inf.RX_msg_property=X1_RX_pro_short;
        if(X1_PT_inf.RX_L__EDGE)
        {
            //X1_PT_inf.RX_L__EDGE=0;
            X1_PT_inf.RX_wave_num++;
        }
        if(X1_PT_inf.RX_L_level_count>12)
        {
            X1_PT_inf.state = X1_end_1;
            //DEBUG_ZX(DEBUG_other, " X1_PT_inf- X1_end_1 ", X1_PT_inf.state);
        }
    }
    if(X1_PT_inf.state == X1_end_1)
    {
        if(X1_PT_inf.RX_wave_num>0)
        {
            X1_PT_inf.RX_msg = X1_PT_inf.RX_wave_num-1;
            //if(X1_PT_inf.RX_msg==14)PH_online_state = 1;
            //X1_PT_inf.RX_wave_num=0;
            //DEBUG_ZX(DEBUG_other, " X1_PT_inf- RX_msg ", X1_PT_inf.RX_msg);
            //DBG("\r\n X1_PT_inf- RX_wave_num %d\r\n", X1_PT_inf.RX_wave_num);
        }
        else
        {
            //DEBUG_ZX(DEBUG_other, " X1_PT_inf- err ", 0);
        }
        
        
        if(X1_PT_inf.RX_L_level_count==500)
        {
            X1_PT_inf.RX_msg_property=X1_RX_pro_long;
            X1_PT_inf.ready=1;
        }
        if(X1_PT_inf.RX_L_level_count==1000)
        {
            X1_PT_inf.RX_msg_property=X1_RX_pro_hold;
            X1_PT_inf.RX_L_level_count=510;
            X1_PT_inf.ready=1;
        }
        if(X1_PT_inf.RX_H__EDGE)
        {
            X1_PT_inf.RX_H__EDGE=0;
            X1_PT_inf.state = X1_end_2;
            //DEBUG_ZX(DEBUG_other, " X1_PT_inf- X1_end_2 ", X1_PT_inf.state);
        }
        
    }
    if(X1_PT_inf.state == X1_end_2)
    {
        if(X1_PT_inf.RX_msg_property==X1_RX_pro_short)
        {/*
            if(X1_PT_inf.RX_msg==1)
            {
                if(X1_PT_inf.RX_d_msg==0)
                {
                    X1_PT_inf.RX_d_msg = X1_PT_inf.RX_msg;
                    X1_PT_inf.RX_msg=0;
                    
                    X1_PT_inf.RX_d_msg_con = 100;
                }
                else
                {
                    X1_PT_inf.RX_d_msg_con = 0;
                    X1_PT_inf.RX_d_msg = 0;
                    X1_PT_inf.RX_msg_property=X1_RX_pro_double;
                    X1_PT_inf.ready=1;
                    //DEBUG_ZX(DEBUG_other, " X1_RX_pro_double ", 2);
                }
            }
            else*/
            {
                X1_PT_inf.ready=1;
            }
        }
        X1_PT_inf.RX_wave_num=0;
        X1_PT_inf.state = X1_wait;

        //DEBUG_ZX(DEBUG_other, " X1_PT_inf- X1_idle ", X1_PT_inf.state);
    }
    if(X1_PT_inf.RX_d_msg_con>0)
    {
        X1_PT_inf.RX_d_msg_con--;
        if(X1_PT_inf.RX_d_msg_con==0)
        {
            X1_PT_inf.RX_msg=X1_PT_inf.RX_d_msg;
            X1_PT_inf.RX_msg_property=X1_RX_pro_short_d;
            X1_PT_inf.RX_d_msg=0;
            X1_PT_inf.ready=1;
        }
    }
    
    
    static u8 X1_last_state=0;
    if(X1_last_state!=X1_PT_inf.state)
    {
        X1_last_state=X1_PT_inf.state;
        //DEBUG_ZX(DEBUG_other, " X1_PT_inf state ", X1_PT_inf.state);
    }
    X1_PT_inf.RX_L__EDGE=0;
    X1_PT_inf.RX_H__EDGE=0;
    
}


void X1_send_msg(u8 LV_msg)
{
    static u8 Curr_send_num=0;
    if(X1_PT_inf.Get_msg_res)
    {
        X1_PT_inf.Get_msg_res=0;
        Curr_send_num=0;
    }
    if(LV_msg==0)return;
    if(X1_PT_inf.TX_msg_arr[Curr_send_num]==0)
    {
        X1_PT_inf.TX_msg_arr[Curr_send_num] = LV_msg;
        Curr_send_num++;
        if(Curr_send_num==10)
        {
            Curr_send_num=0;
        }
    }

}

void Get_X1_msg()
{
    static u8 Curr_get_num=0;
    if(X1_PT_inf.TX_msg_res==1)
    {
        X1_PT_inf.TX_msg_res=0;
        Curr_get_num=0;
        return;
    }
    if(X1_PT_inf.TX_msg_arr[Curr_get_num]!=0)
    {
        X1_PT_inf.TX_msg = X1_PT_inf.TX_msg_arr[Curr_get_num];
        X1_PT_inf.TX_msg_arr[Curr_get_num]=0;
        //DBG("\r\n Get_X1_msg \r\n", X1_PT_inf.TX_msg);
        Curr_get_num++;
        if(Curr_get_num==10)
        {
            Curr_get_num=0;
        }
    }
    
}

void X1_PT_send()       //发送消息模块
{
    if(X1_PT_inf.TX_msg==0)
    {
        Get_X1_msg();
        IO_contral_init(X1_tx_pin,0,0,0,1);
    }
    else
    {
        X1_PT_inf.TX_frame++;
        
        if(X1_PT_inf.TX_step==0)
        {
            X1_PT_inf.TX_state = 1;
            if(X1_PT_inf.TX_frame>=6)
            {
                X1_PT_inf.TX_frame=1;
                X1_PT_inf.TX_step++;
            }
            X1_PT_inf.TX_step++;
        }
        if(X1_PT_inf.TX_step==1)
        {
            //DBG("\r\n TX_step \r\n", X1_PT_inf.TX_step);
            if(X1_PT_inf.TX_frame>=X1_PT_inf.TX_frame_par)
            {
                X1_PT_inf.TX_frame=1;
                X1_PT_inf.TX_wave_num++;
            }
            if(X1_PT_inf.TX_wave_num==(X1_PT_inf.TX_msg*2))
            {
                X1_PT_inf.TX_wave_num=0;
                X1_PT_inf.TX_step++;
            }
            
            if((X1_PT_inf.TX_wave_num%2)==1)
            {
                X1_PT_inf.TX_state = 1;
            }
            else
            {
                X1_PT_inf.TX_state = 0;
            }
            
        }
        if(X1_PT_inf.TX_step==2)
        {
            //DEBUG_ZX(DEBUG_other, " TX_step ", X1_PT_inf.TX_step);
            X1_PT_inf.TX_state = 0;
            if(X1_PT_inf.TX_frame==X1_PT_inf.TX_end_frame_par)
            {
                X1_PT_inf.TX_frame=0;
                X1_PT_inf.TX_step++;
            }
        }
        if(X1_PT_inf.TX_step==3)
        {
            //DBG("X1_PT_send ---  TX_step \r\n", X1_PT_inf.TX_step);
            
            X1_PT_inf.TX_state = 1;
            
          /*  if(X1_PT_inf.TX_frame==50)
            {
                X1_PT_inf.TX_frame=0;
                X1_PT_inf.TX_step = 0;
                X1_PT_inf.TX_msg = 0;
            }*/
            X1_PT_inf.TX_frame=0;
            X1_PT_inf.TX_step = 0;
            X1_PT_inf.TX_msg = 0;
        }
        
        if(X1_PT_inf.TX_state)
        {
            IO_contral_init(X1_tx_pin,0,0,0,1);    
        }
        else
        {
            IO_contral_init(X1_tx_pin,0,0,0,0); 
        }
    }
}


u32 Special_msg_deal()
{
    u16 LV_msg=0;
    u8 LV_opt=0;
    u16 ZX_key_msg=0;
	

	
//DEBUG_ZX(0, " Special_msg_deal ", 0);
#if X1_protocol_EN

        if(X1_PT_inf.ready)
	    {
	        X1_PT_inf.ready=0;
	 
	        LV_msg = (u16)X1_PT_inf.RX_msg+X1_MSG1;
	        X1_PT_inf.RX_msg = 0;
	        switch(X1_PT_inf.RX_msg_property)
	        {
	            case X1_RX_pro_short:   LV_opt = _Key_click__;     break; //_Key_click__
	            case X1_RX_pro_long:    LV_opt = _Key_long_1s;     break; //_Key_long_1s
	            case X1_RX_pro_hold:    LV_opt = _Key_hold___;     break; //_Key_hold___
	            case X1_RX_pro_short_d: LV_opt = _Key_click_d;     break; //_Key_click_d
	            case X1_RX_pro_double:  LV_opt = _Key_double_;     break; //_Key_double_
	        }
	        X1_PT_inf.RX_msg_property = 0;
	    }
	    if(LV_msg!=0)
	    {
			DBG("X1_PT_inf   LV_msg == %d\n", LV_msg);
			DBG("X1_PT_inf   LV_opt == %d\n", LV_opt);
	        ZX_key_msg = key_operation_to_fun(LV_msg,LV_opt);
	    }
    

#if X1_rx2_EN
    if(X1_PT_inf2.ready)
    {
        X1_PT_inf2.ready=0;
 
        LV_msg = (u16)X1_PT_inf2.RX_msg+X1_MSG0;
        X1_PT_inf2.RX_msg = 0;
        switch(X1_PT_inf2.RX_msg_property)
        {
            case X1_RX_pro_short:   LV_opt = _Key_click__;     break; //_Key_click__
            case X1_RX_pro_long:    LV_opt = _Key_long_1s;     break; //_Key_long_1s
            case X1_RX_pro_hold:    LV_opt = _Key_hold___;     break; //_Key_hold___
            case X1_RX_pro_short_d: LV_opt = _Key_click_d;     break; //_Key_click_d
            case X1_RX_pro_double:  LV_opt = _Key_double_;     break; //_Key_double_
        }
        X1_PT_inf2.RX_msg_property = 0;


		if(LV_msg!=0)
	    {
			DBG("X1_PT_inf2   LV_msg == %d\n", LV_msg);
			DBG("X1_PT_inf2   LV_opt == %d\n", LV_opt);
	        ZX_key_msg = key_operation_to_fun(LV_msg,LV_opt);
	    }
		 
    }
   

  
  #endif

  
#endif
    return ZX_key_msg;
}




u16 key_operation_to_fun(u16 ZX_key_num,u8 ZX_key_operation)
{
    u8 for1x=0,for2x=0;
    u16 ZX_key_msg=0;

  //  DEBUG_ZX(DEBUG_other, "ZX_key_num=", ZX_key_num);
    
    /*
    if(ZX_key_operation!=0xff)
    {
        ZX_key_operation += 0xf0;
    }*/

    for(for1x;for1x<ZX_key_info_length;for1x++)
    {
        if(ZX_key_info[for1x].Key_name==ZX_key_num)
        {
            // for(for2x=0;for2x<4;for2x++)
			for(for2x=0;for2x<10;for2x++)//Z_key_long_ns_EN
            {
                if(ZX_key_info[for1x].Key_function[for2x][0]==ZX_key_operation)
                {
                    ZX_key_msg = Key_msg_ulteriorly_dispose(0,ZX_key_info[for1x].Key_function[for2x][1]);
                }
            }

        }
    }
   // DEBUG_ZX(0, "~~~ZX_key_msg=", ZX_key_msg);
   // DEBUG_ZX(0, " |Time_frame| ", Time_frame);


    return ZX_key_msg;
}
u16 Key_msg_ulteriorly_dispose(u8 lv_type,u8 ZX_key_msg)
{
    u16 ret_msg=0;
    u16 ret_msg2=0;
    u16 ret_msg3=0;
    u16 ret_msg_custom=NO_MSG;
    u8 Phone_state=0;
    
    static u8 test_dac_value=9;
    DEBUG_ZX(0, "ZX_key_msg=", ZX_key_msg);


	DBG("Machine_state : %d\r\n", Machine_state);
#if fun_idle_en == 0
    if(Machine_state!=Machine_run)
    {
        if((ZX_key_msg!=_Key_fun1_poweron)
            &&(ZX_key_msg!=_Key_fun1_IR_powerup)
            &&(ZX_key_msg!=_Key_fun2_powerdown_poweron)
            &&(ZX_key_msg!=_Key_fun2_IR_powerdown_poweron)
			&&(ZX_key_msg!=_Key_fun1_mode_idle)
            )
        {
			DBG("Machine_state : NO_MSG\r\n");
            //ZX_key_msg = NO_MSG;
			return MSG_NONE;
        }
    }

	
#endif

      #if fun_idle_en

	    //在待机模式,除了开机消息,其它所有按键消息不触发
       if(Idle_sw.idle_mode ==  on_line && ZX_key_msg != _Key_fun1_powerdown) 
       {
          printf("-----  KEY   idle mode  on line, return   -----\n");
		  return;
	   }
	   //播放开关语音之间不触发按键
       if(Idle_sw.power_on_tone == tone_start|| Idle_sw.power_off_tone == tone_start) 
       {
          if(Idle_sw.power_on_tone == tone_start)
          {
            printf("----- KEY  Idle_sw.power_on_tone == tone_start return   -----\n");
          }
		  else if(Idle_sw.power_off_tone == tone_start)
		  {
		  	printf("----- KEY  Idle_sw.power_off_tone == tone_start return   -----\n");
		  }
		  
		  return;
	   }
	  #endif
	  if(Machine_state!=Machine_run && Machine_state!=Machine_FakePoweroff)//加上此处,规避按键开机不松开机键又会关机
      {
	    DBG("-----KEY Machine_state!=Machine_run, return   -----\n");
           //ZX_key_msg = NO_MSG;
		return MSG_NONE;
        
      }
    
    if(ZX_key_msg>_Key_DEBUG_MIN)
    {
        ZX_key_msg = Split_DEBUG_function(ZX_key_msg);
    }
    

    if((ZX_key_msg>_Key_fun3_MIN)&&(ZX_key_msg<_Key_fun3_MAX))
    {
        ZX_key_msg = Split_3_function(ZX_key_msg);
    }
    if((ZX_key_msg>_Key_fun2_MIN)&&(ZX_key_msg<_Key_fun2_MAX))
    {
        ZX_key_msg = Split_2_function(ZX_key_msg);
    }




    
//--------------------------------------------------------------





    if(ZX_key_msg==_Key_fun1_3D_SW)
    {
        ret_msg = MSG_3D;
    }
	if(ZX_key_msg==_Key_fun1_bt_ctr)
	{
	    if(Tws_state)
	    {
           main_msg_send(MSG_BT_TWS_DISCONNECT);
		   main_msg_send(MSG_BT_TWS_CLEAR_PAIRED_LIST);
		}
		ret_msg = MSG_BT_CONNECT_CTRL;
	}
   if(ZX_key_msg==_Key_fun1_bt_dis)
	{
	    if(Tws_state)
	    {
          // main_msg_send(MSG_BT_TWS_DISCONNECT);
		  // main_msg_send(MSG_BT_TWS_CLEAR_PAIRED_LIST);
		}
		ret_msg = MSG_BT_DIS_CTRL;
	}


    if(ZX_key_msg==_Key_fun1_MIC_vol_up)
    {
        ret_msg = MSG_MIC_VOLUP;
    }
    if(ZX_key_msg==_Key_fun1_MIC_vol_down)
    {
        ret_msg = MSG_MIC_VOLDOWN;
    }
    if(ZX_key_msg==_Key_fun1_clean_voice)
    {
	      if(power_on_key_none==0)
	      {
	        ret_msg = MSG_VOCAL_CUT;
	      }
    }


    if(ZX_key_msg==_Key_fun1_DMA_RGB_SW)
    {
        ret_msg = MSG_DMA_RGB_SW;
    }
	if(ZX_key_msg==_Key_fun1_DMA_RGB_MODE_effect_SW)
    {
        ret_msg = MSG_RGB_MODE;
    }
	



    if(ZX_key_msg==_Key_fun1_RGB_SW)
    {
        //Z_post_msg(NULL,Custom_Event2_RGB_OFF,0);
        RGB_SW = !RGB_SW;
		 #if LEDS_mix_RGB_EN
	         if(RGB_SW==0)
	         {
                CLOSE_RGB_R_IO;
				CLOSE_RGB_G_IO;
				CLOSE_RGB_B_IO;
			 }
			 else
			 {
                OPEN_RGB_R_IO;
				OPEN_RGB_G_IO;
				OPEN_RGB_B_IO;
				
			 }
		#endif 
		#if RGB_Since_the_flash_EN
	         if(RGB_SW==0)
	         {
                 RGB_Since_the_flash_ON;
			 }
			 else
			 {
                RGB_Since_the_flash_OFF;
				
			 }
		#endif
    }
     
	if (ZX_key_msg == _Key_fun1_RGB_next )
	{
		
		if (RGB_curr_effect == RGB_Effect_breathe1){
			RGB_curr_effect = RGB_Effect_rhythm1;
		} else if (RGB_curr_effect == RGB_Effect_rhythm1){
			RGB_curr_effect = RGB_Effect_flowing_water;
		} else if (RGB_curr_effect == RGB_Effect_flowing_water){
			RGB_curr_effect = RGB_Effect_breathe1;
		}
		DBG("RGB_curr_effect : %d\n", RGB_curr_effect);
#ifdef BT_TWS_SUPPORT
		
		//	tws_led_rgb_send(RGB_curr_effect);
#endif

	}

    if(ZX_key_msg==_Key_fun1_IR_powerdown)//
    {
         DEBUG_ZX(0, "@@ _Key_fun1_IR_powerdown@@ ", 0);
        ret_msg3 = Custom_Event1_IR_Poweroff;
    }
	
    if(ZX_key_msg==_Key_fun1_powerdown)//
    {
        
		 #if fun_idle_en == 0	 
		 if(Machine_state==Machine_run)
	     {
	        DEBUG_ZX(0, "@@ _Key_fun1_powerdown@@ ", 0);
	        ret_msg3 = Custom_Event1_Poweroff;
		 }
		 #elif fun_idle_en && CHARGE_EN //开待机模式
		 /*
		    开机: 1.通常按键开机; 2.插充电进待机后再按键正式开机
		    关机: 1.通常按键关机; 2.插充电关机进待机 (在待机下,拔充电关MOS)
		 */
		 DBG("Idle_sw.idle_mode : %d\n", Idle_sw.idle_mode);
		 /// if(Idle_sw.idle_mode == on_line)//开机
		  if(GetSystemMode()==ModeIdle)
	       {
	          ret_msg3 = Custom_Event1_Poweron;
	        
	           /* RemindSoundItemRequestEnable();
               
			    Machine_state=Machine_poweron_tone;
			 	Flag_poweroff = 0;
				Idle_sw.power_on_tone = tone_start;
				//Idle_sw.idle_mode = off_line;
				AudioCoreSourceUnmute(APP_SOURCE_NUM, 1, 1);
				IO_contral_init(PA1_mute_pin,0,0,0,1);//先解mute
				PA_contral();
                ret_msg = MSG_QUIT_IDLE_MODE; */
							
	          
		   }
		   else// if(Idle_sw.idle_mode == off_line)//关机
		   {
	         //非待机下,按键关机,先播关机音
             ret_msg3 = Custom_Event1_Poweroff;
               /* Machine_state = Machine_Poweroff_tone;
				AudioCoreSourceMute(APP_SOURCE_NUM, 1, 1);	
                RemindSoundItemRequestEnable(); 
				PA_contral();
				ret_msg = MSG_ENTER_IDLE_MODE; */

		   }
		  #endif
    }

    if(ZX_key_msg==_Key_fun1_FM_sel_mode)
    {
        if(Fm_sel_mode==Fm_sel_next_station)
        {
        DEBUG_ZX(DEBUG_Fm, "@@ Fm_sel_mode==Fm_sel_next_step@@ ", 0);
            Fm_sel_mode = Fm_sel_next_step;
        }
        else
        {
            DEBUG_ZX(DEBUG_Fm, "@@ Fm_sel_mode==Fm_sel_next_station@@ ", 0);
            Fm_sel_mode = Fm_sel_next_station;
        }
        //if(Global_pause_flag==0)
       // {
       //     Z_post_msg(NULL,Custom_Event1_LED7_dis_FM_INF,0);
       // }
    }


    if(ZX_key_msg==_Key_fun1_fm_scan)
    {
		DBG("\nMSG_RADIO_PLAY_SCAN\n");
        ret_msg = MSG_RADIO_PLAY_SCAN;
    }

    if(ZX_key_msg==_Key_fun1_EFFECT)
    {
        ret_msg = MSG_EFFECTMODE;
    }
    if(ZX_key_msg==_Key_fun1_music_mute)
    {
       // ret_msg = MSG_MUTE;
        ret_msg = MSG_MUTE_TONE;
    }

	#if WIRELESS_CHAGER_CTRL_EN
	if(ZX_key_msg==_key_fun1_Vol_Set)
    {
        ret_msg = MSG_VOL_CTRL;
    }
	#endif
    if(ZX_key_msg==_Key_fun1_pp)
    {/*
        if((Task_state==Task_line)||(Task_state==Task_fm))
        {
            ret_msg2 = Custom_Event1_pp;
            //Custom_Event1_number = Custom_Event1_AUX1_pp;
            //ret_msg = Custom_Event1;
        }
        else */
        
//		X1_send_msg(_X1_SEND_MSG_LED_MODE_HDMI_ON);
//		X1_send_msg(_X1_SEND_MSG_LED_MODE_SPIDIF_ON);
//		X1_send_msg(_X1_SEND_MSG_LED_MODE_OPTICAL_ON);
//		X1_send_msg(_X1_SEND_MSG_LED_MODE_BT_ON);
//		X1_send_msg(_X1_SEND_MSG_LED_MODE_AUX_ON);
        ret_msg = MSG_PLAY_PAUSE;
    }
	
	if(ZX_key_msg==_Key_fun1_mode_and_moyin)
    {
        if(moyin_in_out_flag == moyin_out)//没进魔音,就切模式
        {
           ret_msg = MSG_MODE;
        }
		else  if(moyin_in_out_flag == moyin_in)
        {
           ret_msg = MSG_Music_EFFECT_SW;
        }
    }
	 
    if(ZX_key_msg==_Key_fun1_prev)
    {
        if(Task_state==Task_TwsSlave)
		{
            tws_send_key_msg(MSG_PRE);
		}
		else
		{
           ret_msg = MSG_PRE;
		}
    }
    if(ZX_key_msg==_Key_fun1_next)
    {
        if(Task_state==Task_TwsSlave)
		{
            tws_send_key_msg(MSG_NEXT);
		}
		else
		{
           ret_msg = MSG_NEXT;
		}
    }
    if(ZX_key_msg==_Key_fun1_volume_up)
    {
        ret_msg = MSG_MUSIC_VOLUP;
    }
    if(ZX_key_msg==_Key_fun1_volume_down)
    {
        ret_msg = MSG_MUSIC_VOLDOWN;
    }
	if(ZX_key_msg==_Key_fun1_sw_vol_up)
    {
        ret_msg = MSG_SW_VOL_UP;
    }
	if(ZX_key_msg==_Key_fun1_sw_vol_down)
    {
        ret_msg = MSG_SW_VOL_DOWN;
    }
    if(ZX_key_msg==_Key_fun1_mode)
    {
        ret_msg = MSG_MODE;
    }
    if(ZX_key_msg==_Key_fun1_EQ)
    {
        ret_msg = MSG_EQ;
    }

	if(ZX_key_msg==_Key_NULL_MSG)
    {
        ret_msg = MSG_KEY_NULL;
    } 

    if(ZX_key_msg==_Key_fun1_set_vol_sw)
    {
        ret_msg = MSG_VOL_MODE_SW;
    }
	 

	if (ZX_key_msg == _Key_fun1_call_back){
		ret_msg = MSG_BT_HF_REDAIL_LAST_NUM;
	}
	if (ZX_key_msg == _Key_fun1_call_reject){
		ret_msg = MSG_BT_HF_CALL_REJECT;
	}
	if(ZX_key_msg==_Key_fun1_music_mode)
	{
	    ret_msg = MSG_REPEAT;

	}

	if(ZX_key_msg==_Key_fun1_music_eq)
	{
	    ret_msg = MSG_Music_EQ_SW;

	}

	
	if(ZX_key_msg==_Key_fun1_QN8027_sw)
    {
        //ret_msg = Custom_Event2_QN8027_sw;
        ret_msg = Custom_Event1_FMTX;
    }

	
	#ifdef CFG_FUNC_RECORDER_EN
   	if(ZX_key_msg==_Key_fun1_rec)
		{
	    ret_msg = MSG_REC;

	}
	if(ZX_key_msg==_Key_fun1_rec_playback)
		{
	    ret_msg = MSG_REC_PLAYBACK;

	}
   if(ZX_key_msg==_Key_fun1_rec_filedel)
		{
	    ret_msg = MSG_REC_FILE_DEL;

	}
	#endif

	if (ZX_key_msg == _Key_fun1_TWS){
		if (Tws_state)
		{
			main_msg_send(MSG_BT_TWS_DISCONNECT);
			main_msg_send(MSG_BT_TWS_CLEAR_PAIRED_LIST);
			main_msg_send(MSG_BT_DIS_CTRL);
		} 
		else
		{
            if(btManager.btLinkState)
            {
		       main_msg_send(MSG_BT_DIS_CTRL);//先断开手机
            }
			ret_msg = MSG_BT_TWS_PAIRING;
		}
	}

	if (ZX_key_msg == _Key_fun1_bt_mode){
		ret_msg = MSG_MODE_BT;

	}
	if (ZX_key_msg == _Key_fun1_ctrl_light){
		yellow_light = !yellow_light;
		if (yellow_light){
			LED_YELLOW_OPEN;
		} else {
			LED_YELLOW_CLOSE;
		}
#ifdef BT_TWS_SUPPORT

	//tws_led_light_send(yellow_light);
#endif
	}

	if (ZX_key_msg == _Key_fun1_led_close){
		if (RGB_curr_effect != RGB_Effect_close){
			RGB_back_effect = RGB_curr_effect;
			RGB_curr_effect = RGB_Effect_close;
		} else {
			RGB_curr_effect = RGB_back_effect;
		}
#ifdef BT_TWS_SUPPORT
		
		//	tws_led_rgb_send(RGB_curr_effect);
#endif		
	}

    if(ZX_key_msg==_Key_fun1_Custom_out1_ctl)//
    {
        static u8 Custom_out1_x=0;
        Custom_out1_x = !Custom_out1_x;
        if(Custom_out1_x)
        {
            IO_contral_init(Pin_Custom_out1,0,0,0,!Custom_out1_Default_state);
        }
        else
        {
            IO_contral_init(Pin_Custom_out1,0,0,0,Custom_out1_Default_state);
        }
        //DEBUG_ZX(0, "!_Key_fun1_Custom_out1_ctl!!!!!!!!!!!!", 0);
    }
    if(ZX_key_msg==_Key_fun1_Custom_out2_ctl)//
    {
        static u8 Custom_out2_x=0;
        Custom_out2_x = !Custom_out2_x;
        if(Custom_out2_x)
        {
            IO_contral_init(Pin_Custom_out2,0,0,0,!Custom_out2_Default_state);
        }
        else
        {
            IO_contral_init(Pin_Custom_out2,0,0,0,Custom_out2_Default_state);
        }
    }
    if(ZX_key_msg==_Key_fun1_Custom_out3_ctl)//
    {
        static u8 Custom_out3_x=0;
        Custom_out3_x = !Custom_out3_x;
        if(Custom_out3_x)
        {
            IO_contral_init(Pin_Custom_out3,0,0,0,!Custom_out3_Default_state);
        }
        else
        {
            IO_contral_init(Pin_Custom_out3,0,0,0,Custom_out3_Default_state);
        }
    }

    switch(ZX_key_msg)
    {
        case _Key_fun1_BTMIC_Power_sw   : ret_msg3 = Custom_Event2_BTMIC_sw;           break;
        case _Key_fun1_KT0641_sw        : ret_msg3 = Custom_Event2_KT0641_sw;           break;
        case _Key_fun1_KT0641_mute      : ret_msg3 = Custom_Event2_KT0641_mute;         break;
        case _Key_fun1_KT0641_vol_up    : ret_msg3 = Custom_Event2_KT0641_vol_up;       break;
        case _Key_fun1_KT0641_vol_down  : ret_msg3 = Custom_Event2_KT0641_vol_down;     break;
    }
// 场景 或音效切换
    switch(ZX_key_msg)
    {
        case _Key_fun1_scene_up          : ret_msg = MSG_Scene_up;           break;
        case _Key_fun1_scene_down        : ret_msg = MSG_Scene_down;        break;
        case _Key_fun1_sound_effect_one  : ret_msg3 = Custom_Event2_sound_effect_one;      break;
        case _Key_fun1_sound_effect_two  : ret_msg3 = Custom_Event2_sound_effect_two ;   break;
		case _Key_fun1_scene_ktv  :        ret_msg= MSG_EFFECT_KTV;       break;
        case _Key_fun1_scene_country  :    ret_msg = MSG_EFFECT_Country ;   break;
	    case _Key_fun1_scene_recordingroom  : ret_msg =MSG_EFFECT_RecordingRoom ;	break;
		case _Key_fun1_scene_micba  : ret_msg = MSG_EFFECT_MicBa ;	break;
		case _Key_fun1_scene_zhuanye  : ret_msg = MSG_EFFECT_ZhuanYe ;	break;
		case _Key_fun1_scene_zhuchi  : ret_msg = MSG_EFFECT_ZhuChi ;	break;
		case _Key_fun1_effect_SW  : 
	   		if (power_on_key_none == 0){
				ret_msg =MSG_Music_EFFECT_SW ;	
			}
			break;
	}
// 提示音
	switch(ZX_key_msg)
	 {
	    case _Key_fun1_tone_wuya: 				ret_msg3 = Custom_Event1_tone_wuya;          break;
        case _Key_fun1_tone_zhangsheng: 		ret_msg3 = Custom_Event1_tone_zhangsheng;    break;
        case _Key_fun1_tone_bishi: 				ret_msg3 = Custom_Event1_tone_bishi;         break;
        case _Key_fun1_tone_huanhu : 		    ret_msg3 = Custom_Event1_tone_huanhu ;      break;
		case _Key_fun1_tone_ganga: 				ret_msg3 = Custom_Event1_tone_ganga;        break;
        case _Key_fun1_tone_qinw: 		        ret_msg3 = Custom_Event1_tone_qinw;         break;
		case _Key_fun1_tone_memeda: 		    ret_msg3 = Custom_Event1_tone_memeda;         break;
        case _Key_fun1_tone_hongxiao: 		    ret_msg3 = Custom_Event1_tone_hongxiao;    	break;
        case _Key_fun1_tone_liqu : 		        ret_msg3 = Custom_Event1_tone_liqu ;   		break;
		case _Key_fun1_tone_max : 		        ret_msg3 = Custom_Event1_max_vol ;   		break;
        case _Key_fun1_tone_min : 		        ret_msg3 = Custom_Event1_min_vol ;   		break;

	}

    // mic 部分控制
   switch(ZX_key_msg)
	 {
	case _Key_fun1_MIC_ECHO_DOWN:	ret_msg = MSG_MIC_EchoReverb_DW;      break;
	case _Key_fun1_MIC_ECHO_UP:	    ret_msg = MSG_MIC_EchoReverb_UP; 		 break;
	case _Key_fun1_MIC_SW:		    ret_msg = MSG_MIC1_CTRL_EN; 		     break;
	case _Key_fun1_PAM_SW:          ret_msg = MSG_PAM_CTRL_EN; 		     break;

	case _Key_fun1_LINE3_MIC_L_SW:        ret_msg = MSG_LINE3_L_SW;     break;
	case _Key_fun1_LINE3_MIC_R_SW:        ret_msg = MSG_LINE3_R_SW;     break;
	 
   	}
   // 魔音控制
   
	switch(ZX_key_msg)
	  {
	case _Key_fun1_moyin_ctrl: 		 ret_msg = MSG_EFFECT_MOYIN_CTRL;	      break;
	case _Key_fun1_bianyin: 		 ret_msg = MSG_EFFECT_BIANYIN;			  break;
	case _Key_fun1_effect_moyin: 	 ret_msg = MSG_EFFECT_MOYIN;			  break;
	case _Key_fun1_moyin_in_out : 	 ret_msg = MSG_MOYIN_IN_OUT;			  break;
	case _Key_fun1_moyin_on_effect_SW : 	 ret_msg = MSG_MOYIN_IN_EFFECT_SW;	  break;
	 }

    switch(ZX_key_msg)
    {
        case _Key_fun1_pp:  if(Task_state==Task_line && Tws_Slave_state == 0)
		                    {
								ret_msg2 = Custom_Event1_pp;   
							}  
		                    else if(Task_state==Task_line && Tws_Slave_state == 1)
							{
								ret_msg = MSG_PLAY_PAUSE;
							}  
							if(Task_state==Task_TwsSlave)
							{
                                tws_send_key_msg(MSG_PLAY_PAUSE);
							}
                            if(Task_state==Task_fm)         ret_msg2 = Custom_Event1_pp;  
                            if(Task_state==Task_HDMI_in)    ret_msg2 = Custom_Event1_pp;   
                            break;
        case _Key_fun1_prev:    
                                if(Task_state==Task_fm) 
                                {
                                    if(Fm_sel_mode==Fm_sel_next_station ) ret_msg = MSG_RADIO_PLAY_PRE;
                                    if(Fm_sel_mode==Fm_sel_next_step    ) ret_msg = MSG_FF_START;
                                }

                                break;

        case _Key_fun1_next:    
                                if(Task_state==Task_fm              ) 
                                {
                                    if(Fm_sel_mode==Fm_sel_next_station ) ret_msg = MSG_RADIO_PLAY_NEXT;
                                    if(Fm_sel_mode==Fm_sel_next_step    ) ret_msg = MSG_FB_START;
                                }
								

                                break;

    }




            //ret_msg = Custom_Event1;
    

    if(ret_msg3!=0)
    {
        DEBUG_ZX(DEBUG_Msg3, "ret_msg3=", ret_msg3);
        Custom_Event1_number = ret_msg3;
        return Custom_Event2;
    }
    else if(ret_msg2!=0)
    {
        DEBUG_ZX(DEBUG_Msg2, "ret_msg2=", ret_msg2);
        Custom_Event1_number = ret_msg2;
        return Custom_Event1;
    }
    else
    {
        DEBUG_ZX(DEBUG_Msg1, "ret_msg=", ret_msg);
        return ret_msg;
    }
    
}
u8 Split_DEBUG_function(u8 ZX_key_msg)
{
    static u8 Flag_9=0;
    static u8 flag_change=0;
    
   // DEBUG_ZX(0, " _Key_custom_x1 ", ZX_key_msg);
    
    if(ZX_key_msg==_Key_custom_x1)
    {
        DEBUG_ZX(0, " _Key_custom_x1 ", _Key_custom_x1);        
    }
    if(ZX_key_msg==_Key_custom_x2)
    {
        DEBUG_ZX(0, " _Key_custom_x2 ", _Key_custom_x2);        
    }
    if(ZX_key_msg==_Key_custom_x3)
    {
        DEBUG_ZX(0, " _Key_custom_x3 ", _Key_custom_x3);        
    }
    if(ZX_key_msg==_Key_custom_x4)
    {
        DEBUG_ZX(0, " _Key_custom_x4 ", _Key_custom_x4);        
    }
    if(ZX_key_msg==_Key_custom_x5)
    {
        DEBUG_ZX(0, " _Key_custom_x5 ", _Key_custom_x5);        
    }
    if(ZX_key_msg==_Key_custom_x6)
    {
        DEBUG_ZX(0, " _Key_custom_x6 ", _Key_custom_x6);        
    }
    
    return ZX_key_msg;
}


extern TIMER   DispMicVolTmr;

u8 Split_3_function(u8 ZX_key_msg)
{

     if(ZX_key_msg == _Key_fun3_sw_vol_up)
     {
         if(Vol_Sw_Flag == Set_Mic_Vol)
         {
            // ZX_key_msg = _Key_fun1_MIC_vol_up; //MIC音量是调节蓝牙MIC端的 就注释    
            ZX_key_msg = 0;//MIC音量是调节蓝牙MIC端的就清除消息
	     } 
		 else if(Vol_Sw_Flag == Set_Reverb_Vol)
	     {
	         ZX_key_msg = _Key_fun1_MIC_ECHO_UP;     
		 }
		 else if(Vol_Sw_Flag == Set_Music_Vol)
	     {
	         ZX_key_msg = _Key_fun1_volume_up;     
		 }

		 #if Set_MIC_Tmr_EN  
		     SetMicTmrFlag = 1;
			 TimeOutSet(&DispMicVolTmr, 20000); 
		 #endif	
	 }
	 
	 if(ZX_key_msg == _Key_fun3_sw_vol_down)
     {
         if(Vol_Sw_Flag == Set_Mic_Vol)
         {
             //ZX_key_msg = _Key_fun1_MIC_vol_down; //MIC音量是调节蓝牙MIC端的 就注释
              ZX_key_msg = 0;//MIC音量是调节蓝牙MIC端的就清除消息
	     } 
		 else if(Vol_Sw_Flag == Set_Reverb_Vol)
	     {
	         ZX_key_msg = _Key_fun1_MIC_ECHO_DOWN;     
		 }
		 else if(Vol_Sw_Flag == Set_Music_Vol)
	     {
	         ZX_key_msg = _Key_fun1_volume_down;     
		 }

		  #if Set_MIC_Tmr_EN  
		     SetMicTmrFlag = 1;
			 TimeOutSet(&DispMicVolTmr, 20000); 
		 #endif	
		 
	 }

		 
    return ZX_key_msg;
}
u8 Split_2_function(u8 ZX_key_msg)
{

    if(ZX_key_msg==_Key_fun2_powerdown_poweron)
    {
    
        if(Machine_state==Machine_FakePoweroff)
        {
            //DEBUG_ZX(DEBUG_other, "--MSG-- _Key_fun1_poweron", 1);
            ZX_key_msg = _Key_fun1_poweron;
        }
        if(Machine_state==Machine_run)
        {
            DEBUG_ZX(0, "---- _Key_fun1_powerdown----", 1);
            ZX_key_msg = _Key_fun1_powerdown;
        }
    }
    if(ZX_key_msg==_Key_fun2_IR_powerdown_poweron)
    {
        if(Machine_state==Machine_FakePoweroff)
        {
            ZX_key_msg = _Key_fun1_IR_powerup;
        }
        if(Machine_state==Machine_run)
        {
            ZX_key_msg = _Key_fun1_IR_powerdown;
        }
    }


	if (ZX_key_msg == _Key_fun2_bt_ctr_hangup){
		if (BT_state == BT_HFP_CALL_IN ||
			BT_state == BT_HFP_CALL_OUT ||
			BT_state == BT_HFP_CALL_ACTIVE ||
			BT_state == BT_HFP_CALL_SUSPEND){
			ZX_key_msg = _Key_fun1_call_reject;
		} else {
			ZX_key_msg = _Key_fun1_bt_ctr;
			
		}
	}

	if (ZX_key_msg == _Key_fun2_pp_fm_scan){
		if (Task_state == Task_fm){
			ZX_key_msg = _Key_fun1_fm_scan;
		} else {
			ZX_key_msg = _Key_fun1_pp;
		}
	}

    if (ZX_key_msg == _Key_fun2_clean_voice_and_moyin)
		{
			if (moyin_in_out_flag == moyin_out)
			{
				ZX_key_msg = _Key_fun1_clean_voice;
			}
			else 
			{
				ZX_key_msg = _Key_fun1_effect_SW;
			}
		}
	
     if (ZX_key_msg == _Key_fun2_TWS_bt_dis)
		{
		   if((GetBtManager()->twsRole == BT_TWS_MASTER)&&(GetBtManager()->twsState == BT_TWS_STATE_CONNECTED))
			{
			   //if(btManager.btLinkState)
			   {
				   ZX_key_msg = _Key_fun1_bt_ctr;
			   }
			}
			else if((GetBtManager()->twsRole == BT_TWS_SLAVE)&&(GetBtManager()->twsState == BT_TWS_STATE_CONNECTED))
			{
				ZX_key_msg = _Key_fun1_TWS;
			}
			else
			{
               //if(btManager.btLinkState)
			   {
				   ZX_key_msg = _Key_fun1_bt_ctr;
			   }
			}
		}


    return ZX_key_msg;
}

u8 Get_iis_playing_state()
{
    u8 LV_play_state=0;
	if(T_IIS_in_inf.play_state		==_Music_play)LV_play_state=1;
			return LV_play_state;

}
u8 Get_rec_playing_state()
{
    u8 LV_play_state=_Rec_stop;
	if(T_rec_inf.play_state	==_Rec_dealing)LV_play_state=_Rec_dealing;
	else if(T_rec_inf.play_state==_Rec_playback)LV_play_state=_Rec_playback;
	else 
		LV_play_state=_Rec_stop;
	return LV_play_state;

}

u8 Get_Global_playing_state()
{
    u8 LV_play_state=0;

	//DBG("Task_state : %d\n", Task_state);
	//DBG("T_sd0_inf.play_state : %d\n", T_sd0_inf.play_state);
	 if(Task_state==Task_bt)	
	   {
	   if(T_bt_inf.play_state      ==_Music_play)LV_play_state=1;
	      return LV_play_state;
	}
    if(Task_state==Task_TwsSlave)	
	   {
	   if(T_TwsSlave_inf.play_state      ==_Music_play)LV_play_state=1;
	      return LV_play_state;
	}
	  
	if(Task_state==Task_sd_for_sd0) 
	   {
		if(T_sd0_inf.play_state 	==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_sd_for_sd1) 
	   {
		if(T_sd1_inf.play_state 	==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_sd_for_usb) 
	   {
		if(T_usb_inf.play_state 	==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}	 
	if(Task_state==Task_pc) 
	   {
		if(T_pc_inf.play_state 	==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_fm) 
	   {
		if(T_FM_inf.play_state	   ==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_line) 
	   {
		if(T_linein1_inf.play_state	   ==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_OPTICAL) 
	   {
		if(T_OPTICAL_inf.play_state	   ==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_COAXIAL) 
	   {
		if(T_COAXIAL_inf.play_state	   ==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}

  	if(Task_state==Task_HDMI_in) 
	   {
		if(T_HDMI_in_inf.play_state	   ==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
    if(Task_state==Task_IIS_in) 
	   {
		if(T_IIS_in_inf.play_state	   ==_Music_play)LV_play_state=1;
		 return LV_play_state;
	}
    
  //  if(T_rec_inf.play_state     ==_Music_play)LV_play_state=1;
  


    return LV_play_state;
}

u8 Get_Global_pause_state()
{
    u8 LV_play_state=0;
	 if(Task_state==Task_bt)	
	   {
	   if(T_bt_inf.play_state      ==_Music_puse)LV_play_state=1;
	      return LV_play_state;
	}
	if(Task_state==Task_sd_for_sd0) 
	   {
		if(T_sd0_inf.play_state 	==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_sd_for_sd1) 
	   {
		if(T_sd1_inf.play_state 	==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_sd_for_usb) 
	   {
		if(T_usb_inf.play_state 	==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}	 
	if(Task_state==Task_pc) 
	   {
		if(T_pc_inf.play_state 	==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_fm) 
	   {
		if(T_FM_inf.play_state	   ==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_line) 
	   {
		if(T_linein1_inf.play_state	   ==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_OPTICAL) 
	   {
		if(T_OPTICAL_inf.play_state	   ==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
	if(Task_state==Task_COAXIAL) 
	   {
		if(T_COAXIAL_inf.play_state	   ==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}

  	if(Task_state==Task_HDMI_in) 
	   {
		if(T_HDMI_in_inf.play_state	   ==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}
    if(Task_state==Task_IIS_in) 
	   {
		if(T_IIS_in_inf.play_state	   ==_Music_puse)LV_play_state=1;
		 return LV_play_state;
	}




    return LV_play_state;
}

void PA_processing_event()
{
    
    Global_playing_flag = Get_Global_playing_state();
    Global_pause_flag   = Get_Global_pause_state();
    Global_rec_playing_flag=Get_rec_playing_state();
    if(Global_playing_flag==1||Global_rec_playing_flag==_Rec_dealing||Global_rec_playing_flag==_Rec_playback)
    {
        Global_PA_work = PA_on;
        DBG("PA_contral()PA_on  Global_playing_flag==1\n");
    }
    else
    {
        DBG("PA_contral()PA_off  Global_playing_flag==0\n");
        Global_PA_work = PA_off;
    }
/*
    if(sound.vol.sys_vol_l==0)
    {
        Global_PA_work = PA_off;
    }*/

    #if 0//(PH_DET_EN == 0)&&(PA1_EN == 1)//普通音箱

		//TWS状态,主从机同步mute、unmtue; 只同步播放\暂停状态的 mute、unmtue
		if(Tws_state)
	    {
	       tws_master_pa_control_send(Global_PA_work);//主发送

	       //if(GetSystemMode() == AppModeTwsSlavePlay)
		   if(GetBtManager()->twsRole == BT_TWS_SLAVE) //从同步
		   {
	          if(Tws_Slave_PA_state)
	          {
	             printf("Tws_Slave_PA_state  == PA_on\n");
	              Global_PA_work = PA_on;   
			  }
			  else
			  {
			       printf("Tws_Slave_PA_state  == PA_off\n");
	               Global_PA_work = PA_off;  
			  }
		   }
		}
	 
   	
	#endif

	
	
	if(phone_state==1)
	{
		Global_PA_work = PA_on;
		DBG("PA_contral()PA_on  hone_state==1\n");
	}

    if(Tone_play_state==1)
    {
        Global_PA_work = PA_on;
         DBG("PA_contral()PA_on  Tone_play_state==1\n");
    }
    /*
    if(VOl_didi_continuous==1)
    {
        Global_PA_work = PA_on;
    }*/
    

	
    if(Com_mute_contral)
    {
        Global_PA_work = PA_off;
        DBG("PA_contral()PA_off  Com_mute_contral==1\n");
    }
/*
    if(Clean_tone_zizi)
    {
        Global_PA_work = PA_off;
    }*/
    
    
    if(KTV_MIC_state)
    {
        Global_PA_work = PA_on;
       DBG("PA_contral()PA_on  KTV_MIC_state\n");
    }
	
    #if 0//Built_in_echo_EN == 0
        Global_PA_work = PA_on;//做音箱常开
	#endif
	
	if(PAM_ctrl_en == 2)
    {
        Global_PA_work = PA_off;
		DBG("PA_contral()PA_off  PAM_ctrl_en == 2\n");
    }
	

	if (Tws_play_state){
     //   Global_PA_work = PA_on;

	}


    if((Machine_state==Machine_Poweroff)||(Machine_state==Machine_init))
    {
        Global_PA_work = PA_off;
        DBG("PA_contral()PA_off  Machine_state == Poweroff\init\n");
    }
   
    if(PA_keep_on)
    {
        Global_PA_work = PA_on;
        DBG("PA_contral()PA_on  PA_keep_on\n");
    }
    if(PA_keep_off)
    {
        Global_PA_work = PA_off;
       DBG("PA_contral()PA_off  PA_keep_off\n");
    }
    
    if(update_ok)
    {
        Global_PA_work = PA_on;
		DBG("PA_contral()PA_on  update_ok\n");
    }
	
	if(PH_online_state)
    {
        Global_PA_work = PA_off;
        DBG("PA_contral()PA_off  PH_online_state\n");
    }
	 

    if(Machine_state == Machine_Poweroff)
    {
        Global_PA_work = PA_off;
        DBG("PA_contral()PA_off  Machine_state == Machine_Poweroff\n");
    }

#if fun_idle_en
       if(Idle_sw.idle_mode == on_line && update_ok==0&&Tone_play_state==0)
       {
           Global_PA_work = PA_off;
		   DBG("PA_contral()PA_off  Idle_sw.idle_mode == on_line \n");
	   }
	   /*if(Machine_state == Machine_FakePoweroff&&update_ok==0)
       {
           Global_PA_work = PA_off;
		   DBG("PA_contral()PA_off  Machine_FakePoweroff\n");
       }*/
	      
   
#endif

    if(power_down_zx_flag)
    {
		 Global_PA_work = PA_off;
		 DBG("PA_contral()PA_off  power_down_zx_flag\n");
	}

    
}

void PA_contral()
{
    PA_processing_event();

    if(Global_PA_work==PA_on)
    {
        if(T_FM_inf.play_state== _Music_play)
        {
            PA_work_mode = PA_AB_unmute;
        }
        else PA_work_mode = PA_D_unmute;
    }
    else      
    {
        if(T_FM_inf.play_state== _Music_puse)
        {
            PA_work_mode = PA_AB_mute;
        }
        else PA_work_mode = PA_D_mute;
    }
    //DEBUG_ZX(0, "PA_contral++  PA_work_mode=", PA_work_mode);
    if(Last_PA_workmode!=PA_work_mode)
    {
        Last_PA_workmode = PA_work_mode;
    }
    else
    {
        return;
    }
    DEBUG_ZX(0, "++  PA_work_mode=", PA_work_mode);
    
#if PA1_EN
    switch(PA1_type)
    {
        case PA_type_1_x: PA_contral_solution_1_x(PA1_mute_pin);                    break;
        case PA_type_1_1: PA_contral_solution_1_1(PA1_mute_pin);                    break;
        case PA_type_1_2: PA_contral_solution_1_2(PA1_mute_pin);                    break;
        case PA_type_1_3: PA_contral_solution_1_3(PA1_mute_pin);                    break;
        case PA_type_1_4: PA_contral_solution_1_4(PA1_mute_pin);                    break;
        case PA_type_1_5: PA_contral_solution_1_5(PA1_mute_pin);                    break;
        case PA_type_2_1: PA_contral_solution_2_1(PA1_mute_pin,PA1_AB_D_pin);       break;
        case PA_type_2_2: PA_contral_solution_2_2(PA1_mute_pin,PA1_AB_D_pin);       break;
		case PA_type_2_3: PA_contral_solution_2_3(PA1_mute_pin,PA1_AB_D_pin);       break;



        default:break;
    }
#endif

#if PA2_EN
    switch(PA2_type)
    {
        case PA_type_1_1: PA_contral_solution_1_1(PA2_mute_pin);                    break;
        //case PA_type_1_2: PA_contral_solution_1_2(PA2_mute_pin);                    break;
        //case PA_type_2_1: PA_contral_solution_2_1(PA2_mute_pin,PA2_AB_D_pin);       break;


        default:break;
    }
#endif

}











LED_indicator LED_1_inf=
{
    0,
    Pin_LED1,
    0,
    0,
    LED_ON,
    LED1_ON_E,
    0,
};


LED_indicator LED_2_inf=
{
    0,
    Pin_LED2,
    0,
    0,
    LED_ON,
    LED2_ON_E,
    0,
};

LED_indicator LED_3_inf=
{
    0,
    Pin_LED3,
    0,
    0,
    LED_OFF,
    LED3_ON_E,
    0,
};

LED_indicator LED_4_inf=
{
    0,
    Pin_LED4,
    0,
    0,
    LED_OFF,
    LED4_ON_E,
    0,
};

static u8 State_LED1_plan=0;
static u8 State_LED2_plan=0;
void Update_led_play(u16 Lv_play)
{
    if((Lv_play&0xf000)==iLED1)
    {
        LED_1_inf.plan = Lv_play & 0x00ff;
    }

#if LED2_EN
    if((Lv_play&0xf000)==iLED2)
    {
        LED_2_inf.plan = Lv_play & 0x00ff;
    }
#endif	

#if LED3_EN
    if((Lv_play&0xf000)==iLED3)
    {
        LED_3_inf.plan = Lv_play & 0x00ff;
    }
#endif	


#if LED4_EN
		if((Lv_play&0xf000)==iLED4)
		{
			LED_4_inf.plan = Lv_play & 0x00ff;
		}
#endif



    
}

void LED_processing_event()
{
    //u8 State_LED_plan_tar=0;
    static u8 LED_onoff=0;
	static u8 first_connect = 0;
    /*
    if(Task_state==Task_NULL)
    {
        State_LED_plan_tar = LED_ALL_off;
    }*/
    if(Press_Key_cnt>0)
      {
        Press_Key_cnt--;
	    Update_led_play(Z_led4_press_key);
    	}
	else
	    Update_led_play(Z_led4_Nopress_key);

	
    Global_playing_flag = Get_Global_playing_state();
    Global_pause_flag   = Get_Global_pause_state();
    if((Machine_state==Machine_poweron_tone)||(Machine_state==Machine_run)||Machine_state==Machine_init)
	{
        //State_LED_plan_tar = LED1_on;
        //if(LED_onoff==0)
        {
            LED_onoff = 1;
            LED_1_inf.sw=1;
            #if LED2_EN
            LED_2_inf.sw=1;
			Update_led_play(Z_led2_poweron);
            #endif
			#if LED3_EN
            LED_3_inf.sw=1;
            #endif
			#if LED4_EN
            LED_4_inf.sw=1;
            #endif
            Update_led_play(Z_led_poweron);
			if(led_effect_num==0)
			led_effect_num = Z_led_master;
        }
	}
	else
	{
        //if(LED_onoff==1)
        {
            LED_onoff = 0;
            LED_1_inf.sw=0;
            LED_1_inf.plan=0;
            #if LED2_EN
            LED_2_inf.sw=0;
            LED_2_inf.plan=0;
            #endif

			#if LED3_EN
            LED_3_inf.sw=0;
            LED_3_inf.plan=0;
            #endif

			#if LED4_EN
            LED_4_inf.sw=0;
            LED_4_inf.plan=0;
            #endif
			
            Update_led_play(Z_led_poweron);
        }
	}

		#ifdef CFG_FUNC_RECORDER_EN
    
            if(Global_rec_playing_flag==_Rec_dealing)
        	{
                //State_LED_plan_tar = Z_led_FM_play;
               // DBG("_Rec_dealing\n");
                Update_led_play(Z_led3_rec_dealing);
				//Update_led_play(Z_led_poweroff);
				Update_led_play(Z_led2_recfile_stop);
        	}
			else    if(Global_rec_playing_flag==_Rec_playback)
               {
                	//DBG("_Rec_playback\n");
               		Update_led_play(Z_led2_recfile_play);
					//Update_led_play(Z_led_poweroff);
					Update_led_play(Z_led3_rec_finish);
        	}
			else
				{
				  		//DBG("_Rec_stop\n");
						//Update_led_play(Z_led_poweroff);
						Update_led_play(Z_led2_poweron);
						Update_led_play(Z_led3_rec_finish);
			}
       
		#endif

    if(Task_state==Task_bt)
    {

        if((BT_state == BT_master)||(BT_state == BT_master_slave))
    	{
    	    //State_LED_plan_tar = Z_led_master;
    	  
            Update_led_play(Z_led_master);
			led_effect_num = Z_led_master;
			//RGB_curr_effect = RGB_Effect_red_blue_blinky;
    	}
		if(BT_state == BT_connect)
			{
			
				 Update_led_play(Z_led_bt_connected);
					 led_effect_num = Z_led_bt_pause;

		}
	   if(BT_state == BT_disconnect || BT_state == BT_reconnect)
			{
				 Update_led_play(Z_led_bt_disconnected);
				 led_effect_num = Z_led_master;
				// RGB_curr_effect = RGB_Effect_red_blue_blinky;
		}
        if(BT_state == BT_A2DP_Start||BT_state == BT_A2DP_Suspend)
        {
            if(Global_playing_flag==1)
            {
                //State_LED_plan_tar = Z_led_play_with_slave;
               
                Update_led_play(Z_led_bt_play);
				led_effect_num = Z_led_play;
            }
            else
            {
                //State_LED_plan_tar = Z_led_pause_with_slave;
               
                Update_led_play(Z_led_bt_pause);
				led_effect_num = Z_led_pause;
            }
        }
        
        if(BT_state == BT_master_phone||BT_state == BT_master_slave_phone)
		{			
			 Update_led_play(Z_led_play_with_slave);
			 led_effect_num = Z_led_play_with_slave;
		}

    	if(BT_state==BT_HFP_CALL_IN)
    	{
            //State_LED_plan_tar = Z_led_Call_I;
            Update_led_play(Z_led_Call_I);
    	}
    	else if(BT_state==BT_HFP_CALL_OUT)
    	{
            //State_LED_plan_tar = Z_led_Call_O;
            Update_led_play(Z_led_Call_O);
    	}
    	else if(BT_state==BT_HFP_CALL_ACTIVE)
    	{
            //State_LED_plan_tar = Z_led_Call_A;
            Update_led_play(Z_led_Call_A);
    	}else if(BT_state==BT_HFP_CALL_SUSPEND)
    	{
            //State_LED_plan_tar = Z_led_Call_A;
            Update_led_play(Z_led_Call_S);
    	}

		if (BT_state == BT_tws_mode){
            Update_led_play(Z_led_tws_master);
		}

    }
    else
    {
        if((Task_state&0xff)==Task_sd||(Task_state&0xff)==Task_sd_for_sd0||(Task_state&0xff)==Task_sd_for_sd1||(Task_state&0xff)==Task_usb)
        {

		
        	if(Global_playing_flag==1)
        	{
        		
                //State_LED_plan_tar = Z_led_TF_pause;
                Update_led_play(Z_led_TF_play);
				led_effect_num = Z_led_play;
        	}        
			if(Global_playing_flag==0)
        	{
        		
                //State_LED_plan_tar = Z_led_TF_play;
                Update_led_play(Z_led_TF_pause);
				led_effect_num = Z_led_pause;
        	}

        }
        if(Task_state==Task_fm)
        {
            if(Global_playing_flag==1)
        	{
                //State_LED_plan_tar = Z_led_FM_play;
                Update_led_play(Z_led_FM_play);
        	}
        	if(Global_playing_flag==0)
        	{
                //State_LED_plan_tar = Z_led_FM_pause;
                Update_led_play(Z_led_FM_pause);
        	}
        }
        if((Task_state&0xf0)==Task_line)
        {
            if(Global_playing_flag==1)
        	{
                
                //State_LED_plan_tar = Z_led_AUX_play;
                Update_led_play(Z_led_AUX_play);
				led_effect_num = Z_led_play;
        	}
        	if(Global_playing_flag==0)
        	{
                //State_LED_plan_tar = Z_led_AUX_pause;
                Update_led_play(Z_led_AUX_pause);
				led_effect_num = Z_led_pause;
        	}
        }
        if(Task_state==Task_pc)
        {
            if(Global_playing_flag==1)
        	{
                Update_led_play(Z_led_PC_play);
        	}
        	if(Global_playing_flag==0)
        	{
                Update_led_play(Z_led_PC_pause);
        	}
        }

        if(Task_state==Task_HDMI_in)
        {
            if(Global_playing_flag==1)
        	{
                Update_led_play(Z_led_HDMI_play);
        	}
        	if(Global_playing_flag==0)
        	{
                Update_led_play(Z_led_HDMI_pause);
        	}
        }
        if(Task_state==Task_OPTICAL)
        {
            if(Global_playing_flag==1)
        	{
                Update_led_play(Z_led_OPTICAL_play);
        	}
        	if(Global_playing_flag==0)
        	{
                Update_led_play(Z_led_OPTICAL_pause);
        	}
        }
        if(Task_state==Task_COAXIAL)
        {
            if(Global_playing_flag==1)
        	{
                Update_led_play(Z_led_COAXIAL_play);
        	}
        	if(Global_playing_flag==0)
        	{
                Update_led_play(Z_led_COAXIAL_pause);
        	}
        }
	
    	if(Flag_fm_scan==1)
    	{
            //State_LED_plan_tar = Z_led_FM_scan;
            Update_led_play(Z_led_FM_scan);
    	}

    }

   

	if(BT_state==BT_slave_master)
	{
        //State_LED_plan_tar = Z_led_slave;
        Update_led_play(Z_led_slave);
	}
	if(Flag_low_power==1)
	{
        //State_LED_plan_tar = Z_led_lowpower;
        Update_led_play(Z_led_lowpower);
	}
	else
	{
        if(LED_1_inf.plan==LED_plan_warning1)
        {
            LED_1_inf.plan=0;
        }
        if(LED_2_inf.plan==LED_plan_warning1)
        {
            LED_2_inf.plan=0;
        }
		if(LED_3_inf.plan==LED_plan_warning1)
        {
            LED_3_inf.plan=0;
        }
	    if(LED_4_inf.plan==LED_plan_warning1)
        {
            LED_4_inf.plan=0;
        }
	}

    if(Machine_state==Machine_poweron_tone||Machine_state==Machine_init)
	{
          Update_led_play(Z_led_poweron);
	}
    

}
void Dfac_indicatorLED(LED_indicator *LV_LED)
{
    if(LV_LED->sw==0)
    {
        LV_LED->state=LED_OFF;
    }
    else
    {
        if(LV_LED->plan==0)
        {
            LV_LED->state=LED_OFF;
        }
        if(LV_LED->plan==LED_plan_on)
        {
            LV_LED->state=LED_ON;
        }
        if(LV_LED->plan==LED_plan_off)
        {
            LV_LED->state=LED_OFF;
        }
        if(LV_LED->plan==LED_plan_slow_flicker1)
        {
            LV_LED->count++;
            if(LV_LED->count<200)
            {
                LV_LED->state=LED_ON;
            }
            else if(LV_LED->count<400)
            {
                LV_LED->state=LED_OFF;
            }
            else LV_LED->count=0;
        }
        if(LV_LED->plan==LED_plan_slow_flicker2)
        {
            LV_LED->count++;
            if(LV_LED->count<80)
            {
                LV_LED->state=LED_ON;
            }
            else if(LV_LED->count<160)
            {
                LV_LED->state=LED_OFF;
            }
            else LV_LED->count=0;
        }
		 if(LV_LED->plan==LED_plan_slow_flicker3)
        {
            LV_LED->count++;
            if(LV_LED->count<50)
            {
                LV_LED->state=LED_ON;
            }
            else if(LV_LED->count<100)
            {
                LV_LED->state=LED_OFF;
            }
            else LV_LED->count=0;
        }
		  if(LV_LED->plan==LED_plan_slow_flicker4)
        {
            LV_LED->count++;
            if(LV_LED->count<100)
            {
                LV_LED->state=LED_ON;
            }
            else if(LV_LED->count<200)
            {
                LV_LED->state=LED_OFF;
            }
            else LV_LED->count=0;
        }
		   if(LV_LED->plan==LED_plan_slow_flicker5)
        {
            LV_LED->count++;
            if(LV_LED->count<200)
            {
                LV_LED->state=LED_ON;
            }
            else if(LV_LED->count<400)
            {
                LV_LED->state=LED_OFF;
            }
            else LV_LED->count=0;
        }
        if(LV_LED->plan==LED_plan_fast_flicker1)
        {
            LV_LED->count++;
            if(LV_LED->count<20)
            {
                LV_LED->state=LED_ON;
            }
            else if(LV_LED->count<40)
            {
                LV_LED->state=LED_OFF;
            }
            else LV_LED->count=0;
        }
        if(LV_LED->plan==LED_plan_warning1)
        {
            LV_LED->count++;
            
            if(LV_LED->count<10)       LV_LED->state=LED_ON;
            else if(LV_LED->count<20)  LV_LED->state=LED_OFF;
            else if(LV_LED->count<30)  LV_LED->state=LED_ON;
            else if(LV_LED->count<40)  LV_LED->state=LED_OFF;
            else if(LV_LED->count<50)  LV_LED->state=LED_ON;
            else if(LV_LED->count<60)  LV_LED->state=LED_OFF;
            else if(LV_LED->count<70)  LV_LED->state=LED_ON;
            else if(LV_LED->count<80)  LV_LED->state=LED_OFF;
            else if(LV_LED->count<200) LV_LED->state=LED_OFF;
            else LV_LED->count=0;
        }
    }
    

    if(power_down_zx_flag)//关机
	{
        LV_LED->state = LED_OFF;
	}
		 
#if fun_idle_en
          // if(Machine_state==Machine_FakePoweroff)
         if(Idle_sw.idle_mode == on_line)
		 {
		      LV_LED->state = LED_OFF;
		 }
#endif


        
    if(LV_LED->state==LED_ON)
    {
        if(LV_LED->on_E==1) 
        {
            LV_LED->pin_state=1;
        }
        else
        {
            LV_LED->pin_state=0;
        }

		Led_Flicker_falg = 1;
    }
    if(LV_LED->state==LED_OFF)
    {
        if(LV_LED->on_E==1) 
        {
            LV_LED->pin_state=0;
        }
        else
        {
            LV_LED->pin_state=1;
        }

		Led_Flicker_falg = 0;
    }
}
void LED_scan_II()
{

   static u8 Led_Flicker_falg2=0; 

#if 0//def CFG_DMA_RGB_LED_EN
extern volatile uint8_t DmaLedDataFlag;
       if(DmaLedDataFlag)
       {
           return;
	   }
#endif 


  #if LED1_EN || LED2_EN || LED3_EN || LED4_EN
    LED_processing_event();
  	#endif
   
    #if LED1_EN
	    Dfac_indicatorLED(&LED_1_inf);
	#endif
	#if LED2_EN
		Dfac_indicatorLED(&LED_2_inf);
	#endif
	#if LED3_EN
		Dfac_indicatorLED(&LED_3_inf);
	#endif
	
   	#if LED4_EN
		Dfac_indicatorLED(&LED_4_inf);
	#endif 
		
		
#if LED1_EN
		// if(Idle_sw.idle_mode == off_line)
		 {
		     if(Led_Flicker_falg2 != Led_Flicker_falg)
	         {
	            Led_Flicker_falg2 = Led_Flicker_falg;
	            IO_contral_init(LED_1_inf.pin,0,0,0,LED_1_inf.pin_state);
			 }
		 }
		 
		
#endif
	
	
#if LED2_EN
		IO_contral_init(LED_2_inf.pin,0,0,0,LED_2_inf.pin_state);
#endif
    //_10ms_fun();    
}



void Blue_LED_PowerOn_Show(void)
{

    static u8 dir=0;
	static u8 cnt=0;
    static u8 frame=0;
	static u8 speed_cnt=0;

    if(Machine_state==Machine_init||Machine_state==Machine_poweron_tone)
    {
        speed_cnt++;
        if(speed_cnt >= 8)
		{
             speed_cnt=0;
			if(dir==0)
	        {
	           LED_Grade++;
			   if(LED_Grade >= 100)
			   {
	             LED_Grade=100;
				 dir=1;
			   }
			}
	        else
	        {
			   if(LED_Grade >0)
			   {
	             LED_Grade--;
			   }

			   if(LED_Grade==0)
			   {
	              dir=0;
			   }
			} 
		}
       
	}

	
}

void LED_Breathe_Contral()
{
    static u8 frame=0;
    frame++;
    if(frame>MAX_led_grade)frame=0;
    
    if(frame<LED_Grade)
    {
         IO_contral_init(Pin_LED1,0,0,0,1);
    }
    if(frame>LED_Grade)
    {
         IO_contral_init(Pin_LED1,0,0,0,0);
    }
}


//DEBUG_ZX(0, "  ", 0);
//printf("-- %d \r\n",X);
void DEBUG_ZX(u8 Number,char *p,u32 dat2)
{
	#if Z__DEBUG_custom

	#if Z__DEBUG_ALL==1
	    DBG("\r\n @@==== Number : %u ; char : %s  ; value : %u ;\r\n",Number,p,dat2);
	    return;
	#endif



    #if Z__DEBUG_BAT==0
    if(Number==DEBUG_BAT)return;
	#endif
	#if Z__DEBUG_IOKEY==0
    if(Number==DEBUG_IOKEY)return;
	#endif
	#if Z__DEBUG_ADKEY==0
    if(Number==DEBUG_ADKEY)return;
	#endif
	#if Z__DEBUG_Msg1==0
    if(Number==DEBUG_Msg1)return;
	#endif
	#if Z__DEBUG_Msg2==0
    if(Number==DEBUG_Msg2)return;
	#endif
	#if Z__DEBUG_Msg3==0
    if(Number==DEBUG_Msg3)return;
	#endif
   	#if Z__DEBUG_task==0
    if(Number==DEBUG_task)return;
	#endif	
	
   	#if Z__DEBUG_Timing==0
    if(Number==DEBUG_Timing)return;
	#endif	
	 #if Z__DEBUG_Tuch==0
    if(Number==DEBUG_Tuch)return;
	#endif	
	 #if Z__DEBUG_LED==0
    if(Number==DEBUG_led)return;
	#endif
    #if Z__DEBUG_FM==0
	   if(Number==DEBUG_Fm)return;
   #endif
     #if Z__DEBUG_BT==0
	   if(Number==DEBUG_Bt)return;
   #endif

   #if Z__DEBUG_REC==0
	   if(Number==DEBUG_Rec)return;
   #endif
   #if Z__DEBUG_AudEff==0
	   if(Number==DEBUG_AudEff)return;
   #endif
    printf("\r\n @@=====================Number : %d ; char : %s  ; value : %d ;\r\n",Number,p,dat2);
	#endif
}


void Z_unified_delay_ms(u16 LV_x)
{
    DelayMs(LV_x);
}




void Z_post_msg(u16 type,u8 msg_num0)
{
	MessageContext		msgSend;
    Custom_Event1_number = msg_num0;
    msgSend.msgId = type;
    MessageSend(mainAppCt.msgHandle, &msgSend);
}

void main_msg_send(u16 type)
{
	MessageContext		msgSend;
    msgSend.msgId = type;
    MessageSend(mainAppCt.msgHandle, &msgSend);
}

void Disp_msg_send(u16 type)
{
#if DIGITAL_TUBE_EN 

	MessageContext		msgSend;
    msgSend.msgId = type;
    MessageSend(GetDisplayMessageHandle(), &msgSend);
#endif	
}

void ZX_boot_init()
{
   Machine_state=Machine_init;
    Io_init();
    
    //PA_keep_off = 1;

    
    ZX_key_info_length = sizeof(ZX_key_info)/sizeof(ZX_key_info[0]);


    #if Built_in_echo_EN==1// && Echo_MIC_DET_EN==0
        KTV_MIC_state = 1;
    #endif

    RGB_SW = 1;

    #if LED1_EN//LEDS_mix_RGB_EN
        NVIC_SetPriority(Timer5_IRQn, 0);
     	Timer_Config(TIMER5,200,0);
     	Timer_Start(TIMER5);
     	NVIC_EnableIRQ(Timer5_IRQn);
		//rgb_timer_state = 1;
    #endif
	


    
    Z_init_ok = 1;
}


void ZX_MOS_contral(u16 MOS_pin,u8 MOS_state)
{

    if(MOS_state==MOS_ON)
    {
    DEBUG_ZX(0, "MOS_contral  1", MOS_pin);
        IO_contral_init(MOS_pin,0,0,0,1);
        //PowON();
    }
    if(MOS_state==MOS_OFF)
    {
    DEBUG_ZX(0, "MOS_contral  0", MOS_pin);
        IO_contral_init(MOS_pin,0,0,0,0);
    }
}

void power_on_zx()
{
	DBG("BOEU    power_on_zx  \n");

    #if Power_on_off_plan==2
    ZX_MOS_contral(P2_Pin_POWER_MOS,MOS_ON);
    #endif
    
    #if Power_on_off_plan==3
    ZX_MOS_contral(P3_Pin_POWER_MOS,MOS_ON);
    #endif

    //KT0641_init();


    
    Machine_state = Machine_run;
    
}
void power_down_zx()
{
	DEBUG_ZX(0, "power_down_zx", 0);

    u8 VL_x=0;

   #ifdef CFG_DMA_RGB_LED_EN	 
		 mainAppCt.rgb_mode=RGB_Effect_PowerOff;
    #endif
    #if LEDS_mix_RGB_EN
	   RGB_curr_effect = RGB_Effect_PowerOff;
    #endif
    
	Machine_state = Machine_Poweroff;
	power_down_zx_flag = 1;
		
    #if RGB_Since_the_flash_EN
        RGB_Since_the_flash_OFF;
	#endif
	#if LEDS_mix_RGB_EN
		   RGB_SW = 0;
	#endif

	#if BT_MICA_CTRL_EN
        BT_MICA_POWER_DIS();
	#endif
    #if BT_MICB_CTRL_EN
        BT_MICB_POWER_DIS();
	#endif



	#if POWER_LED_EN
	Power_LED1_OFF;
	Power_LED2_OFF;
	Power_LED3_OFF;
	#endif	   
    
    #if CFG_DAC0_EN
	AudioDAC_SoftMute(DAC0,1,1);
	#endif
	#if CFG_DAC1_EN
	AudioDAC_SoftMute(DAC1,1,1);
	#endif 
	#ifdef  DISP_DEV_AIP1668_LED
	//memset(&gDispBuff, 0, sizeof(gDispBuff));
	{
       u8 i;
	   for(i=0;i<16;i++)
	   {
          gDispBuff[i]=0;
	   }
	}
	aip1668_init();
	aip1668_write_ndata(gDispBuff,16);
	led_screen_sw=0;
    #endif 
	PA_contral();
	DelayMs(10);

   // MUTE_ON();
	#if 0//CHAGER_DCDC_CTRL_EN
	CHAGER_DCDC_DIS();
	#endif
    #if RGB_DCDC_CTRL_EN
	RGB_DCDC_DIS();
    #endif

	
    //dac_mute(1, 1);
    /*
    	set_sys_vol(0,0,FADE_ON);
    	dac_off_control();
    	//ldo5v_detect_deal(2);
    */
#if 0//Power_on_off_plan==2
    for(;VL_x<5;VL_x++)
    {
     DEBUG_ZX(0, " power_down_zx=2 ", 0);
        DelayMs(5);
        ZX_MOS_contral(P2_Pin_POWER_MOS,MOS_OFF);
        DelayMs(200);
    }
    while(1);
#endif


#if Power_on_off_plan==3
    for(;VL_x<5;VL_x++)
    {
        DelayMs(5);
        ZX_MOS_contral(P3_Pin_POWER_MOS,MOS_OFF);
        DelayMs(200);
    }
    while(1);
#endif
   
	#if (Power_on_off_plan==7)
	#ifdef CFG_SOFT_POWER_KEY_EN
	DBG("power_down_zx --- SoftKeyPowerOff\r\n");
	#if PA1_EN
       IO_contral_init(PA1_mute_pin,0,0,0,0);
	   DelayMs(10);
	#endif
	
	 //SoftKeyPowerOff();
	 P7_POWER_MOS_OFF();
	#endif
	#endif
	
#if (Power_on_off_plan==8)

	DBG("power_down_zx ---   POWER_MOS_OFF()-----------\r\n");
    #if PA1_EN
       IO_contral_init(PA1_mute_pin,0,0,0,0);
	   DelayMs(200);
	#endif
    POWER_MOS_OFF();
	
#endif

#if Power_on_off_plan==2
    IO_contral_init(P2_Pin_POWER_MOS,0,0,0,0);
#endif


#if (Power_on_off_plan==1)||(Power_on_off_plan==5)||(Power_on_off_plan==6)
    while(1)
    {
        DEBUG_ZX(0, "soft_poweroff  ", 0);
        #ifdef CFG_FUNC_BACKUP_EN
		  SystemPowerDown();
		#endif
        DelayMs(1000);
    }
#endif




}


//if(Task_state==Task_bt)
void Save_task_state(u8 LV_state)
{
    static u8 last_mode=0;
    if(last_mode!=LV_state)
    {
        Task_state = LV_state;
        last_mode = LV_state;
        DEBUG_ZX(DEBUG_task, "******** Task_state!! ******", Task_state);
    }
    //Last_bt_state = BT_state;
    
    if(Task_state!=Task_bt)
    {
        //BT_state = BT_master;
    }
    if((Task_state&0xf0)!=Task_idle)
    {
        if((Power_on_off_plan==1)||(Power_on_off_plan==5))
        {
            //Save_task_state_to_VM(Task_state);
        }
    }
    //Flag_PITCH=0;
}



void tone_play_end()
{
	MessageContext		msgSend;

   DEBUG_ZX(0, " tone_play_end ",0);

    if(Idle_sw.power_on_tone == tone_start)
    {
       Idle_sw.power_on_tone = tone_end;
	   main_msg_send(MSG_QUIT_IDLE_MODE);
	}
    if(Idle_sw.power_off_tone == tone_start)
    {
       Idle_sw.power_off_tone = tone_end;
	}

    if(Tws_master_bt_con==1)
    {
       Tws_master_bt_con=0;
	   if(btManager.btLinkState==0)
	   {
	      main_msg_send(MSG_BT_CONNECT_CTRL);//连接手机
	   }
    }
	 
    if(PAM_ctrl_en == 1)
    {
        PAM_ctrl_en = 2;
	}


    if(Flag_poweroff)
   	{
   	  //  IO_contral_init(PA1_mute_pin,0,0,0,0);
	 	DelayMs(50);
   	}

    if(max_min_tone_msg_send_en == FALSE)
    {
        max_min_tone_msg_send_en = TRUE;
	}
 
    if(Flag_poweroff)
    {
       // tws_effect_mode_send(_TWS_Slave_power_off);
       #if fun_idle_en==0
        power_down_zx(); 
	   #else
            if(IsInCharge())//插入充电,进IDLE
			{
                 main_msg_send(MSG_ENTER_IDLE_MODE);
				 Machine_state=Machine_FakePoweroff;

				#ifdef CFG_DMA_RGB_LED_EN	 
					 mainAppCt.rgb_mode=RGB_Effect_PowerOff_Charge;
			    #endif
			    #if LEDS_mix_RGB_EN
				   RGB_curr_effect = RGB_Effect_PowerOff_Charge;
			    #endif
	
			}
			else
			{ 
                power_down_zx(); 
			}
	   #endif
	
    }
    Tone_play_state=0;
    PA_contral();
   if(Flag_IR_poweroff)		
	{
	//  DEBUG_ZX(0, " tone_play_end--IR POWER off!! ",0);
	Flag_IR_poweroff=0;
	msgSend.msgId= MSG_DEEPSLEEP;
	MessageSend(mainAppCt.msgHandle, &msgSend);
   	}

   if(update_ok)
   {
       update_ok=0;
    #if fun_idle_en == 0
         power_down_zx(); //升级后关机
	#endif
   }
   
}



#if Echo_MIC_DET_EN
Dev_port_1 MIC_DET_inf=
{
    Pin_Echo_MIC_DET,
    Z_DEV_way_IO,
    0,
    0,
    Z_DEV_OFF_HOLD,
    30,
    0,
    Effective_Pin_Echo_MIC,
};
#endif

#if BT_MIC_A_DET_EN
Dev_port_1 BT_MIC_A_DET_inf=
{
    Pin_BT_MIC_A_DET,
    Z_DEV_way_IO,
    0,
    0,
    Z_DEV_OFF_HOLD,
    30,
    0,
    Effective_Pin_BT_MIC_A,
};
#endif

#if BT_MIC_B_DET_EN
Dev_port_1 BT_MIC_B_DET_inf=
{
    Pin_BT_MIC_B_DET,
    Z_DEV_way_IO,
    0,
    0,
    Z_DEV_OFF_HOLD,
    30,
    0,
    Effective_Pin_BT_MIC_B,
};
#endif
#if BT_MIC_DET_EN
Dev_port_1 BT_MIC_DET_inf=
{
    Pin_BT_MIC_DET,
    Z_DEV_way_IO,
    0,
    0,
    Z_DEV_OFF_HOLD,
    30,
    0,
    Effective_Pin_BT_MIC,
};
#endif
#if PH_DET_EN
Dev_port_1 PH_DET_inf=
{
    Pin_PH_DET,
    Z_DEV_way_IO,
    0,
    0,
    Z_DEV_OFF_HOLD,
    30,
    0,
    EV_PH,
};
#endif




void Dfac_IO_deal(Dev_port_1 *LV_port)
{
    if(LV_port->DET_way==Z_DEV_way_IO)
    {
        LV_port->IO_cur = IO_in_check(LV_port->DET_pin);
    }

    if(LV_port->IO_cur!=LV_port->IO_Stable)
    {
        LV_port->DET_cnt++;
        if(LV_port->DET_cnt==LV_port->DET_fil)
        {
            LV_port->IO_Stable = LV_port->IO_cur;
            LV_port->DET_cnt=0;
        }
    }
    else
    {
        LV_port->DET_cnt=0;
    }
    
    if(LV_port->IO_Stable==LV_port->IO_Effective)
    {
        if(LV_port->DEV_state==Z_DEV_OFF_HOLD)
        {
            LV_port->DEV_state = Z_DEV_ON;
        }
    }
    else
    {
        if(LV_port->DEV_state==Z_DEV_ON_HOLD)
        {
            LV_port->DEV_state = Z_DEV_OFF;
        }
    }
}

void IO_sacn()
{

#if Echo_MIC_DET_EN
    Dfac_IO_deal(&MIC_DET_inf);
    
    if(MIC_DET_inf.DEV_state==Z_DEV_ON)
    {
        MIC_DET_inf.DEV_state = Z_DEV_ON_HOLD;
       

       // KTV_MIC_state = 1;
        PA_contral();
    }
    if(MIC_DET_inf.DEV_state==Z_DEV_OFF)
    {
        MIC_DET_inf.DEV_state = Z_DEV_OFF_HOLD;
       
        //KTV_MIC_state = 0;
        PA_contral();
    }
#endif 
#if BT_MIC_A_DET_EN
    Dfac_IO_deal(&BT_MIC_A_DET_inf);
    
    if(BT_MIC_A_DET_inf.DEV_state==Z_DEV_ON)
    {
        BT_MIC_A_DET_inf.DEV_state = Z_DEV_ON_HOLD;

        BT_MIC_state = 1;
        PA_contral();
    }
    if(BT_MIC_A_DET_inf.DEV_state==Z_DEV_OFF)
    {
        BT_MIC_A_DET_inf.DEV_state = Z_DEV_OFF_HOLD;
       
        BT_MIC_state = 0;
        PA_contral();
    }
#endif 

#if BT_MIC_B_DET_EN
    Dfac_IO_deal(&BT_MIC_B_DET_inf);
    
    if(BT_MIC_B_DET_inf.DEV_state==Z_DEV_ON)
    {
        BT_MIC_B_DET_inf.DEV_state = Z_DEV_ON_HOLD;
       
       BT_MIC2_state = 1;
        PA_contral();
    }
    if(BT_MIC_B_DET_inf.DEV_state==Z_DEV_OFF)
    {
        BT_MIC_B_DET_inf.DEV_state = Z_DEV_OFF_HOLD;
		
        BT_MIC2_state = 0;
        PA_contral();
    }
#endif 

#if BT_MIC_DET_EN
    Dfac_IO_deal(&BT_MIC_DET_inf);
    
    if(BT_MIC_DET_inf.DEV_state==Z_DEV_ON)
    {
        BT_MIC_DET_inf.DEV_state = Z_DEV_ON_HOLD;
       

        PA_contral();
    }
    if(BT_MIC_DET_inf.DEV_state==Z_DEV_OFF)
    {
        BT_MIC_DET_inf.DEV_state = Z_DEV_OFF_HOLD;
       
        PA_contral();
    }
#endif 
#if PH_DET_EN
    Dfac_IO_deal(&PH_DET_inf);
    
    if(PH_DET_inf.DEV_state==Z_DEV_ON)
    {
        PH_DET_inf.DEV_state = Z_DEV_ON_HOLD;

	 #if  PH_EN
        PH_EN_pin_ON;
	 #endif
        PH_online_state = 1;
	   #if PH_DET_AUDIO_EFFECT_EN
	       // AudioLine3Mic1Enable(0);
	       //AudioCoreSourceMute2(MIC_SOURCE_NUM, 1, 1);
            main_msg_send(MSG_EFFECT_PH_ON_LINE); 
	   #endif
        PA_contral();
       
    }
    if(PH_DET_inf.DEV_state==Z_DEV_OFF)
    {
        PH_DET_inf.DEV_state = Z_DEV_OFF_HOLD;
       #if  PH_EN
         PH_EN_pin_OFF;
	   #endif
        PH_online_state = 0;
	    #if PH_DET_AUDIO_EFFECT_EN
	      //AudioLine3Mic1Enable(0);    
	      //AudioCoreSourceUnmute2(MIC_SOURCE_NUM, 1, 1);
          main_msg_send(MSG_EFFECT_PH_OFF_LINE);
    	#endif
        PA_contral();

    }
#endif 


}
void Input_check()//
{
    if(Z_init_ok)
    {
        //Rheostat_scan();
        //Encoder_scan();
        IO_sacn();
    }
    

    
}





void RGB_Contral_R()
{
    static u8 R_frame=0;
    R_frame++;
    if(R_frame>MAX_led_grade)R_frame=0;
    
    if(R_frame<RGB_R_Grade)
    {
        LEDS_RGB_R_ON;
    }
    if(R_frame>RGB_R_Grade)
    {
        LEDS_RGB_R_OFF;
    }
}
void RGB_Contral_G()
{
    static u8 G_frame=0;
    G_frame++;
    if(G_frame>MAX_led_grade)G_frame=0;
    
    if(G_frame<RGB_G_Grade)
    {
        LEDS_RGB_G_ON;
        //DBG(" _G_: %d \n", 1);
    }
    if(G_frame>RGB_G_Grade)
    {
        LEDS_RGB_G_OFF;
    }
}
void RGB_Contral_B()
{
    static u8 B_frame=0;
    B_frame++;
    if(B_frame>MAX_led_grade)B_frame=0;
    
    if(B_frame<RGB_B_Grade)
    {
        LEDS_RGB_B_ON;
    }
    if(B_frame>RGB_B_Grade)
    {
        LEDS_RGB_B_OFF;
    }
}








//AudioEffectSilenceDectorApply
void DAC_energy_deal1()
{
    u32 Curr_DAC_energy2=0;

   // _Curr_MIC_energy = gCtrlVars.MicAudioSdct_unit.level;
    Curr_DAC_energy2 = Get_Music_Energy();
    Curr_DAC_energy = Curr_DAC_energy2;
   // Curr_DAC_energy = ((Curr_DAC_energy2&0xfffffffc)>>2);
	
  // _Curr_Music_energy = Curr_DAC_energy2;  //DBG
}



u16 Curr_DAC_grade; 

void DAC_energy_deal2()//100级
{
    static u8  TOP_clean_con=0;
    
    static u16  MAX_clean_con=0;
    static u16 DAC_grade=0;
    static u32 TOP_DAC_energy=0;
    static u32 MAX_DAC_energy=0;

    Curr_DAC_energy = Get_Curr_Music_energy;
    if(Curr_DAC_energy>=TOP_DAC_energy)
    {
        if(Curr_DAC_energy>0)
        {
            TOP_DAC_energy=Curr_DAC_energy;
            TOP_clean_con=0;
            if(TOP_DAC_energy>=MAX_DAC_energy)
            {
                MAX_DAC_energy = TOP_DAC_energy;
                MAX_clean_con=0;
            }
            /*
            else
            {
                MAX_clean_con++;
                if(MAX_clean_con>200)
                {
                    MAX_clean_con=0;
                    MAX_DAC_energy=0;
                }
            }*/
            DAC_grade = Curr_DAC_energy/(MAX_DAC_energy/100);
            //Curr_DAC_grade=DAC_grade;
            if(DAC_grade>65)Curr_DAC_grade=DAC_grade+4;
            //if(DAC_grade==0)Curr_DAC_grade=0;
        }
        //else Curr_DAC_grade=0;
        
    }
    else
    {
        TOP_clean_con++;
        if(TOP_clean_con>6)//5
        {
            TOP_clean_con=0;
            TOP_DAC_energy=0;
        }
    }
    
        
    MAX_clean_con++;
    if(MAX_clean_con>900)//900
    {
        MAX_clean_con=0;
        MAX_DAC_energy=0;
    }



	
    
    if(Curr_DAC_grade>2)
    {
        Curr_DAC_grade=Curr_DAC_grade-2;
    }
    else Curr_DAC_grade=0;

    
}
void RGB_effect_rhythm()
{
    u8 Rhythm_grade=0;
    static u16 RGB_Effect1_timer=0;
    static u8 Effect2_color=0;
    
    DAC_energy_deal2();
    Rhythm_grade = Curr_DAC_grade;
    
    RGB_Effect1_timer++;
    if(RGB_Effect1_timer>1000)//1000
    {
        RGB_Effect1_timer=0;
        Effect2_color++;
        if(Effect2_color>7)Effect2_color=1;
    }
    switch(Effect2_color)
    {
        case 1:     
		            if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_R_IO
					}
					else
					{						
						OPEN_RGB_R_IO
					}
					///CLOSE_RGB_G_IO
					//CLOSE_RGB_B_IO
		            RGB_R_Grade = Rhythm_grade;
                    RGB_G_Grade = 0;
                    RGB_B_Grade = 0;
                    break;
       case 2:     
		            if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_R_IO
						//CLOSE_RGB_G_IO
					}
					else
					{
						OPEN_RGB_R_IO
						OPEN_RGB_G_IO
					}
					//CLOSE_RGB_B_IO
					RGB_R_Grade = Rhythm_grade;
                    RGB_G_Grade = Rhythm_grade;
                    RGB_B_Grade = 0;
					
                    break;
        case 3:     
		           // CLOSE_RGB_R_IO
		            if( Rhythm_grade < 10 )
					{	
						//CLOSE_RGB_G_IO
					}
					else
					{
					   OPEN_RGB_G_IO
					}					
					//CLOSE_RGB_B_IO
		            RGB_R_Grade = 0;
                    RGB_G_Grade = Rhythm_grade;
                    RGB_B_Grade = 0;
                    break;
        case 4:     
		          //  CLOSE_RGB_R_IO
					//CLOSE_RGB_G_IO
					if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_B_IO
					
					}else{
						
						OPEN_RGB_B_IO
					}
		            RGB_R_Grade = 0;
                    RGB_G_Grade = 0;
                    RGB_B_Grade = Rhythm_grade;
                    break;
        case 5:     
				    //CLOSE_RGB_R_IO
					if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_G_IO
						//CLOSE_RGB_B_IO
					}
					else
					{
					    OPEN_RGB_G_IO
						OPEN_RGB_B_IO
					}
		            RGB_R_Grade = 0;
                    RGB_G_Grade = Rhythm_grade;
                    RGB_B_Grade = Rhythm_grade;
                    break;
        case 6:     
					if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_R_IO
					}
					else
					{
				        OPEN_RGB_R_IO
					}
					CLOSE_RGB_G_IO
					if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_B_IO
					}
					else
					{
					   OPEN_RGB_B_IO
					}
		            RGB_R_Grade = Rhythm_grade;
                    RGB_G_Grade = 0;
                    RGB_B_Grade = Rhythm_grade;
                    break;
        case 7:     
				    if( Rhythm_grade < 10 )
					{
						//CLOSE_RGB_R_IO
						//CLOSE_RGB_G_IO
						//CLOSE_RGB_B_IO
					}
					else
					{
						OPEN_RGB_R_IO
						OPEN_RGB_G_IO
						OPEN_RGB_B_IO
					}
		            RGB_R_Grade = Rhythm_grade;
                    RGB_G_Grade = Rhythm_grade;
                    RGB_B_Grade = Rhythm_grade;
                    break;
					
		case 8:     
				    CLOSE_RGB_R_IO
					CLOSE_RGB_G_IO
					CLOSE_RGB_B_IO
		            RGB_R_Grade = 0;
                    RGB_G_Grade = 0;
                    RGB_B_Grade = 0;
                    break;
    }
}
void RGB_effect_rhythm2()
{
    u8 Rhythm_grade=0;
    static u16 RGB_Effect1_timer=0;
    static u8 Effect2_color=0;
    
    //DAC_energy_deal2();
    //Rhythm_grade = Curr_DAC_grade;

	static u8  TOP_clean_con=0;
    
    static u16  MAX_clean_con=0;
    static u16 DAC_grade=0;
    static u32 TOP_DAC_energy=0;
    static u32 MAX_DAC_energy=0;
    static u8 effect_rhythm2_flash_con=0;

    
    if(Curr_DAC_energy>=TOP_DAC_energy)
    {
        if(Curr_DAC_energy>0)
        {
            TOP_DAC_energy=Curr_DAC_energy;
            TOP_clean_con=0;
            if(TOP_DAC_energy>=MAX_DAC_energy)
            {
                MAX_DAC_energy = TOP_DAC_energy;
                MAX_clean_con=0;
            }
			DAC_grade = Curr_DAC_energy/(MAX_DAC_energy/100);
            //Curr_DAC_grade=DAC_grade;
            if(DAC_grade>60)
			{
				DAC_energy_top_flag = 1;
				Curr_DAC_grade=110;
			}
            
        }
        
        
    }
    else
    {
        TOP_clean_con++;
        if(TOP_clean_con>10)//5
        {
            TOP_clean_con=0;
            TOP_DAC_energy=0;
        }
    }
    
        
    MAX_clean_con++;
    if(MAX_clean_con>900)//900
    {
        MAX_clean_con=0;
        MAX_DAC_energy=0;
    }
	
	if(Curr_DAC_grade>2)
    {
        Curr_DAC_grade=Curr_DAC_grade-2;
    }
    else Curr_DAC_grade=0;
	
/*
	if(DAC_energy_top_flag)
	{
		effect_rhythm2_flash_con++;
		if	   (effect_rhythm2_flash_con<10	)Rhythm_grade = 0;
		//else if(effect_rhythm2_flash_con<40	)Rhythm_grade = Curr_DAC_grade;
		//else if(effect_rhythm2_flash_con<45	)Rhythm_grade = 0;
		//else if(effect_rhythm2_flash_con<80	)Rhythm_grade = 100;
		//else if(effect_rhythm2_flash_con<100	)Rhythm_grade = 0;
		else 
		{
			DAC_energy_top_flag=0;
		}
	}
	else
	{
		effect_rhythm2_flash_con=0;
		Rhythm_grade = Curr_DAC_grade;
	}*/
	
	Rhythm_grade = Curr_DAC_grade;

	
/*
	if(Rhythm_grade>100)
	{
		Rhythm_grade=0;
	}
	else
	{
		Rhythm_grade = 100-Rhythm_grade;
	}*/



	
    RGB_R_Grade = Rhythm_grade;
    RGB_G_Grade = Rhythm_grade;
    RGB_B_Grade = Rhythm_grade;

	
}

void RGB_effect_breathe1()
{
    static u16 RGB_Effect2_timer=RGB_effect_breathe1_speed;
    
    static u8 Color1_value=100;
    static u8 Color2_value=0;
    static u8 Color3_value=0;
    /*
    if(mode==0)
    {
        RGB_Effect2_timer=effect_breathe2_speed;
        Color1_value=100;
    }*/

    
    RGB_Effect2_timer++;
    if(RGB_Effect2_timer>=RGB_effect_breathe1_speed)//20
    {
        RGB_Effect2_timer=0;


        if((Color1_value==100)&&(Color3_value==0))
        {
            if(Color2_value<100)Color2_value++;
        }
        if((Color2_value==100)&&(Color1_value==0))
        {
            if(Color3_value<100)Color3_value++;
        }
        if((Color3_value==100)&&(Color2_value==0))
        {
            if(Color1_value<100)Color1_value++;
        }
        
        if(Color1_value==100)
        {
            if(Color3_value>0)Color3_value--;
        }
        if(Color2_value==100)
        {
            if(Color1_value>0)Color1_value--;
        }
        if(Color3_value==100)
        {
            if(Color2_value>0)Color2_value--;
        }
        
/*
        RGB_R_Grade = 0;
        RGB_G_Grade = 10;
        RGB_B_Grade = 100;
*/
        
        RGB_R_Grade = Color1_value;
        RGB_G_Grade = Color2_value;
        RGB_B_Grade = Color3_value;
    }
}

void RGB_effect_flowing_water()
{
 	static u16 RGB_Effect2_timer=RGB_effect_breathe1_speed;
    
    static u8 Color1_value=100;
    static u8 Color2_value=0;
    static u8 Color3_value=0;


	static u16 timer_cnt = 0;
	static u16 timer_step = 45;
	

    
    RGB_Effect2_timer++;
    if(RGB_Effect2_timer>=RGB_effect_breathe1_speed)//20
    {
        RGB_Effect2_timer=0;


        if((Color1_value==100)&&(Color3_value==0))
        {
            if(Color2_value<100)Color2_value++;
        }
        if((Color2_value==100)&&(Color1_value==0))
        {
            if(Color3_value<100)Color3_value++;
        }
        if((Color3_value==100)&&(Color2_value==0))
        {
            if(Color1_value<100)Color1_value++;
        }
        
        if(Color1_value==100)
        {
            if(Color3_value>0)Color3_value--;
        }
        if(Color2_value==100)
        {
            if(Color1_value>0)Color1_value--;
        }
        if(Color3_value==100)
        {
            if(Color2_value>0)Color2_value--;
        }
        
/*
        RGB_R_Grade = 0;
        RGB_G_Grade = 10;
        RGB_B_Grade = 100;
*/
        
        RGB_R_Grade = Color1_value;
        RGB_G_Grade = Color2_value;
        RGB_B_Grade = Color3_value;
    }	


	timer_cnt++;
	if (timer_cnt <= timer_step){
		RGB_POWER_1_IO_L;
		RGB_POWER_2_IO_L;
		RGB_POWER_3_IO_H;		
	} else if (timer_cnt <= timer_step * 2) {
		RGB_POWER_1_IO_L;
		RGB_POWER_2_IO_H;
		RGB_POWER_3_IO_H;		
	} else if (timer_cnt <= timer_step * 3) {
		RGB_POWER_1_IO_H;
		RGB_POWER_2_IO_H;
		RGB_POWER_3_IO_H;		
	}else if (timer_cnt <= timer_step * 4) {
		RGB_POWER_1_IO_L;
		RGB_POWER_2_IO_H;
		RGB_POWER_3_IO_H;		
	} else if (timer_cnt <= timer_step * 5) {
		RGB_POWER_1_IO_L;
		RGB_POWER_2_IO_L;
		RGB_POWER_3_IO_H;		
	} else if (timer_cnt <= timer_step * 6) {
		RGB_POWER_1_IO_L;
		RGB_POWER_2_IO_L;
		RGB_POWER_3_IO_L;		
	} else if(timer_cnt > timer_step * 6) {
		timer_cnt = 0;
	}
	

}

void RGB_effect_red_blue_blinky()
{
	static u16 timer_cnt = 0;
	static u16 timer_step = 250;
		
	static u8 Color1_value=100;
	static u8 Color3_value=0;


	timer_cnt++;
	
	if (timer_cnt <= timer_step){
		Color3_value = 0;
		Color1_value = 100;
	} else if (timer_cnt <= timer_step*2){
		Color3_value = 100;
		Color1_value = 0;
		
	} else if (timer_cnt > timer_step*2){
		timer_cnt = 0;
	}
	

	
	RGB_R_Grade = Color1_value;
	RGB_G_Grade = 0;
	RGB_B_Grade = Color3_value;



}

void RGB_effect_bt_on()
{	
	RGB_R_Grade = 0;
	RGB_G_Grade = 0;
	RGB_B_Grade = 100;
}


void RGB_Contral()
{
  
#if LEDS_mix_RGB_EN

    static u32 DAC_energy[20] = {0};
    static u8 i = 0; 
    static u32 DAC_energy_nmu;

	
    static u16 DAC_0_con=0;
    DAC_energy_deal1();
    if(RGB_SW==0)
    {
		/*
        RGB_R_Grade = 0;
        RGB_G_Grade = 0;
        RGB_B_Grade = 0;*/
        /*
        IO_contral_init(LEDS_RGB_R_IO_pin,0,0,0,!LEDS_RGB_R_OPEN);      
        IO_contral_init(LEDS_RGB_G_IO_pin,0,0,0,!LEDS_RGB_G_OPEN);      
        IO_contral_init(LEDS_RGB_B_IO_pin,0,0,0,!LEDS_RGB_B_OPEN); */
        
		CLOSE_RGB_R_IO;
		CLOSE_RGB_G_IO;
		CLOSE_RGB_B_IO;
		
		
		RGB_Timer_OFF();
		
    }
    else 
    {

	   
        #if RGB_effect_plan==1
		RGB_Timer_ON();
        DAC_energy_deal1();
        
        //if(Curr_DAC_energy<10 || BT_state_flag == 0 || A2DP_Play_State == 0)
	    if(Curr_DAC_energy<10)
        {
            Curr_DAC_energy=0;
			
        }
       
		
        if((Curr_DAC_energy==0)/*||(Global_playing_flag==0)*/)
        {
          				
            DAC_0_con++;
            if(DAC_0_con>80)//2000
            {
                OPEN_RGB_R_IO;
				OPEN_RGB_G_IO;
				OPEN_RGB_B_IO;
                RGB_curr_effect=RGB_effect_breathe_type;  //呼吸
            }
        }
        else
        {
            DAC_0_con=0;
					
              RGB_curr_effect=RGB_Effect_rhythm1;   //节奏
			  			
        }
		
        
       
        #endif

        #if RGB_effect_plan==2

        switch(RGB_effect_num)
        {
            case 0: RGB_curr_effect=0; break;
            case 1: RGB_curr_effect=RGB_effect_mode_breathe; break;
            case 2: RGB_curr_effect=RGB_effect_mode_rhythm;  break;
        }
        if(RGB_curr_effect==RGB_effect_mode_rhythm)
        {
            DAC_energy_deal1();
        }
        
        #endif
        
        #if RGB_effect_plan==3
      
        switch(RGB_effect_num)
        {
            case 0: RGB_curr_effect=0;                          break;
            case 1: RGB_curr_effect=RGB_effect_mode_R;          break;
            case 2: RGB_curr_effect=RGB_effect_mode_G;          break;
            case 3: RGB_curr_effect=RGB_effect_mode_B;          break;
            case 4: RGB_curr_effect=RGB_effect_mode_RB;          break;
            case 5: RGB_curr_effect=RGB_effect_mode_RG;          break;
            case 6: RGB_curr_effect=RGB_effect_mode_GB;          break;
           // case 7: RGB_curr_effect=RGB_effect_mode_breathe;    break;
           // case 8: RGB_curr_effect=RGB_effect_mode_rhythm;     break;
        }
      /*  if(RGB_curr_effect==RGB_effect_mode_rhythm)
        {
            DAC_energy_deal1();
        }*/
        
        #endif
        
        if(RGB_curr_effect==0)
        {
            RGB_R_Grade = 0;
            RGB_G_Grade = 0;
            RGB_B_Grade = 0;
        }
        if(RGB_curr_effect==RGB_Effect_rhythm1)
        {
			//if (Curr_DAC_energy == 0){
			if (Curr_DAC_energy < 15){
			    LEDS_RGB_R_IO_L;
				LEDS_RGB_G_IO_L;
				LEDS_RGB_B_IO_L;

			} else {
				LEDS_RGB_R_IO_H;
				LEDS_RGB_G_IO_H;
				LEDS_RGB_B_IO_H;

			}
            RGB_effect_rhythm();
        } 
        if(RGB_curr_effect==RGB_Effect_rhythm2)
        {
			//if (Curr_DAC_energy < 10){
			if (Curr_DAC_energy < 20){
				/*LEDS_RGB_R_IO_L;
				LEDS_RGB_G_IO_L;
				LEDS_RGB_B_IO_L;*/

			} else {
				/*LEDS_RGB_R_IO_H;
				LEDS_RGB_G_IO_H;
				LEDS_RGB_B_IO_H;*/

			}
						
            RGB_effect_rhythm2();
        } 
        if(RGB_curr_effect==RGB_Effect_breathe1)
        {
			//LEDS_RGB_R_IO_H;
			//LEDS_RGB_G_IO_H;
			//LEDS_RGB_B_IO_H;
            RGB_effect_breathe1();
        }
        if(RGB_curr_effect==RGB_Effect_breathe2)
        {
            //RGB_effect_breathe2(1);
        }
		if (RGB_curr_effect == RGB_Effect_flowing_water){
			RGB_effect_flowing_water();
		}
		if (RGB_curr_effect == RGB_Effect_red_blue_blinky){
			LEDS_RGB_R_IO_H;
			LEDS_RGB_G_IO_H;
			LEDS_RGB_B_IO_H;
			RGB_effect_red_blue_blinky();
		}
		if (RGB_curr_effect == RGB_Effect_bt_on){
			RGB_POWER_2_IO_H;
			RGB_effect_bt_on();
		}
		if (RGB_curr_effect == RGB_Effect_close){
			RGB_POWER_1_IO_L;
			RGB_POWER_2_IO_L;
			RGB_POWER_3_IO_L;
		}
        //RGB_effect_rhythm();
        
        
    }
    
    
#endif
}



void RGB_Mode_Contral(void)
{
   static u8 poweron_charge_det=0;
   static u8 Lowpower_set=1;

   
   if(Idle_sw.idle_mode == off_line)//非待机模式
   {
       if(IsInCharge())//插入充电
       {
           if(poweron_charge_det==0)//设置一次闪灯
           {
              poweron_charge_det=1;
			  if(Idle_sw.power_on_tone!=tone_start)
			  {
				  /*#ifdef CFG_DMA_RGB_LED_EN
				    mainAppCt.rgb_mode=mainAppCt.temp_rgb_mode;
	              #endif*/
	              #if LEDS_mix_RGB_EN
				     RGB_curr_effect = RGB_Effect_PowerOn_Charge;
	              #endif
			  }
		   }
	   }
	   else//未插入充电
	   {
	       if(poweron_charge_det)
	       {
              poweron_charge_det=0;
              #if LEDS_mix_RGB_EN
			     RGB_curr_effect = Temp_RGB_curr_effect;
              #endif
		   }
           

            if(Flag_low_power)//报低电
            {
               if(Flag_low_power_tone)//设置一次灯效
               {
                  Flag_low_power_tone=0;
                  #if LEDS_mix_RGB_EN
			        RGB_curr_effect = RGB_Effect_Lowpower;
			      #endif
			   }
			}
		
	   }
   }
  
  /* static u16 c=0;
   c++;
   if(c>500)
   	{
       c=0;
    DBG("--------- RGB_Mode_Contral\n");
   }*/

   switch(RGB_curr_effect)
	{
		case RGB_Effect_PowerOn:		
               RGB_PowerOn_Show();
			  
			break;
			
		case RGB_Effect_PowerOff:						
			   RGB_PowerOff_Show();
			break;
			
	    case RGB_Effect_Bt_Con:
                RGB_Bt_Con_Show();
			 break;
			 
		case RGB_Effect_Bt_Dis:
			    RGB_Bt_Dis_Show();
			 break;
			 
	    case RGB_Effect_Bt_Play:
			      RGB_Bt_Play_Show();
			   //  RGB_effect_rhythm();
			 break;	

		case RGB_Effect_Bt_Pause:
			      RGB_Bt_Pause_Show();
			 break;	
			 
		case RGB_Effect_HFP_CALL_IN:
			     RGB_HFP_CALL_IN_Show();
			 break;	
		 /*case RGB_Effect_HFP_Answer:
			 break;*/
			 
		case RGB_Effect_Lowpower:
			    RGB_LowPower_Show();
			 break;	

		case RGB_Effect_PowerOn_Charge:
			    RGB_PowerOn_Charge_Show();
			 break;	
			 
		case RGB_Effect_PowerOff_Charge:
			    RGB_PowerOff_Charge_Show();
			 break;	
			 
		case RGB_Effect_TWS_Con:
			    RGB_TWS_Show();
			 break;	 
		case 0xff:
			   
			 break;	
	}

}

/*
  开机
  1：绿色灯呼吸3次   
  2：从暗到亮在到暗为1次       
  3：每次用时2秒，共6秒
*/
void RGB_PowerOn_Show(void)
{

    static u16 RGB_Effect_timer=RGB_effect_breathe1_speed;
    
    static u8 Color_R_value=0;
    static u8 Color_G_value=0;
    static u8 Color_B_value=0;
    static u8 dir=0;
    
    RGB_Effect_timer++;
    if(RGB_Effect_timer>=RGB_effect_breathe1_speed)
    {
        RGB_Effect_timer=0;

        if(dir==0)
        {
            if(Color_G_value < 100)
            {
               Color_G_value++;
		    }

			if(Color_G_value >= 100)
			{
                Color_G_value = 100;
				dir=1;
			}
            LEDS_RGB_R_IO_L;
			LEDS_RGB_G_IO_H;
            LEDS_RGB_B_IO_L; 
        }
		else
		{
            
			if(Color_G_value > 0)
			{
                Color_G_value--;
			}

			if(Color_G_value==0)
			{
                dir=0;
			}
			LEDS_RGB_R_IO_L;
			LEDS_RGB_G_IO_L;
            LEDS_RGB_B_IO_L;
		}
       
        
        RGB_R_Grade = 0;
        RGB_G_Grade = Color_G_value;
        RGB_B_Grade = 0;
    }
}

//关机
void RGB_PowerOff_Show(void)
{

    RGB_R_Grade = 0;
    RGB_G_Grade = 0;
    RGB_B_Grade = 0;

	LEDS_RGB_R_IO_L;
	LEDS_RGB_G_IO_L;
    LEDS_RGB_B_IO_L;
}

//蓝牙连接  黄颜色常亮
void RGB_Bt_Con_Show(void)
{

    RGB_R_Grade = 100;
    RGB_G_Grade = 100;
    RGB_B_Grade = 0;

	LEDS_RGB_R_IO_H;
	LEDS_RGB_G_IO_H;
    LEDS_RGB_B_IO_L;

}

//蓝牙断开  黄颜色常亮
void RGB_Bt_Dis_Show(void)
{

    RGB_R_Grade = 100;
    RGB_G_Grade = 100;
    RGB_B_Grade = 0;

	LEDS_RGB_R_IO_H;
	LEDS_RGB_G_IO_H;
    LEDS_RGB_B_IO_L;

}

//蓝牙播放
/*void RGB_Bt_Play_Show(void)
{

    static uint8_t r_duty = 0;
	static uint8_t g_duty = 0;
	static uint8_t b_duty = 0;
    static uint8_t color = 0;
	static bool breathe_dir = 0; // 0 从灭到亮  1 从亮到灭
    static u16 RGB_Effect_timer=RGB_effect_breathe1_speed;


	RGB_Effect_timer++;
	if(RGB_Effect_timer >= RGB_effect_breathe1_speed)
	{
	   RGB_Effect_timer=0;
        switch(color)
		{
		  case 0://红
	             
	             if(breathe_dir==0)
	             {
	                  r_duty++;
					  if(r_duty >= 100)
					  {
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(r_duty > 0)
					 {
	                     r_duty--;
					 }

					 if(r_duty == 0)
					 {
	                       color = 1;
						   breathe_dir = 0;
					 }
					 	
				 }

				 g_duty=0;
				 b_duty=0;
		   
			break;
				
		case 1://橙色 
		    
	             if(breathe_dir==0)
	             {
	                  r_duty++;
	                  g_duty++;
					  if(g_duty >= 50)
					  {
					     g_duty=50;
					  }
					  if(r_duty >= 100)
					  {
					     g_duty=100;
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(r_duty > 0 || g_duty > 0)
					 {
					     r_duty--;
	                     g_duty--;
					 }

					 if(r_duty ==0&&g_duty == 0)
					 {
	                       color = 2;
						   breathe_dir = 0;
					 }
					 	
				 }

				 b_duty=0;
		
			break;

		case 2: //黄
		    
	             if(breathe_dir==0)
	             {
	                  r_duty++;
					  g_duty++;
					  if(r_duty >= 100)
					  {
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(r_duty > 0)
					 {
	                     r_duty--;
					 }
					 if(g_duty > 0)
					 {
	                     g_duty--;
					 }

					 if(r_duty == 0&&g_duty==0)
					 {
	                       color = 3;
						   breathe_dir = 0;
					 }
					 	
				 }

				 b_duty=0;
		
			break;
			
		case 3: //绿
		        if(breathe_dir==0)
	             {
					  g_duty++;
					  if(g_duty >= 100)
					  {
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(g_duty > 0 )
					 {
						 g_duty--;
					 }

					 if(g_duty == 0)
					 {
	                       color = 4;
						   breathe_dir = 0;
					 }
					 	
				 }
				 
	             r_duty=0;
				 b_duty=0;
		
			break;

		case 4: //青
			
			    if(breathe_dir==0)
	             {
	                  g_duty++;
					  b_duty++;
					  if(g_duty >= 100||b_duty>100) 
					  {
					     g_duty=100;
						 b_duty=100;
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(g_duty > 0 && b_duty > 0)
					 {
	                     g_duty--;
						 b_duty--;
					 }

					 if(g_duty == 0 || b_duty == 0)
					 {
	                       color = 5;
						   breathe_dir = 0;
					 }
					 	
				 }

				 r_duty=0;

				 break;
		case 5: //蓝
			
		        if(breathe_dir==0)
	             {
					  b_duty++;
					  if(b_duty >= 100)
					  {
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(b_duty > 0 )
					 {
						 b_duty--;
					 }

					 if(b_duty == 0)
					 {
	                       color = 6;
						   breathe_dir = 0;
					 }
					 	
				 }

				 r_duty=0;
	             g_duty=0;
				 
			break;
			
		case 6:///紫色
			
		        if(breathe_dir==0)
	             {
	                  r_duty++;
					  b_duty++;
					  if(r_duty >= 100 ||b_duty >= 100)
					  {
	                     breathe_dir = 1;
					  }
				 }
				 else
				 {
	                 
					 if(r_duty > 0 &&  b_duty > 0)
					 {
					     r_duty--;
						 b_duty--;
					 }

					 if(r_duty == 0 || b_duty == 0)
					 {
	                       color = 0;
						   breathe_dir = 0;
					 }
					 	
				 }

				 g_duty=0;
				 
			break;
			
		    default:
		    
		        break;
				
		}
	}
	


    RGB_R_Grade = r_duty;
    RGB_G_Grade = g_duty;
    RGB_B_Grade = b_duty;
	

}*/
void RGB_Bt_Play_Show(void)
{

    static uint8_t r_duty = 0;
	static uint8_t g_duty = 0;
	static uint8_t b_duty = 0;
    static uint8_t color = 0;
	static bool breathe_dir = 0; // 0 从灭到亮  1 从亮到灭
    static u16 RGB_Effect_timer=RGB_effect_breathe1_speed;
    static u8 LV=0;
	static u16 speed=0;

	
    LV=Get_DAC_Energy_LV_7();

	if(LV==7) speed=20;
	else if(LV==6) speed=40;
	else if(LV==5) speed=60;
	else if(LV==4) speed=80;
	else if(LV==3) speed=100;
	else if(LV==2) speed=120;
	else if(LV==1) speed=140;
	else if(LV==0) speed=300;

	RGB_Effect_timer++;
	if(RGB_Effect_timer >= speed)
	{
	   RGB_Effect_timer=0;
	   color++;
	   if(color>6) color=0;
        switch(color)
		{
		  case 0://红
	             r_duty=100;
				 g_duty=0;
				 b_duty=0;
		   
			break;
				
		case 1:
		    
	             r_duty=100;
	             g_duty=100;
				 b_duty=100;
		
			break;

		case 2: //黄
		    
	              r_duty=100;
				  g_duty=100;
				  b_duty=0;
		
			break;
			
		case 3: //绿
		      
				 g_duty=100;
	             r_duty=0;
				 b_duty=0;
			break;

		case 4: //青
			
			    g_duty=100;
		        b_duty=100;
				r_duty=0;
		    break;
			
		case 5: //蓝
			
			    b_duty=100;
				r_duty=0;
	            g_duty=0;
			break;
			
		case 6:///紫色
	             r_duty=100;
			     b_duty=100;
				 g_duty=0;
				 
			break;
			
		    default:
		    
		        break;
				
		}
	}
	

    RGB_R_Grade = r_duty;
    RGB_G_Grade = g_duty;
    RGB_B_Grade = b_duty;

	if(r_duty==100)
	{
	   LEDS_RGB_R_IO_H;
	}
	else
	{  
	   LEDS_RGB_R_IO_L;
	}
	
	if(g_duty==100)
	{
		LEDS_RGB_G_IO_H;  
	}
	else 
	{
	    LEDS_RGB_G_IO_L;
	}
	
	if(b_duty==100) 
	{
		LEDS_RGB_B_IO_H; 
	}
	else 
	{ 
	   LEDS_RGB_B_IO_L;
	}
		

}

//蓝牙暂停  蓝色灯常亮
void RGB_Bt_Pause_Show(void)
{

    RGB_R_Grade = 0;
    RGB_G_Grade = 0;
    RGB_B_Grade = 100;

	LEDS_RGB_R_IO_L;
	LEDS_RGB_G_IO_L;
    LEDS_RGB_B_IO_H;

}



/*
  来电\接通状态

  挂断,恢复接通之前工作状态
*/
void RGB_HFP_CALL_IN_Show(void)
{

    RGB_Bt_Play_Show();

}



/*
  低电状态
*/
void RGB_LowPower_Show(void)
{
   static u16 _2ms_timer=0;
   static u8 cnt=0;

   
   if(Flag_low_power)
   {
       _2ms_timer++;
	   if(_2ms_timer < 150)
	   {
            RGB_R_Grade = 100;
			LEDS_RGB_R_IO_H;
	   }
	   else  if(_2ms_timer < 300)
	   {
            RGB_R_Grade = 0;
			LEDS_RGB_R_IO_L;
	   }
	   else
	   {
	       _2ms_timer=0;
		   cnt++;
	   }

	   RGB_G_Grade=0;
	   RGB_B_Grade=0;

	  LEDS_RGB_G_IO_L;
      LEDS_RGB_B_IO_L;
	   if(cnt >= 2)
	   {
           cnt=0;
		   _2ms_timer=0;
           RGB_curr_effect=Temp_RGB_curr_effect;
	   }
   }
   else
   {
      cnt=0;
   }

  


}

/*
  开机充电
*/
void RGB_PowerOn_Charge_Show(void)
{
    
   static u16 _2ms_timer=0;
   static u8 cnt=0;
   
   if(IsInCharge())
   {
       _2ms_timer++;
	   if(_2ms_timer < 250)
	   {
            RGB_R_Grade = 100;
			LEDS_RGB_R_IO_H;
	   }
	   else  if(_2ms_timer < 500)
	   {
            RGB_R_Grade = 0;
			LEDS_RGB_R_IO_L;
	   }
	   else
	   {
	       _2ms_timer=0;
		   cnt++;
	   }

	   RGB_G_Grade=0;
	   RGB_B_Grade=0;
	   LEDS_RGB_G_IO_L;
	   LEDS_RGB_B_IO_L;
	   if(cnt >= 3)
	   {
           cnt=0;
		   _2ms_timer=0;
           RGB_curr_effect=Temp_RGB_curr_effect;
	   }
   }
   else
   {
      cnt=0;
   }

}

/*
  关机充电
*/
void RGB_PowerOff_Charge_Show(void)
{

    static uint8_t r_duty = 0;
	static bool breathe_dir = 0; // 0 从灭到亮  1 从亮到灭
    static u16 RGB_Effect_timer=RGB_effect_breathe1_speed;
	
#if 0
      //待机模式                      插入充电         未充满
   if(Idle_sw.idle_mode == on_line && IsInCharge() && PowerLevelGet() < PWR_FULL)
   {

         RGB_Effect_timer++;

		 if(RGB_Effect_timer >= RGB_effect_breathe1_speed)
		 {
		     RGB_Effect_timer=0;
			 
             if(breathe_dir==0)
             {
                  r_duty++;
				  if(r_duty >= 100)
				  {
                     breathe_dir = 1;
				  }
			 }
			 else
			 {
                 
				 if(r_duty > 0)
				 {
                     r_duty--;
				 }
				 if(r_duty == 0)
				 {
					   breathe_dir = 0;
				 }
				 	
			 }

		 RGB_R_Grade=r_duty;
		 RGB_G_Grade=0;
		 RGB_B_Grade=0;
		 }
             
   }
   else if(Idle_sw.idle_mode == on_line && IsInCharge() && PowerLevelGet() == PWR_FULL)
   {
         RGB_R_Grade=0;
		 RGB_G_Grade=100;
		 RGB_B_Grade=0;
   }

 #else
   if(Idle_sw.idle_mode == on_line && IsInCharge() && PowerLevelGet() < PWR_FULL)
   {
		 RGB_R_Grade=100;
		 RGB_G_Grade=0;
		 RGB_B_Grade=0;
		 LEDS_RGB_R_IO_H;
		 LEDS_RGB_G_IO_L;
		 LEDS_RGB_B_IO_L;
   }
   else if(Idle_sw.idle_mode == on_line && IsInCharge() && PowerLevelGet() == PWR_FULL)
   {
         RGB_R_Grade=0;
		 RGB_G_Grade=100;
		 RGB_B_Grade=0;
		 LEDS_RGB_R_IO_L;
		 LEDS_RGB_G_IO_H;
		 LEDS_RGB_B_IO_L;
   }
 
 #endif

 
	 
}

/*
  TWS   黄颜色灯常亮
*/
void RGB_TWS_Show(void)
{

    RGB_R_Grade=100;
    RGB_G_Grade=100;
	RGB_B_Grade=0;
	LEDS_RGB_R_IO_H;
	LEDS_RGB_G_IO_H;
    LEDS_RGB_B_IO_L;

}



/*
   设置能量等级,分7级
*/
u8 Get_DAC_Energy_LV_7(void)
{
    static u32 DAC_Energy=0;
	static u8 Energy_LV=0;
	
    DAC_Energy = Get_Music_Energy();
// printf("DAC_Energy == %d\n",DAC_Energy);

	if(DAC_Energy > 2300)
	{
        Energy_LV = 7;
	}
	else if(DAC_Energy > 1700)
	{
        Energy_LV = 6;
	}
	else if(DAC_Energy > 1000)
	{
        Energy_LV = 5;
	}
	else if(DAC_Energy > 400)
	{
        Energy_LV = 4;
	}
	else if(DAC_Energy > 100)
	{
        Energy_LV = 3;
	}
	else if(DAC_Energy > 10)
	{
        Energy_LV = 2;
	}
	else if(DAC_Energy > 0)
	{
        Energy_LV = 1;
	}
    else 
	{
        Energy_LV = 0;
	}

   return Energy_LV;
	

}


void RGB_Timer_ON()
{
	if(!rgb_timer_state)
	{
		Timer_Pause(TIMER5,0);
		rgb_timer_state=1;
	}
    
}
void RGB_Timer_OFF()
{
	if(rgb_timer_state)
	{
		Timer_Pause(TIMER5,1);
		rgb_timer_state=0;
	}
}

#if 0//LEDS_mix_RGB_EN
void Timer5Interrupt(void)
{
	Timer_InterruptFlagClear(TIMER5, UPDATE_INTERRUPT_SRC);

	/*
	static u16 ____1s=0;
	____1s++;
    if(____1s==1000)
    {
        ____1s=0;
        DBG(" _1s_: %d \n", 1);
    }*/

#if 0//def CFG_DMA_RGB_LED_EN
extern volatile uint8_t DmaLedDataFlag;
       if(DmaLedDataFlag)
       {
           return;
	   }
#endif 
	
    #if LEDS_mix_RGB_EN
	    RGB_Contral_R();
	    RGB_Contral_G();
	    RGB_Contral_B();
    #endif

}
#endif

#if LED1_EN
void Timer5Interrupt(void)
{
	Timer_InterruptFlagClear(TIMER5, UPDATE_INTERRUPT_SRC);

	if(Machine_state==Machine_init||Machine_state==Machine_poweron_tone)
    { 
        if(Idle_sw.idle_mode == off_line)
	       LED_Breathe_Contral();
	}

}                
#endif


void User_Set_Music_Energy(u16 Music_Energy)
{
   
     Get_Curr_Music_energy = Music_Energy;
	
}

                   
                   

//IO_contral_init

//********************************************** iic


#define IIC_CLK_O               {IIC_CLK_IO_O;IIC_CLK_IO_UH;IIC_CLK_IO_DL;}
#define IIC_CLK_H               {IIC_CLK_IO_OH;}
#define IIC_CLK_L               {IIC_CLK_IO_OL;}

#define IIC_DAT_O               {IIC_DAT_IO_OUT;IIC_DAT_IO_PU1;}
#define IIC_DAT_I               {IIC_DAT_IO_IN ;IIC_DAT_IO_PU1;}
#define IIC_DAT_R               (IIC_DAT_IO_R)
#define IIC_DAT_H               {IIC_DAT_IO_OUT1;}
#define IIC_DAT_L               {IIC_DAT_IO_OUT0;}


#define iic2_data_out()     IIC_DAT_O
#define iic2_data_in()      IIC_DAT_I
#define iic2_data_r()       IIC_DAT_R
#define iic2_data_h()       IIC_DAT_H
#define iic2_data_l()       IIC_DAT_L

#define iic2_clk_out()      IIC_CLK_O
#define iic2_clk_h()        IIC_CLK_H
#define iic2_clk_l()        IIC_CLK_L


#define iic2_delay()      __udelay(5)


 /*--------------------------------------------------------------------------*/
 /**@file	  iic_io.c
	@brief	  IO模拟的IIC的驱动
	@details
	@author
	@date	2011-3-7
	@note
 */
 /*----------------------------------------------------------------------------*/
 /**@brief	 IIC的IO口初始化
	@param	 无
	@return  无
	@note	 void iic_init_io(void)
 */
 /*----------------------------------------------------------------------------*/
 void IIC_Init(void)
 {
	 iic2_data_h();
	 iic2_data_out();		  //SDA设置成输出
	 iic2_clk_h();
	 iic2_clk_out();			 //SCL设置成输出
 }

 /*----------------------------------------------------------------------------*/
 /**@brief	 读取ACK
	@param	 无
	@return  都会的ACK/NACK的电平
	@note	 bool r_ack(void)
 */
 /*----------------------------------------------------------------------------*/
 void IIC_Start(void)
 {
	 //iic2_init_io();
	 iic2_data_h();
	 iic2_clk_h();
	 iic2_delay();
	 iic2_data_l();
	 iic2_delay();
	 iic2_clk_l();
	 iic2_delay();
 }

 /*----------------------------------------------------------------------------*/
 /**@brief	 STOP IIC
	@param	 无
	@return  无
	@note	 void iic_stop(void)
 */
 /*----------------------------------------------------------------------------*/
 void IIC_Stop(void)
 {
	 //iic2_data_out();
	 iic2_data_l();
	 iic2_delay();
	 iic2_clk_h();
	 iic2_delay();
	 iic2_data_h();
	 iic2_delay();

 }

 /*----------------------------------------------------------------------------*/
 /**@brief	 读取ACK
	@param	 无
	@return  都会的ACK/NACK的电平
	@note	 bool r_ack(void)
 */
 /*----------------------------------------------------------------------------*/
 u8 IIC_Wait_Ack(void)
 {
	 u8 nack;
	 iic2_data_in();
	 iic2_delay();
	 iic2_clk_h();
	 iic2_delay();
	 iic2_delay();
	 iic2_delay();
	 iic2_delay();
	 iic2_delay();
	 nack = iic2_data_r();
	 iic2_clk_l();
	 iic2_delay();
	 iic2_data_out();
	 return nack;
 }

 /*----------------------------------------------------------------------------*/
 /**@brief	 发送一个ACK信号的数据,
	@param	 flag ：发送的ACK/nack的类型
	@return  无
	@note	 void s_ack(u8 flag)
 */
 /*----------------------------------------------------------------------------*/
 void IIC_Ack(u8 flag)
 {
	 //iic2_data_out();
	 iic2_clk_l();
	 if(flag)
	 {
		 iic2_data_h();
	 }
	 else
	 {
		 iic2_data_l();
	 }
	 iic2_delay();
	 iic2_clk_h();
	 iic2_delay();
	 iic2_clk_l();
 }
 /*----------------------------------------------------------------------------*/
 /**@brief	 从IIC总线接收一个BYTE的数据,
	@param	 无
	@return  读取回的数据
	@note	 u8 iic_revbyte_io( void )
 */
 /*----------------------------------------------------------------------------*/
 u8 iic2_revbyte_io( void )
 {
	 u8 byteI2C = 0;
	 u8 i;
	 iic2_data_in();
	 iic2_delay();
	 for (i = 0;i < 8;i++)
	 {
		 iic2_clk_h();
		 byteI2C <<= 1;
		 iic2_delay();
		 if (iic2_data_r())
		 {
			 byteI2C++;
		 }
		 iic2_clk_l();
		 iic2_delay();
	 }
	 iic2_data_out();
	 return byteI2C;
 }
 /*----------------------------------------------------------------------------*/
 /**@brief	 从IIC总线接收一个BYTE的数据,并发送一个指定的ACK
	@param	 para ：发送ACK 还是 NACK
	@return  读取回的数据
	@note	 u8 iic_revbyte( u8 para )
 */
 /*----------------------------------------------------------------------------*/
 u8 IIC_Read_Byte( u8 para )
 {
	 u8 byte;
	 byte = iic2_revbyte_io();
	 IIC_Ack(para);
	 return byte;
 }
 /*----------------------------------------------------------------------------*/
 /**@brief	 向IIC总线发送一个BYTE的数据
	@param	 byte ：要写的EEROM的地址
	@return  无
	@note	 void iic_sendbyte_io(u8 byte)
 */
 /*----------------------------------------------------------------------------*/
void iic2_sendbyte_io(u8 byte)
{
    u8 i;
    //iic2_data_out();
    iic2_delay();
    for (i = 0;i < 8;i++)
    {
        if (byte & BIT(7))
        {
            iic2_data_h(); 			   //最高位是否为1,为1则SDA= 1,否则 SDA=0
        }
        else
        {
            iic2_data_l();
        }
        iic2_delay();
        iic2_clk_h();
        iic2_delay();
        byte <<= 1;				   //数据左移一位,进入下一轮送数
        iic2_clk_l();
        //iic2_delay();
    }
}
 /*----------------------------------------------------------------------------*/
 /**@brief	 向IIC总线发送一个BYTE的数据,并读取ACK
	@param	 byte ：要写的EEROM的地址
	@return  无
	@note	 void iic_sendbyte(u8 byte)
 */
 /*----------------------------------------------------------------------------*/
void IIC_Send_Byte(u8 byte)
{
    iic2_sendbyte_io(byte);
    IIC_Wait_Ack();
}

void IIC_send_1byte(u8 chip_address,u8 r_address,u8 data)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(chip_address);    //发送器件地址
    IIC_Send_Byte(r_address);       //发送地址
	IIC_Send_Byte(data);            //发送字节							   
    IIC_Stop();                     
}
u8 IIC_read_1byte(u8 chip_address,u8 r_address)
{
    u8 temp=0;		  	    																 
    IIC_Start();  
	IIC_Send_Byte(chip_address);   //发送器件地址

    IIC_Send_Byte(r_address);   //发送地址
	IIC_Start();  	 	   
	IIC_Send_Byte(chip_address+1);           
    temp=IIC_Read_Byte(1);		   
    IIC_Stop();
	return temp;
}
u16 iic_read_16(u8 LV_slave_add,u8 LV_address)
{
    u8 cnt=0;
    u8 LV_data_H;
    u8 LV_data_L;


    IIC_Start();				  //I2C启动
    IIC_Send_Byte(LV_slave_add+0);		  //写命令
    IIC_Send_Byte(LV_address);		  //写命令
    IIC_Start();
    IIC_Send_Byte(LV_slave_add+1);		  //写命令

    LV_data_H = IIC_Read_Byte(0);
    LV_data_L = IIC_Read_Byte(1);

    IIC_Stop();				  //I2C停止时序
    return ((LV_data_H<<8)+LV_data_L);
}
void iic_write_16(u8 LV_slave_add,u8 LV_address,u16 LV_data)
{
    u8 cnt=0;
    u8 LV_data_H = (LV_data&0xff00)>>8;
    u8 LV_data_L = LV_data&0x00ff;


    IIC_Start();				  //I2C启动
    IIC_Send_Byte(LV_slave_add+0);		  //写命令

    //for(;cnt<3;cnt++)
    {
        IIC_Send_Byte(LV_address);	   //写数据
        IIC_Send_Byte(LV_data_H);	   //写数据
        IIC_Send_Byte(LV_data_L);	   //写数据

    }
    IIC_Stop();				  //I2C停止时序
}


#if KT0641_EN
u8 KT0641_work_mode;
u8 KT0641_grade=1;


u8 temp_RF_POW_SW; //发射功率状态临时存储

u32 Memery_Frequency=BAND_BOTTOM;
u8 Key_RF_POW_flag;


UINT16 KT_Bus_Read(UINT8 Register_Address)
{
    return iic_read_16(KT0641_address,Register_Address);
}
void KT_Bus_Write(UINT8 Register_Address, UINT16 Word_Data)
{
    iic_write_16(KT0641_address,Register_Address,Word_Data);
}




u8 KT_WirelessMicTx_PreInit(void)              
{
    UINT16 regx;
    UINT8 i;

    for (i = 0; i < INIT_FAIL_TH; i++)
    {
        regx = KT_Bus_Read(0x01); //Read Manufactory ID 
        Z_unified_delay_ms(10);
          if (regx == 0x4B54)
            return(1);
    }
    return(0);
}
u8 KT_WirelessMicTx_Init(void)
{
    UINT16 regx;
	Z_unified_delay_ms(50);
//	regx=KT_Bus_Read(0x0a);
//    KT_Bus_Write(0x0a,regx|0x1000); //recali
//	regx = KT_Bus_Read(0x03);
//	KT_Bus_Write(0x03, regx | (STANDBY << 15));   	//Write Standby bit to 1
//    Delay_ms(500);
//    regx = KT_Bus_Read(0x03);
//    KT_Bus_Write(0x03, (regx & 0x7FFF) | (WAKEUP << 15)); //Write Standby bit to 0
//    Delay_ms(500);
//	regx = KT_Bus_Read(0x1e);
//	KT_Bus_Write(0x1e,regx|0x0010);
//	Delay_ms(5);
//	regx = KT_Bus_Read(0x1e);
//	KT_Bus_Write(0x1e,regx&~0x0010);
//	Delay_ms(100);

    regx = KT_Bus_Read(0x03);
    KT_Bus_Write(0x03, (regx & 0xfffe) | PA_SEL);

    regx = KT_Bus_Read(0x24);
    KT_Bus_Write(0x24, (regx & 0xEFFF) | (BATTERY_METER_ENABLE << 12));

    //ref_vtr_vth_sel = 1
    regx=KT_Bus_Read(0x47);                        
    KT_Bus_Write(0x47,(regx | 0x0200));

    //vref_mon_en=1
    regx = KT_Bus_Read(0x0a);
    KT_Bus_Write(0x0a, regx | 0x0200);            

    regx = KT_Bus_Read(0x1C);
    KT_Bus_Write(0x1C, (regx & 0xF1E1) | ( FDEV_MONITOR_TC_250ms << 10 ) | (COMPANDOR_ENABLE<<4) | 
                 ( PRE_EMPHASIS_ENABLE << 9 ) | ( COMPANDOR_TC_12ms << 1 ));

    KT_WirelessMicTx_Mic_Sens(MIC_SENS_GAIN_9);        

    //cic overflow detect enable
    regx = KT_Bus_Read(0x1e); 
    KT_Bus_Write(0x1e, (regx & 0xffdf)|(1<<5)); 

    regx=KT_Bus_Read(0x30);
    KT_Bus_Write(0x30, ( regx & 0x8000 ) | (AGC_VHIGH << 11) | (AGC_VLOW << 7) | (AGC_ATTACK << 4) | 
                 (AGC_RELEASE << 1) | AGC_DIS );
 
    regx=KT_Bus_Read(0x31);
    KT_Bus_Write( 0x31, ( regx & 0xff00 ) | (GAIN_SEL << 6) | (COMPEN_GAIN << 4) | 
                  (BLANK_EN << 3) | BLANK_TIME );

    regx=KT_Bus_Read(0x39);
    KT_Bus_Write(0x39,(regx & 0xFC00) | (HARD_LIMIT << 6) | (CPRS_1XLPF_BP << 5) | 
                 (CPRS_KNEE_DIS << 4) | CPRS_THRSH);

    KT_Bus_Write( 0x3a,(ALC_DIS << 15) | (ALC_SOFTKNEE << 14) | (ALC_VMAX << 7) | 
                  (ALC_ATTACK << 4) | ALC_RELEASE );

	#ifdef SILENCE_MUTE
    regx=KT_Bus_Read(0x25);
    KT_Bus_Write( 0x25,(regx&0xc000)|(SLNC_MUTE_DIS << 13) | (SLNC_MUTE_TIME << 8) | 
                 (SLNC_MUTE_LOW_LEVEL << 4) | SLNC_MUTE_HIGH_LEVEL );
    regx=KT_Bus_Read(0x26);
    KT_Bus_Write( 0x26,(regx&0xfffb)|(SILENCE_MUTE_ACT_MCU << 2));
	#endif

    #ifdef AUX_CH
        regx=KT_Bus_Read(0x1f);
        KT_Bus_Write(0x1f,(regx&0x80f0)|(AUXDATA_EN << 14)|(AUX_REG_NUM << 12) |
                     (AUX_CARRY_NUM << 9)|BPSK_NEW_MODE); 
        KT_Bus_Write(0x20,(AUX_ADDRB << 8) | AUX_ADDRA);
        KT_Bus_Write(0x21,(AUX_ADDRD << 8) | AUX_ADDRC);    
    #endif

    #ifdef OTHER_RX
        #ifdef XTAL_24M_ONLY
        KT_WirelessMicTx_Set_Pilot_Freq(XTAL_24M_FREQ); //set pilot frequency
        #endif
        #ifdef XTAL_24P576M_ONLY
        KT_WirelessMicTx_Set_Pilot_Freq(XTAL_24P576M_FREQ); //set pilot frequency
        #endif
    #endif

    //for VCO unlock because of temperature change
    regx=KT_Bus_Read(0x10);
    KT_Bus_Write(0x10,(regx|0x0010)); //vco_ldo_calicode=1.4v
    regx=KT_Bus_Read(0x45);
    KT_Bus_Write(0x45,((regx&0xfff0)|0x0007));

    regx=KT_Bus_Read(0x2d);
    KT_Bus_Write(0x2d,(regx&0xfff8)|3); //lofine_vref_sel=0.4v

    #ifdef pll_unlock 
        regx = KT_Bus_Read(0x2f);                
        KT_Bus_Write(0x2f, regx|0x0080); //pll_unlock_en
        regx = KT_Bus_Read(0x44);                
        KT_Bus_Write(0x44, (regx&0x1fff)|(1<<13)); //lo_lock_hth_vsel
        regx = KT_Bus_Read(0x44);                
        KT_Bus_Write(0x44, (regx&0xe3ff)|(3<<10)); //lo_lock_lth_vsel
        regx = KT_Bus_Read(0x44);                
        KT_Bus_Write(0x44, regx&0xfdff); //lo_lock_det_pd
    #endif

    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Standby                                                 
//功能描述：待机程序                                                                
//函数说明：                                                                        
//全局变量：                                                                        
//输    入：无                                                                        
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_Standby(void)
{
    KT_WirelessMicTx_PowerDownProcedure();
    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_WakeUp                                                     
//功能描述：唤醒程序                                                                
//函数说明：脱离待机模式                                                        
//全局变量：                                                                        
//输    入：无                                                                        
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_WakeUp(void)
{
    UINT16 reg3;
    reg3 = KT_Bus_Read(0x03);
    KT_Bus_Write(0x03, (reg3 & 0x7FFF) | (WAKEUP << 15)); //Write Standby bit to 0
    Z_unified_delay_ms(50);
    KT_WirelessMicTx_Init();
	KT_WirelessMicTx_MuteSel(AUDIO_UNMUTE);             
//  wakeUp以后需要做一些tune台及设置音效和设置PA等工作，可参考main.c里面的KT_MicTX_Init函数
    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_PASW                                                     
//功能描述：PA打开、关闭程序                                                        
//函数说明：用来控制芯片打开或者关闭PA                                                
//全局变量：                                                                        
//输    入：bPA_Switch                                                                
//返    回：正确：1                 错误：0                                            
//设 计 者：Zhou Dongfeng           时间：2016-04-05                                         
//修 改 者：Zhou Dongfeng           时间：2016-08-26                                         
//版    本：V0.4                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_PASW(u8 bPA_Switch)
{
    UINT16 regF;

    regF = KT_Bus_Read(0x0F);
    KT_Bus_Write(0x0F, (regF & 0xFFF7) | ( (UINT8)bPA_Switch << 3 ));
    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_PAGain                                                     
//功能描述：PA增益调整                                                                
//函数说明：cPaGain范围为0-63，共64档                                    
//全局变量：                                                                        
//输    入：cPaGain                                                                    
//返    回：正确：1                 错误：0                                            
//设 计 者：Zhou Dongfeng           时间：2016-04-05                                         
//修 改 者：                        时间：                                         
//版    本：V0.1                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_PAGain(UINT8 cPaGain)
{
    UINT16 reg11;

    reg11 = KT_Bus_Read(0x11);
    KT_Bus_Write(0x11, (reg11 & 0xFF00) | cPaGain );        

    return(1);    
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Fdev_Monitor                                             
//功能描述：实时频偏读取程序                                                        
//函数说明：读取当前状态的频偏大小                                                    
//全局变量：                                                                        
//输    入：无                                                                        
//返    回：(reg1C & 0xF000) >> 12 （为0-15的整数）                                    
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_Fdev_Monitor(void)
{
    UINT16 reg1C;

    reg1C = KT_Bus_Read(0x1C);

    return( (reg1C & 0xF000) >> 12 );
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Mic_Sens                                                 
//功能描述：Mic灵敏度调整                                                            
//函数说明：cMicSens为0-15，共16档                                                
//全局变量：                                                                        
//输    入：cMicSens                                                                
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_Mic_Sens(UINT8 cMicSens)
{
    UINT16 reg1C;

    reg1C = KT_Bus_Read(0x1C);
    KT_Bus_Write(0x1C, (reg1C & 0xFE1F) | ( (UINT16)cMicSens << 5 ));

    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Comp_Dis                                                 
//功能描述：压扩功能打开、关闭程序                                                    
//函数说明：用来控制芯片打开或者关闭压扩功能                                        
//全局变量：                                                                        
//输    入：bComp_Dis                                                                
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
//BOOL KT_WirelessMicTx_Comp_Dis(BOOL bComp_Dis)                            
//{
//    UINT16 reg1C;
//
//    reg1C = KT_Bus_Read(0x1C);
//    KT_Bus_Write(0x1C, (reg1C & 0xFFEF) | ( (UINT8)bComp_Dis << 4 ));
//
//    return(1);
//}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_MuteSel                                                 
//功能描述：静音功能打开、关闭程序                                                    
//函数说明：用来选择打开或者关闭静音功能                                            
//全局变量：                                                                        
//输    入：bMute_Sel                                                                
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_MuteSel(u8 bMute_Sel)
{
    UINT16 reg1C;

    reg1C = KT_Bus_Read(0x1C);
    KT_Bus_Write(0x1C, (reg1C & 0xFFFE) | bMute_Sel); //Write Mute bit

    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Pilot                                                     
//功能描述：32.768KHz导频信号打开、关闭程序                                            
//函数说明：用来选择是否使用32.768KHz导频信号                                        
//全局变量：                                                                        
//输    入：bPilot_Dis                                                                
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_Pilot(u8 bPilot_Dis)
{
    UINT16 reg1F;

    reg1F = KT_Bus_Read(0x1F);
#ifdef OTHER_RX
    KT_Bus_Write(0x1F, (reg1F & 0xFFBF) | ( (UINT8)bPilot_Dis << 6 ));
#endif
#ifdef KT_RX
    KT_Bus_Write(0x1F, (reg1F & 0x7FFF) | ( (UINT16)bPilot_Dis << 15 ));
#endif
    Z_unified_delay_ms(100);

    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Pilot_Fdev                                                 
//功能描述：32.768KHz导频信号调制频偏大小设置程序                                    
//函数说明：cPilot_Fdev围为0-3，共4档                                                
//全局变量：                                                                        
//输    入：cPilot_Fdev                                                                
//返    回：正确：1                 错误：0                                            
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_Pilot_Fdev(UINT8 cPilot_Fdev)
{
    UINT16 reg1F;

    reg1F = KT_Bus_Read(0x1F);
#ifdef OTHER_RX
    KT_Bus_Write(0x1F, (reg1F & 0xFFCF) | (cPilot_Fdev << 4));
#endif
#ifdef KT_RX
    KT_Bus_Write(0x1F, (reg1F & 0xFE7F) | ((UINT16)cPilot_Fdev << 7));
#endif
    Z_unified_delay_ms(20);

    return(1);
}

#ifdef OTHER_RX
//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Set_Pilot_Freq                                             
//功能描述：设置导频频率                                                            
//函数说明：设置导频频率                                                
//全局变量：                                                                        
//输    入：xtal_sel: 0 or 1                                                        
//返    回：成功：1； 失败：0                                                 
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：                        时间：                                         
//版    本：V1.0                                                                     
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_Set_Pilot_Freq(u8 bXtal_Sel)
{
    UINT16 regx;
    if (bXtal_Sel == XTAL_24M_FREQ)
    {
        KT_Bus_Write(0x2C, ((PILOT_FREQ << 13) / 9375));
    }
    else
    {
        KT_Bus_Write(0x2C, ((PILOT_FREQ << 13) / 9600));
    }
    return(1);
}
#endif

#ifdef RXISKT0616M_XTAL_DUAL
//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Calc_ChanReg                                    
//功能描述：晶体的频率控制字计算                                                
//函数说明：输入以KHz为单位的VCO震荡频率;                                            
//            计算结果存在*chan_ptr,*chan_frac_ptr,*chan_frac_msb_ptr中                
//全局变量：                                                                        
//输    入：Freq （输入以KHz为单位的VCO频率）                                        
//返    回：正确：1    错误：0                                                            
//设 计 者：YANG Pei                时间：2012-04-19                            
//修 改 者：KANG Hekai              时间：2013-03-29                            
//版    本：V2.0                                                                    
//          V2.5 修改余数<40或大于xtal-40的bug                                                                    
//-----------------------------------------------------------------------------
u8 KT_WirelessMicTx_Calc_ChanReg_Old(INT32 Freq, UINT16 *chan_ptr, INT16 *chan_frac_ptr, 
                                       UINT8 *chan_frac_msb_ptr, UINT16 xtal_freq)
{
    *chan_ptr = Freq / xtal_freq;
    Freq = Freq % xtal_freq; 
    *chan_frac_ptr = (Freq << 16) / xtal_freq;
    if ((Freq <= 40) && (Freq >= 0))
    {
        *chan_frac_ptr = 0xffff;
        *chan_frac_msb_ptr =3;
    }
    else if ((Freq < xtal_freq ) && (Freq >= xtal_freq - 40))
    {
        (*chan_ptr)++; 
        *chan_frac_ptr = 0xffff;
        *chan_frac_msb_ptr = 3;
    }
    else if ( (Freq >= (xtal_freq / 2 - 40)) && (Freq <= (xtal_freq / 2 + 40)) )
    {
        *chan_frac_ptr = 0x7fff;
        *chan_frac_msb_ptr = 0;
    }
    else if ( Freq > (xtal_freq >> 1) )
    {
        (*chan_ptr)++; 
        *chan_frac_msb_ptr = 3;
    }
    else    
    {
        *chan_frac_msb_ptr = 0;
    }
    return(1);
}

bit selectXtalOld(INT32 Freq)
{
    UINT16 chan0,chan1;
    INT16 chan_frac0,chan_frac1;
    UINT8 chan_frac_msb0,chan_frac_msb1;
    INT16 mod0,mod1,mod2,mod3;

    Freq<<=1;
    KT_WirelessMicTx_Calc_ChanReg_Old(Freq, &chan0, &chan_frac0, &chan_frac_msb0,24000);
    KT_WirelessMicTx_Calc_ChanReg_Old(Freq, &chan1, &chan_frac1, &chan_frac_msb1,24576);
    mod0 = chan_frac0;
    mod1 = chan_frac1;
    mod2 = chan_frac0 << 1;
    mod3 = chan_frac1 << 1;
    if(mod0 < 0)
        mod0=~mod0;             //mod0=abs(mod0);
    if(mod1 < 0)
        mod1=~mod1;             //mod1=abs(mod1);
    if(mod2 < 0)
        mod2=~mod2;             //mod2=abs(mod2);
    if(mod3 < 0)
        mod3=~mod3;             //mod3=abs(mod3);
    if(mod2 < mod0)
        mod0 = mod2;
    if(mod3 < mod1)
        mod1 = mod3;
    if(mod0<mod1)
    {
        return(XTAL_24P576M_FREQ);
    }
    else 
        return(XTAL_24M_FREQ);
}
#endif

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_BatteryMeter_Read                                         
//功能描述：电池电压读取程序                                                        
//函数说明：读取当前的电池电压，返回值为0-2047的整数                                
//全局变量：                                                                        
//输    入：无                                                                        
//返    回：reg7 & 0x07FF（为0-2047的整数）                                            
//设 计 者：KANG Hekai              时间：2011-06-10                                
//修 改 者：KANG Hekai              时间：2011-06-10                                
//版    本：V1.0                                                                    
//-----------------------------------------------------------------------------
UINT16 KT_WirelessMicTx_BatteryMeter_Read(void)
{
    UINT16 reg7;

    reg7 = KT_Bus_Read(0x07);

    return( reg7 & 0x07FF );
}

//函 数 名：KT_WirelessMicTx_Calc_ChanReg                                    
//功能描述：晶体的频率控制字计算                                                
//函数说明：输入以KHz为单位的VCO震荡频率;                                            
//            计算结果存在*chan_ptr,*chan_frac_ptr,*chan_frac_msb_ptr中                
//全局变量：                                                                        
//输    入：Freq （输入以KHz为单位的VCO频率）                                        
//返    回：正确：1    错误：0                                                            
//设 计 者：Zhou Dongfeng           时间：2016-04-05                                         
//修 改 者：Zhou Dongfeng           时间：2016-09-02                                     
//版    本：V0.1    For KT0646M_VX
//            V0.2    修改了变量定义的方式
//            V0.3    修改了vco_lowth
//            V0.4    统一了变量定义格式
//-----------------------------------------------------------------------------
//lopa_div1 * lopa_div2
UINT8 const lopa_div_tab[13]=  
{
    6, 8, 10, 12,
      16, 20, 24,
      32, 40, 48,
      64, 80, 96
};

//reg0x08<bit9:8> and <bit7:6>
UINT8 const lopa_div_tab2[13][2]= 
{
     {0,0},{0,1}, {0,2}, {0,3},
           {1,1}, {1,2}, {1,3},
           {2,1}, {2,2}, {2,3},
           {3,1}, {3,2}, {3,3}
};

u8 KT_WirelessMicTx_Calc_ChanReg(INT32 Freq,  UINT8  *lopa_div1_ptr, UINT8  *lopa_div2_ptr,
                                    UINT16  *chan_ptr,  UINT16  *chan_frac_ptr, 
                                    UINT8  *chan_frac_msb_ptr, UINT32 *chan_frac_temp_ptr,INT32 xtal_freq)
{
    UINT32 chan_frac;
    INT32 Fvco;
    UINT32 temp,tempMin=xtal_freq;
//    double chan_frac_d;
//    double chan_frac_temp_d;
    UINT16 i = 12;
    UINT8 lopa_div;
//    UINT16 chan_temp;
    long vco_highth = 4320000; //VCO Range:4.32G-5.76G
    
    //lodiv 期望值
    lopa_div = vco_highth / Freq;

    //lodiv 实际值
    for(i=0; i<12; i++)
    {
        if(lopa_div < lopa_div_tab[i])
        {            
            break;                                                   
        }
    }
    //输出lopa_div1和lopa_div2
    *lopa_div1_ptr = lopa_div_tab2[i][1];
    *lopa_div2_ptr = lopa_div_tab2[i][0];
    
    //计算VCO 频率
    Fvco = Freq * lopa_div_tab[i];
    //整数
    *chan_ptr = Fvco / (xtal_freq * 7) + 1; //dll_mode : *7
    //小数
//    chan_frac_d = Fvco;
//    chan_frac_d = chan_frac_d / (xtal_freq * 7);     //31.1221
//    chan_frac_d = chan_frac_d - (*chan_ptr);
//    chan_frac_d = chan_frac_d * 65536;                 //-57532.95
//    chan_frac = chan_frac_d;
    chan_frac= (((Fvco % (xtal_freq * 7))-(xtal_freq * 7))<<12)/((xtal_freq * 7)>>4);

    *chan_frac_msb_ptr = (chan_frac & 0x00030000) >> 16 ; //reg0x08<bit1:0>
    *chan_frac_ptr = chan_frac & 0x0000ffff; //reg0x09

//    chan_temp = Freq / xtal_freq + 1;
//    chan_frac_temp_d = Freq;
//    chan_frac_temp_d = chan_frac_temp_d / xtal_freq - chan_temp;
//    *chan_frac_temp_ptr = chan_frac_temp_d;

//    *chan_frac_temp_ptr= ((xtal_freq>>1)-abs(((Freq%xtal_freq)-(xtal_freq>>1))))*(3072000/xtal_freq);
    /*
    for(i=1;i<4;i++)
    {
        temp = ((xtal_freq/(2*i))-abs(((Freq%(xtal_freq/i))-(xtal_freq/(2*i)))))*i;
        if(temp<tempMin)
        {
            tempMin=temp;
        }
    }
    i=8;
    temp = ((xtal_freq/(2*i))-abs(((Freq%(xtal_freq/i))-(xtal_freq/(2*i)))))*i;
    if(temp<tempMin)
    {
        tempMin=temp;
    }*/
    *chan_frac_temp_ptr=tempMin*(3072000/xtal_freq);        
    return(1);
}
//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Tune                                                     
//功能描述：发射频率设置函数                                                        
//函数说明：输入以KHz为单位的发射频率，                                                
//全局变量：                                                                        
//输    入：Freq （输入以KHz为单位的发射频率）                                        
//返    回：正确：1                 错误：0                                            
//设 计 者：Zhou Dongfeng           时间：2016-04-05                                         
//修 改 者：Zhou Dongfeng           时间：2016-08-26                                         
//版    本：V0.1    For KT0646M_VX
//          V0.2    修改了变量定义的方式并增加了VX和VX2的宏定义，
//                  修改了解决DLL不锁定部分程序的位置
//                  去掉了不用的chan_cfg2变量
//                  去掉了MCU_POWER_OK位的查询工作
//                  去掉了PLL锁定的判断
//          V0.4    统一了变量定义格式
//-----------------------------------------------------------------------------
//使用24MHz晶振的发射频点
UINT32 const use24M[26] = 
{   490500,492000,516000,541500,556000,565500,566000,590000,614000,615000,639000,651250,688000,
    688500,712000,712250,712500,722500,736500,760500,762000,787500,810000,811500,835500,859500
};

//使用24.576MHz晶振的发射频点
UINT32 const use24576M[14] = 
{
    7500,9000,10000,10500,12000,13500,14000,15000,16000,16500,18000,19500,20000,22000
};

u8 KT_WirelessMicTx_Tune(INT32 Freq)
{
    UINT16 chan0;
    UINT16 chan_frac0;
    UINT8 chan_frac_msb0;
    UINT8 lopa_div10,lopa_div20;
    UINT16 regx;
    UINT32 chan_frac_temp0;    
#ifdef XTAL_DUAL
	UINT16 state;
    UINT16 chan1,regy;
    UINT16 chan_frac1;
    UINT8 chan_frac_msb1;
    UINT8 lopa_div11,lopa_div21;
    UINT16 use24M_flag=0,use24576M_flag=0;
    UINT32 chan_frac_temp1;
    #ifndef RXISKT0616M_XTAL_DUAL
    UINT8 i;
    UINT32 state_tmp;
    #endif
//    double mod0,mod1,mod2,mod3;
#endif

    #ifdef DOUBLE_KVCO
        regx=KT_Bus_Read(0x2d); //kvco_cali_bps=0
        KT_Bus_Write(0x2d, (regx&0xf7ff));
    
        regx=KT_Bus_Read(0x0a); //locpcali_bps=0
        KT_Bus_Write(0x0a, (regx&0xffef));
        regx=KT_Bus_Read(0x3d);
        KT_Bus_Write(0x3d, (regx&0xffbf));              
    
        regx=KT_Bus_Read(0x3c);
        KT_Bus_Write(0x3c, (regx&0xfffd)); //loamp_cali_bps=0
    #endif    

//    regx = KT_Bus_Read(0x2d);
//    KT_Bus_Write(0x2d, (regx&0xfff8)|0x0003); //lo_fine_vref_sel

#ifdef XTAL_24M_ONLY
    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div10, &lopa_div20, &chan0, &chan_frac0, 
                                  &chan_frac_msb0,&chan_frac_temp0, 24000);
#endif

#ifdef XTAL_24P576M_ONLY
    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div10, &lopa_div20, &chan0, &chan_frac0, 
                                  &chan_frac_msb0,&chan_frac_temp0,24576);
#endif

#ifdef XTAL_DUAL

    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div10, &lopa_div20, &chan0, &chan_frac0, 
                                  &chan_frac_msb0,&chan_frac_temp0,24000);
    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div11, &lopa_div21, &chan1, &chan_frac1, 
                                  &chan_frac_msb1,&chan_frac_temp1,24576);
    #ifndef RXISKT0616M_XTAL_DUAL
        for(i=0;i<26;i++)
        {
            if(Freq==use24M[i])    
            {
                use24M_flag=1;
                break;
            }
        }
    
        state_tmp = Freq%24000;
        for(i=0;i<14;i++)
        {
            if(state_tmp==use24576M[i])
            {
                use24576M_flag=1;
                break;
            }
        }
        if (use24M_flag)
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24M_FREQ);
        }
        else if(use24576M_flag)
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24P576M_FREQ);
            chan0 = chan1;
            lopa_div10 = lopa_div11;
            lopa_div20 = lopa_div21;
            chan_frac0 = chan_frac1;
            chan_frac_msb0 = chan_frac_msb1;
        }
        else 
        {    
            if(chan_frac_temp0>chan_frac_temp1)
            {
                KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24M_FREQ);    
            }
            else
            {
                KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24P576M_FREQ);
                chan0 = chan1;
                lopa_div10 = lopa_div11;
                lopa_div20 = lopa_div21;
                chan_frac0 = chan_frac1;
                chan_frac_msb0 = chan_frac_msb1;    
            }
        }
    #else
        if(selectXtalOld(Freq))
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24P576M_FREQ);
            chan0 = chan1;
            lopa_div10 = lopa_div11;
            lopa_div20 = lopa_div21;
            chan_frac0 = chan_frac1;
            chan_frac_msb0 = chan_frac_msb1;
        }
        else
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24M_FREQ);
        }
    #endif

#endif

    KT_Bus_Write(0x08,(chan0 << 10) | (lopa_div10 << 8) | (lopa_div20 << 6) | (0 << 5) | 
                 (0 << 4) | (0 <<2) | chan_frac_msb0); //
    KT_Bus_Write(0x09, chan_frac0|0x0001);

    //dll未锁定，切换晶振后需要rst
    regx = KT_Bus_Read(0x0e);
    KT_Bus_Write(0x0e, regx | 0x0080); //dll_rst is from regbank
    regx = KT_Bus_Read(0x0e);
    KT_Bus_Write(0x0e, regx | 0x0100); //dll_rst=1
    Z_unified_delay_ms(1);
    regx = KT_Bus_Read(0x0e);
    KT_Bus_Write(0x0e, regx & 0xfeff); //dll_rst=0

    regx=KT_Bus_Read(0x0a);
    KT_Bus_Write(0x0a,regx|0x0020); //tune

    regx=KT_Bus_Read(0x0d);
    regx = (regx&0x0800)>>11;
    /*
    while(!regx) //wait pll ready
    {
        regx=KT_Bus_Read(0x0d);
        regx = (regx&0x0800)>>11; 
    }*/
    Z_unified_delay_ms(10);
    if(regx==0)
    {
        regx=KT_Bus_Read(0x0d);
        regx = (regx&0x0800)>>11; 
    }
    Z_unified_delay_ms(10);
    if(regx==0)
    {
        regx=KT_Bus_Read(0x0d);
        regx = (regx&0x0800)>>11; 
    }

//    regx = KT_Bus_Read(0x2d);
//    KT_Bus_Write(0x2d, (regx&0xfff8)); //lo_fine_vref_sel=0

    #ifdef DOUBLE_KVCO
        regx=KT_Bus_Read(0x2e); //double+16MHz/V locoarse_var_sel
        state=regx&0x1C00;
        state=state>>10;
        if(state >= 3)
        {
            state = 7;                                
        }
        else
        {
            state = (state<<1) + 3;
        }
        regx=regx&0xe3ff|(state<<10);
                                           
        regy=KT_Bus_Read(0x2d); //kvco_cali_bps=1
        KT_Bus_Write(0x2d, (regy|0x0800));              
    
        KT_Bus_Write(0x2e, regx); //write locoarse/lofine_var_sel
    
        regx=KT_Bus_Read(0x2a); //write cp_code
        state=(regx&0x007e)>>1;
        regx=KT_Bus_Read(0x3d);
        KT_Bus_Write(0x3d, (regx&0xff80)|(1<<6)|state);
    
        regx=KT_Bus_Read(0x0a); //locpcali_bps=1
        KT_Bus_Write(0x0a, (regx|0x0010));              
    
        regx=KT_Bus_Read(0x3c);
        KT_Bus_Write(0x3c, (regx|0x0002)); //loamp_cali_bps=1
    
        regx=KT_Bus_Read(0x0a);
        KT_Bus_Write(0x0a,regx|0x0020); //tune
    
        regx=KT_Bus_Read(0x0d);
        regx = (regx&0x0800)>>11;
        while(!regx) //wait pll ready
        {
            regx=KT_Bus_Read(0x0d);
            regx = (regx&0x0800)>>11; 
        }
    #endif

    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_Set_XTAL
//功能描述：设置晶体频率
//函数说明：设置晶体频率
//全局变量：
//输    入：xtal_sel: 0 or 1
//返    回：成功：1； 失败：0
//设 计 者：YANG Pei                时间：2012-04-10
//修 改 者：Zhou Dongfeng           时间：2016-08-26
//版    本：V0.4
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_Set_XTAL(u8 bXtal_Sel)
{
    UINT16 regx;
    
    KT_Bus_Write(0x3E,0x8000); //soft_rst=1      rst dsp part
    KT_Bus_Write(0x0E, 0x0002); //au_rst_bypass=1
    regx = KT_Bus_Read(0x0E);
    KT_Bus_Write(0x0E, regx | 0x0004); //au_dig_rst=1
    
    regx = KT_Bus_Read(0x47);
    KT_Bus_Write( 0x47, (regx & 0xFFDF) | ((UINT8)bXtal_Sel << 5) ); //bXtal_Sel=0

    Z_unified_delay_ms(50);

    KT_Bus_Write(0x0E, 0x0000); //au_rst_bypass=0    au_dig_rst=0
    KT_Bus_Write(0x3E, 0x0000); //soft_rst=0
#ifdef OTHER_RX
    KT_WirelessMicTx_Set_Pilot_Freq(bXtal_Sel); //set pilot frequency
#endif
    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_SW_XTAL_Freq                                                      
//功能描述：切换晶体频率                                                             
//函数说明：                                                                     
//全局变量：无                                                                     
//输    入：xtal_sel;                                                             
//返    回：成功：1； 失败：0                                                                 
//设 计 者：KANG Hekai              时间：                                         
//修 改 者：Zhou Dongfeng           时间：2016-08-26                                         
//版    本：V0.4                                                                     
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_SW_XTAL_Freq(u8 bXtal_Sel)
{
    UINT16 regx;

    regx = KT_Bus_Read(0x47);
    if (bXtal_Sel==1) //24.576MHz
    {
        //Display_Ch_Num(13,1);
        if ((regx & 0x0020)==0) //bXtal_Sel=0    24MHz
            KT_WirelessMicTx_Set_XTAL(bXtal_Sel);
    }
    else
    {
        //Display_Ch_Num(13,0);
        if ((regx & 0x0020)!=0) //bXtal_Sel=1     24.576MHz
            KT_WirelessMicTx_Set_XTAL(bXtal_Sel);
    }
    return(1);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_PowerDownProcedure                                                      
//功能描述：关机处理程序                                                             
//函数说明：                                                                     
//全局变量：无                                                                     
//输    入：无;                                                             
//返    回：无                                                                     
//设 计 者：Kang Hekai              时间：2014-02-13                                         
//修 改 者：Zhou Dongfeng           时间：2016-08-26                                         
//版    本：V0.4                                                                     
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_PowerDownProcedure(void)
{
    UINT16 regx;

    KT_WirelessMicTx_MuteSel(AUDIO_MUTE);
//    Delay_ms(50);
    KT_WirelessMicTx_Pilot(PILOT_DISABLE);
//    Delay_ms(500);

//    KT_WirelessMicTx_PAGain(0);
    KT_WirelessMicTx_PASW(PA_OFF);
//    Delay_ms(5);

    regx=KT_Bus_Read(0x0a);
    KT_Bus_Write(0x0a,regx|0x1000); //recali

    regx = KT_Bus_Read(0x03);
    KT_Bus_Write(0x03, regx | (_STANDBY << 15)); //Write Standby bit to 1
//    Delay_ms(20);
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_ECHO                                        
//功能描述：ECHO混响配置程序                                                
//函数说明：配置ECHO混响效果                
//全局变量：                                                                        
//输    入：bEcho_Dis    （ECHO开关）
//          Echo_Ratio     （ECHO反馈）                                        
//          Echo_Delay     （ECHO延时）
//返    回：无                                                            
//设 计 者：Zhou Dongfeng           时间：2016-04-26                                         
//修 改 者：                        时间：                                         
//版    本：V0.1    For KT0646M_VX                                                                 
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_ECHO(u8 bEcho_Dis,UINT8 Echo_Ratio,UINT8 Echo_Delay)
{
    UINT16 regx;

    KT_Bus_Write( 0x32, (ECHO_MCU << 15) | ((UINT8)bEcho_Dis << 14) | (ECHO_STRU << 13) | 
                 (Echo_Ratio << 8) | (Echo_Delay << 3) | ECHO_GAINUP );
    
    regx = KT_Bus_Read(0x33);
    KT_Bus_Write( 0x33, (regx & 0xFFFC) | ECHO_GAINDOWN );                          
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_EQSW                                        
//功能描述：EQ均衡器开关程序                                                
//函数说明：配置EQ均衡器开启关闭                
//全局变量：                                                                        
//输    入：bEq_Dis    （EQ开关）
//返    回：无                                                            
//设 计 者：Zhou Dongfeng           时间：2016-08-26                                         
//修 改 者：                        时间：
//版    本：V0.4
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_EQSW(u8 bEq_Dis)
{
    UINT16 regx;

    regx = KT_Bus_Read(0x34);
    KT_Bus_Write(0x34, ((regx & 0x7FFF) | (UINT16)bEq_Dis << 15));
}

//-----------------------------------------------------------------------------
//函 数 名：KT_WirelessMicTx_EQGAIN                                        
//功能描述：EQ均衡器配置程序                                                
//函数说明：配置EQ均衡器各频率增益效果                
//全局变量：                                                                        
//输    入：Eq_Freq （EQ频率）                                        
//          Eq_Gain （EQ增益）
//返    回：无                                                            
//设 计 者：Zhou Dongfeng           时间：2016-04-26                                         
//修 改 者：Zhou Dongfeng           时间：2016-08-26
//版    本：V0.4
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_EQGAIN(UINT8 Eq_Freq,UINT8 Eq_Gain)
{
    UINT16 regx;
    UINT8 temp1,temp2;
    temp1=Eq_Freq/3+0x34;
    temp2=(2-(Eq_Freq%3))*5;
    regx = KT_Bus_Read(temp1);
    
    KT_Bus_Write(temp1, ((regx & ~(0x001f<<temp2)) | ( (UINT16)Eq_Gain << temp2 )));
/*    switch(Eq_Freq)
    {
        case EQ_25H:
        {
            regx = KT_Bus_Read(0x34);
            KT_Bus_Write(0x34, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_40H:
        {
            regx = KT_Bus_Read(0x34);
            KT_Bus_Write(0x34, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_63H:
        {
            regx = KT_Bus_Read(0x34);
            KT_Bus_Write(0x34, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_100H:
        {
            regx = KT_Bus_Read(0x35);
            KT_Bus_Write(0x35, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_160H:
        {
            regx = KT_Bus_Read(0x35);
            KT_Bus_Write(0x35, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_250H:
        {
            regx = KT_Bus_Read(0x35);
            KT_Bus_Write(0x35, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_400H:
        {
            regx = KT_Bus_Read(0x36);
            KT_Bus_Write(0x36, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_630H:
        {
            regx = KT_Bus_Read(0x36);
            KT_Bus_Write(0x36, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_1KH:
        {
            regx = KT_Bus_Read(0x36);
            KT_Bus_Write(0x36, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_1K6:
        {
            regx = KT_Bus_Read(0x37);
            KT_Bus_Write(0x37, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_2K5:
        {
            regx = KT_Bus_Read(0x37);
            KT_Bus_Write(0x37, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_4KH:
        {
            regx = KT_Bus_Read(0x37);
            KT_Bus_Write(0x37, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_6K3:
        {
            regx = KT_Bus_Read(0x38);
            KT_Bus_Write(0x38, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_10K:
        {
            regx = KT_Bus_Read(0x38);
            KT_Bus_Write(0x38, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_16K:
        {
            regx = KT_Bus_Read(0x38);
            KT_Bus_Write(0x38, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;
        default    :    break; 
    }     */
}
void KT_MicTX_Init(void)
{
    //Load_Freq();
    //Memery_Frequency=Load_Frequency;
    KT_WirelessMicTx_Tune(Memery_Frequency);
	KT_Bus_Write(0x12,(MANUFACTURERID<<8)|((Memery_Frequency-BAND_BOTTOM)/BAND_STEP+1));
    KT_WirelessMicTx_EQGAIN(EQ_25H,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_40H,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_63H,EQ_GAIN_0dB);

    KT_WirelessMicTx_EQGAIN(EQ_100H,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_160H,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_250H,EQ_GAIN_0dB);

    KT_WirelessMicTx_EQGAIN(EQ_400H,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_630H,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_1KH,EQ_GAIN_0dB);

    KT_WirelessMicTx_EQGAIN(EQ_1K6,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_2K5,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_4KH,EQ_GAIN_0dB);

    KT_WirelessMicTx_EQGAIN(EQ_6K3,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_10K,EQ_GAIN_0dB);
    KT_WirelessMicTx_EQGAIN(EQ_16K,EQ_GAIN_0dB);

    KT_WirelessMicTx_EQSW(EQ_DISABLE);
    KT_WirelessMicTx_ECHO(ECHO_DISABLE,ECHO_RATIO_24,ECHO_DELAY_197ms);
//    Delay_ms(200);
        
    KT_MicTX_RFSwitch();
    KT_WirelessMicTx_PASW(PA_ON);

    KT_WirelessMicTx_Pilot_Fdev(PILOT_FDEV_5K);
    KT_WirelessMicTx_Pilot(PILOT_ENABLE);
    //WriteFix_1621(LCD_PILOT);
}
void KT_MicTX_RFSwitch (void)
{/*
    if (RF_POW_SW == 0)
    {
        if(Key_RF_POW_flag)//去抖
            Delay_ms(100);
        if (RF_POW_SW == 0) 
        {
            KT_WirelessMicTx_PAGain(10); //差分10dBm，单端7dBm
            RF_Power(3);
        }
    }
    else
    {
        if(Key_RF_POW_flag)//去抖
            Delay_ms(100); 
        if (RF_POW_SW == 1)    
        {
            KT_WirelessMicTx_PAGain(42); //差分18dBm，单端15dBm
            RF_Power(7);
        }
    }*/
    Z_unified_delay_ms(20);
    KT_WirelessMicTx_PAGain(42);
    //temp_RF_POW_SW = RF_POW_SW;
    //Display_Frequency_and_RFPower(Memery_Frequency);
}
void freqConfirm(void)
{
	UINT16 pilotSave,regx;
	
	KT_Bus_Write(0x12,(CHANGE_TRANSMISSION<<8)|((Memery_Frequency-BAND_BOTTOM)/BAND_STEP+1));
	Z_unified_delay_ms(500);
	Key_RF_POW_flag = 0;
	pilotSave=KT_Bus_Read(0x1F);	  //读导频的设置
 	KT_WirelessMicTx_Pilot(PILOT_DISABLE);
    KT_WirelessMicTx_PAGain(0);
    KT_WirelessMicTx_PASW(PA_OFF);                        
    Z_unified_delay_ms(5);
    KT_WirelessMicTx_Tune(Memery_Frequency);
	KT_Bus_Write(0x12,(MANUFACTURERID<<8)|((Memery_Frequency-BAND_BOTTOM)/BAND_STEP+1));
	KT_Bus_Write(0x13,0x0000);
    KT_MicTX_RFSwitch();
    KT_WirelessMicTx_PASW(PA_ON);
	KT_Bus_Write(0x1F,pilotSave);//恢复导频的设置
	regx = KT_Bus_Read(0x1e);
	KT_Bus_Write(0x1e,regx|0x0010);
	Z_unified_delay_ms(5);
	regx = KT_Bus_Read(0x1e);
	KT_Bus_Write(0x1e,regx&~0x0010);
    Key_RF_POW_flag = 1;
}

void KT0641_init()
{
	//while(!KT_WirelessMicTx_PreInit());
    KT_WirelessMicTx_Init();
    KT_MicTX_Init();
    KT_Bus_Write(0x17,0x1187); 
    //IO_contral_init(Port_A5,1,1,0,0);
    //IO_contral_init(Port_A6,1,1,0,0);

/*
    transmitID=KT_MicTX_readID();
	if((transmitID==0)||(transmitID==0xffff))
	{
		KT_MicTX_generatID();
	}
	transmitID=0x1187;
	KT_Bus_Write(0x17,transmitID);
	*/
}
void KT0641_mute_deal(u8 lv_x)
{
    static u8 mute0641=0;

    mute0641 = !mute0641;
    if(mute0641)
    {
        //KT_WirelessMicTx_MuteSel(1);
        /*
        KT_WirelessMicTx_PAGain(0);
        KT_WirelessMicTx_PASW(PA_OFF);*/
        KT_WirelessMicTx_PowerDownProcedure();
    }
    else
    {
        //KT_WirelessMicTx_MuteSel(0);
        //KT_WirelessMicTx_PAGain(10);
        KT_WirelessMicTx_PASW(PA_ON);
    }
    
    /*
    if(lv_x)
    {
        KT_WirelessMicTx_MuteSel(1);
    }
    else
    {
        KT_WirelessMicTx_MuteSel(0);
    }*/
    
}
void KT0641_fre_set(u32 lv_fre)
{
    Memery_Frequency=lv_fre;
   //if((Memery_Frequency > BAND_TOP) || (Memery_Frequency < BAND_BOTTOM))
    if((Memery_Frequency > BAND_TOP) || (Memery_Frequency < 470000))//修正
	{
    	Memery_Frequency = BAND_BOTTOM;
	}
	freqConfirm();
}
void KT0641_msg_deal(u16 lv_msg)
{
    if(lv_msg==Custom_Event2_KT0641_mute)
    {
        KT0641_mute_deal(1);
    }
    if(lv_msg==Custom_Event2_KT0641_sw)
    {
    
        if(KT0641_work_mode==0)
        {
        DEBUG_ZX(0, " Custom_Event2_KT0641_sw ", 0);
            IO_contral_init(KT0641_power_pin,0,0,0,1);
                //Z_post_msg(NULL,Custom_Event2_mute2,0);
            PA_keep_off = 1;
            PA_contral();

            
            KT0641_work_mode=1;
            IO_contral_init(IIC_DAT_pin,0,0,0,1);
            IO_contral_init(IIC_CLK_pin,0,0,0,1);
            DelayMs(100);
            KT0641_init();
            //KT_WirelessMicTx_Pilot(PILOT_ENABLE);
        }
        else
        {
            //KT_WirelessMicTx_PAGain(0);
            //KT_WirelessMicTx_PASW(PA_OFF); 
            //KT0641_mute_deal(1);
            
            KT_WirelessMicTx_PowerDownProcedure();
            /*
            KT_WirelessMicTx_MuteSel(AUDIO_MUTE);
            KT_WirelessMicTx_Pilot(PILOT_DISABLE);
            KT_WirelessMicTx_PASW(PA_OFF);
            */
            DelayMs(20);
            IO_contral_init(IIC_DAT_pin,1,0,0,0);
            IO_contral_init(IIC_CLK_pin,1,0,0,0);
            
            IO_contral_init(KT0641_power_pin,0,0,0,0);
            //Z_post_msg(NULL,Custom_Event2_unmute2,0);
            PA_keep_off = 0;
            PA_contral();
            KT0641_work_mode=0;
        }
    }
    if(lv_msg==Custom_Event2_KT0641_next)
    {
        KT0641_grade++;
        if(KT0641_grade>KT0641_pre_MAX)KT0641_grade=1;
        
        switch(KT0641_grade)
        {
            case 1:KT0641_fre_set(KT0641_pre1);     break;
            case 2:KT0641_fre_set(KT0641_pre2);     break;
            case 3:KT0641_fre_set(KT0641_pre3);     break;
        }
        
    }
}





#endif



u32 Get_Mic_Energy(void)
{

  //  return gCtrlVars.MicAudioSdct_unit.level;

}


u32 Get_Music_Energy(void)
{


    static u32 DAC_Energy[6]={0};
    static u8 i = 0;
	

      /*if(GetSystemMode()==ModeBtAudioPlay && btManager.btLinkState==0)
      {
          return 0;
	  }*/


	 /* if(  GetSystemMode()==ModeUsbDevicePlay
	  	|| GetSystemMode()==ModeCoaxialAudioPlay)
      {
          DAC_Energy[i] = Get_Curr_Music_energy;

		  i++;
		  if(i > 5)
		  {
             i=0;

	             //一直是一个值
	             if(DAC_Energy[0]==DAC_Energy[1])
	             {
	                if(DAC_Energy[0]==DAC_Energy[2])
	                {
		               if(DAC_Energy[0]==DAC_Energy[3])
		               {
		                   if(DAC_Energy[0]==DAC_Energy[4])
			               {
		                       if(DAC_Energy[0]==DAC_Energy[5])
				               {
		                            Get_Curr_Music_energy = 0;
							   }
						   }
		               }
					}
				 }


			 return Get_Curr_Music_energy;
		  }

           
	  }
	  else*/
	  {
         return Get_Curr_Music_energy / 10;
	  }

   
}







u8 BOEU_AudioMusicVolUp(void)
{

     #if Z__SYS_GAMUT_VOL == 6  //分6阶
            Custom_MusicVolume++;  
       
	        if(Custom_MusicVolume > Z__SYS_GAMUT_VOL)
	        {
               Custom_MusicVolume = Z__SYS_GAMUT_VOL;
			}
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 6;break;
			case 2: Music_Volume = 12;break;
			case 3: Music_Volume = 18;break;
			case 4: Music_Volume = 24;break;
			case 5: Music_Volume = 28;break;
			case 6: Music_Volume = 32;break;
			
			default :  Music_Volume = 32;break;
		 }
	 #elif Z__SYS_GAMUT_VOL == 10	 //分10阶
           Custom_MusicVolume++;  
       
	        if(Custom_MusicVolume > Z__SYS_GAMUT_VOL)
	        {
               Custom_MusicVolume = Z__SYS_GAMUT_VOL;
			}
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 2;break;
			case 2: Music_Volume = 4;break;
			case 3: Music_Volume = 6;break;
			case 4: Music_Volume = 7;break;
			case 5: Music_Volume = 9;break;
			case 6: Music_Volume = 12;break;
			case 7: Music_Volume = 17;break;
			case 8: Music_Volume = 22;break;
			case 9: Music_Volume = 27;break;
			case 10: Music_Volume = 32;break;
			default :  Music_Volume = 32;break;
		 }
	 #elif Z__SYS_GAMUT_VOL == 16	 //分16阶
           Custom_MusicVolume++;  
       
	        if(Custom_MusicVolume > Z__SYS_GAMUT_VOL)
	        {
               Custom_MusicVolume = Z__SYS_GAMUT_VOL;
			}
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 2;break;
			case 2: Music_Volume = 5;break;
			case 3: Music_Volume = 8;break;
			case 4: Music_Volume = 10;break;
			case 5: Music_Volume = 12;break;
			case 6: Music_Volume = 15;break;
			case 7: Music_Volume = 17;break;
			case 8: Music_Volume = 19;break;
			case 9: Music_Volume = 21;break;
			case 10: Music_Volume = 23;break;
			case 11: Music_Volume = 25;break;
			case 12: Music_Volume = 27;break;
			case 13: Music_Volume = 29;break;
			case 14: Music_Volume = 30;break;
			case 15: Music_Volume = 31;break;
			case 16: Music_Volume = 32;break;
			default :  Music_Volume = 32;break;
		 }	 	 

	 #endif

    APP_DBG("Custom_MusicVolume == %d\n",Custom_MusicVolume);
	APP_DBG("BOEU_AudioMusicVolUp  Music_Volume == %d\n",Music_Volume); 

	return Music_Volume;

} 


u8 BOEU_AudioMusicVolDown(void)
{

     #if Z__SYS_GAMUT_VOL == 6  //分6阶
        if(Custom_MusicVolume > 0)
		{
           Custom_MusicVolume--;
		}
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 6;break;
			case 2: Music_Volume = 12;break;
			case 3: Music_Volume = 18;break;
			case 4: Music_Volume = 24;break;
			case 5: Music_Volume = 28;break;
			case 6: Music_Volume = 32;break;
			
			default :  Music_Volume = 32;break;
		 }
	 #elif Z__SYS_GAMUT_VOL == 10	 //分10阶
           if(Custom_MusicVolume > 0)
		   {
              Custom_MusicVolume--;
		   }
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 2;break;
			case 2: Music_Volume = 4;break;
			case 3: Music_Volume = 6;break;
			case 4: Music_Volume = 7;break;
			case 5: Music_Volume = 9;break;
			case 6: Music_Volume = 12;break;
			case 7: Music_Volume = 17;break;
			case 8: Music_Volume = 22;break;
			case 9: Music_Volume = 27;break;
			case 10: Music_Volume = 32;break;
			default :  Music_Volume = 32;break;
		 }
	 #elif Z__SYS_GAMUT_VOL == 16	 //分16阶
           if(Custom_MusicVolume > 0)
		   {
              Custom_MusicVolume--;
		   }
			
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 2;break;
			case 2: Music_Volume = 5;break;
			case 3: Music_Volume = 8;break;
			case 4: Music_Volume = 10;break;
			case 5: Music_Volume = 12;break;
			case 6: Music_Volume = 15;break;
			case 7: Music_Volume = 17;break;
			case 8: Music_Volume = 19;break;
			case 9: Music_Volume = 21;break;
			case 10: Music_Volume = 23;break;
			case 11: Music_Volume = 25;break;
			case 12: Music_Volume = 27;break;
			case 13: Music_Volume = 29;break;
			case 14: Music_Volume = 30;break;
			case 15: Music_Volume = 31;break;
			case 16: Music_Volume = 32;break;
			default :  Music_Volume = 32;break;
		 }	 		 

	 #endif

    APP_DBG("Custom_MusicVolume == %d\n",Custom_MusicVolume);
	APP_DBG("BOEU_AudioMusicVolUp  Music_Volume == %d\n",Music_Volume); 

	return Music_Volume;

} 




u8 BOEU_AudioMicVolUp(void)
{

   #if Z__SYS_MIC_VOL == 6  //分6阶
            MIC_VOL_Adjust_flag++;  
       
	        if(MIC_VOL_Adjust_flag > Z__SYS_MIC_VOL)
	        {
               MIC_VOL_Adjust_flag = Z__SYS_MIC_VOL;
			}
			
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0: Mic_Volume = 0;break;
			case 1: Mic_Volume = 6;break;
			case 2: Mic_Volume = 12;break;
			case 3: Mic_Volume = 18;break;
			case 4: Mic_Volume = 24;break;
			case 5: Mic_Volume = 28;break;
			case 6: Mic_Volume = 32;break;
			
			default :  Mic_Volume = 32;break;
		 }
	 #elif Z__SYS_MIC_VOL == 10	 //分10阶
           MIC_VOL_Adjust_flag++;  
       
	        if(MIC_VOL_Adjust_flag > Z__SYS_MIC_VOL)
	        {
               MIC_VOL_Adjust_flag = Z__SYS_MIC_VOL;
			}
			
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0:  Mic_Volume = 0;break;
			case 1:  Mic_Volume = 6;break;
			case 2:  Mic_Volume = 10;break;
			case 3:  Mic_Volume = 14;break;
			case 4:  Mic_Volume = 18;break;
			case 5:  Mic_Volume = 22;break;
			case 6:  Mic_Volume = 24;break;
			case 7:  Mic_Volume = 26;break;
			case 8:  Mic_Volume = 28;break;
			case 9:  Mic_Volume = 30;break;
			case 10: Mic_Volume = 32;break;
			
			default :  Mic_Volume = 32;break;
		 }
		 
	 #elif Z__SYS_MIC_VOL == 16	 //分16阶

          MIC_VOL_Adjust_flag++;  
	      if(MIC_VOL_Adjust_flag > Z__SYS_MIC_VOL)
	      {
              MIC_VOL_Adjust_flag = Z__SYS_MIC_VOL;
		  }
			
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0: Mic_Volume = 0;break;
			case 1: Mic_Volume = 4;break;
			case 2: Mic_Volume = 6;break;
			case 3: Mic_Volume = 8;break;
			case 4: Mic_Volume = 10;break;
			case 5: Mic_Volume = 12;break;
			case 6: Mic_Volume = 15;break;
			case 7: Mic_Volume = 17;break;
			case 8: Mic_Volume = 19;break;
			case 9: Mic_Volume = 21;break;
			case 10: Mic_Volume = 23;break;
			case 11: Mic_Volume = 25;break;
			case 12: Mic_Volume = 27;break;
			case 13: Mic_Volume = 29;break;
			case 14: Mic_Volume = 30;break;
			case 15: Mic_Volume = 31;break;
			case 16: Mic_Volume = 32;break;
			default :  Mic_Volume = 32;break;
		 }	 	 	 

	 #endif

    APP_DBG("MIC_VOL_Adjust_flag == %d\n",MIC_VOL_Adjust_flag);
	APP_DBG("BOEU_AudioMicVolUp  Mic_Volume == %d\n",Mic_Volume); 

	return Mic_Volume;


}


u8 BOEU_AudioMicVolDown(void)
{

   #if Z__SYS_MIC_VOL == 6  //分6阶
           if(MIC_VOL_Adjust_flag > 0)
	       {
               MIC_VOL_Adjust_flag--;
	       }
	 		
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0: Mic_Volume = 0;break;
			case 1: Mic_Volume = 6;break;
			case 2: Mic_Volume = 12;break;
			case 3: Mic_Volume = 18;break;
			case 4: Mic_Volume = 24;break;
			case 5: Mic_Volume = 28;break;
			case 6: Mic_Volume = 32;break;
			
			default :  Mic_Volume = 32;break;
		 }
	 #elif Z__SYS_MIC_VOL == 10	 //分10阶
         		      
	        if(MIC_VOL_Adjust_flag > 0)
	        {
               MIC_VOL_Adjust_flag--;
			}
			
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0:  Mic_Volume = 0;break;
			case 1:  Mic_Volume = 6;break;
			case 2:  Mic_Volume = 10;break;
			case 3:  Mic_Volume = 14;break;
			case 4:  Mic_Volume = 18;break;
			case 5:  Mic_Volume = 22;break;
			case 6:  Mic_Volume = 24;break;
			case 7:  Mic_Volume = 26;break;
			case 8:  Mic_Volume = 28;break;
			case 9:  Mic_Volume = 30;break;
			case 10: Mic_Volume = 32;break;
			
			default :  Mic_Volume = 32;break;
		 }
	 #elif Z__SYS_MIC_VOL == 16	 //分16阶

         if(MIC_VOL_Adjust_flag > 0)
	     {
              MIC_VOL_Adjust_flag--;
		 }
			
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0: Mic_Volume = 0;break;
			case 1: Mic_Volume = 4;break;
			case 2: Mic_Volume = 6;break;
			case 3: Mic_Volume = 8;break;
			case 4: Mic_Volume = 10;break;
			case 5: Mic_Volume = 12;break;
			case 6: Mic_Volume = 15;break;
			case 7: Mic_Volume = 17;break;
			case 8: Mic_Volume = 19;break;
			case 9: Mic_Volume = 21;break;
			case 10: Mic_Volume = 23;break;
			case 11: Mic_Volume = 25;break;
			case 12: Mic_Volume = 27;break;
			case 13: Mic_Volume = 29;break;
			case 14: Mic_Volume = 30;break;
			case 15: Mic_Volume = 31;break;
			case 16: Mic_Volume = 32;break;
			default :  Mic_Volume = 32;break;
		 }		 	 

	 #endif

    APP_DBG("MIC_VOL_Adjust_flag == %d\n",MIC_VOL_Adjust_flag);
	APP_DBG("AudioMicVolDown  Mic_Volume == %d\n",Mic_Volume); 

	return Mic_Volume;


}


u8 BOEU_MIC_EchoReverb_UP(void)
{
      #if Z__MIC_ECHO_VOL == 6
            Custom_MicECHO_Volume++;
             if(Custom_MicECHO_Volume > Z__MIC_ECHO_VOL)
	        {
               Custom_MicECHO_Volume = Z__MIC_ECHO_VOL;
			}
	   
	     switch(Custom_MicECHO_Volume)
		 {
			case 0: MicECHO_Volume = 0;break;
			case 1: MicECHO_Volume = 6;break;
			case 2: MicECHO_Volume = 12;break;
			case 3: MicECHO_Volume = 18;break;
			case 4: MicECHO_Volume = 24;break;
			case 5: MicECHO_Volume = 28;break;
			case 6: MicECHO_Volume = 32;break;
			
			default :  MicECHO_Volume = 32;break;
		 }
	 #elif Z__MIC_ECHO_VOL == 10	 //分10阶
           Custom_MicECHO_Volume++;  
       
	        if(Custom_MicECHO_Volume > Z__MIC_ECHO_VOL)
	        {
               Custom_MicECHO_Volume = Z__MIC_ECHO_VOL;
			}
			
         switch(Custom_MicECHO_Volume)
		 {
			case 0:  MicECHO_Volume = 0;break;
			case 1:  MicECHO_Volume = 6;break;
			case 2:  MicECHO_Volume = 10;break;
			case 3:  MicECHO_Volume = 14;break;
			case 4:  MicECHO_Volume = 18;break;
			case 5:  MicECHO_Volume = 22;break;
			case 6:  MicECHO_Volume = 24;break;
			case 7:  MicECHO_Volume = 26;break;
			case 8:  MicECHO_Volume = 28;break;
			case 9:  MicECHO_Volume = 30;break;
			case 10: MicECHO_Volume = 32;break;
			
			default :  MicECHO_Volume = 32;break;
		 }
	 #elif Z__MIC_ECHO_VOL == 16	 //分16阶

         Custom_MicECHO_Volume++;  
       
	        if(Custom_MicECHO_Volume > Z__MIC_ECHO_VOL)
	        {
               Custom_MicECHO_Volume = Z__MIC_ECHO_VOL;
			}
			
         switch(Custom_MicECHO_Volume)
		 {
			case 0: MicECHO_Volume = 0;break;
			case 1: MicECHO_Volume = 4;break;
			case 2: MicECHO_Volume = 6;break;
			case 3: MicECHO_Volume = 8;break;
			case 4: MicECHO_Volume = 10;break;
			case 5: MicECHO_Volume = 12;break;
			case 6: MicECHO_Volume = 15;break;
			case 7: MicECHO_Volume = 17;break;
			case 8: MicECHO_Volume = 19;break;
			case 9: MicECHO_Volume = 21;break;
			case 10: MicECHO_Volume = 23;break;
			case 11: MicECHO_Volume = 25;break;
			case 12: MicECHO_Volume = 27;break;
			case 13: MicECHO_Volume = 29;break;
			case 14: MicECHO_Volume = 30;break;
			case 15: MicECHO_Volume = 31;break;
			case 16: MicECHO_Volume = 32;break;
			default :  MicECHO_Volume = 32;break;
		 }		 	 

	 #endif

	APP_DBG("Custom_MicECHO_Volume == %d\n",Custom_MicECHO_Volume);
	APP_DBG("BOEU_MIC_EchoReverb_UP  MicECHO_Volume == %d\n",MicECHO_Volume); 

	return MicECHO_Volume;
}



u8 BOEU_MIC_EchoReverb_DW(void)
{
      #if Z__MIC_ECHO_VOL == 6
           
            if(Custom_MicECHO_Volume > 0)
	        {
               Custom_MicECHO_Volume--;
			}
	   
	     switch(Custom_MicECHO_Volume)
		 {
			case 0: MicECHO_Volume = 0;break;
			case 1: MicECHO_Volume = 6;break;
			case 2: MicECHO_Volume = 12;break;
			case 3: MicECHO_Volume = 18;break;
			case 4: MicECHO_Volume = 24;break;
			case 5: MicECHO_Volume = 28;break;
			case 6: MicECHO_Volume = 32;break;
			
			default :  MicECHO_Volume = 32;break;
		 }
	 #elif Z__MIC_ECHO_VOL == 10	 //分10阶
            if(Custom_MicECHO_Volume > 0)
	        {
               Custom_MicECHO_Volume--;
			}
			
         switch(Custom_MicECHO_Volume)
		 {
			case 0:  MicECHO_Volume = 0;break;
			case 1:  MicECHO_Volume = 6;break;
			case 2:  MicECHO_Volume = 10;break;
			case 3:  MicECHO_Volume = 14;break;
			case 4:  MicECHO_Volume = 18;break;
			case 5:  MicECHO_Volume = 22;break;
			case 6:  MicECHO_Volume = 24;break;
			case 7:  MicECHO_Volume = 26;break;
			case 8:  MicECHO_Volume = 28;break;
			case 9:  MicECHO_Volume = 30;break;
			case 10: MicECHO_Volume = 32;break;
			
			default :  MicECHO_Volume = 32;break;
		 }
	 #elif Z__MIC_ECHO_VOL == 16	 //分16阶

         if(Custom_MicECHO_Volume > 0)
	     {
             Custom_MicECHO_Volume--;
	     }
			
         switch(Custom_MicECHO_Volume)
		 {
			case 0: MicECHO_Volume = 0;break;
			case 1: MicECHO_Volume = 4;break;
			case 2: MicECHO_Volume = 6;break;
			case 3: MicECHO_Volume = 8;break;
			case 4: MicECHO_Volume = 10;break;
			case 5: MicECHO_Volume = 12;break;
			case 6: MicECHO_Volume = 15;break;
			case 7: MicECHO_Volume = 17;break;
			case 8: MicECHO_Volume = 19;break;
			case 9: MicECHO_Volume = 21;break;
			case 10: MicECHO_Volume = 23;break;
			case 11: MicECHO_Volume = 25;break;
			case 12: MicECHO_Volume = 27;break;
			case 13: MicECHO_Volume = 29;break;
			case 14: MicECHO_Volume = 30;break;
			case 15: MicECHO_Volume = 31;break;
			case 16: MicECHO_Volume = 32;break;
			default :  MicECHO_Volume = 32;break;
		 }			 	 

	 #endif

	APP_DBG("Custom_MicECHO_Volume == %d\n",Custom_MicECHO_Volume);
	APP_DBG("BOEU_MIC_EchoReverb_DW  MicECHO_Volume == %d\n",MicECHO_Volume); 

	return MicECHO_Volume;
	
}




/*
*  对music、MIC、混响音量值调节所对应的标志初始化

此函数内容设置与以下函数紧紧关联:
 BOEU_AudioMusicVolDown();
 BOEU_AudioMusicVolUp();
 BOEU_AudioMicVolUp();
 BOEU_AudioMicVolDown();
 BOEU_MIC_EchoReverb_UP();
 BOEU_MIC_EchoReverb_DW();

*/
void BOEU_Music_MIC_ECHO_Vol_Init(void)
{


#if Z__SYS_GAMUT_VOL == 6  //分6阶

         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 6;break;
			case 2: Music_Volume = 12;break;
			case 3: Music_Volume = 18;break;
			case 4: Music_Volume = 24;break;
			case 5: Music_Volume = 28;break;
			case 6: Music_Volume = 32;break;
			
			default :  Music_Volume = 32;break;
		 }
		 
#elif Z__SYS_GAMUT_VOL == 10	 //分10阶
			
         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 2;break;
			case 2: Music_Volume = 4;break;
			case 3: Music_Volume = 6;break;
			case 4: Music_Volume = 7;break;
			case 5: Music_Volume = 9;break;
			case 6: Music_Volume = 12;break;
			case 7: Music_Volume = 17;break;
			case 8: Music_Volume = 22;break;
			case 9: Music_Volume = 27;break;
			case 10: Music_Volume = 32;break;
			default :  Music_Volume = 32;break;
		 }
#elif Z__SYS_GAMUT_VOL == 16	 //分16阶

         switch(Custom_MusicVolume)
		 {
			case 0: Music_Volume = 0;break;
			case 1: Music_Volume = 2;break;
			case 2: Music_Volume = 5;break;
			case 3: Music_Volume = 8;break;
			case 4: Music_Volume = 10;break;
			case 5: Music_Volume = 12;break;
			case 6: Music_Volume = 15;break;
			case 7: Music_Volume = 17;break;
			case 8: Music_Volume = 19;break;
			case 9: Music_Volume = 21;break;
			case 10: Music_Volume = 23;break;
			case 11: Music_Volume = 25;break;
			case 12: Music_Volume = 27;break;
			case 13: Music_Volume = 29;break;
			case 14: Music_Volume = 30;break;
			case 15: Music_Volume = 31;break;
			case 16: Music_Volume = 32;break;
			default :  Music_Volume = 32;break;
		 }	 		 

#endif


#if Z__SYS_MIC_VOL == 6  //分6阶

         switch(MIC_VOL_Adjust_flag)
		 {
			case 0: Mic_Volume = 0;break;
			case 1: Mic_Volume = 6;break;
			case 2: Mic_Volume = 12;break;
			case 3: Mic_Volume = 18;break;
			case 4: Mic_Volume = 24;break;
			case 5: Mic_Volume = 28;break;
			case 6: Mic_Volume = 32;break;
			
			default :  Mic_Volume = 32;break;
		 }
		 
#elif Z__SYS_MIC_VOL == 10	 //分10阶
			
         switch(MIC_VOL_Adjust_flag)
		 {
			case 0:  Mic_Volume = 0;break;
			case 1:  Mic_Volume = 6;break;
			case 2:  Mic_Volume = 10;break;
			case 3:  Mic_Volume = 14;break;
			case 4:  Mic_Volume = 18;break;
			case 5:  Mic_Volume = 22;break;
			case 6:  Mic_Volume = 24;break;
			case 7:  Mic_Volume = 26;break;
			case 8:  Mic_Volume = 28;break;
			case 9:  Mic_Volume = 30;break;
			case 10: Mic_Volume = 32;break;
			
			default :  Mic_Volume = 32;break;
		 }
		 
#elif Z__SYS_MIC_VOL == 16	 //分16阶

         switch(MIC_VOL_Adjust_flag)
		 {
			case 0: Mic_Volume = 0;break;
			case 1: Mic_Volume = 4;break;
			case 2: Mic_Volume = 6;break;
			case 3: Mic_Volume = 8;break;
			case 4: Mic_Volume = 10;break;
			case 5: Mic_Volume = 12;break;
			case 6: Mic_Volume = 15;break;
			case 7: Mic_Volume = 17;break;
			case 8: Mic_Volume = 19;break;
			case 9: Mic_Volume = 21;break;
			case 10: Mic_Volume = 23;break;
			case 11: Mic_Volume = 25;break;
			case 12: Mic_Volume = 27;break;
			case 13: Mic_Volume = 29;break;
			case 14: Mic_Volume = 30;break;
			case 15: Mic_Volume = 31;break;
			case 16: Mic_Volume = 32;break;
			default :  Mic_Volume = 32;break;
		 }	 		 

#endif

#if Z__MIC_ECHO_VOL == 6

	     switch(Custom_MicECHO_Volume)
		 {
			case 0: MicECHO_Volume = 0;break;
			case 1: MicECHO_Volume = 6;break;
			case 2: MicECHO_Volume = 12;break;
			case 3: MicECHO_Volume = 18;break;
			case 4: MicECHO_Volume = 24;break;
			case 5: MicECHO_Volume = 28;break;
			case 6: MicECHO_Volume = 32;break;
			
			default :  MicECHO_Volume = 32;break;
		 }
#elif Z__MIC_ECHO_VOL == 10	 //分10阶
    
         switch(Custom_MicECHO_Volume)
		 {
			case 0:  MicECHO_Volume = 0;break;
			case 1:  MicECHO_Volume = 6;break;
			case 2:  MicECHO_Volume = 10;break;
			case 3:  MicECHO_Volume = 14;break;
			case 4:  MicECHO_Volume = 18;break;
			case 5:  MicECHO_Volume = 22;break;
			case 6:  MicECHO_Volume = 24;break;
			case 7:  MicECHO_Volume = 26;break;
			case 8:  MicECHO_Volume = 28;break;
			case 9:  MicECHO_Volume = 30;break;
			case 10: MicECHO_Volume = 32;break;
			
			default :  MicECHO_Volume = 32;break;
		 }
		 	 
#elif Z__MIC_ECHO_VOL == 16	 //分16阶
    
         switch(Custom_MicECHO_Volume)
		 {
			case 0: MicECHO_Volume = 0;break;
			case 1: MicECHO_Volume = 4;break;
			case 2: MicECHO_Volume = 6;break;
			case 3: MicECHO_Volume = 8;break;
			case 4: MicECHO_Volume = 10;break;
			case 5: MicECHO_Volume = 12;break;
			case 6: MicECHO_Volume = 15;break;
			case 7: MicECHO_Volume = 17;break;
			case 8: MicECHO_Volume = 19;break;
			case 9: MicECHO_Volume = 21;break;
			case 10: MicECHO_Volume = 23;break;
			case 11: MicECHO_Volume = 25;break;
			case 12: MicECHO_Volume = 27;break;
			case 13: MicECHO_Volume = 29;break;
			case 14: MicECHO_Volume = 30;break;
			case 15: MicECHO_Volume = 31;break;
			case 16: MicECHO_Volume = 32;break;
			default :  MicECHO_Volume = 32;break;
		 }	 	
		 
#endif

	 
  #if 0
     APP_DBG("init        Custom_MusicVolume == %d\n",Custom_MusicVolume);
	 APP_DBG("init       MIC_VOL_Adjust_flag == %d\n",MIC_VOL_Adjust_flag);
	 APP_DBG("init     Custom_MicECHO_Volume == %d\n",Custom_MicECHO_Volume);
	 APP_DBG("init            MicECHO_Volume == %d\n",MicECHO_Volume);
	 APP_DBG("init              Music_Volume == %d\n",Music_Volume);
	 APP_DBG("init                Mic_Volume == %d\n",Mic_Volume);
	 
   #endif
   APP_DBG("--- BOEU_Music_MIC_ECHO_Vol_Init() ---\n");
   
}


/*
   TWS 主音量阶数设置同步    (按键音量+-)
   对 Custom_MusicVolume 变量 设置同步
*/
void TWS_Music_Vol_Sync(u8 music_vol)
{


	//if(GetBtManager()->twsState != BT_TWS_STATE_CONNECTED)
		//return;


#ifndef TWS_VOLUME_SYNC_ENABLE
    return;
#endif

      #if Z__SYS_GAMUT_VOL == 6  //分6阶
            
         switch(music_vol)
		 {
			case 0:  Custom_MusicVolume = 0;break;
			case 6:  Custom_MusicVolume = 1;break;
			case 12: Custom_MusicVolume = 2;break;
			case 18: Custom_MusicVolume = 3;break;
			case 24: Custom_MusicVolume = 4;break;
			case 28: Custom_MusicVolume = 5;break;
			case 32: Custom_MusicVolume = 6;break;
			
			default :  Custom_MusicVolume = 6; break;
		 }
     #elif Z__SYS_GAMUT_VOL == 10
         switch(music_vol)
		 {
			case 0:  Custom_MusicVolume = 0;break;
			case 2:  Custom_MusicVolume = 1;break;
			case 4:  Custom_MusicVolume = 2;break;
			case 6:  Custom_MusicVolume = 3;break;
			case 7:  Custom_MusicVolume = 4;break;
			case 9:  Custom_MusicVolume = 5;break;
			case 12: Custom_MusicVolume = 6;break;
			case 17: Custom_MusicVolume = 7;break;
			case 22: Custom_MusicVolume = 8;break;
			case 27: Custom_MusicVolume = 9;break;
			case 32: Custom_MusicVolume = 10;break;
			
			default :  Custom_MusicVolume = 10;break;
						
		 }
	 #elif Z__SYS_GAMUT_VOL == 16
         switch(music_vol)
		 {
			case 0: Custom_MusicVolume = 0;break;
			case 2: Custom_MusicVolume = 1;break;
			case 5: Custom_MusicVolume = 2;break;
			case 8: Custom_MusicVolume = 3;break;
			case 10: Custom_MusicVolume = 4;break;
			case 12: Custom_MusicVolume = 5;break;
			case 15: Custom_MusicVolume = 6;break;
			case 17: Custom_MusicVolume = 7;break;
			case 19: Custom_MusicVolume = 8;break;
			case 21: Custom_MusicVolume = 9;break;
			case 23: Custom_MusicVolume = 10;break;
			case 25: Custom_MusicVolume = 11;break;
			case 27: Custom_MusicVolume = 12;break;
			case 29: Custom_MusicVolume = 13;break;
			case 30: Custom_MusicVolume = 14;break;
			case 31: Custom_MusicVolume = 15;break;
			case 32: Custom_MusicVolume = 16;break;
			
			default :  Custom_MusicVolume = 16;break;
         }
    #endif 

	

}



/*
*  兼容四节line插入,四节line插入为中电压,检测脚都为AD脚
*
*/
u16 BOEU_Get_Line_det_io_val(void)
{
    
     u16 Val;
	 
  #if Z__CFG_LINEIN_DET_EN==1
    switch(Z__LINEIN_DET_GPIO)
    {
         case GPIOA20:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX20);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA20_A23); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX20);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX23);
                break;
        case GPIOA21:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX21);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA21_A24); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX21);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX24);
                break;
        case GPIOA22:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX22);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA22_A25); 
               GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX22);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX25);
                break;
        case GPIOA23:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX23);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA20_A23); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX23);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX20);
                break;
        case GPIOA24:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX24);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA21_A24); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX21);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX24);
                break;
        case GPIOA25:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX25);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA22_A25); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX22);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX25);
                break;
        case GPIOA26:  
               // GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX25);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA26);
                break;
		case GPIOA30:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX30);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA30); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX30);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX30);
                break;
		case GPIOA31:  
                GPIO_RegOneBitSet(GPIO_A_ANA_EN, GPIO_INDEX31);
                Val = ADC_SingleModeDataGet(ADC_CHANNEL_GPIOA31); 
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX31);
                GPIO_RegOneBitClear(GPIO_A_ANA_EN, GPIO_INDEX31);
                break;

			
	}


     return Val;
  
  #endif
}


		
extern BT_MANAGER_ST    btManager;

/*
*  蓝牙无连接自动关机计时
*/
void BOEU_BT_No_Connection_Timing_Power_Off(void)
{
     // 在1ms里调,时间精确点
     //MessageContext		msgSend;
     static u16 _1ms_cnt = 0;
	 static u16 _1s_cnt = 0;
	// static u8 Curr_Mode;
	 static u8 power_off_msg_send_flag = 1;
	 

	 _1ms_cnt++;// 1ms++

	 if(_1ms_cnt >= 1000)
	 {
          _1ms_cnt = 0;

			  if(GetSystemMode() == ModeBtAudioPlay)
			  {
	               if(btManager.btLinkState == 0)
	               {
	                    _1s_cnt++;
						if(_1s_cnt >= (Z_No_Connection_Timing_Power_Off_EN*60))
						{
						      _1s_cnt=0;
	                         // if(power_off_msg_send_flag == 1)
	                          {
	                               // power_off_msg_send_flag = 0;
								   MessageContext		msgSend;
	                               Custom_Event1_number = Custom_Event1_Poweroff;
	                    		   msgSend.msgId = Custom_Event2;
		                           MessageSend(mainAppCt.msgHandle, &msgSend);
							  }
						}
						
				   }
				   else
				   {
	                     _1s_cnt = 0;
				   }
			  }
			  else
			  {
	               _1s_cnt = 0;
			  }

          

	 }

}


/*
*  蓝牙暂停播放自动关机计时
*/
void BOEU_BT_Pause_Timing_Power_Off(void)
{
   
     //MessageContext		msgSend;
     static u16 _1ms_cnt = 0;
	 static u16 _1s_cnt = 0;
	// static u8 Curr_Mode;
	 static u8 power_off_msg_send_flag = 1;
	 

	 _1ms_cnt++;// 1ms++

	 if(_1ms_cnt >= 1000)
	 {
          _1ms_cnt = 0;

			  if(GetSystemMode() == ModeBtAudioPlay)
			  {
	               if(GetBtPlayState()==BT_PLAYER_STATE_PAUSED)
	               {
	                    _1s_cnt++;
                     
						if(_1s_cnt >= (Z_BT_Pause_Timing_Power_Off_EN*60))
						{
	                          if(power_off_msg_send_flag == 1)
	                          {
	                                power_off_msg_send_flag = 0;
									
									MessageContext		msgSend;
	                                Custom_Event1_number = Custom_Event1_Poweroff;
	                    			msgSend.msgId = Custom_Event2;
		                           MessageSend(mainAppCt.msgHandle, &msgSend);
							  }
						}
						
				   }
				   else
				   {
	                     _1s_cnt = 0;
				   }
			  }
			  else
			  {
	               _1s_cnt = 0;
			  }

          

	 }

}




/*
*  T卡\U盘暂停播放自动关机计时
*/
void BOEU_Media_Pause_Timing_Power_Off(void)
{

#if fun_sd_en || fun_udisk_en  

     //MessageContext		msgSend;
     static u16 _1ms_cnt = 0;
	 static u16 _1s_cnt = 0;
	// static u8 Curr_Mode;
	 static u8 power_off_msg_send_flag = 1;
	 

	 _1ms_cnt++;// 1ms++

	 if(_1ms_cnt >= 1000)
	 {
          _1ms_cnt = 0;
             
			  if((GetSystemMode() == ModeCardAudioPlay)
              #if fun_udisk_en
               || (GetSystemMode() == ModeUDiskAudioPlay)
			  #endif
			  )
			  {
	               if(GetMediaPlayerState() == PLAYER_STATE_PAUSE)
	               {
	                    _1s_cnt++;
                      
						if(_1s_cnt >= (Z_Media_Pause_Timing_Power_Off_EN*60))
						{
	                          if(power_off_msg_send_flag == 1)
	                          {
	                                power_off_msg_send_flag = 0;
									
									MessageContext		msgSend;
	                                Custom_Event1_number = Custom_Event1_Poweroff;
	                    			msgSend.msgId = Custom_Event2;
		                           MessageSend(mainAppCt.msgHandle, &msgSend);
							  }
						}
						
				   }
				   else
				   {
	                     _1s_cnt = 0;
				   }
			  }
			  else
			  {
	               _1s_cnt = 0;
			  }

          

	 }
	 
#endif

}


void Use_Encoder_fun(void)
{

	     u8 det_Encoder_A_tata=0;
		 u8 det_Encoder_B_tata=0;
		 static u8 cw_flag=0; //顺时针标志
		 static u8 ccw_flag=0;  //逆时针标志
		 static u8 flag=0;


    if(!Z_init_ok)
    {
         return;
	}
		 
		 det_Encoder_A_tata =  IO_in_check(Encoder_A_pin);
		 det_Encoder_B_tata =  IO_in_check(Encoder_B_pin);
		 


		if(det_Encoder_A_tata==1&&det_Encoder_B_tata==1)
		  {
              
		     return;
		  }
		

		 /*
           时序参考 :  CFG_RES_CODE_KEY_USE
		 */

          if(det_Encoder_A_tata==0)
          {
             // if(flag==0)
              {
                   if(det_Encoder_B_tata==1)
	               {
	                    cw_flag=1;
						ccw_flag=0;
				   }
				  
				   flag = 1;
			  }
               
		  }

		  if(det_Encoder_B_tata==0)
		  {
              if(det_Encoder_A_tata==1)
               {
                    cw_flag=0;
					ccw_flag=1;
			   }
		  }

		  
     
          if(det_Encoder_A_tata==0&&det_Encoder_B_tata==0&&cw_flag==1)
          {                       
                 cw_flag=0;
				 flag=0;
                 //main_msg_send(MSG_SW_VOL_UP);  
				 
				//printf("L XI  顺时针 111111111111111111\n");
			 
		  } 

          
          if(det_Encoder_B_tata==0&&det_Encoder_A_tata==0&&ccw_flag==1)
          {          
             
                 ccw_flag=0;
				 flag=0;

				// main_msg_send(MSG_SW_VOL_DOWN); 
				//printf("L XI  逆时针  222222222222222222222222\n");
		  } 



	 
}





#ifdef DISP_DEV_AIP1668_LED


u16 music_vol_DispBuff=0;
u16 echo_vol_DispBuff=0;
u16 mon_vol_DispBuff=0;
u16 high_vol_DispBuff=0;
u16 bass_vol_DispBuff=0;





u8 led_screen_sw=0;  // 开关屏标志 ； 1值开启；0值关闭
u8 background_light_level;
u8 background_light_level_back;

u8 aip1668_buff[16];
u8 aip1668_show_buff[16]; //显示缓存
extern uint8_t gDispBuff[];
extern bool gLedDispRefreshFlag;
void AIP1668_Delay(unsigned int us)//200ns
{
	int i;
	/*for(i=0;i<us;i++)
	{
		//200ns@320M
		__asm __volatile__(
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n""nop\n""nop\n"
		"nop\n"
		);
	}*/
	for(i=0;i<us;i++)
	{
		//@320M
		__asm __volatile__(
		"nop\n"
		);
	}

	
}



#define AIP1668_DLY(x)			AIP1668_Delay((x)*1)//AIP1668_Delay((x)*5)

#define AIP1668_Delay_EN           0 //通讯是否要延时   boeu



void bit_turn(u8 byte,u8 *pdata,u8 cnt)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(byte&BIT(i))
		{
			if(i<4){pdata[cnt*2]|=(BIT(i*2)|BIT((i*2)+1));}
			else {pdata[cnt*2+1]|=(BIT((i-4)*2)|BIT((i-4)*2+1));}	
		}
	}
}
void data_turn(u8* buff)
{
	u8 i;
	
	for(i=0;i<4;i++)
	{
		bit_turn(*buff++,aip1668_show_buff,i);
		if(i>1)
		{
			aip1668_show_buff[(i<<1)+4]|=(aip1668_buff[(i<<1)]|((aip1668_buff[(i<<1)+1]&0x01)<<7));
			aip1668_show_buff[(i<<1)+5]|=(aip1668_buff[(i<<1)+1]>>1)|(aip1668_buff[(i<<1)]&0x80);			
		}
	}		
}

void aip1668_io_init(void)
{
	AIP1668_IO_INIT();
}
void aip1668_write_byte(u8 dat)
{

  //printf("aip1668_write_byte()\n");

	u8 i,temp;
	temp=dat;

#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif

	for(i=0;i<8;i++)
	{		
		if(temp&0x01)
		{
			AIP1668_DIO(1);
		}
		else
		{
			AIP1668_DIO(0);
		}
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
		AIP1668_CLK(0);
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
		AIP1668_CLK(1);
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
		temp>>=1;
	}
	AIP1668_CLK(1);
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
}

void aip1668_write_cmd(u8 cmd)
{
	AIP1668_STB(0);
	aip1668_write_byte(cmd);
	AIP1668_STB(1);	
}
void aip1668_write_ndata(u8 *pdata,u8 n)
{
	u8 i;
	AIP1668_STB(0);
	aip1668_write_byte(0xc0);
	for(i=0;i<n;i++)
	{
		aip1668_write_byte(*pdata);
		pdata++;
	}
	
	AIP1668_STB(1);	
}
void aip1668_set_bright(u8 level)
{
	AIP1668_STB(0);
	aip1668_write_cmd((0x80|(led_screen_sw<<3))|(level&0x07));
	AIP1668_STB(1);	
}
void aip1668_init(void)
{
	aip1668_io_init();
	//AIP1668_DLY(1);
	//aip1668_write_cmd(0x01);
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
    aip1668_write_cmd(0x40);
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
	aip1668_write_cmd(0xc0);
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
	//aip1668_write_cmd(0x8f);
	aip1668_write_cmd((0x80|(led_screen_sw<<3))|(background_light_level&0x07));
	
#if AIP1668_Delay_EN	
	AIP1668_DLY(1);
#endif
	aip1668_write_cmd(0xc0);	
}

void icon_deal(void)
{




 
     

}

void show_data_deal(void)
{
	
    u8 i=0;


	/*aip1668_show_buff[0]|=gDispBuff[0];
	aip1668_show_buff[2]|=gDispBuff[1];
	aip1668_show_buff[4]|=gDispBuff[2];
	aip1668_show_buff[6]|=gDispBuff[3];*/


	  for(i=0;i<16;i++)
	  {
          aip1668_buff[i] = gDispBuff[i];
		 
	  }

	
}

void set_show_data(u8 buf,u8 data)
{

   aip1668_buff[buf] = data;
}

void buff_data_deal_scan(void)
{
	if(gLedDispRefreshFlag == FALSE)
	{
		return;
	}


	//printf("buff_data_deal_scan(void)\n");
		memset(aip1668_buff,0x00,sizeof(aip1668_buff));
		memset(aip1668_show_buff,0x00,sizeof(aip1668_show_buff));
		//icon_deal();
		show_data_deal();
		
}
void aip1668_deal(void)
{
	static u8 cnt=0;

#if 1
       
		//aip1668_write_ndata(aip1668_show_buff,16);	
		 aip1668_write_ndata(aip1668_buff,16);

		   
#else	
{
	u8 i;
	for(i=0;i<16;i++)
	{
		aip1668_buff[i]=0xff;
		
	}
	
}	
		/*aip1668_buff[0]|=0xff;
		aip1668_buff[1]|=0xff;
		aip1668_buff[2]|=0xff;
		aip1668_buff[3]|=0xff;*/
		aip1668_write_ndata(aip1668_buff,16);

#endif		

}



#endif 



void aip1668_deal_test(void)
{
   static u8 flag=0;
   static u8 c=0;
   static u8 cnt=0;
   static u8 buf=0;
   static u8 j=0;
  


     cnt++;
	if(cnt>2)
	{
	   
		aip1668_init();	
		cnt=0;
		
	}

	

	u8 i;

#if 0  //逐个亮灯,查询每个LED的数据位置
	if(!tempffgg)
	{ 
        return;
	}
	
	c++;

	if(c>=1&&tempffgg)// tempffgg 可做按键触发一下
	{
       c=0;


        aip1668_buff[buf] |= 1 << j;
        printf("buf == %d        j == %d\n",buf,j);
		 
		j++;
		if(j >= 8)
		{
            j=0;
			buf++;
			if(buf >= 16)
			{
               buf=0;
			}
		}
		
       
		    			
		 aip1668_write_ndata(aip1668_buff,16);

		 tempffgg=0;
	
	}
	
#else  //全部点亮

    for(i=0;i<16;i++)
	{
		aip1668_buff[i]=0xff;
	
	}



      aip1668_write_ndata(aip1668_buff,16);

#endif


}


TIMER	TonePlayTimer;

/*
    开机时,未连接蓝牙播1分钟提示音
*/
void PowerOnBtPairTonePlay(void)
{
   static u8 poweron_flg=1;
   static u8 _1s_cnt=0;

   if(Idle_sw.idle_mode == off_line)
   {
     
      if(BtPairTonePlay==1 && btManager.btLinkState == 0 && IsTimeOut(&TonePlayTimer))
      {
          TimeOutSet(&TonePlayTimer, 1000);  
		  		  
		  if(BtPairTonePlay==1)
		  {
		      _1s_cnt++;
			  //APP_DBG("PowerOnBtPairTonePlay()  _1s_cnt == %d\n",_1s_cnt);
			  if(_1s_cnt >= 60)
			  {
	             _1s_cnt=0;
				 BtPairTonePlay=0xff;
			  }
		  
		     if(Tone_play_state==0)
		     {
                Z_post_msg(Custom_Event2,Custom_Event1_tone_BtPair);
		     }
		  }
	  }
	  else if( (BtPairTonePlay==1 && btManager.btLinkState == 1)
	  	     || BtPairTonePlay==1 && GetBtManager()->twsState == BT_TWS_STATE_CONNECTED)
	  {
          BtPairTonePlay=0xff;
		  _1s_cnt=0;
	  }
	  	
   }
   else
   {
      if(BtPairTonePlay != 0) BtPairTonePlay=0;
      if(_1s_cnt>0) _1s_cnt=0;
   }
  
 

}






