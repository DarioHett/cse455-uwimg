#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int size_column = im.w; 
    int size_row = im.h;
    int size_channel = im.c;

    if (x >= size_column) {
        x = size_column - 1;
    } else if (x < 0) {
        x = 0;
    }

    if (y >= size_row) {
        y = size_row - 1;
    } else if (y < 0) {
        y = 0;
    }

    if (c >= size_channel) {
        c = size_channel - 1;
    } else if (c < 0) {
        c = 0;
    }

    float pixel = im.data[x+y*size_column+c*size_column*size_row];
    return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    int size_column = im.w; 
    int size_row = im.h;
    int size_channel = im.c;
    _Bool error = 0;

    if (x >= size_column)    {
        error = 1;
    }    else if (x < 0)    {
        error = 1;
    }

    if (y >= size_row)    {
        error = 1;
    }    else if (y < 0)    {
        error = 1;
    }

    if (c >= size_channel)    {
        error = 1;
    }    else if (c < 0)    {
        error = 1;
    }

    if (error == 0) {
        im.data[x+y*size_column+c*size_column*size_row] = v;
    } 
}

image copy_image(image im)
{
    int size_column = im.w; 
    int size_row = im.h;
    int size_channel = im.c;
    image copy = make_image(size_column, size_row, size_channel);
    // TODO Fill this in
    copy.data = calloc(size_column*size_row*size_channel, sizeof(float));

    for(int k = 0; k < size_channel; ++k){
        for(int j = 0; j < size_row; ++j){
            for(int i = 0; i < size_column; ++i){
                int index = i + size_column*j + size_column*size_row*k;
                copy.data[index] = im.data[index];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    // Y' = 0.299 R' + 0.587 G' + .114 B'
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    float r,g,b,z;
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            r = get_pixel(im,x,y,0);
            g = get_pixel(im,x,y,1);
            b = get_pixel(im,x,y,2);

            z = 0.299*r + 0.587*g + 0.114*b;
            set_pixel(gray,x,y,0,z);
        }
    }
    return gray;
}


void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    int size_column = im.w; 
    int size_row = im.h;
    for(int j = 0; j < size_row; ++j){
        for(int i = 0; i < size_column; ++i){
            int index = i + size_column*j + size_column*size_row*c;
            im.data[index] += v;

        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    int size_column = im.w; 
    int size_row = im.h;
    int size_channel = im.c;

    for(int k = 0; k < size_channel; ++k){
        for(int j = 0; j < size_row; ++j){
            for(int i = 0; i < size_column; ++i){
                int index = i + size_column*j + size_column*size_row*k;
                float value = im.data[index];
                if (value > 1) im.data[index] = 1;
                else if (value < 0) im.data[index] = 0;
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float Red, Blue, Green, Hue, Saturation, Value, min, C, H_p;
    int size_column = im.w; 
    int size_row = im.h;
    for(int j = 0; j < size_row; ++j){
        for(int i = 0; i < size_column; ++i){
            Red = get_pixel(im, i, j, 0);
            Green = get_pixel(im, i, j, 1);
            Blue = get_pixel(im, i, j, 2);
            Value = three_way_max(Red,Green,Blue);
            min = three_way_min(Red,Green,Blue);
            C = Value - min;
            if ((Red == 0) && (Green == 0) && (Blue == 0)){
                Saturation = 0;
            }else {
                Saturation = C / Value;
            }

            if(C == 0){
                Hue = 0;
            }else{           
                if(Value == Red){
                    H_p = (Green - Blue) / C;
                } else if(Value == Green){
                    H_p = ((Blue - Red) / C) + 2;
                } else{
                    H_p = ((Red - Green) / C) + 4;
                }
                Hue = H_p / 6;
                if(H_p < 0){
                    Hue += 1;
                }
            }

            set_pixel(im, i, j, 0, Hue);
            set_pixel(im, i, j, 1, Saturation);
            set_pixel(im, i, j, 2, Value);

        }
    }

}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    float Red, Blue, Green, Hue, Saturation, Value, m, C, X, H_p;
    int size_column = im.w; 
    int size_row = im.h;
    for(int j = 0; j < size_row; ++j){
        for(int i = 0; i < size_column; ++i){
            Hue = get_pixel(im, i, j, 0);
            Saturation = get_pixel(im, i, j, 1);
            Value = get_pixel(im, i, j, 2);

            H_p = Hue * 6;
            C = Value * Saturation;
            X = C * (1 - fabs(fmod(H_p, 2) - 1));
            m = Value - C;
            Hue *= 360;

            if((Hue >= 0) && (Hue < 60)){
                Red = C;
                Green = X;
                Blue = 0;
            } else if((Hue >= 60) && (Hue < 120)){
                Red = X;
                Green = C;
                Blue = 0;
            } else if((Hue >= 120) && (Hue < 180)){
                Red = 0;
                Green = C;
                Blue = X;
            } else if((Hue >= 180) && (Hue < 240)){
                Red = 0;
                Green = X;
                Blue = C;
            } else if((Hue >= 240) && (Hue < 300)){
                Red = X;
                Green = 0;
                Blue = C;
            } else if((Hue >= 300) && (Hue < 360)){
                Red = C;
                Green = 0;
                Blue = X;
            } else{
                Red = 0;
                Green = 0;
                Blue = 0;
            }

            set_pixel(im, i, j, 0, (Red + m));
            set_pixel(im, i, j, 1, (Green + m));
            set_pixel(im, i, j, 2, (Blue + m));

        }
    }
}