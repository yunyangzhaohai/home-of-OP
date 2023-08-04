import hashlib

def birthday_attack(hash_function, message_space_size):
    hash_table = {}
    collisions_found = False

    while not collisions_found:
        # 生成随机消息
        message = generate_random_message(message_space_size)

        # 计算哈希值
        hash_value = hash_function(message).digest()

        # 检查是否存在相同的哈希值
        if hash_value in hash_table:
            # 找到碰撞
            original_message = hash_table[hash_value]
            collision_message = message
            collisions_found = True
        else:
            # 将哈希值和消息存储到哈希表中
            hash_table[hash_value] = message

    return original_message, collision_message

def generate_random_message(message_space_size):
    # 在消息空间中生成随机消息
    message = random.randint(0, message_space_size - 1)
    return message.to_bytes((message.bit_length() + 7) // 8, byteorder='big')
