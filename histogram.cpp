
#include <iostream>
#include <opencv2/opencv.hpp>
#include "histogram.h"
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


int histogram(char* dirname,char* tar, int nos) {
    Mat data_base;
    Mat dst;
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
    //calculating features for target histogram

    float* hist_3d = new float[Hsize * Hsize * Hsize];
    //basic inititlization
    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d[i] = 0;
    }

    for (int i = 0; i < target.rows; i++) {
        for (int j = 0; j < target.cols; j++) {
            bix = (target.at<Vec3b>(i, j)[0]) / divisor;
            gix = (target.at<Vec3b>(i, j)[1]) / divisor;
            rix = (target.at<Vec3b>(i, j)[2]) / divisor;
            hist_3d[rix * Hsize * Hsize + gix * Hsize + bix]++;
        }
    }

    ////////////histogram normalization////////////////////
    float histogram_sum = 0.0;

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        histogram_sum += hist_3d[i];
    }

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d[i] = hist_3d[i] / histogram_sum;
    }

    vector<pair<float, string>> output;

    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {

        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif")) {

            // build the overall filename
            strcpy_s(buffer, dirname);
            strcat_s(buffer, "/");
            strcat_s(buffer, dp->d_name);

            data_base = imread(buffer, IMREAD_COLOR);
            int local_rix = 0, local_gix = 0, local_bix = 0;

            //Initializng data_base histogram

            float* local_hist_3d = new float[Hsize * Hsize * Hsize];
            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_hist_3d[i] = 0;
            }
            //Calculating features for data_base histogram
            for (int i = 0; i < data_base.rows; i++) {
                for (int j = 0; j < data_base.cols; j++) {

                    local_bix = (data_base.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (data_base.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (data_base.at<Vec3b>(i, j)[2]) / divisor;
                    local_hist_3d[local_rix * Hsize * Hsize + local_gix * Hsize + local_bix]++;

                }
            }

            //normalising the histograms

            float local_histogram_sum = 0.0;
            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_histogram_sum += local_hist_3d[i];
            }
            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_hist_3d[i] = local_hist_3d[i] / local_histogram_sum;
            }

            //calculating histogram similiarity
            float sim_hist_3d = 0.0;

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                sim_hist_3d += min(hist_3d[i], local_hist_3d[i]);
            }

            //histogram intersection//
            float difference = (1 - sim_hist_3d);


            delete local_hist_3d;
            output.push_back({ difference, buffer });

        }


    };
    ////sorting the image and arranging in ascending order.//////

    Mat dst_frames;
    std::sort(output.begin(), output.end());

    for (int i = 1; i < 4; i++) {
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