/*
*****************************************************************************
* Filename:    Led.C
* Description:
* maintainer: Louis
******************************************************************************
*/
#include <string.h>
#include "type.h"
#include "app_config.h"
#include "gpio.h"
//#include "stdint.h"
#include "dma.h"
#include "delay.h"
#include "timeout.h"
#include "main_task.h"
#include "reset.h"
#include "irqn.h"
#include "ctrlvars.h"
#include "bt_manager.h"
#include "bt_play_api.h"
#include "media_play_api.h"


#ifdef CFG_DMA_RGB_LED_EN


u32  curr_DAC_energy=0;
u32  curr_DAC_energy2=0;
bool  RGB_OFF_FLAG=0;	
u32  curr_MAX_DAC_energy=0;	

u32 _curr_DAC_energy[10]={0};
u8 DAC_energy_i=0;

#define energy_cnt                 2
u32 get_curr_DAC_energy[energy_cnt] = {0};
u8  DAC_energy_cnt=0;



u16 APP_LED_speed=0;

u32  APP_Curr_DAC_energy=0;
u16  APP_Curr_DAC_grade=0;


TIMER APP_gRGBVolTime;




volatile uint8_t DmaLedDataFlag = 0;

uint8_t gLedG,gLedR,gLedB,gLedCount,gLednum,led_test;
uint8_t  led_Time_5ms;
TIMER gLedSwitchTime;
TIMER gLedSwitchTime_delay;
TIMER gRGBScanTime;
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
TIMER gRGBScanTime1;
uint8_t DataChangeFlag;
#endif
//音频通路资源初始化
#define LED_BUFFER_LEN		(8*4*3*7+300)//(8*4*3*32+300)//8*4*3*3 + 800//1024
#define	ADR_GPIO_A_REG_O	(0x40010004+0)
#define	ADR_GPIO_B_REG_O	(0x40010034)
#define	REG_TIMER4_CTRL		(*(volatile unsigned long *) 0x4002C800)
#define	REG_TIMER4_NUM		(*(volatile unsigned long *) 0x4002C80C)
//#define gpio_a20             1<<4
#define RGB_LED_NUM       7  

#define ENERGY_LEVEL		14		//能量等级14级
#define RGB_MAX				252		//rgbj最大亮度
#define FFT_PAUSE			150	//	//3s
#define RGB_RUN_MAX			4//12		//12个灯一起跑圈
#define RGB_LED_MAX			16//24		//一个圈有24个灯
#define RGB_MODE2_RUN_SLOW	166		//166ms
#define RGB_MODE_MAX		3	//	//3个闪灯模式
#define RGB_MODE3_MAX		14		//闪灯模式3

#define RGB_MODE3_RUN_SLOW	200	//	//1s
uint32_t rgb_buf[3*RGB_LED_NUM];
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
uint32_t rgb_buf1[3*RGB_LED_NUM];
#endif


enum
{
	RED=0, //红
	GREEN, //绿
	BLUE,  //蓝
	YELLOW,//黄
	PINK,  //粉
	CYAN,  //青
	WHITE,  //白
	Colour_Other,  //其它

};

enum //彩虹
{
	Rainbow_RED=0, //红
	Rainbow_ORANGE, //橙
	Rainbow_YELLOW,  //黄
	Rainbow_GREEN,//绿
	Rainbow_BLUE,  //蓝
	Rainbow_CYAN,  //靛
	Rainbow_purple,  //紫

    Rainbow_Close,//关闭
};



uint32_t rgb_data_change(uint8_t data);	//24 = 3*8
const uint8_t rgb_1_level_led[ENERGY_LEVEL][4] =
{
    {0,	1, 2, 3},
	{4,	5, 6, 7},
	{8,	9, 10, 11},
	{12,13, 14, 15},
	{16,17, 18, 19},
	{20,21, 22, 23},
	{24,25, 26, 27},
	{28,29,	30, 31},
};
const uint8_t rgb_2_level_run[ENERGY_LEVEL] =	//单位 1ms
{
	166, 156, 146, 136, 126, 116, 40, 35, 30, 25, 20, 16, 13, 10,
};
const uint8_t rgb_2_level_add[ENERGY_LEVEL] =	//单位 1ms
{
	6, 7, 7, 8, 8, 9, 25, 29, 33, 40, 50, 63, 77, 100,
};
const uint8_t rgb_3_level_run[ENERGY_LEVEL] =	//单位 5ms
{
	200, 180, 160, 140, 120, 100, 50, 40, 35, 30, 25, 20, 15, 10,
};
const uint8_t rgb_3_level_led[RGB_MODE3_MAX][2] =
{
	{0, 6},
	{0, 12},
	{12,12},
	{6, 6},
	{6, 12},
	{18,12},
	{0, 24},
	{12,6},
	{12,12},
	{0, 12},
	{18,6},
	{18,12},
	{6, 12},
	{0, 24},
};

uint8_t const energy_qtz_tbl[ENERGY_LEVEL] = {
	3,14,22,30,40,51,61,71,76,81,85,91,96,100,
};
//jasontest 要加上AT(.com_text.rgb.tbl)   //非线性量化表.
uint8_t const energy_qtz_tbl_low[ENERGY_LEVEL] = {
	3,14,18,22,30,40,43,48,53,60,63,68,73,80,
};
//jasontest 要加上
uint8_t const energy_qtz_tbl_high[ENERGY_LEVEL] = {
	3,14,22,40,61,76,85,91,96,100,105,110,115,120,
};
#if 1
#if 1//ndef CFG_MORE_GPIO_RGB_CTRL_EN
#define LED_PORT		LED_GPIO1//	//1<<4
#define	LED_GPIO1		1<<6

#else
#define LED_PORT		LED_GPIO1|LED_GPIO2		//(1<<4)|(1<<6)
#define	LED_GPIO1		1<<4
#define	LED_GPIO2		1<<6
#endif



uint8_t  DmaTestBuffer[LED_BUFFER_LEN]=
{
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
////1
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
////2
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
////3
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
////4
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//5
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//6

	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
 //7
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,

#if 0
//8
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//9
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//10
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//11
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
 //12
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//13
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//14
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//15 
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//16

	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//17
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//18
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//19
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//20
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//21
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//22

	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//23 
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//24

    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//25
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//26
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//27
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//28
  
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//29
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,

//30

    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
//31 
    LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,

//32
#endif

};  
#else
   uint32_t  DmaTestBuffer[LED_BUFFER_LEN]=
{
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//1
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//2
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//3
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//4
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//5
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//6
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//7
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//8
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//1
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//2
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//3
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//4
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//5
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//6
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//7
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//8
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//1
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//2
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//3
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//4
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//5
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//6
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//7
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//8
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//1
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//2
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//3
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//4
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//5
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//6
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//7
    gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
	gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,gpio_a20,0,0,0,
//8
};
#endif 

/*
*********************************************************
*				IO口和DMA通道初始化
*********************************************************
*/
void Led_Dma_Proc_Init(void)
{
	DMA_CONFIG DMAConfig;
	uint32_t DmaTestLen;
	memset(DmaTestBuffer,0,sizeof(DmaTestBuffer));
	GPIO_RegOneBitClear(GPIO_A_IE, RGB_effect_SW_ctr_pin);
	GPIO_RegOneBitSet(GPIO_A_OE, RGB_effect_SW_ctr_pin);
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
	GPIO_RegOneBitClear(GPIO_A_IE, GPIOA22);
	GPIO_RegOneBitSet(GPIO_A_OE, GPIOA22);
#endif
	DmaTestLen = LED_BUFFER_LEN;//LED_BUFFER_LEN*4;
	DMAConfig.Dir = DMA_CHANNEL_DIR_MEM2PERI;
	DMAConfig.Mode = DMA_BLOCK_MODE; //DMA_CIRCULAR_MODE;
	DMAConfig.SrcAddress = (uint32_t)DmaTestBuffer;
	DMAConfig.SrcDataWidth = DMA_SRC_DWIDTH_BYTE;
	DMAConfig.SrcAddrIncremental = DMA_SRC_AINCR_SRC_WIDTH;
	DMAConfig.DstAddress = ADR_GPIO_A_REG_O;
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
	GPIO_RegBitsSet(GPIO_A_DMA_OUT_MASK, (~(GPIO_INDEX20|GPIO_INDEX22)));
#else
	GPIO_RegBitsSet(GPIO_A_DMA_OUT_MASK, (~(RGB_CTR_GPIO_INDEX)));
#endif
	DMAConfig.DstDataWidth =  DMA_DST_DWIDTH_BYTE;
	DMAConfig.DstAddrIncremental = DMA_DST_AINCR_NO;
	DMAConfig.BufferLen = DmaTestLen;
	DMA_TimerConfig(PERIPHERAL_ID_TIMER4, &DMAConfig);
	DMA_BlockConfig(PERIPHERAL_ID_TIMER4);
	DMA_BlockBufSet(PERIPHERAL_ID_TIMER4,DmaTestBuffer,DmaTestLen);
	//DMA_CircularWritePtrSet(PERIPHERAL_ID_TIMER4, DmaTestLen+16);//将写指针指向buffer外
	DMA_ChannelEnable(PERIPHERAL_ID_TIMER4);
	Timer_Config(TIMER4, 1, 0);
	REG_TIMER4_NUM = 47;//41;//
	Timer_Start(TIMER4);
	REG_TIMER4_CTRL |= (1<<8);
	TimeOutSet(&gRGBScanTime,20);
}

/*
******************************************************
*				将数据传送到DMA通道中
******************************************************
*/
void DmaDataChange1(void)
{
	uint32_t TempData,TempData1;
	uint16_t j;
	uint8_t i,k;

//	if(DataNum > 2) DataNum -= 2;
//	TempData = GRB_Table[DataNum];
	
   //for(i=0;i<96;i++)
   for(i=0;i<21;i++)
   {
     
    for(j=0;j<32;j++)
	{
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
		if(DataChangeFlag)
		{
#endif
			TempData = 0;
			if(rgb_buf[i] &  BIT(31-j)){
				TempData = LED_GPIO1;//TempData = LED_PORT;
			}
	        DmaTestBuffer[ i*32+j] =(uint8_t) TempData;
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
		}
		else
		{
			TempData = 0;
			if(rgb_buf1[i] &  BIT(31-j)){
				TempData = LED_GPIO2;//TempData = LED_PORT;
			}
	        DmaTestBuffer[ i*32+j] =(uint8_t) TempData;
		}
#endif
    }
  }
}

