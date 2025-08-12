/**
 **************************************************************************************
 * @file    adc_key.c
 * @brief   
 *
 * @author  pi
 * @version V1.0.0
 *
 * $Created: 2018-01-11 17:30:47$
 *
 * @Copyright (C) 2018, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
#include "type.h"
#include "app_config.h"
//driver
#include "adc.h"
#include "clk.h"
#include "gpio.h"
#include "timeout.h"
#include "adc_key.h"
#include "debug.h"
#include "sadc_interface.h"
#include "rtos_api.h"
#include "zx_top1.h"
#include "zx_top2.h"
/*
 * ******************************************
 * PWER KEY标准电阻值： 
 * 0 = 16K(1.40V),  1 = 27K(1.80V), 2 = 43K(2.2V), 3= 82K(2.6V), 4 = 220K(3.0V), 
 *
 ********************************************
*/

/*
 * ******************************************
 * ADC KEY标准电阻值： 上拉 10k (3.30V)
 * 0 = 100(0.00V), 1 = 1K(0.30V),  2 = 2.2K(0.59V), 3 = 3.6K(0.87V), 4= 5.6K(1.18V), 5 = 8.2K(1.48V), 
 * 6 = 12K(1.80V), 7 = 18K(2.10V), 8 = 27K(2.40V),  9 = 47K(2.70V),  10=100K(3.00V),
 *
 ********************************************
*/

#if 0//ndef CFG_APP_CONFIG 
/*********** default config, determined by the hardware circuit **************/
#define 	CFG_RES_ADC_KEY_USE
#define		CFG_PARA_ADC_KEY_COUNT				11 		//key count per adc channel 
#define		CFG_RES_ADC_KEY_PORT_CH1			ADC_CHANNEL_GPIOA20_A23
#define		CFG_RES_ADC_KEY_CH1_ANA_EN			GPIO_A_ANA_EN
#define		CFG_RES_ADC_KEY_CH1_ANA_MASK		GPIO_INDEX23

/****************************************************************************/
#endif //no CFG_APP_CONFIG

#define     NORMAL_ADKEY                (1)    ///1 = 标准的ADKEY值处理，0 = 用户自定义ADKEY值处理
#define     POWERKEY_11_ADKEY           (0)    ///1 = 标准的11个 ADKEY值处理，0 = 5个 ADKEY值处理+1个POWER KEY功能键


#define 	ADC_KEY_SCAN_TIME			10
#define 	ADC_KEY_JITTER_TIME			30
#define     ADC_KEY_DOUBLE_TIME         300//  zsh  A2
#define 	ADC_KEY_CP_TIME				1000 	//for hold or long press
#define 	ADC_KEY_CPH_TIME			200		//for long long press
#define	    ADC_KEY_COUNT				11
#define		ADC_KEY_FULL_VAL			4096		

typedef enum _ADC_KEY_STATE
{
	ADC_KEY_STATE_IDLE,
	ADC_KEY_STATE_JITTER,
	ADC_KEY_STATE_PRESS_DOWN,
	ADC_KEY_STATE_CP,//  zsh  A2 
	ADC_KEY_STATE_CP_DOUBLE,//  zsh  A2

} ADC_KEY_STATE;

#define PWRKEY_0    ADC_030V
#define PWRKEY_1    ADC_160V
#define PWRKEY_2    ADC_200V
#define PWRKEY_3    ADC_240V
#define PWRKEY_4    ADC_280V
#define PWRKEY_5    ADC_310V


#define PWR_ADCKEY_COUNT    11

// zsh A2
#define V_ADC_1			0
#define V_ADC_2			ADC_060V
#define V_ADC_3			ADC_100V  //  2 4.7K
#define V_ADC_4			ADC_148V  //  3 8.2K
#define V_ADC_5			ADC_180V  //  4 12K
#define V_ADC_6			ADC_210V  //  5 18K
#define V_ADC_7			ADC_240V  //  6 27K
#define V_ADC_8			ADC_260V  //  7 39K
#define V_ADC_9			ADC_280V  //  8 58K
#define V_ADC_10		ADC_300V
#define V_ADC_11		ADC_316V
#define V_ADC_MAX		ADC_330V

