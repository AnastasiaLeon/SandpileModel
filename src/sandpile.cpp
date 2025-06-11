#include "../include/sandpile.h"
#include <fstream>
#include <iostream>
#include <cstdint>
#include <limits>

// Constructor (initializes the sandpile from a file)
Sandpile::Sandpile(const char* inputFile) : width(0), height(0), grid(nullptr), initialSand(nullptr) {
    loadInitialState(inputFile);
}

// Destructor (frees dynamically allocated memory)
Sandpile::~Sandpile() {
    for (int i = 0; i < height; ++i) {
        delete[] grid[i]; // Free rows
    }
    delete[] grid;  
}

// Load initial state from file
void Sandpile::loadInitialState(const char* inputFile) {
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return;
    }

    int16_t x, y; // Coordinates
    uint64_t sand; // Number of sand grains
    // Initialize with max/min values
    int minX = std::numeric_limits<uint16_t>::max();
    int minY = std::numeric_limits<uint16_t>::max();
    int maxX = std::numeric_limits<uint16_t>::min();
    int maxY = std::numeric_limits<uint16_t>::min();

    // Read boundaries
    while (file >> x >> y >> sand) {
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    // Determine initial grid size
    width = maxX - minX + 1;
    height = maxY - minY + 1;
    
    // Reset file and populate initial values
    file.clear(); // Clear stream error flags to allow reuse
    file.seekg(0, std::ios::beg); // Set read position to file start
    
    // Allocate memory for initial grid state
    initialSand = new uint64_t*[height];
    for (int i = 0; i < height; ++i) {
        initialSand[i] = new uint64_t[width]();
    }

    // Set initial sand grain values
    while (file >> x >> y >> sand) {
        initialSand[y - minY][x - minX] = sand;
    }

    // Copy initial values to grid
    initializeGrid();
    file.close();
}

// Initialize grid (based on initial data)
void Sandpile::initializeGrid() {
    grid = new uint64_t*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new uint64_t[width];
        for (int j = 0; j < width; ++j) {
            grid[i][j] = initialSand[i][j];
        }
    }
}

// Update sandpile state
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

// Topple sand grains in specified cell
void Sandpile::topple(int y, int x) {
    if (grid[y][x] > 3) {
        grid[y][x] -= 4;

        // Add one grain to neighboring cells
        if (y > 0) grid[y - 1][x] += 1; // Top
        if (y < height - 1) grid[y + 1][x] += 1; // Bottom
        if (x > 0) grid[y][x - 1] += 1; // Left
        if (x < width - 1) grid[y][x + 1] += 1; // Right

        // Check if grid expansion is needed
        expandGridIfNeeded(y, x);
    }
}

// Expand grid if necessary
void Sandpile::expandGridIfNeeded(int y, int x) {
    bool expandUp = false;
    bool expandDown = false;
    bool expandLeft = false;
    bool expandRight = false;

    // Check top and bottom rows
    for (int j = 0; j < width; ++j) {
        if (grid[0][j] > 3) expandUp = true;
        if (grid[height - 1][j] > 3) expandDown = true;
    }

    // Check left and right columns
    for (int i = 0; i < height; ++i) {
        if (grid[i][0] > 3) expandLeft = true;
        if (grid[i][width - 1] > 3) expandRight = true;
    }

    int newWidth = width + (expandLeft ? 1 : 0) + (expandRight ? 1 : 0);
    int newHeight = height + (expandUp ? 1 : 0) + (expandDown ? 1 : 0);

    // If no expansion is needed, return
    if (newWidth == width && newHeight == height) return;

    // Create new grid with updated dimensions
    uint64_t** newGrid = new uint64_t*[newHeight];
    for (int i = 0; i < newHeight; ++i) {
        newGrid[i] = new uint64_t[newWidth]();
    }

    // Determine offsets
    int offsetY = expandUp ? 1 : 0;
    int offsetX = expandLeft ? 1 : 0;

    // Copy values from old grid to new grid
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            newGrid[i + offsetY][j + offsetX] = grid[i][j];
        }
    }

    // Free old grid memory
    for (int i = 0; i < height; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    // Reassign grid to new grid and update dimensions
    grid = newGrid;
    width = newWidth;
    height = newHeight;
}

// Check if model is stable
bool Sandpile::isStable() const {
    return isStableFlag;
}

// Get grid width
int Sandpile::getWidth() const {
    return width;
}

// Get grid height
int Sandpile::getHeight() const {
    return height;
}

// Generate pixel matrix for BMP image
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