#if 0//TEST
uint8_t  DmaTestBufferTemp[8*4*3*8]=
{
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//1
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//2
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//3
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//4
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//5
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//6
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	//7
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,
	LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,0,0,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,LED_PORT,LED_PORT,LED_PORT,0,	//8
};

uint32_t CountColor=0;
void DmaDataShift(uint32_t LedIndex)
{
	uint32_t i = 0;
	memset(DmaTestBuffer1,0,sizeof(DmaTestBuffer1));
	//75%占空比点亮，25%占空比熄灭
	for(i=0;i<8*4*3*32/4;i++)
	{
		DmaTestBuffer1[i*4] = LED_PORT;
		DmaTestBuffer1[i*4+1] = 0;
		DmaTestBuffer1[i*4+2] = 0;
	}

	if(LedIndex<=24)
	{
		memcpy(&DmaTestBuffer1[8*4*3*LedIndex],DmaTestBufferTemp,sizeof(DmaTestBufferTemp));
	}
	else if(LedIndex>24)
	{
		memcpy(&DmaTestBuffer1[8*4*3*LedIndex],DmaTestBufferTemp,8*4*3*(32-LedIndex));
		memcpy(&DmaTestBuffer1[0],DmaTestBufferTemp,sizeof(DmaTestBufferTemp) - 8*4*3*(32-LedIndex));
	}
}
#endif

uint8_t LedInit = 2;
uint32_t LedIndex=0;
uint32_t Count20ms=0;

/*
****************************************************
*			刷新DMA通道数据
****************************************************
*/
void LedDmaDataMode(void)
{
	DMA_CONFIG DMAConfig;
	uint32_t DmaTestLen;
	int32_t i,test;

	DmaTestLen = LED_BUFFER_LEN;
	//NVIC_DisableIRQ(Timer5_IRQn);
	if(LedInit==2) return;
	if(LedInit==0)
	{
       Led_Dma_Proc_Init();
	   LedInit=1;
       TimeOutSet(&gLedSwitchTime_delay,50);
       TimeOutSet(&gLedSwitchTime, 500); // 2000
     
	  return;
	}

	if(IsTimeOut(&gLedSwitchTime))
	{
	  //RGB_Timer_OFF();
	  NVIC_DisableIRQ(Timer5_IRQn); 
		DmaLedDataFlag = 1;
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
		DataChangeFlag = !DataChangeFlag;
#endif
 		//TimeOutSet(&gLedSwitchTime, 40);
 		TimeOutSet(&gLedSwitchTime, 40);

		DmaDataChange1();
		DMA_InterruptFlagClear(PERIPHERAL_ID_TIMER4,DMA_DONE_INT);
		Timer_InterruptFlagClear(TIMER4,UPDATE_INTERRUPT_SRC);
		Timer_Pause(TIMER4,1);
		DMA_ChannelDisable(PERIPHERAL_ID_TIMER4);
		DMA_BlockBufSet(PERIPHERAL_ID_TIMER4,DmaTestBuffer,DmaTestLen);
		DMA_ChannelEnable(PERIPHERAL_ID_TIMER4);
		Timer_Pause(TIMER4,0);
		DmaLedDataFlag = 0;

        NVIC_EnableIRQ(Timer5_IRQn); 
		//RGB_Timer_ON();
#if TEST
 		Count20ms++;
 		if(Count20ms%10==0)
 		{
 			DmaDataShift(LedIndex);
 			LedIndex++;
 			if(LedIndex>31)
 			{
 				LedIndex = 0;
 			}
 		}

 		memcpy(DmaTestBuffer,DmaTestBuffer1,sizeof(DmaTestBuffer1));
#endif
#if 0
/
 		  DmaDataChange(gLedCount,0);
	      DmaDataChange(gLedCount+1,1);
          DmaDataChange(gLedCount+2,2);
          DmaDataChange(gLedCount+0,3);
          DmaDataChange(gLedCount+1,4);
          DmaDataChange(gLedCount+2,5);
        DmaDataChange(gLedCount+1,6);
        DmaDataChange(gLedCount+0,7);
        DmaDataChange(gLedCount+1,8);
        DmaDataChange(gLedCount+2,9);
        DmaDataChange(gLedCount+1,10);
        DmaDataChange(gLedCount+0,11);
        DmaDataChange(gLedCount+1,12);
        DmaDataChange(gLedCount+2,13);
        DmaDataChange(gLedCount+1,14);
        DmaDataChange(gLedCount+0,15);
        DmaDataChange(gLedCount+1,16);
        DmaDataChange(gLedCount+2,17);
        DmaDataChange(gLedCount+1,18);
        DmaDataChange(gLedCount+0,19);
        DmaDataChange(gLedCount+1,20);
        DmaDataChange(gLedCount+2,21);
        DmaDataChange(gLedCount+1,22);
        DmaDataChange(gLedCount+0,23);
        DmaDataChange(gLedCount+1,24);
        DmaDataChange(gLedCount+2,25);
        DmaDataChange(gLedCount+1,26);
        DmaDataChange(gLedCount+0,27);
        DmaDataChange(gLedCount+1,28);
        DmaDataChange(gLedCount+0,29);
        DmaDataChange(gLedCount+0,30);
        DmaDataChange(gLedCount+0,31);
        /*      DmaDataChange(gLedCount+0,29);
        DmaDataChange(gLedCount+0,30);
        DmaDataChange(gLedCount+0,31);*/
        //LedInit = 2;
#endif


    }
	//DelayMs(5);
	//NVIC_EnableIRQ(Timer5_IRQn);
	//DmaLedDataFlag = 0;
}

/*
///////////////////////////////////////////////////////////////////
//						数据转换函数1							 //
///////////////////////////////////////////////////////////////////
*/
uint32_t rgb_data_change(uint8_t data)	//24 = 3*8
{
	uint32_t data_u32 = 0;
    uint8_t i;
	for(i = 0;i<8;i++){
		// 1 --> 1110   0--> 1000
		if(data & BIT(7)){
			data_u32 = data_u32 | (0x0e);
		}else{
			data_u32 = data_u32 | (0x08);
		}
		data <<= 1;
		if(i < 7){
			data_u32 <<= 4;
		}
	}
	return data_u32;
}

/*
///////////////////////////////////////////////////////////////////
//						数据转换函数2							 //
///////////////////////////////////////////////////////////////////
*/
void rgb_data_deal(void)
{
	uint32_t g_data_32,r_data_32,b_data_32 = 0;
    uint8_t i;
  
    g_data_32 = 0;
	r_data_32 = 0;
	b_data_32 = 0;
	for( i = 0;i< RGB_LED_NUM;i++){

        r_data_32 = rgb_data_change(RGB_R.buf[i]);
        g_data_32 = rgb_data_change(RGB_G.buf[i]);
        b_data_32 = rgb_data_change(RGB_B.buf[i]);
        rgb_buf[i*3] = g_data_32;
        rgb_buf[i*3+1] =  r_data_32;
        rgb_buf[i*3+2] = b_data_32;
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
		r_data_32 = rgb_data_change(RGB_R.buf1[i]);
        g_data_32 = rgb_data_change(RGB_G.buf1[i]);
        b_data_32 = rgb_data_change(RGB_B.buf1[i]);
        rgb_buf1[i*3] = g_data_32;
        rgb_buf1[i*3+1] =  r_data_32;
        rgb_buf1[i*3+2] = b_data_32;
#endif
	}
}

/*
//////////////////////////////////////////////////////////////////
//					RGB闪烁模式处理								//
//////////////////////////////////////////////////////////////////
*/
#if 0
void ModeTest(void)
{
	uint8_t i,j;
	//mainAppCt.rgb_mode = 1;
	switch(mainAppCt.rgb_mode)
	{
		case 0:					//只做所有灯呼吸灯渐变
			if(IsTimeOut(&gRGBScanTime))
			{
				TimeOutSet(&gRGBScanTime,10);
				if((mainAppCt.r_duty < 255)&&(mainAppCt.g_duty == 0)&&(mainAppCt.b_duty == 0))
				{
					mainAppCt.r_duty++;
				}
				else if((mainAppCt.r_duty > 0)&&(mainAppCt.g_duty < 255)&&(mainAppCt.b_duty == 0))
				{
					mainAppCt.r_duty--;
					mainAppCt.g_duty++;
				}
				else if((mainAppCt.r_duty == 0)&&(mainAppCt.g_duty > 0)&&(mainAppCt.b_duty < 255))
				{
					mainAppCt.g_duty--;
					mainAppCt.b_duty++;
				}
				else if((mainAppCt.r_duty < 255)&&(mainAppCt.g_duty == 0)&&(mainAppCt.b_duty > 0))
				{
					mainAppCt.b_duty--;
					mainAppCt.r_duty++;
				}
				for(i=0;i<RGB_LED_NUM;i++)		//此处将每次颜色渐变的数据传输到
				{
					RGB_R.buf[i] = mainAppCt.r_duty;
					RGB_G.buf[i] = mainAppCt.g_duty;
					RGB_B.buf[i] = mainAppCt.b_duty;
				}
			}
			break;
		case 1:						//灯圈一边颜色渐变一边环绕亮灯
			if(IsTimeOut(&gRGBScanTime))
			{
				TimeOutSet(&gRGBScanTime,80);
//************************************************************	//以下这一部分为颜色渐变的处理
				if(mainAppCt.rgb_duty_add>0)
				{
					RGB_R.buf[mainAppCt.rgb_duty_add-1] = 0;
					RGB_G.buf[mainAppCt.rgb_duty_add-1] = 0;
					RGB_B.buf[mainAppCt.rgb_duty_add-1] = 0;
				}
				if((mainAppCt.r_duty < 255)&&(mainAppCt.g_duty == 0)&&(mainAppCt.b_duty == 0))
				{
					mainAppCt.r_duty+=15;
				}
				else if((mainAppCt.r_duty > 0)&&(mainAppCt.g_duty < 255)&&(mainAppCt.b_duty == 0))
				{
					mainAppCt.r_duty-=15;
					mainAppCt.g_duty+=15;
				}
				else if((mainAppCt.r_duty == 0)&&(mainAppCt.g_duty > 0)&&(mainAppCt.b_duty < 255))
				{
					mainAppCt.g_duty-=15;
					mainAppCt.b_duty+=15;
				}
				else if((mainAppCt.r_duty < 255)&&(mainAppCt.g_duty == 0)&&(mainAppCt.b_duty > 0))
				{
					mainAppCt.b_duty-=15;
					mainAppCt.r_duty+=15;
				}
//**************************************************************
				if(mainAppCt.rgb_duty_add < 24)			//一个灯圈中灯的总数
				{
					mainAppCt.rgb_duty_add++;
				}
				else
					mainAppCt.rgb_duty_add = 0;
				for(i=0;i<3;i++)
				{
					RGB_R.buf[mainAppCt.rgb_duty_add+i] = mainAppCt.r_duty;
					RGB_G.buf[mainAppCt.rgb_duty_add+i] = mainAppCt.g_duty;
					RGB_B.buf[mainAppCt.rgb_duty_add+i] = mainAppCt.b_duty;
				}
			}
			break;
	}
}

