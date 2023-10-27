#include <iostream>
#include <opencv2/opencv.hpp>
#include"Filter.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include<dirent.h>
using namespace cv;
using namespace std;


int laws5x5(Mat& src, Mat& dst) {

    cv::Mat local(src.rows, src.cols, CV_16SC3);
    int c, i, j;
    src.copyTo(local);

    // [1 4 6 4 1] L5, divide the result by 16

    for (i = 2; i < src.rows - 2; i++) {
        cv::Vec3b* rptrp2 = src.ptr<cv::Vec3b>(i - 2);
        cv::Vec3b* rptrp1 = src.ptr<cv::Vec3b>(i - 1);
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* rptr1 = src.ptr<cv::Vec3b>(i + 1);
        cv::Vec3b* rptr2 = src.ptr<cv::Vec3b>(i + 2);
        cv::Vec3b* dest = local.ptr<cv::Vec3b>(i);
        for (j = 2; j < src.cols - 2; j++) {

            for (c = 0; c < 3; c++) {
                dest[j][c] = rptrp2[j][c] * 0.0625 +
                    rptrp1[j][c] * 0.25 +
                    rptr[j][c] * 0.375 +
                    rptr1[j][c] * 0.25 +
                    rptr2[j][c] * 0.0625;
            }
        }
    }


    // [-1 -2 0 2 1] E5 ,divide the result by 1

    for (i = 2; i < src.rows - 2; i++) {
        cv::Vec3b* rptr = local.ptr<cv::Vec3b>(i);
        cv::Vec3b* dest = dst.ptr<cv::Vec3b>(i);
        for (j = 2; j < src.cols - 2; j++) {
            for (c = 0; c < 3; c++) {
                dest[j][c] = rptr[j - 2][c] * (-1) +
                    rptr[j - 1][c] * (-2) +
                    rptr[j][c] * 0 +
                    rptr[j + 1][c] * 2 +
                    rptr[j + 2][c] * 1;
            }
        }
    }
    convertScaleAbs(dst, dst);


    return (0);
}

int law(char* dirname, char* tar,int nos) {
    Mat data_base;
    Mat target;
    Mat local;
    Mat local1;
    DIR* dirp = opendir(dirname);

    char buffer[256];
    FILE* fp;
    struct dirent* dp;
    target = imread(tar, IMREAD_COLOR);
    imshow("target_image", target);
    //using L5 and E5 laws filter to count the dustbins

    Mat temp(target.rows, target.cols, CV_16SC3);
    target.copyTo(local);
    laws5x5(temp, local);
    laws5x5(temp, local);

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
            //calculating features for target texture
            bix = (local.at<Vec3b>(i, j)[0]) / divisor;
            gix = (local.at<Vec3b>(i, j)[1]) / divisor;
            rix = (local.at<Vec3b>(i, j)[2]) / divisor;
            text[rix * Hsize * Hsize + gix * Hsize + bix]++;
        }
    }
    
    

    //histogram normalization
    float histogram_sum = 0.0;
    float histogram_sum1 = 0.0;

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        histogram_sum += hist_3d[i];
        histogram_sum1 += text[i];
    }

    for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
        hist_3d[i] = hist_3d[i] / histogram_sum;
        text[i] = text[i] / histogram_sum1;
    }
    // float hist = 1, texture = 2;

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

            
//calculations for data base images

            data_base = imread(buffer, IMREAD_COLOR);
            Mat temp1(data_base.rows, data_base.cols, CV_16SC3);
            data_base.copyTo(local1);
            laws5x5(temp1, local1);
            int local_rix = 0, local_gix = 0, local_bix = 0;
            float* local_hist_3d = new float[Hsize * Hsize * Hsize];

            float* local_text = new float[Hsize * Hsize * Hsize];
            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                local_hist_3d[i] = 0;
                local_text[i] = 0;
            }

            //calculating histogram for data base
            for (int i = 0; i < data_base.rows; i++) {
                for (int j = 0; j < data_base.cols; j++) {
                    local_bix = (data_base.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (data_base.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (data_base.at<Vec3b>(i, j)[2]) / divisor;
                    local_hist_3d[local_rix * Hsize * Hsize + local_gix * Hsize + local_bix]++;

                    // calculating texture
                    local_bix = (local1.at<Vec3b>(i, j)[0]) / divisor;
                    local_gix = (local1.at<Vec3b>(i, j)[1]) / divisor;
                    local_rix = (local1.at<Vec3b>(i, j)[2]) / divisor;
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

            // float hist = 1, texture = 2;                 

        //calculating histogram similiarity
            float sim_hist3d = 0.0;
            float sim_text = 0.0;

            for (int i = 0; i < Hsize * Hsize * Hsize; i++) {
                sim_hist3d += min(hist_3d[i], local_hist_3d[i]);
                sim_text += min(text[i], local_text[i]);

            }
            cout << sim_hist3d << endl;
            //histogram intersection

            float difference = (0.7 * (1 - sim_hist3d) + 0.3 * (1 - sim_text));

            delete local_hist_3d;
            delete local_text;
            output.push_back({ difference, buffer });

        }
    }
    //sorting the image and arranging in ascending order
    std::sort(output.begin(), output.end());
    Mat dst_frames;
    for (int i = 1; i < 10; i++) {
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


