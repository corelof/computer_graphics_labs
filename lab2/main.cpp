#include <iostream>
#include <string>
#include "pgm_image.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 9 && argc != 10) {
        cerr << "command line arguments are invalid" << endl;
        return 1;
    }
    string fin = string(argv[1]);
    string fout = string(argv[2]);
    double thikness, x_s, y_s, x_f, y_f, gamma = 2.4;
    bool srgb = true;
    int brightness;
    try {
        brightness = atoi(argv[3]);
        thikness = stod(argv[4]);
        x_s = stod(argv[5]);
        y_s = stod(argv[6]);
        x_f = stod(argv[7]);
        y_f = stod(argv[8]);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    if(argc == 10) {
        try {
            gamma = stod(argv[9]);
            srgb = false;
        }
        catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
        } 
    }

    PGM_Image* image;
    try {    
        image = new PGM_Image(fin);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    image -> draw_line(Point{x_s, y_s}, Point{x_f, y_f}, thikness, brightness, gamma, srgb);

    try {
        image -> drop(fout);
        delete(image);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}