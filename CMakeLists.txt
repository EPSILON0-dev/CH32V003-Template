cmake_minimum_required(VERSION 3.13)

# Project name variable
set(PROJECT template)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
project(${PROJECT} C CXX ASM)

# Configurable compiler path and prefix
set(COMPILER_PATH "" CACHE PATH "Path to the compiler binaries")
set(COMPILER_PREFIX "" CACHE STRING "Compiler prefix (e.g. riscv-none-embed-)")

# Set compiler executables if COMPILER_PATH and COMPILER_PREFIX are set
if(COMPILER_PATH AND COMPILER_PREFIX)
  set(CMAKE_C_COMPILER ${COMPILER_PATH}/${COMPILER_PREFIX}gcc)
  set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/${COMPILER_PREFIX}g++)
  set(CMAKE_ASM_COMPILER ${COMPILER_PATH}/${COMPILER_PREFIX}gcc)
  set(CMAKE_OBJDUMP ${COMPILER_PATH}/${COMPILER_PREFIX}objdump)
  set(CMAKE_SIZE ${COMPILER_PATH}/${COMPILER_PREFIX}size)
  set(CMAKE_OBJCOPY ${COMPILER_PATH}/${COMPILER_PREFIX}objcopy)
endif()

# Set compiler flags (adjust as needed)
set(CMAKE_C_FLAGS "-march=rv32ec_zicsr -mabi=ilp32e -O2 -ffunction-sections -fdata-sections -Wall -nostdlib -nostartfiles")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti -nostartfiles")
set(CMAKE_ASM_FLAGS "-march=rv32ec_zicsr -mabi=ilp32e -nostartfiles")

# Include directories for sdk and user source
include_directories(${SDK_HEADERS} sdk/inc src)

# Collect sdk source files (C and assembly)
file(GLOB SDK_HEADERS "sdk/inc/*.h")
file(GLOB SDK_C_SOURCES "sdk/src/*.c")
file(GLOB SDK_ASM_SOURCES "sdk/src/*.S")

# Create static library from sdk sources
add_library(sdk STATIC ${SDK_C_SOURCES} ${SDK_ASM_SOURCES})

# Collect src source files (C, C++, assembly)
file(GLOB SRC_C_SOURCES "src/*.c")
file(GLOB SRC_CPP_SOURCES "src/*.cpp")
file(GLOB SRC_ASM_SOURCES "src/*.S")

# Add executable from src sources
add_executable(${PROJECT} ${SRC_C_SOURCES} ${SRC_CPP_SOURCES} ${SRC_ASM_SOURCES})

# Link sdk static library
target_link_libraries(${PROJECT} sdk c gcc)

# Linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/sdk/link.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} -Wl,--print-memory-usage -Wl,--gc-sections")

# Post build commands for hexdump, objdump, and size
add_custom_command(TARGET ${PROJECT} POST_BUILD
  COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${PROJECT}> > ${CMAKE_BINARY_DIR}/${PROJECT}.objdump
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT}> ${CMAKE_BINARY_DIR}/${PROJECT}.bin
)
