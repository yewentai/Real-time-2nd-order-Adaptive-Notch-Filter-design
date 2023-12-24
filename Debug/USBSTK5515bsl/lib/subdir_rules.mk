################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
USBSTK5515bsl/lib/%.obj: ../USBSTK5515bsl/lib/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -vcpu:3.3 --memory_model=huge -g --include_path="/home/jeffee/git/Realtime_ANF/ANF" --include_path="/home/jeffee/git/Realtime_ANF/ANF/USBSTK5515bsl/include" --include_path="/home/jeffee/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/include" --define=c5515 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="USBSTK5515bsl/lib/$(basename $(<F)).d_raw" --obj_directory="USBSTK5515bsl/lib" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


