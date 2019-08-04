import cmd
import re
import sys
import serial
from serial.tools import list_ports

class OuijaShell(cmd.Cmd):
    intro = 'Welcome to the great beyond.  Type help or ? to list commands.\n'
    prompt = '(spook) '
    
    # ----- basic commands -----        
    def do_say(self, arg):
        'Spell out a phrase: say Some phrase here...'
        #Cut the string down to all lowercase no punctuation
        letters = re.sub("[^A-Za-z]*", "", arg).lower()
        self.serial_port.write(b"s {0}\n".format(letters))
        line = self.serial_port.readline()
        print line
        
    def do_ans(self, arg):
        'Send the answer: ans [answer, which is all y/n chars]'
        letters = re.sub("[^YNyn]*", "", arg).lower()
        self.serial_port.write(b"a {0}\n".format(letters))
        line = self.serial_port.readline()
        print line

    def do_quit(self, arg):
        'Shut everything down and stop: quit'
        print('The spirits have said enough')
        return True

    def preloop(self):
        #Find the serial ports
        ports = list_ports.grep("ttyUSB")
        try:
            #Get the first port, or fail if there are none
            port = ports.next()
            self.serial_port = serial.Serial()
            self.serial_port.baudrate = 9600
            self.serial_port.port = port.device
            self.serial_port.timeout = 30
            self.serial_port.open()
            if not self.serial_port.is_open:
                print "Could not open serial port {0}".format(port.device)
                return True
        except StopIteration:
            print "No serial ports found"
            return True

def parse(arg):
    'Convert a series of zero or more numbers to an argument tuple'
    return tuple(map(int, arg.split()))

if __name__ == '__main__':
    OuijaShell().cmdloop()