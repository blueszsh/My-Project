/*--------------------------------------------------------------------------*/
/**@file     iic_io.c
   @brief    IO妯℃嫙鐨処IC鐨勯┍鍔?   @details
   @author  zhiying
   @date   2013-3-26
   @note
*/
/*----------------------------------------------------------------------------*/
#include <string.h>
#include "type.h"
#include "gpio.h"
#include "iic.h"
#include "debug.h"
#include "delay.h"

#define MAX_I2C_MASTER   2

I2C_HANDLE_8027 FM8027I2cHandler = NULL;

/*----------------------------------------------------------------------------*/
/*

*/
/*----------------------------------------------------------------------------*/
void iic_init_io(void)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
		IIC_DAT_IO_PU1;
    iic_data_h();
    iic_data_out();         //SDA
    iic_clk_h();
    iic_clk_out();         	//SCL
 }
/*----------------------------------------------------------------------------*/
/**
 *
*/
/*----------------------------------------------------------------------------*/
void iic_start(void)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
    iic_init_io();
    iic_delay();
    iic_data_l();
    iic_delay();
    iic_clk_l();
    iic_delay();
    //iic_data_h();
}
/*----------------------------------------------------------------------------*/
/**
 *
*/
/*----------------------------------------------------------------------------*/
void iic_stop(void)
{
    iic_data_out();
    iic_data_l();
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_data_h();
    iic_delay();
}
/*----------------------------------------------------------------------------*/
/*
 *
 */
/*----------------------------------------------------------------------------*/
tbool r_ack(void)
{
    tbool tnack;
    iic_data_in();
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    tnack = iic_data_r();
    iic_clk_l();
    iic_delay();
    return tnack;
}
/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void s_ack(u8 flag)
{
    iic_data_out();
    iic_clk_l();
    if (flag) {
        iic_data_h();
    } else {
        iic_data_l();
    }
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_clk_l();
}
/*----------------------------------------------------------------------------*/
/**
*/
/*----------------------------------------------------------------------------*/
tu8 iic_revbyte_io(void)
{
    u8 tbyteI2C = 0X00;
    u8 i;
    iic_data_in();
    iic_delay();
    for (i = 0; i < 8; i++) {
        iic_clk_h();
        tbyteI2C <<= 1;
        iic_delay();
        if (iic_data_r()) {
            tbyteI2C++;
        }
        iic_clk_l();
        iic_delay();
    }
    return tbyteI2C;
}
/*----------------------------------------------------------------------------*/
/**
*/
/*----------------------------------------------------------------------------*/
tu8 iic_revbyte(u8 para)
{
    u8 tbyte;
    tbyte = iic_revbyte_io();
    s_ack(para);
    //delay(30000);
	iic_delay();
    return tbyte;
}
/*----------------------------------------------------------------------------*/
/**
*/
/*----------------------------------------------------------------------------*/
void iic_sendbyte_io(u8 byte)
{
    u8 i;
    iic_data_out();
    iic_delay();
    //log_printf("byte: %02x\n", byte);
    for (i = 0; i < 8; i++) {
        if (byte & BIT(7)) {
            iic_data_h();    //
        } else {
            iic_data_l();
        }
        iic_delay();
        iic_clk_h();
        iic_delay();
        byte <<= 1;
        iic_clk_l();
        iic_delay();
    }
}

/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void iic_sendbyte(u8 byte)
{
     u8 ack;
  
    iic_sendbyte_io(byte);
    ack = r_ack();
    //delay(30000);
	iic_delay();

	//printf("iic_sendbyte()      ack == %d\n",ack);
}





///------------------------------------------------------------------------------
///------------------------------------IIC_API-----------------------------------
///------------------------------------------------------------------------------


bool g_iic_busy = 0; ///<iic绻佸繖鏍囪
/*----------------------------------------------------------------------------*/
/**
 *
*/
/*----------------------------------------------------------------------------*/
void iic_init(void)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
	
    // iic_data_out();
    iic_data_h();
    iic_data_out();
    //iic_clk_out();
    iic_clk_h();    ///<
    iic_clk_out();
}

