#ifndef __zx_top3__
#define __zx_top3__


#include "zx_top2.h"
#include "type.h"






#if Z__DEBUG_custom

    #define Z__DEBUG_SYS        1 //必须开启
    
    #define  Z__DEBUG_BAT        0 //电量部分
    #define  Z__DEBUG_IOKEY      0 //IO按键部分
    #define  Z__DEBUG_ADKEY      0 //AD按键部分
    #define  Z__DEBUG_Msg1       0 //
    #define  Z__DEBUG_Msg2       0 //
    #define  Z__DEBUG_Msg3       0 //
    #define  Z__DEBUG_task      0//  
    #define  Z__DEBUG_Timing    0// 
    #define  Z__DEBUG_Tuch      0//  
    #define  Z__DEBUG_LED       0//  
    #define  Z__DEBUG_FM        0//   FM 调试
    #define  Z__DEBUG_BT        0//   FM 调试
    #define  Z__DEBUG_REC       0//   录音模式
    #define  Z__DEBUG_AudEff    0//   音效调试
    #define  Z__DEBUG_ALL       0 //以上
   
#endif






#if Z__CFG_FUNC_BT_VOL_EN||Z__CFG_FUNC_MIC_CFG_FUNC_MIC_VOLUME_EN||Z__CFG_FUNC_MIC_ECHO_REVERB_GAIN_EN||Z__CFG_FUNC_MIC_TREB_EN||Z__CFG_FUNC_MIC_BASS_EN

#define Z__rt2                   1 

#endif




#if IO_KEY_10_EN||IO_KEY_11_EN||IO_KEY_12_EN||IO_KEY_13_EN

#define Z_IOKEY          1      //
#else
#define Z_IOKEY          0      //

#endif

#if Z_pwoerkey_EN||Z_ADkey_1_EN||Z_ADkey_2_EN

#define Z_ADKEY          1      //
#else
#define Z_ADKEY          0      //

#endif



#if Pin_IR==Port_B6
#define _Pin_IR          IR_GPIOB6 
#elif Pin_IR==Port_B7
#define _Pin_IR          IR_GPIOB7 
#elif Pin_IR==Port_A29
#define _Pin_IR          IR_GPIOA29 
#endif



#define _MIC_NUM	0

#if (Phone_MIC_NUM==1)||(Built_in_MIC_NUM==1)
#undef  _MIC_NUM	
#define _MIC_NUM	1
#endif


#if (Phone_MIC_NUM==2)||(Built_in_MIC_NUM==2)
#undef  _MIC_NUM	
#define _MIC_NUM	2
#endif

#if (Phone_MIC_NUM==3)||(Built_in_MIC_NUM==3)
#undef  _MIC_NUM	
#define _MIC_NUM	3
#endif




//-------------------------------------ADKEY1 转换

#if Pin_ADK1==Port_A20
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA20_A23
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX20
#elif Pin_ADK1==Port_A21
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA21_A24
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX21
#elif Pin_ADK1==Port_A22
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA22_A25
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX22
#elif Pin_ADK1==Port_A23
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA20_A23
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX23
#elif Pin_ADK1==Port_A24
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA21_A24
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX24
#elif Pin_ADK1==Port_A25
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA22_A25
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX25
#elif Pin_ADK1==Port_A26
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA26
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX26
#elif Pin_ADK1==Port_A27
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA27
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX27
#elif Pin_ADK1==Port_A28
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA28
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX28
#elif Pin_ADK1==Port_A29
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA29
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX29
#elif Pin_ADK1==Port_A30
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA30
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX30
#elif Pin_ADK1==Port_A31
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOA31
#define _Pin_ADK1_type      GPIO_A_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX31
#elif Pin_ADK1==Port_B0
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOB0
#define _Pin_ADK1_type      GPIO_B_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX0
#elif Pin_ADK1==Port_B1
#define _Pin_ADK1_ch        ADC_CHANNEL_GPIOB1
#define _Pin_ADK1_type      GPIO_B_ANA_EN
#define _Pin_ADK1_INDEX     GPIO_INDEX1
#endif


