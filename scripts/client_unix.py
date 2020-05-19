#!/usr/bin/env python3
import socket

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = "/var/tmp/unix"

connect_command = [
    "cmdConnect\n",
]
read_commands = [
    "cmdClearErrors\n",
    "getTemperatures\n",
    "getAutoReconnect\n",
    "getBootloaderVersion\n",
    "getCommStatus\n",
    "getControlInput\n",
    "getDCLinkVoltage\n",
    "getDLLVersion\n",
    "getDSPVersion\n",
    "getModVoltageRef\n",
    "getModulatorVersion\n",
    "getPheripherieVersion\n",
    "getPrimaryCurrent\n",
    "getModControlMode\n",
    "getModCurrentRef\n",
    "getModPowerRef\n",
    "getModReadings\n",
    "getModResistanceRef\n",
    "getModTree\n",
    "getSysControlMode\n",
    "getSysCurrentRef\n",
    "getSysReadings\n",
    "getSysTree\n",
]


def send_commands(messages):

    s.connect(server_address)
    try:
        # Send data
        for m in messages:
            message = m.encode("utf-8")
            print("send", message)
            s.sendall(message)
            data = s.recv(512)
            print("recv", data, "\n")
    finally:
        print("closing socket")
        s.close()


#send_commands(read_commands)
send_commands(connect_command)
