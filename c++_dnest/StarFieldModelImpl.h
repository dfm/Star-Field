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
const double StarFieldModel<HyperType>::noiseSigma = 30.;

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

	if(DNest3::randomU() < 0.9)
		logH = perturbHelper1();
	else
		logH = perturbHelper2();

	if(staleness >= 1000)
		calculateMockImage();

	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper1()
{
	double chance = pow(10., 0.5 - 4.*DNest3::randomU());
	double scale = pow(10., 1.5 - 6.*DNest3::randomU());
	int which = DNest3::randInt(2);
	if(which == 0)
	{
		for(int i=0; i<maxNumStars; i++)
		{
			if(DNest3::randomU() <= chance)
			{
				stars[i].decrementImage(mockImage, psf);
				u_x[i] = DNest3::mod(u_x[i] + scale*DNest3::randn(), 1.);
				u_y[i] = DNest3::mod(u_y[i] + scale*DNest3::randn(), 1.);
				stars[i] = hyperparameters.
					generateStar(u_x[i], u_y[i], u_f[i]);
				stars[i].incrementImage(mockImage, psf);
			}
		}
	}
	else if(which == 1)
	{
		for(int i=0; i<maxNumStars; i++)
		{
			if(DNest3::randomU() <= chance)
			{
				stars[i].decrementImage(mockImage, psf);
				u_f[i] = DNest3::mod(u_f[i] + scale*DNest3::randn(), 1.);
				stars[i] = hyperparameters.
					generateStar(u_x[i], u_y[i], u_f[i]);
				stars[i].incrementImage(mockImage, psf);
			}		
		}
	}
	staleness++;
	return 0.;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper2()
{
	double logH = hyperparameters.perturb();
	generateStars();
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
	out<<staleness<<' ';
	hyperparameters.print(out); out<<' ';
	for(int i=0; i<maxNumStars; i++)
		out<<stars[i].x<<' ';
	for(int i=0; i<maxNumStars; i++)
		out<<stars[i].y<<' ';
	for(int i=0; i<maxNumStars; i++)
		out<<stars[i].flux<<' ';
	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
			out<<mockImage(i, j)<<' ';
}

