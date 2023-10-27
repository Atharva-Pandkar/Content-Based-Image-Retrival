#include <opencv2/opencv.hpp>
#include "textureColor.h"
#include"Filter.h"
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

int texture_Color(char* dirname, char* tar,int nos) {
    Mat data_base;
    Mat target;
    Mat local1;
    Mat magn;
    Mat orienration;
    Mat local_magn;
    Mat local_orienration;
    char buffer[256];
    FILE* fp;
    struct dirent* dp;

    target = imread(tar, IMREAD_COLOR);
    imshow("target_image", target);

    // using gradient magnitude to calculate the texture
    Mat sx(target.rows, target.cols, CV_16SC3);
    Mat sy(target.rows, target.cols, CV_16SC3);
    sobelX3x3(target, sx);
    sobelY3x3(target, sy);
    
    target.copyTo(magn);
    magnitude(sx, sy, magn);

    
    const int Hsize = 8; /*no of bins equal to 8*/
    int c = 0;
    int i = 0;
    int j = 0;
    int rix = 0, bix = 0, gix = 0;
    int divisor = 256 / Hsize;

    //calculating features for target histogram 

    float* hist_3d = new float[Hsize * Hsize * Hsize];
    float* text = new float[Hsize * Hsize * Hsize];
    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d[i] = 0;
        text[i] = 0;
    }


    for (int i = 0; i < target.rows; i++) {
        for (int j = 0; j < target.cols; j++) {
            bix = (target.at<Vec3b>(i, j)[0]) / divisor;
            gix = (target.at<Vec3b>(i, j)[1]) / divisor;
            rix = (target.at<Vec3b>(i, j)[2]) / divisor;
            hist_3d[rix * Hsize * Hsize + gix * Hsize + bix]++;
        }
    }

    //calculating texture for target histogram 
    for (int i = 0; i < magn.rows; i++) {
        for (int j = 0; j < magn.cols; j++) {
            bix = (magn.at<Vec3b>(i, j)[0]) / divisor;
            gix = (magn.at<Vec3b>(i, j)[1]) / divisor;
            rix = (magn.at<Vec3b>(i, j)[2]) / divisor;
            text[rix * Hsize * Hsize + gix * Hsize + bix]++;
            
        }
    }

    //histogram normalization
    float histogram_sum = 0.0;
    float histogram_sum1 = 0.0;

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        histogram_sum += hist_3d[i];
        histogram_sum1+= text[i];
    }
    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d[i] = hist_3d[i] / histogram_sum;
        text[i] = text[i] / histogram_sum1;
    }


    vector<pair<float, string>> output;
    DIR* dirp =opendir( dirname);
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

            

//calculations for data base images

            // using gradient magnitude to calculate the texture
            data_base = imread(buffer, IMREAD_COLOR);
            Mat local_sx(data_base.rows, data_base.cols, CV_16SC3);
            Mat local_sy(data_base.rows, data_base.cols, CV_16SC3);
            data_base.copyTo(local_magn);
            sobelX3x3(data_base, local_sx);
            sobelY3x3(data_base, local_sy);
            magnitude(local_sx, local_sy, local_magn);


            int local_rix = 0, local_gix = 0, local_bix = 0;

            float* local_hist_3d = new float[Hsize * Hsize * Hsize];
            float* local_text = new float[Hsize * Hsize * Hsize];

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_text[i] = 0;
                local_hist_3d[i] = 0;
            }

            //calculating features for data base histogram
            for (int i = 0; i < data_base.rows; i++) {
                for (int j = 0; j < data_base.cols; j++) {
                    local_bix = (data_base.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (data_base.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (data_base.at<Vec3b>(i, j)[2]) / divisor;
                    local_hist_3d[local_rix * Hsize * Hsize + local_gix * Hsize + local_bix]++;

                }
            }
            //calculating textures for data_base histogram
            for (int i = 0; i < local_magn.rows; i++) {
                for (int j = 0; j < local_magn.cols; j++) {
                    local_bix = (local_magn.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (local_magn.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (local_magn.at<Vec3b>(i, j)[2]) / divisor;
                    local_text[local_rix * Hsize * Hsize + local_gix * Hsize + local_bix]++;

                }
            }

            //histogram normalization
            float local_histogram_sum = 0.0;
            float local_histogram_sum1 = 0.0;

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_histogram_sum += local_hist_3d[i];
                local_histogram_sum1 += local_text[i];
            }

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {

                local_hist_3d[i] = local_hist_3d[i] / local_histogram_sum;
                local_text[i] = local_text[i] / local_histogram_sum1;
                
            }

            //calculating histogram similiarity
            float sim_hist3d = 0.0;
            float sim_text = 0.0;


            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                sim_hist3d += min(hist_3d[i], local_hist_3d[i]);
                sim_text += min(text[i], local_text[i]);

            }
            cout << sim_hist3d << endl;
            
            //histogram intersection
            float difference = (0.5 * (1 - sim_hist3d) + 0.5 * (1 - sim_text));

            delete local_hist_3d;
            delete local_text;
            output.push_back({ difference, buffer });

        }
    };
    //sorting the image and arranging in ascending order
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
