# Toolchain file for STM32 (ARM Cortex-M4)
# Portable: works on Windows (MSYS2) and Linux

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    # Do not modify PATH globally; document expected environment instead.
    # Set MSYS2 bin directory in PATH before running cmake:
    #   set PATH=C:\msys64\mingw64\bin;%PATH%
    set(TOOLCHAIN_PREFIX "C:/msys64/mingw64/bin/arm-none-eabi-")
    set(TOOLCHAIN_SUFFIX ".exe")
else()
    set(TOOLCHAIN_PREFIX "arm-none-eabi-")
    set(TOOLCHAIN_SUFFIX "")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_SUFFIX}")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++${TOOLCHAIN_SUFFIX}")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_SUFFIX}")

# Flags pour Cortex-M4 (STM32F4)
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
set(CMAKE_ASM_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -x assembler-with-cpp")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)