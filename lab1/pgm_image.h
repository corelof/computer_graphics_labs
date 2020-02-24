#pragma once
#include <bits/stdc++.h>
#include "pnm_image.h"

using namespace std;

class PGM_Image: public PNM_Image{
    private:
        int width, height, color_depth;
        vector<vector<int>> image;
    public:
        PGM_Image(string);
        void inverse_colors();
        void rotate_right();
        void rotate_left();
        void reflect_verical();
        void reflect_horizontal();
        void drop(string);
};
