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
	fstream fin(filename, ios::in);
	if(!fin)
		cerr<<"Error: Can't load "<<filename<<"."<<endl;
	fin>>image;
	fin.close();
	loaded = true;
}

