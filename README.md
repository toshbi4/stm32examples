# stm32examples

For uploading firmware by st-link:
  1. On Linux
    1.1 git clone git://github.com/texane/stlink.git
    1.2 cd stlink && make
    1.3 cd build/Release/bin
    1.4 sudo ./st-flash write your-path/xxx.bin 0x08000000
