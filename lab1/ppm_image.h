#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <tuple>
#include <fstream>
#include "pnm_image.h"
typedef tuple<unsigned char, unsigned char, unsigned char> color;

using namespace std;

class PPM_Image: public PNM_Image{
    private:
        int width, height, color_depth;
        vector<vector<color>> image;
    public:
        PPM_Image(string);
        void inverse_colors();
        void rotate_right();
        void rotate_left();
        void reflect_verical();
        void reflect_horizontal();
        void drop(string);
};
