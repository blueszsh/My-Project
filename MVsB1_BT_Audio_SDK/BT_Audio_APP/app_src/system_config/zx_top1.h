#ifndef __zx_top1__
#define __zx_top1__

#include "type.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
//#define BOOL         uint8_t

extern u16 Save_key_number;
extern u16 Save_key_number2;
extern u16 Save_KeyMsg_ype;
extern u8 inquire_key_i;
extern u8 inquire_key_j;

extern u16 RecDataUserKeyValue;
extern u8 RecDataIRKeyValue;//按键码值
extern u8 RecDataUserKeyValue1;//用户码值
extern u8 RecDataUserKeyValue2;
extern u8 RecDataUserKeyValue3;

extern const uint8_t Remote_433_Val[18];
extern bool use_bt_mode_flag;

extern u32 Get_Curr_Music_energy;

extern bool BT_MIC_state;
extern bool BT_MIC2_state;
extern bool PowerOn_Key_Press;
extern bool PowerOff_Key_Press;
extern bool BT_PowerKey_Ctr_EN;
extern u16 btmic_power_cnt;
extern bool Single_Press_EN;

extern u8 Vol_Sw_Flag;
extern u8 SetMicTmrFlag;

extern u8 Tws_master_bt_con;


extern u8 Tws_Slave_PA_state;

extern u8 g_tws_mic_echo;
extern u8 g_tws_mic_rever;
extern u8 g_tws_mic_vol;
extern u8 power_on_mute_po_flag;
extern u32 _Curr_MIC_energy;
extern u32 _Curr_MIC_energy_2;

//extern u32 _Curr_Music_energy;
//extern u32 _Curr_Music_energy_2;
extern u8 A2DP_Play_State;
extern u8 pam_en;
extern u8 Slave_effect_bianyin_state;
extern u8 PH_online_state;
extern u8 power_on_tone_flag;
extern u8 LOW_voltage_flag;
extern u8 BT_state_flag;
extern bool is_bt_mode;
extern u8 Tws_state;
extern u8 RGB_curr_effect;
extern u8 Temp_RGB_curr_effect;
extern u8 yellow_light;
extern u8 Flag_max_vol;
extern u8 Tws_Slave_state;
extern u8 Tws_play_state;
extern u16 led_effect_num;
extern u8 PAM_ctrl_en;
extern u8 TWS_DIS_tone;
extern u8 FM_TX_FALG;
extern bool APP_Volume_UP_DW_Flag;
extern u8 max_min_tone_msg_send_en;
extern bool TWS_Slave_A2DP_Play_State;
extern u8 PowerOnInitAudioEffect_flag;
extern u8 MIC_VOL_Adjust_flag;
extern u8 Curr_key_number;
extern u8 ADkey_press_num;
extern u8 ADkey2_press_num;
extern u8 IOkey_press_num;
extern u8	Press_Key_cnt;
extern u8 moyin_in_out_flag;
extern u8 remind_state;
extern u8 _2ms_con_m;
extern u8 Flag_turnon;
extern u8 Flag_key_clock;
extern u8 Global_playing_flag;
extern u8 Global_rec_playing_flag;
extern bool Power_On_Low_Power_Flag;
extern u8 Global_pause_flag;
extern bool Power_Key_Releasd;
extern bool tone_play_flag;
extern u8 Custom_MusicVolume;
extern u8 Music_Volume;
extern u8 Mic_Volume;
extern u8 Custom_MicECHO_Volume;
extern u8 MicECHO_Volume;

extern bool music_mute_flag;
extern u8 Machine_state;
extern bool wireless_charger_switch;
extern u8 power_down_zx_flag;
extern u8 Global_PA_work;
extern u8 PA_work_mode;
extern u8 Last_PA_workmode;
extern u8 Task_state;
extern u8 BT_state;
extern u8 Tone_play_state;
extern u8 Custom_Event1_number;
extern u8 Custom_Event1_get;
extern u8 KTV_MIC_state;
extern bool mic1_ctrl_en;
extern bool mic2_ctrl_en;
extern u8 bianyi_state;
extern u8 temp_bianyi_state;
extern u8 ZX_key_info_length;
extern u8 update_ok;
extern u8 BtPairTonePlay;

extern int16_t *mic_pcm_temp;


extern u8 RGB_SW;
extern u8 RGB_R_Grade;
extern u8 RGB_G_Grade;
extern u8 RGB_B_Grade;
extern bool DMA_RGB_POWER_SW; 
extern u8 Tws_play_flag;
//extern u8 linein_det_status;
extern u8 phone_state;


extern u8 Flag_fm_scan;
extern u8 Flag_low_power;
extern u8 Flag_low_power_tone;
extern u8 Flag_poweroff;
extern u8 Flag_IR_poweroff;

extern u8 Com_mute_contral;

extern u8 PA_keep_on;
extern u8 PA_keep_off;

extern bool mode_switch_flag;
extern u8 frist_set_eq;
extern u8 update_ok;
extern bool mic_switch;
extern u8 Fm_sel_mode;
extern bool rest_mode_flag;
extern u8 bianyi_state;
extern u8 effect_state;
#define Port_A        0x0000
#define Port_A0      (0x0000+0)
#define Port_A1      (0x0000+1)
#define Port_A2      (0x0000+2)
#define Port_A3      (0x0000+3)
#define Port_A4      (0x0000+4)
#define Port_A5      (0x0000+5)
#define Port_A6      (0x0000+6)
#define Port_A7      (0x0000+7)
#define Port_A8      (0x0000+8)
#define Port_A9      (0x0000+9)
#define Port_A10     (0x0000+10)
#define Port_A11     (0x0000+11)
#define Port_A12     (0x0000+12)
#define Port_A13     (0x0000+13)
#define Port_A14     (0x0000+14)
#define Port_A15     (0x0000+15)
#define Port_A16     (0x0000+16)
#define Port_A17     (0x0000+17)
#define Port_A18     (0x0000+18)
#define Port_A19     (0x0000+19)
#define Port_A20     (0x0000+20)
#define Port_A21     (0x0000+21)
#define Port_A22     (0x0000+22)
#define Port_A23     (0x0000+23)
#define Port_A24     (0x0000+24)
#define Port_A25     (0x0000+25)
#define Port_A26     (0x0000+26)
#define Port_A27     (0x0000+27)
#define Port_A28     (0x0000+28)
#define Port_A29     (0x0000+29)
#define Port_A30     (0x0000+30)
#define Port_A31     (0x0000+31)


#define Port_B        0x0100
#define Port_B0      (0x0100+0)
#define Port_B1      (0x0100+1)
#define Port_B2      (0x0100+2)
#define Port_B3      (0x0100+3)
#define Port_B4      (0x0100+4)
#define Port_B5      (0x0100+5)
#define Port_B6      (0x0100+6)
#define Port_B7      (0x0100+7)


#define Port_LDOIN   (0xf000+1)


#define Port_invalid (0xff00+0)


#define Port_I__A0          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(0));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(0));}
#define Port_O__A0          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(0));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(0));}
#define Port_R__A0          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(0))
#define Port_UH_A0          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(0))
#define Port_UL_A0          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(0))
#define Port_DH_A0          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(0))
#define Port_DL_A0          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(0))
#define Port_OH_A0          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(0))
#define Port_OL_A0          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(0))


