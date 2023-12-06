#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::string Utils::calculateSHA1(const std::string& input) {
    // Basic SHA-1 hash algorithm (for educational purposes only)
    // This is not suitable for production use.

    // Constants for SHA-1 algorithm
    const uint32_t k[] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

    // Initialize variables
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Pre-processing: Pad the input
    std::string msg = input;
    uint64_t ml = msg.length() * 8;
    msg += '\x80';
    while ((msg.length() + 8) % 64 != 0) {
        msg += '\x00';
    }
    msg += (char)((ml >> 56) & 0xFF);
    msg += (char)((ml >> 48) & 0xFF);
    msg += (char)((ml >> 40) & 0xFF);
    msg += (char)((ml >> 32) & 0xFF);
    msg += (char)((ml >> 24) & 0xFF);
    msg += (char)((ml >> 16) & 0xFF);
    msg += (char)((ml >> 8) & 0xFF);
    msg += (char)(ml & 0xFF);

    // Process each block
    for (size_t i = 0; i < msg.length(); i += 64) {
        std::vector<uint32_t> w(80, 0);

        // Break block into 16 big-endian words
        for (size_t j = 0; j < 16; ++j) {
            w[j] = (msg[i + j * 4] << 24) | (msg[i + j * 4 + 1] << 16) | (msg[i + j * 4 + 2] << 8) | msg[i + j * 4 + 3];
        }

        // Extend to 80 words
        for (size_t j = 16; j < 80; ++j) {
            w[j] = leftRotate(w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16], 1);
        }

        // Initialize hash value for this chunk
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        // Main loop
        for (size_t j = 0; j < 80; ++j) {
            uint32_t f, k;
            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = leftRotate(a, 5) + f + e + k + w[j];
            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        // Add this chunk's hash to result so far
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Produce the final hash value (big-endian)
    std::stringstream result;
    result << std::hex << std::uppercase << h0 << h1 << h2 << h3 << h4;

    return result.str();
}

uint32_t Utils::leftRotate(uint32_t value, uint32_t shift) {
    return (value << shift) | (value >> (32 - shift));
}
