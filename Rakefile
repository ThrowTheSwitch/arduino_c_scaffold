require 'rake/clean'
require 'fileutils'

# Program Name
PROG = 'scaffold'

# AVR device
MCU = ENV['MCU'] || 'atmega328p' # 'attiny85'
PARTNO = ENV['PARTNO'] || MCU

# Flash burner
PROGRAMMER = ENV['PROGRAMMER'] || 'arduino' # usbasp
SERIAL_PORT = ENV['SERIAL_PORT'] ||  Dir.glob(
      "/dev/tty#{RUBY_PLATFORM =~ /linux/ ? 'USB' : '.usbmodel'}").first

# Arduino Uno 115200 | Duemilanove 57600
BAUDS = ENV['BAUDS'] || 115200 # 57600
F_CPU = ENV['F_CPU'] || '16000000UL'

# Paths
SRCDIR = 'src'
BUILDDIR = 'build'
SRC = FileList["#{SRCDIR}/**/*.c"]

# Create a mapping from objects to source files.
OBJ = SRC.inject({}) do |cont, s|
  obj_file = File.join(BUILDDIR, s.ext('o'))
  cont[obj_file] = s
  cont
end

# Setup what should be cleaned.
CLEAN.include(OBJ.keys, BUILDDIR, "#{PROG}.bin", "#{PROG}.hex")

# Some toolchain info.
TARGET = {
  :compiler => 'avr-gcc',
  :compiler_args => [
    '-DTARGET',
    "-DF_CPU=#{F_CPU}",
    "-mmcu=#{MCU}",
    '-Iinclude/',
    '-Wall',
    '-Os',
    '-c'
  ],
  :linker => 'avr-gcc',
  :linker_args => [
    "-mmcu=#{MCU}"
  ],
  :objcopy => 'avr-objcopy'
}

# Map the default task to the chip programming task
task :default => ['target:program']

namespace :target do
  # Define tasks to make .o files from .c files
  # using the mapping deined in the OBJ hash.
  rule '.o' => lambda { |tn| OBJ[tn] } do |t|
    FileUtils.mkdir_p(File.dirname(t.name))
    cc = TARGET[:compiler]
    args = TARGET[:compiler_args].join(" ")
    sh "#{cc} #{args} -o #{t.name} #{t.source}"
  end

  # Define tasks to make preprocessed sources from
  # c files.
  rule '.e' => lambda { |tn| OBJ[tn.ext('o')] } do |t|
    FileUtils.mkdir_p File.dirname(t.name)
    cc = TARGET[:compiler]
    args = TARGET[:compiler_args].join(" ")
    sh "#{cc} -E #{args} -o #{t.name} #{t.source}"
  end

  desc "Build the project for the Arduino"
  task :build => OBJ.keys

  desc "Generate the preprocessed source files"
  task :preprocess => OBJ.keys.map {|o| o.ext('.e')}

  desc "Link the built project for the Arduino"
  task :link => :build do
    ld = TARGET[:linker]
    args = TARGET[:linker_args].join(" ")
    objs = OBJ.keys.join(" ")
    sh "#{ld} #{args} #{objs} -o #{PROG}.bin"
  end

  desc "Convert the output binary to a hex file."
  task :convert => :link do
    objcopy = TARGET[:objcopy]
    sh "#{objcopy} -O ihex -R .eeprom #{PROG}.bin #{PROG}.hex"
  end

  desc "Program the Arduino over the serial port."
  task :program => [:convert] do
    sh "avrdude -F -V -c #{PROGRAMMER} -p #{PARTNO} #{SERIAL_PORT ? '-P ' + SERIAL_PORT : nil} -b #{BAUDS} -U flash:w:#{PROG}.hex"
  end

  desc "Make a backup hex image of the flash contents."
  task :backup, [:backup_name] do |t, args|
    sh "avrdude -F -V -c #{PROGRAMMER} -p #{PARTNO} #{SERIAL_PORT ? '-P ' + SERIAL_PORT : nil} -b #{BAUDS} -U flash:r:#{args.backup_name}:i"
  end
end