#define Port_I__A1          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(1));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(1));}
#define Port_O__A1          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(1));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(1));}
#define Port_R__A1          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(1))
#define Port_UH_A1          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(1))
#define Port_UL_A1          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(1))
#define Port_DH_A1          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(1))
#define Port_DL_A1          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(1))
#define Port_OH_A1          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(1))
#define Port_OL_A1          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(1))




#define Port_I__A5          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(5));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(5));}
#define Port_O__A5          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(5));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(5));}
#define Port_R__A5          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(5))
#define Port_UH_A5          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(5))
#define Port_UL_A5          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(5))
#define Port_DH_A5          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(5))
#define Port_DL_A5          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(5))
#define Port_OH_A5          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(5))
#define Port_OL_A5          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(5))

#define Port_I__A6          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(6));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(6));}
#define Port_O__A6          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(6));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(6));}
#define Port_R__A6          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(6))
#define Port_UH_A6          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(6))
#define Port_UL_A6          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(6))
#define Port_DH_A6          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(6))
#define Port_DL_A6          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(6))
#define Port_OH_A6          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(6))
#define Port_OL_A6          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(6))


#define Port_I__A23          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(23));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(23));}
#define Port_O__A23          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(23));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(23));}
#define Port_R__A23          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(23))
#define Port_UH_A23          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(23))
#define Port_UL_A23          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(23))
#define Port_DH_A23          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(23))
#define Port_DL_A23          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(23))
#define Port_OH_A23          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(23))
#define Port_OL_A23          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(23))



#define Port_I__A24          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(24));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(24));}
#define Port_O__A24          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(24));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(24));}
#define Port_R__A24          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(24))
#define Port_UH_A24          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(24))
#define Port_UL_A24          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(24))
#define Port_DH_A24          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(24))
#define Port_DL_A24          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(24))
#define Port_OH_A24          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(24))
#define Port_OL_A24          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(24))




#define Port_I__A28          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(28));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(28));}
#define Port_O__A28          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(28));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(28));}
#define Port_R__A28          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(28))
#define Port_UH_A28          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(28))
#define Port_UL_A28          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(28))
#define Port_DH_A28          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(28))
#define Port_DL_A28          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(28))
#define Port_OH_A28          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(28))
#define Port_OL_A28          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(28))


#define Port_I__A29          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(29));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(29));}
#define Port_O__A29          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(29));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(29));}
#define Port_R__A29          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(29))
#define Port_UH_A29          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(29))
#define Port_UL_A29          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(29))
#define Port_DH_A29          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(29))
#define Port_DL_A29          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(29))
#define Port_OH_A29          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(29))
#define Port_OL_A29          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(29))


#define Port_I__A30          {GPIO_RegOneBitClear(GPIO_A_OE ,Z_IO_INDEX(30));GPIO_RegOneBitSet(GPIO_A_IE,Z_IO_INDEX(30));}
#define Port_O__A30          {GPIO_RegOneBitClear(GPIO_A_IE ,Z_IO_INDEX(30));GPIO_RegOneBitSet(GPIO_A_OE,Z_IO_INDEX(30));}
#define Port_R__A30          GPIO_RegOneBitGet   (GPIO_A_IN ,Z_IO_INDEX(30))
#define Port_UH_A30          GPIO_RegOneBitSet   (GPIO_A_PU ,Z_IO_INDEX(30))
#define Port_UL_A30          GPIO_RegOneBitClear (GPIO_A_PU ,Z_IO_INDEX(30))
#define Port_DH_A30          GPIO_RegOneBitSet   (GPIO_A_PD ,Z_IO_INDEX(30))
#define Port_DL_A30          GPIO_RegOneBitClear (GPIO_A_PD ,Z_IO_INDEX(30))
#define Port_OH_A30          GPIO_RegOneBitSet   (GPIO_A_OUT,Z_IO_INDEX(30))
#define Port_OL_A30          GPIO_RegOneBitClear (GPIO_A_OUT,Z_IO_INDEX(30))




#define Port_I__B4          {GPIO_RegOneBitClear(GPIO_B_OE ,Z_IO_INDEX(4));GPIO_RegOneBitSet(GPIO_B_IE,Z_IO_INDEX(4));}
#define Port_O__B4          {GPIO_RegOneBitClear(GPIO_B_IE ,Z_IO_INDEX(4));GPIO_RegOneBitSet(GPIO_B_OE,Z_IO_INDEX(4));}
#define Port_R__B4          GPIO_RegOneBitGet   (GPIO_B_IN ,Z_IO_INDEX(4))
#define Port_UH_B4          GPIO_RegOneBitSet   (GPIO_B_PU ,Z_IO_INDEX(4))
#define Port_UL_B4          GPIO_RegOneBitClear (GPIO_B_PU ,Z_IO_INDEX(4))
#define Port_DH_B4          GPIO_RegOneBitSet   (GPIO_B_PD ,Z_IO_INDEX(4))
#define Port_DL_B4          GPIO_RegOneBitClear (GPIO_B_PD ,Z_IO_INDEX(4))
#define Port_OH_B4          GPIO_RegOneBitSet   (GPIO_B_OUT,Z_IO_INDEX(4))
#define Port_OL_B4          GPIO_RegOneBitClear (GPIO_B_OUT,Z_IO_INDEX(4))

#define Port_I__B5          {GPIO_RegOneBitClear(GPIO_B_OE ,Z_IO_INDEX(5));GPIO_RegOneBitSet(GPIO_B_IE,Z_IO_INDEX(5));}
#define Port_O__B5          {GPIO_RegOneBitClear(GPIO_B_IE ,Z_IO_INDEX(5));GPIO_RegOneBitSet(GPIO_B_OE,Z_IO_INDEX(5));}
#define Port_R__B5          GPIO_RegOneBitGet   (GPIO_B_IN ,Z_IO_INDEX(5))
#define Port_UH_B5          GPIO_RegOneBitSet   (GPIO_B_PU ,Z_IO_INDEX(5))
#define Port_UL_B5          GPIO_RegOneBitClear (GPIO_B_PU ,Z_IO_INDEX(5))
#define Port_DH_B5          GPIO_RegOneBitSet   (GPIO_B_PD ,Z_IO_INDEX(5))
#define Port_DL_B5          GPIO_RegOneBitClear (GPIO_B_PD ,Z_IO_INDEX(5))
#define Port_OH_B5          GPIO_RegOneBitSet   (GPIO_B_OUT,Z_IO_INDEX(5))
#define Port_OL_B5         GPIO_RegOneBitClear  (GPIO_B_OUT,Z_IO_INDEX(5))













#define Custom_Event1 0xfff1
#define Custom_Event2 0xfff2




#define Z_IO_INDEX(x)   (GPIO_INDEX)(1<<x)

enum
{
    Set_Mic_Vol=0,
	Set_Reverb_Vol,
	Set_Music_Vol,
};

enum
{
    IdleMode_Enter=0,//已进入IDLE模式
    IdleMode_QuitI, //已退出IDLE模式
    IdleMode_Enter_Starting,//IDLE模式进入中
    IdleMode_QuitI_Starting,//IDLE模式退出中
};


