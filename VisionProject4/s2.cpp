//
//  s2.cpp
//  VisionProject4
//
//  Created by Elizabeth Kelly on 11/4/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Function that computes the directions and intensities of the light sources.
 * image is the .pgm file to process.
 * fileName is the parameters file to read from.
 * Returns a string with the computed directions.
 **/
string ComputeNormalValues(Image * image, string fileName) {
    
    //parameters from the input file
    double radius, xC = 0, yC = 0;
    //to compute the normal vector and directions
    double xB = 0, yB = 0;
    double zChange = 0;
    double max_brightness = 0;
    
    //input the data from the parameters file
    ifstream inFile;
    inFile.open(fileName);
    inFile >> xC;
    inFile >> yC;
    inFile >> radius;
    inFile.close();
    
    //scan the image to find the point of maximum brightness
    for (int i = 0; i < image->num_rows(); ++i) {
        for (int j = 0; j < image->num_columns(); ++j) {
            if (image->GetPixel(i, j) > max_brightness) {
                max_brightness = image->GetPixel(i, j);
                xB = i;
                yB = j;
            }
        }
    }

    image->SetPixel(xC, yC, 0); //test
    image->SetPixel(xB, yB, 0); //test
    
    //equation of a 3d circle (x1-x2)^2 + (y1-y2)^2 + (z1-z2)^2 = r^2
    //we need to find z
    //z-zCenter = sqrt(r^2 + (x1-x2)^2 - (y1-y2)^2)
    
    int xChange = xB-xC; //x minus the center x point
    int yChange = yB-yC; //y minus the center y point
    zChange = sqrt((radius*radius) - (xChange*xChange) - (yChange*yChange));
    
    //scale the direction vector by the magnitude (brightest pixel) divided by the sphere radius
    double magnitude = max_brightness/radius;
    
    string line;
    ostringstream stream;
    
    stream << xChange*magnitude << " " << yChange*magnitude << " " << zChange*magnitude << endl;
    line = stream.str();
    
    return line;
}// end ComputeNormalValues

/**
 * Function that processes the input images and outputs to the output file.
 * Calls the function ComputeNormalValues to do the calculations.
 * in_file is parameters file from s1.
 * image1, image2, image3 are the immages with different lightsources
 * out_file is the output directions file.
 * Returns true if the images were read, processed, and directions file written.
 **/
bool ProcessFiles(string in_file, string image1, string image2, string image3, string out_file) {
    
    Image orig_image1;
    Image orig_image2;
    Image orig_image3;
    
    
    if (!ReadImage(image1, &orig_image1)) {
        cout <<"Can't open file " << image1 << endl;
        return false;
    }
    if (!ReadImage(image2, &orig_image2)) {
        cout <<"Can't open file " << image2 << endl;
        return false;
    }

    if (!ReadImage(image3, &orig_image3)) {
        cout <<"Can't open file " << image3 << endl;
        return false;
    }

    string str1 = ComputeNormalValues(&orig_image1, in_file);
    string str2 = ComputeNormalValues(&orig_image2, in_file);
    string str3 = ComputeNormalValues(&orig_image3, in_file);

    
    ofstream param_file;
    param_file.open(out_file);
    param_file << str1;
    param_file << str2;
    param_file << str3;
    param_file.close();

    return true;
}// end ProcessFiles

int main(int argc, char **argv){
    
    if (argc!=6) {
        cout << "Usage: <input param file> <image 1> <image 2> <image 3> <output directions file> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    const string image1(argv[2]);
    const string image2(argv[3]);
    const string image3(argv[4]);
    const string output_file(argv[5]);
    
    ProcessFiles(input_file, image1, image2, image3, output_file);
    
    return 0;
}
