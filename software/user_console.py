import cmd
import sys
import serial

class OuijaShell(cmd.Cmd):
    intro = 'Welcome to the great beyond.  Type help or ? to list commands.\n'
    prompt = '(spook) '
    
    # ----- basic commands -----
    def do_move(self, arg):
        'Move to a location: move x y'
        args = parse(arg)
        if len(args) == 2:
            self.serial_port.write(b"m {0} {1}\n".format(args[0], args[1]))
        line = self.serial_port.readline()
        print line
        
    def do_say(self, arg):
        'Spell out a phrase: say Some phrase here...'
        self.serial_port.write(b"s {0}\n".format(arg))
        line = self.serial_port.readline()
        print line
        
    def do_home(self, arg):
        'Home the control head: home'
        self.serial_port.write(b"h\n")
        line = self.serial_port.readline()
        print line

    def do_quit(self, arg):
        'Shut everything down and stop: quit'
        print('The spirits have said enough')
        return True

    def preloop(self):
        self.serial_port = serial.Serial()
        self.serial_port.baudrate = 9600
        self.serial_port.port = '/dev/ttyUSB0'
        self.serial_port.timeout = 30
        self.serial_port.open()
        if not self.serial_port.is_open:
            print "Could not open serial port"
            return True

def parse(arg):
    'Convert a series of zero or more numbers to an argument tuple'
    return tuple(map(int, arg.split()))

if __name__ == '__main__':
    OuijaShell().cmdloop()