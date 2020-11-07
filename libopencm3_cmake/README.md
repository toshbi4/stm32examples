# stm32f103-libopencm3-exti

Examples for libopencm3 with cmake

Don't forget to upload submodules by command:

git submodule update --init --recursive

Building:

  mkdir build

  cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/cortex-m3.cmake

  make libopencm3

  make
