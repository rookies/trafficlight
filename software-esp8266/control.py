#!/usr/bin/python3
import socket, time, argparse, random

class Sender(object):
    def __init__(self, host, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.hostpair = (host, port)

    def send(self, v1, v2, v3):
        self.sock.sendto(f'{v1},{v2},{v3}'.encode('ascii'), self.hostpair)

class Main(object):
    maxVal = 1023
    actions = ['on','off','trafficlight','crazy']

    def __init__(self):
        parser = argparse.ArgumentParser(description='Control the trafficlight via UDP.')
        parser.add_argument('host', type=str, help='the host to connect to')
        parser.add_argument('port', type=int, help='the port to connect to')
        parser.add_argument('action', choices=self.actions, help='the action to execute')
        self.args = parser.parse_args()

        print(f'Sending to {self.args.host}:{self.args.port} ...')
        self.sender = Sender(self.args.host, self.args.port)

    def _on(self):
        self.sender.send(*(3*[self.maxVal]))

    def _off(self):
        self.sender.send(*(3*[0]))

    def _trafficlight(self):
        try:
            while True:
                self.sender.send(self.maxVal,0,0)
                time.sleep(5)
                self.sender.send(*(2*[self.maxVal]),0)
                time.sleep(1)
                self.sender.send(0,0,self.maxVal)
                time.sleep(5)
                self.sender.send(0,self.maxVal,0)
                time.sleep(1)
        except KeyboardInterrupt:
            self._off()

    def _crazy(self):
        try:
            while True:
                self.sender.send(*random.choices([0,self.maxVal], k=3))
                time.sleep(random.uniform(.05,.1))
        except KeyboardInterrupt:
            self._off()


    def __call__(self):
        if self.args.action == 'on':
            print('Switching everything on.')
            self._on()
        elif self.args.action == 'off':
            print('Switching everything off.')
            self._off()
        elif self.args.action == 'trafficlight':
            print('Entering traffic light mode. Exit with Ctrl+C.')
            self._trafficlight()
        elif self.args.action == 'crazy':
            print('Entering crazy mode. Exit with Ctrl+C.')
            self._crazy()

if __name__ == '__main__':
    main = Main()
    main()
