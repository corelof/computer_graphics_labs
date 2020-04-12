#include "pgm_image.h"
#include <cmath>

double sqr(double a) {
    return a * a;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}

double find_y(int x, double gradient, Point begin) {
    return begin.y + gradient * (x - begin.x);
}

Point::Point(double xx, double yy) {
    this -> x = xx;
    this -> y = yy;
}

PGM_Image::PGM_Image(string filename) {
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()) 
        throw runtime_error("failed to open file");

    char cc[2];
    fin >> cc[0] >> cc[1];
    if(cc[0] != 'P' || cc[1] != '5')
        throw runtime_error("expected P5 format");
    fin >> width >> height >> color_depth;
    image.assign(height, vector<unsigned char>(width));
    char pixel;
    fin.read(&pixel, 1);
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
        {
            fin.read(&pixel, sizeof(unsigned char));
            image[i][j] = pixel;
        }
    fin.close();
}

void PGM_Image::drop(string filename) {
    ofstream fout(filename, ios::binary);
    if(!fout.is_open()) {
        throw runtime_error("cannot open output file");
    }
    fout << "P5\n" << width << ' ' << height << '\n' << color_depth << '\n';
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            fout << (unsigned char)(image[i][j]);
    fout.flush();
    fout.close();
}

void PGM_Image::plot(int x, int y, double brightness, int color, bool rot) {
    if(rot) {
        swap(x, y);
    }
    if(x < 0 || x >= width || y < 0 || y >= height || brightness < 0) {
        return;
    }
    double old = double((unsigned char)image[y][x]) / color_depth;
    old = (old < 0.04045 ? old / 12.92 : pow((old + 0.055) / 1.055, 2.4));
    old *= (1.0 - brightness);
    old += brightness * color / 255.0;
    old = (old <= 0.0031308 ? old * 12.92 : pow(1.055 * old,  0.416) - 0.055);
    image[y][x] = old * color_depth;
}

void PGM_Image::draw_line(Point begin, Point end, double thikness, int color) {
    bool rot = abs(begin.x - end.x) < abs(begin.y - end.y);
    if(rot) {
        swap(begin.x, begin.y);
        swap(end.x, end.y);
    }
    if (begin.x > end.x) {
        swap(begin.x, end.x);
        swap(begin.y, end.y);
    }
    double grad = (end.y - begin.y) / (end.x - begin.x), y;
    // draw line beginning
    for(int plotX = round(begin.x) - thikness / 2; plotX < round(begin.x); plotX++, y += grad) {
        y = find_y(plotX, grad, begin);
        Point point(round(begin.x), find_y(round(begin.x), grad, begin));
        for(int plotY = int(y - (thikness - 1) / 2.0); plotY <= int(y - (thikness - 1) / 2.0 + thikness); plotY++) {
            plot(plotX, plotY, min(1.0, (thikness + 1.0) / 2.0 - dist(plotX, plotY, point.x, point.y)), color, rot);
        }
    }
    // draw line
    for(int plotX = round(begin.x); plotX <= round(end.x); plotX++, y += grad) {
        y = find_y(plotX, grad, begin);
        for(int plotY = int(y - (thikness - 1) / 2.0); plotY <= int(y - (thikness - 1) / 2.0 + thikness); plotY++) {
            plot(plotX, plotY, min(1.0, (thikness + 1.0) / 2.0 - fabs(y - plotY)), color, rot);
        }
    }
    // draw line end
    for(int plotX = round(end.x) + 1; plotX <= round(end.x) + thikness / 2; plotX++, y += grad) {
        y = find_y(plotX, grad, begin);
        Point point(round(end.x), find_y(round(end.x), grad, begin));
        for(int plotY = int(y - (thikness - 1) / 2.0); plotY <= int(y - (thikness - 1) / 2.0 + thikness); plotY++) {
            plot(plotX, plotY, min(1.0, (thikness + 1.0) / 2.0 - dist(plotX, plotY, point.x, point.y)), color, rot);
        }
    }
}