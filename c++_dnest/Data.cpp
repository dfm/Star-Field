#include "Data.h"
#include <fstream>

using namespace std;

Data::Data()
{

}

void Data::load(const char* filename)
{
	fstream fin(filename, ios::in);
	fin>>image;
	fin.close();
}

double& Data::operator () (int i, int j)
{
	return image(i, j);
}

