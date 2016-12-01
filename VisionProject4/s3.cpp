//
//  s3.cpp
//  VisionProject4
//
//  Created by Elizabeth Kelly on 11/6/16.
//  Copyright © 2016 Elizabeth Kelly. All rights reserved.
//

#include "Image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Function that computes the normals.
 * S is the vector of directions.
 * image1, image2, image3 are the images to process.
 * step is the value to increment by
 * threshold is the value to be used as a threshold
 * out_file is the output needle map image.
 * Returns true if the images were read, processed, and written.
 **/
bool ComputeNormal(vector<double> S, Image *image1, Image *image2, Image *image3, int step, int threshold, string out_file) {
    
    double S_matrix[3][3];
    
    S_matrix[0][0] = S[0];
    S_matrix[0][1] = S[1];
    S_matrix[0][2] = S[2];
    S_matrix[1][0] = S[3];
    S_matrix[1][1] = S[4];
    S_matrix[1][2] = S[5];
    S_matrix[2][0] = S[6];
    S_matrix[2][1] = S[7];
    S_matrix[2][2] = S[8];
    
    // invert the values (S) matrix
    // compute rho * n to get N (rho is length (|N|) of N, n is N/|N|
    // when we get n, we get rho which is albedo
    
    double det_of_n = (S_matrix[0][0] * (S_matrix[1][1]*S_matrix[2][2] - S_matrix[1][2]*S_matrix[2][1])) - (S_matrix[0][1] * (S_matrix[1][0]*S_matrix[2][2] - S_matrix[1][2]*S_matrix[2][0]))
        + (S_matrix[0][2] * (S_matrix[1][0]*S_matrix[2][1] - S_matrix[1][1]*S_matrix[2][0]));

    double invert_det = 1/det_of_n;
    //invert matrix S
    double S_inv_mat[3][3];
    
    S_inv_mat[0][0] = (S_matrix[1][1] * S_matrix[2][2] - S_matrix[2][1]*S_matrix[1][2])*invert_det;
    S_inv_mat[0][1] = (S_matrix[0][2] * S_matrix[2][1] - S_matrix[0][1]*S_matrix[2][2])*invert_det;
    S_inv_mat[0][2] = (S_matrix[0][1] * S_matrix[1][2] - S_matrix[0][2]*S_matrix[1][1])*invert_det;
    S_inv_mat[1][0] = (S_matrix[1][2] * S_matrix[2][0] - S_matrix[1][0]*S_matrix[2][2])*invert_det;
    S_inv_mat[1][1] = (S_matrix[0][0] * S_matrix[2][2] - S_matrix[0][2]*S_matrix[2][0])*invert_det;
    S_inv_mat[1][2] = (S_matrix[1][0] * S_matrix[0][2] - S_matrix[0][0]*S_matrix[1][2])*invert_det;
    S_inv_mat[2][0] = (S_matrix[1][0] * S_matrix[2][1] - S_matrix[2][0]*S_matrix[1][1])*invert_det;
    S_inv_mat[2][1] = (S_matrix[2][0] * S_matrix[0][1] - S_matrix[0][0]*S_matrix[2][1])*invert_det;
    S_inv_mat[2][2] = (S_matrix[0][0] * S_matrix[1][1] - S_matrix[1][0]*S_matrix[0][1])*invert_det;
    
    //vector of Intensities
    vector<int> I;
    
    //stores the x, y coordinates of the pixel visible in all three images
    //stores the computed normals for that coordinate in a vector
    struct Normals {
        int x;
        int y;
        vector<double> normals;
    };
    
    //Normal vector
    vector<Normals> N;
    
    for (int i = 0; i < image1->num_rows(); i = i+step) {
        for (int j = 0; j < image1->num_columns(); j = j+step) {
            
            int I1 = image1->GetPixel(i, j);
            int I2 = image2->GetPixel(i, j);
            int I3 = image3->GetPixel(i, j);
            if (I1 > threshold && I2 > threshold && I3 > threshold) {
                I.clear();
                I.push_back(I1);
                I.push_back(I2);
                I.push_back(I3);
                
                //rho x n
                double pn1 = S_inv_mat[0][0]*I[0] + S_inv_mat[0][1]*I[1] + S_inv_mat[0][2]*I[2];
                double pn2 = S_inv_mat[1][0]*I[0] + S_inv_mat[1][1]*I[1] + S_inv_mat[1][2]*I[2];
                double pn3 = S_inv_mat[2][0]*I[0] + S_inv_mat[2][1]*I[1] + S_inv_mat[2][2]*I[2];
                //calculate rho, which is the magnitude/length of the vector
                double rho = sqrt(pn1*pn1 + pn2*pn2 + pn3*pn3);
                
                Normals n;
                n.x = i;
                n.y = j;
                n.normals.push_back(pn1/rho);
                n.normals.push_back(pn2/rho);
                n.normals.push_back(pn3/rho);
                
                N.push_back(n);
            }
        }
    }

    //draw needles
    for (int t = 0; t < N.size(); t++) {
        int x1 = N[t].x;
        int y1 = N[t].y;
        
        //scale the vector
        int x2 = x1 + 10*N[t].normals[0];
        int y2 = y1 + 10*N[t].normals[1];
        
        DrawLine(x1, y1, x2, y2, 255, image1);
        //draw a white circle around black gridpoint
        image1->SetPixel(x1, y1-1, 255);
        image1->SetPixel(x1-1, y1, 255);
        image1->SetPixel(x1, y1+1, 255);
        image1->SetPixel(x1+1, y1, 255);

        //draw the gridpoint in black
        image1->SetPixel(x1, y1, 0);
    }

    if (!WriteImage(out_file, *image1)){
        cout << "Can't write to file " << out_file << endl;
    }
    
    return true;
}// end ComputeNormal

/**
 * Function that processes the input images and outputs to the output file.
 * Calls the function ComputeNormal to do the calculations.
 * in_file is directions file from s2.
 * image1, image2, image3 are the immages with different lightsources
 * step is the value to increment by
 * threshold is the value to be used as a threshold
 * out_file is the output needle map image.
 * Returns true if the images were read and ComputeNormal called.
 **/
bool ProcessFiles(string in_file, string image1, string image2, string image3, int step, int threshold, string out_file) {
    
    Image orig_image1;
    Image orig_image2;
    Image orig_image3;
    
    //vector of directions
    vector<double> values;
    string line; //for tokenizing
    
    //read from directions file
    ifstream inFile;
    inFile.open(in_file);
    if (inFile.is_open()) {
        while (inFile >> line) {
            stringstream stream(line);
            while (stream) {
                string token;
                stream >> token;
                if (token != "") {
                    values.push_back(stod(token.c_str()));
                }
            }
            
        }
    }
    else {
        return false;
    }
    inFile.close();
    
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

    ComputeNormal(values, &orig_image1, &orig_image2, &orig_image3, step, threshold, out_file);
    
    return true;
}// end ProcessFiles

int main(int argc, char **argv){
    
    if (argc!=8) {
        cout << "Usage: <input directions file> <image 1> <image 2> <image 3> <step> <threshold> <output needle image> " << endl;
        return 0;
    }
    const string input_file(argv[1]);
    const string image1(argv[2]);
    const string image2(argv[3]);
    const string image3(argv[4]);
    const string step(argv[5]);
    int N = atoi(step.c_str());
    const string threshold(argv[6]);
    int T = atoi(threshold.c_str());
    const string output_file(argv[7]);
    
    ProcessFiles(input_file, image1, image2, image3, N, T, output_file);
    
    return 0;
}
