#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <chrono>
#include <omp.h>
// Ñ­»·×óÒÆº¯Êý
uint32_t rotateLeft(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

// FFº¯Êý
uint32_t FF(uint32_t X, uint32_t Y, uint32_t Z, int i) {
    if (i >= 0 && i <= 15)
        return X ^ Y ^ Z;
    else
        return (X & Y) | (X & Z) | (Y & Z);
}

// GGº¯Êý
uint32_t GG(uint32_t X, uint32_t Y, uint32_t Z, int i) {
    if (i >= 0 && i <= 15)
        return X ^ Y ^ Z;
    else
        return (X & Y) | (~X & Z);
}

// P0º¯Êý
uint32_t P0(uint32_t X) {
    return X ^ rotateLeft(X, 9) ^ rotateLeft(X, 17);
}

// Ñ¹Ëõº¯Êý
void compress(std::vector<uint32_t>& V, const std::vector<uint32_t>& B) {
    uint32_t A = V[0], Bm = V[1], C = V[2], D = V[3], E = V[4], F = V[5], G = V[6], H = V[7];
    uint32_t SS1, SS2, TT1, TT2;

    for (int i = 0; i < 64; ++i) {
        if (i >= 16) {
            SS1 = rotateLeft(rotateLeft(A, 12) + E + rotateLeft(0x7A879D8A, i % 32), 7);
            SS2 = SS1 ^ rotateLeft(A, 12);
            TT1 = FF(A, Bm, C, i) + D + SS2 + B[i];
            TT2 = GG(E, F, G, i) + H + SS1 + B[i];
        }
        else { // 0 <= i <= 15
            SS1 = rotateLeft(rotateLeft(A, 12) + E + rotateLeft(0x79CC4519, i), 7);
            SS2 = SS1 ^ rotateLeft(A, 12);
            TT1 = FF(A, Bm, C, i) + D + SS2 + B[i];
            TT2 = GG(E, F, G, i) + H + SS1 + B[i];
        }

        D = C;
        C = rotateLeft(Bm, 9);
        Bm = A;
        A = TT1;
        H = G;
        G = rotateLeft(F, 19);
        F = E;
        E = P0(TT2);
    }

    V[0] ^= A;
    V[1] ^= Bm;
    V[2] ^= C;
    V[3] ^= D;
    V[4] ^= E;
    V[5] ^= F;
    V[6] ^= G;
    V[7] ^= H;
}

// Ìî³äº¯Êý
std::vector<uint32_t> padding(const std::string& message) {
    size_t ml = message.length() * 8;
    size_t len = ((ml + 8) / 512 + 1) * 512 - 64;
    std::vector<uint32_t> M(len / 32, 0);

    for (size_t i = 0; i < message.length(); ++i) {
        M[i / 4] |= static_cast<uint32_t>(message[i]) << ((3 - i % 4) * 8);
    }
    M[message.length() / 4] |= 0x80u << ((3 - message.length() % 4) * 8);
    M[len / 32 - 2] = (ml >> 24) & 0xFF;
    M[len / 32 - 1] = (ml & 0xFFFFFFu);

    return M;
}

// Hash¼ÆËãº¯Êý
std::string hash(const std::string& message) {
    std::vector<uint32_t> V = { 0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, 0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E };
    std::vector<uint32_t> M = padding(message);

    auto start = std::chrono::steady_clock::now();
#pragma omp parallel for
    for (size_t i = 0; i < M.size() / 16; ++i) {
        std::vector<uint32_t> B(M.begin() + i * 16, M.begin() + (i + 1) * 16);
        compress(V, B);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();

    std::stringstream ss;
    for (const auto& v : V) {
        ss << std::hex << std::setfill('0') << std::setw(8) << v;
    }

    std::cout << "Hash calculation time: " << duration*1000 << " ms" << std::endl;

    return ss.str();
}

int main() {
    std::string message = "Hello, SM3!";
    std::string digest = hash(message);
    std::cout << "Message: " << message << std::endl;
    std::cout << "Hash: " << digest << std::endl;

    return 0;
}
