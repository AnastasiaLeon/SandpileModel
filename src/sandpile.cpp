#include "../include/sandpile.h"
#include <fstream>
#include <iostream>
#include <cstdint>
#include <limits>

//конструктор(инициализирует песчаную кучу из файла)
Sandpile::Sandpile(const char* inputFile) : width(0), height(0), grid(nullptr), initialSand(nullptr) {
    loadInitialState(inputFile);
}

//деструктор(освобождает динамически выделенную память)
Sandpile::~Sandpile() {
    for (int i = 0; i < height; ++i) {
        delete[] grid[i];//освобождаем строки
    }
    delete[] grid;  
}

//загрузка начального состояния из файла
void Sandpile::loadInitialState(const char* inputFile) {
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла с входными данными!" << inputFile << std::endl;
        return;
    }

    int16_t x, y;//координаты
    uint64_t sand;//кол-во песчинок
    //инициализация макс/мин значениями 
    int minX = std::numeric_limits<uint16_t>::max();
    int minY = std::numeric_limits<uint16_t>::max();
    int maxX = std::numeric_limits<uint16_t>::min();
    int maxY = std::numeric_limits<uint16_t>::min();

    //считываем границы
    while (file >> x >> y >> sand) {
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    //определение начального размера сетки 
    width = maxX - minX + 1;
    height = maxY - minY + 1;
    
    //сбрасывание файла + заполнение начальных значений
    file.clear(); //очистка флагов ошибок состояния потока чтобы он мог использоваться снова
    file.seekg(0, std::ios::beg); //установка позиции указателей чтения - с начала файла
    
    //выделение памяти для начального состояния сетки
    initialSand = new uint64_t*[height];
    for (int i = 0; i < height; ++i) {
        initialSand[i] = new uint64_t[width]();
    }

    //задаем начальные знач песчинок 
    while (file >> x >> y >> sand) {
        initialSand[y - minY][x - minX] = sand;
    }

    //копируем начальные значения в сетку
    initializeGrid();
    file.close();
}

//инициализация сетки (на основе нач данных)
void Sandpile::initializeGrid() {
    grid = new uint64_t*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new uint64_t[width];
        for (int j = 0; j < width; ++j) {
            grid[i][j] = initialSand[i][j];
        }
    }
}

//обновление состояния песчаной кучи
void Sandpile::update() {
    bool stable = true;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] > 3) {
                topple(i, j);
                stable = false;
            }
        }
    }
    isStableFlag = stable;
}

//обвал песчинок в указаннной ячейке
void Sandpile::topple(int y, int x) {
    if (grid[y][x] > 3) {
        grid[y][x] -= 4;

        //добавление 4ёх песчинок в соседние клетки
        if (y > 0) grid[y - 1][x] += 1;//верхняя
        if (y < height - 1) grid[y + 1][x] += 1;//нижняя
        if (x > 0) grid[y][x - 1] += 1;//левая
        if (x < width - 1) grid[y][x + 1] += 1;//правая

        //проверка: екобходимо ли расширить сетку
        expandGridIfNeeded(y, x);
    }
}



//расширение сетки
void Sandpile::expandGridIfNeeded(int y, int x) {
    bool expandUp = false;
    bool expandDown = false;
    bool expandLeft = false;
    bool expandRight = false;

    //проверка нижнего и верхнего ряда 
    for (int j = 0; j < width; ++j) {
        if (grid[0][j] > 3) expandUp = true;
        if (grid[height - 1][j] > 3) expandDown = true;
    }

    //проверка левого и правого столбца
    for (int i = 0; i < height; ++i) {
        if (grid[i][0] > 3) expandLeft = true;
        if (grid[i][width - 1] > 3) expandRight = true;
    }

    int newWidth = width + (expandLeft ? 1 : 0) + (expandRight ? 1 : 0);
    int newHeight = height + (expandUp ? 1 : 0) + (expandDown ? 1 : 0);

    //если не требуется расширение - то завершаем
    if (newWidth == width && newHeight == height) return;

    //создание новой сетки с новыми размерами
    uint64_t** newGrid = new uint64_t*[newHeight];
    for (int i = 0; i < newHeight; ++i) {
        newGrid[i] = new uint64_t[newWidth]();
    }

    //определяем смещения
    int offsetY = expandUp ? 1 : 0;
    int offsetX = expandLeft ? 1 : 0;

    //копируем значения из старой сетки в новую
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            newGrid[i + offsetY][j + offsetX] = grid[i][j];
        }
    }

    //освобождение памяти старой сетки
    for (int i = 0; i < height; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    //переназначаем сетку на новую + обновляем размеры
    grid = newGrid;
    width = newWidth;
    height = newHeight;
}



//проверяем модель на стабильность
bool Sandpile::isStable() const {
    return isStableFlag;
}

//получаем ширину сетки
int Sandpile::getWidth() const {
    return width;
}

//получаем высоту сетки
int Sandpile::getHeight() const {
    return height;
}

//! генерация пиксельной матрицы для бмп изображения
uint8_t** Sandpile::getPixelMatrix(int color0, int color1, int color2, int color3, int color4) const {
    uint8_t** pixelMatrix = new uint8_t*[height];
    for (int i = 0; i < height; ++i) {
        pixelMatrix[i] = new uint8_t[width];
        for (int j = 0; j < width; ++j) {
            uint64_t sand = grid[i][j];
            if (sand == 0) pixelMatrix[i][j] = color0;
            else if (sand == 1) pixelMatrix[i][j] = color1;
            else if (sand == 2) pixelMatrix[i][j] = color2;
            else if (sand == 3) pixelMatrix[i][j] = color3;
            else pixelMatrix[i][j] = color4;
        }
    }
    return pixelMatrix;
}