//-------------------------------------ADKEY2 转换

#if Pin_ADK2==Port_A20
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA20_A23
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX20
#elif Pin_ADK2==Port_A21
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA21_A24
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX21
#elif Pin_ADK2==Port_A22
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA22_A25
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX22
#elif Pin_ADK2==Port_A23
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA20_A23
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX23
#elif Pin_ADK2==Port_A24
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA21_A24
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX24
#elif Pin_ADK2==Port_A25
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA22_A25
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX25
#elif Pin_ADK2==Port_A26
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA26
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX26
#elif Pin_ADK2==Port_A27
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA27
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX27
#elif Pin_ADK2==Port_A28
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA28
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX28
#elif Pin_ADK2==Port_A29
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA29
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX29
#elif Pin_ADK2==Port_A30
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA30
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX30
#elif Pin_ADK2==Port_A31
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOA31
#define _Pin_ADK2_type      GPIO_A_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX31
#elif Pin_ADK2==Port_B0
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOB0
#define _Pin_ADK2_type      GPIO_B_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX0
#elif Pin_ADK2==Port_B1
#define _Pin_ADK2_ch        ADC_CHANNEL_GPIOB1
#define _Pin_ADK2_type      GPIO_B_ANA_EN
#define _Pin_ADK2_INDEX     GPIO_INDEX1
#endif









#if Pin_BAT_check==Port_A20
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA20_A23
#elif Pin_BAT_check==Port_A21
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA21_A24
#elif Pin_BAT_check==Port_A22
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA22_A25
#elif Pin_BAT_check==Port_A23
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA20_A23
#elif Pin_BAT_check==Port_A24
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA21_A24
#elif Pin_BAT_check==Port_A25
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA22_A25
#elif Pin_BAT_check==Port_A26
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA26
#elif Pin_BAT_check==Port_A27
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA27
#elif Pin_BAT_check==Port_A28
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA28
#elif Pin_BAT_check==Port_A29
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA29
#elif Pin_BAT_check==Port_A30
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA30
#elif Pin_BAT_check==Port_A31
#define _Pin_bat_ch        ADC_CHANNEL_GPIOA31
#elif Pin_BAT_check==Port_B0
#define _Pin_bat_ch        ADC_CHANNEL_GPIOB0
#elif Pin_BAT_check==Port_B1
#define _Pin_bat_ch        ADC_CHANNEL_GPIOB1
#endif






//-------------------------------------MUTE 转换
#if PA1_mute_pin==Port_A10
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX10

#elif PA1_mute_pin==Port_A17
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX17

#elif PA1_mute_pin==Port_A20
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX20

#elif PA1_mute_pin==Port_A21
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX21

#elif PA1_mute_pin==Port_A22
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX22

#elif PA1_mute_pin==Port_A23
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX23

#elif PA1_mute_pin==Port_A24
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX24

#elif PA1_mute_pin==Port_A25
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX25

#elif PA1_mute_pin==Port_A26
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX26

#elif PA1_mute_pin==Port_A27
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX27

#elif PA1_mute_pin==Port_A28
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX28

#elif PA1_mute_pin==Port_A29
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX29

#elif PA1_mute_pin==Port_A30
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX30

#elif PA1_mute_pin==Port_A31
#define _MUTE_CTL_PORT        A
#define _MUTE_CTL_PIN         GPIO_INDEX31

#elif PA1_mute_pin==Port_B0
#define _MUTE_CTL_PORT        B
#define _MUTE_CTL_PIN         GPIO_INDEX0

#elif PA1_mute_pin==Port_B1
#define _MUTE_CTL_PORT        B
#define _MUTE_CTL_PIN         GPIO_INDEX1

#elif PA1_mute_pin==Port_B6
#define _MUTE_CTL_PORT        B
#define _MUTE_CTL_PIN         GPIO_INDEX6

#endif











#if 1//LEDS_mix_RGB_EN

#define MAX_led_grade           100