enum
{
    Fm_sel_next_station,
    Fm_sel_next_step,
};

enum
{
    DEBUG_ALL,    
    DEBUG_BAT,    
    DEBUG_IOKEY,    
    DEBUG_ADKEY,    
    DEBUG_Msg1,    
    DEBUG_Msg2, 
    DEBUG_Msg3, 
    DEBUG_led, 
    DEBUG_Fm, 
    DEBUG_Bt, 
    DEBUG_Rec, 
    DEBUG_task,
    DEBUG_Timing,
    DEBUG_Tuch,
    DEBUG_AudEff,
};
	

enum
{
    MOS_OFF,
    MOS_ON,
};

enum
{
    IOKey_num__1 =1,
    IOKey_num__2,
    IOKey_num__3,
    IOKey_num__4,



//--------------------
    PowK1_Res_100R =100,
  //  PowK1_Res___0 =100,
    PowK1_Res__16,
    PowK1_Res__27,
    PowK1_Res__43,
    PowK1_Res__82,
    PowK1_Res_220,
//--------------------
    PowK2_Res_100R =110,
    PowK2_Res_2_2,
    PowK2_Res_4_7,
    PowK2_Res_8_2,
    PowK2_Res__12,
    PowK2_Res__18,
    PowK2_Res__27,
    PowK2_Res__39,
    PowK2_Res__58,
    PowK2_Res_100,
    PowK2_Res_220,
//--------------------
    ADK1_Res___0 =130,
    
    ADK1_Res_2_2,
    ADK1_Res_4_7,
    ADK1_Res_8_2,
    ADK1_Res__12,// 134
    ADK1_Res__18,// 135
    ADK1_Res__27,// 136
    ADK1_Res__39,
    ADK1_Res__58,// 138
    ADK1_Res_100,
    ADK1_Res_220,
    
//--------------------
    ADK2_Res___0 =150,
    
	  ADK2_Res_2_2,
	  ADK2_Res_4_7,
	  ADK2_Res_8_2,
	  ADK2_Res__12,
	  ADK2_Res__18,
	  ADK2_Res__27,
	  ADK2_Res__39,
	  ADK2_Res__58,
	  ADK2_Res_100,
	  ADK2_Res_220,
	  
//--------------------  //433 遥控
    Remote_00=500,
    Remote_01,Remote_02,Remote_03,Remote_04,Remote_05,Remote_06,Remote_07,Remote_08,Remote_09,Remote_0a,Remote_0b,Remote_0c,Remote_0d,Remote_0e,Remote_0f,
    Remote_10,Remote_11,Remote_12,Remote_13,Remote_14,Remote_15,Remote_16,Remote_17,Remote_18,Remote_19,Remote_1a,Remote_1b,Remote_1c,Remote_1d,Remote_1e,Remote_1f,
    Remote_20,Remote_21,Remote_22,Remote_23,Remote_24,Remote_25,Remote_26,Remote_27,Remote_28,Remote_29,Remote_2a,Remote_2b,Remote_2c,Remote_2d,Remote_2e,Remote_2f,
    Remote_30,Remote_31,Remote_32,Remote_33,Remote_34,Remote_35,Remote_36,Remote_37,Remote_38,Remote_39,Remote_3a,Remote_3b,Remote_3c,Remote_3d,Remote_3e,Remote_3f,
    Remote_40,Remote_41,Remote_42,Remote_43,Remote_44,Remote_45,Remote_46,Remote_47,Remote_48,Remote_49,Remote_4a,Remote_4b,Remote_4c,Remote_4d,Remote_4e,Remote_4f,
    Remote_50,Remote_51,Remote_52,Remote_53,Remote_54,Remote_55,Remote_56,Remote_57,Remote_58,Remote_59,Remote_5a,Remote_5b,Remote_5c,Remote_5d,Remote_5e,Remote_5f,
    Remote_60,Remote_61,Remote_62,Remote_63,Remote_64,Remote_65,Remote_66,Remote_67,Remote_68,Remote_69,Remote_6a,Remote_6b,Remote_6c,Remote_6d,Remote_6e,Remote_6f,
    Remote_70,Remote_71,Remote_72,Remote_73,Remote_74,Remote_75,Remote_76,Remote_77,Remote_78,Remote_79,Remote_7a,Remote_7b,Remote_7c,Remote_7d,Remote_7e,Remote_7f,
    Remote_80,Remote_81,Remote_82,Remote_83,Remote_84,Remote_85,Remote_86,Remote_87,Remote_88,Remote_89,Remote_8a,Remote_8b,Remote_8c,Remote_8d,Remote_8e,Remote_8f,
    Remote_90,Remote_91,Remote_92,Remote_93,Remote_94,Remote_95,Remote_96,Remote_97,Remote_98,Remote_99,Remote_9a,Remote_9b,Remote_9c,Remote_9d,Remote_9e,Remote_9f,
    Remote_a0,Remote_a1,Remote_a2,Remote_a3,Remote_a4,Remote_a5,Remote_a6,Remote_a7,Remote_a8,Remote_a9,Remote_aa,Remote_ab,Remote_ac,Remote_ad,Remote_ae,Remote_af,
    Remote_b0,Remote_b1,Remote_b2,Remote_b3,Remote_b4,Remote_b5,Remote_b6,Remote_b7,Remote_b8,Remote_b9,Remote_ba,Remote_bb,Remote_bc,Remote_bd,Remote_be,Remote_bf,
    Remote_c0,Remote_c1,Remote_c2,Remote_c3,Remote_c4,Remote_c5,Remote_c6,Remote_c7,Remote_c8,Remote_c9,Remote_ca,Remote_cb,Remote_cc,Remote_cd,Remote_ce,Remote_cf,
    Remote_d0,Remote_d1,Remote_d2,Remote_d3,Remote_d4,Remote_d5,Remote_d6,Remote_d7,Remote_d8,Remote_d9,Remote_da,Remote_db,Remote_dc,Remote_dd,Remote_de,Remote_df,
    Remote_e0,Remote_e1,Remote_e2,Remote_e3,Remote_e4,Remote_e5,Remote_e6,Remote_e7,Remote_e8,Remote_e9,Remote_ea,Remote_eb,Remote_ec,Remote_ed,Remote_ee,Remote_ef,
    Remote_f0,Remote_f1,Remote_f2,Remote_f3,Remote_f4,Remote_f5,Remote_f6,Remote_f7,Remote_f8,Remote_f9,Remote_fa,Remote_fb,Remote_fc,Remote_fd,Remote_fe,Remote_ff, 


//--------------------
    
