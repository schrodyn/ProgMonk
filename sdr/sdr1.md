# Setup HackRF Environment

1. Download **GNU Radio**

As wikipedia says: **GNU Radio** is a free software development 
toolkit that provides signal processing blocks to implement 
software-defined radios and signal-processing systems.

on Arch: 

```
pacman -S gnuradio gnuradio-companion 
```

now don't install **gnuradio-osmosdr** package, we will compile
everything from git:

First of all, we clone **hackrf** repository:

```
git clone https://github.com/mossmann/hackrf.git
```

Now compile host **hackrf-tools**:

```
cd hackrf/host/
mkdir build && cd build
cmake ../ -DINSTALL_UDEV_RULES=ON
make
sudo make install
sudo ldconfig
```

This stuff will install **hackrf-tools** and **libhackrf**

Now let's compile HackRF firmware:

```
cd hackrf/firmware
```

you should read README for more instructions, in my case
I want to compile firmware for HackRF Jawbreaker:

## HackRF Jawbreaker

1\. Install gcc arm:

```
pacman -S arm-none-eabi-gcc
```

2\. Install a C standrad library for arm

```
pacman -S arm-none-eabi-newlib
```

3\. Firmware Required dependency:

https://github.com/mossmann/libopencm3

If you are using git, the preferred way to install libopencm3 is to 
use the submodule:

```
$ cd ..
$ git submodule init
$ git submodule update
$ cd firmware/libopencm3
$ make
$ sudo make install
```

4\. Let's build a standard firmware image for HackRF Jawbreaker:


```
$ cd firmware/hackrf_usb
$ mkdir build
$ cd build
$ cmake .. -DBOARD=JAWBREAKER
$ make
```

If you have a HackRF One, use `-DBOARD=HACKRF_ONE` instead.

You may encounter a lot of errors like this if you compile for
HackRF Jawbreaker:

```
hackrf_core.c:289:2: error: unknown field 'gpio_hp' specified in initializer
  .gpio_hp = &gpio_hp,
  ^
```

Okay, I've found source code for this part:

```c
rf_path_t rf_path = {
	.switchctrl = 0,
	.gpio_hp = &gpio_hp,
	.gpio_lp = &gpio_lp,
	.gpio_tx_mix_bp = &gpio_tx_mix_bp,
	.gpio_no_mix_bypass = &gpio_no_mix_bypass,
	.gpio_rx_mix_bp = &gpio_rx_mix_bp,
	.gpio_tx_amp = &gpio_tx_amp,
	.gpio_tx = &gpio_tx,
	.gpio_mix_bypass = &gpio_mix_bypass,
	.gpio_rx = &gpio_rx,
	.gpio_no_tx_amp_pwr = &gpio_no_tx_amp_pwr,
	.gpio_amp_bypass = &gpio_amp_bypass,
	.gpio_rx_amp = &gpio_rx_amp,
	.gpio_no_rx_amp_pwr = &gpio_no_rx_amp_pwr,
};
```

we need search for **rf_path_t** struct. It is located in 
**rf_path.h** header:

```c
typedef struct rf_path_t {
	uint8_t switchctrl;
#ifdef HACKRF_ONE
	gpio_t gpio_hp;
	gpio_t gpio_lp;
	gpio_t gpio_tx_mix_bp;
	gpio_t gpio_no_mix_bypass;
	gpio_t gpio_rx_mix_bp;
	gpio_t gpio_tx_amp;
	gpio_t gpio_tx;
	gpio_t gpio_mix_bypass;
	gpio_t gpio_rx;
	gpio_t gpio_no_tx_amp_pwr;
	gpio_t gpio_amp_bypass;
	gpio_t gpio_rx_amp;
	gpio_t gpio_no_rx_amp_pwr;
#endif
} rf_path_t;
```

This means developers forgot to add **ifdef** to the definition
of **rf_path**. Let's add it and recompile:

```c
rf_path_t rf_path = {
	.switchctrl = 0,
#ifdef HACKRF_ONE
	.gpio_hp = &gpio_hp,
	.gpio_lp = &gpio_lp,
	.gpio_tx_mix_bp = &gpio_tx_mix_bp,
	.gpio_no_mix_bypass = &gpio_no_mix_bypass,
	.gpio_rx_mix_bp = &gpio_rx_mix_bp,
	.gpio_tx_amp = &gpio_tx_amp,
	.gpio_tx = &gpio_tx,
	.gpio_mix_bypass = &gpio_mix_bypass,
	.gpio_rx = &gpio_rx,
	.gpio_no_tx_amp_pwr = &gpio_no_tx_amp_pwr,
	.gpio_amp_bypass = &gpio_amp_bypass,
	.gpio_rx_amp = &gpio_rx_amp,
	.gpio_no_rx_amp_pwr = &gpio_no_rx_amp_pwr,
#endif
};
```

Now everything is ok

5\. Now it's time to upload our compiled firmware on the board:

```
hackrf_spiflash -w hackrf_usb.bin
```

This time I got the error:

```
File size 19724 bytes.
hackrf_open() failed: HACKRF_ERROR_NOT_FOUND (-5)
```

and also

```
$ hackrf_info 
No HackRF boards found.
```

Okay, I've found issue in my USB cable, so try another one, you
should see three luminous leds (instead of two in my case)

After that i've found another error:

```
$ hackrf_info 
Found HackRF board 0:
hackrf_open() failed: HACKRF_ERROR_LIBUSB (-1000)
```

You should run this command as root:

```
$ sudo hackrf_info 
Found HackRF board 0:
Board ID Number: 1 (Jawbreaker)
Firmware Version: git-755a9f6
Part ID Number: 0xa0000a30 0xbc6b4753
Serial Number: 0x00000000 0x00000000 0x066062c8 0x39196f87
```

```
$ sudo hackrf_spiflash -w hackrf_usb.bin
File size 19724 bytes.
Erasing SPI flash.
Writing 19724 bytes at 0x000000.
```

Now it's succeeded.


