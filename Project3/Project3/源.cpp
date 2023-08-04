#include <iostream>
#include <openssl/evp.h>
#include <openssl/objects.h>

// ���������ݽ��й�ϣ����
std::string hashData(const std::string& data, const EVP_MD* md) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);
    EVP_DigestUpdate(ctx, data.c_str(), data.length());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_DigestFinal_ex(ctx, hash, &hashLen);

    EVP_MD_CTX_free(ctx);

    // �������ƹ�ϣֵת��Ϊʮ�������ַ���
    std::string result;
    for (unsigned int i = 0; i < hashLen; ++i) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        result += buf;
    }

    return result;
}

int main() {
    // ʾ�����ݺͳ�����չ����
    std::string input = "Hello, world!";
    std::string extension = "This is an extension";
    std::string originalHash = hashData(input, EVP_sm3());  // ʹ��SM3��ϣ�㷨���ԭʼ��ϣֵ

    std::cout << "Original Hash: " << originalHash << std::endl;

    // ����һ�����۸ĵĹ�ϣֵ
    std::string craftedHash = originalHash + extension;

    // �Ա��۸ĵĹ�ϣֵ������֤
    std::string craftedResult = hashData(extension, EVP_sm3());  // �Դ۸Ĳ��ֽ��й�ϣ����

    if (craftedResult == craftedHash) {
        std::cout << "Length extension attack succeeded!" << std::endl;
    }
    else {
        std::cout << "Length extension attack failed." << std::endl;
    }

    return 0;
}
