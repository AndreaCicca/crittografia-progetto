/**
 * @file sha1.hh
 * @author Andrea Ciccarello
 * @brief Definizione classe SHA1
 * @version 1.0
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <cstdint>
#include <string>

namespace cripto {

constexpr unsigned int SHA1_BLOCK_SIZE  = 64; // 512 bits
constexpr unsigned int SHA1_DIGEST_SIZE = 20; // 160 bits

class SHA1 {
  public:
    SHA1();
    void               initialization(const uint8_t *data, size_t length);
    void               final(uint8_t digest[SHA1_DIGEST_SIZE]);
    static std::string toHexString(const uint8_t *digest, size_t length);

  private:
    void transform(const uint8_t block[SHA1_BLOCK_SIZE]);
    void padding();

    //! I 5 registri di stato inizializzati con i valori iniziali
    //! specificati dallo standard.
    uint32_t state[5];

    //! Tiene traccia del numero di bit processati fino a questo momento,
    //! nel momento in cui si fa
    uint64_t bitCount;

    /**
     * @brief Lunghezza corrente del buffer interno per accumulare i dati
     * prima di processarlo Quando si raggiungono i 64 byte (512 bit) il
     * buffer viene processato e il bufferLength azzerato.
     *
     * @note Il buffer è di 64 byte (512 bit) come specificato dallo
     * standard.
     */
    size_t bufferLength;

    /**
     * @brief Numero del blocco da elaborare che ha come scopo quello di
     * migliorare l'output del logging a livello trace.
     *
     * Ogni qual volta che si invoca la funzione transform viene aumentato
     * questo contatore, queste informaizoni vengono poi successicamente
     * inserite all'intenro del file di logging prima di mostrare i nuovi valori
     * dei registri
     *
     */
    uint64_t block_number;

    uint8_t buffer[SHA1_BLOCK_SIZE];
};

} // namespace cripto

// Pseudocodice SHA1

// Note: All variables are unsigned 32 bits and wrap modulo 232 when calculating

// Initialize variables:
// h0 = 0x67452301
// h1 = 0xEFCDAB89
// h2 = 0x98BADCFE
// h3 = 0x10325476
// h4 = 0xC3D2E1F0

// Pre-processing:
// append the bit '1' to the message
// append k bits '0', where k is the minimum number ≥ 0 such that the resulting
// message
//     length (in bits) is congruent to 448 (mod 512)
// append length of message (before pre-processing), in bits, as 64-bit
// big-endian integer Process the message in successive 512-bit chunks: break
// message into 512-bit chunks for each chunk
//     break chunk into sixteen 32-bit big-endian words w[i], 0 <= i <= 15

//     Extend the sixteen 32-bit words into eighty 32-bit words:
//     for i from 16 to 79
//         w[i] = (w[i-3] xor w[i-8] xor w[i-14] xor w[i-16]) leftrotate 1

//     Initialize hash value for this chunk:
//     a = h0
//     b = h1
//     c = h2
//     d = h3
//     e = h4

//     Main loop:
//     for i from 0 to 79
//         if 0 ≤ i ≤ 19 then
//             f = (b and c) or ((not b) and d)
//             k = 0x5A827999
//         else if 20 ≤ i ≤ 39
//             f = b xor c xor d
//             k = 0x6ED9EBA1
//         else if 40 ≤ i ≤ 59
//             f = (b and c) or (b and d) or (c and d)
//             k = 0x8F1BBCDC
//         else if 60 ≤ i ≤ 79
//             f = b xor c xor d
//             k = 0xCA62C1D6
//         temp = (a leftrotate 5) + f + e + k + w[i]
//         e = d
//         d = c
//         c = b leftrotate 30
//         b = a
//         a = temp

//     Add this chunk's hash to result so far:
//     h0 = h0 + a
//     h1 = h1 + b
//     h2 = h2 + c
//     h3 = h3 + d
//     h4 = h4 + e

// Produce the final hash value (big-endian):
// digest = hash = h0 append h1 append h2 append h3 append h4