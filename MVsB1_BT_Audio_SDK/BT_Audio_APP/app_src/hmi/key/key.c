/**
 **************************************************************************************
 * @file    Key.c
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

#include "adc_levels.h"
#include "app_config.h"
#include "app_message.h"
#include "debug.h"
#include "key.h"
#include "ctrlvars.h"
#include "timeout.h"

#ifdef CFG_RES_ADC_KEY_SCAN
#include "adc_key.h"
#endif
#ifdef CFG_RES_IR_KEY_SCAN
#include "ir_key.h"
#endif
#ifdef CFG_RES_CODE_KEY_USE
#include "code_key.h"
#endif
#ifdef CFG_RES_IO_KEY_SCAN
#include "io_key.h"
#endif
#if Remote_433_EN
#include "Remote_433_key.h"
#endif

#if (defined(CFG_FUNC_BACKUP_EN) && defined(USE_POWERKEY_SOFT_PUSH_BUTTON))
#include "power_key.h"
#endif

extern void GIE_DISABLE(void); //defined@interrupt.c
extern void GIE_ENABLE(void);

uint16_t gFuncID = 0;
uint16_t gKeyValue;
#ifdef CFG_FUNC_DBCLICK_MSG_EN
KEYBOARD_MSG dbclick_msg;
TIMER	DBclicTimer;
#endif
/*************************************************
* ADC KEY按键属性对应消息列表
*
*  注：用户需要根据功能定义重新修改此表！！！
***************************************************/
static const uint16_t ADKEY_TAB[][KEY_MSG_DEFAULT_NUM] =
{
	//KEY_PRESS       SHORT_RELEASE                 LONG_PRESS                  KEY_HOLD                    LONG_PRESS_RELEASE

	//power adc key
	{MSG_NONE,        MSG_PITCH_UP,                 MSG_POWERDOWN,  			MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_MEDIA_PLAY_BROWER_UP,     MSG_MIC_BASS_DW,    		MSG_MIC_BASS_DW,   			MSG_NONE},
	{MSG_NONE,        MSG_MEDIA_PLAY_BROWER_DN,	    MSG_MIC_BASS_UP,    		MSG_MIC_BASS_UP,   			MSG_NONE},
	{MSG_NONE,        MSG_MEDIA_PLAY_BROWER_ENTER,	MSG_MIC_TREB_DW,    		MSG_MIC_TREB_DW,   			MSG_NONE},
	{MSG_NONE,        MSG_MEDIA_PLAY_BROWER_RETURN,	MSG_MIC_TREB_UP,    		MSG_MIC_TREB_UP,   			MSG_NONE},
	{MSG_NONE,        MSG_MIC_EFFECT_UP,	        MSG_MIC_EFFECT_UP,          MSG_MIC_EFFECT_UP,          MSG_NONE},
	{MSG_NONE,        MSG_MIC_EFFECT_DW,            MSG_MIC_EFFECT_DW,          MSG_MIC_EFFECT_DW,          MSG_NONE},
	{MSG_NONE,        MSG_NONE,   					MSG_NONE,  					MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NONE,  					MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NONE,     				MSG_NONE,   				MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NONE,  					MSG_NONE,  					MSG_NONE,                   MSG_NONE},

	//adc1 key
#ifdef CFG_APP_REST_MODE_EN
	{MSG_NONE,		  MSG_PLAY_PAUSE,				MSG_POWER,					MSG_NONE,					MSG_NONE},
#elif defined (BT_SNIFF_ENABLE)
	{MSG_NONE,        MSG_PLAY_PAUSE,   			MSG_BT_SNIFF,      			MSG_NONE,                   MSG_NONE},
#else
	{MSG_NONE,        MSG_PLAY_PAUSE,   			MSG_DEEPSLEEP,      			MSG_NONE,                   MSG_NONE},
#endif
	{MSG_NONE,        MSG_PRE,          			MSG_FB_START,               MSG_FB_START,               MSG_FF_FB_END},
	{MSG_NONE,        MSG_NEXT,	        			MSG_FF_START,               MSG_FF_START,               MSG_FF_FB_END},
	{MSG_NONE,        MSG_MUSIC_VOLDOWN,	    	MSG_MUSIC_VOLDOWN,   		MSG_MUSIC_VOLDOWN,			MSG_NONE},
	{MSG_NONE,        MSG_MUSIC_VOLUP,	    		MSG_MUSIC_VOLUP,     		MSG_MUSIC_VOLUP,     		MSG_NONE},
	{MSG_NONE,        MSG_EQ,	        			MSG_3D,        				MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_MUTE,         			MSG_VB,        				MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_EFFECTMODE,   			MSG_VOCAL_CUT, 				MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_REPEAT,  					MSG_REPEAT_AB,              MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_REC,     					MSG_REC_PLAYBACK,           MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_MODE,  					MSG_BT_TWS_PAIRING, 		MSG_NONE,                   MSG_NONE},

	//adc2 key
	{MSG_NONE,        MSG_BROWSE,   				MSG_REC_FILE_DEL,     		MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_FOLDER_PRE,    			MSG_BT_HF_CALL_REJECT,      MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_FOLDER_NEXT,	 			MSG_BT_HF_REDAIL_LAST_NUM,  MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_RTC_SET_TIME,	     		MSG_RTC_DISP_TIME,          MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_RTC_SET_ALARM,	     	MSG_MIC_FIRST,              MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_RTC_UP,	     		    MSG_BT_HF_VOICE_RECOGNITION,MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_RTC_DOWN,       		    MSG_HDMI_AUDIO_ARC_ONOFF,   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_BT_CONNECT_CTRL, 			MSG_BT_ENTER_DUT_MODE,      MSG_NONE,                   MSG_NONE},
#ifdef	CFG_XIAOAI_AI_EN
	{MSG_NONE,        MSG_BT_XM_AI_START,           MSG_UPDATE,                 MSG_NONE,                   MSG_BT_XM_AI_STOP},
#else
	#ifdef	CFG_FUNC_AI_EN
	{MSG_NONE,        MSG_BT_AI,           			MSG_UPDATE,                 MSG_NONE,                   MSG_BT_XM_AI_STOP},
	#else
	{MSG_NONE,        MSG_BT_TWS_OUT_MODE,          MSG_UPDATE,                 MSG_NONE,                   MSG_BT_XM_AI_STOP},
	#endif
#endif
	{MSG_NONE,        MSG_MIC_VOLDOWN,   			MSG_MIC_VOLDOWN, 			MSG_MIC_VOLDOWN,  			MSG_NONE},
	{MSG_NONE,        MSG_MIC_VOLUP,     			MSG_MIC_VOLUP,   			MSG_MIC_VOLUP,    			MSG_NONE},

};

