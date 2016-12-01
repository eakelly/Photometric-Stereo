//
//  main.cpp
//  VisionProject4
//
//  Created by Elizabeth Kelly on 10/27/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Function that finds the location of the sphere using thresholding and then
 * computes the sphere’s center and radius.
 * in_file is the gray-level .pgm file to scan.
 * threshold is the value to threshold
 * out_file is the parameters file for the sphere object.
 * Returns true if the image was read and data output to file.
 **/
bool ConvertToBinary(string in_file, string out_file, int threshold) {
    
    Image orig_image;
    
    //to compute the centroid
    double area = 0.0;
    int x_bar = 0, y_bar = 0;
    
    if (!ReadImage(in_file, &orig_image)) {
        cout <<"Can't open file " << in_file << endl;
        return false;
    }
    
    size_t rows = orig_image.num_rows();
    size_t cols = orig_image.num_columns();

    //scan the image and set the pixels using the threshold
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            
            if (orig_image.GetPixel(i, j) < threshold) {
                orig_image.SetPixel(i, j, 0); //make the pixel black
            }
            else { //if the pixel is > threshold
                orig_image.SetPixel(i, j, 255); //make the pixel white
                area++;
                x_bar = x_bar + i;
                y_bar = y_bar + j;
            }
        }
    }
    //set number of colors in PGM as 1 in the header
    orig_image.SetNumberGrayLevels(1);
    
    x_bar = x_bar/area;
    y_bar = y_bar/area;
    orig_image.SetPixel(x_bar, y_bar, 0);
    
    size_t x_min = rows, y_min = cols;
    int x_max = 0, y_max = 0;
    
    //to get the diameter
    int x_top = 0, y_top = 0;
    int x_left = 0, y_left = 0;
    int x_bottom = 0, y_bottom = 0;
    int x_right = 0, y_right = 0;
    
    int radius = 0;
    //get the topmost, bottomost, leftmost, and rightmost pixel coordinates
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (orig_image.GetPixel(i, j) > 0) {
                // the leftmost point of the circle will have the smallest y coordinate
                if (j < y_min) {
                    y_min = j;
                    x_left = i;
                    y_left = j;
                }
                // the rightmost point of the circle will have the greatest y coordinate
                if (j > y_max) {
                    y_max = j;
                    x_right = i;
                    y_right = j;
                }
                // the topmost point of the circle will have the smallest x coordinate
                if (i < x_min) {
                    x_min = i;
                    x_top = i;
                    y_top = j;
                }
                // the bottommost point of the circle will have the greatest x coordinate
                if (i > x_max) {
                    x_max = i;
                    x_bottom = i;
                    y_bottom = j;
                }
            }
            
        }
    }

    radius = sqrt(area/M_PI);
    
    // use the distance formula to calculate the distance between two points and find the radius
    // radius = sqrt((x-a)^2 + (y-b)^2) where a,b are the coordinates of the circle center
    
    //distance from topmost pixel to bottommost
    double d1 = (x_top-x_bottom)*(x_top-x_bottom);
    double d2 = (y_top-y_bottom)*(y_top-y_bottom);
    //distance from leftmost pixel to rightmost
    double d3 = (x_left-x_right)*(x_left-x_right);
    double d4 = (y_left-y_right)*(y_left-y_right);
    
    double distance1 = sqrt(d1+d2);
    double distance2 = sqrt(d3+d4);
    
    //the averages of the two distances divided by 2
    radius = ((distance1+distance2)/2)/2;
    
    ofstream param_file;
    param_file.open(out_file);
    param_file << x_bar << " " << y_bar << " " << radius;
    param_file.close();

    return true;
}// end ConvertToBinary

int main(int argc, char **argv){
    
    if (argc!=4) {
        cout << "Usage: <input original image> <threshold value> <output parameters file> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    int threshold = atoi(argv[2]); //90 is best
    const string output_file(argv[3]);
    
    ConvertToBinary(input_file, output_file, threshold);
    
    return 0;
}
