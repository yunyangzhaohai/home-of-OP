#include <iostream>
#include <string>
#include <cryptopp/sm4.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;

std::string EncryptSM4(const std::string& plaintext, const byte key[SM4::DEFAULT_KEYLENGTH], const byte iv[SM4::BLOCKSIZE])
{
    std::string ciphertext;

    try
    {
        CBC_Mode<SM4>::Encryption encryptor(key, sizeof(key), iv);

        StringSource(plaintext, true,
            new StreamTransformationFilter(encryptor,
                new HexEncoder(
                    new StringSink(ciphertext)
                )
            )
        );
    }
    catch (const Exception& ex)
    {
        std::cerr << "Encryption error: " << ex.what() << std::endl;
    }

    return ciphertext;
}

std::string DecryptSM4(const std::string& ciphertext, const byte key[SM4::DEFAULT_KEYLENGTH], const byte iv[SM4::BLOCKSIZE])
{
    std::string decryptedtext;

    try
    {
        CBC_Mode<SM4>::Decryption decryptor(key, sizeof(key), iv);

        StringSource(ciphertext, true,
            new HexDecoder(
                new StreamTransformationFilter(decryptor,
                    new StringSink(decryptedtext)
                )
            )
        );
    }
    catch (const Exception& ex)
    {
        std::cerr << "Decryption error: " << ex.what() << std::endl;
    }

    return decryptedtext;
}

int main()
{
    std::string plaintext = "Hello World!";
    byte key[SM4::DEFAULT_KEYLENGTH] = { 0x00 }; // 用你自己的密钥替换
    byte iv[SM4::BLOCKSIZE] = { 0x00 }; // 用你自己的IV替换

    std::string ciphertext = EncryptSM4(plaintext, key, iv);
    std::string decryptedtext = DecryptSM4(ciphertext, key, iv);

    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Ciphertext (hex): " << ciphertext << std::endl;
    std::cout << "Decrypted Text: " << decryptedtext << std::endl;

    return 0;
}
