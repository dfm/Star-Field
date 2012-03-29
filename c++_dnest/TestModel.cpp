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


#include "TestModel.h"
#include "Utils.h"
#include "RandomNumberGenerator.h"
#include <cmath>

using namespace std;
using namespace DNest;

const double TestModel::noiseSigma = 1E-5;
const double TestModel::noiseCoeff = 0;
const int TestModel::maxNumStars = 500;
Data TestModel::data;
PSF TestModel::psf(1.0*Data::dx, 5.0*Data::dx, 0.5);

TestModel::TestModel()
:mockImage(Data::ni, Data::nj)
{
	if(!data.isLoaded())
		data.load("data_100stars.txt");
}

TestModel::~TestModel()
{

}

Model* TestModel::factory() const
{
	return new TestModel;
}

Model* TestModel::clone() const
{
	return new TestModel(*this);
}

void TestModel::copyFrom(const Model* other)
{
	*this = *((TestModel*)other);
}

void TestModel::fromPrior(int thread)
{
	hyp.fromPrior(thread);
	int numStars = randInt(thread, maxNumStars + 1);
	for(int i=0; i<numStars; i++)
		stars.push_back(hyp.generateStar(thread));
	calculateMockImage();

	Model::fromPrior(thread);
	calculateLogLikelihood();
}

double TestModel::perturb(int thread)
{
	if(staleness >= 1000)
		calculateMockImage();

	double logH = 0.0;
	int which = randInt(thread, 4);

	if(which == 0)
		logH = perturbHelper1(thread);
	else if(which == 1)
		logH = perturbHelper2(thread);
	else if(which == 2)
		logH = perturbHelper3(thread);
	else
		logH = perturbHelper4(thread);

	Model::perturb(thread);
	calculateLogLikelihood();
	return logH;
}

double TestModel::perturbHelper1(int thread)
{
	if(stars.size() == 0)
		return 0;

	// Move a star in position
	double scale = pow(10.0, 1.5 - 6.0*randomU(thread));
	int which = randInt(thread, stars.size());

	double logH = -hyp.logp(stars[which]);
	stars[which].decrementImage(mockImage, psf);
	stars[which].x += hyp.sig*scale*randn(thread);
	stars[which].y += hyp.sig*scale*randn(thread);
	stars[which].incrementImage(mockImage, psf);
	logH += hyp.logp(stars[which]);

	staleness++;
	return logH;
}

double TestModel::perturbHelper2(int thread)
{
	if(stars.size() == 0)
		return 0;

	// Move a star in flux
	double scale = pow(10.0, 1.5 - 6.0*randomU(thread));
	int which = randInt(thread, stars.size());

	double logH = -hyp.logp(stars[which]);
	stars[which].decrementImage(mockImage, psf);
	stars[which].flux += hyp.meanFlux*scale*randn(thread);
	stars[which].incrementImage(mockImage, psf);
	logH += hyp.logp(stars[which]);

	staleness++;
	return logH;
}

double TestModel::perturbHelper3(int thread)
{
	// Move hyperparameters

	double logH = 0;
	if(randomU(thread) <= 0.5)
	{
		logH -= hyp.logp(stars);
		logH += hyp.perturb(thread);
		logH += hyp.logp(stars);
	}
	else
	{
		logH += hyp.perturb(thread, stars);
		calculateMockImage();
	}
	return logH;
}

double TestModel::perturbHelper4(int thread)
{
	// Add or remove a star
	if(randomU(thread) <= 0.5)
	{
		// Add
		if((int)stars.size() == maxNumStars)
			return 0;
		stars.push_back(hyp.generateStar(thread));
		stars.back().incrementImage(mockImage, psf);
	}
	else
	{
		// Remove
		if(stars.size() == 0)
			return 0;
		int which = randInt(thread, stars.size());
		stars[which].decrementImage(mockImage, psf);
		stars.erase(stars.begin() + which);
	}
	staleness++;

	return 0;
}

void TestModel::calculateMockImage()
{
	mockImage.zero();
	for(size_t i=0; i<stars.size(); i++)
		stars[i].incrementImage(mockImage, psf);
	staleness = 0;
}

void TestModel::calculateLogLikelihood()
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

void TestModel::print(ostream& out) const
{
	out<<stars.size()<<' '<<hyp<<' '<<mockImage;
}

