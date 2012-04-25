#include "Hyperparameters.h"

using namespace std;

double Hyperparameters::logp(const vector<Star>& stars) const
{
	double result = 0.0;
	for(size_t i=0; i<stars.size(); i++)
		result += _logp(stars[i]);
	return result;
}

