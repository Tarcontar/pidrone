set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm-eabi)

set(TRIPLE arm-none-eabi)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER ${TRIPLE}-gcc)
set(CMAKE_CXX_COMPILER ${TRIPLE}-g++)
set(CMAKE_ASM_COMPILER ${TRIPLE}-as)
set(CMAKE_OBJCOPY ${TRIPLE}-objcopy)
set(CMAKE_OBJDUMP ${TRIPLE}-objdump)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

set(SHARED_FLAGS "-specs=nano.specs -specs=nosys.specs -mthumb -mcpu=cortex-m3 -msoft-float -mfix-cortex-m3-ldrd")
set(CMAKE_C_FLAGS "${SHARED_FLAGS} -Os -g -Wall")
set(CMAKE_CXX_FLAGS "${SHARED_FLAGS} -fno-builtin -Wall")
set(CMAKE_ASM_FLAGS "${SHARED_FLAGS} -g -Wa,--no-warn -x assembler-with-cpp")
