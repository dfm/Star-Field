#include "Array.h"

using namespace std;

Array::Array()
:ni(0), nj(0)
,size(0)
{

}

Array::Array(int ni, int nj)
:ni(ni), nj(nj)
,size(ni*nj)
,pixels(ni, vector<double>(nj))
{

}

void Array::resize(int ni, int nj)
{
	this->ni = ni; this->nj = nj;
	size = ni*nj;
	pixels.resize(ni, vector<double>(nj));
}

void Array::set(double value)
{
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			pixels[i][j] = value;
}

void Array::increment(double value)
{
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			pixels[i][j] += value;
}

void Array::decrement(double value)
{
	for(int i=0; i<ni; i++)
		for(int j=0; j<nj; j++)
			pixels[i][j] -= value;
}

