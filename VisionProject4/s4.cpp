//
//  s4.cpp
//  VisionProject4
//
//  Created by Elizabeth Kelly on 11/10/16.
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
 * Function that computes the albedos.
 * S is the vector of directions.
 * image1, image2, image3 are the images to process.
 * threshold is the value to be used as a threshold
 * out_file is the output albedo map image.
 * Returns true if the images were read, processed, and written.
 **/
bool ComputeAlbedo(vector<double> S, Image *image1, Image *image2, Image *image3, int threshold, string out_file) {
    
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
    // compute rho * n to get N (rho is the length (|N|) of N, n is N/|N|
    // when we get n, we get rho which is albedo
    
    double det_of_n = (S_matrix[0][0] * (S_matrix[1][1]*S_matrix[2][2] - S_matrix[1][2]*S_matrix[2][1])) -
                      (S_matrix[0][1] * (S_matrix[1][0]*S_matrix[2][2] - S_matrix[1][2]*S_matrix[2][0])) +
                      (S_matrix[0][2] * (S_matrix[1][0]*S_matrix[2][1] - S_matrix[1][1]*S_matrix[2][0]));
    
    double invert_det = 1/det_of_n;
    //invert matrix S
    double S_inv_mat[3][3];
    
    S_inv_mat[0][0] = (S_matrix[1][1] * S_matrix[2][2] - S_matrix[1][2]*S_matrix[2][1])*invert_det;
    S_inv_mat[0][1] = (S_matrix[0][2] * S_matrix[2][1] - S_matrix[0][1]*S_matrix[2][2])*invert_det;
    S_inv_mat[0][2] = (S_matrix[0][1] * S_matrix[1][2] - S_matrix[0][2]*S_matrix[1][1])*invert_det;
    S_inv_mat[1][0] = (S_matrix[1][2] * S_matrix[2][0] - S_matrix[1][0]*S_matrix[2][2])*invert_det;
    S_inv_mat[1][1] = (S_matrix[0][0] * S_matrix[2][2] - S_matrix[0][2]*S_matrix[2][0])*invert_det;
    S_inv_mat[1][2] = (S_matrix[0][2] * S_matrix[1][0] - S_matrix[0][0]*S_matrix[1][2])*invert_det;
    S_inv_mat[2][0] = (S_matrix[1][0] * S_matrix[2][1] - S_matrix[2][0]*S_matrix[1][1])*invert_det;
    S_inv_mat[2][1] = (S_matrix[2][0] * S_matrix[0][1] - S_matrix[0][0]*S_matrix[2][1])*invert_det;
    S_inv_mat[2][2] = (S_matrix[0][0] * S_matrix[1][1] - S_matrix[1][0]*S_matrix[0][1])*invert_det;

    //vector of Intensities
    vector<int> I;
    
    //stores the x, y coordinates of the pixel visible in all three images
    //stores the computed albedo for that coordinate
    struct Normals {
        int x;
        int y;
        double albedo;
    };
    
    //Normal vector
    vector<Normals> N;
    
    for (int i = 0; i < image1->num_rows(); i++) {
        for (int j = 0; j < image1->num_columns(); j++) {
            
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
                n.albedo = rho;
                
                N.push_back(n);
            }
        }
    }
    
    Image albedo_img;
    albedo_img.AllocateSpaceAndSetSize(image1->num_rows(), image1->num_columns());
    albedo_img.SetNumberGrayLevels(255);
    
    for (int i = 0; i < albedo_img.num_rows(); i++) {
        for (int j = 0; j < albedo_img.num_columns(); j++) {
            albedo_img.SetPixel(i, j, 0);
        }
    }
    
    //find the maximum value of albedo to scale the pixel
    double max_albedo = 0;

    for (int t = 0; t < N.size(); t++) {
        if (N[t].albedo > max_albedo) {
            max_albedo = N[t].albedo;
        }
    }

    for (int r = 0; r < N.size(); r++) {
        int i = N[r].x;
        int j = N[r].y;
        int pixel_val = (N[r].albedo * 255)/max_albedo;
        
        albedo_img.SetPixel(i, j, pixel_val);
    }

    if (!WriteImage(out_file, albedo_img)){
        cout << "Can't write to file " << out_file << endl;
    }
    
    return true;
}// end ComputeAlbedo

/**
 * Function that processes the input images and outputs to the output file.
 * Calls the function ComputeNormal to do the calculations.
 * in_file is directions file from s2.
 * image1, image2, image3 are the immages with different lightsources
 * threshold is the value to be used as a threshold
 * out_file is the output albedo map image.
 * Returns true if the images were read and ComputeAlbedo called.
 **/
bool ProcessFiles(string in_file, string image1, string image2, string image3, int threshold, string out_file) {
    
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
    
    ComputeAlbedo(values, &orig_image1, &orig_image2, &orig_image3, threshold, out_file);
    
    return true;
}// end ProcessFiles

int main(int argc, char **argv){
    
    if (argc!=7) {
        cout << "Usage: <input directions file> <image 1> <image 2> <image 3> <threshold> <output albedo image>" << endl;
        return 0;
    }
    const string input_file(argv[1]);
    const string image1(argv[2]);
    const string image2(argv[3]);
    const string image3(argv[4]);
    const string threshold(argv[5]);
    int T = atoi(threshold.c_str());
    const string output_file(argv[6]);
    
    ProcessFiles(input_file, image1, image2, image3, T, output_file);
    
    return 0;
}