#if LEDS_RGB_R_OPEN
#define LEDS_RGB_R_ON           LEDS_RGB_R_IO_H;
#define LEDS_RGB_R_OFF          LEDS_RGB_R_IO_L; 
#else
#define LEDS_RGB_R_ON           LEDS_RGB_R_IO_L;
#define LEDS_RGB_R_OFF          LEDS_RGB_R_IO_H; 
#endif

#if LEDS_RGB_G_OPEN
#define LEDS_RGB_G_ON           LEDS_RGB_G_IO_H;
#define LEDS_RGB_G_OFF          LEDS_RGB_G_IO_L; 
#else
#define LEDS_RGB_G_ON           LEDS_RGB_G_IO_L;
#define LEDS_RGB_G_OFF          LEDS_RGB_G_IO_H; 
#endif

#if LEDS_RGB_B_OPEN
#define LEDS_RGB_B_ON           LEDS_RGB_B_IO_H;
#define LEDS_RGB_B_OFF          LEDS_RGB_B_IO_L; 
#else
#define LEDS_RGB_B_ON           LEDS_RGB_B_IO_L;
#define LEDS_RGB_B_OFF          LEDS_RGB_B_IO_H; 
#endif

#endif


#if ir_default_en

#undef Remote_UserCode
#define Remote_UserCode              0xff00        //红外遥控用户码  

