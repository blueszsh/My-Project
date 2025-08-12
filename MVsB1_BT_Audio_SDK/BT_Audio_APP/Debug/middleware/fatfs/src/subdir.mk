################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middleware/fatfs/src/diskio.c \
../middleware/fatfs/src/ff.c \
../middleware/fatfs/src/ffpresearch.c \
../middleware/fatfs/src/ffsystem.c \
../middleware/fatfs/src/ffunicode.c 

OBJS += \
./middleware/fatfs/src/diskio.o \
./middleware/fatfs/src/ff.o \
./middleware/fatfs/src/ffpresearch.o \
./middleware/fatfs/src/ffsystem.o \
./middleware/fatfs/src/ffunicode.o 

C_DEPS += \
./middleware/fatfs/src/diskio.d \
./middleware/fatfs/src/ff.d \
./middleware/fatfs/src/ffpresearch.d \
./middleware/fatfs/src/ffsystem.d \
./middleware/fatfs/src/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/fatfs/src/%.o: ../middleware/fatfs/src/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： Andes C Compiler'
	$(CROSS_COMPILE)gcc -DFUNC_OS_EN=1 -DUSE_DBG_CODE -DCFG_APP_CONFIG -DHAVE_CONFIG_H -DFUNC_OS_DEBUG -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/system_config" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/flashboot" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/mode_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/bluetooth_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_framework/audio_engine" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/driver/driver_api/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/driver/driver_api/inc/otg" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/driver/driver/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/power" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/rtc/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/fatfs/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/bluetooth/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/audio/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/mv_utils/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/lrc/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/rtos/rtos_api" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/rtos/freertos/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/middleware/flashfs/inc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode__common" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_bt" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_i2s" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_idle" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_linein" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_media" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_radio" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_usb_audio" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/app_mode_spdif" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/audio" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/bluetooth" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/flash_manage" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/rtc" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/upgrade" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/components/soft_watchdog" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/fm" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/detect" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/display" -I"/cygdrive/F/MV-2023/MY-301/V1/MVsB1_BT_Audio_SDK_24bit_v0.4.2/BT_Audio_APP/app_src/hmi/key" -Os1 -mcmodel=medium -g3 -Wall -mcpu=d1088-spu -c -fmessage-length=0 -ldsp -mext-dsp -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


