# stm32examples

For uploading firmware by st-link:

  1. On Linux
    
    1.1 git clone git://github.com/texane/stlink.git
    1.2 cd stlink && make
    1.3 cd build/Release/bin
    1.4 sudo ./st-flash write your-path/xxx.bin 0x08000000

For uploading firmware via Jlink:
  1. On Linux

    1.1 Download JLink utils from the official Jlink site: https://www.segger.com/downloads/jlink/
    1.2 Unpack and install
    1.3 Use JLinkExe util and one from the utils
    1.3.1 JLinkExe -if SWD -speed 1000 -device STM32F401RE
    1.3.2 loadfile xxx.hex / xxx.bin 0x08000000
