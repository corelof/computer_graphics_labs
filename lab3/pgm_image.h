#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>

using namespace std;

class PGM_Image{
    private:
        int width, height, color_depth;
        vector<vector<double>> image;
        vector<vector<double>> err;
    public:
        PGM_Image(string, bool, double, bool);
        void drop(string, double, bool, int);
        void dither(int, int, double, bool);
};