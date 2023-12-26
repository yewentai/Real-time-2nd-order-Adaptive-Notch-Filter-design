################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -vcpu:3.3 --memory_model=huge -g --include_path="C:/Users/yewentai/Documents/Lab_SPAI/Final_Lab" --include_path="C:/Users/yewentai/Documents/Lab_SPAI/Final_Lab/include" --include_path="C:/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/include" --define=c5515 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -vcpu:3.3 --memory_model=huge -g --include_path="C:/Users/yewentai/Documents/Lab_SPAI/Final_Lab" --include_path="C:/Users/yewentai/Documents/Lab_SPAI/Final_Lab/include" --include_path="C:/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/include" --define=c5515 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


