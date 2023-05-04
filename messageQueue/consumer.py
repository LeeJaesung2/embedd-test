import posix_ipc
from ctypes import Structure, c_long, c_int, c_char
import time
import struct


# Define message buffer structure
class MsgBuf(Structure):
    _fields_ = [
        ("msgtype", c_long),
        ("value", c_int),
        ("buf", c_char * 1024)
    ]


def consumer(a):
    BUFFER_SIZE = 1024

    # Define message buffer structure
    msg_struct = struct.Struct('li' + str(BUFFER_SIZE) + 's')

    # Get message queue ID using same key as C++ program
    key = "/8989"
    mq = posix_ipc.MessageQueue(key, posix_ipc.O_CREAT)

    print("Message queue created with ID:", mq)
    # Read messages from queue
    while True:
        # Read message from queue
        try:
            data, msg_type = mq.receive(timeout=1)
        except posix_ipc.BusyError:
            time.sleep(1)
            continue

        # Unpack message data using struct
        msg = msg_struct.unpack(data)
        print(msg)
        value, buf = msg[1], msg[2].decode('utf-8')

        # Print message contents
        print("Received value:", value)
        print("Message data:", buf)

        if value >= 10:
            print("All messages received!")
            break

    # Clean up message queue
    mq.close()
    posix_ipc.unlink_message_queue(key)
    return 0
