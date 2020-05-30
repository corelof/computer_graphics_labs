#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <cmath>

struct pixel {
    unsigned char f, s, t;
};

using namespace std;

class Image{
    private:
        int width, height, color_depth;
        vector<vector<pixel> > image;
    public:
        Image(int, string);
        void drop(int, string);
        void change_color_space(string, string);
};


pixel HSL_to_RGB(pixel p);

pixel RGB_to_HSL(pixel);

pixel HSV_to_RGB(pixel);

pixel RGB_to_HSV(pixel);

pixel CMY_to_RGB(pixel);

pixel RGB_to_CMY(pixel);

pixel YCoCg_to_RGB(pixel);

pixel RGB_to_YCoCg(pixel);

pixel RGB_to_YCbCr_601(pixel);

pixel YCbCr_601_to_RGB(pixel);

pixel RGB_to_YCbCr_709(pixel);

pixel YCbCr_709_to_RGB(pixel);