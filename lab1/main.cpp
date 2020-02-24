#include <bits/stdc++.h>
#include "pgm_image.h"
#include "ppm_image.h"
#include "pnm_image.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 4) {
        cout << "command line arguments are invalid" << endl;
        exit(0);
    }
    string fin = string(argv[1]);
    string fout = string(argv[2]);
    string operations = string(argv[3]);

    PNM_Image* image;

    ifstream inFile(fin);
    if(!inFile.is_open()) {
        cout << "file isn't opened. Check permissions / file existance" << endl;
        exit(0);
    }
    char format[2];
    for(int i = 0; i < 2; i ++)
        if(!inFile.eof())
            inFile >> format[i];
        else {
            inFile.close();
            cout << "file has no two chars at start" << endl;
            exit(0);
        }
    inFile.close();
    if(!(format[0] == 'P' && (format[1] == '5' || format[1] == '6'))) {
        cout << "file is not P5/P6 file" << endl;
        exit(0);    
    }

    try {    
        if(format[1] == '5')
            image = new PGM_Image(fin);
        else
            image = new PPM_Image(fin);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
        exit(0);
    }

    for(auto i : operations)
        if(i == '0')
            image -> inverse_colors();
        else if(i == '1')
            image -> reflect_horizontal();
        else if(i == '2')
            image -> reflect_verical();
        else if(i == '3')
            image -> rotate_right();
        else if(i == '4')
            image -> rotate_left();
        else {
            cout << "operation " << i << " not found, skipping" << endl;
        }

    try { image -> drop(fout); }
    catch (const exception& e) {
        cout << e.what() << endl;
        exit(0);
    }
}