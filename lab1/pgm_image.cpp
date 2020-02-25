#include "pgm_image.h"

PGM_Image::PGM_Image(string filename) {
    ifstream fin(filename, ios::binary);
    char cc;
    fin >> cc >> cc;
    fin >> width >> height >> color_depth;
    image.assign(height, vector<unsigned char>(width));
    fin.read(&cc, 1);
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
        {
            if(fin.eof()) {
                fin.close();
                throw runtime_error("some pixels not found");
            }
            fin.read(&cc, sizeof(unsigned char));
            image[i][j] = cc;
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

void PGM_Image::inverse_colors() {
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            image[i][j] = color_depth - image[i][j];
}

void PGM_Image::rotate_right() {
    vector<vector<unsigned char>> new_image(width, vector<unsigned char>(height, 0));
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            new_image[j][height - i - 1] = image[i][j];
    image = new_image;
    swap(height, width);
}

void PGM_Image::rotate_left() {
    vector<vector<unsigned char>> new_image(width, vector<unsigned char>(height, 0));
    for(int i = 0; i < height; i ++)
        for(int j = 0; j < width; j ++)
            new_image[width - j - 1][i] = image[i][j];
    image = new_image;
    swap(height, width);
}

void PGM_Image::reflect_horizontal() {
    for(int i = 0; i < height; i ++)
        reverse(image[i].begin(), image[i].end());
}

void PGM_Image::reflect_verical() {
    for(int i = 0; i < width; i ++)
        for(int j = 0; j < height / 2; j ++)
            swap(image[j][i], image[height - j - 1][i]);
}