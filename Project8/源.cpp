#include <arm_neon.h>

// AES加密函数
void aes_encrypt(const uint8_t* plaintext, const uint8_t* key, uint8_t* ciphertext) {
    // 将输入转换为128位的寄存器类型
    uint8x16_t v_plaintext = vld1q_u8(plaintext);
    uint8x16_t v_key = vld1q_u8(key);

    // 轮密钥扩展
    uint8x16_t v_roundkey = v_key;

    // AES轮数
    const int rounds = 10;

    // 初始轮密钥加
    v_plaintext = veorq_u8(v_plaintext, v_roundkey);

    // AES加密轮
    for (int i = 0; i < rounds; ++i) {
        // 字节替代
        v_plaintext = vaesmcq_u8(v_aesSubq_u8(v_plaintext));

        // 行移位
        v_plaintext = vaesimcq_u8(v_plaintext);

        // 列混合
        v_plaintext = vaesmcq_u8(vaesdq_u8(v_plaintext, v_roundkey));

        // 轮密钥加
        v_roundkey = v_aesSubq_u8(v_roundkey);
        v_plaintext = veorq_u8(v_plaintext, v_roundkey);
    }

    // 最后一轮字节替代
    v_plaintext = vaesmcq_u8(v_aesSubq_u8(v_plaintext));

    // 轮密钥加
    v_roundkey = v_aesSubq_u8(v_roundkey);
    v_plaintext = veorq_u8(v_plaintext, v_roundkey);

    // 将结果存储到输出缓冲区
    vst1q_u8(ciphertext, v_plaintext);
}
