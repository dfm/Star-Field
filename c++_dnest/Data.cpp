#include "Data.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

const int Data::ni = 100;
const int Data::nj = 100;
const double Data::xMin = -1.0;
const double Data::xMax =  1.0;
const double Data::yMin = -1.0;
const double Data::yMax =  1.0;
const double Data::xRange = (Data::xMax - Data::xMin);
const double Data::yRange = (Data::yMax - Data::yMin);
const double Data::range = sqrt(Data::xRange*Data::yRange);
const double Data::dx = Data::xRange/(Data::nj - 1);
const double Data::dy = Data::yRange/(Data::ni - 1);
const vector<Array> Data::xy = Array::meshgrid(Data::ni, Data::nj, Data::xMin, Data::xMax, Data::yMin, Data::yMax);
const Array& Data::x = Data::xy[0];
const Array& Data::y = Data::xy[1];

Data::Data()
:pixels(ni, nj)
,loaded(false)
{

}

void Data::load(const char* filename)
{
	fstream fin(filename, ios::in);
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			fin>>pixels[i][j];
	fin.close();
	loaded = true;
}

bool Data::isLoaded() const
{
	return loaded;
}


vector<double>& Data::operator [] (int i)
{
	return pixels[i];
}

const vector<double>& Data::operator [] (int i) const
{
	return pixels[i];
}

