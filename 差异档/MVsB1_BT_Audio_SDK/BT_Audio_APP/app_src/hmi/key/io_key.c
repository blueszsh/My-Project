/**
 **************************************************************************************
 * @file    IO_key.c
 * @brief   
 *
 * @author  ken bu/bkd
 * @version V0.0.1
 *
 * $Created: 2018-09-03 
 *
 * @Copyright (C) 2018, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
#include "type.h"
#include "app_config.h"
//driver
//#include "adc.h"
#include "clk.h"
#include "gpio.h"
#include "timeout.h"
#include "io_key.h"
#include "debug.h"


#ifndef CFG_APP_CONFIG 
/*********** default config, determined by the hardware circuit **************/
//#define 	CFG_RES_IO_KEY_SCAN
#define		CFG_PARA_IO_KEY_COUNT				11 		//key count per IO channel 
//#define		CFG_RES_IO_KEY_PORT_CH1			IO_CHANNEL_GPIOA23
#define		CFG_RES_IO_KEY_CH1_ANA_EN			GPIO_A_ANA_EN
#define		CFG_RES_IO_KEY_CH1_ANA_MASK		GPIO_INDEX23

/****************************************************************************/
#endif //no CFG_APP_CONFIG


#define 	IO_KEY_SCAN_TIME			5
#define 	IO_KEY_JITTER_TIME			5
#define     IO_KEY_DOUBLE_TIME          400  // zsh A2

#define 	IO_KEY_CP_TIME				1000 	//for hold or long press
#define 	IO_KEY_CPH_TIME			    200		//for long long press

#define		IO_KEY_FULL_VAL			    4096		//

typedef enum _IO_KEY_STATE
{
	IO_KEY_STATE_IDLE,
	IO_KEY_STATE_JITTER,
	IO_KEY_STATE_PRESS_DOWN,
	IO_KEY_STATE_CP,
	IO_KEY_STATE_CP_DOUBLE,//  zsh  A2
    IO_KEY_STATE_NS_RELEASE,
} IO_KEY_STATE;

#ifdef CFG_RES_IO_KEY_SCAN

TIMER			IOKeyMoreTimer;

TIMER			IOKeyWaitTimer;
TIMER			IOKeyScanTimer;
IO_KEY_STATE	IOKeyState;

const uint32_t GPIOKey_Init_Tab[][5] =
{
	#ifdef CFG_SOFT_POWER_KEY_EN
	{POWER_KEY_OE,POWER_KEY_IE,POWER_KEY_PU,POWER_KEY_PD,POWER_KEY_PIN},
	#endif
	#ifdef CFG_GPIO_KEY1_EN
	{GPIO_KEY1_OE,GPIO_KEY1_IE,GPIO_KEY1_PU,GPIO_KEY1_PD,GPIO_KEY1},
	#endif
	#ifdef CFG_GPIO_KEY2_EN
	{GPIO_KEY2_OE,GPIO_KEY2_IE,GPIO_KEY2_PU,GPIO_KEY2_PD,GPIO_KEY2},
	#endif	   
};

int32_t IOKeyInit(void)
{
	volatile uint32_t i,pu,pd,ie,oe,pin;
	
	IOKeyState = IO_KEY_STATE_IDLE;

	TimeOutSet(&IOKeyScanTimer, IO_KEY_SCAN_TIME);
	
	#if defined(CFG_SOFT_POWER_KEY_EN) || defined(CFG_GPIO_KEY1_EN) || defined(CFG_GPIO_KEY2_EN)
	for(i = 0; i < sizeof(GPIOKey_Init_Tab)/sizeof(GPIOKey_Init_Tab[0]); i++)
	{
		oe    = GPIOKey_Init_Tab[i][0];
		ie    = GPIOKey_Init_Tab[i][1];
		pu    = GPIOKey_Init_Tab[i][2];
		pd    = GPIOKey_Init_Tab[i][3];
		pin   = GPIOKey_Init_Tab[i][4];

		///input en
		GPIO_RegOneBitSet(ie, pin);
		GPIO_RegOneBitClear(oe, pin);
		///pull enable
		GPIO_RegOneBitSet(pu, pin);
		GPIO_RegOneBitClear(pd, pin);  
	}
	#endif

	return 0;
}


