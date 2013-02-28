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


#ifndef _StarFieldModel_
#define _StarFieldModel_

#include "Model.h"
#include <ostream>
#include <vector>
#include "Star.h"
#include "Array.h"
#include "Data.h"
#include "PSF.h"
#include "Hyperparameters.h"

template<class HyperType>
class StarFieldModel:public DNest3::Model
{
	private:
		static const int maxNumStars;

		PSF psf;
		double noiseSigma, noiseCoeff; // Noise properties
		double background;

		HyperType hyperparameters;
		int numStars;
		std::vector<Star> stars;

		Array mockImage;
		int staleness;
		double logL;

		void calculateMockImage();
		void calculateLogLikelihood();
		double perturb1();
		double perturb2();
		double perturb3();
		double perturb4();
		double perturb5();

	public:
		StarFieldModel();

		// Generate the point from the prior
		void fromPrior();

		// Metropolis-Hastings proposals
		double perturb();

		// Likelihood function
		double logLikelihood() const { return logL; }

		// Print to stream
		void print(std::ostream& out) const;
};

#include "StarFieldModelImpl.h"

#endif

