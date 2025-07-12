cmake_minimum_required(VERSION 3.16)

set(TOOLCHAIN_PATH "/home/epsilon/Documents/compilers/riscv-gnu-toolchain/bin/")
set(TOOLCHAIN_PREFIX riscv64-unknown-elf-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(OBJDUMP ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objdump)
set(OBJCOPY ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objcopy)

set(ARCH_FLAGS "-march=rv32ec_zicsr -mabi=ilp32e -ffunction-sections -fdata-sections")
set(WARN_FLAGS "-Wall -Wextra")
set(OPT_FLAGS "-Os")
set(LD_FLAGS "${CPU_FLAGS} -Xlinker -g -nostartfiles -Wl,--gc-sections -Wl,--print-memory-usage -T${CMAKE_CURRENT_SOURCE_DIR}/Link.ld")

set(CMAKE_C_FLAGS "${ARCH_FLAGS} ${OPT_FLAGS} ${WARN_FLAGS}")
SET(CMAKE_ASM_FLAGS "${ARCH_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LD_FLAGS}" CACHE INTERNAL " ${OPT_FLAGS}")

project(firmware)

add_compile_definitions(USE_XTAL_PINS_AS_GPIO)
add_compile_definitions(UART_BOOT_RECEIVE_SERVICE)
add_compile_definitions(UART_COMMANDS_RECEIVE_SERVICE)
add_compile_definitions(USE_PC0_AS_BOOT_SOURCE)
add_compile_definitions(FLASH_CONFIG_SECTION)    


set(TARGET_ELF ${PROJECT_NAME}.elf)
add_executable(${TARGET_ELF} main.c)
enable_language(C ASM)

add_subdirectory(./ch32v003_hal/)
add_subdirectory(./bmsis/)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${OBJDUMP} "-CdSx" $<TARGET_FILE:${TARGET_ELF}> > $<TARGET_FILE:${TARGET_ELF}>.lst
    COMMENT "Generating disassembler file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${OBJCOPY} "-Obinary" $<TARGET_FILE:${TARGET_ELF}> $<TARGET_FILE:${TARGET_ELF}>.bin
    COMMENT "Generating bin file")
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${OBJCOPY} "-Oihex" $<TARGET_FILE:${TARGET_ELF}> $<TARGET_FILE:${TARGET_ELF}>.hex
    COMMENT "Generating hex file")

add_custom_target(flash
        COMMAND openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/wch/wch-riscv.cfg
            -c init
            -c halt
            -c "flash erase_sector wch_riscv 0 last"
            -c "program ${TARGET_ELF} verify"
            -c wlink_reset_resume
            -c exit
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${tgt}
        COMMENT "Running openocd ${tgt_name}"
        USES_TERMINAL)