static uint8_t IOChannelKeyGet(void)
{
	uint8_t	KeyIndex = IO_CHANNEL_EMPTY;

#ifdef CFG_SOFT_POWER_KEY_EN
	if(!GPIO_RegOneBitGet(POWER_KEY_IN,POWER_KEY_PIN))
	{
		KeyIndex = 0;
		return KeyIndex;
	}
#endif
#ifdef CFG_GPIO_KEY1_EN
	if(!GPIO_RegOneBitGet(GPIO_KEY1_IN,GPIO_KEY1))
	{
		KeyIndex = 1;
	}
#endif
#ifdef CFG_GPIO_KEY2_EN	
	else if(!GPIO_RegOneBitGet(GPIO_KEY2_IN,GPIO_KEY2))
	{
		KeyIndex = 2;
	}
#endif
	return KeyIndex;
}


static uint8_t GetIOKeyIndex(void)
{
	uint8_t KeyIndex = IO_CHANNEL_EMPTY;

	KeyIndex = IOChannelKeyGet();

	return KeyIndex;
}


/**
 * @func        IOKeyScan
 * @brief       IOKeyScan 按下有消抖，弹起没有消抖处理。
 * @param       void  
 * @Output      None
 * @return      IOKeyMsg, IO_CHANNEL_EMPTY or--KeyType & KeyIndex: 0 ~ CFG_PARA_ADC_KEY_COUNT * Channels
 * @Others      
 * Record
 * 1.Date        : 20180123
 *   Author      : pi.wang
 *   Modification: Created function
*/
#if 0////////// Z_key_double_EN 
IOKeyMsg IOKeyScan(void)
{
	static 	uint8_t 	PreKeyIndex = IO_CHANNEL_EMPTY;
	uint8_t				KeyIndex;
	IOKeyMsg Msg = {IO_CHANNEL_EMPTY, IO_KEY_UNKOWN_TYPE};

	if(!IsTimeOut(&IOKeyScanTimer))
	{
		return Msg;
	}
	TimeOutSet(&IOKeyScanTimer, IO_KEY_SCAN_TIME);

	KeyIndex = Scan_IO_key();

	switch(IOKeyState)
	{
		case IO_KEY_STATE_IDLE:
			if(KeyIndex == IO_CHANNEL_EMPTY)
			{
				return Msg;
			}
			PreKeyIndex = KeyIndex;
			TimeOutSet(&IOKeyWaitTimer, IO_KEY_JITTER_TIME);
			//APP_DBG("GOTO JITTER!\n");
			IOKeyState = IO_KEY_STATE_JITTER;

		case IO_KEY_STATE_JITTER:
			if(PreKeyIndex != KeyIndex)
			{
				//APP_DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = IO_CHANNEL_EMPTY;
				IOKeyState = IO_KEY_STATE_IDLE;
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
			{
				//APP_DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);
				IOKeyState = IO_KEY_STATE_PRESS_DOWN;
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_PRESSED;
			}
			break;

		case IO_KEY_STATE_PRESS_DOWN:
			if(PreKeyIndex != KeyIndex)
			{
				//APP_DBG("IO KEY SP%bu\n", PreKeyIndex);
				IOKeyState = IO_KEY_STATE_IDLE;
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_RELEASED;
					Power_Key_Releasd=TRUE;//  zsh  A2 
				PreKeyIndex = IO_CHANNEL_EMPTY;
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
			{
				//return key cp value
				//APP_DBG("IO KEY CPS!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);
				IOKeyState = IO_KEY_STATE_CP;
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_LONG_PRESSED;
			}
			break;

		case IO_KEY_STATE_CP:
			if(PreKeyIndex != KeyIndex)
			{
				//return key cp value
				//APP_DBG("IO KEY CPR!\n");
				IOKeyState = IO_KEY_STATE_IDLE;
				Msg.index = PreKeyIndex;
				Msg.type  = IO_KEY_LONG_RELEASED;
				PreKeyIndex = IO_CHANNEL_EMPTY;
					Power_Key_Releasd=TRUE;//  zsh  A2
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
			{
				//return key cph value
				//APP_DBG("IO KEY CPH!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);
				Msg.index = PreKeyIndex;
				Msg.type  = IO_KEY_LONG_PRESS_HOLD;
			}
			break;

		default:
			IOKeyState = IO_KEY_STATE_IDLE;
			PreKeyIndex = IO_CHANNEL_EMPTY;
			break;
	}

	return Msg;
}
#elif 0 
IOKeyMsg IOKeyScan(void)
{
	static 	uint8_t 	PreKeyIndex = IO_CHANNEL_EMPTY;
	uint8_t				KeyIndex;
	IOKeyMsg Msg = {IO_CHANNEL_EMPTY, IO_KEY_UNKOWN_TYPE};
	static uint8_t key_press_cnt=0;//  zsh  V1 
	static uint8_t original_keyIndex=IO_CHANNEL_EMPTY; //  zsh  V1 

	if(!IsTimeOut(&IOKeyScanTimer))
	{
		return Msg;
	}
	TimeOutSet(&IOKeyScanTimer, IO_KEY_SCAN_TIME);

//	KeyIndex = GetIOKeyIndex();
	KeyIndex = Scan_IO_key();

	switch(IOKeyState)
	{
		case IO_KEY_STATE_IDLE:
			if(KeyIndex == IO_CHANNEL_EMPTY)
			{
				return Msg;
			}
			PreKeyIndex = KeyIndex;
			TimeOutSet(&IOKeyWaitTimer, IO_KEY_JITTER_TIME);
			//DBG("GOTO JITTER!\n");
			IOKeyState = IO_KEY_STATE_JITTER;

		case IO_KEY_STATE_JITTER:
			if(PreKeyIndex != KeyIndex)
			{
				//DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = IO_CHANNEL_EMPTY;
				IOKeyState = IO_KEY_STATE_IDLE;
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
			{
				//DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);
				IOKeyState =IO_KEY_STATE_CP_DOUBLE;// IO_KEY_STATE_PRESS_DOWN;//  zsh  V1 
				//Msg.index = PreKeyIndex;
				//Msg.type = IO_KEY_PRESSED;
				original_keyIndex=PreKeyIndex;
			}
			break;
			
        case IO_KEY_STATE_CP_DOUBLE:
			if(PreKeyIndex != KeyIndex&&key_press_cnt<=1)
			{
			    DEBUG_ZX(DEBUG_IOKEY, "@@ key pressed timer=", key_press_cnt);
				if(key_press_cnt==0)
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_DOUBLE_TIME);
				IOKeyState = IO_KEY_STATE_CP_DOUBLE;
				PreKeyIndex=KeyIndex;
			    key_press_cnt++;
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
				{
				if(key_press_cnt==1||key_press_cnt==0)
					{
					PreKeyIndex=original_keyIndex;
					original_keyIndex=IO_CHANNEL_EMPTY;
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);
				IOKeyState = IO_KEY_STATE_PRESS_DOWN;

				}
				else if(key_press_cnt==2)
			      {
			        DEBUG_ZX(DEBUG_IOKEY, "@@ DOUBLE_RELEASED", 0);
			        TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);
					IOKeyState = IO_KEY_STATE_PRESS_DOWN;
					Msg.index = PreKeyIndex;
					Msg.type = IO_KEY_DOUBLE_RELEASED;
					original_keyIndex=IO_CHANNEL_EMPTY;
				}else if(key_press_cnt==3)
					{
					 DEBUG_ZX(DEBUG_IOKEY, "@@ key press 3 timer", 0);
					TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);
					IOKeyState = IO_KEY_STATE_PRESS_DOWN;
					original_keyIndex=IO_CHANNEL_EMPTY;
					//Msg.index = PreKeyIndex;
					//Msg.type = ADC_KEY_DOUBLE_RELEASED;
				}
				//ednif
			}
			break;

		case IO_KEY_STATE_PRESS_DOWN:
			if(PreKeyIndex != KeyIndex)
			{

			IOKeyState =  IO_KEY_STATE_IDLE;
			//  zsh  V1 	
			if(key_press_cnt>=2)
				{
				PreKeyIndex = IO_CHANNEL_EMPTY;
				key_press_cnt=0;
				Power_Key_Releasd=TRUE;
				}
			else
				{
				DEBUG_ZX(DEBUG_IOKEY, "@@ KEY_RELEASED", 0);
				//IOKeyState = IO_KEY_STATE_IDLE;
				key_press_cnt=0;
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_RELEASED;
				Power_Key_Releasd=TRUE;
				//PreKeyIndex = IO_CHANNEL_EMPTY;
				}
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
			{
				//return key cp value
				 DEBUG_ZX(DEBUG_IOKEY, "@@ KEY_LONG_PRESSED", 0);//  zsh  V1 
				 key_press_cnt=0;
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);
				IOKeyState = IO_KEY_STATE_CP;
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_LONG_PRESSED;
			}
			break;

		case IO_KEY_STATE_CP:
			if(PreKeyIndex != KeyIndex)
			{
				//return key cp value
				//DBG("IO KEY CPR!\n");
				  DEBUG_ZX(DEBUG_IOKEY, "@@ KEY_LONG_RELEASED", 0);//  zsh  V1 
				IOKeyState = IO_KEY_STATE_IDLE;
				Msg.index = PreKeyIndex;
				Msg.type  = IO_KEY_LONG_RELEASED;
				PreKeyIndex = IO_CHANNEL_EMPTY;
				Power_Key_Releasd=TRUE;//  zsh  V1 
			}
			else if(IsTimeOut(&IOKeyWaitTimer))
			{
				//return key cph value
				//DBG("IO KEY CPH!\n");
				  DEBUG_ZX(DEBUG_IOKEY, "@@ KEY_LONG_PRESS_HOLD", 0);//  zsh  V1 
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);
				Msg.index = PreKeyIndex;
				Msg.type  = IO_KEY_LONG_PRESS_HOLD;
			}
			break;

		default:
			IOKeyState = IO_KEY_STATE_IDLE;
			PreKeyIndex = IO_CHANNEL_EMPTY;
			break;
	}

	return Msg;
}
#elif 0


