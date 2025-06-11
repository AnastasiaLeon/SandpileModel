#ifndef SANDPILE_H
#define SANDPILE_H

#include <cstdint>

class Sandpile {
public:
    Sandpile(const char* inputFile);
    ~Sandpile();
    void update();
    bool isStable() const;
    int getWidth() const;
    int getHeight() const;
    uint8_t** getPixelMatrix(int color0, int color1, int color2, int color3, int color4) const; 

private:
    int width;
    int height;
    uint64_t** grid;
    uint64_t** initialSand;
    bool isStableFlag;
    void loadInitialState(const char* inputFile);
    void initializeGrid();
    void topple(int y, int x);
    void expandGridIfNeeded(int y, int x);
};

#endif