#include <iostream>
#include <opencv2/opencv.hpp>
#include "baseline.h"
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

int baseline(char* dirname,char* tar) {

    int numberOfImages = 5;
    char buffer[256];
    FILE* fp;
    DIR* dirp;
    struct dirent* dp;
    int a = 0;
    Mat target;
    Mat base_nine;
    Mat local;
    Mat close1;
    Mat close2;
    Mat close3;

    dirp = opendir(dirname);
    string path[1106];

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
            path[a] = strcat_s(buffer, dp->d_name);
            path[a] = buffer;
            a++;
        }
    }
    int n = sizeof(path) / sizeof(path[0]);
    sort(path, path + n);

    int b = 0;
    target = imread(tar);
    imshow("target_image", target);

    vector<vector<int>> v(1, vector<int>(245, 0));
    // create the histogram for target image
    int px = (target.rows / 2) - 4;
    int py = (target.cols / 2) - 4;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int c = 0; c < 3; c++) {
                v[0][(i * 9) + j + (c * 9 * 9)] = target.at<cv::Vec3b>(px + i, py + j)[c];
            }
        }
    }
    ///data base//

    vector<vector<float>> value(1106, vector<float>(2));
    vector<string> p;

    for (int b = 0; b < 1106; b++) {
        Mat data_base = imread(path[b], IMREAD_COLOR);
        vector<vector<int>> vi(1106, vector<int>(243, 0));
        // get the histogram for database
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                for (int c = 0; c < 3; c++) {
                    vi[b][i * 9 + j + c * 9 * 9] = data_base.at<cv::Vec3b>(data_base.rows / 2 - 4 + i, data_base.cols / 2 - 4 + j)[c];
                }
            }
        }
        value[b][0] = 0;
        //calculate baseline
        for (int i = 0; i < 243; i++) {
            value[b][0] += (v[0][i] - vi[b][i]) * (v[0][i] - vi[b][i]);
        }

        value[b][1] = b;

    }

    sort(value.begin(), value.end());

    cout << " top closest images : " << endl;

    for (int b = 0; b < 1106; b++) {
        cout << "Distance : " << value[b][0] << "path :" << value[b][1] << endl;
    }
    close1 = imread(path[int(value[1][1])], IMREAD_COLOR);
    close2 = imread(path[int(value[2][1])], IMREAD_COLOR);
    close3 = imread(path[int(value[3][1])], IMREAD_COLOR);
    imshow("Zero", imread(path[int(value[0][1])], IMREAD_COLOR));
    imshow("First", close1);
    imshow("Second", close2);
    imshow("Third", close3);

    while (true) {
        b = waitKey(0);

        if (b == 113) {
            destroyAllWindows();
            break;
        }
    }
    return 0;

}