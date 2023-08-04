#include <iostream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;

std::string EncryptAES(const std::string& plaintext, const byte key[AES::DEFAULT_KEYLENGTH], const byte iv[AES::BLOCKSIZE])
{
    std::string ciphertext;

    try
    {
        CBC_Mode<AES>::Encryption encryptor(key, sizeof(key), iv);

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

std::string DecryptAES(const std::string& ciphertext, const byte key[AES::DEFAULT_KEYLENGTH], const byte iv[AES::BLOCKSIZE])
{
    std::string decryptedtext;

    try
    {
        CBC_Mode<AES>::Decryption decryptor(key, sizeof(key), iv);

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
    byte key[AES::DEFAULT_KEYLENGTH] = { 0x00 }; // 用你自己的密钥替换
    byte iv[AES::BLOCKSIZE] = { 0x00 }; // 用你自己的IV替换

    std::string ciphertext = EncryptAES(plaintext, key, iv);
    std::string decryptedtext = DecryptAES(ciphertext, key, iv);

    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Ciphertext (hex): " << ciphertext << std::endl;
    std::cout << "Decrypted Text: " << decryptedtext << std::endl;

    return 0;
}