#else
void ModeTest(void)
{

	switch(mainAppCt.rgb_mode)
	{
		case RGB_Effect_PowerOn:		
               DMA_RGB_PowerOn_Show();
			  
			break;
			
		case RGB_Effect_PowerOff:						
			  
			break;
	    case RGB_Effect_Bt_Con:
                DMA_RGB_Bt_Con_Show();
			 break;
			 
		case RGB_Effect_Bt_Dis:
			    DMA_RGB_Bt_Dis_Show();
			 break;
			 
	    case RGB_Effect_Bt_Play:
			      DMA_RGB_Bt_Play_Show();
			 break;	

		case RGB_Effect_Bt_Pause:
			      DMA_RGB_Bt_Pause_Show();
			 break;	
			 
		case RGB_Effect_HFP_CALL_IN:
			    DMA_RGB_HFP_CALL_IN_Show();
			 break;	

		/* case RGB_Effect_HFP_Answer:
			 break;	*/
			 
		case RGB_Effect_Lowpower:
			   DMA_RGB_LowPower_Show();
			 break;	

		case RGB_Effect_PowerOn_Charge:
			   DMA_RGB_PowerOn_Charge_Show();
			 break;	
			 
		case RGB_Effect_PowerOff_Charge:
			   DMA_RGB_PowerOff_Charge_Show();
			 break;	
			 
		case RGB_Effect_TWS_Con:
			   DMA_RGB_TWS_Show();
			 break;
			 
		case 0xff:
			    User_ALL_RGB_OFF();//全灭
			 break;	
	}
}

#endif


/*
  LedNum: 第几个灯
  Color :  颜色  0:R   1:G   2:B   3:R+G   4:R+B  5:G+B     6:R+G+B
                 红     绿    蓝    黄      粉    青(淡蓝)    白
  ctr   :  亮或灭 0: 灭    1: 亮
*/
void RGB_Show_Color(u8 LedNum, u8 Color,u8 ctr)
{


      switch(Color)
		{
	        case 0:
	            mainAppCt.r_duty= 150;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= 0;
	            break;
	        case 1:
	          
	            mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= 150;
	            mainAppCt.b_duty= 0;
	            break;
	        case 2:
	            mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= 150;
	            break;
	        case 3:
	             mainAppCt.r_duty= 150;
	            mainAppCt.g_duty= 150;
	            mainAppCt.b_duty= 0;
	            break;
	        case 4:
	             mainAppCt.r_duty= 150;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= 150;
	            break;
	        case 5:
	            mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= 150;
	            mainAppCt.b_duty= 150;
	            break;
	        case 6:
	            mainAppCt.r_duty= 150;
	            mainAppCt.g_duty= 150;
	            mainAppCt.b_duty= 150;
	            break;
			case 7:
				  //淡绿色
	            mainAppCt.r_duty= 40;
	            mainAppCt.g_duty= 150;
	            mainAppCt.b_duty= 20;
	            break;
		}


	    if(ctr == 0)
	    { 
	       mainAppCt.r_duty = 0;
           mainAppCt.g_duty = 0;
           mainAppCt.b_duty = 0;
		}


        RGB_R.buf[LedNum] = mainAppCt.r_duty;
        RGB_G.buf[LedNum] = mainAppCt.g_duty;
        RGB_B.buf[LedNum] = mainAppCt.b_duty;
   
}


/*
  LedNum: 第几个灯
  Color :  颜色  
                 
  ctr   :  亮或灭 0: 灭    1: 亮
*/
void RGB_Rainbow_Color(u8 LedNum, u8 Color,u8 ctr)
{


      switch(Color)
		{
	        case Rainbow_RED:
	            mainAppCt.r_duty= RGB_MAX;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= 0;
	            break;
				
	        case Rainbow_ORANGE:
	            mainAppCt.r_duty= RGB_MAX;
	            mainAppCt.g_duty= 125;
	            mainAppCt.b_duty= 0;
	            break;
				
	        case Rainbow_YELLOW:
	            mainAppCt.r_duty= RGB_MAX;
	            mainAppCt.g_duty= RGB_MAX;
	            mainAppCt.b_duty= 0;
	            break;
				
	        case Rainbow_GREEN:
	            mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= RGB_MAX;
	            mainAppCt.b_duty= 0;
	            break;
				
	        case Rainbow_BLUE:
	             mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= RGB_MAX;
	            break;
				
	        case Rainbow_CYAN:
	            mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= RGB_MAX;
	            mainAppCt.b_duty= RGB_MAX;
	            break;
				
	        case Rainbow_purple:
	            mainAppCt.r_duty= RGB_MAX;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= RGB_MAX;
	            break;
				
			case Rainbow_Close:
				
	            mainAppCt.r_duty= 0;
	            mainAppCt.g_duty= 0;
	            mainAppCt.b_duty= 0;
	            break;
		}


	    if(ctr == 0)
	    { 
	       mainAppCt.r_duty = 0;
           mainAppCt.g_duty = 0;
           mainAppCt.b_duty = 0;
		}


        RGB_R.buf[LedNum] = mainAppCt.r_duty;
        RGB_G.buf[LedNum] = mainAppCt.g_duty;
        RGB_B.buf[LedNum] = mainAppCt.b_duty;
   
}




/*
    Leds: 从0到 Leds个灯点亮,Leds 之后的灭掉
    Color:颜色   
*/
void RGB_LEDS_Show_Color(u8 Leds, u8 Color)
{
     u8 i=0;

   
     for(i=0;i<8;i++)
     {
         if(i < Leds)
         {
            RGB_Show_Color(i, Color,1);
         }
		 else
		 {
             RGB_Show_Color(i, Color,0);
		 }
     }
	 	
}



/*
   RGB LED 全部点亮
*/
void User_ALL_RGB_ON(void)
{
     static u8 led=0;
     static u8 colour;
	 static u8 ctr = 1;


    RGB_Show_Color(led, colour,ctr);
     led++;
	 colour++;
     
	 if(led >= RGB_LED_NUM)
	 {	   
	      
          led=0;
	 }
	 if(colour > Colour_Other)
	 {
        colour = 0;
	 }

    
}

/*
   RGB LED 全部灭
*/
void User_ALL_RGB_OFF(void)
{
     static u8 led=0;
     static u8 colour;
	 static u8 ctr = 0;


    RGB_Show_Color(led, colour,0);
     led++;
	 colour++;
     
	 if(led >= RGB_LED_NUM)
	 {	   
	      
          led=0;
	 }
	/* if(colour > Colour_Other)
	 {
        colour = 0;
	 }*/

	#if 0//RGB_Since_the_flash_EN
         RGB_Since_the_flash_OFF;
	#endif
    
}

/*
   全部灯点亮显示红色 
*/
void User_ALL_RGB_RED(void)
{
     static u8 led=0;
     static u8 colour;
	


     RGB_Show_Color(led, RED,1);
	  led++;
	  if(led >= RGB_LED_NUM)
	  {
            led=0;
	  }

	/* for(led=0;led < RGB_LED_NUM;led++)
	 {
           RGB_Show_Color(led, RED,1);
	 }*/
    
    /* for(led=0;led < RGB_LED_NUM;led++)
	 {
	     if(led < 12)
	     {
              RGB_Show_Color(led, RED,0);
		 } 
		 else
		 {
              RGB_Show_Color(led, RED,1);
		 }
           
	 }*/
	 
    
}




/*
   彩色灯转圈

*/
void Colours_Led_Loop(void)
{
    static u8 led=0;
	static u8 set_led=0;
	static u8 i=0;
	static u8 j=0;
	static u8 color = RED;
	static u8 colour2 = RED;

// {RED,GREEN,BLUE,YELLOW,PINK,CYAN,WHITE,Colour_Other},
	static u8 colour_Sorting[RGB_LED_NUM][RGB_LED_NUM] = 
	{
        {RED,GREEN,BLUE,YELLOW},
		{GREEN,BLUE,YELLOW,RED},
		{BLUE,YELLOW,RED,GREEN},
		{YELLOW,RED,GREEN,BLUE},	
        
	};
	

 
	 for(led=0;led<RGB_LED_NUM;led++)
     {
		 color = colour_Sorting[i][led];  
		 RGB_Show_Color(led,color,1); 
     }
	
  	 
	 i++;
	 if(i >= RGB_LED_NUM)
	 {
	    i=0;
	 }
	 
	



}







//渐变
void RGB_gradual_change(void)
{

	 u8 i =0;
	 static u8 r_duty=0;
	 static u8 g_duty=0;
	 static u8 b_duty=0;
	 
           if((r_duty < 255)&&(g_duty == 0)&&(b_duty == 0))
			{
				r_duty++;
			}
			else if((r_duty > 0)&&(g_duty < 255)&&(b_duty == 0))
			{
				r_duty--;
				g_duty++;
			}
			else if((r_duty == 0)&&(g_duty > 0)&&(b_duty < 255))
			{
				g_duty--;
				b_duty++;
			}
			else if((r_duty < 255)&&(g_duty == 0)&&(b_duty > 0))
			{
				b_duty--;
				r_duty++;
			}
			else
			{
                r_duty=0;
				g_duty=0;
				b_duty=0;
			}

			
			for(i=0;i<RGB_LED_NUM;i++)
			{
				RGB_R.buf[i] = r_duty;
				RGB_G.buf[i] = g_duty;
				RGB_B.buf[i] = b_duty;
			}


}









