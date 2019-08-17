import cmd
import re
import sys
import serial
from serial.tools import list_ports
from mastermind import check_word


class OuijaShell(cmd.Cmd):
    intro = 'Welcome to the great beyond.  Type help or ? to list commands.\n'
    prompt = '(spook) '
    
    # ----- basic commands -----
    def do_move(self, arg):
        'Move to a location: move x y'
        args = parse(arg)
        if len(args) == 2:
            self.serial_port.write(b"m {0} {1}\n".format(args[0], args[1]))
        else:
            print "Move takes two args, x and y"
            return False
        line = self.serial_port.readline()
        print line
        
    def do_say(self, arg):
        'Spell out a phrase: say Some phrase here...'
        #Cut the string down to all lowercase no punctuation
        letters = re.sub("[^A-Za-z]*", "", arg).lower()
        self.serial_port.write(b"s {0}\n".format(letters))
        line = self.serial_port.readline()
        print line
        
    def do_home(self, arg):
        'Home the control head: home'
        self.serial_port.write(b"h\n")
        line = self.serial_port.readline()
        print line

    def do_yes(self, arg):
        'Move to the "yes" message'
        self.serial_port.write(b"y\n")
        line = self.serial_port.readline()
        print line

    def do_intro(self, arg):
        self.serial_port.write(b"s {0}\n".format("saysevenletterword"))
        line = self.serial_port.readline()
        print line

    def do_no(self, arg):
        'Move to the "no" message'
        self.serial_port.write(b"n\n")
        line = self.serial_port.readline()
        print line

    def do_check(self, arg):
        'Checks a word, which spells the word out and then outputs yes/no code mastermind style: check [target word] [their guess]'
        word, guess = arg.split(" ")
        word = re.sub("[^A-Za-z]*", "", word).lower()
        guess = re.sub("[^A-Za-z]*", "", guess).lower()
        self.serial_port.write(b"s {0}\n".format(guess))
        # Just doing this so it blocks
        line = self.serial_port.readline()
        # TODO make sure the dict is in 
        score = check_word(word, guess)
        for point in score:
            if point == 'y':
                self.serial_port.write(b"y\n")
            else:
                self.serial_port.write(b"n\n")
            #Block until it's done
            line = self.serial_port.readline()
        #Move to the "Ouija" logo
        self.serial_port.write(b"m {0} {1}\n".format(2000, 14500))
        #Block until it's done
        line = self.serial_port.readline()
            
            #TODO if you want a delay, put it here
        
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
