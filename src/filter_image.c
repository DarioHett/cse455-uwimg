#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    int index;
    float c_sum[im.c];
    for(int k = 0; k < im.c; k++){
        c_sum[k] = 0;
        for(int j = 0; j < im.h; j++){
            for(int i = 0; i < im.w; i++){
                index = i + im.h*j + im.h*im.w*k;
                c_sum[k] += im.data[index];
            }
        }
    }
    for(int k = 0; k < im.c; k++){
        for(int j = 0; j < im.h; j++){
            for(int i = 0; i < im.w; i++){
                index = i + im.h*j + im.h*im.w*k;
                im.data[index] /= c_sum[k];
            }
        }
    }
}

image make_box_filter(int w)
{
    image fi = make_image(w,w,1);
    for (int i = 0; i < w*w; i++) {
    fi.data[i] = 1.0f / (float)(w*w);
    }
    
    return fi;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == im.c || filter.c == 1);
    image out;
    image tmp_im = make_image(im.w,im.h,im.c);
    float v,w;
    int kf = 0;
    w = filter.w;
    
    for (int k = 0; k < im.c; k++){
        if (k < filter.c) {kf = k;}
        for (int j = 0; j < im.h; j++){
            for (int i = 0; i < im.w; i++){
                v = 0;
                for (int fx = 0; fx < w; fx++){
                    for (int fy = 0; fy < w; fy++) {
                        v += get_pixel(im, i - floor(w / 2) + fx, j - floor(w / 2) + fy, k) * get_pixel(filter, fx, fy, kf);
                    }
                }
                set_pixel(tmp_im, i, j, k, v);
            }
        }
    }
    if (im.c == filter.c && preserve == 0) {
        v = 0.;
        out = make_image(im.w,im.h,1);
        for (int j = 0; j < tmp_im.h; j++){
            for (int i = 0; i < tmp_im.w; i++){
                v = 0.;
                for (int k = 0; k < tmp_im.c; k++){
                    v += get_pixel(tmp_im, i, j, k);
                }
                set_pixel(out, i, j, 0, v);
            }
        }
    } else if (im.c == filter.c && preserve == 1) {
        out = copy_image(tmp_im);
        //l1_normalize(out);
    } else if (im.c > 1 && filter.c == 1 && preserve == 0) {
        v = 0.;
        out = make_image(im.w,im.h,1);
        for (int j = 0; j < tmp_im.h; j++){
            for (int i = 0; i < tmp_im.w; i++){
                v = 0.;
                for (int k = 0; k < tmp_im.c; k++){
                    v += get_pixel(tmp_im, i, j, k);
                }
                set_pixel(out, i, j, 0, v);
            }
        }
    } else if (im.c > 1 && filter.c == 1 && preserve == 1) {
        out = copy_image(tmp_im);
        //l1_normalize(out);
    };
    return out;
 
}

image make_highpass_filter()
{
    float array[9] = {0.0f,(-1.0f),0.0f,(-1.0f),(4.0f),(-1.0f),(0.0f),(-1.0f),(0.0f)};
    image filter = make_image(3,3,1);
    for (int i = 0; i < 9; i++)
    {
        filter.data[i] = array[i];
    }
    return filter;
}

image make_sharpen_filter()
{
    float array[9] = {0,-1,0,-1,5,-1,0,-1,0};
    image filter = make_image(3,3,1);
    for (int i = 0; i < 9; i++)
    {
        filter.data[i] = array[i];
    }
    return filter;
}

image make_emboss_filter()
{
    float array[9] = {-2,-1,0,-1,1,1,0,1,2};
    image filter = make_image(3,3,1);
    for (int i = 0; i < 9; i++)
    {
        filter.data[i] = array[i];
    }
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma) {
    float v, var;
    int w = ceil(6.f*(float)sigma);
    w = ((w%2 == 1) ? w : w+1);
    image out = make_image(w,w,1);
    int center = floor(w/2); 
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            float x = (float)(i-center);
            float y = (float)(j-center);
            var = sigma*sigma;
            v = exp((-1.0f)*(x*x+y*y)/(2*var))/(TWOPI*var);
//            printf("x: %i, y: %i, v:%.2f", i,j,v);
            set_pixel(out, i, j, 0, v);
//            l1_normalize(out);
        }
    }
    l1_normalize(out);
    return out;
}

