#include "precomp.h"
#include "huffman.cpp"

int main()
{
    std::vector<std::pair<char, int>> freq{
        std::make_pair<char, int>('a', 5),
        std::make_pair<char, int>('b', 9),
        std::make_pair<char, int>('c', 12),
        std::make_pair<char, int>('d', 13),
        std::make_pair<char, int>('e', 16),
        std::make_pair<char, int>('f', 45),
    };
    HuffmanTreeCompression huffman(freq);

    std::vector<bool> compressedBits;
    huffman.CompressText("abcffffdefff", compressedBits);
    printf("compressedBits=");
    for (auto compressedBit : compressedBits) {
        printf(compressedBit ? "1" : "0");
    }
    printf("\r\n");
    printf("compressedSize=%d-bits\r\n", (int)compressedBits.size());

    std::string decompressedText;
    huffman.DecompressText(compressedBits, decompressedText);
    printf("decompressedText=%s\r\n", decompressedText.c_str());

    return 0;
}