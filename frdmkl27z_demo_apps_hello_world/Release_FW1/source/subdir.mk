################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/hello_world.c 

OBJS += \
./source/hello_world.o 

C_DEPS += \
./source/hello_world.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DFW_NUM=1 -DDEBUG -DPRINTF_FLOAT_ENABLE=0 -DSCANF_FLOAT_ENABLE=0 -DPRINTF_ADVANCED_ENABLE=0 -DSCANF_ADVANCED_ENABLE=0 -DFRDM_KL27Z -DFREEDOM -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -D__REDLIB__ -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world\source" -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world" -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world\drivers" -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world\utilities" -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world\CMSIS" -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world\board" -I"E:\testing\mcux_proj\frdmkl27z_demo_apps_hello_world\startup" -Os -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


