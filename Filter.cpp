#include <iostream>
#include <opencv2/opencv.hpp>
#include "Filter.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/imgcodecs.hpp>



// Negative image
int Neg(cv::Mat& src, cv::Mat& dst) {
    int c, i, j;
    for (i = 0; i < src.rows; i++) {
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* rrptr = dst.ptr<cv::Vec3b>(i);
        for (j = 0; j < src.cols; j++) {
            rrptr[j][0] = 255 - rptr[j][0];
            rrptr[j][1] = 255 - rptr[j][1];
            rrptr[j][2] = 255 - rptr[j][2];
            
        }
    }
    return(0);
}

// Alternative gray scale live video
// Method :- Lumonosity Grayscale = 0.3 * R + 0.59*G + 0.11*B
int greyscale(cv::Mat& src, cv::Mat& dst) {

    int c, i, j;
    for (i = 0; i < src.rows; i++) {
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        uchar* dest = dst.ptr<uchar>(i);
        for (j = 0; j < src.cols; j++) {
            dest[j] = 0.11 * rptr[j][0]+ 0.59 * rptr[j][1] +0.3 * rptr[j][2];
            }
    }
    return (0);
}


// // a 5x5 Gaussian filter as separable 1x5 filters

int blur5x5(cv::Mat& src, cv::Mat& dst) {

    cv::Mat local;
    int c, i, j;
    src.copyTo(local);

    // [1 2 4 2 1] horizontal, divide the result by 10

    for (i = 2; i < src.rows - 2; i++) {
        //Pointer created for faster performance
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i); 
        cv::Vec3b* dest = local.ptr<cv::Vec3b>(i);
        for (j = 2; j < src.cols - 2; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptr[j - 2][c] * 0.1 + rptr[j - 1][c] * 0.2 + rptr[j][c] * 0.4 + rptr[j + 1][c] * 0.2 + rptr[j][c] * 0.1;
                }
        }
    }


    // [1 2 4 2 1] vertical,divide the result by 10

    for (i = 2; i < src.rows - 2; i++) {
        cv::Vec3b* rptrm2 = local.ptr<cv::Vec3b>(i-2);
        cv::Vec3b* rptrm1 = local.ptr<cv::Vec3b>(i-1);
        cv::Vec3b* rptr = local.ptr<cv::Vec3b>(i);
        cv::Vec3b* rptrp1 = local.ptr<cv::Vec3b>(i+1);
        cv::Vec3b* rptrp2 = local.ptr<cv::Vec3b>(i+2);
        cv::Vec3b* dest = dst.ptr<cv::Vec3b>(i);
        for (j = 2; j < src.cols - 2; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptrm2[j][c] * 0.1 + rptrm1[j][c] * 0.2 + rptr[j][c] * 0.4 + rptrp1[j][c] * 0.2 + rptrp2[j][c] * 0.1;
                }
        }
    }

    return (0);
}



// // a 3x3 Sobel X and 3x3 Sobel Y filter as separable 1x3 filters

// // sobelx

int sobelX3x3(cv::Mat& src, cv::Mat& dst) {
    cv::Mat local(src.rows, src.cols, CV_16SC3);
    int c, i, j;
    
    // [-1 0 1] horizontal, divide the result by 1
    
    for (i = 1; i < src.rows - 1; i++) {
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3s* dest = local.ptr<cv::Vec3s>(i);
        for (j = 1; j < src.cols - 1; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptr[j - 1][c] * (-1) + rptr[j][c] * 0 + rptr[j + 1][c] * 1;
            }
        }
    }


    // [1 2 1] vertical, divide the result by 4

    for (i = 1; i < src.rows - 1; i++) {
        cv::Vec3s* rptrm1 = local.ptr<cv::Vec3s>(i-1);
        cv::Vec3s* rptr = local.ptr<cv::Vec3s>(i);
        cv::Vec3s* rptrp1 = local.ptr<cv::Vec3s>(i + 1);
        cv::Vec3s* dest = dst.ptr<cv::Vec3s>(i);
        for (j = 1; j < src.cols - 1; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptrm1[j][c] * 0.25 + rptr[j][c] * 0.5 + rptrp1[j][c] * 0.25;
            }
        }
    }
    convertScaleAbs(dst, dst);
    return (0);
}

