#include "pgm_image.h"
#include <cmath>

const double ld1 = 1;

vector<vector<double> > OrderedDitheringMatrix = {
    {0, 48, 12, 60, 3, 51, 15, 63},
    {32, 16, 44, 28, 35, 19, 47, 31},
    {8, 56, 4, 52, 11, 59, 7, 55},
    {40, 24, 36, 20, 43, 27, 39, 23},
    {2, 50, 14, 62, 1, 49, 13, 61},
    {34, 18, 46, 30, 33, 17, 45, 29},
    {10, 58, 6, 54, 9, 57, 5, 53},
    {42, 26, 38, 22, 41, 25, 37, 21},
},
HalftoneMatrix = {
    {7, 13, 11, 4},
    {12, 16, 14, 8},
    {10, 15, 6, 2},
    {5, 9, 3, 1},
};

PGM_Image::PGM_Image(string filename, bool gradient, double gamma, bool srgb) {
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()) 
        throw runtime_error("failed to open file");

    char cc[2];
    fin >> cc[0] >> cc[1];
    if(cc[0] != 'P' || cc[1] != '5')
        throw runtime_error("expected P5 format");
    fin >> width >> height >> color_depth;
    image.assign(height, vector<unsigned char>(width));
    if(!gradient) {
        char pixel;
        fin.read(&pixel, 1);
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
            {
                fin.read(&pixel, sizeof(unsigned char));
                double old = (double)((unsigned char)pixel) / (double)color_depth;
                if(srgb)
                    old = (old < 0.04045 ? old / 12.92 : pow((old + 0.055) / 1.055, gamma));
                else
                    old = pow(old, gamma);
                image[i][j] = round(old * color_depth);
            }
    } else {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
                image[i][j] = round(j*ld1 / (width - 1) * color_depth);
    }
    fin.close();
}

void PGM_Image::drop(string filename, double gamma, bool srgb, int bit) {
    int ocd = color_depth;
    color_depth = (1 << bit) - 1;
    ofstream fout(filename, ios::binary);
    if(!fout.is_open()) {
        throw runtime_error("cannot open output file");
    }
    fout << "P5\n" << width << ' ' << height << '\n' << color_depth << '\n';
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
        {
            double old = (double)image[i][j] / ocd;
            if(srgb)
                old = (old <= 0.0031308 ? old * 12.92 : pow(old,  ld1/gamma)*1.055 - 0.055);
            else
                old = pow(old, ld1 / gamma);
            int color = round(old * color_depth);
            fout << (unsigned char)color;
        }
    fout.flush();
    fout.close();
}

void PGM_Image::dither(int bit, int algo, double gamma, bool srgb) {
    auto nearest_color = [&bit](int pixel_color){
        vector<unsigned char> res(8);
        vector<unsigned char> mask;
        int idx = 7;
        for(int i = 0; i < bit; i ++) {
            mask.push_back((pixel_color >> idx) % 2);
            idx--;
        }
        idx = 0;
        for(int i = 0; i < 8; i ++) {
            res[i] = mask[idx];
            idx = (idx + 1) % mask.size();
        }
        int cl = 0;
        for(auto i : res)
            cl = cl * 2 + i;
        return cl;
    };

    // No dithering
    if(algo == 0) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
                image[i][j] = nearest_color(image[i][j]);
        return;
    }

    // Ordered 8x8
    if(algo == 1) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
                image[i][j] = nearest_color(image[i][j] + (OrderedDitheringMatrix[i%8][j%8] * ld1 / 64) - 0.5);
        return;
    }
    // Random
    if(algo == 2) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
                image[i][j] = nearest_color(image[i][j] + ((double) rand() / (RAND_MAX)) - 0.5);
        return;
    }
    // Halftone 4x4
    if(algo == 7) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
                image[i][j] = nearest_color(image[i][j] + (HalftoneMatrix[i%4][j%4] * ld1 / 8 - 0.5));
        return;
    }

    err.assign(height, vector<unsigned char>(width, 0));
    // Floyd–Steinberg
    if(algo == 3) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
            {
                image[i][j] += err[i][j];
                double nc = nearest_color(image[i][j]);
                double error = (image[i][j] - nc) / 16;
                image[i][j] = nc;
                if(j + 1 < width)
                    err[i][j+1] += error * 7;
                if(i + 1 < height) {
                    if(j - 1 >= 0)
                        err[i+1][j-1] += error * 3;
                    err[i+1][j] += error * 5;
                    if(j + 1 < width)
                        err[i+1][j+1] += error;
                }
            }
        return;
    }
    // Jarvis, Judice, Ninke
    if(algo == 4) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
            {
                image[i][j] += err[i][j];
                double nc = nearest_color(image[i][j]);
                double error = (image[i][j] - nc) / 48;
                image[i][j] = nc;
                if(j + 1 < width) err[i][j+1] += error * 7;
                if(j + 2 < width) err[i][j+2] += error * 5;
                if(i + 1 < height)
                {
                    if(j - 2 >= 0) err[i+1][j-2] += error * 3;
                    if(j - 1 >= 0) err[i+1][j-1] += error * 5;
                    err[i+1][j] += error * 7;
                    if(j + 1 < width) err[i+1][j+1] += error * 5;
                    if(j + 2 < width) err[i+1][j+2] += error * 3;
                }
                if(i + 2 < height)
                {
                    if(j - 2 >= 0) err[i+2][j-2] += error * 1;
                    if(j - 1 >= 0) err[i+2][j-1] += error * 3;
                    err[i+2][j] += error * 5;
                    if(j + 1 < width) err[i+2][j+1] += error * 3;
                    if(j + 2 < width) err[i+2][j+2] += error * 1;
                }
            }
        return;
    }
    // Sierra-3
    if(algo == 5) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
            {
                image[i][j] += err[i][j];
                double nc = nearest_color(image[i][j]);
                double error = (image[i][j] - nc) / 32;
                image[i][j] = nc;
                if(j + 1 < width) err[i][j+1] += error * 5;
                if(j + 2 < width) err[i][j+2] += error * 3;
                if(i + 1 < height)
                {
                    if(j - 2 >= 0) err[i+1][j-2] += error * 2;
                    if(j - 1 >= 0) err[i+1][j-1] += error * 4;
                    err[i+1][j] += error * 5;
                    if(j + 1 < width) err[i+1][j+1] += error * 4;
                    if(j + 2 < width) err[i+1][j+2] += error * 2;
                }
                if(i + 2 < height)
                {
                    if(j - 1 >= 0) err[i+2][j-1] += error * 2;
                    err[i+2][j] += error * 3;
                    if(j + 1 < width) err[i+2][j+1] += error * 2;
                }
            }
        return;
    }
    // Atkinson
    if(algo == 6) {
        for(int i = 0; i < height; i ++)
            for(int j = 0; j < width; j ++)
            {
                image[i][j] += err[i][j];
                double nc = nearest_color(image[i][j]);
                double error = (image[i][j] - nc) / 8;
                image[i][j] = nc;
                if(j + 1 < width) err[i][j+1] += error;
                if(j + 2 < width) err[i][j+2] += error;
                if(i + 1 < height)
                {
                    if(j - 1 >= 0) err[i+1][j-1] += error;
                    err[i+1][j] += error;
                    if(j + 1 < width) err[i+1][j+1] += error;
                }
                if(i + 2 < height)
                {
                    err[i+2][j] += error;
                }
            }
        return;
    }
}