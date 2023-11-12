import socket
from serverInfo import ESP32_IP, ESP32_PORT
# for obvious reasons this is not in the repo    

def sendData(data):
    """
    Send data over TCP port. IP and PORT stated in `serverInfo.py`.
    """
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ESP32_IP, ESP32_PORT))
    sock.sendall(data.encode())
    sock.close()