/*
     writer: 博益友    BOEU 
     增加多击处理
*/
IOKeyMsg IOKeyScan(void)
{
	static 	uint8_t 	PreKeyIndex = IO_CHANNEL_EMPTY;
	uint8_t				KeyIndex;
	IOKeyMsg Msg = {IO_CHANNEL_EMPTY, IO_KEY_UNKOWN_TYPE};
	
	static u8 key_click_cnt = 0;
	static u8 Save_KeyIndex = IO_CHANNEL_EMPTY;
	static u8 Save_KeyIndex2 = IO_CHANNEL_EMPTY;
    static u8 Key_More_flag = 0;
    static u8 SetKeyMoreTimerFlag = 1;
	

	if(!IsTimeOut(&IOKeyScanTimer))
	{
       return Msg;
	}
	TimeOutSet(&IOKeyScanTimer, IO_KEY_SCAN_TIME);
	
	//KeyIndex = GetIOKeyIndex();
	KeyIndex = Scan_IO_key();
	
	switch(IOKeyState)
	{
		case IO_KEY_STATE_IDLE:

           if(key_click_cnt > 0 && IsTimeOut(&IOKeyMoreTimer))//多击到时
           {
                  if(key_click_cnt==1)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = IO_KEY_RELEASED;//短按按下(消息发送)
					  PreKeyIndex = IO_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("IO 单击消息发送\n");
				  }
				  else if(key_click_cnt==2)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = IO_KEY_DOUBLE_RELEASED;//双击
					  PreKeyIndex = IO_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("IO 双击消息发送\n");
				  }
				  else if(key_click_cnt>=3)//如有更多击次,需修改此判断
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = IO_KEY_3_CLICK_RELEASED;// 3 击
					  PreKeyIndex = IO_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("IO 3击消息发送\n");
				  }
				  Save_KeyIndex = IO_CHANNEL_EMPTY;
				  Save_KeyIndex2 = IO_CHANNEL_EMPTY;
				  key_click_cnt = 0;
				  Key_More_flag = 0;
				  SetKeyMoreTimerFlag = 1;
				  return Msg;
		   }


			
			if(KeyIndex == IO_CHANNEL_EMPTY)//按键抬起
			{
				return Msg;//空消息
			}
			PreKeyIndex = KeyIndex;//按键按下
			TimeOutSet(&IOKeyWaitTimer, IO_KEY_JITTER_TIME);//设置消抖计时
			//APP_DBG("GOTO JITTER!\n");
			IOKeyState = IO_KEY_STATE_JITTER;//设置按键消抖状态

		case IO_KEY_STATE_JITTER://按键按下,消抖状态
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				APP_DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = IO_CHANNEL_EMPTY;//按键无效编号
				IOKeyState = IO_KEY_STATE_IDLE;//按键空闲状态
			}
			else if(IsTimeOut(&IOKeyWaitTimer))//消抖到时
			{
				//APP_DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);//设置长按计时

               //根据击次设置延时时长
               Key_More_flag = Inquire_More_Key_Msg(PreKeyIndex);
			  // APP_DBG("------------  Key_More_flag == %d\n",Key_More_flag);
				if(Key_More_flag == 0)//没有多击
				{
                     TimeOutSet(&IOKeyMoreTimer,1);
					  SetKeyMoreTimerFlag = 1;
				}
				else if(Key_More_flag == _Key_CLICK3_)//有三击
				{
                    if(IsTimeOut(&IOKeyMoreTimer) && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&IOKeyMoreTimer, 800);//设置三击等待计时
					}
				}
				else if(Key_More_flag == _Key_double_)//有双击
				{
                    if(IsTimeOut(&IOKeyMoreTimer)  && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&IOKeyMoreTimer, 500);//设置双击等待计时
					}
				}
				
				
				IOKeyState = IO_KEY_STATE_PRESS_DOWN;//设置按键按下状态
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_PRESSED;//按键按下
				Save_KeyIndex = PreKeyIndex;
				key_click_cnt++;

				if(key_click_cnt == 1)
				{
                   Save_KeyIndex2 = Save_KeyIndex;
				}
				else if(key_click_cnt >= 2)
				{
                    if(Save_KeyIndex2 != Save_KeyIndex)//多击等待时中又按下另外的按键
                    {
                        key_click_cnt =  1;//设为单击,最后松开的按键
                       // APP_DBG("按下另外的按键  设为单击\n");
					}
				}

				
			}
			break;

		case IO_KEY_STATE_PRESS_DOWN://按键按下
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//APP_DBG("key release %d\n", PreKeyIndex);
                APP_DBG("DC_KEY_STATE_PRESS_DOWN key_click_cnt == %d\n", key_click_cnt);
                if(key_click_cnt > 0)
                {
                  // APP_DBG("DC_KEY_STATE_PRESS_DOWN   goto IDLE\n");
                     //PreKeyIndex = IO_CHANNEL_EMPTY;//按键无效编号
				     IOKeyState = IO_KEY_STATE_IDLE;//按键空闲状态
				}
		
			}
			else if(IsTimeOut(&IOKeyWaitTimer))//长按到时
			{
				//return key cp value
				//APP_DBG("IO KEY CPS!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);//设置持续按下时间
				IOKeyState = IO_KEY_STATE_CP;//设置按键持续按压状态
				Msg.index = PreKeyIndex;//按下编号
				Msg.type = IO_KEY_LONG_PRESSED;//长按按下(消息发送)
				key_click_cnt = 0;
				Save_KeyIndex = IO_CHANNEL_EMPTY;
			}
			break;

		case IO_KEY_STATE_CP://按下持续
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//return key cp value
				//APP_DBG("IO KEY CPR!\n");
				IOKeyState = IO_KEY_STATE_IDLE;//设置按键空闲状态
				Msg.index = PreKeyIndex;//按键编号
				Msg.type  = IO_KEY_LONG_RELEASED;//长按释放
				PreKeyIndex = IO_CHANNEL_EMPTY;//清除编号
				Power_Key_Releasd=TRUE;//  zsh  A2
				key_click_cnt = 0;
				Key_More_flag = 0;
			}
			else if(IsTimeOut(&IOKeyWaitTimer))//(长按1S到时后)按键持续按下 
			{
				//return key cph value
				//APP_DBG("IO KEY CPH!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);//设置长按持续消息时间
				Msg.index = PreKeyIndex;
				Msg.type  = IO_KEY_LONG_PRESS_HOLD;//长按保持(消息发送)
				key_click_cnt = 0;
				Key_More_flag = 0;
			}
			break;

		default:
			IOKeyState = IO_KEY_STATE_IDLE;
			PreKeyIndex = IO_CHANNEL_EMPTY;
			key_click_cnt = 0;
			Key_More_flag = 0;
			Save_KeyIndex = IO_CHANNEL_EMPTY;
			Save_KeyIndex2 = IO_CHANNEL_EMPTY;
			break;
	}

	return Msg;
}

