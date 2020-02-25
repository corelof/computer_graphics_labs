#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include "pnm_image.h"

using namespace std;

class PGM_Image: public PNM_Image{
    private:
        int width, height, color_depth;
        vector<vector<unsigned char>> image;
    public:
        PGM_Image(string);
        void inverse_colors();
        void rotate_right();
        void rotate_left();
        void reflect_verical();
        void reflect_horizontal();
        void drop(string);
};
