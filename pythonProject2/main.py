import struct


# 定义辅助函数
def ROTATE_LEFT(x, n):
    return ((x << n) | (x >> (32 - n))) & 0xFFFFFFFF


def FF(X, Y, Z, j):
    if j >= 0 and j <= 15:
        return X ^ Y ^ Z
    elif j >= 16 and j <= 63:
        return (X & Y) | (X & Z) | (Y & Z)


def GG(X, Y, Z, j):
    if j >= 0 and j <= 15:
        return X ^ Y ^ Z
    elif j >= 16 and j <= 63:
        return (X & Y) | (~X & Z)


# 定义Rho方法
def rho_compress(M):
    # 初始化A、B、C、D、E、F、G、H
    A, B, C, D, E, F, G, H = 0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, \
                             0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E

    W = [0] * 68

    # 分组迭代处理
    for i in range(0, len(M), 64):
        # 将分组划分为16个32位字
        for j in range(16):
            W[j] = struct.unpack('>I', M[i + j * 4: i + j * 4 + 4])[0]

        # 对W[16], W[17], ..., W[67]进行填充
        for j in range(16, 68):
            W[j] = ROTATE_LEFT(W[j - 16] ^ W[j - 9] ^ ROTATE_LEFT(W[j - 3], 15), 7) ^ ROTATE_LEFT(W[j - 13], 7) ^ W[
                j - 6]

        # 迭代计算
        for j in range(64):
            SS1 = ROTATE_LEFT((ROTATE_LEFT(A, 12) + E + ROTATE_LEFT(j, j)), 7)
            SS2 = SS1 ^ ROTATE_LEFT(A, 12)
            TT1 = (FF(A, B, C, j) + D + SS2 + W[j]) & 0xFFFFFFFF
            TT2 = (GG(E, F, G, j) + H + SS1 + W[j]) & 0xFFFFFFFF

            D = C
            C = ROTATE_LEFT(B, 9)
            B = A
            A = TT1
            H = G
            G = ROTATE_LEFT(F, 19)
            F = E
            E = TT2

        # 更新A、B、C、D、E、F、G、H
        A = (A ^ (A << 16)) & 0xFFFFFFFF
        B = (B ^ (B << 16)) & 0xFFFFFFFF
        C = (C ^ (C << 16)) & 0xFFFFFFFF
        D = (D ^ (D << 16)) & 0xFFFFFFFF
        E = (E ^ (E << 16)) & 0xFFFFFFFF
        F = (F ^ (F << 16)) & 0xFFFFFFFF
        G = (G ^ (G << 16)) & 0xFFFFFFFF
        H = (H ^ (H << 16)) & 0xFFFFFFFF

    return A, B, C, D, E, F, G, H


# 测试代码
message = b"abc"  # 待哈希的消息

# 对消息进行填充和长度处理
msg_len = len(message)
padded_msg = message + b"\x80" + b"\x00" * ((56 - (msg_len + 1) % 64) % 64) + \
             struct.pack(">Q", msg_len * 8)

A, B, C, D, E, F, G, H = rho_compress(padded_msg)
print("Hash value:", hex(A), hex(B), hex(C), hex(D), hex(E), hex(F), hex(G), hex(H))