    IR_00=1000,
          IR_01,IR_02,IR_03,IR_04,IR_05,IR_06,IR_07,IR_08,IR_09,IR_0a,IR_0b,IR_0c,IR_0d,IR_0e,IR_0f,
    IR_10,IR_11,IR_12,IR_13,IR_14,IR_15,IR_16,IR_17,IR_18,IR_19,IR_1a,IR_1b,IR_1c,IR_1d,IR_1e,IR_1f,
    IR_20,IR_21,IR_22,IR_23,IR_24,IR_25,IR_26,IR_27,IR_28,IR_29,IR_2a,IR_2b,IR_2c,IR_2d,IR_2e,IR_2f,
    IR_30,IR_31,IR_32,IR_33,IR_34,IR_35,IR_36,IR_37,IR_38,IR_39,IR_3a,IR_3b,IR_3c,IR_3d,IR_3e,IR_3f,
    IR_40,IR_41,IR_42,IR_43,IR_44,IR_45,IR_46,IR_47,IR_48,IR_49,IR_4a,IR_4b,IR_4c,IR_4d,IR_4e,IR_4f,
    IR_50,IR_51,IR_52,IR_53,IR_54,IR_55,IR_56,IR_57,IR_58,IR_59,IR_5a,IR_5b,IR_5c,IR_5d,IR_5e,IR_5f,
    IR_60,IR_61,IR_62,IR_63,IR_64,IR_65,IR_66,IR_67,IR_68,IR_69,IR_6a,IR_6b,IR_6c,IR_6d,IR_6e,IR_6f,
    IR_70,IR_71,IR_72,IR_73,IR_74,IR_75,IR_76,IR_77,IR_78,IR_79,IR_7a,IR_7b,IR_7c,IR_7d,IR_7e,IR_7f,
    IR_80,IR_81,IR_82,IR_83,IR_84,IR_85,IR_86,IR_87,IR_88,IR_89,IR_8a,IR_8b,IR_8c,IR_8d,IR_8e,IR_8f,
    IR_90,IR_91,IR_92,IR_93,IR_94,IR_95,IR_96,IR_97,IR_98,IR_99,IR_9a,IR_9b,IR_9c,IR_9d,IR_9e,IR_9f,
    IR_a0,IR_a1,IR_a2,IR_a3,IR_a4,IR_a5,IR_a6,IR_a7,IR_a8,IR_a9,IR_aa,IR_ab,IR_ac,IR_ad,IR_ae,IR_af,
    IR_b0,IR_b1,IR_b2,IR_b3,IR_b4,IR_b5,IR_b6,IR_b7,IR_b8,IR_b9,IR_ba,IR_bb,IR_bc,IR_bd,IR_be,IR_bf,
    IR_c0,IR_c1,IR_c2,IR_c3,IR_c4,IR_c5,IR_c6,IR_c7,IR_c8,IR_c9,IR_ca,IR_cb,IR_cc,IR_cd,IR_ce,IR_cf,
    IR_d0,IR_d1,IR_d2,IR_d3,IR_d4,IR_d5,IR_d6,IR_d7,IR_d8,IR_d9,IR_da,IR_db,IR_dc,IR_dd,IR_de,IR_df,
    IR_e0,IR_e1,IR_e2,IR_e3,IR_e4,IR_e5,IR_e6,IR_e7,IR_e8,IR_e9,IR_ea,IR_eb,IR_ec,IR_ed,IR_ee,IR_ef,
    IR_f0,IR_f1,IR_f2,IR_f3,IR_f4,IR_f5,IR_f6,IR_f7,IR_f8,IR_f9,IR_fa,IR_fb,IR_fc,IR_fd,IR_fe,IR_ff, 

};

enum
{
    X1_MSG0=2000,
    X1_MSG1,X1_MSG2, X1_MSG3, X1_MSG4, X1_MSG5, X1_MSG6, X1_MSG7, X1_MSG8, X1_MSG9, X1_MSG10,X1_MSG11, 
    X1_MSG12, X1_MSG13, X1_MSG14, X1_MSG15, X1_MSG16, X1_MSG17, X1_MSG18, X1_MSG19, X1_MSG20, 
};


enum
{
    _Key_press__    =1,
    _Key_click__    ,
    _Key_long_1s    ,
    _Key_hold___    ,
    _Key_long_up    ,

    _Key_double_     ,
    _Key_click_d    ,
    _Key_CLICK3_    ,

    
    _Key_long_2s    ,
    _Key_long_3s    ,
    _Key_long_4s    ,
    _Key_long_5s    ,
    _Key_long_6s    ,

};




enum
{
    Custom_Event1_GO=1,
    Custom_Event1_tone_LOWPOWER,
    Custom_Event1_tone_tws_master_phone,
    Custom_Event1_tone_tws_master_slave,
    Custom_Event1_tone_tws_slave_master,
    Custom_Event1_tone_TWS_Pair,
    Custom_Event1_tone_tws_con,
    Custom_Event1_tone_tws_discon,

	Custom_Event1_tone_BtPair,
    
    Custom_Event1_TWS_SET_VOL,
    Custom_Event1_TWS_SET_VOL_stop,
    
    Custom_Event1_xx,
    Custom_Event1_siri_open,

    Custom_Event1_Poweroff,
    Custom_Event1_IR_Poweroff,
    Custom_Event1_lowpower,
    Custom_Event1_Poweroff_slave,
    Custom_Event1_Poweron,
    Custom_Event1_110p_pp,
    
    Custom_Event1_IDLE_pp,
    
    //Custom_Event1_tone_MOYOUT,

    Custom_Event1_MUSIC_RPT,
    
    Custom_Event1_tone_wuya,
	Custom_Event1_tone_zhangsheng,
	Custom_Event1_tone_huanhu,
	Custom_Event1_tone_bishi,
	Custom_Event1_tone_ganga,
	Custom_Event1_tone_qinw,
	Custom_Event1_tone_memeda,
	Custom_Event1_tone_hongxiao,
	Custom_Event1_tone_liqu,
    Custom_Event1_tone_qihong,


	
	Custom_Event1_max_vol,
	Custom_Event1_min_vol,
	Custom_Event1_Key_Tone,
	Custom_Event1_Mic_Echo_up,
	Custom_Event1_Mic_Echo_down,

	Custom_Event1_TWS_Disconnect,

	Custom_Event1_Moyin_sw,
	Custom_Event1_Tws_Tone,
	Custom_Event1_Tone_Mic_Max,
	Custom_Event1_Tone_Mic_Min,
	Custom_Event1_Tone_Echo_Max,
	Custom_Event1_Tone_Echo_Min,
	Custom_Event1_Tone_Effect_Key,

    
    Custom_Event1_music_next_folder,

    Custom_Event1_bt_call_change,
    Custom_Event1_TWS_pairing,
    Custom_Event1_tone_photo_on,
    Custom_Event1_tone_photo_off,
    

    Custom_Event1_EQ_SW,
    Custom_Event1_long_1s_tone,
    
    Custom_Event1_REC_SW,
    Custom_Event1_PA_ONOFF,
    Custom_Event1_vol_HLsw,
    
    Custom_Event1_KT0913_Scan_ALL,
    Custom_Event1_KT0913_Scan_ALL_END,
    Custom_Event1_KT0913_PP,
    Custom_Event1_KT0913_NEXT,
    Custom_Event1_KT0913_PREV,
    Custom_Event1_KT0913_FMAM_SW,
    

    Custom_48000_SW,



    
    Custom_Event1_SD01u_pp,
    Custom_Event1_SD01u_next,
    Custom_Event1_SD01u_prev,
    Custom_Event1_SD01u_next10,
    Custom_Event1_SD01u_prev10,


    Custom_Event1_pp,


    
    Custom_Event1_SD0_pp,
    Custom_Event1_SD1_pp,
    Custom_Event1_USB_pp,