/*
    音乐律动 (闪动)
    不播放时,变色
*/
void DMA_RGB_effect_rhythm(void)
{
    
    uint8_t i = 0;
	static uint16_t ColorUse_1ms_cnt = 0;
	static uint8_t Color_Use = 1;
    static uint8_t LedNum = 16;
    static uint16_t TimeCnt = 0;
    static uint16_t rgb_breathe_cnt = 0;
	
    
    TimeCnt++;
	
	 
	DAC_energy_dealt();


	ColorUse_1ms_cnt++;//  
    
     if(ColorUse_1ms_cnt >= 4000)
     {
        ColorUse_1ms_cnt=0;
	    Color_Use++;
        if(Color_Use > 7)
        {
           Color_Use=1;
		}
		if(RGB_OFF_FLAG==0)
		{
            RGB_OFF_FLAG = 1;
		}
     }


	 if(curr_DAC_energy <= 2)//没音乐时,变色
	 {
         RGB_OFF_FLAG = 0;
	 }
		 
     TimeCnt = 0;
        
		switch(Color_Use)
		{
           case 1:
		   	       if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
		   	    break;
		   case 2:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= 0;
					}
		   	    break;
			case 3:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
			case 4:
		   	       if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= 0;
					}
		   	    break;
			case 5:
		   	       if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
			case 6:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
			case 7:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
		}
		
           
           

      
                     
   
    for(i = 0;i < RGB_LED_NUM;i++)
	{
        RGB_R.buf[i] = mainAppCt.r_duty;
        RGB_G.buf[i] = mainAppCt.g_duty;
        RGB_B.buf[i] = mainAppCt.b_duty;
    }

#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
    for(i = 0;i<RGB2_LED_NUM;i++){   
        RGB_G.buf1[i] = mainAppCt.g_duty;
        RGB_B.buf1[i] = mainAppCt.b_duty;
        RGB_R.buf1[i] = mainAppCt.r_duty;
    } 
#endif

	 
}



void rgb_breathe(void)//换色呼吸
{
    static uint8_t r_duty = 0;
	static uint8_t g_duty = 0;
	static uint8_t b_duty = 0;
    static uint8_t color = RED;
	static bool breathe_dir = 0; // 0 从灭到亮  1 从亮到灭
	uint8_t i = 0;
	
	switch(color)
	{
	  case RED:
             
             if(breathe_dir==0)
             {
                  r_duty++;
				  if(r_duty >= RGB_MAX)
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
                       color = GREEN;
					   breathe_dir = 0;
				 }
				 	
			 }

			 g_duty=0;
			 b_duty=0;
	   
		break;
			
	case GREEN: 
	    
             if(breathe_dir==0)
             {
                  g_duty++;
				  if(g_duty >= RGB_MAX)
				  {
                     breathe_dir = 1;
				  }
			 }
			 else
			 {
                 
				 if(g_duty > 0)
				 {
                     g_duty--;
				 }

				 if(g_duty == 0)
				 {
                       color = BLUE;
					   breathe_dir = 0;
				 }
				 	
			 }

			 r_duty=0;
			 b_duty=0;
	
		break;

	case BLUE: 
	    
             if(breathe_dir==0)
             {
                  b_duty++;
				  if(b_duty >= RGB_MAX)
				  {
                     breathe_dir = 1;
				  }
			 }
			 else
			 {
                 
				 if(b_duty > 0)
				 {
                     b_duty--;
				 }

				 if(b_duty == 0)
				 {
                       color = YELLOW;
					   breathe_dir = 0;
				 }
				 	
			 }

			 r_duty=0;
			 g_duty=0;
	
		break;
		
	case YELLOW: 
	        if(breathe_dir==0)
             {
                  r_duty++;
				  g_duty++;
				  if(r_duty >= RGB_MAX || g_duty >= RGB_MAX)
				  {
                     breathe_dir = 1;
				  }
			 }
			 else
			 {
                 
				 if(r_duty > 0 && g_duty > 0)
				 {
                     r_duty--;
					 g_duty--;
				 }

				 if(r_duty == 0 || g_duty == 0)
				 {
                       color = PINK;
					   breathe_dir = 0;
				 }
				 	
			 }

			 b_duty=0;
	
		break;

	case PINK: 
		
		    if(breathe_dir==0)
             {
                  r_duty++;
				  b_duty++;
				  if(r_duty >= RGB_MAX || b_duty >= RGB_MAX)
				  {
                     breathe_dir = 1;
				  }
			 }
			 else
			 {
                 
				 if(r_duty > 0 && b_duty > 0)
				 {
                     r_duty--;
					 b_duty--;
				 }

				 if(r_duty == 0 || b_duty == 0)
				 {
                       color = CYAN;
					   breathe_dir = 0;
				 }
				 	
			 }

			 g_duty=0;

			 break;
	case CYAN: 
		
	        if(breathe_dir==0)
             {
                  g_duty++;
				  b_duty++;
				  if(g_duty >= RGB_MAX || b_duty >= RGB_MAX)
				  {
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
                       color = WHITE;
					   breathe_dir = 0;
				 }
				 	
			 }

			 r_duty=0;

			 
		break;
		
	case WHITE:
		
	        if(breathe_dir==0)
             {
                  r_duty++;
                  g_duty++;
				  b_duty++;
				  if(r_duty >= RGB_MAX || g_duty >= RGB_MAX || b_duty >= RGB_MAX)
				  {
                     breathe_dir = 1;
				  }
			 }
			 else
			 {
                 
				 if(r_duty > 0 && g_duty > 0 && b_duty > 0)
				 {
				     r_duty--;
                     g_duty--;
					 b_duty--;
				 }

				 if(r_duty == 0 || g_duty == 0 || b_duty == 0)
				 {
                       color = RED;
					   breathe_dir = 0;
				 }
				 	
			 }

			 
		break;
		
	    default:
	    
	        break;
			
	}

	for(i = 0;i<RGB_LED_NUM;i++)
	{   
        RGB_G.buf[i] = g_duty;
        RGB_B.buf[i] = b_duty;
        RGB_R.buf[i] = r_duty;
    } 
#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
    for(i = 0;i<RGB2_LED_NUM;i++){   
        RGB_G.buf1[i] = g_duty;
        RGB_B.buf1[i] = b_duty;
        RGB_R.buf1[i] = r_duty;
    } 
#endif

}



//呼吸+随音乐律动
void RGB_DMA_Rhythm(void)
{
    
    uint8_t i = 0;
	static uint16_t ColorUse_1ms_cnt = 0;
	static uint8_t Color_Use = 0;
    static uint8_t LedNum = 16;
    static uint16_t TimeCnt = 0;
    static uint16_t rgb_breathe_cnt = 0;
	
    
    TimeCnt++;
	ColorUse_1ms_cnt++;//  
    
     if(ColorUse_1ms_cnt >= 4000)
     {
        ColorUse_1ms_cnt=0;
	    Color_Use++;
        if(Color_Use > 7)
        {
           Color_Use=1;
		}
		if(RGB_OFF_FLAG==0)
		{
            RGB_OFF_FLAG = 1;
		}
     }
	 
	DAC_energy_dealt();

	if(GetSystemMode() != ModeUsbDevicePlay)
	{
           if(Get_Global_playing_state()==0 || curr_DAC_energy <= 2)
		    {
		        rgb_breathe_cnt++;
				if(Get_Global_playing_state()==0 || rgb_breathe_cnt >= 1000)
				{
				    rgb_breathe_cnt = 1000;
		            if(IsTimeOut(&gRGBScanTime))
					{
					   TimeOutSet(&gRGBScanTime,9);
			           rgb_breathe();//呼吸
			        }
				   return;
				}
		        
			}
			else
			{
		         rgb_breathe_cnt = 0;
			}
	}
	else
	{
           if(curr_DAC_energy <= 2)
		    {
		        rgb_breathe_cnt++;
				if(rgb_breathe_cnt >= 1000)
				{
				    rgb_breathe_cnt = 1000;
		            if(IsTimeOut(&gRGBScanTime))
					{
					   TimeOutSet(&gRGBScanTime,9);
			           rgb_breathe();//呼吸
			        }
				   return;
				}
		        
			}
			else
			{
		         rgb_breathe_cnt = 0;
			}
	}
  
	
		 
     TimeCnt = 0;
        
		switch(Color_Use)
		{
           case 1:
		   	       if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
		   	    break;
		   case 2:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= 0;
					}
		   	    break;
			case 3:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
			case 4:
		   	       if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= 0;
					}
		   	    break;
			case 5:
		   	       if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
			case 6:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
			case 7:
		   	        if(RGB_OFF_FLAG)
		            {
		              mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
		              mainAppCt.r_duty= RGB_MAX;
		              mainAppCt.g_duty= RGB_MAX;
		              mainAppCt.b_duty= RGB_MAX;
					}
		   	    break;
		}
		
           
           

      
                     
   
    for(i = 0;i < RGB_LED_NUM;i++)
	{
        RGB_R.buf[i] = mainAppCt.r_duty;
        RGB_G.buf[i] = mainAppCt.g_duty;
        RGB_B.buf[i] = mainAppCt.b_duty;
    }

#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
    for(i = 0;i<RGB2_LED_NUM;i++){   
        RGB_G.buf1[i] = mainAppCt.g_duty;
        RGB_B.buf1[i] = mainAppCt.b_duty;
        RGB_R.buf1[i] = mainAppCt.r_duty;
    } 
#endif

	 
}


//逆时针转圈
void RGB_CCW_Turn(void)
{
    static uint8_t i = 0;
	static int8_t LedNum = RGB_LED_NUM;
    static uint8_t TimeCnt = 0;
    static uint8_t colour=0;
    static uint8_t ctr=0;
		
	
  	TimeCnt++;

	if(TimeCnt >= 20)
	{
        TimeCnt=0;
		 LedNum--;
		
	}

	
    for(i = 0;i <= RGB_LED_NUM;i++)
	{
	   
	   if(i < LedNum)
	   {
          ctr = 0;
		 
	   }
	   else
	   {
          ctr = 1;
	   }
	  
	   
	   RGB_Show_Color(i, colour, ctr);
          		
    }
	    
     if(LedNum == (-1))
	 {
	    LedNum=RGB_LED_NUM;
		colour++;
		if(colour >= 7)
		{
            colour=0;
		}
	 }

		

           
}

