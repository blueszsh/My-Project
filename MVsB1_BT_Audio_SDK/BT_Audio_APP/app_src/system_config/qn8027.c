/******************************************************************************
Des:
    from other Function.
Fun:

Date:

Author:

Modify:

*******************************************************************************/
#include<string.h>
#include "type.h"
#include "app_config.h"
#include "delay.h"
#include "gpio.h"
#include "qn8027.h"
//#include "QN8035.h"
#include "iic.h"
#include "i2c_host.h"
#include "debug.h"
#include "rtos_api.h" //for SDIOMutex


#if 1

#define NORMAL_I2C_READ 1
#define DEBUG_QN8027_INFO
#define delay_n10ms(a)  WaitMs((a*10))
#define SFR(sfr, start, len, dat) (sfr = (sfr & ~((~(0xffffffff << (len))) << (start))) | (((dat) & (~(0xffffffff << (len)))) << (start)))
#define    FUNC_RADIO_QN8027_EN                  1


#ifdef FUNC_RADIO_QN8027_EN

/*----------------------------------------------------------------------------*/
/**
void qn8027_write(u8 iic_addr,u8 iic_data)
*/
/*----------------------------------------------------------------------------*/
void qn8027_write(u8 reg_addr, u8 reg_data)
{
	uint8_t     tryCount = 8;
	
	/*if( FM8027I2cHandler == NULL )
	{
      #if defined(DEBUG_QN8027_INFO) && (Z__DEBUG_custom==1)
	  printf("\n %s[%d] FM8027I2cHandler empty",__FUNCTION__,__LINE__);
	  #endif
	  return;
	}*/
	iic_write(QN8027_ADDR_WRITE, reg_addr, &reg_data, 1);
}

/*----------------------------------------------------------------------------*/
/**@brief
 *
 */
/*----------------------------------------------------------------------------*/
u8 qn8027_read(u8 reg_addr)
{
	//uint8_t	TempData = 0;
	uint8_t  tryCount = 8;
	uint8_t  Byte_RecivData;
    
	/*if( FM8027I2cHandler == NULL )
	{
		
	  #if defined(DEBUG_QN8027_INFO) && (Z__DEBUG_custom==1)
	  printf("\n %s[%d] FM8027I2cHandler empty",__FUNCTION__,__LINE__);
	  #endif
	  return 0;
	}*/
 
    iic_start();
    iic_sendbyte(QN8027_ADDR_WRITE);
	iic_sendbyte(reg_addr);
	iic_start();
    iic_sendbyte(QN8027_ADDR_READ);
	Byte_RecivData = iic_revbyte(1);
    iic_stop();
    return Byte_RecivData;
}

void qn8027_write_Bit(u8 reg, u8 start, u8 len, u8 data)
{
    u8 temp;

   /* if( FM8027I2cHandler == NULL )
	  return;*/

    temp = qn8027_read(reg);	
    SFR(temp, start, len, data);	
    qn8027_write(reg, temp);
}


/*----------------------------------------------------------------------------*/
/**@brief
 *
 */
