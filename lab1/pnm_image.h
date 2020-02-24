#pragma once

#include <bits/stdc++.h>

using namespace std;

class PNM_Image{
    public:
        virtual void inverse_colors() = 0;
        virtual void rotate_right() = 0;
        virtual void rotate_left() = 0;
        virtual void reflect_verical() = 0;
        virtual void reflect_horizontal() = 0;
        virtual void drop(string) = 0;
};
