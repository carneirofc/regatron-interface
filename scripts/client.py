#!/usr/bin/env python3
import time
import socket

messages = [
    "getDebug\n",
    "setDebug 1\n",
]

def test():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the port where the server is listening
    server_address = ("localhost", 20005)
    s.connect(server_address)
    try:
        # Send data
        for i in range(100000):
            for m in messages:
                message = m.encode("utf-8")
                ##print("> {}".format(message))
                s.sendall(message)
                data = s.recv(128)
                ##print("< {}".format(data))
    finally:
        print("closing socket")
        s.close()

if __name__ == '__main__':
    import timeit
    print(timeit.timeit("test()", setup="from __main__ import test", number=1))
