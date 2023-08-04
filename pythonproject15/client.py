import socket
from gmssl.sm2 import CryptSM2, func

HOST = '127.0.0.1'  # 服务器IP地址
PORT = 8888  # 端口号

# 创建客户端套接字
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
print(f'已连接到服务器：{HOST}:{PORT}')

# 接收服务器的公钥
server_public_key = client_socket.recv(1024).decode('utf-8')

# 生成客户端的SM2密钥对
client_private_key = CryptSM2()
client_private_key.generate_key()
client_public_key = client_private_key.public_key

# 使用客户端私钥对消息进行签名
message = '你好，服务器！'
signature = func.bytes_to_hex(func.list_to_bytes(client_private_key.sign(message.encode())))

# 发送签名消息给服务器
client_socket.send(f'{message}:{signature}'.encode('utf-8'))

# 关闭客户端套接字
client_socket.close()