    Custom_Event1_AUX1_mute,
    Custom_Event1_AUX1_unmute,
    Custom_Event1_AUX1_pp,
    Custom_Event1_AUX1_next,
    Custom_Event1_AUX1_prev,
    Custom_Event1_AUX1_next10,
    Custom_Event1_AUX1_prev10,



    Custom_Event1_AUX2_pp,
    Custom_Event1_AUX3_pp,

    Custom_Event1_FM_pp,
    Custom_Event1_LED7_dis_FM_INF,
    //Custom_Event1_FM_next,
    //Custom_Event1_FM_prev,

    Custom_Event1_msg_slave_to_master,//对箱从机消息同步到主机用
    Custom_Event1_DSP_11OP_TONE_EQ1,
	Custom_Event1_DSP_11OP_TONE_EQ2,
	Custom_Event1_DSP_11OP_TONE_EQ3,
	Custom_Event1_DSP_11OP_TONE_EQ4,
    
	Custom_Event1_TONE_POWER_ON,

    

    Custom_Event2_GO,// 以下的消息只会在task_common中接受
    Custom_Event2_tone_lowpower,
    Custom_Event2_bt_idle_switch,
    Custom_Event2_to_AUX,
    Custom_Event2_to_idle,
    Custom_Event2_to_bt,
    Custom_Event2_mute,
    //Custom_Event2_set_vol,
    Custom_Event2_set_music_vol,
    Custom_Event2_tone_poweroff,
    Custom_Event2_tone_maxvol_2,
    Custom_Event2_tone_minvol_2,
    Custom_Event2_tone_maxvol_1,
    Custom_Event2_tone_minvol_1,
    
    Custom_Event2_VOL_up,
    Custom_Event2_VOL_down,
    
    Custom_Event2_PA_contral_once,
    Custom_Event2_EQ_SW,
    Custom_Event2_poweroff,
    Custom_Event2_fake_poweroff,
    Custom_Event2_RGB_OFF,
    Custom_Event2_one_color_OFF,
    Custom_Event2_Timer1_OFF,
    
    Custom_Event2_ECHO_ON,
    Custom_Event2_ECHO_OFF,
    Custom_Event2_MIC_UP,
    Custom_Event2_MIC_DOWN,

    Custom_Event2_one_pwm_sw,
    Custom_Event2_one_pwm_next,
    Custom_Event2_one_pwm_next2,

    
    Custom_Event2_Moyin_set,
    Custom_Event2_tone_MOYIN_CTL,
    Custom_Event2_tone_MOYIN_next,

    Custom_Event2_clean_voice,

    
    Custom_Event2_to_110p_ch1,
    Custom_Event2_to_110p_ch2,
    Custom_Event2_to_110p_ch3,
    
    Custom_Event2_to_idle_110p_ch1,
    Custom_Event2_to_idle_110p_ch2,
    Custom_Event2_to_idle_110p_ch3,
    
    Custom_Event2_to_set_110p_vol,
    
    Custom_Event2_headset_on,
    Custom_Event2_headset_off,
    
    Custom_Event2_mode_sw,

    Custom_Event2_BTMIC_sw,
    
    Custom_Event2_KT0641_next,
    Custom_Event2_KT0641_sw,
    Custom_Event2_KT0641_mute,
    Custom_Event2_KT0641_vol_up,
    Custom_Event2_KT0641_vol_down,

	Custom_Event1_FMTX,
    Custom_Event2_QN8027_sw,
    Custom_Event2_QN8027_next,
    Custom_Event2_QN8027_next2,

	
	Custom_Event2_sound_effect_one,
    Custom_Event2_sound_effect_two,
    Custom_Event2_sound_scene_ktv,
    Custom_Event2_sound_scene_recordingroom,
    Custom_Event2_sound_scene_country,
    
	Custom_Event2_scene_up,
    Custom_Event2_scene_down,

	
	
    Custom_Event2_BT_Master_send_com_maxvol,
    Custom_Event2_BT_Master_receive_com_maxvol,
    
    Custom_Event2_TONE1,
    Custom_Event2_TONE2,
    Custom_Event2_TONE3,
    Custom_Event2_TONE4,
    Custom_Event2_TONE5,
    Custom_Event2_TONE6,
    Custom_Event2_TONE7,
    
    Custom_Event2_VOLkey_LONG_UP,
    
    
//-------------------------------
    Custom_Event2_END,



    
};








enum
{
//-------------------------------
    _Key_fun1_MIN,                          //1功能区间

    
    _Key_MSG_0=1,
    _Key_MSG_1,
    _Key_MSG_2,
    _Key_MSG_3,
    _Key_MSG_4,
    _Key_MSG_5,
    _Key_MSG_6,
    _Key_MSG_7,
    _Key_MSG_8,
    _Key_MSG_9,// 10

    _Key_fun1_pp,
    _Key_fun1_clean_voice,
    _Key_fun1_music_mute,//音乐mute 
    _Key_fun1_prev,
    _Key_fun1_next,// 15
    _Key_fun1_prev_10,
    _Key_fun1_next_10, 

    
    _Key_fun1_max_tone,
    _Key_fun1_min_tone,
    _Key_fun1_key_up,//20
    
    _Key_fun1_slave_tone_end,
    _Key_fun1_siri_open,
    

    _Key_fun1_VOLkey_hold,
    _Key_fun1_VOLkey_LONG,
    _Key_fun1_VOLkey_LONG_UP,
    
    _Key_fun1_disp_vol_set,

    _Key_fun1_music_FF,
    _Key_fun1_music_FR,

    
    _Key_fun1_next_folder,
    _Key_fun1_volume_up,
    _Key_fun1_volume_down,//30
    _Key_fun1_MIC_vol_up,
    _Key_fun1_MIC_vol_down,
    _Key_fun1_mode,
    _Key_fun1_mode_and_moyin, //切换模式和魔音
    _Key_fun1_moyin_in_out,
    _Key_fun1_moyin_on_effect_SW,//进入魔音后才可切换
    _Key_fun1_EQ,
    _Key_fun1_EFFECT,
    _Key_fun1_bianyin,
    _Key_fun1_effect_moyin,
    _Key_fun1_FakeVoiceNext,
    _Key_fun1_FakeVoiceNext2,
    _Key_fun1_music_mode,
    _Key_fun1_music_eq,
    _Key_fun1_fm_scan,
    _Key_fun1_fm_scan_down,// 40
    _Key_fun1_fm_scan_up,
    _Key_fun1_FM_sel_mode, 
    _Key_fun1_call_change,
    _Key_fun1_call_back,
    _Key_fun1_call_ctr,
    _Key_fun1_call_reject,
    _Key_fun1_call_hangup,
    _Key_fun1_powerdown,// 47
    
    _Key_fun1_IR_powerdown,
    _Key_fun1_IR_powerup,//
    
    _Key_fun1_poweron,// 50
    _Key_fun1_scene_ktv,// KTV模式
    _Key_fun1_scene_country,// 乡村模式
    _Key_fun1_scene_recordingroom,// 录音棚模式
    _Key_fun1_scene_micba,// 
    _Key_fun1_scene_zhuanye,// 
    _Key_fun1_scene_zhuchi,// 
    
