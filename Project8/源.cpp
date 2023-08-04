#include <arm_neon.h>

// AES���ܺ���
void aes_encrypt(const uint8_t* plaintext, const uint8_t* key, uint8_t* ciphertext) {
    // ������ת��Ϊ128λ�ļĴ�������
    uint8x16_t v_plaintext = vld1q_u8(plaintext);
    uint8x16_t v_key = vld1q_u8(key);

    // ����Կ��չ
    uint8x16_t v_roundkey = v_key;

    // AES����
    const int rounds = 10;

    // ��ʼ����Կ��
    v_plaintext = veorq_u8(v_plaintext, v_roundkey);

    // AES������
    for (int i = 0; i < rounds; ++i) {
        // �ֽ����
        v_plaintext = vaesmcq_u8(v_aesSubq_u8(v_plaintext));

        // ����λ
        v_plaintext = vaesimcq_u8(v_plaintext);

        // �л��
        v_plaintext = vaesmcq_u8(vaesdq_u8(v_plaintext, v_roundkey));

        // ����Կ��
        v_roundkey = v_aesSubq_u8(v_roundkey);
        v_plaintext = veorq_u8(v_plaintext, v_roundkey);
    }

    // ���һ���ֽ����
    v_plaintext = vaesmcq_u8(v_aesSubq_u8(v_plaintext));

    // ����Կ��
    v_roundkey = v_aesSubq_u8(v_roundkey);
    v_plaintext = veorq_u8(v_plaintext, v_roundkey);

    // ������洢�����������
    vst1q_u8(ciphertext, v_plaintext);
}
