################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./anf.obj" \
"./anf_cascade.obj" \
"./mainoffline_anf.obj" \
"./mainonline_anf.obj" \
"./USBSTK5515bsl/lib/PLL.obj" \
"./USBSTK5515bsl/lib/aic3204.obj" \
"./USBSTK5515bsl/lib/aic3204_init.obj" \
"./USBSTK5515bsl/lib/bargraph5515.obj" \
"./USBSTK5515bsl/lib/oled5515.obj" \
"./USBSTK5515bsl/lib/pushbuttons5515.obj" \
"./USBSTK5515bsl/lib/stereo.obj" \
"./USBSTK5515bsl/lib/usbstk5505.obj" \
"./USBSTK5515bsl/lib/usbstk5505_gpio.obj" \
"./USBSTK5515bsl/lib/usbstk5505_i2c.obj" \
"./USBSTK5515bsl/lib/usbstk5505_led.obj" \
"./USBSTK5515bsl/lib/usbstk5515.obj" \
"./USBSTK5515bsl/lib/usbstk5515_gpio.obj" \
"./USBSTK5515bsl/lib/usbstk5515_i2c.obj" \
"./USBSTK5515bsl/lib/usbstk5515_led.obj" \
"../lnkx.cmd" \
"../USBSTK5515bsl/lib/55xdsph.lib" \
"../USBSTK5515bsl/lib/CSLc55x5h.lib" \
$(GEN_CMDS__FLAG) \
-l"/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/lib/rts55h.lib" \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include USBSTK5515bsl/lib/subdir_vars.mk
-include subdir_rules.mk
-include USBSTK5515bsl/lib/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ANF.out \

EXE_OUTPUTS__QUOTED += \
"ANF.out" \

BIN_OUTPUTS += \
ANF.hex \

BIN_OUTPUTS__QUOTED += \
"ANF.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ANF.out"

# Tool invocations
ANF.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C5500 Linker'
	"/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -vcpu:3.3 --memory_model=huge -g --define=c5515 --display_error_number --diag_warning=225 --ptrdiff_size=32 -z -m"ANF.map" --stack_size=0x200 --heap_size=0x400 -i"/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/lib" -i"/home/jeffee/git/Realtime_ANF/ANF/USBSTK5515bsl/lib" -i"/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/include" --reread_libs --display_error_number --warn_sections --xml_link_info="ANF_linkInfo.xml" --rom_model --sys_stacksize=0x200 -o "ANF.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

ANF.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C5500 Hex Utility'
	"/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/bin/hex55"  -o "ANF.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "anf.obj" "anf_cascade.obj" "mainoffline_anf.obj" "mainonline_anf.obj" "USBSTK5515bsl/lib/PLL.obj" "USBSTK5515bsl/lib/aic3204.obj" "USBSTK5515bsl/lib/aic3204_init.obj" "USBSTK5515bsl/lib/bargraph5515.obj" "USBSTK5515bsl/lib/oled5515.obj" "USBSTK5515bsl/lib/pushbuttons5515.obj" "USBSTK5515bsl/lib/stereo.obj" "USBSTK5515bsl/lib/usbstk5505.obj" "USBSTK5515bsl/lib/usbstk5505_gpio.obj" "USBSTK5515bsl/lib/usbstk5505_i2c.obj" "USBSTK5515bsl/lib/usbstk5505_led.obj" "USBSTK5515bsl/lib/usbstk5515.obj" "USBSTK5515bsl/lib/usbstk5515_gpio.obj" "USBSTK5515bsl/lib/usbstk5515_i2c.obj" "USBSTK5515bsl/lib/usbstk5515_led.obj" 
	-$(RM) "anf.d" "anf_cascade.d" "mainoffline_anf.d" "mainonline_anf.d" "USBSTK5515bsl/lib/PLL.d" "USBSTK5515bsl/lib/aic3204.d" "USBSTK5515bsl/lib/aic3204_init.d" "USBSTK5515bsl/lib/bargraph5515.d" "USBSTK5515bsl/lib/oled5515.d" "USBSTK5515bsl/lib/pushbuttons5515.d" "USBSTK5515bsl/lib/stereo.d" "USBSTK5515bsl/lib/usbstk5505.d" "USBSTK5515bsl/lib/usbstk5505_gpio.d" "USBSTK5515bsl/lib/usbstk5505_i2c.d" "USBSTK5515bsl/lib/usbstk5505_led.d" "USBSTK5515bsl/lib/usbstk5515.d" "USBSTK5515bsl/lib/usbstk5515_gpio.d" "USBSTK5515bsl/lib/usbstk5515_i2c.d" "USBSTK5515bsl/lib/usbstk5515_led.d" 
	-$(RM) "anf.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

