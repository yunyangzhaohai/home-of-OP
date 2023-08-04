import socket
from gmssl import sm2, func

HOST = '127.0.0.1'  # 服务器IP地址
PORT = 8888  # 端口号

# 生成SM2密钥对
private_key = sm2.CryptSM2()
private_key.generate_key()

# 创建服务器套接字
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

print(f'服务器在 {HOST}:{PORT} 上监听')

while True:
    # 等待客户端连接
    client_socket, client_address = server_socket.accept()
    print(f'客户端已连接：{client_address}')

    # 发送公钥给客户端
    public_key = private_key.public_key
    client_socket.send(public_key.encode('utf-8'))

    # 接收待解密的密文
    ciphertext = client_socket.recv(1024).decode('utf-8')

    # 使用私钥进行解密
    plain_text = private_key.decrypt(ciphertext)

    print(f'解密结果：{plain_text}')

    # 关闭客户端连接
    client_socket.close()

# 关闭服务器套接字
server_socket.close()
