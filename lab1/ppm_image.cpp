#include "ppm_image.h"

PPM_Image::PPM_Image(string filename) {
    ifstream fin(filename);
    char cc;
    fin >> cc >> cc;
    fin >> width >> height >> color_depth;
    fin.read(&cc, 1);
    image.assign(height, vector<tuple<int, int, int>>(width));

    char col[3];
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
        {
            if(fin.eof()) throw runtime_error("some pixels not found");
            fin.read(col, 3);
            image[i][j] = {col[0], col[1], col[2]};
        }
    fin.close();
}

void PPM_Image::drop(string filename) {
    ofstream fout(filename);
    if(!fout.is_open()) {
        throw runtime_error("cannot open output file");
    }
    fout << "P6\n" << width << ' ' << height << '\n' << color_depth << '\n';
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            fout << char(get<0>(image[i][j])) << char(get<1>(image[i][j])) << char(get<2>(image[i][j]));
    fout.flush();
    fout.close();
}

void PPM_Image::inverse_colors() {
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            image[i][j] = {color_depth - get<0>(image[i][j]), color_depth - get<1>(image[i][j]), color_depth - get<2>(image[i][j])};
}

void PPM_Image::rotate_right() {
    vector<vector<tuple<int, int, int>>> new_image(width, vector<tuple<int, int, int>>(height));
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            new_image[j][height - i - 1] = image[i][j];
    image = new_image;
    swap(height, width);
}

void PPM_Image::rotate_left() {
    vector<vector<tuple<int, int, int>>> new_image(width, vector<tuple<int, int, int>>(height));
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            new_image[width - j - 1][i] = image[i][j];
    image = new_image;
    swap(height, width);
}

void PPM_Image::reflect_horizontal() {
    for(int i = 0; i < height; i ++)
        reverse(image[i].begin(), image[i].end());
}

void PPM_Image::reflect_verical() {
    for(int i = 0; i < width; i ++)
        for(int j = 0; j < height / 2; j ++)
            swap(image[j][i], image[height - j - 1][i]);
}