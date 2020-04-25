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

double triangle_size(Point p1, Point p2, Point p3) {
    double a = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    double b = sqrt((p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y));
    double c = sqrt((p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y));
    double p = (a + b + c) / 2;
    return sqrt(p * (p-a) * (p-b) * (p-c));
}

bool point_in_rect(Point p1, Point p2, Point p3, Point p4, Point pp) {
    double h = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    double w = sqrt((p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y));
    double rect_size = h * w;
    double tr_size = triangle_size(pp, p1, p2) + triangle_size(pp, p1, p3) + triangle_size(pp, p3, p4) + triangle_size(pp, p4, p2);
    return fabs(rect_size - tr_size) < 1e-5;
}

double intersection_size(Point p1, Point p2, Point p3, Point p4, int px, int py) {
    if(
        point_in_rect(p1, p2, p3, p4, Point{px*1.0, py*1.0}) &&
        point_in_rect(p1, p2, p3, p4, Point{px+1.0, py*1.0}) &&
        point_in_rect(p1, p2, p3, p4, Point{px*1.0, py+1.0}) &&
        point_in_rect(p1, p2, p3, p4, Point{px+1.0, py+1.0})
    ) return 1.0;
    int ins = 0, totp = 0;
    for(double i = px + 0.1; i + 0.1 <= px + 1; i += 0.1)
        for(double j = py + 0.1; j + 0.1 <= py + 1; j += 0.1)
        {
            totp ++;
            if(point_in_rect(p1, p2, p3, p4, Point{i, j}))
                ins++;
        }
    return ins * 1.0 / totp;
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

void PGM_Image::plot(int x, int y, double brightness, int color, double gamma, bool srgb) {
    if(x < 0 || x >= width || y < 0 || y >= height || brightness < 0) {
        return;
    }
    double old = double((unsigned char)image[y][x]) / color_depth;
    if(srgb)
        old = (old < 0.04045 ? old / 12.92 : pow((old + 0.055) / 1.055, gamma));
    else
        old = pow(old, gamma);
    old *= (1.0 - brightness);
    double corrected_color = color * 1.0 / 255;
    if(srgb)
        corrected_color = (corrected_color < 0.04045 ? corrected_color / 12.92 : pow((corrected_color + 0.055) / 1.055, gamma));
    else
        corrected_color = pow(corrected_color, gamma);
    old += brightness * corrected_color;
    if(srgb)
        old = (old <= 0.0031308 ? old * 12.92 : pow(old,  1.0/gamma)*1.055 - 0.055);
    else
        old = pow(old, 1.0 / gamma);
    if(old >= 0.9999) old = 1.0;
    image[y][x] = color_depth * old;
}

void PGM_Image::draw_line(Point begin, Point end, double thikness, int color, double gamma, bool srgb) {
    Point line_vector = {end.x - begin.x, end.y - begin.y};
    Point th_vector = {1.0, 0.0};
    if(line_vector.x != 0)
        th_vector = {-line_vector.y / line_vector.x, 1.0};
    double thikness_coef = sqrt((thikness*thikness / 4) / (th_vector.x*th_vector.x + th_vector.y*th_vector.y));
    Point p1 = {begin.x + thikness_coef*th_vector.x, begin.y + thikness_coef*th_vector.y};
    Point p2 = {begin.x - thikness_coef*th_vector.x, begin.y - thikness_coef*th_vector.y};
    Point p3 = {end.x + thikness_coef*th_vector.x, end.y + thikness_coef*th_vector.y};
    Point p4 = {end.x - thikness_coef*th_vector.x, end.y - thikness_coef*th_vector.y};
    for(int i = max(0, int(min(min(p1.y, p2.y), min(p3.y, p4.y))) - 3); i < min(height, int(max(max(p1.y, p2.y), max(p3.y, p4.y))) + 3); i ++)
        for(int j = max(0, int(min(min(p1.x, p2.x), min(p3.x, p4.x))) - 3); j < min(width, int(max(max(p1.x, p2.x), max(p3.x, p4.x))) + 3); j ++) {
            plot(j, i, intersection_size(p1, p2, p3, p4, j, i), color, gamma, srgb);
        }
}