// sobely

int sobelY3x3(cv::Mat& src, cv::Mat& dst) {
    cv::Mat local(src.rows, src.cols, CV_16SC3);
    int c, i, j;

    // [1 2 1] horizontal, divide the result by 4

    for (i = 1; i < src.rows - 1; i++) {
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3s* dest = local.ptr<cv::Vec3s>(i);
        for (j = 1; j < src.cols - 1; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptr[j - 1][c] * 0.25 + rptr[j][c] * 0.5 + rptr[j + 1][c] * 0.25;
            }
        }
    }

    // [1 0 -1] vertical, divide the result by 1

    for (i = 1; i < src.rows - 1; i++) {
        cv::Vec3s* rptrm1 = local.ptr<cv::Vec3s>(i - 1);
        cv::Vec3s* rptr = local.ptr<cv::Vec3s>(i);
        cv::Vec3s* rptrp1 = local.ptr<cv::Vec3s>(i + 1);
        cv::Vec3s* dest = dst.ptr<cv::Vec3s>(i);
        for (j = 1; j < src.cols - 1; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptrm1[j][c] * 1 + rptr[j][c] * 0 + rptrp1[j][c] * (-1);
            }
        }
    }
    convertScaleAbs(dst, dst);
    return (0);
}

// blurs and quantizes a color image

int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels = 15) {
    blur5x5(src, dst);
    int c, i, j;
    int b = 255 / levels;
    for (i = 0; i < dst.rows; i++) {
        cv::Vec3b* rptr = dst.ptr<cv::Vec3b>(i);
        for (j = 0; j < dst.cols; j++) {
            for (c = 0; c < 3; c++) {
                int xt = rptr[j][c] / b;
                int xf = xt * b;
                rptr[j][c] = xf;
            }
        }
    }
    return (0);
}

// //  a gradient magnitude image from the X and Y Sobel images

// // Euclidean distance for magnitude: I = sqrt( sx*sx + sy*sy )

int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst) {
    int c, i, j;
    int sum_squareroot;
    for (i = 0; i < sx.rows; i++) {
        cv::Vec3b* xptr = sx.ptr<cv::Vec3b>(i);
        cv::Vec3b* yptr = sy.ptr<cv::Vec3b>(i);
        cv::Vec3b* dest = dst.ptr<cv::Vec3b>(i);
        for (j = 0; j < sx.cols; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = sqrt(xptr[j][c] * xptr[j][c] + yptr[j][c] * yptr[j][c]);
                }
        }
    }
    return(0);
}
// cartoonization function using the gradient magnitude and blur/quantize filters
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold) {
    cv::Mat magn, blurq;
    src.copyTo(magn);
    src.copyTo(blurq);
    cv::Mat sx(src.rows, src.cols, CV_16SC3);
    cv::Mat sy(src.rows, src.cols, CV_16SC3);
    int c, i, j;
    sobelX3x3(src, sx);
    sobelY3x3(src, sy);
    magnitude(sx, sy, magn);
    blurQuantize(src, blurq, levels);
    for (i = 0; i < magn.rows; i++) {
        cv::Vec3b* mptr = magn.ptr<cv::Vec3b>(i);
        cv::Vec3b* bptr = blurq.ptr<cv::Vec3b>(i);
        cv::Vec3b* dest = dst.ptr<cv::Vec3b>(i);
        for (j = 0; j < magn.cols; j++) {
            for (c = 0; c < 3; c++) {
                if (mptr[j][c] > magThreshold) {
                    dest[j][c] = 0;
                }
                else {
                    dest[j][c] = bptr[j][c];
                }
            }
        }
    }
    return (0);
}