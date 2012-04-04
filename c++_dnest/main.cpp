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
#include "StarFieldModel.h"
#include "MultiSampler.h"
#include "RandomNumberGenerator.h"
#include "DNestSampler.h"
#include <boost/thread.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace DNest;
namespace po = boost::program_options;

void runMultiThread(const Model* exampleModel, const string& levelsFile);				// Run multithreaded sampler
void runLegacy(const Model* exampleModel, const string& levelsFile);	// Run legacy sampler

int main(int argc, char** argv)
{
	// What kind of model will we be running?
	StarFieldModel exampleModel;

	// Which kind of sampler, and should we pre-load level structure?
	bool legacy = false;
	string levelsFile = "";

	// Handle command line options
	{
		// Declare the supported options.
		po::options_description desc("Welcome to DNest. Allowed options");
		desc.add_options()
		    ("help", "produce help message")
		    ("levels", po::value<string>(), "Input levels file")
		    ("legacy", "use legacy sampler (serial)")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help"))
		{
		    cout<<desc<<endl;
		    return 1;
		}

		if(vm.count("legacy")) 
			legacy = true;

		if(vm.count("levels"))
			levelsFile = vm["levels"].as<string>();
	}

	// Run the sampler!
	if(legacy)
		runLegacy(&exampleModel, levelsFile);
	else
		runMultiThread(&exampleModel, levelsFile);		

	return 0;
}

void runLegacy(const Model* exampleModel, const string& levelsFile)
{
	RandomNumberGenerator::initialise(-time(0));

	if(levelsFile == "")
	{
		DNestSampler sampler(exampleModel);
		sampler.run();
	}
	else
	{
		DNestSampler sampler(exampleModel, levelsFile.c_str());
		sampler.run();
	}

	RandomNumberGenerator::free();

}

void runMultiThread(const Model* exampleModel, const string& levelsFile)
{
	// Multithreaded
	int threads = boost::thread::hardware_concurrency();
	cout<<"# Running on "<<threads<<" threads."<<endl;

	DNestOptions options("OPTIONS");

	if(levelsFile == "")
	{
		MultiSampler sampler(threads, exampleModel, options);
		sampler.runThreaded();
	}
	else
	{
		MultiSampler sampler(threads, exampleModel, options, levelsFile.c_str());
		sampler.runThreaded();
	}
}

