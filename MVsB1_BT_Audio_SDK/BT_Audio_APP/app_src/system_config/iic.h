#ifndef	_IIC_H_
#define _IIC_H_

#include "zx_top1.h"
#include "zx_top2.h"


#define u8 uint8_t
#define tu8 uint8_t
#define tbool bool

void iic_write(u8 chip_id, u8 iic_addr, u8 *iic_dat, u8 n);
void iic_readn(u8 chip_id, u8 iic_addr, u8 *iic_dat, u8 n);
void iic_init(void);
void iic_init_io(void);
void write_info(u8 addr, u8 dat);
tu8 read_info(u8 addr);

void write_eerom(u8 addr, u8 dat);
tu8 read_eerom(u8 iic_addr);
void eeprom_page_write_stop(void);
void eeprom_page_write_start(void);
void eeprom_verify(void);

void iic_start(void);
void iic_stop(void);
void iic_sendbyte_io(u8 byte);
void iic_sendbyte(u8 byte);
tu8 iic_revbyte(u8 para);

extern bool g_iic_busy;

//#define iic_delay()      delay(3000)

#define iic_delay()        __udelay(5)
//#define IIC_PORT			JL_PORTA
#define IIC_DAT				6
#define IIC_CLK				5


typedef struct
{
	uint32_t  SdaMask;
	uint32_t  SclMask;
	uint8_t   SdaPortIndex;
	uint8_t   SclPortIndex;

	bool   IsUsed;
} I2C_INFO_8027, *I2C_HANDLE_8027;



extern I2C_HANDLE_8027 FM8027I2cHandler;

#pragma  message " marco declared same with I2c_host.c avoid to predfined"

#ifndef IIC_GPIO_IN
#define		IIC_GPIO_IN		(0x00 + 0)	/**<GPIOA input data register macro*/
#endif

#ifndef IIC_GPIO_OUT
#define		IIC_GPIO_OUT	(0x01 + 0)	/**<GPIOA output data register macro*/
#endif

#ifndef IIC_GPIO_SET
#define     IIC_GPIO_SET	(0x02 + 0)
#endif

#ifndef  IIC_GPIO_CLR
#define     IIC_GPIO_CLR	(0x03 + 0)
#endif

#ifndef IIC_GPIO_TGL
#define     IIC_GPIO_TGL	(0x04 + 0)
#endif

#ifndef IIC_GPIO_IE
#define		IIC_GPIO_IE		(0x05 + 0)	/**<GPIOA input enable register macro.0: disable, 1: enable*/
#endif

#ifndef IIC_GPIO_OE
#define		IIC_GPIO_OE		(0x06 + 0)	/**<GPIOA output enable register macro.0: disable, 1: enable*/
#endif

#ifndef IIC_GPIO_DS
#define		IIC_GPIO_DS		(0x07 + 0)	/**<GPIOA pull-up/pull-down drive strength(when GPIOA input or output) register macro. 0: weak pull(20uA), 1: strong pull(70uA)*/
#endif

#ifndef      IIC_GPIO_PU
#define		IIC_GPIO_PU		(0x08 + 0)	/**<GPIOA pull up register macro*/
#endif
#ifndef     IIC_GPIO_PD
#define		IIC_GPIO_PD		(0x09 + 0)	/**<GPIOA pull down register macro.{pu,pd} 1'b1: pull-up, 1'b1: pull-down, 1'b0: no pull-up, no pull-down, */
#endif

#define iic_clk_out()    IIC_CLK_IO_O
#define iic_clk_h()     IIC_CLK_IO_OH
#define iic_clk_l()     IIC_CLK_IO_OL
#define iic_data_out()   IIC_DAT_IO_OUT	
#define iic_data_in()    IIC_DAT_IO_IN



#define iic_data_r()    IIC_DAT_IO_R
#define iic_data_h()    IIC_DAT_IO_OUT1
#define iic_data_l()    IIC_DAT_IO_OUT0

/*
#define iic_data_out()   do{IIC_PORT->DIR &= ~IIC_DAT;IIC_PORT->PU |= IIC_DAT;}while(0)
#define iic_data_in()    do{IIC_PORT->DIR |=  IIC_DAT;IIC_PORT->PU |= IIC_DAT;}while(0)
#define iic_data_r()     (IIC_PORT->IN & IIC_DAT)
#define iic_data_h()     do{IIC_PORT->OUT |= IIC_DAT;}while(0)
#define iic_data_l()     do{IIC_PORT->OUT &=~IIC_DAT;}while(0)

#define iic_clk_out()    do{IIC_PORT->DIR &=~IIC_CLK;IIC_PORT->PU |= IIC_CLK;}while(0)
#define iic_clk_h()      do{IIC_PORT->OUT |= IIC_CLK;}while(0)
#define iic_clk_l()      do{IIC_PORT->OUT &=~IIC_CLK;}while(0)
*/


#define app_IIC_write(a, b, c, d) \
  iic_write(a, b, c, d)
#define app_IIC_readn(a, b, c, d)  \
  iic_readn(a, b, c, d)
#define app_E2PROM_write(a, b)  \
  write_eerom(a, b)
#define app_E2PROM_read(a)   \
  read_eerom(a)


#endif






