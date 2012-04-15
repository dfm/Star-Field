#include "Hyperparameters.h"

using std::vector;

double Hyperparameters::logp(const vector<Star>& stars) const
{
	double result = 0.0;
	for(size_t i=0; i<stars.size(); i++)
		result += logp(stars[i]);
	return result;
}

