#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include "pgm_image.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if(argc != 7) {
        cerr << "command line arguments are invalid" << endl;
        return 1;
    }
    string fin = string(argv[1]);
    string fout = string(argv[2]);
    bool gradient;
    int algo, bit;
    bool srgb = false;
    double gamma;
    try {
        gradient = (string(argv[3]) == "1");
        algo = atoi(argv[4]);
        bit = atoi(argv[5]);
        if(string(argv[6]) == "0")
        {
            gamma = 2.4;
            srgb = true;
        } else {
            gamma = stold(argv[6]);
        }
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    
    PGM_Image* image;
    try {    
        image = new PGM_Image(fin, gradient, gamma, srgb);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    image -> dither(bit, algo, gamma, srgb);

    try {
        image -> drop(fout, gamma, srgb, bit);
        delete(image);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
