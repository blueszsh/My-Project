/**
 **************************************************************************************
 * @file    audio_effect_param.h
 * @brief
 *
 * @author  Sam
 * @version V1.0.0
 *
 * &copy; Shanghai Mountain View Silicon Technology Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
 
#ifndef __AUDIO_EFFECT_PARAM_H__
#define __AUDIO_EFFECT_PARAM_H__
 
#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include "type.h"
#include "audio_effect_api.h"

typedef struct _AudioEffectCommParamBuffContext
{
	GainControlParam		paramGainControl_0;
	GainControlParam		paramGainControl_1;
	GainControlParam		paramGainControl_2;
	GainControlParam		paramGainControl_3;
	GainControlParam		paramGainControl_4;
	GainControlParam		paramGainControl_5;
	ExpanderParam		paramExpander_6;
	SilenceDetectorParam		paramSilenceDetector_7;
	CompanderParam		paramCompander_8;
	LLCompressorParam		paramLLCompressor_9;
	VBParam		paramVB_10;
	VBClassicParam		paramVBClassic_11;
	ThreeDParam		paramThreeD_12;
	ExciterParam		paramExciter_13;
	StereoWidenerParam		paramStereoWidener_14;
	EQDRCParam		paramEQDRC_15;
	EQParam		paramEQ_16;
	LLCompressorParam		paramLLCompressor_17;
	PhaseControlParam		paramPhaseControl_18;
	VBParam		paramVB_19;
	VBClassicParam		paramVBClassic_20;
	EQDRCParam		paramEQDRC_21;
	EQParam		paramEQ_22;
}AudioEffectCommParamBuffContext;

typedef struct _AudioEffectAECParam_HFPBuffContext
{
	ExpanderParam		paramExpander_0;
	EQParam		paramEQ_1;
	AECParam		paramAEC_2;
	GainControlParam		paramGainControl_3;
	DRCParam		paramDRC_4;
}AudioEffectAECParam_HFPBuffContext;


#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AUDIO_EFFECT_PARAM_H__

