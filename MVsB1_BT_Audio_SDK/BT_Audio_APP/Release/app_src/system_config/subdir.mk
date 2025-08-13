################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app_src/system_config/Dma_Led.c \
../app_src/system_config/iic.c \
../app_src/system_config/qn8027.c \
../app_src/system_config/zx.c 

OBJS += \
./app_src/system_config/Dma_Led.o \
./app_src/system_config/iic.o \
./app_src/system_config/qn8027.o \
./app_src/system_config/zx.o 

C_DEPS += \
./app_src/system_config/Dma_Led.d \
./app_src/system_config/iic.d \
./app_src/system_config/qn8027.d \
./app_src/system_config/zx.d 


# Each subdirectory must supply rules for building sources it contributes
app_src/system_config/%.o: ../app_src/system_config/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： Andes C Compiler'
	$(CROSS_COMPILE)gcc -DFUNC_OS_EN=1 -DCFG_APP_CONFIG -DHAVE_CONFIG_H -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/cec/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_hdmi" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/audio" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/soft_watchdog" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_spdif" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_framework/slow_device_engine" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode__common" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_bt" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_i2s" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_idle" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_linein" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_media" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_radio" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/app_mode_usb_audio" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/bluetooth" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/ble" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/flash_manage" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/rtc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/components/upgrade" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/hmi/detect" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/hmi/display" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/hmi/fm" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/hmi/key" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/system_config" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/flashboot" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_framework/mode_engine" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_framework/bluetooth_engine" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_framework/audio_engine" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/driver/driver_api/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/driver/driver_api/inc/otg" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/driver/driver/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/app_src/power" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/rtc/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/fatfs/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/bluetooth/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/audio/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/mv_utils/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/lrc/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/rtos/rtos_api" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/rtos/freertos/inc" -I"/cygdrive/F/My-Project/MVsB1_BT_Audio_SDK/BT_Audio_APP/middleware/flashfs/inc" -Os1 -mcmodel=medium -g3 -Wall -mcpu=d1088-spu -c -fmessage-length=0 -ldsp -mext-dsp -std=gnu99 -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


