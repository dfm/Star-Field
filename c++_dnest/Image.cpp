#include "Image.h"
#include <cassert>

using namespace std;

#include <iostream>

Image::Image()
{

}

Image::Image(int nI, int nJ, double xMin, double xMax, double yMin, double yMax)
:nI(nI), nJ(nJ)
,xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax)
,pixels(nI, nJ)
{
	assert(xMax > xMin && yMax > yMin && nI >= 1 && nJ >= 1);
	compute();
}

Image::Image(int nI, int nJ, double xMin, double xMax, double yMin, double yMax, double value)
:nI(nI), nJ(nJ)
,xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax)
,pixels(nI, nJ)
{
	assert(xMax > xMin && yMax > yMin && nI >= 1 && nJ >= 1);
	compute();
	setConstant(value);
}

void Image::compute()
{
	xRange = xMax - xMin;
	yRange = yMax - yMin;
	dx = xRange/(nJ - 1);
	dy = yRange/(nI - 1);
	dA = dx*dy;
}

void Image::setConstant(double value)
{
	pixels.setConstant(value);
}

Image& Image::operator += (const Image& other)
{
	assert(nI == other.nI && nJ == other.nJ);
	pixels += other.pixels;
	return *this;
}

Image& Image::operator -= (const Image& other)
{
	assert(nI == other.nI && nJ == other.nJ);
	pixels -= other.pixels;
	return *this;
}

double& Image::operator () (int i, int j)
{
	return pixels(i, j);
}

const double& Image::operator () (int i, int j) const
{
	return pixels(i, j);
}

void Image::increment(const Star& star, const PSF& psf, double coefficient)
{
	
}

ostream& operator << (ostream& out, const Image& image)
{
	out<<"# ";
	out<<image.nI<<' '<<image.nJ<<' ';
	out<<image.xMin<<' '<<image.xMax<<' ';
	out<<image.yMin<<' '<<image.yMax<<' ';
	out<<endl;
	for(int i=0; i<image.nI; i++)
	{
		for(int j=0; j<image.nJ; j++)
			out<<image.pixels(i, j)<<' ';
		out<<endl;
	}
	return out;
}

istream& operator >> (istream& in, Image& image)
{
	// Skip the hash character
	char hash;
	in>>hash;

	// Read in things to temporary variables.
	// Check validity before actually doing assignment.
	int nI, nJ;
	double xMin, xMax, yMin, yMax;
	in>>nI>>nJ;
	in>>xMin>>xMax;
	in>>yMin>>yMax;

	// Check validity
	assert(xMax > xMin && yMax > yMin && nI >= 1 && nJ >= 1);

	image.nI = nI; image.nJ = nJ;
	image.xMin = xMin; image.xMax = xMax;
	image.yMin = yMin; image.yMax = yMax;

	image.compute();

	// According to the eigen docs, if no change,
	// nothing actually happens (good efficiency)	
	image.pixels.resize(image.nI, image.nJ);

	for(int i=0; i<image.nI; i++)
		for(int j=0; j<image.nJ; j++)
			in>>image(i, j);

	return in;
}

bool operator == (const Image& image1, const Image& image2)
{
	// Return false at first sign of inequality
	if(image1.nI != image2.nI || image1.nJ != image2.nJ)
		return false;

	if(image1.xMin != image2.xMin || image1.xMax != image2.xMax)
		return false;

	// Eigen has not defined array equality checking, so do it here
	for(int i=0; i<image1.nI; i++)
		for(int j=0; j<image1.nJ; j++)
			if(image1.pixels(i, j) != image2.pixels(i, j))
				return false;
	return true;
}

