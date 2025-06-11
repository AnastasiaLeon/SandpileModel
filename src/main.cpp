#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <string>
#include "../include/sandpile.h"
#include "../include/bmp.h"

//сoхранения bmp для текущего состояния
void saveBMP(const Sandpile& model, const char* filename, int color0, int color1, int color2, int color3, int color4) {
    int width = model.getWidth();
    int height = model.getHeight();
    uint8_t** pixelMatrix = model.getPixelMatrix(color0, color1, color2, color3, color4);//1-массив указателей, 2-каждый указатель указывает на строки в массиве

    writeInBMPFile(filename, width, height, pixelMatrix);

    for (int i = 0; i < height; ++i) {
        delete[] pixelMatrix[i];//освобождение строк
    }
    delete[] pixelMatrix; //освобождение массива указателей
}

int getColor(const char* color) {
    int colorInt = -1;
    if (strcmp(color, "white") == 0) {
        colorInt = 0;
    } else if (strcmp(color, "green") == 0) {
        colorInt = 1;
    } else if (strcmp(color, "purple") == 0) {
        colorInt = 2;
    } else if (strcmp(color, "yellow") == 0) {
        colorInt = 3;
    } else if (strcmp(color, "black") == 0) {
        colorInt = 4;
    } else if (strcmp(color, "maroon") == 0) {
        colorInt = 5;
    } else if (strcmp(color, "pink") == 0) {
        colorInt = 6;
    } else if (strcmp(color, "red") == 0) {
        colorInt = 7;
    } else if (strcmp(color, "orange") == 0) {
        colorInt = 8;
    } else if (strcmp(color, "beige") == 0) {
        colorInt = 9;
    } else if (strcmp(color, "grey") == 0) {
        colorInt = 10;
    } else if (strcmp(color, "violet") == 0) {
        colorInt = 11;
    } else if (strcmp(color, "cyan") == 0) {
        colorInt = 12;
    } else if (strcmp(color, "blue") == 0) {
        colorInt = 13;
    } else if (strcmp(color, "navy") == 0) {
        colorInt = 14;
    } else if (strcmp(color, "lime") == 0) {
        colorInt = 15;
    }
    return colorInt;
}

bool isInColors(const char* color, const char* allColors[]) {
    bool isFound = false;
    for (int i = 0; i < 16; i++) {
        if (strcmp(allColors[i], color) == 0) {
            isFound = true;
            break;
        }
    }
    return isFound;
}

