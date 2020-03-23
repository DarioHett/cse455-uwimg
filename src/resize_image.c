#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "image.h"

// Emilio Moyers' (@emoyers) github repo was a lifesaver.
// There are plenty of repos floating around which produce a common error.

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    float result = 0;
    x = round(x);
    y = round(y);
    result = get_pixel(im, x, y, c);
    return result;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    float ax = (float)(im.w) / (float)(w);
    float ay = (float)(im.h) / (float)(h);
    float bx = 0.5 * (ax - 1);
    float by = 0.5 * (ay - 1);
    int size_column = w; 
    int size_row = h;
    int size_channel = im.c;
    int index = 0;
    float map_value_x = 0;
    float map_value_y = 0;
    image nn_resize_img = make_image(size_column, size_row, size_channel);
    // TODO Fill this in
    nn_resize_img.data = calloc(size_column*size_row*size_channel, sizeof(float));

    for(int k = 0; k < size_channel; ++k){
        for(int j = 0; j < size_row; ++j){
            for(int i = 0; i < size_column; ++i){
                index = i + size_column*j + size_column*size_row*k;
                map_value_x = (ax * i) + bx;
                map_value_y = (ay * j) + by;
                nn_resize_img.data[index] = nn_interpolate(im, map_value_x, map_value_y, k);
            }
        }
    }
    return nn_resize_img;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float result = 0;
    int x_high = 0;
    int x_low = 0;
    int y_high = 0;
    int y_low = 0;
    float pixel_top_left = 0.0f;
    float pixel_top_right = 0.0f;
    float pixel_bottom_left = 0.0f;
    float pixel_bottom_right = 0.0f;
    float rectangle_top_left = 0.0f;
    float rectangle_top_right = 0.0f;
    float rectangle_bottom_left = 0.0f;
    float rectangle_bottom_right = 0.0f; 
    if (x > round(x)){
        x_high = round(x) + 1;
        x_low = round(x);
    }
    else{
        x_high = round(x);
        x_low = round(x) - 1;        
    } 

    if (y > round(y)){
        y_high = round(y) + 1;
        y_low = round(y);
    }
    else{
        y_high = round(y);
        y_low = round(y) - 1;
    } 


    pixel_top_left = get_pixel(im, x_low, y_high, c);
    pixel_top_right = get_pixel(im, x_high, y_high, c);
    pixel_bottom_left = get_pixel(im, x_low, y_low, c);
    pixel_bottom_right = get_pixel(im, x_high, y_low, c);

    rectangle_top_left = (x - x_low) * (y_high - y);
    rectangle_top_right = (x_high - x) * (y_high - y);
    rectangle_bottom_left = (x - x_low) * (y - y_low);
    rectangle_bottom_right = (x_high - x) * (y - y_low);


    result = (rectangle_top_left * pixel_bottom_right) + (rectangle_top_right * pixel_bottom_left) + (rectangle_bottom_left * pixel_top_right) + (rectangle_bottom_right * pixel_top_left);
    return result;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    float ax = (float)(im.w) / (float)(w);
    float ay = (float)(im.h) / (float)(h);
    float bx = 0.5 * (ax - 1);
    float by = 0.5 * (ay - 1);
    int size_column = w; 
    int size_row = h;
    int size_channel = im.c;
    int index = 0;
    float map_value_x = 0;
    float map_value_y = 0;
    image bilinear_resize_img = make_image(size_column, size_row, size_channel);
    // TODO Fill this in
    bilinear_resize_img.data = calloc(size_column*size_row*size_channel, sizeof(float));

    for(int k = 0; k < size_channel; ++k){
        for(int j = 0; j < size_row; ++j){
            for(int i = 0; i < size_column; ++i){
                index = i + size_column*j + size_column*size_row*k;
                map_value_x = (ax * i) + bx;
                map_value_y = (ay * j) + by;
                bilinear_resize_img.data[index] = bilinear_interpolate(im, map_value_x, map_value_y, k);
            }
        }
    }
    return bilinear_resize_img;
}