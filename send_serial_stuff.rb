
require 'serialport'
sp = SerialPort.new "/dev/tty.usbmodemfd121", 115200
sleep 2.0
sp.write "This is a test\n"
# we should get back an echo here (if everything is working)
puts sp.readline
