#include "Lookup.h"
#include <cmath>
#include <cassert>

Lookup Lookup::instance;

Lookup::Lookup()
:num(10000)
,xMin(0.)
,xMax(100.)
,dx((xMax - xMin)/(num-1))
,_exp(num)
{
	for(int i=0; i<num; i++)
		_exp[i] = exp(-i*dx);
}

double Lookup::evaluate(double x)
{
	int i = static_cast<int>(x/instance.dx);
	if(i < 0 || i >= static_cast<int>(instance._exp.size()))
		return 0.;
	return instance._exp[i];
}