    _Key_fun1_scene_up,//  场景上选
    _Key_fun1_scene_down,//  场景下选
    _Key_fun1_effect_SW,    
    _Key_fun1_sound_effect_one,// 音效一
    _Key_fun1_sound_effect_two,// 音效二
    _Key_fun1_bt_dis,
    _Key_fun1_bt_ctr,
    _Key_fun1_TWS,
    _Key_fun1_bt_mode,
    _Key_fun1_HID_SW,
    _Key_fun1_Photograph,
    _Key_fun1_ToFakeVoice,
    _Key_fun1_bt_idle,              //蓝牙和空闲模式切换
	_Key_fun1_rec,// 录音开启
	_Key_fun1_rec_playback,// 录音回放
	_Key_fun1_rec_filedel,// 录音删除

    
    _Key_fun1_KT0641_next,  
    _Key_fun1_KT0641_sw,
    _Key_fun1_KT0641_mute,
    _Key_fun1_KT0641_vol_up,
    _Key_fun1_KT0641_vol_down,

    _Key_fun1_QN8027_sw,
	_Key_fun1_QN8027_next, 
	_Key_fun1_QN8027_next2, 


    _Key_fun1_PAM_SW,//喇叭开关

    _Key_fun1_BTMIC_Power_sw,
    
    _Key_fun1_Custom_out1_ctl, 
    _Key_fun1_Custom_out2_ctl, 
    _Key_fun1_Custom_out3_ctl, 

    _Key_fun1_RGB_SW,
    _Key_fun1_RGB_next,
    _Key_fun1_RGB_next2,

    _Key_fun1_DMA_RGB_MODE_effect_SW,//RGB 模式切换
    _Key_fun1_DMA_RGB_SW, //RGB 开关
    
    _Key_fun1_led_close,
    
    _Key_fun1_tone_wuya,//乌鸦提示音
    _Key_fun1_tone_bishi,// 鄙视提示音
    _Key_fun1_tone_zhangsheng,//掌声提示音
    _Key_fun1_tone_huanhu,//欢呼提示音
    _Key_fun1_tone_ganga,//尴尬提示音
    _Key_fun1_tone_qinw,//轻吻提示音
    _Key_fun1_tone_hongxiao,//哄笑提示音
    _Key_fun1_tone_liqu,//哄笑提示音
    _Key_fun1_tone_memeda,
    _Key_fun1_tone_max,
    _Key_fun1_tone_min,
    
    _Key_fun1_moyin_ctrl,//魔音控制
    
    
	_Key_fun1_MIC_SW,// mic 开关
	_Key_fun1_MIC_ECHO_UP,// mic 混响加
	_Key_fun1_MIC_ECHO_DOWN,// mic 混响加
	
	_Key_fun1_LINE3_MIC_L_SW,
	_Key_fun1_LINE3_MIC_R_SW,

    _Key_fun1_set_vol_sw,
	_Key_fun1_sw_vol_up,
	_Key_fun1_sw_vol_down,

    _Key_fun1_3D_SW,
	_key_fun1_Vol_Set,// 无线充开启
	_Key_fun1_110p_eq_sw,	//切换EQ模式
	_Key_fun1_110p_eq_movie,	//切换EQ模式
	_Key_fun1_110p_eq_music,	//切换EQ模式
	_Key_fun1_110p_eq_news,	//切换EQ模式
	_Key_fun1_110p_eq_game,	//切换EQ模式

	_Key_fun1_mode_hdmi,
	_Key_fun1_mode_bt,
	_Key_fun1_mode_optical,
	_Key_fun1_mode_aux,
	_Key_fun1_mode_spidif,
	_Key_fun1_mode_idle,
	_Key_fun1_dacx_vol_up,
	_Key_fun1_dacx_vol_down,
	_Key_fun1_ctrl_light,		//控制暖光灯
    _Key_fun1_MAX,                      
//-------------------------------
    _Key_fun2_MIN,                      //2功能区间
    
    _Key_fun2_REC_start_stop,      
    _Key_fun2_to_rec_REC_start,

    _Key_fun2_mode_and_moyin, //切换模式和魔音
    _Key_fun2_clean_voice_and_moyin, //消原音和魔音


    _Key_fun2_TWS_bt_dis,
    _Key_fun2_TWS_HID,      
    _Key_fun2_pp_call,
    _Key_fun2_powerdown_reject,
    _Key_fun2_call_back_reject,
    _Key_fun2_bt_ctr_reject,
    _Key_fun2_bt_ctr_hangup,
    _Key_fun2_powerdown_fm_scan,
    _Key_fun2_mode_FakeVoiceNext,
    
    _Key_fun2_pp_fm_scan,
    _Key_fun2_fm_scan_reject,
    _Key_fun2_fm_scan_bt_ctr,
    _Key_fun2_mode_reject,
    
    _Key_fun2_powerdown_poweron,
    _Key_fun2_IR_powerdown_poweron,
    
    _Key_fun2_music_mode_FM_sel_mode,
    
    _Key_fun2_MAX,                      
//-------------------------------
    _Key_fun3_MIN,                      //3功能区间
    
    _Key_fun3_sw_vol_up,
	_Key_fun3_sw_vol_down,
	
    _Key_fun3_pp_call_hid,     
    _Key_fun3_reject_bt_ctr_fm_scan,
    _Key_fun3_pp_call_fm_scan,

    _Key_fun3_to_rec_REC_start_REC_pp,

    _Key_fun3_MAX,                      
//-------------------------------



//-------------------------------
    _Key_DEBUG_MIN,                     //调试区间

    _Key_multiplex_1,
    _Key_multiplex_2,
    _Key_multiplex_3,
    _Key_custom_1,
    _Key_custom_2,
    _Key_custom_3,
    _Key_custom_4,
    _Key_custom_leds_add1,
    _Key_custom_didi_tone,
    _Key_custom_lowbattery_tone,
    _Key_custom_x1_next,
    _Key_custom_x1,
    _Key_custom_x2,
    _Key_custom_x3,
    _Key_custom_x4,
    _Key_custom_x5,
    _Key_custom_x6,
    _Key_custom_mode,
    _Key_custom_vol_1_30,
    _Key_custom_daconoff,
    _Key_custom_printf_dac_energy,
    
    _Key_NULL_MSG,//空消息
};




enum
{
    Machine_Poweroff,    
    Machine_Poweroff_tone,    
    Machine_Poweron,    
    Machine_init,    
    Machine_poweron_tone,    
    Machine_run,    
    Machine_FakePoweroff,
};


enum
{
    RGB_effect_mode_breathe1=1,
    RGB_effect_mode_breathe2,
    RGB_effect_mode_rhythm,
    RGB_effect_mode_R,
    RGB_effect_mode_G,
    RGB_effect_mode_B,
    RGB_effect_mode_RG,
    RGB_effect_mode_RB,
    RGB_effect_mode_GB,
    
};


enum
{
    RGB_Effect_breathe1=1,		//
    RGB_Effect_breathe2,		//
	RGB_Effect_rhythm1, 		//
	RGB_Effect_rhythm2,	
	RGB_Effect_flowing_water, 		//
	RGB_Effect_red_blue_blinky,		//
	RGB_Effect_bt_on, 	//
	RGB_Effect_close,	//


