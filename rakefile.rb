require 'rake/clean'

SERIAL_PORT = '/dev/tty.usbmodemfd121'
PROG = 'scaffold'
SRCDIR = 'src'
BUILDDIR = 'build'
SRC = FileList["#{SRCDIR}/**/*.c"]
OBJ = SRC.collect { |fn| File.join(BUILDDIR, fn.ext('o')) }

CLEAN.include(OBJ, BUILDDIR, PROG, FileList["#{BUILDDIR}/**/*.o"])

TARGET = {
	:compiler => 'avr-gcc',
	:linker   => 'avr-gcc',
	:c_args   => [
		'-DTARGET',
		'-DF_CPU=16000000UL',
		'-mmcu=atmega328p',
		'-Wall',
		'-Os',
		'-c'
	],
	:l_args   => [
		'-mmcu=atmega328p'
	]
}
HOST = {
	:compiler => 'gcc',
	:linker   => 'gcc',
	:c_args   => [
		'-DHOST',
		'-Wall',
		'-O',
		'-c'
	]
}

task :default => [:release]
task :release => ['target:program']

namespace :target do
	desc "Build the project for the Arduino"
	task :build do
		puts SRC.inspect
		puts OBJ.inspect
	end

	desc "Link the built project for the Arduino"
	task :link => :build do
		args = TARGET[:l_args].join(" ")
		objs = "oops.o"
		sh "avr-gcc #{args} #{objs} -o #{PROG}.bin"
	end

	desc "Convert the output binary to a hex file for programming to the Arduino"
	task :convert => :link do
		sh "avr-objcopy -O ihex -R .eeprom #{PROG}.bin #{PROG}.hex"
	end

	desc "Program the Arduino over the serial port."
	task :program => :convert do
		sh "avrdude -F -V -c arduino -p ATMEGA328P -P #{SERIAL_PORT} -b 115200 -U flash:w:#{PROG}.hex"
	end
end