image add_image(image a, image b) {
    // TODO
    assert((a.h == b.h) && (a.w == b.w) && (a.c == b.c));
    image out = make_image(a.w,a.h,a.c);
    float v;
    for (int k = 0; k < a.c; k++)    {
        for (int j = 0; j < a.h; j++)        {
            for (int i = 0; i < a.w; i++)            {
                v = 0.0f;
                v += get_pixel(a,i,j,k);
                v += get_pixel(b,i,j,k);
                set_pixel(out,i,j,k,v);
            }
        }
    }
    return out;
}

image sub_image(image a, image b) {
    assert((a.h == b.h) && (a.w == b.w) && (a.c == b.c));
    image out = make_image(a.w,a.h,a.c);
    float v;
    for (int k = 0; k < a.c; k++)    {
        for (int j = 0; j < a.h; j++)        {
            for (int i = 0; i < a.w; i++)            {
                v = 0.0f;
                v += get_pixel(a,i,j,k);
                v -= get_pixel(b,i,j,k);
                set_pixel(out,i,j,k,v);
            }
        }
    }
    return out;
}

image make_gx_filter() {
    float array[9] = {-1,0,1,-2,0,2,-1,0,1};
    image filter = make_image(3,3,1);
    for (int i = 0; i < 9; i++)
    {
        filter.data[i] = array[i];
    }
    return filter;
}

image make_gy_filter() {
    float array[9] = {-1,0,1,-2,0,2,-1,0,1};
    image filter = make_image(3,3,1);
    for (int i = 0; i < 3; i++)
    {
        filter.data[i*3] = array[i];
        filter.data[i*3+1] = array[i+3];
        filter.data[i*3+2] = array[i+6];
    }
    return filter;
}

void feature_normalize(image im) {
    float min, max;
    for (int i = 0; i < im.w*im.c*im.h; i++)    {
        min = ((im.data[i] < min) ? im.data[i] : min);
        max = ((im.data[i] > max) ? im.data[i] : max);
    }
    float rng = max-min;
    for (int i = 0; i < im.w*im.c*im.h; i++)    {
        im.data[i] = ((rng == 0.0f) ? 0.0 : (float)((im.data[i]-min)/rng));
    }
}

image *sobel_image(image im) {
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();
    image G_x = convolve_image(im, gx_filter, 0);
    image G_y = convolve_image(im, gy_filter, 0);
    image res_1 = make_image(im.w, im.h, 1);
    image res_2 = make_image(im.w, im.h, 1);

    for (int i = 0; i < G_x.w*G_x.h; i++)    {
        float gx = G_x.data[i];
        float gy = G_y.data[i];
        res_1.data[i] = (float)pow((gx*gx + gy*gy),0.5);
        res_2.data[i] = (float)atan2(gy,gx);
    }
    image *result;
    result = calloc(2, sizeof(res_1));
    result[0] = res_1;
    result[1] = res_2;

    free_image(gx_filter);
    free_image(gy_filter);
    free_image(G_x);
    free_image(G_y);

    return result;
}

image colorize_sobel(image im) {
    image filter = make_gaussian_filter(2);
    image blur_im = convolve_image(im, filter, 1);

    image *sobel = sobel_image(blur_im);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);

    image trans = make_image(im.w,im.h,im.c);
    for (int i = 0; i < im.w*im.h*im.c; i++) {
        trans.data[i] = (((i / (im.w*im.h)) < 1) ? (float)sobel[1].data[i%(im.w*im.h)] : (float)sobel[0].data[i%(im.w*im.h)]);
    }
    hsv_to_rgb(trans);
    free_image(blur_im);
    free_image(filter);
    free(sobel);

    return trans;
}
