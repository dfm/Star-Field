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
	hyperparameters.print(out); out<<' ';
	out<<staleness<<' ';

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

