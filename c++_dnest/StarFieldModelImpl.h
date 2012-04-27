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


#include "StarFieldModel.h"
#include "Utils.h"
#include "RandomNumberGenerator.h"
#include <cmath>

using namespace std;
using namespace DNest;

template<class HyperType>
const double StarFieldModel<HyperType>::noiseSigma = 0.02;

template<class HyperType>
const double StarFieldModel<HyperType>::noiseCoeff = 0;

template<class HyperType>
const int StarFieldModel<HyperType>::maxNumStars = 200;

template<class HyperType>
PSF StarFieldModel<HyperType>::psf;

template<class HyperType>
StarFieldModel<HyperType>::StarFieldModel()
:mockImage(Data::get_data().get_ni(), Data::get_data().get_nj())
{
	if(!Data::get_data().isLoaded())
		cerr<<"WARNING: Data not loaded."<<endl;
	psf.set(1.0*Data::get_data().get_dx(), 5.0*Data::get_data().get_dx(), 0.5);
}

template<class HyperType>
StarFieldModel<HyperType>::~StarFieldModel()
{

}

template<class HyperType>
Model* StarFieldModel<HyperType>::factory() const
{
	return new StarFieldModel<HyperType>;
}

template<class HyperType>
Model* StarFieldModel<HyperType>::clone() const
{
	return new StarFieldModel<HyperType>(*this);
}

template<class HyperType>
void StarFieldModel<HyperType>::copyFrom(const Model* other)
{
	*this = *((StarFieldModel<HyperType>*)other);
}

template<class HyperType>
void StarFieldModel<HyperType>::fromPrior()
{
	hyperparameters.fromPrior();
	int numStars = randInt(maxNumStars + 1);
	for(int i=0; i<numStars; i++)
		stars.push_back(hyperparameters.generateStar());
	calculateMockImage();

	Model::fromPrior();
	calculateLogLikelihood();
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb()
{
	double logH = 0.0;
	int which = randInt(3);

	if(which == 0)
		logH = perturbHelper1();
	else if(which == 1)
		logH = perturbHelper2();
	else if(which == 2)
		logH = perturbHelper3();

	if(staleness >= 1000)
		calculateMockImage();

	Model::perturb();
	calculateLogLikelihood();
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper1()
{
	if(stars.size() == 0)
		return 0;

	// Chance of moving each star
	double chance = pow(10., 0.5 - 4*randomU());
	double scale = pow(10., 1.5 - 6.*randomU());

	double logH = 0;

	int which = randInt(2);

	if(which == 0)
	{
		// Move positions
		for(size_t i=0; i<stars.size(); i++)
		{
			if(randomU() <= chance)
			{
				stars[i].decrementImage(mockImage, psf);
				logH += hyperparameters.perturbPosition(stars[i], scale);
				stars[i].incrementImage(mockImage, psf);
			}
		}
	}
	else
	{
		// Move fluxes
		for(size_t i=0; i<stars.size(); i++)
		{
			if(randomU() <= chance)
			{
				stars[i].decrementImage(mockImage, psf);
				logH += hyperparameters.perturbFlux(stars[i], scale);
				stars[i].incrementImage(mockImage, psf);
			}
		}
	}

	staleness++;
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper2()
{
	// Move hyperparameters

	double logH = 0;
	if(randomU() <= 0.5)
	{
		logH += hyperparameters.perturb1(stars);
	}
	else
	{
		logH += hyperparameters.perturb2(stars);
		calculateMockImage();
	}
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper3()
{
	// Add or remove stars
	int delta = (int)round(maxNumStars*pow(10.0, 1.5 - 6.0*randomU())*randn()); // Change in number of stars
	int newNumStars = (int)stars.size() + delta;
	newNumStars = mod(newNumStars, maxNumStars);
	delta = newNumStars - (int)stars.size();

	if(delta == 0)
		delta = (randomU() < 0.5)?(1):(-1);

	if(randomU() <= 0.5)
	{
		// Add
		if((int)stars.size() + delta > maxNumStars)
			return 0;

		for(int i=0; i<delta; i++)
		{
			stars.push_back(hyperparameters.generateStar());
			stars.back().incrementImage(mockImage, psf);
		}
	}
	else
	{
		// Remove
		if((int)stars.size() + delta < 0)
			return 0;

		for(int i=0; i<-delta; i++)
		{
			int which = randInt(stars.size());
			stars[which].decrementImage(mockImage, psf);
			stars.erase(stars.begin() + which);
		}
	}
	staleness++;

	return 0;
}

template<class HyperType>
void StarFieldModel<HyperType>::calculateMockImage()
{
	mockImage.setZero();
	for(size_t i=0; i<stars.size(); i++)
		stars[i].incrementImage(mockImage, psf);
	staleness = 0;
}

template<class HyperType>
void StarFieldModel<HyperType>::calculateLogLikelihood()
{
	logl.logl = 0.0;

	double var;
	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
		{
			var = pow(noiseSigma, 2) + pow(noiseCoeff*mockImage(i, j), 2);
			logl.logl += -0.5*(2*M_PI*var) - 0.5*pow(Data::get_data()(i, j) - mockImage(i, j), 2)/var;
		}
}

template<class HyperType>
void StarFieldModel<HyperType>::print(ostream& out) const
{
	out<<stars.size()<<' '<<staleness<<' '<<hyperparameters<<' ';
	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
			out<<mockImage(i, j)<<' ';
}

