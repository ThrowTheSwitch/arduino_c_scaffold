#

## Usage

```bash
$ rake -T # see all tasks
$ rake # build the project, flash to the arduino
$ rake clobber # clean the project
$ rake SERIAL_PORT=/dev/ttyS99999 # use a different serial port
$ rake target:backup[backup.hex] # dump the flash from the target into backup.hex
$ rake target:preprocess # build a preprocessed version of the C files
```

## Board Resources

* ATMega328P datasheet - http://www.atmel.com/dyn/resources/prod_documents/doc8161.pdf
* Arduino UNO Schematic - http://arduino.cc/en/uploads/Main/arduino-uno-schematic.pdf

## Getting Started

The following are instructions for getting up and running under different
operating systems. This will take you through installing avr-gcc, avr-libc,
avrdude, and the AVR GNU Binutils.

### Windows

First, install a recent version of Ruby. You can grab a Windows installer for
Ruby from the [RubyInstaller for Windows][rifw] page. After installing Ruby,
you'll need to open a console and use the ```gem``` command to install the
Cucumber and

Second, you'll need a copy of [WinAVR][WinAVR_DL]. Download the latest version
and install it. You will want to make sure the option to 'Add directories to
PATH' is checked.

WinAVR contains all the tools you need to use this scaffolding including (but
not limited to):

* AVR GNU Binutils
* AVR GNU Compiler Collection (GCC)
* AVRDUDE
* avr-libc

In order to complete the next step, you'll need to download the Arduino
software package this can be found on the [Arduino download page][adp].

After you've installed Ruby and WinAVR, connect the Arduino UNO to your Windows
computer via the USB cable. It will attempt to install a driver, but will
fail. Follow the instructions on [arduino.cc][acc] to complete the
installation of the drivers.

### OSX

I recommend installing the needed AVR tools by means of [Homebrew][hb]. If
you're already setup with homebrew, then you should just have to execute the
following commands. If not, take a moment to read a bit about Homebrew and get
it setup on your Mac.

Once you're ready, here are the commands to install the needed tools:

```bash
$ brew install avrdude
$ brew install https://raw.github.com/larsimmisch/homebrew-alt/3a4f8ce4bcda88c25f4fa4ea3f42688a2ed03d12/avr/avr-binutils.rb
$ brew install https://raw.github.com/larsimmisch/homebrew-alt/124853640317af04e11269c517d449dbd202773d/avr/avr-gcc.rb
$ brew install https://raw.github.com/larsimmisch/homebrew-alt/7d774c6b15dafdee6ca518aff5e8368528d69ae0/avr/avr-libc.rb
```

**These are built from source and will take a while to complete.**

### Linux

Getting the environment up and running on Linux is quite simple. One
need only install some packages. These are all supported in the package
manager. Open a terminal and run the following command:

Debian/Ubuntu

```bash
$ sudo apt-get install binutils-avr avr-libc avrdude gcc-avr
```

Archlinux

```bash
$ sudo pacman -S avrdude gcc-avr
```

## Building and Running the Sample Project

The scaffold is driven by a Rakefile. If you're not familiar with
[Rake][rake], you'll want to take some time to familiarize yourself with it.
It's integral to the operation of this scaffolding.

The different stages of the build process are broken down into different rake
tasks. These can be viewed by running the command ```rake -T``` in the
scaffold directory. You'll see something like this:

    $ rake -T
    rake clean                       # Remove any temporary products.
    rake clobber                     # Remove any generated file.
    rake target:backup[backup_name]  # Make a backup hex image of the flash con...
    rake target:build                # Build the project for the Arduino
    rake target:convert              # Convert the output binary to a hex file ...
    rake target:link                 # Link the built project for the Arduino
    rake target:preprocess           # Generate the preprocessed source files
    rake target:program              # Program the Arduino over the serial port.

The name of the serial port used by the Arduino UNO needs to be passed as an
environment variable to the rake command. If you do not know how to identify
the name of the serial port used by the Arduino UNO, consult with the section
corresponding with your operating system under the 'Identifying the Name of
the Arduino UNO Serial Port' later in this document.

### Building the Scaffold

The scaffold can be built, linked, and flashed by running the following
command:

    rake SERIAL_PORT=[serial port name]

The following is an example of the output one can expect to see when running
the ```rake``` command in a windows environment. In this example, the serial
port is COM3. It may be different in your environment.

    C:\Development\arduino_c_scaffold>rake SERIAL_PORT=COM3
    avr-gcc -DTARGET -DF_CPU=16000000UL -mmcu=atmega328p -Iinclude/ -Wall -Os -c -o build/src/main.o src/main.c
    avr-gcc -mmcu=atmega328p build/src/main.o -o scaffold.bin
    avr-objcopy -O ihex -R .eeprom scaffold.bin scaffold.hex
    avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b 115200 -U flash:w:scaffold.hex

    avrdude: AVR device initialized and ready to accept instructions

    Reading | ################################################## | 100% 0.02s

    avrdude: Device signature = 0x1e950f
    avrdude: NOTE: FLASH memory has been specified, an erase cycle will be performed
             To disable this feature, specify the -D option.
    avrdude: erasing chip
    avrdude: reading input file "scaffold.hex"
    avrdude: input file scaffold.hex auto detected as Intel Hex
    avrdude: writing flash (304 bytes):

    Writing | ################################################## | 100% 0.08s

    avrdude: 304 bytes of flash written

    avrdude: safemode: Fuses OK

    avrdude done.  Thank you.

If the file at ```src/main.c``` hasn't been altered, you should notice that
the yellow surface-mount LED on the Arduino UNO has begun to blink. It should
be cycling on and off with durations of about 1 second.

## Identifying the Name of the Arduino UNO Serial Port

### Windows

TBD

### OSX

TBD

### Linux

Grep for "USB" ttys (virtual serial ports):

    ls /dev/ | grep "USB"

Usually:

    /dev/ttyUSB0


## Exploring the Source Code

TBD

## AVR C Scaffold

This works with others arduinos and barebones AVRs too.
Tested on Duemilanove and the Blackwidow.

Chips ATMEGA328p, ATMEGA32, ATMEGA64 and ATTiny85.
Programmer USBASP.

[WinAVR_DL]: http://sourceforge.net/projects/winavr/files/ "WinAVR Download"
[rifw]: http://rubyinstaller.org/
[acc]: http://arduino.cc/en/Guide/Windows#toc4
[adp]: http://arduino.cc/en/Main/Software
[rake]: http://en.wikipedia.org/wiki/Rake_(software)
[hb]: http://mxcl.github.com/homebrew/
