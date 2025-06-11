#ifndef BMP_H
#define BMP_H

#include <cstdint> 

void writeInBMPFile(const char* fileName, int width, int height, uint8_t** pixelMatrix);

#endif