	RGB_Effect_PowerOn,//9
    RGB_Effect_PowerOff,//10
    RGB_Effect_Bt_Con, //11
	RGB_Effect_Bt_Dis,//12
	RGB_Effect_Bt_Play,//13
	RGB_Effect_Bt_Pause,//14
	RGB_Effect_HFP_CALL_IN,//来电 //15
    RGB_Effect_HFP_Answer,//接听 //16
	RGB_Effect_Lowpower,//低电  //17
	RGB_Effect_PowerOn_Charge,//开机充电 //18
	RGB_Effect_PowerOff_Charge,//关机充电 //19
	RGB_Effect_TWS_Con,//20
	
};




typedef struct VoiceChangerMode
{
	
	uint32_t 			 enable;
	uint32_t 			 pitch_ratio;
	uint32_t 		     formant_ratio;
	

} VoiceChangerMode;


extern  VoiceChangerMode   VoiceChanger[5];
struct _X1_PT{
    u8 state;       //协议状态
    u8 msg;
    u8 ready;       //接收到完整的消息后置位  可查询此位进行接收的消息处理 识别到后需要清零
    
    u8 RX_en;
    u8 RX_msg;      //接收到的按键值
    u8 RX_msg_type;
    u8 RX_msg_property;     //识别接收到的按键的操作的类型  长按 短按 等
    u8 RX_level;
    u8 RX_H_level_flag;
    u8 RX_H__EDGE;
    u16 RX_H_level_count;
    u8 RX_L_level_flag;
    u8 RX_L__EDGE;
    u16 RX_L_level_count;
    u8 RX_wave_num;
    
    u8 RX_d_msg_con;
    u8 RX_d_msg;

    u8 TX_en;
    u8 TX_state;
    //u8 TX_level;
    //u8 TX_count;
    u8 TX_frame;
    u8 TX_frame_par;
    u8 TX_step;
    u8 TX_wave_num;
    u8 TX_msg;              //需要发送的消息
    u8 TX_msg_arr[10];
    u8 TX_msg_res;
    
    u8 Get_msg_res;
    u8 TX_end_frame_par;
    
};

typedef struct Key_info
{
    u16 Key_name;
	//u8 Key_function[5][2];
	u8 Key_function[10][2];//Z_key_long_ns_EN

}_Key_fun_info;

enum
{
  moyin_out,
  moyin_in,  
};

enum
{
  tone_end,
  tone_start,  
};


typedef enum
{    
     on_line,
	 off_line, 	
}idle_mode_state;


//待机、开机相关结构体
typedef struct {

	idle_mode_state idle_mode;  // on_line :设置即将进待机或在待机模式;   off_line: 设置退出待机或当前非待机;
    u8 power_on_tone;//开机提示音  tone_start: 开始播开机提示音  tone_end: 播放结束
    u8 power_off_tone;//关机提示音  tone_start: 开始播关机提示音  tone_end: 播放结束
    
} idle_sw;



#define Task_bt         0x10
#define Task_sd         0x21
#define Task_usb        0x24
#define Task_fm         0x30
#define Task_line       0x40
#define Task_pc         0x50
#define Task_rec        0x60
#define Task_idle       0x70
#define Task_OPTICAL    0x80    //光纤
#define Task_COAXIAL    0x90    //同轴
#define Task_HDMI_in    0xa0    //
#define Task_IIS_in     0xb0    //
#define Task_IIS_out    0xc0    //
#define Task_TwsSlave   0xd0    //


enum
{
    Task_sd_for_sd0=Task_sd+1,
    Task_sd_for_sd1,
    Task_sd_for_usb,
};

enum
{
    Task_AUX_for_linein1=Task_line+1,//
    Task_AUX_for_linein2,           //
    Task_AUX_for_linein3,           //
    Task_AUX_for_EX_linein1,
    Task_AUX_for_EX_linein2,
    Task_AUX_for_EX_linein3,
    Task_AUX_for_EX_FM1,
    Task_AUX_for_IDLE=0xf0,
    
};
enum
{
    //Task_idle_for_IDLE=Task_idle+1,
    Task_idle_for_powerup=Task_idle+1,
    Task_idle_for_poweroff,
    Task_idle_for_fake_poweroff,
    Task_idle_for_TaskScheduler,
    Task_idle_for_lowpower_protect,
};


enum
{
    BT_master,
	BT_connect,
	BT_disconnect,
	BT_reconnect,
	BT_A2DP_Start,
	BT_A2DP_Suspend,
	BT_HFP_CALL_IN,
	BT_HFP_CALL_OUT,
	BT_HFP_CALL_ACTIVE,
	BT_HFP_CALL_SUSPEND,
    BT_master_phone, 
    BT_master_slave,//tws已连接,为TWS主机,并且主机未连手机
    BT_master_slave_phone,//tws已连接,为TWS主机,并且主机连手机
    BT_slave_master,//tws已连接,为TWS从机
    BT_tws_mode,
};

#define PA_type_1_x           (0x10+0x0f)   //自定义



#define PA_type_1_1           (0x10+0x01)   //只有一个mute脚 高开低mute
#define PA_type_1_2           (0x10+0x02)   //只有一个mute脚 低开高mute
#define PA_type_1_3           (0x10+0x03)   //进收音为  上拉-输入状态

#define PA_type_1_4           (0x10+0x04)   //未开放
#define PA_type_1_5           (0x10+0x05)   //未开放
#define PA_type_1_6           (0x10+0x06)   //未开放
#define PA_type_1_7           (0x10+0x07)   //未开放
#define PA_type_1_8           (0x10+0x08)   //未开放
#define PA_type_1_9           (0x10+0x09) //  //1线脉冲，收音模式4个低电平 其他2个低电平


#define PA_type_2_1           (0x20+0x01)   //有mute脚和ABD控制脚 高开低mute 高D类 低AB类
#define PA_type_2_2           (0x20+0x02)   //有mute脚和ABD控制脚 高开低mute 高D类 低AB类

#define PA_type_2_3           (0x20+0x03)   //

#define PA_type_2_4           (0x20+0x04)   //未开放
#define PA_type_2_5           (0x20+0x05)   //未开放
#define PA_type_2_6           (0x20+0x06)   //未开放
#define PA_type_2_7           (0x20+0x07)   //未开放
#define PA_type_2_8           (0x20+0x08)   //未开放
#define PA_type_2_9           (0x20+0x09)   //未开放



typedef struct {
    u16 DET_pin;
    u8 DET_way;
    u8 IO_cur;   
    u8 IO_Stable;   //稳定值
    u8 DEV_state;
    u8 DET_fil;  
    u8 DET_cnt;  
    u8 IO_Effective;
}Dev_port_1;

enum
{
    Z_DEV_OFF_HOLD,
    Z_DEV_OFF,
    Z_DEV_ON_HOLD,
    Z_DEV_ON,
};


#define Z_DEV_way_IO            1
#define Z_DEV_way_ADK1          2
#define Z_DEV_way_ADK2          3





typedef struct {
    u8 name;
    u8 Online_state;
    u8 play_state;
    
} Z_Task_inf;

