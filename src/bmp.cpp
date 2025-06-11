#include "../include/bmp.h"
#include <fstream>
#include <iostream>

// Write a 4-byte integer to file in little-endian format
void littleEndian(std::ofstream& file, int num) {
    file << uint8_t(num % 256) << uint8_t((num >> 8) % 256)
         << uint8_t((num >> 16) % 256) << uint8_t((num >> 24) % 256); 
}

// File header (occupies the first 14 bytes of the BMP file)
void fileHeader(std::ofstream& file, int fileSize, int referenceToArray) {
    file << "BM";
    littleEndian(file, fileSize);
    littleEndian(file, 0); // Reserved
    littleEndian(file, referenceToArray); // Offset from file start to pixel data
}

// Core image header (40 bytes)
void coreHeader(std::ofstream& file, int width, int height) {
    littleEndian(file, 40); // Header size
    littleEndian(file, width); // Width
    littleEndian(file, height); // Height
    file << uint8_t(1) << uint8_t(0); // Color planes
    file << uint8_t(4) << uint8_t(0); // 4 bits per pixel - 16 colors
    littleEndian(file, 0); // No compression
    int size = ((width + 1) / 2 + (4 - (width + 1) / 2 % 4) % 4) * height;
    littleEndian(file, size); // Pixel data size
    littleEndian(file, 2835); // Horizontal resolution (72 DPI)
    littleEndian(file, 2835); // Vertical resolution
    littleEndian(file, 16); // Number of colors
    littleEndian(file, 0); // Important colors
}

// Color palette
void colorPalette(std::ofstream& file) {
    file << uint8_t(255) << uint8_t(255) << uint8_t(255) << uint8_t(0); // White
    file << uint8_t(0)   << uint8_t(127) << uint8_t(0)   << uint8_t(0); // Green
    file << uint8_t(90)  << uint8_t(0)   << uint8_t(130) << uint8_t(0); // Purple
    file << uint8_t(0)   << uint8_t(200) << uint8_t(255) << uint8_t(0); // Yellow
    file << uint8_t(0)   << uint8_t(0)   << uint8_t(0)   << uint8_t(0); // Black
    file << uint8_t(0)   << uint8_t(0)   << uint8_t(139) << uint8_t(0); // Maroon
    file << uint8_t(128) << uint8_t(128) << uint8_t(240) << uint8_t(0); // Pink
    file << uint8_t(0)   << uint8_t(0)   << uint8_t(255) << uint8_t(0); // Red
    file << uint8_t(0)   << uint8_t(100) << uint8_t(255) << uint8_t(0); // Orange
    file << uint8_t(196) << uint8_t(228) << uint8_t(255) << uint8_t(0); // Beige
    file << uint8_t(192) << uint8_t(192) << uint8_t(192) << uint8_t(0); // Grey
    file << uint8_t(255) << uint8_t(0)   << uint8_t(255) << uint8_t(0); // Violet
    file << uint8_t(250) << uint8_t(206) << uint8_t(135) << uint8_t(0); // Cyan
    file << uint8_t(255) << uint8_t(0)   << uint8_t(0)   << uint8_t(0); // Blue
    file << uint8_t(139) << uint8_t(0)   << uint8_t(0)   << uint8_t(0); // Navy
    file << uint8_t(0)   << uint8_t(255) << uint8_t(0)   << uint8_t(0); // Lime
}

// Write pixel matrix
void PixelMatrix(std::ofstream& file, int width, int height, uint8_t** pixelMatrix) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x += 2) {
            uint8_t leftPixel = pixelMatrix[y][x];
            uint8_t rightPixel = (x + 1 < width) ? pixelMatrix[y][x + 1] : 0;
            uint8_t byte = (leftPixel << 4) | rightPixel; // Left pixel in high 4 bits, right in low 4 bits
            file << byte;
        }

        int padding = (4 - ((width + 1) / 2) % 4) % 4; // Align rows to 4-byte boundaries
        for (int i = 0; i < padding; i++) {
            file << uint8_t(0); // Padding
        }
    }
}

// Write entire image to BMP file
void writeInBMPFile(const char* fileName, int width, int height, uint8_t** pixelMatrix) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    int headerSize = 54 + 64; // 54 for header, 64 for palette
    int pixelDataSize = ((width + 1) / 2 + (4 - ((width + 1) / 2) % 4) % 4) * height;
    fileHeader(file, headerSize + pixelDataSize, headerSize); // File header
    coreHeader(file, width, height); // Image header
    colorPalette(file); // Palette
    PixelMatrix(file, width, height, pixelMatrix); // Matrix
    file.close();
}