/*----------------------------------------------------------------------------*/
/** @brief: eeprom 鏍稿疄
    @param: void
    @return:void
    @author:Juntham
    @note:  void eeprom_verify(void)
*/
/*----------------------------------------------------------------------------*/
void eeprom_verify(void)
{
    puts("eeprom_verify 1\n");
	/*if( FM8027I2cHandler == NULL)
		return;*/

    if ((read_eerom(0) != 0x55)
        || (read_eerom(1) != 0xAA)) {
        write_eerom(0, 0x55);
        write_eerom(1, 0xAA);
    }

    puts("eeprom_verify 2\n");
    if ((read_eerom(0) != 0x55)
        || (read_eerom(1) != 0xAA)) {
        puts("iic-no\n");//
    } else {
        puts("iic-yes\n"); //
    }
}

void iic_write_one_byte(u8 iic_dat)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
  
    g_iic_busy  = 1;
    iic_start();                //
    iic_sendbyte(iic_dat);      //
    iic_stop();                 //I2
}
/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void iic_write(u8 chip_id, u8 iic_addr, u8 *iic_dat, u8 n)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
	
    g_iic_busy  = 1;
    iic_start();                //
    iic_sendbyte(chip_id);         //
    if (0xff != iic_addr) {
        iic_sendbyte(iic_addr);   //
     }
    for (; n > 0; n--) {
        iic_sendbyte(*iic_dat++);      //
     }
    iic_stop();                 //I2C
    g_iic_busy = 0;
}
/*----------------------------------------------------------------------------*/
/*
 *
 */
/*----------------------------------------------------------------------------*/
void iic_readn(u8 chip_id, u8 iic_addr, u8 *iic_dat, u8 n)
{
  /* if( FM8027I2cHandler == NULL)
		return;*/

    g_iic_busy = 1;
    iic_start();
    iic_sendbyte(chip_id);
    if (0xff != iic_addr) {
        iic_sendbyte(iic_addr);
     }
    for (; n > 1; n--) {
        *iic_dat++ = iic_revbyte(0);
     }
    *iic_dat++ = iic_revbyte(1);
    iic_stop();
    g_iic_busy = 0;
}

/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
tu8 read_info(u8 addr)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/

    return read_eerom(addr);
}

/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void write_info(u8 addr, u8 dat)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
	
    write_eerom(addr, dat);
}

/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void  eeprom_page_write_start(void)
{
    g_iic_busy  = 1;
    iic_start();                //I2C
    iic_sendbyte(0xa0);         //
}
/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void eeprom_page_write_stop(void)
{
  /* if( FM8027I2cHandler == NULL)
		return;*/

	iic_stop();                     //I2C
    g_iic_busy = 0;
}
/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
tu8 read_eerom(u8 iic_addr)
{
    u8  tbyte;

	/*if( FM8027I2cHandler == NULL)
		return;*/
	
    g_iic_busy = 1;
    iic_start();
    iic_sendbyte(0xa0);
    iic_sendbyte(iic_addr);
    iic_start();
    iic_sendbyte(0xa1);
    tbyte = iic_revbyte(1);
    iic_stop();
    g_iic_busy = 0;
    return  tbyte;
}

/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void write_eerom(u8 addr, u8 dat)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/
	
    iic_write(0xa0, addr, &dat, 1);
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
}

/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
void set_memory(u8 addr, u8 dat)
{
/*if( FM8027I2cHandler == NULL)
		return;*/

    write_eerom(addr, dat);
}
/*----------------------------------------------------------------------------*/
/**
 *
 */
/*----------------------------------------------------------------------------*/
u8 get_memory(u8 addr)
{
	/*if( FM8027I2cHandler == NULL)
		return;*/

    return read_eerom(addr);
}
