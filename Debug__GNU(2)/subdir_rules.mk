################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"D:/CCS/ccs/tools/compiler/msp430-gcc-8.3.1.25_win64/bin/msp430-elf-gcc-8.3.1.exe" -c -mmcu=msp430g2553 -mhwmult=none -I"D:/CCS/ccs/ccs_base/msp430/include_gcc" -I"D:/msp430/msp430-radio/msp430-radio/src" -I"D:/CCS/ccs/tools/compiler/msp430-gcc-8.3.1.25_win64/msp430-elf/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