#define ADKEY_0    (V_ADC_1+V_ADC_2)/2  //   >0.0V  <0.6V 100R 
#define ADKEY_1    (V_ADC_2+V_ADC_3)/2  //   >0.6V   <0.9V2.2k
#define ADKEY_2    (V_ADC_3+V_ADC_4)/2  //   >0.9V   <1.2V4.7K
#define ADKEY_3    (V_ADC_4+V_ADC_5)/2 //    >1.2V   <1.5V8.2K
#define ADKEY_4    (V_ADC_5+V_ADC_6)/2  //   >1.5V   <1.8V12K
#define ADKEY_5    (V_ADC_6+V_ADC_7)/2  //   >1.8V   <2.1V 18K
#define ADKEY_6    (V_ADC_7+V_ADC_8)/2  //   >2.1V   <2.4K27K
#define ADKEY_7    (V_ADC_8+V_ADC_9)/2  //   >2.4K   <2.7V39K
#define ADKEY_8    (V_ADC_9+V_ADC_10)/2 //   >2.7V   <3.0V58K
#define ADKEY_9    (V_ADC_10+V_ADC_11)/2  // >3.0V   <3.1V 100K
#define ADKEY_10   (V_ADC_11+V_ADC_MAX)/2  // >3.1V  <3.3V 220K
#define ADKEY_11   (V_ADC_10+V_ADC_10)/2
#define ADKEY_12   (ADC_000V+ADC_030V)/2
#define ADKEY_13   (ADC_000V+ADC_030V)/2
#define ADKEY_14   (ADC_000V+ADC_030V)/2
// end
// zsh A2
// powerkey 内部上拉为22K
#define P_V_ADC_1			0
#define P_V_ADC_2			ADC_030V
#define P_V_ADC_3			ADC_060V
#define P_V_ADC_4			ADC_090V
#define P_V_ADC_5			ADC_120V
#define P_V_ADC_6			ADC_150V
#define P_V_ADC_7			ADC_180V
#define P_V_ADC_8			ADC_210V
#define P_V_ADC_9			ADC_240V
#define P_V_ADC_10		    ADC_270V
#define P_V_ADC_11		    ADC_300V
#define P_V_ADC_MAX		    ADC_330V




#define P_ADKEY_0    (P_V_ADC_1+P_V_ADC_2)/2  //   >0.0V  <0.6V 100R 
#define P_ADKEY_1    (P_V_ADC_2+P_V_ADC_3)/2  //   >0.6V   <0.9V2.2k
#define p_ADKEY_2    (P_V_ADC_3+P_V_ADC_4)/2  //   >0.9V   <1.2V4.7K
#define P_ADKEY_3    (P_V_ADC_4+P_V_ADC_5)/2 //    >1.2V   <1.5V8.2K
#define P_ADKEY_4    (P_V_ADC_5+P_V_ADC_6)/2  //   >1.5V   <1.8V12K
#define P_ADKEY_5    (P_V_ADC_6+P_V_ADC_7)/2  //   >1.8V   <2.1V 18K
#define P_ADKEY_6    (P_V_ADC_7+P_V_ADC_8)/2  //   >2.1V   <2.4K27K
#define P_ADKEY_7    (P_V_ADC_8+P_V_ADC_9)/2  //   >2.4K   <2.7V39K
#define P_ADKEY_8    (P_V_ADC_9+P_V_ADC_10)/2 //   >2.7V   <3.0V58K
#define P_ADKEY_9    (P_V_ADC_10+P_V_ADC_11)/2  // >3.0V   <3.1V 100K
#define P_ADKEY_10   (P_V_ADC_11+P_V_ADC_MAX)/2  // >3.1V  <3.3V 220K
#define P_ADKEY_11   (V_ADC_10+V_ADC_10)/2
#define P_ADKEY_12   (ADC_000V+ADC_030V)/2
#define P_ADKEY_13   (ADC_000V+ADC_030V)/2
#define P_ADKEY_14   (ADC_000V+ADC_030V)/2

//end

const uint16_t UserADKey_Tab[11]=
{
	ADKEY_0,
	ADKEY_1,
	ADKEY_2,
	ADKEY_3,
	ADKEY_4,
	ADKEY_5,
	ADKEY_6,
	ADKEY_7,
	ADKEY_8,
	ADKEY_9,
	ADKEY_10,
};
//zsh A2
const uint16_t UserPWRKey_Tab[]=
{
#if Z_pwoerkey_11_EN
	P_ADKEY_0,
	P_ADKEY_1,
	p_ADKEY_2,
	P_ADKEY_3,
	P_ADKEY_4,
	P_ADKEY_5,
	P_ADKEY_6,
	P_ADKEY_7,
	P_ADKEY_8,
	P_ADKEY_9,
	P_ADKEY_10,
#else
	PWRKEY_0,
	PWRKEY_1,
	PWRKEY_2,
	PWRKEY_3,
	PWRKEY_4,
	PWRKEY_5,
#endif
};
#ifdef CFG_RES_ADC_KEY_USE
TIMER			AdcKeyMoreTimer;//按键多击时间

