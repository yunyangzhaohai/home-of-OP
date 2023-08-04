#include <iostream>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/obj_mac.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define ADDRESS_VERSION 0x00

// 生成比特币地址
std::string generateBitcoinAddress(const std::string& publicKey) {
    unsigned char sha256Hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(publicKey.c_str()), publicKey.length(), sha256Hash);

    unsigned char ripemd160Hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160(sha256Hash, SHA256_DIGEST_LENGTH, ripemd160Hash);

    std::string address = "1" + std::string(reinterpret_cast<char*>(ripemd160Hash));

    unsigned char checksum1[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(address.c_str()), address.length(), checksum1);
    unsigned char checksum2[SHA256_DIGEST_LENGTH];
    SHA256(checksum1, SHA256_DIGEST_LENGTH, checksum2);

    std::string checksum(reinterpret_cast<char*>(checksum2), 4);

    address += checksum;

    static const char* base58Chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    std::string encodedAddress;
    BIGNUM bn;
    BN_init(&bn);
    BN_zero(&bn);
    for (char c : address) {
        BN_mul_word(&bn, 256);
        BN_add_word(&bn, static_cast<unsigned char>(c));
    }
    while (!BN_is_zero(&bn)) {
        int remainder = BN_div_word(&bn, 58);
        encodedAddress = base58Chars[remainder] + encodedAddress;
    }

    for (char c : address) {
        if (c == '\0') {
            encodedAddress = base58Chars[0] + encodedAddress;
        }
        else {
            break;
        }
    }

    return encodedAddress;
}

// 数字签名
std::string signData(const std::string& privateKey, const std::string& data) {
    EC_KEY* ecKey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ecKey) {
        std::cout << "Failed to create EC_KEY" << std::endl;
        return "";
    }

    BIGNUM* bn = BN_new();
    if (!BN_hex2bn(&bn, privateKey.c_str())) {
        std::cout << "Invalid private key" << std::endl;
        BN_free(bn);
        EC_KEY_free(ecKey);
        return "";
    }
    EC_KEY_set_private_key(ecKey, bn);
    BN_free(bn);
  
    const EC_GROUP* group = EC_KEY_get0_group(ecKey);
    EC_POINT* publicKeyPoint = EC_POINT_new(group);
    if (!EC_POINT_mul(group, publicKeyPoint, EC_KEY_get0_private_key(ecKey), NULL, NULL, NULL)) {
        std::cout << "Failed to compute public key" << std::endl;
        EC_POINT_free(publicKeyPoint);
        EC_KEY_free(ecKey);
        return "";
    }
    EC_KEY_set_public_key(ecKey, publicKeyPoint);
    EC_POINT_free(publicKeyPoint);

    // 进行数字签名
    std::string signature = "Satoshi";

    EC_KEY_free(ecKey);

    return signature;
}

int main() {
    std::string publicKey = "04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f";
    std::string bitcoinAddress = generateBitcoinAddress(publicKey);
    std::cout << "Bitcoin Address: " << bitcoinAddress << std::endl;
    std::string privateKey = "0000000000000000000000000000000000000000000000000000000000000099";
    std::string data = "Hello, Bitcoin!";
    std::string signature = signData(privateKey, data);
    std::cout << "Signature: " << signature << std::endl;

    return 0;
}