/*----------------------------------------------------------------------------*/
#if 1
void qn8027_init(u16 power, u16 fre)
{
    u8 read_data;

	#ifdef DEBUG_QN8027_INFO
    printf("qn8027_init\n");
    printf("qn8027_id:%x:%x\n", qn8027_read(QN8027_CID1), qn8027_read(QN8027_CID2));

	
    //printf("qn8027_id:  %x:\n", qn8027_read(QN8027_CID1));
   
	//printf("qn8027_id:  %x:\n", qn8027_read(QN8027_CID2));
    #endif
	
#if  NORMAL_I2C_READ    
	qn8027_write(QN8027_SYSTEM, 0x81); 		//reset all reg
    delay_n10ms(2);

#if (FMTX_CHIP_CLK_SOURCE == CLK_BY_EXTERN_OSC)
    qn8027_write(QN8027_REG_XTL, 0x3f);		
#else
    qn8027_write(QN8027_REG_XTL, 0xff);		
#endif

#if (FMTX_CHIP_OSC_SELECT == _OSC_24M)
    qn8027_write(QN8027_REG_VGA, 0x91);     //24M

#else
    qn8027_write(QN8027_REG_VGA, 0x21);		//12M
#endif
    qn8027_write(QN8027_SYSTEM, 0x41);		//reset FSM
    qn8027_write(QN8027_SYSTEM, 0x01);
    delay_n10ms(2);

    qn8027_write(0x18, 0xe4);           //SNR improve xx1x x1xx
    qn8027_write(0x1b, 0xf0);			//    
	qn8027_write(QN8027_CH1, 0x7e);		//
#if PA_OFF_WHEN_NO_AUDIO
    qn8027_write(QN8027_GPLT, 0xA9);    //
#else
    qn8027_write(QN8027_GPLT, 0xB9);    //
#endif
    qn8027_write(QN8027_FDEV, 64);

    qn8027_write(QN8027_SYSTEM, 0x22);			//
    
	//delay_n10ms(10);
    //qn8027_set_freq(950);

    delay_n10ms(10);
    qn8027_set_power_init(72, QN8027_pre1);
    //delay_n10ms(10);		
	//qn8027_transmit_start();	

	
#endif

/*
    qn8027_write(0x00,0x81);
    //os_time_dly(3);
    delay_n10ms(2);
    //QN8027_write_reg(0x03,0x50);
    //QN8027_write_reg(0x04,0x21);
    qn8027_write(0x00,0x41);
    qn8027_write(0x00,0x01);
    //os_time_dly(3);
    delay_n10ms(2);
    qn8027_write(0x18,0xE4);
    //QN8027_write_reg(0x1B,0xF0);
    qn8027_write(0x1B,0x00F);
    
    //QN8027_write_reg(0x01,((8750-7600)/5));//e6
    qn8027_write(0x02,0xB9);
    qn8027_write(0x00,0x00);
    */
}
#else
void qn8027_init(u16 power, u16 fre)
{
    u8 read_data;

	
    printf("qn8027_init\n");
    printf("qn8027_id:%x:%x\n", qn8027_read(QN8027_CID1), qn8027_read(QN8027_CID2));



  // 按照文档说明----  正确的 QN8027 初始化程序如下：

      qn8027_write(0x00,0x81);
      delay_n10ms(2);
      qn8027_write(0x03,0x50);
      qn8027_write(0x04,0x91);
      qn8027_write(0x00,0x41);
	  qn8027_write(0x00,0x01);
	  qn8027_write(0x03,0x50);
	  delay_n10ms(2);
	  qn8027_write(0x18,0xe4);
	  qn8027_write(0x1b,0xf0);

	  qn8027_write(0x01,0x7e);
	  qn8027_write(0x02,0xb9);
	  qn8027_write(0x00,0x22);


}

 #endif

/*----------------------------------------------------------------------------*/
/**
@brief    QN8027   
@param    fre
@return     
@note     
void qn8027_set_freq(u16 fre)
*/
/*----------------------------------------------------------------------------*/
void qn8027_set_freq(u16 fre)
{
#if NORMAL_I2C_READ		
    u8  ch_temp0;
    u16 ch_temp1;
    ch_temp1 = (fre - 760) * 2;
    ch_temp0 = (ch_temp1 >> 8) & 0x3;
    ch_temp0 = (0x20 | ch_temp0);
    qn8027_write(QN8027_CH1, ch_temp1 & 0xff);
    qn8027_write(QN8027_SYSTEM, ch_temp0);

	printf("qn8027_set_freq(u16 fre)  ==  %x \n ",qn8027_read(QN8027_SYSTEM));
#endif
}

void qn8027_set_power(u8 power, u16 freq)
{
    if (power > QN8027_TX_POWER_MAX || power < QN8027_TX_POWER_MIN) {
        power = QN8027_TX_POWER_MAX;
    }
    qn8027_write(QN8027_PAC, power | BIT(7));
    delay_n10ms(1);
    qn8027_write(QN8027_SYSTEM, 0x01);	//enter IDLE mode  
    delay_n10ms(1);	 
    qn8027_write(QN8027_SYSTEM, 0x22);	//enter TX mode
    delay_n10ms(1);
    qn8027_set_freq(freq);

	printf("-----------  freq == %d  \n",freq);
}

void qn8027_transmit_start(void)
{
#if NORMAL_I2C_READ    
	qn8027_write_Bit(QN8027_SYSTEM, 5, 1, 1);
#endif
}

void qn8027_transmit_stop(void)
{
#if NORMAL_I2C_READ	
    qn8027_write_Bit(QN8027_SYSTEM, 5, 1, 0);
#endif
}

