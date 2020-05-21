cmake_minimum_required(VERSION 3.10)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

project(first_hello_world C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

######## CHIP CONFIGURATION ##########
set(ROOT_PROJ   ${CMAKE_CURRENT_SOURCE_DIR})
set(CPU         "cortex-m4")
set(ARCH_NAME   "arm")
set(ARCH_VER    "v7e-m") ## TO BE CHECKED
set(FAMILY      "stm32f3")
set(CHIP        "STM32F301x6")
set(ARCH        "${ARCH_NAME}${ARCH_VER}")
######################################

# MCU Config
set(FPU "-mfpu=fpv4-sp-d16")
set(FLOAT-ABI "-mfloat-abi=hard")

# Toolchain path
set(TOOLCHAIN_PATH  "/home/mehdi/ARMToolChain/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux/gcc-arm-none-eabi-9-2019-q4-major/bin")

# Specify C, C++ and ASM compilers
SET(CMAKE_C_COMPILER    ${TOOLCHAIN_PATH}/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PATH}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN_PATH}/arm-none-eabi-as)
set(AR                  ${TOOLCHAIN_PATH}/arm-none-eabi-ar)
set(OBJCOPY             ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy)
set(OBJDUMP             ${TOOLCHAIN_PATH}/arm-none-eabi-objdump)
set(SIZE                ${TOOLCHAIN_PATH}/arm-none-eabi-size)
set(GDB                 ${TOOLCHAIN_PATH}/arm-none-eabi-gdb)
set(SIZE                ${TOOLCHAIN_PATH}/arm-none-eabi-size)

# Definitions passed at compile time (#defines)
add_definitions(-DFAMILY=${FAMILY})
add_definitions(-DCHIP=${CHIP})
add_definitions(-D${CHIP})
add_definitions(-DUSE_FULL_LL_DRIVER)
add_definitions(-USE_HAL_DRIVER)
add_definitions(-DHSE_VALUE=8000000)
add_definitions(-DHSE_STARTUP_TIMEOUT=100)
add_definitions(-DLSE_STARTUP_TIMEOUT=5000)
add_definitions(-DLSE_VALUE=32768)
add_definitions(-DHSI_VALUE=8000000)
add_definitions(-DLSI_VALUE=40000)
add_definitions(-DDD_VALUE=3300)
add_definitions(-DPREFETCH_ENABLE=1)

# Compilation flags
add_compile_options(-mcpu=${CPU})
add_compile_options(-march=${ARCH})
add_compile_options(-mthumb)
add_compile_options(${FPU})
add_compile_options(${FLOAT_ABI})
add_compile_options(-Og)
add_compile_options(-Wall)
add_compile_options(-fdata-sections)
add_compile_options(-ffunction-sections)
# Only for debugging
add_compile_options(-g -gdwarf-2)


# Linker script path
file(GLOB_RECURSE LINKER_SCRIPT ${ROOT_PROJ}/*.ld)

# Variables initialized first time
SET(CMAKE_CXX_FLAGS_INIT "-std=c++11")
SET(CMAKE_C_FLAGS_INIT "-std=gnu99")

################################## Source code ###############################################################

file(GLOB SOURCES  "helloWorld.cpp")

################################## Source code END ###########################################################

set(EXE_NAME "${PROJECT_NAME}_${CHIP}")
add_executable(${EXE_NAME}.elf "${SOURCES}" "${LINKER_SCRIPT}")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=${CPU} -mthumb ${FPU} ${FLOAT_ABI} --specs=nano.specs -T${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections")

# Libs and external dependencies
target_link_libraries(${EXE_NAME}.elf -lc -lm -lnosys)

# Outputs
set(ELF_FILE ${PROJECT_BINARY_DIR}/${EXE_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${EXE_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${EXE_NAME}.bin)

add_custom_command(TARGET "${EXE_NAME}.elf" POST_BUILD
        # Build .hex and .bin files
        COMMAND ${OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
        COMMAND ${OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
        COMMENT "Building ${PROJECT_NAME}.bin and ${PROJECT_NAME}.hex"

        # Copy files to a custom build directory
        COMMAND ${CMAKE_COMMAND} -E copy ${ELF_FILE} "${ROOT_PROJ}/builds/${CHIP}/${EXE_NAME}.elf"
        COMMAND ${CMAKE_COMMAND} -E copy ${HEX_FILE} "${ROOT_PROJ}/builds/${CHIP}/${EXE_NAME}.hex"
        COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} "${ROOT_PROJ}/builds/${CHIP}/${EXE_NAME}.bin"

        # Display sizes
        COMMAND ${SIZE} --format=berkeley ${EXE_NAME}.elf ${EXE_NAME}.hex
        COMMENT "Invoking: Cross ARM GNU Print Size"
        )

#add_custom_target(UPLOAD
#        ${GDB} -iex "target remote tcp:127.0.0.1:3333"
#        -iex "monitor program ${EXE_NAME}.elf"
#        -iex "monitor reset init"
#        -iex "disconnect" -iex "quit ")

