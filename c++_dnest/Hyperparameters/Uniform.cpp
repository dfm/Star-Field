#include "Uniform.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest3;

Uniform::Uniform()
:minLogMu(log(1E-3))
,maxLogMu(log(1E3))
,rangeLogMu(maxLogMu - minLogMu)
{

}

void Uniform::fromPrior()
{
	mu = exp(minLogMu + rangeLogMu*randn());
}

double Uniform::perturb1(vector<Star>& stars)
{
	double logH = 0.;
	double proposal = log(mu);
	proposal += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
	proposal = mod(proposal - minLogMu, rangeLogMu) + minLogMu;
	proposal = exp(proposal);

	double ratio = proposal/mu;
	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux *= ratio;
	mu = proposal;

	return logH;
}

double Uniform::perturb2(const vector<Star>& stars)
{
	double logH = 0.;
	double proposal = log(mu);
	proposal += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
	proposal = mod(proposal - minLogMu, rangeLogMu) + minLogMu;
	proposal = exp(proposal);

	double logMu1 = log(mu);
	double logMu2 = log(proposal);
	for(size_t i=0; i<stars.size(); i++)
	{
		if(stars[i].flux < 0.)
			cerr<<"# Warning: Negative flux star."<<endl;
		logH -= -logMu1 - stars[i].flux/mu;
		logH += -logMu2 - stars[i].flux/proposal;
	}
	mu = proposal;
	return logH;
}


void Uniform::print(ostream& out) const
{
	out<<mu;
}



