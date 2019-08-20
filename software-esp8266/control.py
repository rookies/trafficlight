#!/usr/bin/python3
import socket, time, argparse, random, sys

class Sender(object):
    def __init__(self, host, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.hostpair = (host, port)

    def send(self, v1, v2, v3):
        self.sock.sendto(f'{v1},{v2},{v3}'.encode('ascii'), self.hostpair)

class Main(object):
    maxVal = 1023

    def __init__(self):
        parser = argparse.ArgumentParser(description='Control the trafficlight via UDP.')
        parser.add_argument('host', type=str, help='the host to connect to')
        parser.add_argument('port', type=int, help='the port to connect to')
        parser.add_argument('--on', dest='action_on', action='store_true', help='switch all lights on')
        parser.add_argument('--off', dest='action_off', action='store_true', help='switch all lights off')
        parser.add_argument('--trafficlight', dest='action_trafficlight', action='store_true', help='imitate a real traffic light')
        parser.add_argument('--crazy', dest='action_crazy', action='store_true', help='drive everyone crazy')
        parser.add_argument('--set', dest='action_set', action='store', nargs=3, metavar=('R', 'Y', 'G'), help='set the lights to the given brightness values')
        parser.add_argument('--fade', dest='action_fade', action='store_true', help='fade between the lights')
        self.args = parser.parse_args()

        # Check if exactly one action is specified:
        self.action = None
        for k,v in vars(self.args).items():
            if k[:7] == 'action_' and v:
                if self.action is None:
                    self.action = k
                else:
                    sys.stderr.write('error: please specify exactly one action\n\n')
                    parser.print_help()
                    sys.exit(2)
        if self.action is None:
            sys.stderr.write('error: please specify an action\n')
            parser.print_help()
            sys.exit(2)

        print(f'Sending to {self.args.host}:{self.args.port} ...')
        self.sender = Sender(self.args.host, self.args.port)

    def _set(self, v1, v2, v3):
        # TODO: logarithmic mapping
        self.sender.send(v1, v2, v3)

    def action_on(self):
        print('Switching everything on.')
        self._set(*(3*[self.maxVal]))

    def action_off(self):
        print('Switching everything off.')
        self._set(*(3*[0]))

    def action_trafficlight(self):
        print('Entering traffic light mode. Exit with Ctrl+C.')
        try:
            while True:
                self._set(self.maxVal,0,0)
                time.sleep(5)
                self._set(*(2*[self.maxVal]),0)
                time.sleep(1)
                self._set(0,0,self.maxVal)
                time.sleep(5)
                self._set(0,self.maxVal,0)
                time.sleep(1)
        except KeyboardInterrupt:
            self.action_off()

    def action_crazy(self):
        print('Entering crazy mode. Exit with Ctrl+C.')
        try:
            while True:
                self._set(*random.choices([0,self.maxVal], k=3))
                time.sleep(random.uniform(.05,.1))
        except KeyboardInterrupt:
            self.action_off()

    def action_set(self):
        print('Setting lights.')
        self._set(*self.args.action_set)

    def action_fade(self):
        print('Entering fade mode. Exit with Ctrl+C.')
        try:
            # TODO
            pass
        except KeyboardInterrupt:
            self.action_off()

    def __call__(self):
        getattr(self, self.action)()

if __name__ == '__main__':
    main = Main()
    main()