TIMER			AdcKeyWaitTimer;
TIMER			AdcKeyScanTimer;

ADC_KEY_STATE	AdcKeyState;
static 	uint8_t 	PreKeyIndex = ADC_CHANNEL_EMPTY;

int32_t AdcKeyInit(void)
{
	AdcKeyState = ADC_KEY_STATE_IDLE;
	PreKeyIndex = ADC_CHANNEL_EMPTY;
	TimeOutSet(&AdcKeyScanTimer, 0);

#ifdef CFG_RES_ADC_KEY_PORT_CH1
	GPIO_RegOneBitSet(CFG_RES_ADC_KEY_CH1_ANA_EN, CFG_RES_ADC_KEY_CH1_ANA_MASK);
#endif 

#ifdef CFG_RES_ADC_KEY_PORT_CH2
	GPIO_RegOneBitSet(CFG_RES_ADC_KEY_CH2_ANA_EN, CFG_RES_ADC_KEY_CH2_ANA_MASK);
#endif 

	return 0;
}


static uint8_t AdcChannelKeyGet(uint8_t Channel)
{
	uint16_t	Val,i;
	uint8_t	KeyIndex = ADC_CHANNEL_EMPTY;

	//Val = ADC_SingleModeDataGet(Channel);
	ADC_SingleModeDataStart(Channel);
	osTaskDelay(1);
	Val = ADC_SingleModeDataOut();

	if( Channel == ADC_CHANNEL_POWERKEY )
	{
		//APP_DBG("P Val = %d\n", Val);
		//if(Val < 500)
		//{
		//	return ADC_CHANNEL_EMPTY;
		//}
		if(Val >= (ADC_KEY_FULL_VAL - (ADC_KEY_FULL_VAL / ADC_KEY_COUNT) / 2))
		{
			return ADC_CHANNEL_EMPTY;
		}
	}
	else
	{
		//APP_DBG("A Val = %d\n", Val);
		if(Val >= (ADC_KEY_FULL_VAL - (ADC_KEY_FULL_VAL / ADC_KEY_COUNT) / 2))
		{
			return ADC_CHANNEL_EMPTY;
		}
	}

#ifdef CFG_RES_POWERKEY_ADC_EN
	if( Channel == ADC_CHANNEL_POWERKEY )
	{
	#if 0//POWERKEY_11_ADKEY  //zsh A2
		KeyIndex = (Val + (ADC_KEY_FULL_VAL / ADC_KEY_COUNT) / 2) * ADC_KEY_COUNT / ADC_KEY_FULL_VAL;
	#else
		for(i = 0; i < sizeof(UserPWRKey_Tab)/sizeof(UserPWRKey_Tab[0]); i++)
		{
			if(Val < UserPWRKey_Tab[i])
			{
			    #if Z_pwoerkey_11_EN
				KeyIndex = i+PowK2_Res_100R;
				#else
                KeyIndex = i+PowK1_Res_100R;
				#endif
				break;
			}
		}	
	#endif
	}
#endif

#ifdef CFG_RES_ADC_KEY_PORT_CH1
	if(Channel == CFG_RES_ADC_KEY_PORT_CH1)
	{
		#if 0//NORMAL_ADKEY // zsh A2
		KeyIndex = PWR_ADCKEY_COUNT + (Val + (ADC_KEY_FULL_VAL / ADC_KEY_COUNT) / 2) * ADC_KEY_COUNT / ADC_KEY_FULL_VAL;
		#else
		for(i = 0; i < sizeof(UserADKey_Tab)/sizeof(UserADKey_Tab[0]); i++)
		{
			if(Val < UserADKey_Tab[i])
			{
				KeyIndex = ADK1_Res___0 + i;
				break;
			}
		}		
		#endif
	}
#endif

#ifdef CFG_RES_ADC_KEY_PORT_CH2
	if(Channel == CFG_RES_ADC_KEY_PORT_CH2)
	{
		#if 0//NORMAL_ADKEY // zsh A2
		KeyIndex = PWR_ADCKEY_COUNT + ADC_KEY_COUNT + (Val + (ADC_KEY_FULL_VAL / ADC_KEY_COUNT) / 2) * ADC_KEY_COUNT / ADC_KEY_FULL_VAL;
		#else
		for(i = 0; i < sizeof(UserADKey_Tab)/sizeof(UserADKey_Tab[0]); i++)
		{
			if(Val < UserADKey_Tab[i])
			{
				KeyIndex = ADK2_Res___0 + i;
				break;
			}
		}		
		#endif
	}
#endif

	//if( KeyIndex != ADC_CHANNEL_EMPTY )
	//{
		//APP_DBG("KeyIndex = %d\n", KeyIndex);
	//}
	return KeyIndex;
}


