#include "Data.h"
#include <fstream>
#include <iostream>

using namespace std;

Data::Data()
:loaded(false)
{

}

void Data::load(const char* filename)
{
	// Load the data
	fstream fin(filename, ios::in);
	if(!fin)
		cerr<<"Error: Can't load "<<filename<<"."<<endl;
	fin>>image;
	fin.close();
	loaded = true;


	vector<double> x(image.get_nJ());
	vector<double> y(image.get_nI());
	for(int j=0; j<image.get_nJ(); j++)
		x[j] = image.get_xMin() + (j + 0.5)*image.get_dx();
	for(int i=0; i<image.get_nI(); i++)
		y[i] = image.get_yMax() - (i + 0.5)*image.get_dy();

	// Assign the xc, yc arrays
	xc = image.get_pixels();
	yc = image.get_pixels();
	for(int i=0; i<image.get_nI(); i++)
	{
		for(int j=0; j<image.get_nJ(); j++)
		{
			xc(i, j) = x[j];
			yc(i, j) = y[i];
		}
	}

}

