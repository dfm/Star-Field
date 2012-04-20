#include "Data.h"
#include <fstream>

using namespace std;

const int Data::numPixels = 100;

Data::Data()
:pixels(numPixels, numPixels)
{

}

void Data::load(const char* filename)
{
	fstream fin(filename, ios::in);
	for(int i=0; i<numPixels; i++)
		for(int j=0; j<numPixels; j++)
			fin>>pixels(i, j);
	fin.close();
}

