#include "../include/bmp.h"
#include <fstream>
#include <iostream>

//записывает 4х байтовое целое число в файл в формате little-endian, те младшие байты первые 
void littleEndian(std::ofstream& file, int num) {
    file << uint8_t(num % 256) << uint8_t((num >> 8) % 256)
         << uint8_t((num >> 16) % 256) << uint8_t((num >> 24) % 256); 
}

//заголовок файла! (занимает первые 14 байт бмп файла)
void fileHeader(std::ofstream& file, int fileSize, int referenceToArray) {
    file << "BM";
    littleEndian(file, fileSize);
    littleEndian(file, 0);//резерв
    littleEndian(file, referenceToArray);//смещение от начала файла до пиксельных данных
}

//основной заголовок изображения (40 байт)
void coreHeader(std::ofstream& file, int width, int height) {
    littleEndian(file, 40);//размер заголовка
    littleEndian(file, width);//ширина
    littleEndian(file, height);//высота
    file << uint8_t(1) << uint8_t(0);//цвета
    file << uint8_t(4) << uint8_t(0);//4 юита на пиксель - 16 цветов
    littleEndian(file, 0);//нет сжатия
    int size = ((width + 1) / 2 + (4 - (width + 1) / 2 % 4) % 4) * height;
    littleEndian(file, size);//размер пиксельных дынных
    littleEndian(file, 2835);//горизонтальное разрешение (72 DPI)
    littleEndian(file, 2835);//вертикальное
    littleEndian(file, 16);//кол-во цветов
    littleEndian(file, 0);//важные цвета
}

//палитра
void colorPalette(std::ofstream& file) {
    file << uint8_t(255) << uint8_t(255) << uint8_t(255) << uint8_t(0);//white  -  белый
    file << uint8_t(0)   << uint8_t(127) << uint8_t(0)   << uint8_t(0);//green  -  зеленый
    file << uint8_t(90)  << uint8_t(0)   << uint8_t(130) << uint8_t(0);//purple -  фиолетовый
    file << uint8_t(0)   << uint8_t(200) << uint8_t(255) << uint8_t(0);//yellow -  желтый
    file << uint8_t(0)   << uint8_t(0)   << uint8_t(0)   << uint8_t(0);//black  -  черный
    file << uint8_t(0)   << uint8_t(0)   << uint8_t(139) << uint8_t(0);//maroon -  темно-красный
    file << uint8_t(128) << uint8_t(128) << uint8_t(240) << uint8_t(0);//pink   -  розовый
    file << uint8_t(0)   << uint8_t(0)   << uint8_t(255) << uint8_t(0);//red    -  якро-красный
    file << uint8_t(0)   << uint8_t(100) << uint8_t(255) << uint8_t(0);//orange -  оранжевый
    file << uint8_t(196) << uint8_t(228) << uint8_t(255) << uint8_t(0);//beige  -  бежевый
    file << uint8_t(192) << uint8_t(192) << uint8_t(192) << uint8_t(0);//grey   -  серый
    file << uint8_t(255) << uint8_t(0)   << uint8_t(255) << uint8_t(0);//violet -  малиновый
    file << uint8_t(250) << uint8_t(206) << uint8_t(135) << uint8_t(0);//cyan   -  голубой
    file << uint8_t(255) << uint8_t(0)   << uint8_t(0)   << uint8_t(0);//blue   -  ярко-синий
    file << uint8_t(139) << uint8_t(0)   << uint8_t(0)   << uint8_t(0);//navy   -  темно-синий
    file << uint8_t(0)   << uint8_t(255) << uint8_t(0)   << uint8_t(0);//lime   -  ярко-зеленый
}

//запись матрицы пикселей
void PixelMatrix(std::ofstream& file, int width, int height, uint8_t** pixelMatrix) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x += 2) {
            uint8_t leftPixel = pixelMatrix[y][x];
            uint8_t rightPixel = (x + 1 < width) ? pixelMatrix[y][x + 1] : 0;
            uint8_t byte = (leftPixel << 4) | rightPixel; //левый пиксель - старшие 4 бита, правый - младшие
            file << byte;
        }

        int padding = (4 - ((width + 1) / 2) % 4) % 4;//выравнивание строк на 4-байтовые границы
        for (int i = 0; i < padding; i++) {
            file << uint8_t(0);//отступ
        }
    }
}

//записm всего изображения в бмп
void writeInBMPFile(const char* fileName, int width, int height, uint8_t** pixelMatrix) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    int headerSize = 54 + 64;// 54 - заголовок, 64 - палитра
    int pixelDataSize = ((width + 1) / 2 + (4 - ((width + 1) / 2) % 4) % 4) * height;
    fileHeader(file, headerSize + pixelDataSize, headerSize);//заголовок файла
    coreHeader(file, width, height);//заголовок изобрадения
    colorPalette(file);//палитра
    PixelMatrix(file, width, height, pixelMatrix);//матрица
    file.close();
} 