int main(int argc, char* argv[]) {    
    const char* inputFile = nullptr;
    const char* usersOutputDir = nullptr;
    int maxIter = -1;
    bool isMaxIter = false;
    int freq = -1;
    const char* colorZero = "white";
    const char* colorOne = "green";
    const char* colorTwo = "purple";
    const char* colorThree = "yellow";
    const char* colorFour = "black";


    //парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            inputFile = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            usersOutputDir = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--max-iter") == 0) {
            maxIter = std::atoi(argv[++i]);
            isMaxIter = true;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--freq") == 0) {
            freq = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-c0") == 0 || strcmp(argv[i], "--color0") == 0) {
            colorZero =  argv[++i];
        } else if (strcmp(argv[i], "-c1") == 0 || strcmp(argv[i], "--color1") == 0) {
            colorOne =  argv[++i];
        } else if (strcmp(argv[i], "-c2") == 0 || strcmp(argv[i], "--color2") == 0) {
            colorTwo =  argv[++i];
        } else if (strcmp(argv[i], "-c3") == 0 || strcmp(argv[i], "--color3") == 0) {
            colorThree =  argv[++i];
        } else if (strcmp(argv[i], "-c4") == 0 || strcmp(argv[i], "--color4") == 0) {
            colorFour =  argv[++i];
        } else {
            std::cerr << "Ошибка! Неправильно заданы входные данные!" << std::endl;
            return 1;
        }
    }

    if (usersOutputDir == nullptr) {
        std::cerr << "Ошибка! Выходная директория не задана!" << std::endl;
        return 1; 
    } else if (inputFile == nullptr) {
        std::cerr << "Ошибка! Входной файл не задан!" << std::endl;
        return 1; 
    } else if (freq == -1) {
        std::cerr << "Ошибка! Некорректно заданы входные данные! Проверьте, задали ли Вы частоту сохранения картинок!" << std::endl;
        return 1; 
    }

    const char* allColors[16] = {"white", "green", "purple", "yellow", "black", "maroon", "pink", "red", "orange", "beige", "grey", "violet", "cyan", "blue", "navy", "lime"};

    if (!(isInColors(colorZero, allColors) && isInColors(colorOne, allColors) && isInColors(colorTwo, allColors) && isInColors(colorThree, allColors) && isInColors(colorFour, allColors))) {
        std::cerr << "Ошибка! Некорректно заданы входные данные! Проверьте, правильно ли вы написали названия цветов!" << std::endl;
        return 1;
    }

    int color0 = getColor(colorZero);
    int color1 = getColor(colorOne);
    int color2 = getColor(colorTwo);
    int color3 = getColor(colorThree);
    int color4 = getColor(colorFour);


    //проверка существования базовой выходной директории
    struct stat info;
    char baseOutputDir[256];
    strncpy(baseOutputDir, usersOutputDir, sizeof(baseOutputDir) - 1);
    baseOutputDir[sizeof(baseOutputDir) - 1] = '\0'; // Ensure null-termination

    if (stat(baseOutputDir, &info) == 0) { // Directory exists
        if (!(info.st_mode & S_IFDIR)) { // It's not a directory
            std::cerr << "Указанный путь \"" << baseOutputDir << "\" не является директорией!" << std::endl;
            return 1;
        }
    } else { // Directory does not exist, create it
        if (mkdir(baseOutputDir, 0777) != 0) {
            std::cerr << "Ошибка создания заданной выходной директории \"" << baseOutputDir << "\"!" << std::endl;
            return 1;
        }
    }

    // Извлечение имени файла из inputFile (без пути и расширения)
    std::string inputFilePathStr(inputFile);
    size_t lastSlashPos = inputFilePathStr.find_last_of("/\\");
    std::string filenameWithExtension = (lastSlashPos == std::string::npos) ? inputFilePathStr : inputFilePathStr.substr(lastSlashPos + 1);

    size_t lastDotPos = filenameWithExtension.find_last_of('.');
    std::string filenameWithoutExtension = (lastDotPos == std::string::npos) ? filenameWithExtension : filenameWithExtension.substr(0, lastDotPos);

    // Создание полного пути для новой поддиректории
    std::string currentSpecificOutputDirName = filenameWithoutExtension;
    char specificOutputDir[512]; // Buffer for the final path
    int count = 0;
    bool uniquePathFound = false;

    while (!uniquePathFound) {
        std::string tempPath = currentSpecificOutputDirName;
        if (count > 0) {
            tempPath += "(" + std::to_string(count) + ")";
        }
        
        snprintf(specificOutputDir, sizeof(specificOutputDir), "%s/%s", baseOutputDir, tempPath.c_str());
        specificOutputDir[sizeof(specificOutputDir) - 1] = '\0'; // Ensure null-termination

        struct stat info_sub;
        if (stat(specificOutputDir, &info_sub) != 0) { // Directory does not exist, or an error occurred
            uniquePathFound = true;
        } else { // Directory exists
            if (info_sub.st_mode & S_IFDIR) { // It's a directory
                count++; // Increment count to try next suffix
            } else { // It's a file, not a directory
                std::cerr << "Путь \"" << specificOutputDir << "\" не является директорией!" << std::endl;
                return 1; // Critical error, cannot proceed
            }
        }
    }

    // Создание специфичной поддиректории для выходных файлов
    if (mkdir(specificOutputDir, 0777) != 0) {
        std::cerr << "Ошибка создания директории для файла \"" << filenameWithoutExtension << "\" по пути \"" << specificOutputDir << "\"!" << std::endl;
        return 1;
    }

    //инициализация песчаной кучи
    Sandpile model(inputFile);

    //начальное состояние пикселей в куче
    if (freq != 0) {
        char initialFilename[256];
        snprintf(initialFilename, sizeof(initialFilename), "%s/state_0.bmp", specificOutputDir);
        saveBMP(model, initialFilename, color0, color1, color2, color3, color4);
    }

    if (isMaxIter) {
        for (int iter = 1; iter <= maxIter; ++iter) {
            model.update();//обновление состояния модели

            //если требуется - создание файла с текущим состоянием
            if (freq > 0 && iter % freq == 0) {
                char filename[256];
                snprintf(filename, sizeof(filename), "%s/state_%d.bmp", specificOutputDir, iter);
                saveBMP(model, filename, color0, color1, color2, color3, color4);
            }

            //если песчаная куча устойчива - прервыаем цикл
            if (model.isStable()) {
                break;
            }
        }
    } else {
        int iter = 1;
        while (!model.isStable()) {
            model.update();//обновление состояния модели

            //если требуется - создание файла с текущим состоянием
            if (freq > 0 && iter % freq == 0) {
                char filename[256];
                snprintf(filename, sizeof(filename), "%s/state_%d.bmp", specificOutputDir, iter);
                saveBMP(model, filename, color0, color1, color2, color3, color4);
            }

            iter++;
        }
    }

    //финальное состояние 
    char finalFilename[256];
    snprintf(finalFilename, sizeof(finalFilename), "%s/final_state.bmp", specificOutputDir);
    saveBMP(model, finalFilename, color0, color1, color2, color3, color4);

    return 0;
}