#else

/*
     writer: 博益友    BOEU 
     增加多击处理
*/
IOKeyMsg IOKeyScan(void)
{
		static 	uint8_t 	PreKeyIndex = IO_CHANNEL_EMPTY;
	uint8_t				KeyIndex;
	IOKeyMsg Msg = {IO_CHANNEL_EMPTY, IO_KEY_UNKOWN_TYPE};
	
	static u8 key_click_cnt = 0;
	static u8 Save_KeyIndex = IO_CHANNEL_EMPTY;
	static u8 Save_KeyIndex2 = IO_CHANNEL_EMPTY;
    static u8 Key_More_flag = 0;
    static u8 SetKeyMoreTimerFlag = 1;
	
	 static uint8_t key_ns_flag=0;
	static uint16_t IO_KEY_Ns_TIME = 0;

//	if(!IsTimeOut(&IOKeyScanTimer))
//	{
//		return Msg;
//	}
//	TimeOutSet(&IOKeyScanTimer, IO_KEY_SCAN_TIME);
	
	//	KeyIndex = GetIOKeyIndex();
	KeyIndex = Scan_IO_key();
	
	switch(IOKeyState)
	{
		case IO_KEY_STATE_IDLE:

           if(key_click_cnt > 0 && IsTimeOut(&IOKeyMoreTimer))//多击等待到时
           {
      
				
                //根据短按击次判断单击,双击,三击
                  if(key_click_cnt==1)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = IO_KEY_RELEASED;//短按按下(消息发送)
					  PreKeyIndex = IO_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("单击消息发送\n");
				  }
				  else if(key_click_cnt==2)
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = IO_KEY_DOUBLE_RELEASED;//双击
					  PreKeyIndex = IO_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("双击消息发送\n");
				  }
				  else if(key_click_cnt>=3)//如有更多击次,需修改此判断
                  {
                      Msg.index = Save_KeyIndex; //按下编号
					  Msg.type = IO_KEY_3_CLICK_RELEASED;// 3 击
					  PreKeyIndex = IO_CHANNEL_EMPTY;//清除按键编号 
				       APP_DBG("3击消息发送\n");
				  }
				  Save_KeyIndex = IO_CHANNEL_EMPTY;
				  Save_KeyIndex2 = IO_CHANNEL_EMPTY;
				  key_click_cnt = 0;
				  Key_More_flag = 0;
				  SetKeyMoreTimerFlag = 1;
				  return Msg;
		   }


			
			if(KeyIndex == IO_CHANNEL_EMPTY)//按键抬起
			{
				return Msg;//空消息
			}
			PreKeyIndex = KeyIndex;//按键按下
			TimeOutSet(&IOKeyWaitTimer, IO_KEY_JITTER_TIME);//设置消抖计时
			//APP_DBG("GOTO JITTER!\n");
			IOKeyState = IO_KEY_STATE_JITTER;//设置按键消抖状态

		case IO_KEY_STATE_JITTER://按键按下,消抖状态
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//APP_DBG("GOTO IDLE Because jitter!\n");
				PreKeyIndex = IO_CHANNEL_EMPTY;//按键无效编号
				IOKeyState = IO_KEY_STATE_IDLE;//按键空闲状态

				
				
			}
			else if(IsTimeOut(&IOKeyWaitTimer))//消抖到时
			{
				//APP_DBG("GOTO PRESS_DOWN!\n");
				TimeOutSet(&IOKeyWaitTimer, IO_KEY_CP_TIME);//设置长按计时

              //根据按下的按键所定义的击次设置延时时长
               Key_More_flag = Inquire_More_Key_Msg(PreKeyIndex);
			  // APP_DBG("------------  Key_More_flag == %d\n",Key_More_flag);
				if(Key_More_flag == 0)//没有多击
				{
                     TimeOutSet(&IOKeyMoreTimer,0);
					 SetKeyMoreTimerFlag = 1;
				}
				else if(Key_More_flag == _Key_CLICK3_)//有三击
				{
                    if(IsTimeOut(&IOKeyMoreTimer) && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&IOKeyMoreTimer, 800);//设置三击等待计时
					}
				}
				else if(Key_More_flag == _Key_double_)//有双击
				{
                    if(IsTimeOut(&IOKeyMoreTimer)  && SetKeyMoreTimerFlag)
					{
					   SetKeyMoreTimerFlag = 0;
					   TimeOutSet(&IOKeyMoreTimer, 500);//设置双击等待计时
					}
				}
				
				
				IOKeyState = IO_KEY_STATE_PRESS_DOWN;//设置按键按下状态
				Msg.index = PreKeyIndex;
				Msg.type = IO_KEY_PRESSED;//按键按下
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
					//APP_DBG("IOKey 111111111111   key_ns_flag == %d\n",key_ns_flag);
				 }
				 else
				 {
                    key_ns_flag = 0;
					//APP_DBG("IOKey 0000000000000  key_ns_flag == %d\n",key_ns_flag);
				 }

				 
				
			}
			break;

		case IO_KEY_STATE_PRESS_DOWN://按键按下
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				APP_DBG("key release %d\n", PreKeyIndex);
                if(key_click_cnt > 0)
                {
                  // APP_DBG("DC_KEY_STATE_PRESS_DOWN   goto IDLE\n");
                    
				     IOKeyState = IO_KEY_STATE_IDLE;//按键空闲状态
				}
		
			}
			else if(IsTimeOut(&IOKeyWaitTimer))//长按到时
			{
				if(key_ns_flag == 0)
				{
					TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);
					IOKeyState = IO_KEY_STATE_CP;
					Msg.index = PreKeyIndex;
					Msg.type = IO_KEY_LONG_PRESSED;
				}
				else
				{  
				    //按下的按键有做N秒消息,先不发1秒消息
				    switch(key_ns_flag)// 在已经长按1s时间上计时
				    {
						case 2: IO_KEY_Ns_TIME = 1000; break;
						case 3: IO_KEY_Ns_TIME = 2000; break;
						case 4: IO_KEY_Ns_TIME = 3000; break;
						case 5: IO_KEY_Ns_TIME = 4000; break;
						case 6: IO_KEY_Ns_TIME = 5000; break;
					} 
				    TimeOutSet(&IOKeyWaitTimer, IO_KEY_Ns_TIME);
				    IOKeyState = IO_KEY_STATE_CP;
                   			
				}
			}
			break;

		case IO_KEY_STATE_CP://按下持续
			if(PreKeyIndex != KeyIndex)//按键抬起
			{
				//return key cp value
				APP_DBG("IO KEY CPR!\n");

			    if(key_ns_flag == 0)
				{
					IOKeyState = IO_KEY_STATE_IDLE;
					Msg.index = PreKeyIndex;
					Msg.type  = IO_KEY_LONG_RELEASED;
					PreKeyIndex = IO_CHANNEL_EMPTY;
					Power_Key_Releasd=TRUE;//  zsh  A2
				}
				else
				{
				   //有定义N秒的按键,未按够N秒抬起就发1秒消息
				    IOKeyState = IO_KEY_STATE_IDLE;
                    Msg.index = PreKeyIndex;
					Msg.type = IO_KEY_LONG_PRESSED;
					PreKeyIndex = IO_CHANNEL_EMPTY;
					Power_Key_Releasd=TRUE;//  zsh  A2
					key_ns_flag = 0;
				}
			}
			else///// if(IsTimeOut(&IOKeyWaitTimer))//(长按1S到时后)按键持续按下 
			{
				//return key cph value
				//APP_DBG("IO KEY CPH!\n");
				if(key_ns_flag == 0)
				{
				   if(IsTimeOut(&IOKeyWaitTimer))
				   {
						TimeOutSet(&IOKeyWaitTimer, IO_KEY_CPH_TIME);
						Msg.index = PreKeyIndex;
						Msg.type  = IO_KEY_LONG_PRESS_HOLD;
						Save_KeyIndex = IO_CHANNEL_EMPTY;
						Save_KeyIndex2 = IO_CHANNEL_EMPTY;
						key_click_cnt = 0;
						Key_More_flag = 0;
						SetKeyMoreTimerFlag = 1;
				   }
				}
				else
				{
				    //临时代码,做N秒消息的按键,暂不支持长按保持
				   if(IsTimeOut(&IOKeyWaitTimer))
				   {
				     
                        Msg.index = PreKeyIndex;
						switch(key_ns_flag)// 
					    {
							case 2: Msg.type  = IO_KEY_LONG_2S_RELEASED; break;
							case 3: Msg.type  = IO_KEY_LONG_3S_RELEASED; break;
							case 4: Msg.type  = IO_KEY_LONG_4S_RELEASED; break;
							case 5: Msg.type  = IO_KEY_LONG_5S_RELEASED; break;
							case 6: Msg.type  = IO_KEY_LONG_6S_RELEASED; break;
						} 
						key_ns_flag = 0;
						//PreKeyIndex = IO_CHANNEL_EMPTY;
				     	IOKeyState = IO_KEY_STATE_NS_RELEASE;

						 Save_KeyIndex = IO_CHANNEL_EMPTY;
						 Save_KeyIndex2 = IO_CHANNEL_EMPTY;
						 key_click_cnt = 0;
						 Key_More_flag = 0;
						 SetKeyMoreTimerFlag = 1;
						  
						//APP_DBG(">>>>>>>>>>>>> key Msg.type %d\n", Msg.type);
				   }
				}
			}
			break;

          case IO_KEY_STATE_NS_RELEASE:
              Msg.index = IO_CHANNEL_EMPTY;
	          Msg.type = IO_KEY_UNKOWN_TYPE;
			  if(KeyIndex == IO_CHANNEL_EMPTY)//抬起后再设置IDLE
			  {
                  PreKeyIndex = IO_CHANNEL_EMPTY;
	              IOKeyState = IO_KEY_STATE_IDLE;
			  }
			break;
			
		default:
			
			break;
	}

	return Msg;
}


#endif
#endif //CFG_RES_IO_KEY_SCAN


