cmake_minimum_required(VERSION 3.14)

# Option de cross-compilation (peut être passée via -DCROSS_COMPILE=ON)
option(CROSS_COMPILE "Build for STM32 (ARM)" OFF)

# Charger le toolchain ARM AVANT project() si cross-compilation
if(CROSS_COMPILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/platform/stm32/toolchain-arm.cmake" CACHE FILEPATH "Toolchain file")
    include("${CMAKE_CURRENT_SOURCE_DIR}/platform/stm32/toolchain-arm.cmake")
endif()

project(EmbeddedSense C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
if(NOT CROSS_COMPILE)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Google Test - seulement pour la build PC
if(NOT CROSS_COMPILE)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

# --- Core library (platform-independent) ---
set(CORE_SOURCES
    app/acquisition/SensorManager.cpp
    app/control/DormantState.cpp
    app/control/SurveillanceState.cpp
    app/control/AlerteState.cpp
    app/control/ErreurState.cpp
    app/control/StateMachine.cpp
    services/logger/Logger.cpp
)

set(MOCK_DRIVER_SOURCES
    drivers/mock/MockTemperatureSensor.cpp
    drivers/mock/MockHumiditySensor.cpp
    drivers/mock/MockPressureSensor.cpp
)

# --- Cross-compilation STM32 ---
if(CROSS_COMPILE)
    add_executable(embeddedsense_firmware
        app/main_stm32.cpp
        platform/stm32/startup_stm32f4xx.s
    )
    target_link_options(embeddedsense_firmware PRIVATE
        -T${CMAKE_CURRENT_SOURCE_DIR}/platform/stm32/stm32f4xx.ld
        -nostdlib
        -e Reset_Handler
    )
    set_target_properties(embeddedsense_firmware PROPERTIES SUFFIX ".elf")

    find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
    if(ARM_OBJCOPY)
        add_custom_command(TARGET embeddedsense_firmware POST_BUILD
            COMMAND ${ARM_OBJCOPY} -O binary $<TARGET_FILE:embeddedsense_firmware> ${CMAKE_BINARY_DIR}/embeddedsense_firmware.bin
            COMMENT "Generating .bin file"
        )
    endif()
else()
    # --- Core library ---
    add_library(embeddedsense_core STATIC ${CORE_SOURCES})
    target_include_directories(embeddedsense_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # --- Host platform library ---
    add_library(embeddedsense_platform_host STATIC
        platform/host/HostTask.cpp
        platform/host/HostMutex.cpp
        platform/host/HostTimer.cpp
        platform/stm32/FreeRTOSCompat.cpp
        app/acquisition/AcquisitionTask.cpp
        app/communication/MockUART.cpp
        app/communication/CommunicationManager.cpp
        app/communication/CommunicationTask.cpp
        app/processing/SharedData.cpp
        app/processing/ProcessingTask.cpp
    )
    target_include_directories(embeddedsense_platform_host PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(embeddedsense_platform_host PUBLIC embeddedsense_core pthread)

    # --- Mock drivers ---
    add_library(embeddedsense_drivers_mock STATIC ${MOCK_DRIVER_SOURCES})
    target_include_directories(embeddedsense_drivers_mock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(embeddedsense_drivers_mock PUBLIC embeddedsense_core)

    # --- Demo application ---
    add_executable(embeddedsense_demo app/main.cpp)
    target_link_libraries(embeddedsense_demo
        embeddedsense_core
        embeddedsense_drivers_mock
    )

    # --- Tests ---
    add_executable(embeddedsense_tests
        tests/test_main.cpp
        tests/test_sensors.cpp
        tests/test_acquisition.cpp
        tests/test_state_machine.cpp
        tests/test_logging.cpp
        tests/test_threading.cpp
        tests/test_freertos_compat.cpp
        tests/test_integration.cpp
        tests/test_robustness.cpp
    )
    target_link_libraries(embeddedsense_tests
        embeddedsense_core
        embeddedsense_drivers_mock
        embeddedsense_platform_host
        gtest_main
    )
    include(GoogleTest)
    gtest_discover_tests(embeddedsense_tests)
endif()