typedef struct {
    u8 name;
    u8 Online_state;
    u8 play_state;
    
} Z_Task_Rec_inf;


enum
{
    Task_offline,
    Task_online,
};



enum
{
    PA_D_mute=1,
    PA_D_unmute,

    PA_AB_mute,
    PA_AB_unmute,

};

enum
{
    _Music_stop,
    _Music_play,
    _Music_puse,
};
enum
{
    _Rec_stop,
    _Rec_dealing,
    _Rec_playback,
};

enum
{
    PA_on=1,
    PA_off,
};


enum
{
    X1_wait,    //等待
    X1_idle,    //空闲
    X1_end_1,   
    X1_end_2,   
    X1_start,   
    X1_error,   
    
};

enum
{
    X1_RX_pro_short=1,  //短按
    X1_RX_pro_long,     //长按
    X1_RX_pro_hold,     //长按保持
    
    X1_RX_pro_short_d,  //短按延时后发送，配合双击使用
    X1_RX_pro_double,   //双击
};



typedef struct {
    u8 sw;
    u16 pin;
    u8 pin_state;
    u8 plan;
    u8 state;   
    u8 on_E;   
    u16 count;
} LED_indicator;

enum
{
    LED_OFF,
    LED_ON,
};



#define    iLED1        0x1000
#define    iLED2        0x2000
#define    iLED3        0x3000
#define    iLED4        0x4000


#define    iType_only        0x0100     //独立
#define    iType_acco        0x0200     //伴随

enum
{
    LED_plan_off=1,
    LED_plan_on,
    LED_plan_fast_flicker1,
    LED_plan_fast_flicker2,
    LED_plan_slow_flicker1,
    LED_plan_slow_flicker2,
	LED_plan_slow_flicker3,	//500ms
	LED_plan_slow_flicker4,	//1s
    LED_plan_slow_flicker5,//2s
	
    LED_plan_warning1,
    LED_plan_warning2,
    LED_plan_colse,

	LED_plan_breathe,
};



typedef struct {
	unsigned int appMode;
	unsigned char fakeOffFlag;
	unsigned char is_init;
	unsigned char dacxVol;
	unsigned char fristSend;
}S_MY_DEV_INF;









#define KEY_BASE_CNT  4
#define KEY_LONG_CNT  75
#define KEY_HOLD_CNT  15
#define KEY_SHORT_CNT 1

#define KEY_LONG_2S_CNT     5
#define KEY_LONG_3S_CNT     10
#define KEY_LONG_4S_CNT     15
#define KEY_LONG_5S_CNT     20


u32 Get_Mic_Energy(void);
u32 Get_Music_Energy(void);




void Use_Encoder_fun(void);


u8 BOEU_AudioMusicVolDown(void);
u8 BOEU_AudioMusicVolUp(void);
u8 BOEU_AudioMicVolUp(void);
u8 BOEU_AudioMicVolDown(void);
u8 BOEU_MIC_EchoReverb_UP(void);
u8 BOEU_MIC_EchoReverb_DW(void);

u16 BOEU_Get_Line_det_io_val(void);
void BOEU_BT_No_Connection_Timing_Power_Off(void);
void BOEU_BT_Pause_Timing_Power_Off(void);
void BOEU_Media_Pause_Timing_Power_Off(void);

void BOEU_Music_MIC_ECHO_Vol_Init(void);
void TWS_Music_Vol_Sync(u8 music_vol);


void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Read_Byte( u8 para );
void IIC_Send_Byte(u8 byte);

void _1s_fun();

void _1ms_fun();
void _2ms_fun();
void X1_PT_deal(); 
void X1_send_msg(u8 LV_msg);


void IO_contral_init(u16 Pin_io,u8 IOdir,u8 IO_up_en,u8 IO_down_en,u8 Value_out);
void Update_presskey_number();

u8 IO_in_check(u16 Pin_io);

void Io_init();

void ZX_boot_init();

u16 Key_msg_ulteriorly_dispose(u8 lv_type,u8 ZX_key_msg);
u16 key_operation_to_fun(u16 ZX_key_num,u8 ZX_key_operation);

void DEBUG_ZX(u8 Number,char *p,u32 dat2);


u8 Split_3_function(u8 ZX_key_msg);
u8 Split_2_function(u8 ZX_key_msg);
u8 Split_DEBUG_function(u8 ZX_key_msg);


u32 Special_msg_deal();


u8 Scan_IO_key();

u16 KeyScan2();

void power_down_zx();
void power_on_zx();
void ZX_MOS_contral(u16 MOS_pin,u8 MOS_state);

void Save_task_state(u8 LV_state);
void tone_play_end();
void RGB_Contral();
void RGB_Mode_Contral(void);
u8 Get_DAC_Energy_LV_7(void);


void LED_scan_II();

void Z_unified_delay_ms(u16 LV_x);

u16 GET_AD_sampling(u16 Pin_io);


void RGB_Timer_OFF();
void RGB_Timer_ON();

void Z_post_msg(u16 type,u8 msg_num0);
void set_eq_led(unsigned char mode );

void set_mode_led(unsigned int mode);

void main_msg_send(u16 type);
void Disp_msg_send(u16 type);



void User_Set_Music_Energy(u16 DAC_Energy);



void set_music_back(void);

void tws_led_rgb_send(u8 rgb_mode);
void tws_led_light_send(u8 led);
void tws_effect_mode_send(u8 effect_mode);
void tws_echo_send(u8 echo_gain, u8 rever_gain);
void tws_mic_send(u8 mic_vol);

void tws_master_pa_control_send(bool pa_control_flag);
void tws_master_play_flag_send(uint8_t play_flag);
extern u8 aip1668_buff[];

extern u8 led_screen_sw; //开关LED 屏
extern u8 background_light_level;
extern u8 background_light_level_back;
void aip1668_init(void);
void aip1668_deal(void);

void aip1668_deal_test(void);

void buff_data_deal_scan(void);
void set_show_data(u8 buf,u8 data);


void PowerOnBtPairTonePlay(void);




extern idle_sw Idle_sw;


extern Z_Task_inf *Curr_task_inf;


extern Z_Task_inf T_bt_inf;
extern Z_Task_inf T_music_inf;
extern Z_Task_inf T_sd0_inf;
extern Z_Task_inf T_sd1_inf;
extern Z_Task_inf T_usb_inf;
extern Z_Task_inf T_FM_inf;
extern Z_Task_Rec_inf T_rec_inf;

extern Z_Task_inf T_fm_inf;
extern Z_Task_inf T_pc_inf;
extern Z_Task_inf T_idle_inf;
extern Z_Task_inf T_linein1_inf;
extern Z_Task_inf T_TwsSlave_inf;
//extern Z_Task_inf T_linein3_inf;
//extern Z_Task_inf T_EX_linein1_inf;
//extern Z_Task_inf T_EX_linein2_inf;
//extern Z_Task_inf T_EX_linein3_inf;


extern Z_Task_inf T_OPTICAL_inf;
extern Z_Task_inf T_COAXIAL_inf;
extern Z_Task_inf T_HDMI_in_inf;
extern Z_Task_inf T_IIS_in_inf;



extern S_MY_DEV_INF g_t_dev_inf;

extern const _Key_fun_info ZX_key_info[];

#endif