/*
*  //初始化时调用;  //   BOEU
*
*/
void qn8027_set_freq_init(u16 fre)
{
#if NORMAL_I2C_READ		
    u8  ch_temp0;
    u16 ch_temp1;
    ch_temp1 = (fre - 760) * 2;
    ch_temp0 = (ch_temp1 >> 8) & 0x3;
    ch_temp0 = (0x20 | ch_temp0);
   // qn8027_write(QN8027_CH1, ch_temp1 & 0xff);
	qn8027_write(QN8027_CH1, 0xE6);
    qn8027_write(QN8027_SYSTEM, /*0x20*/ch_temp0);

	printf("qn8027_set_freq_init(u16 fre)  ==  %x \n ",qn8027_read(QN8027_SYSTEM));
#endif
}
void qn8027_set_power_init(u8 power, u16 freq)
{
    if (power > QN8027_TX_POWER_MAX || power < QN8027_TX_POWER_MIN) {
        power = QN8027_TX_POWER_MAX;
    }
    qn8027_write(QN8027_PAC, power | BIT(7));
    delay_n10ms(1);
    qn8027_write(QN8027_SYSTEM, 0x01);	//enter IDLE mode  
    delay_n10ms(1);	 
    qn8027_write(QN8027_SYSTEM, 0x22);	//enter TX mode
    delay_n10ms(1);
    qn8027_set_freq_init(freq);

	printf("-----------  freq init == %d  \n",freq);
}




void qn8027_mute(u8 mute)
{
#if NORMAL_I2C_READ	

    if (mute) {
        qn8027_write_Bit(QN8027_SYSTEM, 3, 1, 1);

    } else {
        qn8027_write_Bit(QN8027_SYSTEM, 3, 1, 0);
    }
	delay_n10ms(2);
#endif
}

//-----------------------------------------------------------------------------
//--2020.10.08
bool qn8027_readid(void)
{
	u8  cid1 = 0,cid2 = 0; 
	
	cid1 = qn8027_read(QN8027_CID1); 
	cid2 =  qn8027_read(QN8027_CID2);
	if( cid1 || cid2 )
	{
		#if defined(DEBUG_QN8027_INFO) && (Z__DEBUG_custom==1)
		printf("\n %s[%d] 8027 ID %x--%x", __FUNCTION__,__LINE__,cid1,cid2);
		#endif
		//QN8035Init();
		return TRUE;
	}
	else
	{
		#if defined(DEBUG_QN8027_INFO) && (Z__DEBUG_custom==1)
		printf("\n %s[%d] QN8035 ID ERR ", __FUNCTION__,__LINE__);
		#endif
		return FALSE;
	}
//	QN8035Mute(TRUE);
    return FALSE;
}

bool qn8027_poweron(void)
{
	
#ifdef BP1064A2_DEMO
	CFG_RADIO_IIC_ENABLE();
#endif
	if(!FM8027I2cHandler)
	{	
		FM8027I2cHandler = (I2C_HANDLE_8027 )CFG_RADIO_IIC_HANDLER();
	}
	if(FM8027I2cHandler)
	{
		
	}
	else
	{
		#if defined(DEBUG_QN8027_INFO) && (Z__DEBUG_custom==1)
		printf("\n %s[%d] I2cMasterCreate Fail!\n",__FUNCTION__,__LINE__);
		#endif
		return FALSE;
	}
	return TRUE;
}

bool qn8027_powerdown(void)
{
	//QN8035Mute(TRUE);
//	prevMode = QND_ReadReg(SYSTEM1);
//	QNF_SetRegBit(SYSTEM1, R_TXRX_MASK, STNBY);
#if  NORMAL_I2C_READ	
    qn8027_mute(1);
	qn8027_transmit_stop();
	return TRUE;
#endif	
}






void fmtx_start(void)
{
	
    printf("FM TX___start\n");

	qn8027_set_power(QN8027_TX_POWER_MAX,QN8027_pre1);

	qn8027_transmit_start();
	 
    qn8027_mute(0);
   
	
	
	
}


void fmtx_stop(void)
{
  printf("FM TX___stop\n");	   
   qn8027_mute(1);
   
   qn8027_transmit_stop();
   
   
  
}
#endif //QN8027
#endif