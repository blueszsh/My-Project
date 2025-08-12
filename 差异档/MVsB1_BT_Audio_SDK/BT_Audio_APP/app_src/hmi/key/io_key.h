/**
 **************************************************************************************
 * @file    IO_key.h
 * @brief   IO key 
 *
 * @author  kenbu/bkd
 * @version V0.0.1
 *
 * $Created: 2018-09-03 
 *
 * @Copyright (C) 2018, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */


#ifndef __IO_KEY_H__
#define __IO_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"


#define 	IO_CHANNEL_EMPTY               0xFF


typedef enum _IOKeyType
{
	IO_KEY_UNKOWN_TYPE = 0,
	IO_KEY_PRESSED,
	IO_KEY_RELEASED,
	IO_KEY_LONG_PRESSED,
	IO_KEY_LONG_PRESS_HOLD,
	IO_KEY_LONG_RELEASED,
	IO_KEY_DOUBLE_RELEASED,  // // zsh A2
	IO_KEY_3_CLICK_RELEASED = 8,
	IO_KEY_LONG_2S_RELEASED,
	IO_KEY_LONG_3S_RELEASED,
	IO_KEY_LONG_4S_RELEASED,
	IO_KEY_LONG_5S_RELEASED,
	IO_KEY_LONG_6S_RELEASED,
	
}IOKeyType;

typedef struct _IOKeyMsg
{
    uint16_t index;
    uint16_t type;
}IOKeyMsg;

IOKeyMsg IOKeyScan(void);

int32_t IOKeyInit(void);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif //__IO_KEY_H__

