#include <iostream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

void verifyAES(const std::string& plaintext, const std::string& ciphertext, const byte* key, const byte* iv)
{
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
    encryption.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv);

    std::string encryptedtext;
    CryptoPP::StringSource(plaintext, true, new CryptoPP::StreamTransformationFilter(encryption, new CryptoPP::StringSink(encryptedtext)));

    if (ciphertext == encryptedtext)
    {
        std::cout << "Encryption is correct." << std::endl;
    }
    else
    {
        std::cout << "Encryption is incorrect!" << std::endl;
        std::cout << "Expected ciphertext: " << ciphertext << std::endl;
        std::cout << "Actual ciphertext: " << encryptedtext << std::endl;
    }

    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
    decryption.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv);

    std::string decryptedtext;
    CryptoPP::StringSource(ciphertext, true, new CryptoPP::StreamTransformationFilter(decryption, new CryptoPP::StringSink(decryptedtext)));

    if (plaintext == decryptedtext)
    {
        std::cout << "Decryption is correct." << std::endl;
    }
    else
    {
        std::cout << "Decryption is incorrect!" << std::endl;
        std::cout << "Expected plaintext: " << plaintext << std::endl;
        std::cout << "Actual plaintext: " << decryptedtext << std::endl;
    }
}

int main()
{
    std::string plaintext = "Hello World!";
    std::string ciphertext = "XGQAhJm0xRI3QWfptj8y4A==";
    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f' };
    byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };

    verifyAES(plaintext, ciphertext, key, iv);

    return 0;
}
