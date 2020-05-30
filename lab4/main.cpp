#include <iostream>
#include <string>
#include <vector>
#include "image.h"

using namespace std;

vector<string> color_spaces = {"RGB", "HSL", "HSV", "YCbCr.601", "YCbCr.709", "YCoCg", "CMY"};

int main(int argc, char *argv[]) {
    string from, to, input, output;
    int inc, outc;
    bool wf = false;
    for(int i = 1; i < argc; i ++)
    {
        if(string(argv[i]) == "-f" && i < argc - 1) {
            from = string(argv[i+1]);
        }
        if(string(argv[i]) == "-t" && i < argc - 1) {
            to = string(argv[i+1]);
        }
        if(string(argv[i]) == "-i" && i < argc - 2) {
            inc = atoi(argv[i+1]);
            input = string(argv[i+2]);
        }
        if(string(argv[i]) == "-o" && i < argc - 2) {
            outc = atoi(argv[i+1]);
            output = string(argv[i+2]);
        }
    }
    wf |= argc != 11;
    wf |= from == "" || to == "" || input == "" || output == "";
    wf |= (inc != 1 && inc != 3);
    wf |= (outc != 1 && outc != 3);
    bool inf = false, outf = false;
    for(int i = 0; i < color_spaces.size(); i ++)
    {
        if(color_spaces[i] == from)
            inf = true;
        if(color_spaces[i] == to)
            outf = true;
    }
    wf |= (!inf) | (!outf);
    if(wf)
    {
        cerr << "command line arguments are invalid" << endl;
        return 1;
    }

    Image* img;
    try
    {
        img = new Image(inc, input);
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << '\n';
        return 1;
    }

    img -> change_color_space(from, to);
    try
    {
        img -> drop(outc, output);
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}