#undef Z__IRKEY
#if Remote_UserCode==0xff00
#define Z__IRKEY \
        {\
            IR_ba,{{_Key_click__ ,},\
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_b9,{{_Key_click__ ,_Key_fun1_mode}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_b8,{{_Key_click__ ,_Key_fun1_mute}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_bb,{{_Key_click__ ,_Key_fun1_pp }, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,_Key_fun1_fm_scan }, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_bf,{{_Key_click__ ,_Key_fun1_prev}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_bc,{{_Key_click__ ,_Key_fun1_next}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_f8,{{_Key_click__ ,_Key_fun1_EQ}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_ea,{{_Key_click__ ,_Key_fun1_volume_down}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,_Key_fun1_volume_down}, \
        		   {_Key_hold___ ,_Key_fun1_volume_down}},\
	    },\
        {\
            IR_f6,{{_Key_click__ ,_Key_fun1_volume_up}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,_Key_fun1_volume_up}, \
        		   {_Key_hold___ ,_Key_fun1_volume_up}},\
	    },\
        {\
            IR_e9,{{_Key_click__ ,_Key_MSG_0}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_e6,{{_Key_click__ ,_Key_fun1_music_mode}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,_Key_fun1_FM_sel_mode}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_f2,{{_Key_click__ ,_key_fun1_Vol_Set}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_f3,{{_Key_click__ ,_Key_MSG_1}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_e7,{{_Key_click__ ,_Key_MSG_2}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_a1,{{_Key_click__ ,_Key_MSG_3}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_f7,{{_Key_click__ ,_Key_MSG_4}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_e3,{{_Key_click__ ,_Key_MSG_5}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_a5,{{_Key_click__ ,_Key_MSG_6}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_bd,{{_Key_click__ ,_Key_MSG_7}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
	    {\
            IR_ad,{{_Key_click__ ,_Key_MSG_8}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        {\
            IR_b5,{{_Key_click__ ,_Key_MSG_9}, \
                   {_Key_double_ ,}, \
        		   {_Key_long_1s ,}, \
        		   {_Key_hold___ ,}},\
	    },\
        


#elif (Remote_UserCode==0xfe01)  
   
#define Z__IRKEY \
	    {\
		    IR_e5,\
    			{\
    				{_Key_click__ , },\
                	{_Key_double_ ,},\
                	{_Key_double_ ,}, \
        			{_Key_long_5s,}\
    			},\
	    },\
		{\
		    IR_e4,\
    			{\
    				{_Key_click__ ,   _Key_fun1_mode  },\
                	{_Key_double_ ,},\
                	{_Key_long_1s ,}, \
        			{_Key_hold___ ,}\
    			},\
	      },\
		{\
		    IR_e7,\
    			{\
    				{_Key_click__ ,  _Key_fun1_mute   },\
                 	{_Key_double_ ,},\
                	{_Key_long_1s ,}, \
        			{_Key_hold___ ,}\
    			},\
	    },\
		{\
		    IR_e1,\
			{\
				{_Key_click__ , _Key_fun1_pp },\
				{_Key_double_ ,},\
				{_Key_long_1s ,_Key_fun1_fm_scan},\
				{_Key_hold___ ,  }\
			},\
		},\
		{\
		    IR_e0,\
			{\
				{_Key_click__ , _Key_fun1_prev },\
				{_Key_double_ ,},\
				{_Key_long_1s ,}, \
				{_Key_hold___ ,  }\
			},\
		},\
		{\
		    IR_e3,\
    			{\
    				{_Key_click__ ,_Key_fun1_next },\
    				{_Key_double_ ,},\
                	{_Key_long_1s ,},\
        			{_Key_hold___ ,}\
    			},\
	       },\
	    {\
		    IR_fc,\
    			{\
    				{_Key_click__ ,  _Key_fun1_EQ},\
                	{_Key_double_ ,},\
                	{_Key_long_1s ,},\
        			{_Key_hold___ ,}\
    	   		},\
	    },\
	    {\
		    IR_fd,\
    				{\
    					{_Key_click__ ,  _Key_fun1_volume_down },\
                		{_Key_double_ ,_Key_fun1_volume_down},\
                		{_Key_long_1s ,},\
        				{_Key_hold___ ,_Key_fun1_volume_down}\
    				},\
	    },\
	    {\
		    IR_ff,\
    		{\
    			{_Key_click__ ,_Key_fun1_volume_up },\
                {_Key_double_ ,_Key_fun1_volume_up},\
                {_Key_long_1s ,},\
        		{_Key_hold___ ,  _Key_fun1_volume_up }\
    		},\
	    },\
	    {\
		    IR_f8,\
    		{\
    			{_Key_click__ ,  _key_fun1_Vol_Set },\
                {_Key_double_ ,},\
                {_Key_long_1s ,},\
        		{_Key_hold___ ,	}\
    		},\
	    },\
	     {\
		    IR_f9,\
    		{\
    			{_Key_click__ , },\
                {_Key_double_ ,},\
                {_Key_long_1s ,},\
        		{_Key_hold___ ,  }\
    		},\
	    },\
	      {\
		    IR_fb,\
    		{\
    			{_Key_click__ , },\
                {_Key_double_ ,},\
                {_Key_long_1s ,},\
        		{_Key_hold___ ,   }\
    		},\
	    },\
		{\
		   IR_f4,\
		   {\
			   {_Key_click__ , },\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
	   {\
	   IR_f5,\
		   {\
			   {_Key_click__ ,  },\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
	   {\
	    IR_f7,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,   }\
		   },\
	   },\
		{\
		   IR_f0,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_long_1s ,}\
		   },\
	   },\
	   {\
		   IR_f1,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
	   {\
		   IR_f3,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
	   {\
		   IR_ec,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
	   { \
		   IR_ed,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
	    {\
		   IR_ef,\
		   {\
			   {_Key_click__ ,},\
			   {_Key_double_ ,},\
			   {_Key_long_1s ,},\
			   {_Key_hold___ ,}\
		   },\
	   },\
#elif (Remote_UserCode==0xEF00)  
#define Z__IRKEY \
	    {\
		    IR_fb,\
    			{\
    				{_Key_click__ , _Key_fun1_IR_powerdown },\
                		{_Key_double_ ,},\
        			{_Key_long_5s ,  }\
    			},\
	    },\
		{\
		    IR_fa,\
    			{\
    				{_Key_click__ ,   _Key_fun1_mode  },\
                		{_Key_double_ ,},\
        			{_Key_long_1s  ,       }\
    			},\
	      },\
		{\
		    IR_f9,\
    			{\
    				{_Key_click__ ,  _Key_fun1_mute   },\
                		{_Key_double_ ,},\
        			{_Key_hold___ ,   }\
    			},\
	    },\
		{\
		    IR_ee,\
			{\
				{_Key_click__ , _Key_fun1_pp },\
				{_Key_long_1s ,_Key_fun1_fm_scan},\
				{_Key_hold___ ,  }\
			},\
		},\
		{\
		    IR_f2,\
			{\
				{_Key_click__ , _Key_fun1_prev },\
				{_Key_double_ ,},\
				{_Key_hold___ ,  }\
			},\
		},\
		{\
		    IR_ea,\
    			{\
    				{_Key_click__ ,_Key_fun1_next },\
                		{_Key_long_1s ,},\
        			{_Key_hold___ ,}\
    			},\
	       },\
	    {\
		    IR_f7,\
    			{\
    				{_Key_click__ ,  _Key_fun1_EQ},\
                		{_Key_double_ ,},\
        			{_Key_hold___ ,}\
    	   		},\
	    },\
	    {\
		    IR_ef,\
    				{\
    					{_Key_click__ ,  _Key_fun1_volume_down },\
                		{_Key_double_ ,_Key_fun1_volume_down},\
        				{_Key_hold___ ,_Key_fun1_volume_down}\
    				},\
	    },\
	    {\
		    IR_ed,\
    		{\
    			{_Key_click__ ,_Key_fun1_volume_up },\
                {_Key_double_ ,_Key_fun1_volume_up},\
        		{_Key_hold___ ,  _Key_fun1_volume_up }\
    		},\
	    },\
	    {\
		    IR_f5,\
    		{\
    			{_Key_click__ ,  _key_fun1_Vol_Set },\
                {_Key_double_ ,},\
        		{_Key_hold___ ,	}\
    		},\
	    },\


#endif
#endif























































#if KT0641_EN

#define UINT32 u32
#define INT32 int
#define UINT8 u8
#define UINT16 u16
#define INT8 char





extern u8 KT0641_work_mode;
extern u8 KT0641_grade;

extern u32 Memery_Frequency;
extern u8 Key_RF_POW_flag;

void KT0641_init();
void KT0641_fre_set(u32 lv_fre);


UINT16 transmitID;

#define MANUFACTURERID				0x88
#define CHANGE_PRIVATE_ID	 			0xE0	//
#define CHANGE_TRANSMISSION	 		0xE1	//
#define CLOSE_TRANSMISSION	 			0xE2	//


#define KT0646M

//#define DOUBLE_KVCO
//#define pll_unlock
#define AUX_CH
#define SILENCE_MUTE

//#define XTAL_DUAL
#define XTAL_24M_ONLY
//#define XTAL_24P576M_ONLY

//#define RXISKT0616M

#ifdef     RXISKT0616M
    #define RXISKT0616M_BPSK
    #ifdef XTAL_DUAL
        #define RXISKT0616M_XTAL_DUAL
    #endif
#endif

#define KT_RX //使用KT芯片的接收机
//#define OTHER_RX //使用其他方案的接收机


#define KT0641_address          0x6A

#define XTAL_24M_FREQ 0
#define XTAL_24P576M_FREQ 1



#define INIT_FAIL_TH 3

#ifdef OTHER_RX
    #define PILOT_FREQ     32768
#endif

#define    BAND_TOP        960000//674250//754850
#define    BAND_BOTTOM     KT0641_pre1//755150//740150
#define    BAND_STEP       250

#define PA_SEL 1

#define PA_OFF 0
#define PA_ON  1

#define    AUDIO_UNMUTE    0
#define    AUDIO_MUTE      1

#define    WAKEUP    0
#define    _STANDBY   1

#define    FDEV_MONITOR_TC_250ms    0
#define    FDEV_MONITOR_TC_500ms    1
#define    FDEV_MONITOR_TC_1s       2
#define    FDEV_MONITOR_TC_2s       3

#define    PRE_EMPHASIS_ENABLE      0
#define    PRE_EMPHASIS_DISABLE     1

#define    MIC_SENS_GAIN_0     0
#define    MIC_SENS_GAIN_1     1
#define    MIC_SENS_GAIN_2     2
#define    MIC_SENS_GAIN_3     3
#define    MIC_SENS_GAIN_4     4
#define    MIC_SENS_GAIN_5     5
#define    MIC_SENS_GAIN_6     6
#define    MIC_SENS_GAIN_7     7
#define    MIC_SENS_GAIN_8     8
#define    MIC_SENS_GAIN_9     9
#define    MIC_SENS_GAIN_10    10
#define    MIC_SENS_GAIN_11    11
#define    MIC_SENS_GAIN_12    12
#define    MIC_SENS_GAIN_13    13
#define    MIC_SENS_GAIN_14    14
#define    MIC_SENS_GAIN_15    15

#define    COMPANDOR_ENABLE    0
#define    COMPANDOR_DISABLE   1

#define    COMPANDOR_TC_6ms    0
#define    COMPANDOR_TC_12ms   1
#define    COMPANDOR_TC_24ms   2
#define    COMPANDOR_TC_48ms   3
#define    COMPANDOR_TC_93ms   4
#define    COMPANDOR_TC_199ms  5
#define    COMPANDOR_TC_398ms  6
#define    COMPANDOR_TC_796ms  7

#ifdef SILENCE_MUTE
	#define SLNC_MUTE_DIS          1
	#define SLNC_MUTE_TIME         0x13
	#define SLNC_MUTE_LOW_LEVEL    4
	#define SLNC_MUTE_HIGH_LEVEL   6
	
	#define SILENCE_MUTE_ACT_MCU   0
#endif

#ifdef KT_RX //使用KT芯片的接收机
    #define    PILOT_ENABLE    1
    #define    PILOT_DISABLE   0
#endif

#ifdef OTHER_RX //使用其他方案的接收机
    #define    PILOT_ENABLE    0
    #define    PILOT_DISABLE   1
#endif
#define    PILOT_FDEV_2P5K     0
#define    PILOT_FDEV_5K       1
#define    PILOT_FDEV_7P5K     2
#define    PILOT_FDEV_10K      3

#define    BATTERY_METER_DISABLE    0
#define    BATTERY_METER_ENABLE     1
//ZDF
#define    AGC_VHIGH                3        //AGC上门限：0-15可选
#define    AGC_VLOW                 4        //AGC下门限：0-15可选
#define    AGC_ATTACK               3        //AGC ATTACK时间：0-7可选
#define    AGC_RELEASE              4        //AGC RELEASE时间：0-7可选
#define    AGC_DIS                  1//0        //AGC控制：0-自动控制；1-MCU控制

#define    GAIN_SEL                 3        //PGA增益：0:（-6dB）,1:0dB,2:6dB,3:12dB
#define    COMPEN_GAIN              1//3        //总体增益：0-0dB,1-6dB,2-12dB,3-18dB
#define    BLANK_EN                 1        //BLANK控制：0-DIS；1-EN
#define    BLANK_TIME               3        //BLANK时间：0-7可选

#define    ECHO_MCU                 1        //ECHO控制：0-旋钮控制；1-MCU控制
#define    ECHO_ENABLE              0
#define    ECHO_DISABLE             1
#define    ECHO_STRU                1        //回声大小：0-回声大；1-回声小
#define    ECHO_GAINUP              7        //输出信号进行缩放：7-13.1dB
#define    ECHO_GAINDOWN            0        //输入信号进行缩放：0-（-13dB）

//反馈比例：0-无反馈；25-反馈25/32
#define    ECHO_RATIO_0            0        
#define    ECHO_RATIO_1            1
#define    ECHO_RATIO_2            2
#define    ECHO_RATIO_3            3
#define    ECHO_RATIO_4            4
#define    ECHO_RATIO_5            5
#define    ECHO_RATIO_6            6
#define    ECHO_RATIO_7            7
#define    ECHO_RATIO_8            8
#define    ECHO_RATIO_9            9
#define    ECHO_RATIO_10           10
#define    ECHO_RATIO_11           11
#define    ECHO_RATIO_12           12
#define    ECHO_RATIO_13           13
#define    ECHO_RATIO_14           14
#define    ECHO_RATIO_15           15
#define    ECHO_RATIO_16           16
#define    ECHO_RATIO_17           17
#define    ECHO_RATIO_18           18
#define    ECHO_RATIO_19           19
#define    ECHO_RATIO_20           20
#define    ECHO_RATIO_21           21
#define    ECHO_RATIO_22           22
#define    ECHO_RATIO_23           23
#define    ECHO_RATIO_24           24
#define    ECHO_RATIO_25           25

//信号延时：0-22ms；24-207ms
#define    ECHO_DELAY_22ms         0        
#define    ECHO_DELAY_24ms         1
#define    ECHO_DELAY_27ms         2
#define    ECHO_DELAY_29ms         3
#define    ECHO_DELAY_32ms         4
#define    ECHO_DELAY_35ms         5
#define    ECHO_DELAY_39ms         6
#define    ECHO_DELAY_43ms         7
#define    ECHO_DELAY_47ms         8
#define    ECHO_DELAY_52ms         9
#define    ECHO_DELAY_57ms         10
#define    ECHO_DELAY_63ms         11
#define    ECHO_DELAY_69ms         12
#define    ECHO_DELAY_76ms         13
#define    ECHO_DELAY_84ms         14
#define    ECHO_DELAY_92ms         15
#define    ECHO_DELAY_101ms        16
#define    ECHO_DELAY_111ms        17
#define    ECHO_DELAY_122ms        18
#define    ECHO_DELAY_135ms        19
#define    ECHO_DELAY_148ms        20
#define    ECHO_DELAY_163ms        21
#define    ECHO_DELAY_179ms        22
#define    ECHO_DELAY_197ms        23
//#define    ECHO_DELAY_207ms        24

#define    EQ_ENABLE               1
#define    EQ_DISABLE              0

//均衡器频率
#define    EQ_25H      0            
#define    EQ_40H      1
#define    EQ_63H      2

#define    EQ_100H     3
#define    EQ_160H     4
#define    EQ_250H     5

#define    EQ_400H     6
#define    EQ_630H     7
#define    EQ_1KH      8

#define    EQ_1K6      9
#define    EQ_2K5      10
#define    EQ_4KH      11

#define    EQ_6K3      12
#define    EQ_10K      13
#define    EQ_16K      14

//均衡器增益
#define    EQ_GAIN_Neg12dB           0            
#define    EQ_GAIN_Neg11dB           1
#define    EQ_GAIN_Neg10dB           2
#define    EQ_GAIN_Neg9dB            3
#define    EQ_GAIN_Neg8dB            4
#define    EQ_GAIN_Neg7dB            5
#define    EQ_GAIN_Neg6dB            6
#define    EQ_GAIN_Neg5dB            7
#define    EQ_GAIN_Neg4dB            8
#define    EQ_GAIN_Neg3dB            9
#define    EQ_GAIN_Neg2dB            10
#define    EQ_GAIN_Neg1dB            11
#define    EQ_GAIN_0dB               12
#define    EQ_GAIN_Pos1dB            13
#define    EQ_GAIN_Pos2dB            14
#define    EQ_GAIN_Pos3dB            15
#define    EQ_GAIN_Pos4dB            16
#define    EQ_GAIN_Pos5dB            17
#define    EQ_GAIN_Pos6dB            18
#define    EQ_GAIN_Pos7dB            19
#define    EQ_GAIN_Pos8dB            20
#define    EQ_GAIN_Pos9dB            21
#define    EQ_GAIN_Pos10dB           22
#define    EQ_GAIN_Pos11dB           23
#define    EQ_GAIN_Pos12dB           24

#define    HARD_LIMIT               14       //最大频偏限制：0-15可选
#define    CPRS_1XLPF_BP            1        //压缩滤波器控制：0-开启；1-关闭
#define    CPRS_KNEE_DIS            0        //噪声控制：0-开启；1-关闭
#define    CPRS_THRSH               8        //噪声门限：0-15可选 18uV-14mV

#define    ALC_DIS                 1//0      //ALC控制：0-开启；1-关闭
#define    ALC_SOFTKNEE            1         //ALC拐点类型：0-硬拐点；1-软拐点
#define    ALC_VMAX                68//4     //ALC门限：0-127可选
#define    ALC_ATTACK              3         //ALC ATTACK时间：0-7可选
#define    ALC_RELEASE             6         //ALC RELEASE时间：0-11可选

//电池电压检测
#define    BATTERY_MAX             0x7FF
#define    BATTERY_HIGHTH          0x500
#define    BATTERY_MIDDLETH        0x4C0
#define    BATTERY_LOWTH           0x4A0

#define    LOWVOLTAGE_TH           1400
//#define    LOWVOLTAGE_TH        1800

#define BPSK_NEW_MODE            1//0//1        //1:new mode  0:old mode
#define AUXDATA_EN               1        //BIT 14
#define AUX_REG_NUM              1//3        //BIT 13:12
#define AUX_CARRY_NUM            3        //BIT 11:9    00:12bit    01:16bit    10:18bit    11:20bit

#define AUX_ADDRB                0x12    //BIT 15:8
#define AUX_ADDRA                0x17    //BIT 7:0

#define AUX_ADDRD                0x14    //BIT 15:8
#define AUX_ADDRC                0x13    //BIT 7:0


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void KT_MicTX_RFSwitch (void);

u8 KT_WirelessMicTx_PreInit(void);
u8 KT_WirelessMicTx_Init(void);

u8 KT_WirelessMicTx_Standby(void);
u8 KT_WirelessMicTx_WakeUp(void);
void KT_WirelessMicTx_PowerDownProcedure(void);

u8 KT_WirelessMicTx_PASW(u8 bPA_Switch);
u8 KT_WirelessMicTx_PAGain(UINT8 cPaGain);

UINT8 KT_WirelessMicTx_Fdev_Monitor(void);
u8 KT_WirelessMicTx_Mic_Sens(UINT8 cMicSens);
u8 KT_WirelessMicTx_Comp_Dis(u8 bComp_Dis);
u8 KT_WirelessMicTx_Comp_TC(UINT8 cComp_TC);
u8 KT_WirelessMicTx_MuteSel(u8 bMute_Sel);

u8 KT_WirelessMicTx_Pilot(u8 bPilot_Dis);
u8 KT_WirelessMicTx_Pilot_Fdev(UINT8 cPilot_Fdev);

#ifdef OTHER_RX
UINT8 KT_WirelessMicTx_Set_Pilot_Freq(u8 bXtal_Sel);
#endif

u8 KT_WirelessMicTx_Tune(INT32 Freq); //in KHz

UINT8 KT_WirelessMicTx_Band_Cali_Res(void);
UINT8 KT_WirelessMicTx_Set_XTAL(u8 bXtal_Sel);
UINT8 KT_WirelessMicTx_SW_XTAL_Freq(u8 bXtal_Sel);

void KT_Bus_Write(UINT8 Register_Address, UINT16 Word_Data);
UINT16 KT_Bus_Read(UINT8 Register_Address);

void KT_WirelessMicTx_ECHO(u8 bEcho_Dis,UINT8 Echo_Ratio,UINT8 Echo_Delay);
void KT_WirelessMicTx_EQSW(u8 bEq_Dis);
void KT_WirelessMicTx_EQGAIN(UINT8 Eq_Freq,UINT8 Eq_Gain);
UINT16 KT_WirelessMicTx_BatteryMeter_Read(void);
u8 KT_WirelessMicTx_BatteryMeter_SW(u8 bBatteryMeter_En);
u8 KT_WirelessMicTx_Mic_Sens(UINT8 cMicSens);



void KT0641_msg_deal(u16 lv_msg);


#else

#define KT0641_init()
#define KT0641_fre_set(...)
#define KT0641_msg_deal(...)


#endif




































#endif