uint8_t GetAdcKeyIndex(void)
{
	uint8_t KeyIndex = ADC_CHANNEL_EMPTY;

#ifdef CFG_RES_POWERKEY_ADC_EN
	if( KeyIndex ==  ADC_CHANNEL_EMPTY )
	{
		KeyIndex = AdcChannelKeyGet(ADC_CHANNEL_POWERKEY);
	}
#endif

#ifdef CFG_RES_ADC_KEY_PORT_CH1
	if(KeyIndex == ADC_CHANNEL_EMPTY)
	{
		//GPIO_RegOneBitSet(CFG_RES_ADC_KEY_CH1_ANA_EN, CFG_RES_ADC_KEY_CH1_ANA_MASK);  //若是用到复用的ADC口，例如A20和A23口做ADC，需要打开此代码
		KeyIndex = AdcChannelKeyGet(CFG_RES_ADC_KEY_PORT_CH1);
		//GPIO_RegOneBitClear(CFG_RES_ADC_KEY_CH1_ANA_EN, GPIO_INDEX20);                //若是用到复用的ADC口，例如A20和A23口做ADC，需要打开此代码
	    //GPIO_RegOneBitClear(CFG_RES_ADC_KEY_CH1_ANA_EN, CFG_RES_ADC_KEY_CH1_ANA_MASK);//若是用到复用的ADC口，例如A20和A23口做ADC，需要打开此代码
	}
#endif

#ifdef CFG_RES_ADC_KEY_PORT_CH2
	if(KeyIndex == ADC_CHANNEL_EMPTY)
	{
		KeyIndex = AdcChannelKeyGet(CFG_RES_ADC_KEY_PORT_CH2);
	}
#endif

	return KeyIndex;
}


/**
 * @func        AdcKeyScan
 * @brief       AdcKeyScan 按下有消抖，弹起没有消抖处理。
 * @param       void  
 * @Output      None
 * @return      AdcKeyMsg, ADC_CHANNEL_EMPTY or--KeyType & KeyIndex: 0 ~ CFG_PARA_ADC_KEY_COUNT * Channels
 * @Others      
 * Record
 * 1.Date        : 20180123
 *   Author      : pi.wang
 *   Modification: Created function
*/
#if 0////////// Z_key_double_EN 
AdcKeyMsg AdcKeyScan(void)
{
	uint8_t				KeyIndex=ADC_CHANNEL_EMPTY;
	AdcKeyMsg Msg = {ADC_CHANNEL_EMPTY, ADC_KEY_UNKOWN_TYPE};

//	if(!IsTimeOut(&AdcKeyScanTimer))
//	{
//		return Msg;
//	}
//	TimeOutSet(&AdcKeyScanTimer, ADC_KEY_SCAN_TIME);
	
	KeyIndex = GetAdcKeyIndex();
	
	switch(AdcKeyState)
	{
		case ADC_KEY_STATE_IDLE:
			if(KeyIndex == ADC_CHANNEL_EMPTY)
			{
				return Msg;
			}
			PreKeyIndex = KeyIndex;
			TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_JITTER_TIME);
			//APP_DBG("GOTO JITTER!\n");
			AdcKeyState = ADC_KEY_STATE_JITTER;

		case ADC_KEY_STATE_JITTER:
			if(PreKeyIndex != KeyIndex)
			{
				//APP_DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = ADC_CHANNEL_EMPTY;
				AdcKeyState = ADC_KEY_STATE_IDLE;
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))
			{
				//APP_DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CP_TIME);
				AdcKeyState = ADC_KEY_STATE_PRESS_DOWN;
				Msg.index = PreKeyIndex;
				Msg.type = ADC_KEY_PRESSED;
			}
			break;

		case ADC_KEY_STATE_PRESS_DOWN:
			if(PreKeyIndex != KeyIndex)
			{
				APP_DBG("key release %d\n", PreKeyIndex);
				AdcKeyState = ADC_KEY_STATE_IDLE;
				Msg.index = PreKeyIndex;
				Msg.type = ADC_KEY_RELEASED;
				Power_Key_Releasd=TRUE;// zsh A2
				PreKeyIndex = ADC_CHANNEL_EMPTY;
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))
			{
				//return key cp value
				//APP_DBG("ADC KEY CPS!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CPH_TIME);
				AdcKeyState = ADC_KEY_STATE_CP;
				Msg.index = PreKeyIndex;
				Msg.type = ADC_KEY_LONG_PRESSED;
			}
			break;

		case ADC_KEY_STATE_CP:
			if(PreKeyIndex != KeyIndex)
			{
				//return key cp value
				//APP_DBG("ADC KEY CPR!\n");
				AdcKeyState = ADC_KEY_STATE_IDLE;
				Msg.index = PreKeyIndex;
				Msg.type  = ADC_KEY_LONG_RELEASED;
				PreKeyIndex = ADC_CHANNEL_EMPTY;
				Power_Key_Releasd=TRUE;//  zsh  A2
				/*#if Z_MAX_MIN_TONE_MSG_SEND_At_a_time_EN
                  max_min_tone_msg_send_en = TRUE;  
			   #endif*/
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))
			{
				//return key cph value
				//APP_DBG("ADC KEY CPH!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CPH_TIME);
				Msg.index = PreKeyIndex;
				Msg.type  = ADC_KEY_LONG_PRESS_HOLD;
			}
			break;

		default:
			AdcKeyState = ADC_KEY_STATE_IDLE;
			PreKeyIndex = ADC_CHANNEL_EMPTY;
			break;
	}

	return Msg;
}
#elif 0 

