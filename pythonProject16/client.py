import socket
from gmssl.sm2 import CryptSM2

HOST = '127.0.0.1'  # 服务器IP地址
PORT = 8888  # 端口号

# 创建客户端套接字
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
print(f'已连接到服务器：{HOST}:{PORT}')

# 接收服务器的公钥
server_public_key = client_socket.recv(1024).decode('utf-8')

# 加密待传输的明文
plaintext = '这是需要加密的明文'
ciphertext = CryptSM2().encrypt(plaintext, server_public_key)

# 发送密文给服务器
client_socket.send(ciphertext.encode('utf-8'))

# 关闭客户端套接字
client_socket.close()