/*
    七彩灯,流水状走动
*/
void RGB_LED_mode13(void)
{
      uint8_t i = 0;
    static uint8_t LedNum = 0;
    static uint16_t TimeCnt = 0;
    static uint8_t ColorUse = 0;
	static uint8_t state = 0;
    static uint16_t TimeCnt2 = 0;

	
    static uint8_t Colorful[RGB_LED_NUM][RGB_LED_NUM] = 
    {
       	{0,1,2,3,4,5,6,7},
		{7,0,1,2,3,4,5,6},
		{6,7,0,1,2,3,4,5},
		{5,6,7,0,1,2,3,4},
		{4,5,6,7,0,1,2,3},
		{3,4,5,6,7,0,1,2},
		{2,3,4,5,6,7,0,1},
		{1,2,3,4,5,6,7,0},
	};




	
    TimeCnt++;
	if(TimeCnt >= 60)
	{
        TimeCnt = 0;
		state++;
     
            if(state >= RGB_LED_NUM)
			{
               state = 0;
			    
            }
        LedNum++;
		  
        if(LedNum > RGB_LED_NUM)
		{
            LedNum = 0;
            /* */
        }
                
      
    }


     for(i = 0;i < RGB_LED_NUM;i++)
     {
		  
		ColorUse = Colorful[state][i];		 
        RGB_Show_Color(i, ColorUse, 1);
					  
	 }

	 TimeCnt2++;
	 if(TimeCnt2 >= 4000)
	 {  
	    TimeCnt2 = 0;
       
	 }


}




//音律节奏
void DAC_energy_dealt(void)
{
    
    static u16 timer_cnt=0;
	static u16 RGB_OFF_cnt=0;
	static u16 RGB_OFF_cnt2=0;
	static u16 RGB_ON_cnt=0;
	static u8 flicker=0;
	static u8 flicker2=0;
	static u8 cnt=0;


	

    timer_cnt++;
	if(timer_cnt >= 100) //100
	{
	   timer_cnt = 0;
       curr_DAC_energy = Get_Music_Energy();
	   
	   if(curr_DAC_energy > 100)
	   {
           curr_DAC_energy2 = curr_DAC_energy / 100;
	   }


		 get_curr_DAC_energy[DAC_energy_cnt] =curr_DAC_energy2;
	     DAC_energy_cnt++;
		 if(DAC_energy_cnt >= energy_cnt)
		 {
	        DAC_energy_cnt=0;
		 }

   	
    #if 1
	    if(get_curr_DAC_energy[0] > get_curr_DAC_energy[1])
	    {
	          curr_MAX_DAC_energy = get_curr_DAC_energy[0];
		}
		else if(get_curr_DAC_energy[1] > get_curr_DAC_energy[0])
		{
	         curr_MAX_DAC_energy = get_curr_DAC_energy[1];
		}

		// if((curr_MAX_DAC_energy - curr_DAC_energy2) > 20)//50//30
		//if((curr_MAX_DAC_energy - curr_DAC_energy2) > 13)
		//if((curr_MAX_DAC_energy - curr_DAC_energy2) > 7)//5
		if(curr_DAC_energy > 0 && get_curr_DAC_energy[0] == get_curr_DAC_energy[1])
		{
		    cnt++;
			if(cnt <= 8)
			{
                RGB_OFF_FLAG = 1;
			}
			else if(cnt <= 9)
			{
                RGB_OFF_FLAG = 0;
			}
			else if(cnt <= 15)
			{
                RGB_OFF_FLAG = 1;
			}
			else if(cnt <= 16)
			{
                RGB_OFF_FLAG = 0;
			}
			else if(cnt <= 18)
			{
                RGB_OFF_FLAG = 1;
			}
			else if(cnt <= 19)
			{
                RGB_OFF_FLAG = 0;
			}
			else if(cnt <= 26)
			{
                RGB_OFF_FLAG = 1;
			}
			else if(cnt <= 27)
			{
                RGB_OFF_FLAG = 0;
			}
			else
			{
                cnt = 0;
			}

		}
		else if((  curr_MAX_DAC_energy - curr_DAC_energy2) > 6
	       || (curr_MAX_DAC_energy / 2) > curr_DAC_energy2) 
	     {
	          RGB_OFF_FLAG =0;
			  RGB_OFF_cnt = 0;
			  flicker = 0;
	          RGB_ON_cnt++;
			  if(RGB_ON_cnt >= 4)
			  {
                   RGB_OFF_cnt = 0;
				   RGB_OFF_FLAG = 1;
			  }
		 }
		 else
		 {
	          RGB_OFF_FLAG = 1;
			  RGB_OFF_cnt++;
			  RGB_ON_cnt=0;
			  
			  if(RGB_OFF_cnt >= 7)// 15
			  {
                  RGB_OFF_cnt = 0;
				  RGB_OFF_FLAG = 0;
				 
			  }
			 
		 }

        
		
      	
		 
		 
	 #else

          //if((get_curr_DAC_energy[1]-get_curr_DAC_energy[0]) >= 15)
		  //if((get_curr_DAC_energy[1]-get_curr_DAC_energy[0]) >= 10)
		  //if((get_curr_DAC_energy[1]-get_curr_DAC_energy[0]) >= 65)
		  if((get_curr_DAC_energy[energy_cnt-1] >= get_curr_DAC_energy[0]))
          {
               RGB_OFF_FLAG=0;
		  }
		  else
		  {
               RGB_OFF_FLAG=1;
		  }





	 #endif
	 

		// printf("@@@@@@ == %d\n",curr_MAX_DAC_energy - curr_DAC_energy2);
	   /* printf("@@@@@@RGB_rhythm_sw  == %d\n",RGB_rhythm_sw);
	    printf("RRRR   RGB_R_LV_ctr  == %d\n",RGB_R_LV_ctr);
		printf("GGGG   RGB_G_LV_ctr  == %d\n",RGB_G_LV_ctr);
		printf("BBBB   RGB_B_LV_ctr  == %d\n",RGB_B_LV_ctr);*/
		

	   
	}


	 


	 
}


