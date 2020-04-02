#!/usr/bin/env python3
import socket
import time

messages = [
        'get voltage\n',
        'set voltage 100.0\n',
        'get current\n',
        'set current 200.0\n'
        ]

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = '/var/tmp/reg-socket'
s.connect(server_address)

try:
    while True:
        # Send data
        for m in messages:
            message = m.encode('utf-8')
            print('> {}'.format(message))
            s.sendall(message)
            data = s.recv(128)
            print('< {}'.format(data))
            time.sleep(1)
finally:
    print('closing socket')
    s.close()