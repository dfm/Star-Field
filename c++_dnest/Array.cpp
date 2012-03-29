#include "Array.h"
#include <cassert>

using namespace std;

Array::Array(int ni, int nj)
:ni(ni), nj(nj), values(ni, vector<double>(nj))
{

}

Array::Array(int ni, int nj, double value)
:ni(ni), nj(nj), values(ni, vector<double>(nj, value))
{

}

void Array::zero()
{
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			values[i][j] = 0.0;
}

double Array::sum() const
{
	double total = 0.0;
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			total += values[i][j];
	return total;
}

void Array::normalise()
{
	double total = sum();
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			values[i][j] /= total;
}

const vector<double>& Array::operator [] (int i) const
{
	return values[i];
}

vector<double>& Array::operator [] (int i)
{
	return values[i];
}

Array& Array::operator += (const Array& other)
{
	assert(other.ni == ni && other.nj == nj && this != &other);
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			values[i][j] += other.values[i][j];
	return *this;
}

vector<Array> Array::meshgrid(int ni, int nj, double xMin, double xMax, double yMin, double yMax)
{
	double dx = (xMax - xMin)/(nj - 1);
	double dy = (yMax - yMin)/(ni - 1);
	vector<Array> result(2, Array(ni, nj));

	vector<double> x(nj);
	for(int j=0; j<nj; j++)
		x[j] = xMin + (j + 0.5)*dx;
	vector<double> y(ni);
	for(int i=0; i<ni; i++)
		y[i] = yMax - (i + 0.5)*dy;
	
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
		{
			result[0].values[i][j] = x[j];
			result[1].values[i][j] = y[i];
		}

	return result;
}

int Array::getNi() const
{
	return ni;
}

int Array::getNj() const
{
	return nj;
}

ostream& operator << (ostream& out, const Array& array)
{
	for(int i=0; i<array.ni; i++)
		for(int j=0; j<array.nj; j++)
			out<<array[i][j]<<' ';
	return out;
}

/*
#include <iostream>
int main()
{
	vector<Array> a = Array::meshgrid(100, 100, -1.0, 1.0, -1.0, 1.0);
	cout<<a[1]<<endl;
}
*/
