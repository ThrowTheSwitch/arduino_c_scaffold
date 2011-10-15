require 'rake/clean'
require 'fileutils'

SERIAL_PORT = ENV['SERIAL_PORT']
PROG = 'scaffold'
SRCDIR = 'src'
BUILDDIR = 'build'
SRC = FileList["#{SRCDIR}/**/*.c"]

# Create a mapping from objects
# to source files.
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
    '-DF_CPU=16000000UL',
    '-mmcu=atmega328p',
    '-Iinclude/',
    '-Wall',
    '-Os',
    '-c'
  ],
  :linker => 'avr-gcc',
  :linker_args => [
    '-mmcu=atmega328p'
  ],
  :objcopy => 'avr-objcopy'
}

# Map the default task to the chip programming task
task :default => ['target:program']

# Dummy task to ensure that the SERIAL_PORT environment variable is set.
# It can be set on the command line as follows:
#   $ rake SERIAL_PORT=[serial port name]
task :serial_port do
  unless ENV['SERIAL_PORT']
    raise "SERIAL_PORT is not defined in the environment!"
  end
end

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

  desc "Convert the output binary to a hex file for programming to the Arduino"
  task :convert => :link do
    objcopy = TARGET[:objcopy]
    sh "#{objcopy} -O ihex -R .eeprom #{PROG}.bin #{PROG}.hex"
  end

  desc "Program the Arduino over the serial port."
  task :program => [:convert, :serial_port] do
    sh "avrdude -F -V -c arduino -p ATMEGA328P -P #{SERIAL_PORT} -b 115200 -U flash:w:#{PROG}.hex"
  end

  desc "Make a backup hex image of the flash contents."
  task :backup, [:backup_name] => :serial_port do |t, args|
    sh "avrdude -F -V -c arduino -p ATMEGA328P -P #{SERIAL_PORT} -b 115200 -U flash:r:#{args.backup_name}:i"
  end
end
