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

#include "Utils.h"
#include "RandomNumberGenerator.h"
#include <cmath>

template<class HyperType>
const double StarFieldModel<HyperType>::noiseSigma = 5.;

template<class HyperType>
const double StarFieldModel<HyperType>::noiseCoeff = 0;

template<class HyperType>
const int StarFieldModel<HyperType>::maxNumStars = 200;

template<class HyperType>
PSF StarFieldModel<HyperType>::psf;

template<class HyperType>
StarFieldModel<HyperType>::StarFieldModel()
:mockImage(Data::get_data().get_ni(), Data::get_data().get_nj())
,staleness(0)
{
	if(!Data::get_data().isLoaded())
		std::cerr<<"WARNING: Data not loaded."<<std::endl;
	psf.set(1.0*Data::get_data().get_dx(), 5.0*Data::get_data().get_dx(), 0.5);
}

template<class HyperType>
void StarFieldModel<HyperType>::fromPrior()
{
	hyperparameters.fromPrior();
	numStars = DNest3::randInt(maxNumStars + 1);

	stars.clear();
	for(int i=0; i<numStars; i++)
		stars.push_back(hyperparameters.generateStar());
	calculateMockImage();
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb()
{
	double logH = 0.;
	int which = DNest3::randInt(3);
	if(which == 0)
		logH = perturb1();
	else if(which == 1)
		logH = perturb2();
	else if(which == 2)
		logH = perturb3();

	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb1()
{
	double logH = 0.;

	// Make a proposal for the new number of stars
	int diff = static_cast<int>
			(round(maxNumStars*pow(10., 1.5 - 6.*DNest3::randomU())*DNest3::randn()));
	if(diff == 0)
		diff = (DNest3::randomU() <= 0.5)?(-1):(1);
	int proposal = numStars + diff;
	proposal = DNest3::mod(proposal, maxNumStars + 1);

	int actual_diff = proposal - numStars;

	if(actual_diff > 0)
	{
		for(int i=0; i<actual_diff; i++)
		{
			Star star = hyperparameters.generateStar();
			star.incrementImage(mockImage, psf);
			stars.push_back(star);
			numStars++;
		}
	}
	else if(actual_diff < 0)
	{
		int which;
		for(int i=0; i<-actual_diff; i++)
		{
			which = DNest3::randInt(numStars);
			stars[which].decrementImage(mockImage, psf);
			stars.erase(stars.begin() + which);
			numStars--;
		}
	}

	staleness++;
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb2()
{
	double logH = 0.;
	int which = DNest3::randInt(2);
	if(which == 0)
	{
		logH = hyperparameters.perturb1(stars);
		calculateMockImage();
	}
	else
		logH = hyperparameters.perturb2(stars);
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb3()
{
	double logH = 0.;

	double chance = pow(10., 0.5 - 4.*DNest3::randomU());
	double scale = pow(10., 1.5 - 6.*DNest3::randomU());

	int which = DNest3::randInt(2);

	if(which == 0)
	{
		// Positions
		for(int i=0; i<numStars; i++)
		{
			if(DNest3::randomU() <= chance)
			{
				if(chance < 1.)
					stars[i].decrementImage(mockImage, psf);

				logH += hyperparameters.perturbStar1(stars[i], scale);

				if(chance < 1.)
					stars[i].incrementImage(mockImage, psf);
			}
		}
	}
	else if(which == 1)
	{
		// Fluxes
		for(int i=0; i<numStars; i++)
		{
			if(DNest3::randomU() <= chance)
			{
				if(chance < 1.)
					stars[i].decrementImage(mockImage, psf);

				logH += hyperparameters.perturbStar2(stars[i], scale);

				if(chance < 1.)
					stars[i].incrementImage(mockImage, psf);
			}
		}

	}

	if(chance < 1.)
		staleness++;
	else
		calculateMockImage();

	return logH;
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
double StarFieldModel<HyperType>::logLikelihood() const
{
	double logL = 0.;

	double var;
	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
		{
			var = pow(noiseSigma, 2)
				+ pow(noiseCoeff*mockImage(i, j), 2);
			logL += -0.5*(2*M_PI*var) 
				- 0.5*pow(Data::get_data()(i, j)
				- mockImage(i, j), 2)/var;
		}

	return logL;
}

template<class HyperType>
void StarFieldModel<HyperType>::print(std::ostream& out) const
{
	out<<numStars<<' '<<staleness<<' ';
	hyperparameters.print(out); out<<' ';

	// Print x, pad with zeros
	for(int i=0; i<numStars; i++)
		out<<stars[i].x<<' ';
	for(int i=numStars; i<maxNumStars; i++)
		out<<0<<' ';

	// Print y, pad with zeros
	for(int i=0; i<numStars; i++)
		out<<stars[i].y<<' ';
	for(int i=numStars; i<maxNumStars; i++)
		out<<0<<' ';

	// Print flux, pad with zeros
	for(int i=0; i<numStars; i++)
		out<<stars[i].flux<<' ';
	for(int i=numStars; i<maxNumStars; i++)
		out<<0<<' ';

	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
			out<<mockImage(i, j)<<' ';
}

