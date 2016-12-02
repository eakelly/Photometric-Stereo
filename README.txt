*********************************
* Elizabeth Kelly		*	
* Computational Vision HW 4	*			
*********************************

*******************************************************************************
This program is a vision system that recovers an object’s orientation and reflectance using photometric stereo.	 

i. All parts of the assignment were completed.
ii. No bugs were encountered.
iii. & iv. Complete instructions of how to run this program and the 
needed input and output files are listed below.

Before running this program, please issue the following in the command 
line:

1) make clean
2) make all

*******************************************************************************
This program is separated into four parts:
*******************************************************************************
Program 1: Finds the location of the sphere using thresholding and then
computes the sphere’s center and radius.

To run this program, enter the following on the command line:

./s1 <input original image> <threshold value> <output parameters file> 

Example:
./s1 sphere0.pgm 95 parameters.txt

** The threshold values that works best for sphere0.pgm is 95.

The filename for the output parameters file is parameters.txt

*******************************************************************************
Program 2: Takes a parameter file with the sphere’s center coordinates and 
radiuses input, then uses this data to compute the directions and intensities 
of the light sources. The formula used to compute the normal vector is:

(z-zCenter) = sqrt(radius^2 - (x-xCenter)^2 - (y-yCenter)^2)

where x and y are the coordinates of the pixel with maximum brightness. We can
assume that the brightest surface spot on the sphere is the direction of the 
light source because the surface is Lambertian. This means that the cosine of 
the normal and light source direction determines the light intensity Since only
the largest value of cos(theta) corresponds to max brightness, this means that
theta is 0 degrees and that the surface normal is parallel to the light source
direction. 
———————————————————————————————————————————————————————————————————————————————

To run this program, enter the following on the command line:

./s2 <input param file> <image 1> <image 2> <image 3> <output directions file>

Example:
./s2 parameters.txt sphere1.pgm sphere2.pgm sphere3.pgm directions.txt

The filename for the input parameter file is parameters.txt
The filename for the output directions file is directions.txt

*******************************************************************************
Program 3: Computes the surface normals of the object using the data from the 
directions input file. In order to do this, the program thresholds the three
input images, for every n pixels, in order to find the pixel coordinates of the 
pixel that is visible in all three images (i.e. if the same pixel coordinates 
in each image pass the threshold). The output is then <image1> with a needle 
map of the normals laid on top.

To run this program, enter the following on the command line:

./s3 <input directions file> <image 1> <image 2> <image 3> <step> <threshold> <output needle image> 

Example:
./s3 directions.txt object1.pgm object2.pgm object3.pgm 10 85 needle_image.pgm

** The threshold values that work best:
sphereX.pgm is 95.
objectX.pgm is 85

** The best step value is 10.


The filename for the input directions file is directions.txt
The filename for the output needle map image is needle_image.pgm

*******************************************************************************
Program 4: Takes the same directions file as input and computes the surface 
albedo of the object. The albedo is computed for all pixels that are visible in 
all three images (the pixels that pass the threshold). The output file is an 
albedo map.

To run this program, enter the following on the command line:

./s4 <input directions file> <image 1> <image 2> <image 3> <threshold> <output albedo image>

Example:
./s4 directions.txt object1.pgm object2.pgm object3.pgm 85 albedo_image.pgm

** The threshold values that work best:
sphereX.pgm is 95.
objectX.pgm is 85

The filename for the input directions file is directions.txt
The filename for the output albedo map image is albedo_image.pgm

*******************************************************************************
File list:
*******************************************************************************
Makefile	Makefile with ‘make clean’ and ‘make all’ commands
s1.cpp		Locates the sphere and calculates center points and radius.
s2.cpp		Computes the directions and intensities of the light sources.
s3.cpp		Computes the surface normals.
s4.cpp		Computes the albedo for visible pixels.
Image.h		Header for a class representing a 2D gray-scale image.
Image.cpp 	Implementation of representing 2D gray-scale image.
*******************************************************************************
