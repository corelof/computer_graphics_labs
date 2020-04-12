#include <iostream>
#include <string>
#include "pgm_image.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 9) {
        cerr << "command line arguments are invalid" << endl;
        exit(1);
    }
    string fin = string(argv[1]);
    string fout = string(argv[2]);
    int brightness, x_s, y_s, x_f, y_f;
    try {
        brightness = atoi(argv[3]);
        x_s = atoi(argv[5]);
        y_s = atoi(argv[6]);
        x_f = atoi(argv[7]);
        y_f =atoi(argv[8]);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        exit(1);
    }

    double thikness = stod(argv[4]);

    PGM_Image* image;
    try {    
        image = new PGM_Image(fin);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        exit(1);
    }

    image -> draw_line(*(new Point(x_s, y_s)), *(new Point(x_f, y_f)), thikness, brightness);

    try {
        image -> drop(fout);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        exit(1);
    }
}