/*
     writer: 博益友    BOEU 
     增加多击处理
*/
AdcKeyMsg AdcKeyScan(void)
{
	uint8_t				KeyIndex=ADC_CHANNEL_EMPTY;
	AdcKeyMsg Msg = {ADC_CHANNEL_EMPTY, ADC_KEY_UNKOWN_TYPE};
	static u8 key_click_cnt = 0;
	static u8 Save_KeyIndex = ADC_CHANNEL_EMPTY;
	static u8 Save_KeyIndex2 = ADC_CHANNEL_EMPTY;
    static u8 Key_More_flag = 0;
    static u8 SetKeyMoreTimerFlag = 1;
	

//	if(!IsTimeOut(&AdcKeyScanTimer))
//	{
//		return Msg;
//	}
//	TimeOutSet(&AdcKeyScanTimer, ADC_KEY_SCAN_TIME);
	
	KeyIndex = GetAdcKeyIndex();
	
	switch(AdcKeyState)
	{
		case ADC_KEY_STATE_IDLE:

           if(key_click_cnt > 0 && IsTimeOut(&AdcKeyMoreTimer))//多击等待到时
           {
                //根据短按击次判断单击,双击,三击
                  if(key_click_cnt==1)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = ADC_KEY_RELEASED;//短按按下(消息发送)
					  PreKeyIndex = ADC_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("单击消息发送\n");
				  }
				  else if(key_click_cnt==2)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = ADC_KEY_DOUBLE_RELEASED;//双击
					  PreKeyIndex = ADC_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("双击消息发送\n");
				  }
				  else if(key_click_cnt>=3)//如有更多击次,需修改此判断
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = ADC_KEY_3_CLICK_RELEASED;// 3 击
					  PreKeyIndex = ADC_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("3击消息发送\n");
				  }
				  Save_KeyIndex = ADC_CHANNEL_EMPTY;
				  Save_KeyIndex2 = ADC_CHANNEL_EMPTY;
				  key_click_cnt = 0;
				  Key_More_flag = 0;
				  SetKeyMoreTimerFlag = 1;
				  return Msg;
		   }


			
			if(KeyIndex == ADC_CHANNEL_EMPTY)//按键抬起
			{
				return Msg;//空消息
			}
			PreKeyIndex = KeyIndex;//按键按下
			TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_JITTER_TIME);//设置消抖计时
			//APP_DBG("GOTO JITTER!\n");
			AdcKeyState = ADC_KEY_STATE_JITTER;//设置按键消抖状态

		case ADC_KEY_STATE_JITTER://按键按下,消抖状态
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//APP_DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = ADC_CHANNEL_EMPTY;//按键无效编号
				AdcKeyState = ADC_KEY_STATE_IDLE;//按键空闲状态
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))//消抖到时
			{
				//APP_DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CP_TIME);//设置长按计时

              //根据按下的按键所定义的击次设置延时时长
               Key_More_flag = Inquire_More_Key_Msg(PreKeyIndex);
			  // APP_DBG("------------  Key_More_flag == %d\n",Key_More_flag);
				if(Key_More_flag == 0)//没有多击
				{
                     TimeOutSet(&AdcKeyMoreTimer,0);
					 SetKeyMoreTimerFlag = 1;
				}
				else if(Key_More_flag == _Key_CLICK3_)//有三击
				{
                    if(IsTimeOut(&AdcKeyMoreTimer) && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&AdcKeyMoreTimer, 800);//设置三击等待计时
					}
				}
				else if(Key_More_flag == _Key_double_)//有双击
				{
                    if(IsTimeOut(&AdcKeyMoreTimer)  && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&AdcKeyMoreTimer, 500);//设置双击等待计时
					}
				}
				
				
				AdcKeyState = ADC_KEY_STATE_PRESS_DOWN;//设置按键按下状态
				Msg.index = PreKeyIndex;
				Msg.type = ADC_KEY_PRESSED;//按键按下
				Save_KeyIndex = PreKeyIndex;
				
				key_click_cnt++;

				if(key_click_cnt == 1)
				{
                   Save_KeyIndex2 = Save_KeyIndex;
				}
				else if(key_click_cnt >= 2)
				{
                    if(Save_KeyIndex2 != Save_KeyIndex)//多击等待计时中又按下另外的按键
                    {
                        key_click_cnt =  1;//设为单击,最后松开的按键
                       // APP_DBG("按下另外的按键  设为单击\n");
					}
				}
				
			}
			break;

		case ADC_KEY_STATE_PRESS_DOWN://按键按下
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//APP_DBG("key release %d\n", PreKeyIndex);
               // APP_DBG("DC_KEY_STATE_PRESS_DOWN key_click_cnt == %d\n", key_click_cnt);
                if(key_click_cnt > 0)
                {
                  // APP_DBG("DC_KEY_STATE_PRESS_DOWN   goto IDLE\n");
                     //PreKeyIndex = ADC_CHANNEL_EMPTY;//按键无效编号
				     AdcKeyState = ADC_KEY_STATE_IDLE;//按键空闲状态
				}
		
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))//长按到时
			{
				//return key cp value
				//APP_DBG("ADC KEY CPS!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CPH_TIME);//设置持续按下时间
				AdcKeyState = ADC_KEY_STATE_CP;//设置按键持续按压状态
				Msg.index = PreKeyIndex;//按下编号
				Msg.type = ADC_KEY_LONG_PRESSED;//长按按下(消息发送)
				key_click_cnt = 0;
				Save_KeyIndex = ADC_CHANNEL_EMPTY;
			}
			break;

		case ADC_KEY_STATE_CP://按下持续
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//return key cp value
				//APP_DBG("ADC KEY CPR!\n");
				AdcKeyState = ADC_KEY_STATE_IDLE;//设置按键空闲状态
				Msg.index = PreKeyIndex;//按键编号
				Msg.type  = ADC_KEY_LONG_RELEASED;//长按释放
				PreKeyIndex = ADC_CHANNEL_EMPTY;//清除编号
				Power_Key_Releasd=TRUE;//  zsh  A2
				key_click_cnt = 0;
				Key_More_flag = 0;
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))//(长按1S到时后)按键持续按下 
			{
				//return key cph value
				//APP_DBG("ADC KEY CPH!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CPH_TIME);//设置长按持续消息时间
				Msg.index = PreKeyIndex;
				Msg.type  = ADC_KEY_LONG_PRESS_HOLD;//长按保持(消息发送)
				key_click_cnt = 0;
				Key_More_flag = 0;
			}
			break;

		default:
			
			break;
	}

	return Msg;
}

