
#include <iostream>
#include <opencv2/opencv.hpp>
#include "multihistogram.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include<vector>
#include <dirent.h>
using namespace std;
using namespace cv;

int multi_histogram(char* dirname, char* tar,int nos) {
    Mat data_base;
    Mat target;
    char buffer[256];
    FILE* fp;
    struct dirent* dp;
    DIR* dirp = opendir(dirname);
    const int Hsize = 8; /*no of bins equal to 8*/

    target = imread(tar, IMREAD_COLOR);
    imshow("target_image", target);
    int c = 0, i = 0, j = 0;
    int rix = 0, bix = 0, gix = 0;
    int divisor = 256 / Hsize;

    //calculating features for target histogram #1

    float* hist_3d1 = new float[Hsize * Hsize * Hsize];
    float* hist_3d2 = new float[Hsize * Hsize * Hsize];

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d1[i] = 0;
        hist_3d2[i] = 0;
    }

    for (int i = 0; i < target.rows / 2; i++) {
        for (int j = 0; j < target.cols; j++) {
            bix = (target.at<Vec3b>(i, j)[0]) / divisor;
            gix = (target.at<Vec3b>(i, j)[1]) / divisor;
            rix = (target.at<Vec3b>(i, j)[2]) / divisor;
            hist_3d1[rix * Hsize * Hsize + gix * Hsize + bix]++;

        }
    }

    //calculating features for target histogram #2

    for (int i = target.rows / 2; i < target.rows; i++) {
        for (int j = 0; j < target.cols; j++) {
            bix = (target.at<Vec3b>(i, j)[0]) / divisor;
            gix = (target.at<Vec3b>(i, j)[1]) / divisor;
            rix = (target.at<Vec3b>(i, j)[2]) / divisor;
            hist_3d2[rix * Hsize + gix * Hsize + bix * Hsize]++;

        }
    }
    //histogram normalization

    float histogram_sum = 0.0;
    float histogram_sum1 = 0.0;

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        histogram_sum += hist_3d1[i];
        histogram_sum1 += hist_3d2[i];
    }
    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d1[i] = hist_3d1[i] / histogram_sum;
        hist_3d2[i] = hist_3d2[i] / histogram_sum1;
    }
    vector<pair<float, string>> output;

    // loop over all the files in the image file listing

    while ((dp = readdir(dirp)) != NULL) {

        //check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif")) {

            printf("processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy_s(buffer, dirname);
            strcat_s(buffer, "/");
            strcat_s(buffer, dp->d_name);

            printf("full path name: %s\n", buffer);

            data_base = imread(buffer, IMREAD_COLOR);

            int local_rix = 0, local_gix = 0, local_bix = 0;

            //Initializng data_base histogram #1
            float* local_hist_3d1 = new float[Hsize * Hsize * Hsize];
            float* local_hist_3d2 = new float[Hsize * Hsize * Hsize];

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_hist_3d1[i] = 0;
                local_hist_3d2[i] = 0;
            }
            //Calculating features for data_base histogram #1

            for (int i = 0; i < data_base.rows / 2; i++) {
                for (int j = 0; j < data_base.cols; j++) {
                    local_bix = (data_base.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (data_base.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (data_base.at<Vec3b>(i, j)[2]) / divisor;
                    local_hist_3d1[local_rix * Hsize * Hsize + local_gix * Hsize + local_bix]++;

                }
            }
            //Calculating features for data_base histogram #2
            for (int i = data_base.rows / 2; i < data_base.rows; i++) {
                for (int j = 0; j < data_base.cols; j++) {
                    local_bix = (data_base.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (data_base.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (data_base.at<Vec3b>(i, j)[2]) / divisor;
                    local_hist_3d2[local_rix * Hsize + local_gix * Hsize + local_bix * Hsize]++;

                }
            }

            //normalising the histograms

            float local_histogram_sum = 0.0;
            float local_histogram_sum1 = 0.0;

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_histogram_sum += local_hist_3d1[i];
                local_histogram_sum1 += local_hist_3d2[i];
            }

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_hist_3d1[i] = local_hist_3d1[i] / local_histogram_sum;
                local_hist_3d2[i] = local_hist_3d2[i] / local_histogram_sum1;
            }

            //calculating histogram similiarity

            float sim_hist_3d1 = 0.0;
            float sim_hist_3d2 = 0.0;

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                sim_hist_3d1 += min(hist_3d1[i], local_hist_3d1[i]);
                sim_hist_3d2 += min(hist_3d2[i], local_hist_3d2[i]);
            }

            //histogram intersection

            float difference = ((1 - sim_hist_3d1) + (1 - sim_hist_3d2)) / 2;
            delete local_hist_3d1;
            delete local_hist_3d2;
            output.push_back({ difference, buffer });
        }
    }
    //sorting the image and arranging in ascending order.
    std::sort(output.begin(), output.end());
    Mat dst_frames;
    for (int i = 1; i < nos; i++) {
        cout << output[i].first << " " << output[i].second << endl;
        dst_frames = imread(output[i].second, IMREAD_COLOR);
        imshow("frame" + to_string(i) + "jpg", dst_frames);
    }


    char key = waitKey();
    while (1)
    {
        if (key == 'q') {
            destroyAllWindows();
            break;

        }
    }


    printf("Terminating\n");

    return(0);
}