/*************************************************
* GPIO按键属性对应消息列表
*
*  注：用户需要根据功能定义重新修改此表！！！
***************************************************/
static const uint16_t IOKEY_TAB[][KEY_MSG_DEFAULT_NUM] =
{
    //KEY_PRESS       SHORT_RELEASE                 LONG_PRESS                  KEY_HOLD                    LONG_PRESS_RELEASE

	{MSG_NONE,        MSG_EFFECTMODE,               MSG_SOFT_POWER,  			MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NONE,      				MSG_NONE,    				MSG_NONE,   				MSG_NONE},
	{MSG_NONE,        MSG_NONE,	    				MSG_NONE,    				MSG_NONE,   				MSG_NONE},
};

/*************************************************
* code按键属性对应消息列表
*
*  注：用户需要根据功能定义重新修改此表！！！
***************************************************/
static const uint16_t CODEKEY_TAB[][KEY_MSG_DEFAULT_NUM] =
{
	//KEY_PRESS       SHORT_RELEASE                 LONG_PRESS                  KEY_HOLD                    LONG_PRESS_RELEASE

	{MSG_NONE,        MSG_MUSIC_VOLDOWN,        	MSG_MUSIC_VOLDOWN,   		MSG_MUSIC_VOLDOWN,  		MSG_NONE},
	{MSG_NONE,        MSG_MUSIC_VOLUP,          	MSG_MUSIC_VOLUP,     		MSG_MUSIC_VOLUP,    		MSG_NONE},
};

