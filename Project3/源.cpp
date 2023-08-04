#include <iostream>
#include <openssl/evp.h>
#include <openssl/objects.h>

// 对输入数据进行哈希计算
std::string hashData(const std::string& data, const EVP_MD* md) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);
    EVP_DigestUpdate(ctx, data.c_str(), data.length());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_DigestFinal_ex(ctx, hash, &hashLen);

    EVP_MD_CTX_free(ctx);

    // 将二进制哈希值转换为十六进制字符串
    std::string result;
    for (unsigned int i = 0; i < hashLen; ++i) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        result += buf;
    }

    return result;
}

int main() {
    // 示例数据和长度扩展攻击
    std::string input = "Hello, world!";
    std::string extension = "This is an extension";
    std::string originalHash = hashData(input, EVP_sm3());  // 使用SM3哈希算法获得原始哈希值

    std::cout << "Original Hash: " << originalHash << std::endl;

    // 构造一个被篡改的哈希值
    std::string craftedHash = originalHash + extension;

    // 对被篡改的哈希值进行验证
    std::string craftedResult = hashData(extension, EVP_sm3());  // 对篡改部分进行哈希计算

    if (craftedResult == craftedHash) {
        std::cout << "Length extension attack succeeded!" << std::endl;
    }
    else {
        std::cout << "Length extension attack failed." << std::endl;
    }

    return 0;
}