#else  

/*
     writer: 博益友    BOEU 
     增加多击处理
*/
AdcKeyMsg AdcKeyScan(void)
{
	uint8_t				KeyIndex=ADC_CHANNEL_EMPTY;
	AdcKeyMsg Msg = {ADC_CHANNEL_EMPTY, ADC_KEY_UNKOWN_TYPE};
	static u8 key_click_cnt = 0;
	static u8 Save_KeyIndex = ADC_CHANNEL_EMPTY;
	static u8 Save_KeyIndex2 = ADC_CHANNEL_EMPTY;
    static u8 Key_More_flag = 0;
    static u8 SetKeyMoreTimerFlag = 1;
	 static uint8_t key_ns_flag=0;
	static uint16_t ADC_KEY_Ns_TIME = 0;

//	if(!IsTimeOut(&AdcKeyScanTimer))
//	{
//		return Msg;
//	}
//	TimeOutSet(&AdcKeyScanTimer, ADC_KEY_SCAN_TIME);
	
	KeyIndex = GetAdcKeyIndex();
	
	switch(AdcKeyState)
	{
		case ADC_KEY_STATE_IDLE:

           if(key_click_cnt > 0 && IsTimeOut(&AdcKeyMoreTimer))//多击等待到时
           {
      
				
                //根据短按击次判断单击,双击,三击
                  if(key_click_cnt==1)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = ADC_KEY_RELEASED;//短按按下(消息发送)
					  PreKeyIndex = ADC_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("单击消息发送\n");
				  }
				  else if(key_click_cnt==2)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = ADC_KEY_DOUBLE_RELEASED;//双击
					  PreKeyIndex = ADC_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("双击消息发送\n");
				  }
				  else if(key_click_cnt>=3)//如有更多击次,需修改此判断
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = ADC_KEY_3_CLICK_RELEASED;// 3 击
					  PreKeyIndex = ADC_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("3击消息发送\n");
				  }
				  Save_KeyIndex = ADC_CHANNEL_EMPTY;
				  Save_KeyIndex2 = ADC_CHANNEL_EMPTY;
				  key_click_cnt = 0;
				  Key_More_flag = 0;
				  SetKeyMoreTimerFlag = 1;
				  return Msg;
		   }


			
			if(KeyIndex == ADC_CHANNEL_EMPTY)//按键抬起
			{
				return Msg;//空消息
			}
			PreKeyIndex = KeyIndex;//按键按下
			TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_JITTER_TIME);//设置消抖计时
			//APP_DBG("GOTO JITTER!\n");
			AdcKeyState = ADC_KEY_STATE_JITTER;//设置按键消抖状态

		case ADC_KEY_STATE_JITTER://按键按下,消抖状态
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//APP_DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = ADC_CHANNEL_EMPTY;//按键无效编号
				AdcKeyState = ADC_KEY_STATE_IDLE;//按键空闲状态

				
				
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))//消抖到时
			{
				//APP_DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CP_TIME);//设置长按计时

              //根据按下的按键所定义的击次设置延时时长
               Key_More_flag = Inquire_More_Key_Msg(PreKeyIndex);
			  // APP_DBG("------------  Key_More_flag == %d\n",Key_More_flag);
				if(Key_More_flag == 0)//没有多击
				{
                     TimeOutSet(&AdcKeyMoreTimer,0);
					 SetKeyMoreTimerFlag = 1;
				}
				else if(Key_More_flag == _Key_CLICK3_)//有三击
				{
                    if(IsTimeOut(&AdcKeyMoreTimer) && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&AdcKeyMoreTimer, 800);//设置三击等待计时
					}
				}
				else if(Key_More_flag == _Key_double_)//有双击
				{
                    if(IsTimeOut(&AdcKeyMoreTimer)  && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&AdcKeyMoreTimer, 500);//设置双击等待计时
					}
				}
				
				
				AdcKeyState = ADC_KEY_STATE_PRESS_DOWN;//设置按键按下状态
				Msg.index = PreKeyIndex;
				Msg.type = ADC_KEY_PRESSED;//按键按下
				Save_KeyIndex = PreKeyIndex;
				
				key_click_cnt++;

				if(key_click_cnt == 1)
				{
                   Save_KeyIndex2 = Save_KeyIndex;
				}
				else if(key_click_cnt >= 2)
				{
                    if(Save_KeyIndex2 != Save_KeyIndex)//多击等待计时中又按下另外的按键
                    {
                        key_click_cnt =  1;//设为单击,最后松开的按键
                       // APP_DBG("按下另外的按键  设为单击\n");
					}
				}


				 key_ns_flag = Is_key_Ns_msg(Msg.index);
				 if(key_ns_flag)
				 {
					//APP_DBG("111111111111   key_ns_flag == %d\n",key_ns_flag);
				 }
				 else
				 {
                    key_ns_flag = 0;
					//APP_DBG("0000000000000  key_ns_flag == %d\n",key_ns_flag);
				 }

				 
				
			}
			break;

		case ADC_KEY_STATE_PRESS_DOWN://按键按下
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				APP_DBG("key release %d\n", PreKeyIndex);
                if(key_click_cnt > 0)
                {
                  // APP_DBG("DC_KEY_STATE_PRESS_DOWN   goto IDLE\n");
                    
				     AdcKeyState = ADC_KEY_STATE_IDLE;//按键空闲状态
				}
		
			}
			else if(IsTimeOut(&AdcKeyWaitTimer))//长按到时
			{
				if(key_ns_flag == 0)
				{
					TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CPH_TIME);
					AdcKeyState = ADC_KEY_STATE_CP;
					Msg.index = PreKeyIndex;
					Msg.type = ADC_KEY_LONG_PRESSED;
				}
				else
				{  
				    //按下的按键有做N秒消息,先不发1秒消息
				    switch(key_ns_flag)// 在已经长按1s时间上计时
				    {
						case 2: ADC_KEY_Ns_TIME = 1000; break;
						case 3: ADC_KEY_Ns_TIME = 2000; break;
						case 4: ADC_KEY_Ns_TIME = 3000; break;
						case 5: ADC_KEY_Ns_TIME = 4000; break;
						case 6: ADC_KEY_Ns_TIME = 5000; break;
					} 
				    TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_Ns_TIME);
				    AdcKeyState = ADC_KEY_STATE_CP;
                   			
				}
			}
			break;

		case ADC_KEY_STATE_CP://按下持续
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//return key cp value
				APP_DBG("ADC KEY CPR!\n");

			    if(key_ns_flag == 0)
				{
					AdcKeyState = ADC_KEY_STATE_IDLE;
					Msg.index = PreKeyIndex;
					Msg.type  = ADC_KEY_LONG_RELEASED;
					PreKeyIndex = ADC_CHANNEL_EMPTY;
					Power_Key_Releasd=TRUE;//  zsh  A2
				}
				else
				{
				   //有定义N秒的按键,未按够N秒抬起就发1秒消息
				    AdcKeyState = ADC_KEY_STATE_IDLE;
                    Msg.index = PreKeyIndex;
					Msg.type = ADC_KEY_LONG_PRESSED;
					PreKeyIndex = ADC_CHANNEL_EMPTY;
					Power_Key_Releasd=TRUE;//  zsh  A2
					key_ns_flag = 0;
				}
			}
			else///// if(IsTimeOut(&AdcKeyWaitTimer))//(长按1S到时后)按键持续按下 
			{
				//return key cph value
				//APP_DBG("ADC KEY CPH!\n");
				if(key_ns_flag == 0)
				{
				   if(IsTimeOut(&AdcKeyWaitTimer))
				   {
						TimeOutSet(&AdcKeyWaitTimer, ADC_KEY_CPH_TIME);
						Msg.index = PreKeyIndex;
						Msg.type  = ADC_KEY_LONG_PRESS_HOLD;
						Save_KeyIndex = ADC_CHANNEL_EMPTY;
						Save_KeyIndex2 = ADC_CHANNEL_EMPTY;
						key_click_cnt = 0;
						Key_More_flag = 0;
						SetKeyMoreTimerFlag = 1;
				   }
				}
				else
				{
				    //临时代码,做N秒消息的按键,暂不支持长按保持
				   if(IsTimeOut(&AdcKeyWaitTimer))
				   {
				     
                        Msg.index = PreKeyIndex;
						switch(key_ns_flag)// 
					    {
							case 2: Msg.type  = ADC_KEY_LONG_2S_RELEASED; break;
							case 3: Msg.type  = ADC_KEY_LONG_3S_RELEASED; break;
							case 4: Msg.type  = ADC_KEY_LONG_4S_RELEASED; break;
							case 5: Msg.type  = ADC_KEY_LONG_5S_RELEASED; break;
							case 6: Msg.type  = ADC_KEY_LONG_6S_RELEASED; break;
						} 
						key_ns_flag = 0;
						//PreKeyIndex = ADC_CHANNEL_EMPTY;
				     	AdcKeyState = ADC_KEY_STATE_NS_RELEASE;

						  Save_KeyIndex = ADC_CHANNEL_EMPTY;
						  Save_KeyIndex2 = ADC_CHANNEL_EMPTY;
						  key_click_cnt = 0;
						  Key_More_flag = 0;
						  SetKeyMoreTimerFlag = 1;
				  
					//	APP_DBG(">>>>>>>>>>>>> key Msg.type %d\n", Msg.type);
				   }
				}
			}
			break;

          case ADC_KEY_STATE_NS_RELEASE:
              Msg.index = ADC_CHANNEL_EMPTY;
	          Msg.type = ADC_KEY_UNKOWN_TYPE;
			  if(KeyIndex == ADC_CHANNEL_EMPTY)//抬起后再设置IDLE
			  {
                  PreKeyIndex = ADC_CHANNEL_EMPTY;
	              AdcKeyState = ADC_KEY_STATE_IDLE;
			  }
			break;
			
		default:
			
			break;
	}

	return Msg;
}
#endif
#endif //CFG_RES_ADC_KEY_USE


