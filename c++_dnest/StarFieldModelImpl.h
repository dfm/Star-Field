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
const PSF StarFieldModel<HyperType>::psf;

template<class HyperType>
StarFieldModel<HyperType>::StarFieldModel()
:mockImage(Data::get_data().get_ni(), Data::get_data().get_nj())
{
	if(!Data::get_data().isLoaded())
	{
		Data::load_data("data_100stars.txt");
		psf.set(1.0*Data::get_data().get_dx(), 5.0*Data::get_data().get_dx(), 0.5);
	}
}

template<class HyperType>
StarFieldModel<HyperType>::~StarFieldModel()
{

}

template<class HyperType>
Model* StarFieldModel<HyperType>::factory() const
{
	return new StarFieldModel;
}

template<class HyperType>
Model* StarFieldModel<HyperType>::clone() const
{
	return new StarFieldModel(*this);
}

template<class HyperType>
void StarFieldModel<HyperType>::copyFrom(const Model* other)
{
	*this = *((StarFieldModel*)other);
}

template<class HyperType>
void StarFieldModel<HyperType>::fromPrior()
{
	hyp.fromPrior();
	int numStars = randInt(maxNumStars + 1);
	for(int i=0; i<numStars; i++)
		stars.push_back(hyp.generateStar());
	calculateMockImage();

	Model::fromPrior();
	calculateLogLikelihood();
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb()
{
	if(staleness >= 1000)
		calculateMockImage();

	double logH = 0.0;
	int which = randInt(4);

	if(which == 0)
		logH = perturbHelper1();
	else if(which == 1)
		logH = perturbHelper2();
	else if(which == 2)
		logH = perturbHelper3();
	else
		logH = perturbHelper4();

	Model::perturb();
	calculateLogLikelihood();
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper1()
{
	if(stars.size() == 0)
		return 0;

	// Number of stars to move
	int delta = (int)floor(maxNumStars*pow(10.0, 1.5 - 6.0*randomU())); // Change in number of stars
	if(delta == 0 || delta > maxNumStars)
		delta = 1;

	// Move a star in position
	double scale = pow(10.0, 1.5 - 6.0*randomU());

	double logH = 0;
	for(int i=0; i<delta; i++)
	{
		int which = randInt(stars.size());

		logH -= -hyp.logp(stars[which]);
		stars[which].decrementImage(mockImage, psf);
		stars[which].x += hyp.sig*scale*randn();
		stars[which].y += hyp.sig*scale*randn();
		stars[which].incrementImage(mockImage, psf);
		logH += hyp.logp(stars[which]);
	}

	staleness++;
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper2()
{
	if(stars.size() == 0)
		return 0;

	// Number of stars to move
	int delta = (int)floor(maxNumStars*pow(10.0, 1.5 - 6.0*randomU())); // Change in number of stars
	if(delta == 0 || delta > maxNumStars)
		delta = 1;

	// Move a star in flux
	double scale = pow(10.0, 1.5 - 6.0*randomU());

	double logH = 0;
	for(int i=0; i<delta; i++)
	{
		int which = randInt(stars.size());

		logH -= -hyp.logp(stars[which]);
		stars[which].decrementImage(mockImage, psf);
		stars[which].flux += hyp.meanFlux*scale*randn();
		stars[which].incrementImage(mockImage, psf);
		logH += hyp.logp(stars[which]);
	}

	staleness++;
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper3()
{
	// Move hyperparameters

	double logH = 0;
	if(randomU() <= 0.5)
	{
		logH -= hyp.logp(stars);
		logH += hyp.perturb();
		logH += hyp.logp(stars);
	}
	else
	{
		logH += hyp.perturb(stars);
		calculateMockImage();
	}
	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper4()
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
			stars.push_back(hyp.generateStar());
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
	for(int i=0; i<Data::ni; i++)
		for(int j=0; j<Data::nj; j++)
		{
			var = pow(noiseSigma, 2) + pow(noiseCoeff*mockImage[i][j], 2);
			logl.logl += -0.5*(2*M_PI*var) - 0.5*pow(data[i][j] - mockImage[i][j], 2)/var;
		}
}

template<class HyperType>
void StarFieldModel<HyperType>::print(ostream& out) const
{
	out<<stars.size()<<' '<<hyp<<' '<<mockImage;
}

