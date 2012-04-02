/*
    Copyright (C) 2011 Brendon J. Brewer
    This file is part of DNest, the Diffusive Nested Sampler.

    DNest is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DNest is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DNest.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <boost/thread.hpp>
#include "StarFieldModel.h"
#include "DNestSampler.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace DNest;

int main(int argc, char** argv)
{

	StarFieldModel t;

	// If a command line argument is found,
	// load the level structure from the file specified
	if(argc >= 2)
	{
		DNestSampler sampler(&t, argv[1]);
		sampler.run();
	}
	else
	{
		DNestSampler sampler(&t);
		sampler.run();
	}

	return 0;
}

