#include <iostream>
#include <opencv2/opencv.hpp>
#include "Filter2.h"
#include"baseline.h"
#include"histogram.h"
#include"law.h"
#include"multihistogram.h"
#include"textureColor.h"
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

int main(int argc, char* argv[]) {
    Mat data_base;
    Mat target;
    Mat local;
    Mat local1;


    char dirname[256];
    char tar[256];
    char buffer[256];
    FILE* fp;
    DIR* dirp;
    struct dirent* dp;
    /*
    1. Target
    2. Dir_path
    3. Type
    4. total number of output
    */
    // check for sufficient arguments
    if (argc < 5) {
        printf("usage: \n%s1. <Target location>\n%s 2.<directory name>\n%s 3.<Type>\n%s 4.<Total number of output>", argv[0]);
        exit(-1);
    }

    // get the directory path
    strcpy_s(dirname, argv[2]);
    printf("Processing directory %s\n", dirname);
    strcpy_s(tar, argv[1]);
    // get the target path
    printf("Target File: %s", tar,"\n");
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }

    target = imread(tar, IMREAD_COLOR);
    imshow("target_image", target);
    //get the type
    printf(argv[3]);
    int choice = stoi(argv[3]);
    // get the total number of output
    int n = stoi(argv[4]);
    switch (choice) {
    case 1:
        baseline(dirname,tar);
        break;
    case 2:
        histogram(dirname,tar,n);
        break;
    case 3:
        multi_histogram(dirname, tar,n);
        break;
    case 4:
        texture_Color(dirname, tar,n);
        break;
    case 5:
        law(dirname, tar,n);
        break;
    default:
        printf("Wrong type entered");
        exit(0);
    }
    printf("Terminating\n");

    return(0);

}