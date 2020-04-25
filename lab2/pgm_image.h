#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>

using namespace std;

class Point{
    public:
        double x, y;
        Point(double, double);
};

class PGM_Image{
    private:
        int width, height, color_depth;
        vector<vector<unsigned char>> image;
        void plot(int, int, double, int, double, bool);
    public:
        PGM_Image(string);
        void drop(string);
        void draw_line(Point, Point, double, int, double, bool);
};
