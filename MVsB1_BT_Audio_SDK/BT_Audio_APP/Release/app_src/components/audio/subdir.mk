################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app_src/components/audio/audio_effect.c \
../app_src/components/audio/audio_effect_flash_param.c \
../app_src/components/audio/audio_effect_param.c \
../app_src/components/audio/audio_effect_process.c \
../app_src/components/audio/audio_effect_user.c \
../app_src/components/audio/audio_vol.c \
../app_src/components/audio/comm_param.c \
../app_src/components/audio/ctrlvars.c \
../app_src/components/audio/eq_params.c 

OBJS += \
./app_src/components/audio/audio_effect.o \
./app_src/components/audio/audio_effect_flash_param.o \
./app_src/components/audio/audio_effect_param.o \
./app_src/components/audio/audio_effect_process.o \
./app_src/components/audio/audio_effect_user.o \
./app_src/components/audio/audio_vol.o \
./app_src/components/audio/comm_param.o \
./app_src/components/audio/ctrlvars.o \
./app_src/components/audio/eq_params.o 

C_DEPS += \
./app_src/components/audio/audio_effect.d \
./app_src/components/audio/audio_effect_flash_param.d \
./app_src/components/audio/audio_effect_param.d \
./app_src/components/audio/audio_effect_process.d \
./app_src/components/audio/audio_effect_user.d \
./app_src/components/audio/audio_vol.d \
./app_src/components/audio/comm_param.d \
./app_src/components/audio/ctrlvars.d \
./app_src/components/audio/eq_params.d 


# Each subdirectory must supply rules for building sources it contributes
app_src/components/audio/%.o: ../app_src/components/audio/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： Andes C Compiler'
	$(CROSS_COMPILE)gcc -DFUNC_OS_EN=1 -DCFG_APP_CONFIG -DHAVE_CONFIG_H -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/system_config" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_hdmi" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/cec/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/slow_device_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/flashboot" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/mode_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/bluetooth_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/audio_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/driver/driver_api/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/driver/driver_api/inc/otg" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/driver/driver/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/power" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/rtc/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/fatfs/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/bluetooth/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/audio/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/mv_utils/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/lrc/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/rtos/rtos_api" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/rtos/freertos/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/flashfs/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode__common" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_bt" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_i2s" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_idle" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_linein" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_media" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_radio" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_usb_audio" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/audio" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/bluetooth" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/flash_manage" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/rtc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/upgrade" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/fm" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/detect" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/display" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/key" -Os1 -mcmodel=medium -g3 -Wall -mcpu=d1088-spu -c -fmessage-length=0 -ldsp -mext-dsp -std=gnu99 -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