/*
     DAC energy 处理
*/
void APP_DAC_energy_deal()
{
    static u8  TOP_clean_con=0;
    
    static u16  MAX_clean_con=0;
    static u16 DAC_grade=0;
    static u32 TOP_DAC_energy=0;
    static u32 MAX_DAC_energy=0;
	static u16 cnt=0;

   APP_Curr_DAC_energy = Get_Music_Energy();
    
    if(APP_Curr_DAC_energy>=TOP_DAC_energy)
    {
        if(APP_Curr_DAC_energy>0)
        {
            TOP_DAC_energy=APP_Curr_DAC_energy;
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
            DAC_grade = APP_Curr_DAC_energy/(MAX_DAC_energy/100);
           // APP_Curr_DAC_grade=DAC_grade;
            if(DAC_grade>65)APP_Curr_DAC_grade=DAC_grade+4;
           // if(DAC_grade==0)APP_Curr_DAC_grade=0;
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

    cnt++;
	if(cnt >= 25)
	{
        cnt=0;

		if(DAC_grade >= 100)
		 {
	         APP_LED_speed = 20;
		 }
		 else  if(DAC_grade > 90)
		 {
	         APP_LED_speed = 23;
		 }
	     else  if(DAC_grade > 80)
		 {
	         APP_LED_speed = 30;
		 }
		 else  if(DAC_grade > 70)
		 {
	         APP_LED_speed = 33;
		 }
	     else  if(DAC_grade > 60)
		 {
	         APP_LED_speed = 36;
		 }
		 else  if(DAC_grade > 50)
		 {
	         APP_LED_speed = 40;
		 }
	     else  if(DAC_grade > 40)
		 {
	         APP_LED_speed = 45;
		 }
		 else  if(DAC_grade > 30)
		 {
	         APP_LED_speed = 50;
		 }
		 else
		 {
	         APP_LED_speed = 60;
		 } 
		 
	}
    
	 

   // printf("DAC_grade == %d\n",DAC_grade);
	
    
    if(APP_Curr_DAC_grade>2)
    {
        APP_Curr_DAC_grade=APP_Curr_DAC_grade-2;
    }
    else APP_Curr_DAC_grade=0;

    
}
/*
    音乐律动 (闪动)
*/
void APP_RGB_effect_rhythm()
{
    u8 Rhythm_grade=0;
    static u16 RGB_Effect1_timer=0;
    static u8 Effect2_color=0;
    static u8 led=0;
	
    APP_DAC_energy_deal();
    Rhythm_grade = APP_Curr_DAC_grade;
    
    RGB_Effect1_timer++;
	//RGB_OFF_FLAG = 0;
    if(RGB_Effect1_timer>1000)//1000
    {
        RGB_Effect1_timer=0;
        Effect2_color++;
		//RGB_OFF_FLAG = 1;
        if(Effect2_color>7)
		{
		   Effect2_color=1;
		   
        }
    }
    switch(Effect2_color)
    {
        case 1:     
		            if( Rhythm_grade < 10 )
					{
					   mainAppCt.r_duty= 0;
			             
					}
					else
					{						
					  mainAppCt.r_duty= Rhythm_grade;
		             
					}
							            
                     mainAppCt.g_duty= 0;
		             mainAppCt.b_duty= 0;
                    break;
       case 2:     
		            if( Rhythm_grade < 10 )
					{
					  mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		            
					}
					else
					{
					  mainAppCt.r_duty= Rhythm_grade;
		              mainAppCt.g_duty= Rhythm_grade;
		             
					}
					
				    mainAppCt.b_duty= 0;
					
                    break;
        case 3:     
		            
		            if( Rhythm_grade < 10)
					{	
					 
		              mainAppCt.g_duty= 0;
		            }
					else
					{
					  
		              mainAppCt.g_duty= Rhythm_grade;
		             
					}					
					
		             mainAppCt.r_duty= 0;
					 mainAppCt.b_duty= 0;
                    break;
        case 4:     
		           
					if( Rhythm_grade < 10 )
					{
					  
		              mainAppCt.b_duty= 0;		             					
					}
					else
					{
					
		              mainAppCt.b_duty= Rhythm_grade;
					}
		             mainAppCt.r_duty= 0;
					 mainAppCt.g_duty= 0;
                    break;
        case 5:     
				   
					if( Rhythm_grade < 10 )
					{
					 
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
					 
		              mainAppCt.g_duty= Rhythm_grade;
		              mainAppCt.b_duty= Rhythm_grade;
					}
		            mainAppCt.r_duty= 0;
					
                    break;
        case 6:     
					if( Rhythm_grade < 10 )
					{
						 mainAppCt.r_duty= 0;
					}
					else
					{
				         mainAppCt.r_duty= Rhythm_grade;
					}
					
					if( Rhythm_grade < 10 )
					{
						 mainAppCt.b_duty= 0;
					}
					else
					{
					    mainAppCt.b_duty= Rhythm_grade;
					}

					
                    break;
        case 7:     
				    if( Rhythm_grade < 10 )
					{
					  mainAppCt.r_duty= 0;
		              mainAppCt.g_duty= 0;
		              mainAppCt.b_duty= 0;
					}
					else
					{
					  mainAppCt.r_duty= Rhythm_grade;
		              mainAppCt.g_duty= Rhythm_grade;
		              mainAppCt.b_duty= Rhythm_grade;
					}
		           /* RGB_R_Grade = Rhythm_grade;
                    RGB_G_Grade = Rhythm_grade;
                    RGB_B_Grade = Rhythm_grade;*/
                    break;
					
	
    }



		for(led = 0;led < RGB_LED_NUM;led++)
		{
	        RGB_R.buf[led] = mainAppCt.r_duty;
	        RGB_G.buf[led] = mainAppCt.g_duty;
	        RGB_B.buf[led] = mainAppCt.b_duty;
	    }
}







/*
   开机
   1：炫彩灯跑3次       
   2：从下到上再到下为1次
*/
void DMA_RGB_PowerOn_Show(void)
{
   static u8 i=0,led=0;
   static u8 colour=Rainbow_RED;
   static u8 cnt=0;

  static const colour_map[14][RGB_LED_NUM] = 
  {
     {Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_BLUE,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_BLUE,Rainbow_CYAN,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_BLUE,Rainbow_CYAN,Rainbow_purple},
	 {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_BLUE,Rainbow_CYAN,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_BLUE,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_GREEN,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_YELLOW,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_ORANGE,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},
     {Rainbow_RED,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close,Rainbow_Close},

  };


   
   if(IsTimeOut(&gRGBScanTime))
   {
	  TimeOutSet(&gRGBScanTime,150);

	   for(led=0;led<RGB_LED_NUM;led++)
	   {
		   RGB_Rainbow_Color(led, colour_map[i][led],1);
		   
	   }

	   i++;
	   if(i>=14)
	   {
	      i=0;
		  cnt++;
		  if(cnt >= 3)
		  {
		     cnt=0;
             mainAppCt.rgb_mode = RGB_Effect_Bt_Dis;
		  }
	   }
   }
 

}



/*
  蓝牙连接
*/
void DMA_RGB_Bt_Con_Show(void)
{
   static u8 i=0,led=0;
   static u8 colour=0;
   static u8 cnt=0;



	   for(led=0;led<RGB_LED_NUM;led++)
	   {
	      if(led==0)
	      {
		     colour=Rainbow_ORANGE;
	      }
		  else
		  {
             colour=Rainbow_Close;
		  }
		  RGB_Rainbow_Color(led, colour,1); 
	   }

 

}





/*
  蓝牙未连接
*/
void DMA_RGB_Bt_Dis_Show(void)
{
   static u8 i=0,led=0;
   static u8 colour=0;
   static u8 cnt=0;



	   for(led=0;led<RGB_LED_NUM;led++)
	   {
	      if(led==0)
	      {
		     colour=Rainbow_ORANGE;
	      }
		  else
		  {
             colour=Rainbow_Close;
		  }
		  RGB_Rainbow_Color(led, colour,1); 
	   }

 

}





/*
  蓝牙播放
*/
void DMA_RGB_Bt_Play_Show(void)
{
   static u8 i=0,led=0;
   static u8 ctrl=0;
   static u8 cnt=0;
  

    static u8 set_led_lv=0;	
	static u8 max_led_lv=0;	
	static u8 min_led_lv=0;	
	static bool dir=0; // 0: 往上    1: 往下
	static u16 set_led=0;
	static u8 led_i=0,led_j=0;		
	static u8 Color = RED;
	static u16 Color_Cnt = 0;
	static u16 TimeCnt = 500;
	static u32 Get_Energy=0;

	
   
  static const colour_map[8] = 
  {
      0x0,
	  0x1,	
	  0x2,
	  0x4,
	  0x8,
	  0x10,
	  0x20,
	  0x40,
	  
  };


	   if(Get_Global_playing_state())
		{
		    cnt++;
            //if(cnt >= 60)
            {
                cnt=0;
				max_led_lv = Get_DAC_Energy_LV_7();
			    Get_Energy = Get_Music_Energy();
			}
			
			
             TimeCnt++;

			 if(Get_Energy >= 1)
			 {
                if(TimeCnt > 40)
				 {
	                 TimeCnt=0;

	                     if(dir==0)
						 {
		                      set_led_lv++;
							  if(set_led_lv >= max_led_lv)
							  {
		                         dir=1;
								
								 if(max_led_lv >= 7)
								 {
	                                  min_led_lv = max_led_lv - 3;
								 }
								 else  if(max_led_lv >= 6)
								 {
	                                  min_led_lv = max_led_lv - 3;
								 }
								 else  if(max_led_lv >= 5)
								 {
	                                  min_led_lv = max_led_lv - 3;
								 }
								 else  if(max_led_lv >= 3)
								 {
	                                  min_led_lv = max_led_lv - 2;
								 }
								 else  if(max_led_lv >= 1)
								 {
	                                  min_led_lv = max_led_lv - 1;
								 }
								 else  
								 {
	                                  min_led_lv = 0;
								 }
								  
							  }

							  
						 }
						 else
						 {
						     if(set_led_lv > 0)
						     {
		                         set_led_lv--;
						     }
							 else if(set_led_lv == 0)
							 {
                                 dir=0;
							 }
							 
							  if(set_led_lv <= min_led_lv)
							  {
		                          set_led_lv = min_led_lv;
								  dir=0;
							  }
						 }
				 }
			 }
			 else
			 {
                set_led_lv = 0;
			 }

		
			    //需逐段增加(递减)
			    switch(set_led_lv)
			    {
                    case 0:
						   set_led &= ~(colour_map[0]);
						   set_led &= ~(colour_map[1]);
						   set_led &= ~(colour_map[2]);
						   set_led &= ~(colour_map[3]);
						   set_led &= ~(colour_map[4]);
						   set_led &= ~(colour_map[5]);
						   set_led &= ~(colour_map[6]);
						   set_led &= ~(colour_map[7]);
						break;
						
					case 1:
                           //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led &= ~(colour_map[2]);
						   set_led &= ~(colour_map[3]);
						   set_led &= ~(colour_map[4]);
						   set_led &= ~(colour_map[5]);
						   set_led &= ~(colour_map[6]);
						   set_led &= ~(colour_map[7]);
					
						break;
					case 2:
                            //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led |= colour_map[2];
						   set_led &= ~(colour_map[3]);
						   set_led &= ~(colour_map[4]);
						   set_led &= ~(colour_map[5]);
						   set_led &= ~(colour_map[6]);
						   set_led &= ~(colour_map[7]);
						break;
					case 3:
                             //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led |= colour_map[2];
						   set_led |= colour_map[3];
						   set_led &= ~(colour_map[4]);
						   set_led &= ~(colour_map[5]);
						   set_led &= ~(colour_map[6]);
						   set_led &= ~(colour_map[7]);
						break;
					case 4:
                            //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led |= colour_map[2];
						   set_led |= colour_map[3];
						   set_led |= colour_map[4];
						   set_led &= ~(colour_map[5]);
						   set_led &= ~(colour_map[6]);
						   set_led &= ~(colour_map[7]);
						break;
					case 5:
                            //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led |= colour_map[2];
						   set_led |= colour_map[3];
						   set_led |= colour_map[4];
						   set_led |= colour_map[5];
						   set_led &= ~(colour_map[6]);
						   set_led &= ~(colour_map[7]);
						break;
					case 6:
                          //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led |= colour_map[2];
						   set_led |= colour_map[3];
						   set_led |= colour_map[4];
						   set_led |= colour_map[5];
						   set_led |= colour_map[6];
						   set_led &= ~(colour_map[7]);
						break;
					case 7:
                          //set_led |= colour_map[0];
						   set_led |= colour_map[1];
						   set_led |= colour_map[2];
						   set_led |= colour_map[3];
						   set_led |= colour_map[4];
						   set_led |= colour_map[5];
						   set_led |= colour_map[6];
						   set_led |= colour_map[7];
						break;
				
				
				}
			
				 
					
		          for(led=0;led<RGB_LED_NUM;led++)//所有灯扫一遍
				  {
		              		   
					    if(set_led & 1 << led)
					    {
						     ctrl=1;
						}
						else
						{
						      ctrl=0;  
						}
						switch(led)
						{
                            case 0: Color = Rainbow_RED; break;
							case 1: Color = Rainbow_ORANGE; break;
							case 2: Color = Rainbow_YELLOW; break;
							case 3: Color = Rainbow_GREEN; break;
							case 4: Color = Rainbow_BLUE; break;
							case 5: Color = Rainbow_CYAN; break;
							case 6: Color = Rainbow_purple; break;
						}

						RGB_Rainbow_Color(led, Color, ctrl);
				  }
				 

	}
	else // 暂停
	{
	   set_led |= colour_map[1];
	   set_led &= ~(colour_map[2]);
	   set_led &= ~(colour_map[3]);
	   set_led &= ~(colour_map[4]);
	   set_led &= ~(colour_map[5]);
	   set_led &= ~(colour_map[6]);
	   set_led &= ~(colour_map[7]);
       set_led_lv = 0;
	   
               for(led=0;led<RGB_LED_NUM;led++)//所有灯扫一遍
				  {
		              		   
					    if(set_led & 1 << led)
					    {
						     ctrl=1;
						}
						else
						{
						      ctrl=0;  
						}
						switch(led)
						{
                            case 0: Color = Rainbow_RED; break;
							case 1: Color = Rainbow_ORANGE; break;
							case 2: Color = Rainbow_YELLOW; break;
							case 3: Color = Rainbow_GREEN; break;
							case 4: Color = Rainbow_BLUE; break;
							case 5: Color = Rainbow_CYAN; break;
							case 6: Color = Rainbow_purple; break;
						}

						RGB_Rainbow_Color(led, Color, ctrl);
				  }

	}
		
 
}


/*
  蓝牙暂停
*/
void DMA_RGB_Bt_Pause_Show(void)
{
   static u8 i=0,led=0;
   static u8 colour=0;
   static u8 cnt=0;



	   for(led=0;led<RGB_LED_NUM;led++)
	   {
	      if(led==0)
	      {
		     colour=Rainbow_ORANGE;
	      }
		  else
		  {
             colour=Rainbow_Close;
		  }
		  RGB_Rainbow_Color(led, colour,1); 
	   }

 

}

/*
  来电\接通状态
  通话保持跳动，跟随通话音

  挂断,恢复接通之前工作状态
*/
void DMA_RGB_HFP_CALL_IN_Show(void)
{

    DMA_RGB_Bt_Play_Show();

}



/*
  低电状态
*/
void DMA_RGB_LowPower_Show(void)
{



}

/*
  开机充电
*/
void DMA_RGB_PowerOn_Charge_Show(void)
{



}

/*
  关机充电
*/
void DMA_RGB_PowerOff_Charge_Show(void)
{



}

/*
  TWS
*/
void DMA_RGB_TWS_Show(void)
{



}






#ifdef CFG_MORE_GPIO_RGB_CTRL_EN
void ModeTest1(void)
{
	uint8_t i,j;
	mainAppCt.rgb_mode1 = 0;
	switch(mainAppCt.rgb_mode1)
	{
		case 0:					//只做所有灯呼吸灯渐变
			if(IsTimeOut(&gRGBScanTime1))
			{
				TimeOutSet(&gRGBScanTime1,10);
				if((mainAppCt.r_duty1 < 255)&&(mainAppCt.g_duty1 == 0)&&(mainAppCt.b_duty1 == 0))
				{
					mainAppCt.r_duty1++;
				}
				else if((mainAppCt.r_duty1 > 0)&&(mainAppCt.g_duty1 < 255)&&(mainAppCt.b_duty1 == 0))
				{
					mainAppCt.r_duty1--;
					mainAppCt.g_duty1++;
				}
				else if((mainAppCt.r_duty1 == 0)&&(mainAppCt.g_duty1 > 0)&&(mainAppCt.b_duty1 < 255))
				{
					mainAppCt.g_duty1--;
					mainAppCt.b_duty1++;
				}
				else if((mainAppCt.r_duty1 < 255)&&(mainAppCt.g_duty1 == 0)&&(mainAppCt.b_duty1 > 0))
				{
					mainAppCt.b_duty1--;
					mainAppCt.r_duty1++;
				}
				for(i=0;i<RGB_LED_NUM;i++)
				{
					RGB_R.buf1[i] = mainAppCt.r_duty1;
					RGB_G.buf1[i] = mainAppCt.g_duty1;
					RGB_B.buf1[i] = mainAppCt.b_duty1;
				}
			}
			break;
		case 1:
			if(IsTimeOut(&gRGBScanTime1))
			{
				TimeOutSet(&gRGBScanTime1,100);
				if(mainAppCt.rgb_duty_add1>0)
				{
					RGB_R.buf1[mainAppCt.rgb_duty_add1-1] = 0;
					RGB_G.buf1[mainAppCt.rgb_duty_add1-1] = 0;
					RGB_B.buf1[mainAppCt.rgb_duty_add1-1] = 0;
				}
				if((mainAppCt.r_duty1 < 255)&&(mainAppCt.g_duty1 == 0)&&(mainAppCt.b_duty1 == 0))
				{
					mainAppCt.r_duty1+=15;
				}
				else if((mainAppCt.r_duty1 > 0)&&(mainAppCt.g_duty1 < 255)&&(mainAppCt.b_duty1 == 0))
				{
					mainAppCt.r_duty1-=15;
					mainAppCt.g_duty1+=15;
				}
				else if((mainAppCt.r_duty1 == 0)&&(mainAppCt.g_duty1 > 0)&&(mainAppCt.b_duty1 < 255))
				{
					mainAppCt.g_duty1-=15;
					mainAppCt.b_duty1+=15;
				}
				else if((mainAppCt.r_duty1 < 255)&&(mainAppCt.g_duty1 == 0)&&(mainAppCt.b_duty1 > 0))
				{
					mainAppCt.b_duty1-=15;
					mainAppCt.r_duty1+=15;
				}
				if(mainAppCt.rgb_duty_add1 < 32)
				{
					mainAppCt.rgb_duty_add1++;
				}
				else
					mainAppCt.rgb_duty_add1 = 0;
				for(i=0;i<4;i++)
				{
					RGB_R.buf1[mainAppCt.rgb_duty_add1] = mainAppCt.r_duty1;
					RGB_G.buf1[mainAppCt.rgb_duty_add1] = mainAppCt.g_duty1;
					RGB_B.buf1[mainAppCt.rgb_duty_add1] = mainAppCt.b_duty1;
				}
			}
			break;
	}
}
#endif

#if 0
void rgb_mode_command(void)
{
	uint8_t i;

	uint8_t r_temp = mainAppCt.r_duty;
	uint8_t g_temp = mainAppCt.g_duty;
	uint8_t b_temp = mainAppCt.b_duty;
	uint8_t step_temp = mainAppCt.rgb_step;
	uint8_t j;
	memset(RGB_R.buf,0x00,sizeof(RGB_R.buf));
	memset(RGB_G.buf,0x00,sizeof(RGB_G.buf));
	memset(RGB_B.buf,0x00,sizeof(RGB_B.buf));
    mainAppCt.rgb_mode=1;
	//if(sys_cb.rgb_onoff)
	//osTaskDelay(1);
	{			
		switch(mainAppCt.rgb_mode)
		{
		case 0:		//两边向中间跑
			i = 0;
			if(mainAppCt.fft_level < 2) {	//无声音打底
				mainAppCt.fft_level = 2;
			}
			if(mainAppCt.pause_cnt >= FFT_PAUSE) {
				mainAppCt.fft_level = 10;
			}
            	mainAppCt.fft_level = 8;
            while(mainAppCt.fft_level > i) {
				for( j=0; j<4; j++) {
					RGB_R.buf[rgb_1_level_led[i][j]] = mainAppCt.r_duty;
					RGB_G.buf[rgb_1_level_led[i][j]] = mainAppCt.g_duty;
					RGB_B.buf[rgb_1_level_led[i][j]] = mainAppCt.b_duty;
				}
				i++;
				if(i > 2) {
					rgb_variety(5);		//处理颜色渐变
				}
			}
			break;
			
		case 1:		//两个在转圈		
			i = rgb_2_level_run[mainAppCt.fft_level];
			if(mainAppCt.rgb_run_keep > 0) {
				mainAppCt.rgb_run_keep--;
			}
			if((mainAppCt.rgb_mode2_run_time > i)||(mainAppCt.rgb_run_keep == 0)) {
				mainAppCt.rgb_mode2_run_time = i;
				mainAppCt.rgb_duty_add = rgb_2_level_add[mainAppCt.fft_level];
				if(mainAppCt.rgb_mode2_run_time <= 20) {
					mainAppCt.rgb_run_keep = 30; //30*5ms /(20~10) = 7.5~15个灯
				} else if(mainAppCt.rgb_mode2_run_time <= 40) {
					mainAppCt.rgb_run_keep = 24; //24*5ms /(40~20) = 3~6个灯
				}
			}
		
			for(j=0; j<RGB_RUN_MAX; j++) {
				if(mainAppCt.rgb_mode2_run >= j) {
					i = mainAppCt.rgb_mode2_run - j;
				} else {
					i = RGB_LED_MAX-j+mainAppCt.rgb_mode2_run;
				}
				if(j == 0) {
					if((mainAppCt.r_duty_add < mainAppCt.r_duty)&&((mainAppCt.r_duty - mainAppCt.r_duty_add)>mainAppCt.rgb_duty_add)) {
						mainAppCt.r_duty_add += mainAppCt.rgb_duty_add;
					} else {
						mainAppCt.r_duty_add = mainAppCt.r_duty;
					}
					if((mainAppCt.g_duty_add < mainAppCt.g_duty)&&((mainAppCt.g_duty - mainAppCt.g_duty_add)>mainAppCt.rgb_duty_add)) {
						mainAppCt.g_duty_add += mainAppCt.rgb_duty_add;
					} else {
						mainAppCt.g_duty_add = mainAppCt.g_duty;
					}
					if((mainAppCt.b_duty_add < mainAppCt.b_duty)&&((mainAppCt.b_duty - mainAppCt.b_duty_add)>mainAppCt.rgb_duty_add)) {
						mainAppCt.b_duty_add += mainAppCt.rgb_duty_add;
					} else {
						mainAppCt.b_duty_add = mainAppCt.b_duty;
					}
					RGB_R.buf[i] = mainAppCt.r_duty_add;
					RGB_G.buf[i] = mainAppCt.g_duty_add;
					RGB_B.buf[i] = mainAppCt.b_duty_add;
					RGB_R.buf[i+RGB_LED_MAX] = mainAppCt.r_duty_add;
					RGB_G.buf[i+RGB_LED_MAX] = mainAppCt.g_duty_add;
					RGB_B.buf[i+RGB_LED_MAX] = mainAppCt.b_duty_add;
				} else if (j == (RGB_RUN_MAX-1)){
					if(mainAppCt.r_duty > mainAppCt.r_duty_add) {
						RGB_R.buf[i] = mainAppCt.r_duty - mainAppCt.r_duty_add;
						RGB_R.buf[i+RGB_LED_MAX] = mainAppCt.r_duty - mainAppCt.r_duty_add;
					} else {
						RGB_R.buf[i] = 0;
						RGB_R.buf[i+RGB_LED_MAX] = 0;
					}
					if(mainAppCt.g_duty > mainAppCt.g_duty_add) {
						RGB_G.buf[i] = mainAppCt.g_duty - mainAppCt.g_duty_add;
						RGB_G.buf[i+RGB_LED_MAX] = mainAppCt.g_duty - mainAppCt.g_duty_add;
					} else {
						RGB_G.buf[i] = 0;
						RGB_G.buf[i+RGB_LED_MAX] = 0;
					}
					if(mainAppCt.b_duty > mainAppCt.b_duty_add) {
						RGB_B.buf[i] = mainAppCt.b_duty - mainAppCt.b_duty_add;
						RGB_B.buf[i+RGB_LED_MAX] = mainAppCt.b_duty - mainAppCt.b_duty_add;
					} else {
						RGB_B.buf[i] = 0;
						RGB_B.buf[i+RGB_LED_MAX] = 0;
					}					
				} else {
					RGB_R.buf[i] = mainAppCt.r_duty;
					RGB_G.buf[i] = mainAppCt.g_duty;
					RGB_B.buf[i] = mainAppCt.b_duty;
					RGB_R.buf[i+RGB_LED_MAX] = mainAppCt.r_duty;
					RGB_G.buf[i+RGB_LED_MAX] = mainAppCt.g_duty;
					RGB_B.buf[i+RGB_LED_MAX] = mainAppCt.b_duty;
				}
			}
			break;
			
		case 2:		//两个灯互闪			
			i = rgb_3_level_run[mainAppCt.fft_level];
			if(mainAppCt.rgb_run_keep > 0) {
				mainAppCt.rgb_run_keep--;
			}
			if((mainAppCt.rgb_mode3_run_time > i)||(mainAppCt.rgb_run_keep == 0)) {
				mainAppCt.rgb_mode3_run_time = i;
				if(mainAppCt.rgb_mode3_run_time <= 25) {
					mainAppCt.rgb_run_keep = 50; //50*5ms /((25~10)*5ms) = 2~5下闪烁
				} else if(mainAppCt.rgb_mode3_run_time <= 50) {
					mainAppCt.rgb_run_keep = 50; //50*5ms /((50~25)*5ms) = 1~2个灯
				}
			}

			uint8_t rgb_start = rgb_3_level_led[mainAppCt.rgb_random][0];
			uint8_t rgb_loop = rgb_3_level_led[mainAppCt.rgb_random][1];

			if(mainAppCt.pause_cnt >= FFT_PAUSE) {
				rgb_start = 0;
				rgb_loop = 24;
				mainAppCt.rgb_random_1 = 0;
			}
			
			for(j=0; j<rgb_loop; j++) {
				if(mainAppCt.rgb_random_1 < 5) {	//0~3, 4
					RGB_R.buf[rgb_start] = mainAppCt.r_duty;
					RGB_G.buf[rgb_start] = mainAppCt.g_duty;
					RGB_B.buf[rgb_start] = mainAppCt.b_duty;
				}
				if((mainAppCt.rgb_random_1 < 4)||(mainAppCt.rgb_random_1 == 5)) {	//0~3, 5
					RGB_R.buf[rgb_start+RGB_LED_MAX] = mainAppCt.r_duty;
					RGB_G.buf[rgb_start+RGB_LED_MAX] = mainAppCt.g_duty;
					RGB_B.buf[rgb_start+RGB_LED_MAX] = mainAppCt.b_duty;
				}

				rgb_start++;
				if(rgb_start >= RGB_LED_MAX) {
					rgb_start = 0;
				}
			}
			break;
       case 3:
                  mainAppCt.r_duty=255;
                 for(j=0;j<RGB_LED_NUM;j++){   
                // RGB_R.buf[j-1] =0;
                 RGB_G.buf[j] = mainAppCt.r_duty;
                
                } 
            break;
        }
	}

	rgb_data_deal();
	mainAppCt.rgb_data_ready_ok = 1;

	mainAppCt.r_duty = r_temp;
	mainAppCt.g_duty = g_temp;
	mainAppCt.b_duty = b_temp;
	mainAppCt.rgb_step = step_temp;
}
void rgb_data_deal(void)
{
	uint32_t g_data_32,r_data_32,b_data_32 = 0;
    uint8_t i;
    uint8_t temp_8;
  
    g_data_32 = 0;
	r_data_32 = 0;
	b_data_32 = 0;
	for( i = 0;i< RGB_LED_NUM;i++){
           temp_8= RGB_R.buf[i];

        r_data_32 = rgb_data_change(temp_8);
           temp_8= RGB_G.buf[i];
        g_data_32 = rgb_data_change(temp_8);
           temp_8= RGB_B.buf[i];
        b_data_32 = rgb_data_change(temp_8);
        rgb_buf[i*3] = g_data_32;
        rgb_buf[i*3+1] =  r_data_32;
        rgb_buf[i*3+2] = b_data_32;
       
//		rgb_buf[0+12*i] = (g_data_32 >> 24) & 0xff;
//		rgb_buf[1+12*i] = (g_data_32 >> 16) & 0xff;
//		rgb_buf[2+12*i] = (g_data_32 >> 8) & 0xff;
//		rgb_buf[3+12*i] =  g_data_32 & 0xff;
//
//		rgb_buf[4+12*i] = (r_data_32 >> 24) & 0xff;
//		rgb_buf[5+12*i] = (r_data_32 >> 16) & 0xff;
//		rgb_buf[6+12*i] = (r_data_32 >> 8) & 0xff;
//		rgb_buf[7+12*i] =  r_data_32 & 0xff;
//
//		rgb_buf[8+12*i] = (b_data_32 >> 24) & 0xff;
//		rgb_buf[9+12*i] = (b_data_32 >> 16) & 0xff;
//		rgb_buf[10+12*i] =(b_data_32 >> 8) & 0xff;
//		rgb_buf[11+12*i] = b_data_32 & 0xff;
	}
}
uint32_t rgb_data_change(uint8_t data)	//24 = 3*8
{
	uint32_t data_u32 = 0;
    uint8_t i;
	for(i = 0;i<8;i++){
		// 1 --> 1110   0--> 1000
		if(data & BIT(7)){
			data_u32 = data_u32 | (0x0e);
		}else{
			data_u32 = data_u32 | (0x08);
		}
		data <<= 1;
		if(i < 7){
			data_u32 <<= 4;
		}
	}
	return data_u32;
}
void rgb_variety(uint8_t d)
{
	switch(mainAppCt.rgb_step)
	{
	case 0: 
		mainAppCt.r_duty += d;
		if(mainAppCt.r_duty > RGB_MAX) {
			mainAppCt.r_duty = RGB_MAX;
			mainAppCt.rgb_step++;
		}
		if(mainAppCt.g_duty > d) {
			mainAppCt.g_duty -= d;
		} else {
			mainAppCt.g_duty = 0;
		}
		break;
	case 1: 
		mainAppCt.b_duty += d;
		if(mainAppCt.b_duty > RGB_MAX) {
			mainAppCt.b_duty = RGB_MAX;
			mainAppCt.rgb_step++;
		}
		if(mainAppCt.r_duty > d) {
			mainAppCt.r_duty -= d;
		} else {
			mainAppCt.r_duty = 0;
		}
		break;
	case 2: 
		mainAppCt.g_duty += d;
		if(mainAppCt.g_duty > RGB_MAX) {
			mainAppCt.g_duty = RGB_MAX;
			mainAppCt.rgb_step = 0;
		}
		if(mainAppCt.b_duty > d) {
			mainAppCt.b_duty -= d;
		} else {
			mainAppCt.b_duty = 0;
		}
		break;
	}
}
void rgb_mode2_run_timer(uint8_t d)
{
	static uint8_t run_cnt = 0;

	run_cnt++;
	if(run_cnt >= mainAppCt.rgb_mode2_run_time) {
		run_cnt = 0;
		mainAppCt.rgb_mode2_run += d;
		if(mainAppCt.rgb_mode2_run >= RGB_LED_MAX) {
			mainAppCt.rgb_mode2_run = 0;
		}
		mainAppCt.r_duty_add = 0;
		mainAppCt.g_duty_add = 0;
		mainAppCt.b_duty_add = 0;
	}
}

AT(.com_text.rgb)
void rgb_mode3_run_timer(void)
{
	static uint8_t run_cnt = 0;

	run_cnt++;
	if(run_cnt >= mainAppCt.rgb_mode3_run_time) {
		run_cnt = 0;
		mainAppCt.rgb_random = mainAppCt.sys_random;
		mainAppCt.rgb_random_1 = mainAppCt.sys_random_1;
	}
}


void energy_led_level_calc(void)    //约5ms调用一次.
{
    uint8_t /*level,*/i;
    uint32_t energy;
    static uint8_t disp_level,last_level;
    static uint8_t time_5ms_cnt = 0;
	static uint8_t level_tab_min = 0;
	static uint8_t level_tab_max = 0;
#define LEVEL_TAB_MIN 	150	// 3000/20 = 150
#define LEVEL_TAB_MAX	150	// 3000/20 = 150

    time_5ms_cnt++;
    if(time_5ms_cnt >= 4){   //20ms计算一次能量
        time_5ms_cnt  = 0;

        energy =  mainAppCt.dac_pow;
      
        {
            energy = energy/150;
        }
//		printf("energy=%2x\n",energy);

		if(energy < 80) {
			if(level_tab_min < LEVEL_TAB_MIN) {
				level_tab_min++;
			}
		} else {
			level_tab_min = 0;
		}
		
		if(energy > 90) {
			if(level_tab_max < LEVEL_TAB_MAX) {
				level_tab_max++;
			}
		} else {
			level_tab_max = 0;
		}
		
        //非线性量化表
//        for (i = 0, last_level = 0; i < 4*8; i++) {
		for (i = 0, last_level = 0; i < ENERGY_LEVEL; i++) {
			if(level_tab_max >= LEVEL_TAB_MAX) {
				if (energy < energy_qtz_tbl_high[i]) {
					break;
				}
			} else if(level_tab_min >= LEVEL_TAB_MIN) {
				if (energy < energy_qtz_tbl_low[i]) {
					break;
				}
			} else{
				if (energy < energy_qtz_tbl[i]) {
					break;
				}
			}		   
           	last_level++;
        }

		if(last_level >= ENERGY_LEVEL) {
			last_level = ENERGY_LEVEL-1;
		}
		if(mainAppCt.dac_pow==0) {		//音量为0表示无声
			last_level = 0;
		}

		if(last_level == 0) {
			if(mainAppCt.pause_cnt < FFT_PAUSE) {
				mainAppCt.pause_cnt++;
			}
		} else {
			mainAppCt.pause_cnt = 0;
		}
    }	
    //能量相同, 不用更新
    if(disp_level == last_level){
        return;
    }

    //能量减少时,慢慢下降
    if (disp_level > last_level) {
        disp_level--;
    } else {
        disp_level = last_level;
    }

	mainAppCt.fft_level = disp_level;

    //能量转为占空比,存放到pwm_duty_buf中.
/*    level = disp_level;
    for (i=0; i<ENERGY_LED_NUM; i++){
        if (level >= ENERGY_PWM_MAX_DUTY) {
            pwm_duty_buf[i] = 0;
            level -= ENERGY_PWM_MAX_DUTY;
        } else {
            pwm_duty_buf[i] = (1<<level) - 1;
            pwm_duty_buf[i] = ~pwm_duty_buf[i];
            level = 0;
        }
    }*/
}
#endif
#endif //CFG_DMA_RGB_LED_EN
// end of file