/*************************************************
* 遥控按键属性对应消息列表
*
*  注：用户需要根据功能定义重新修改此表！！！
***************************************************/
static const uint16_t IRKEY_TAB[][KEY_MSG_DEFAULT_NUM] =
{
	//KEY_PRESS       SHORT_RELEASE                 LONG_PRESS                  KEY_HOLD                    LONG_PRESS_RELEASE

#ifdef CFG_APP_REST_MODE_EN
	{MSG_NONE,		  MSG_POWER,					MSG_POWER,					MSG_NONE,					MSG_NONE},
#elif defined (BT_SNIFF_ENABLE)
	{MSG_NONE,        MSG_BT_SNIFF,    				MSG_BT_SNIFF,  				MSG_NONE,                   MSG_NONE},
#else
	{MSG_NONE,        MSG_DEEPSLEEP,    			MSG_DEEPSLEEP,  			MSG_NONE,                   MSG_NONE},
#endif
	{MSG_NONE,        MSG_MODE,  					MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_MUTE,	        			MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_PLAY_PAUSE,   		    MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_PRE,          			MSG_FB_START,         		MSG_FB_START,               MSG_FF_FB_END},
	{MSG_NONE,        MSG_NEXT,	        			MSG_FF_START,         		MSG_FF_START,               MSG_FF_FB_END},
	{MSG_NONE,        MSG_EQ,           			MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_MUSIC_VOLUP,	    		MSG_MUSIC_VOLUP,      		MSG_MUSIC_VOLUP,     		MSG_NONE},
	{MSG_NONE,        MSG_MUSIC_VOLDOWN,	    	MSG_MUSIC_VOLDOWN,    		MSG_MUSIC_VOLDOWN,   		MSG_NONE},
	{MSG_NONE,        MSG_REPEAT,                   MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_RADIO_PLAY_SCAN,          MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_1,            		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_2,            		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_3,	        		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_4,	        		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_5,	        		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_6,	        		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_7,            		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_8,            		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_9,            		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
	{MSG_NONE,        MSG_NUM_0,            		MSG_NONE,                   MSG_NONE,                   MSG_NONE},
};

static const uint16_t PWRKey_TAB = MSG_PLAY_PAUSE;

uint32_t GetGlobalKeyValue(void)
{
    return gFuncID;
}

void ClrGlobalKeyValue(void)
{
    gFuncID = 0;
}


#if (defined(CFG_RES_ADC_KEY_SCAN) ||defined(CFG_RES_IO_KEY_SCAN)|| defined(CFG_RES_IR_KEY_SCAN) || defined(CFG_RES_CODE_KEY_USE) ||  defined(CFG_ADC_LEVEL_KEY_EN))

/**
 * @func        SetGlobalKeyValue
 * @brief       SetGlobalKeyValue,本函数主要目的是多种按键类的消息归一化映射
 * @param       uint8_t KeyType:注意 按键事件类型带转译
                uint16_t KeyValue，Uart提供单独一列映射，键值和旋钮一列，按键事件类型一列，如有必要可拆开单列。
 * @Output      gFuncID:
 				gKeyValue
 * @return      void
 * @Others      
 * Record
 * 1.Date        : 20180119
 *   Author      : pi.wang
 *   Modification: Created function
*/
void SetGlobalKeyValue(uint8_t KeyType, uint16_t KeyValue)
{
}

void SetIrKeyValue(uint8_t KeyType, uint16_t KeyValue)
{
	gFuncID = IRKEY_TAB[KeyValue][KeyType - 1];
}

void SetAdcKeyValue(uint8_t KeyType, uint16_t KeyValue)
{
	gFuncID = ADKEY_TAB[KeyValue][KeyType - 1];
}

uint16_t GetIrKeyValue()
{
	return gFuncID;
}

void KeyInit(void)
{
#ifdef CFG_RES_ADC_KEY_SCAN
	AdcKeyInit();
#endif

#ifdef CFG_RES_IO_KEY_SCAN
	IOKeyInit();
#endif
#if Remote_433_EN
    Remote_433_KeyInit();
#endif
#ifdef CFG_RES_IR_KEY_SCAN
	IRKeyInit();
#endif

#ifdef CFG_RES_CODE_KEY_USE
	CodeKeyInit();
#endif

#ifdef CFG_ADC_LEVEL_KEY_EN
	ADCLevelsKeyInit();
#endif

#if (defined(CFG_FUNC_BACKUP_EN) && defined(USE_POWERKEY_SOFT_PUSH_BUTTON))
	PowerKeyScanInit();
#endif
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	DbclickInit();
#endif
}

inline bool GIE_STATE_GET(void);

/**
 * @func        KeyScan
 * @brief       KeyScan,根据键值和事件类型查表，输出消息值
 * @param       None  
 * @Output      None
 * @return      MessageId
 * @Others      
 * Record
 * 1.Date        : 20180123
 *   Author      : pi.wang
 *   Modification: Created function
*/
MessageId KeyScan(void)
{
	MessageId KeyMsg = MSG_NONE;
#ifdef CFG_RES_IO_KEY_SCAN//bkd add
	IOKeyMsg ioKeyMsg;
#endif
	
#ifdef CFG_RES_ADC_KEY_SCAN//bkd add
	AdcKeyMsg AdcKeyMsg;
#endif

#if Remote_433_EN
    Remote_433KeyMsg  Remote433KeyMsg;
#endif

#ifdef CFG_RES_IR_KEY_SCAN
	IRKeyMsg IRKeyMsg;
#endif
#ifdef CFG_RES_CODE_KEY_USE
	CodeKeyType CodeKey = CODE_KEY_NONE;
#endif

#if (defined(CFG_FUNC_BACKUP_EN) && defined(USE_POWERKEY_SOFT_PUSH_BUTTON))
	PWRKeyMsg PWRKeyMsg;
#endif

#ifdef CFG_RES_ADC_KEY_SCAN
	AdcKeyMsg = AdcKeyScan();
	if(AdcKeyMsg.index != ADC_CHANNEL_EMPTY && AdcKeyMsg.type != ADC_KEY_UNKOWN_TYPE)
	{
		//BeepEnable();
		//gFuncID = ADKEY_TAB[AdcKeyMsg.index][AdcKeyMsg.type - 1];
		gFuncID = key_operation_to_fun(AdcKeyMsg.index,AdcKeyMsg.type);  // zsh A2
		ADkey_press_num=gFuncID;
		#ifdef CFG_FUNC_DBCLICK_MSG_EN
		Save_key_number = AdcKeyMsg.index;
		Save_KeyMsg_ype = AdcKeyMsg.type;
		#endif
		APP_DBG("AdcKeyMsg = %d, %d\n", AdcKeyMsg.index, AdcKeyMsg.type);
	}
#endif

#ifdef CFG_RES_IO_KEY_SCAN
	ioKeyMsg = IOKeyScan();
	if(ioKeyMsg.index != IO_CHANNEL_EMPTY && ioKeyMsg.type != IO_KEY_UNKOWN_TYPE)
	{
	//	gFuncID = IOKEY_TAB[ioKeyMsg.index][ioKeyMsg.type - 1];
		gFuncID = key_operation_to_fun(ioKeyMsg.index,ioKeyMsg.type); // zsh A2
		IOkey_press_num=gFuncID;
		#ifdef CFG_FUNC_DBCLICK_MSG_EN
		Save_key_number = ioKeyMsg.index;
		Save_KeyMsg_ype = ioKeyMsg.type;
		#endif
		APP_DBG("IOKeyindex = %d, %d\n", ioKeyMsg.index, ioKeyMsg.type);

		#if Z_IO_ADC_key_Det_EN
            if(AdcKeyMsg.index != ADC_CHANNEL_EMPTY && AdcKeyMsg.type != ADC_KEY_UNKOWN_TYPE)
            {
                AdcKeyMsg.index = ADC_CHANNEL_EMPTY;
			    AdcKeyMsg.type = ADC_KEY_UNKOWN_TYPE;
				APP_DBG("ADKey   ADC_KEY_UNKOWN_TYPE !!\n");
			}
		#endif
	}

#endif
#if Remote_433_EN

    Remote433KeyMsg = Remote_433_KeyScan();
	if(Remote433KeyMsg.index != Remote_433_KEY_NONE && Remote433KeyMsg.type != Remote_433_KEY_UNKOWN_TYPE)
	{
		gFuncID = key_operation_to_fun(Remote433KeyMsg.index+Remote_00,Remote433KeyMsg.type); // zsh A2
		IOkey_press_num=gFuncID;
		#ifdef CFG_FUNC_DBCLICK_MSG_EN
		Save_key_number = Remote433KeyMsg.index;
		Save_KeyMsg_ype = Remote433KeyMsg.type;
		#endif
		APP_DBG("Remote_433   Keyindex = %d, %d\n", Remote433KeyMsg.index+Remote_00, Remote433KeyMsg.type);
	}

#endif

#ifdef CFG_RES_IR_KEY_SCAN
	IRKeyMsg = IRKeyScan();
	if(IRKeyMsg.index != IR_KEY_NONE && IRKeyMsg.type != IR_KEY_UNKOWN_TYPE)
	{
		#ifdef BT_TWS_SUPPORT
		if(GetSystemMode() == ModeTwsSlavePlay)
		{
			gFuncID = MSG_NONE;
		}
		else
		#endif
		{
		//gFuncID = IRKEY_TAB[IRKeyMsg.index][IRKeyMsg.type - 1];
		gFuncID = key_operation_to_fun(IRKeyMsg.index+1000,IRKeyMsg.type); // zsh A2
		APP_DBG("IRKeyindex = %d, %d\n", IRKeyMsg.index, IRKeyMsg.type);
		}
	}
#endif

#ifdef CFG_RES_CODE_KEY_USE
	CodeKey = CodeKeyScan();
	if(CodeKey != CODE_KEY_NONE)
	{
		gFuncID = CODEKEY_TAB[CodeKey-1][1];
	}
#endif

#ifdef CFG_ADC_LEVEL_KEY_EN
	KeyMsg = AdcLevelKeyProcess();

	if(KeyMsg != MSG_NONE)
	{
		gFuncID = KeyMsg;
	}			
#endif

#if (defined(CFG_FUNC_BACKUP_EN) && defined(USE_POWERKEY_SOFT_PUSH_BUTTON))
	PWRKeyMsg = PowerKeyScan();
	if(PWRKeyMsg.type == PWR_KEY_SP)
	{
		gFuncID = PWRKey_TAB;
	}
#endif

	{
#ifdef CFG_FUNC_DBCLICK_MSG_EN
		DbclickGetMsg(gFuncID);
#endif

#ifdef CFG_FUNC_DBCLICK_MSG_EN
		DbclickProcess();
#endif

		KeyMsg = GetGlobalKeyValue();
		if(KeyMsg==0) //zsh A2
		KeyMsg = Special_msg_deal();

		ClrGlobalKeyValue();
	}


	return KeyMsg;
}

#endif
/*************************************************
* 按键双击初始化处理函数，只支持单击释放的双击
* dbclick_msg 变量如果定义成数组，则可实现多个双击功能
*
***************************************************/
void DbclickInit(void)
{
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	dbclick_msg.dbclick_en            = 1;
	dbclick_msg.dbclick_counter       = 0;
	dbclick_msg.dbclick_timeout       = 0;

	dbclick_msg.KeyMsg                = CFG_PARA_CLICK_MSG;////Single click msg
	dbclick_msg.dbclick_msg           = CFG_PARA_DBCLICK_MSG;//double  click msg
	TimeOutSet(&DBclicTimer, 0);
#endif

}

/*************************************************
* 按键双击或多击处理函数
*
*
***************************************************/
void DbclickProcess(void)
{
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	if(!IsTimeOut(&DBclicTimer))
	{
		return;
	}
	TimeOutSet(&DBclicTimer, 4);

	if(dbclick_msg.dbclick_en)
	{
		if(dbclick_msg.dbclick_timeout)
		{
			dbclick_msg.dbclick_timeout--;
			if(dbclick_msg.dbclick_timeout == 0)
			{
				//gFuncID = dbclick_msg.KeyMsg;//Single click msg
			//发单击消息 
			gFuncID = Key_msg_ulteriorly_dispose(0,ZX_key_info[inquire_key_i].Key_function[0][1]);
				
				dbclick_msg.dbclick_counter = 0;
				dbclick_msg.dbclick_timeout = 0;
				APP_DBG("shot click_msg \n");
			}
		}
	}
#endif ///#ifdef CFG_FUNC_DBCLICK_MSG_EN
}
/*************************************************
* 判断按键双击键值处理函数
*
*
***************************************************/
uint8_t DbclickGetMsg(uint16_t Msg)
{
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	if(dbclick_msg.dbclick_en == 0)   return 0;
	/////此处可增加工作模式判断，不是所有模式需要双击功能////////
	//if( WORK_MODE != BT_MODE) return 0

	// printf("------------- Msg == %d\n",Msg);//未按下 为 0
	
   if(Msg==0||Save_KeyMsg_ype==_Key_long_1s||Save_KeyMsg_ype==_Key_hold___) 
   {
     	return 0;
   }
   // printf("----------------------- Msg == %d\n",Msg);

    //非按键消息不做双击判断
#ifdef CFG_RES_CODE_KEY_USE
   if(Msg==CODEKEY_TAB[0][1]||Msg==CODEKEY_TAB[1][1])
   {
        // 编码器的消息不用判断双击
        return 0;
   }
#endif
#ifdef CFG_ADC_LEVEL_KEY_EN
      if((Msg > MSG_ADC_LEVEL_MSG_START)&&(Msg < MSG_ADC_LEVEL_MSG_END))
      {
           // 电位器消息不做判断双击
           return 0;
	  }
#endif
   
	//if((dbclick_msg.KeyMsg == Msg))
	if(Inquire_Specific_Key_Msg())
	{
    	gFuncID = 0;

		if(dbclick_msg.dbclick_timeout == 0)
		{
		   Save_key_number2 = Save_key_number;
    	   dbclick_msg.dbclick_timeout =  CFG_PARA_DBCLICK_DLY_TIME;///4ms*20=80ms
			dbclick_msg.dbclick_counter += 1;
    	   APP_DBG("start double click_msg \n");
		}
		else
		{
			if(Save_key_number != Save_key_number2)
			{
			   DBG("两次按键不一样,Clear message.       gFuncID = 0\n");
               gFuncID = 0;//在双击延时内,按下另外一个按键,消息清除
			}
			else
			{
			  //发双击消息  
			 gFuncID = Key_msg_ulteriorly_dispose(0,ZX_key_info[inquire_key_i].Key_function[1][1]);
			}
			dbclick_msg.dbclick_counter = 0;
			dbclick_msg.dbclick_timeout = 0;
			APP_DBG("double click_msg \n");
		}
		return 1;
	}
	else
	{
		if(Msg != MSG_NONE)
		{
			dbclick_msg.dbclick_timeout = 0;
			dbclick_msg.dbclick_counter = 0;
		}
		return 0;
	}
#else
	return 0;
#endif
}




/*
*   BOEU add
*  查询触发的按键消息在 ZX_key_info[] 的位置
*  return  TRUE  按下的按键有做双击消息,需要延时判定单\双击
*  return  FALSE 按下的按键没做双击消息,不做延时 
*/
bool Inquire_Specific_Key_Msg(void)
{
    u8 i=0,j=0;

	for(i = 0;i < ZX_key_info_length;i++)
	{
       if(ZX_key_info[i].Key_name==Save_key_number)
       {
                     
             inquire_key_i = i;
		     inquire_key_j = j;
			// DBG("double msg == %d\n",ZX_key_info[inquire_key_i].Key_function[1][1]);
              //if((ZX_key_info[inquire_key_i].Key_function[1][1])==_Key_NULL_MSG)
			  if((ZX_key_info[inquire_key_i].Key_function[1][1])==0)
		      {
		          //触发的按键双击无消息,不做双击延时
                    return FALSE;
			  }
			  else
			  {
                  return TRUE;
			  }
	   }
	}

     return FALSE;
}








/*
*   BOEU add
*  查询触发的按键是否有双击\三击消息
* return  _Key_double_ 有双击消息
  return  _Key_CLICK3_ 有三击消息
  return  0   没有多击
*/
u8 Inquire_More_Key_Msg(u16 key_number)
{
    u8 i=0,j=0;
	u8 key_more = 0;

	for(i = 0;i < ZX_key_info_length;i++)
	{
       if(ZX_key_info[i].Key_name==key_number)
       {
  
            if(  (ZX_key_info[i].Key_function[1][1])==0//没有双击
			   &&(ZX_key_info[i].Key_function[2][1])==0)//没有三击
            {
                  key_more = 0;
			}
			else
			{
			
                if((ZX_key_info[i].Key_function[1][1])!=0)//有双击
			    {
	                 key_more = _Key_double_;
					
				}
				
				//如果一个按键定义了双击和三击, return 三击
				if((ZX_key_info[i].Key_function[2][1])!=0)//有三击
			    {
			         key_more = _Key_CLICK3_;
					
				}

			}
			
			
	   }
	}

     return key_more;
}


/*
*   查询触发的按键是否有做N秒消息(1秒以上)
*  return FALSE  触发的按键无N秒消息
*  return  触发的按键有N秒消息, 2: 两秒, 3:3秒, 4:4秒, 5:5秒, 6: 6秒
*/
u8 Is_key_Ns_msg(u16 KeyIndex)
{

	 uint8_t i=0;
	 u8 ns = 0;

 // 暂时1个按键只支持1个 N秒消息(1秒以上)
	 
	for(i = 0;i < ZX_key_info_length;i++)
	{
       if(ZX_key_info[i].Key_name==KeyIndex)
       {                   
         		   		
			  if(    (ZX_key_info[i].Key_function[4][1])==0 //两秒
			      &&((ZX_key_info[i].Key_function[5][1])==0)// 3秒
			      &&((ZX_key_info[i].Key_function[6][1])==0)// 4秒
			      &&((ZX_key_info[i].Key_function[7][1])==0)// 5秒
			      &&((ZX_key_info[i].Key_function[8][1])==0)// 6秒
			      )
		      {
		          //触发的按键无n秒消息
		          //APP_DBG("key 000000000000000000\n");
		          ns = 0;
				  return ns; 
			  }
			  else if(  (ZX_key_info[i].Key_function[4][1])!=0 //两秒
			      &&((ZX_key_info[i].Key_function[5][1])==0)// 3秒
			      &&((ZX_key_info[i].Key_function[6][1])==0)// 4秒
			      &&((ZX_key_info[i].Key_function[7][1])==0)// 5秒
			      &&((ZX_key_info[i].Key_function[8][1])==0)// 6秒
			      )
			  {
                   ns = 2;
				   return ns;
			  }
			  else if(  (ZX_key_info[i].Key_function[4][1])==0 //两秒
			      &&((ZX_key_info[i].Key_function[5][1])!=0)// 3秒
			      &&((ZX_key_info[i].Key_function[6][1])==0)// 4秒
			      &&((ZX_key_info[i].Key_function[7][1])==0)// 5秒
			      &&((ZX_key_info[i].Key_function[8][1])==0)// 6秒
			      )
			  {
                   ns = 3;
				   return ns;
			  }
			  else if(  (ZX_key_info[i].Key_function[4][1])==0 //两秒
			      &&((ZX_key_info[i].Key_function[5][1])==0)// 3秒
			      &&((ZX_key_info[i].Key_function[6][1])!=0)// 4秒
			      &&((ZX_key_info[i].Key_function[7][1])==0)// 5秒
			      &&((ZX_key_info[i].Key_function[8][1])==0)// 6秒
			      )
			  {
                   ns = 4;
				   return ns;
			  }
			  else if(  (ZX_key_info[i].Key_function[4][1])==0 //两秒
			      &&((ZX_key_info[i].Key_function[5][1])==0)// 3秒
			      &&((ZX_key_info[i].Key_function[6][1])==0)// 4秒
			      &&((ZX_key_info[i].Key_function[7][1])!=0)// 5秒
			      &&((ZX_key_info[i].Key_function[8][1])==0)// 6秒
			      )
			  {
                   ns = 5;
				   return ns;
			  }
		     else if(  (ZX_key_info[i].Key_function[4][1])==0 //两秒
			      &&((ZX_key_info[i].Key_function[5][1])==0)// 3秒
			      &&((ZX_key_info[i].Key_function[6][1])==0)// 4秒
			      &&((ZX_key_info[i].Key_function[7][1])==0)// 5秒
			      &&((ZX_key_info[i].Key_function[8][1])!=0)// 6秒
			      )
			  {
                   ns = 6;
				   return ns;
			  }
	   }
	}

}


