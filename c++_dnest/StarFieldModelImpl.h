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
const double StarFieldModel<HyperType>::noiseSigma = 30.;

template<class HyperType>
const double StarFieldModel<HyperType>::noiseCoeff = 0;

template<class HyperType>
const int StarFieldModel<HyperType>::maxNumStars = 200;

template<class HyperType>
PSF StarFieldModel<HyperType>::psf;

template<class HyperType>
StarFieldModel<HyperType>::StarFieldModel()
:uniforms(maxNumStars)
,stars(maxNumStars)
mockImage(Data::get_data().get_ni(), Data::get_data().get_nj())
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
	for(int i=0; i<maxNumStars; i++)
	{
		uniforms[i] = randomU();
	}
	calculateMockImage();

	Model::fromPrior();
	calculateLogLikelihood();
}

template<class HyperType>
double StarFieldModel<HyperType>::perturb()
{
	double logH = 0.0;
	int which = randInt(2);

	if(which == 0)
		logH = perturbHelper1();
	else if(which == 1)
		logH = perturbHelper2();

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

	return logH;
}

template<class HyperType>
double StarFieldModel<HyperType>::perturbHelper2()
{
	double logH = 0;

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
void StarFieldModel<HyperType>::calculateLogLikelihood()
{
	logl.logl = 0.0;

	double var;
	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
		{
			var = pow(noiseSigma, 2)
				+ pow(noiseCoeff*mockImage(i, j), 2);
			logl.logl += -0.5*(2*M_PI*var) 
				- 0.5*pow(Data::get_data()(i, j)
				- mockImage(i, j), 2)/var;
		}
}

template<class HyperType>
void StarFieldModel<HyperType>::print(ostream& out) const
{
	out<<staleness<<' '<<hyperparameters<<' ';
	for(int i=0; i<Data::get_data().get_ni(); i++)
		for(int j=0; j<Data::get_data().get_nj(); j++)
			out<<mockImage(i, j)<<' ';
}

