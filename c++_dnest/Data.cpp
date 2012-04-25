#include "Data.h"
#include <fstream>
#include <iostream>

using namespace std;

Data::Data()
{

}

void Data::load(const char* filename)
{
	fstream fin(filename, ios::in);
	if(!fin)
		cerr<<"Error: Can't load "<<filename<<"."<<endl;
	fin>>image;
	fin.close();
}

double& Data::operator () (int i, int j)
{
	return image(i, j);
}

