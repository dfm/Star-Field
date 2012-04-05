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

const double StarFieldModel::noiseSigma = 1E-5;
const double StarFieldModel::noiseCoeff = 0;
const int StarFieldModel::maxNumStars = 500;
Data StarFieldModel::data;
PSF StarFieldModel::psf(1.0*Data::dx, 5.0*Data::dx, 0.5);

StarFieldModel::StarFieldModel()
:mockImage(Data::ni, Data::nj)
{
	if(!data.isLoaded())
		data.load("data_100stars.txt");
}

StarFieldModel::~StarFieldModel()
{

}

Model* StarFieldModel::factory() const
{
	return new StarFieldModel;
}

Model* StarFieldModel::clone() const
{
	return new StarFieldModel(*this);
}

void StarFieldModel::copyFrom(const Model* other)
{
	*this = *((StarFieldModel*)other);
}

void StarFieldModel::fromPrior()
{
	hyp.fromPrior();
	int numStars = randInt(maxNumStars + 1);
	for(int i=0; i<numStars; i++)
		stars.push_back(hyp.generateStar());
	calculateMockImage();

	Model::fromPrior();
	calculateLogLikelihood();
}

double StarFieldModel::perturb()
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

double StarFieldModel::perturbHelper1()
{
	if(stars.size() == 0)
		return 0;

	// Number of stars to move
	int delta = (int)round(maxNumStars*pow(10.0, 1.5 - 6.0*randomU())*randn()); // Change in number of stars
	int newNumStars = (int)stars.size() + delta;
	newNumStars = mod(newNumStars, maxNumStars);
	delta = newNumStars - (int)stars.size();

	if(delta == 0)
		delta = (randomU() < 0.5)?(1):(-1);
	delta = abs(delta);

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

double StarFieldModel::perturbHelper2()
{
	if(stars.size() == 0)
		return 0;

	// Number of stars to move
	int delta = (int)round(maxNumStars*pow(10.0, 1.5 - 6.0*randomU())*randn()); // Change in number of stars
	int newNumStars = (int)stars.size() + delta;
	newNumStars = mod(newNumStars, maxNumStars);
	delta = newNumStars - (int)stars.size();

	if(delta == 0)
		delta = (randomU() < 0.5)?(1):(-1);
	delta = abs(delta);

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

double StarFieldModel::perturbHelper3()
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

double StarFieldModel::perturbHelper4()
{
/*
	// Amount to change the number of atoms by
	int delta = (int)round(maxNumAtoms*pow(10.0, 1.5 - 6.0*randomU())*randn());
	int newNumAtoms = (int)atoms.size() + delta;
	newNumAtoms = mod(newNumAtoms, maxNumAtoms);
	delta = newNumAtoms - (int)atoms.size();

	if(delta == 0)
		delta = (randomU() < 0.5)?(1):(-1);

//	if(((int)atoms.size() + delta > maxNumAtoms) || ((int)atoms.size() + delta < 0))
//		return 0;

	if(delta > 0)
		for(int i=0; i<delta; i++)
			addAtom();
	else if(delta < 0)
		for(int i=0; i<-delta; i++)
			removeAtom();
	else
		cerr<<delta<<" This shouldn't happen."<<endl;
*/

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

void StarFieldModel::calculateMockImage()
{
	mockImage.zero();
	for(size_t i=0; i<stars.size(); i++)
		stars[i].incrementImage(mockImage, psf);
	staleness = 0;
}

void StarFieldModel::calculateLogLikelihood()
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

void StarFieldModel::print(ostream& out) const
{
	out<<stars.size()<<' '<<hyp<<' '<<mockImage;
}

