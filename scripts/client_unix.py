#!/usr/bin/env python3
import socket

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)


def perf_tree(messages):

    from time import perf_counter

    s.connect(server_address)
    try:
        count = 500
        t1 = perf_counter()
        for i in range(count):
            # Send data
            for m in messages:
                message = m.encode("utf-8")
                s.sendall(message)
                s.recv(512)
        t2 = perf_counter()
        print("Messages per sec: {}".format(count / (t2 - t1)))
    finally:
        print("closing socket")
        s.close()


# Connect the socket to the port where the server is listening
server_address = "/var/tmp/unix"
messages = [
    "cmdClearErrors\n",
    "cmdStoreParam\n",
    "setAutoReconnect 0\n",
    "getAutoReconnect\n",
    "getBootloaderVersion\n",
    "getCommStatus\n",
    "getDCLinkVoltage\n",
    "getDLLVersion\n",
    "getDSPVersion\n",
    "getModulatorVersion\n",
    "getPheripherieVersion\n",
    "cmdConnect\n",
]


def send(messages):
    s.connect(server_address)
    try:
        # Send data
        for m in messages:
            message = m.encode("utf-8")
            s.sendall(message)
            print(">>", message)
            data = b""
            data = s.recv(128)
            print("<<", data)
    finally:
        s.close()


send(messages)
# perf_tree(messages)
