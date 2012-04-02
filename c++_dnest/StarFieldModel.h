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
#include "StarHyperparameters.h"

class StarFieldModel:public DNest::Model
{
	private:
		static Data data;
		static PSF psf;
		static const int maxNumStars;

		StarHyperparameters hyp;
		std::vector<Star> stars;

		Array mockImage;
		static const double noiseSigma; 
		static const double noiseCoeff;

		int staleness;

		void calculateMockImage();
		double perturbHelper1();
		double perturbHelper2();
		double perturbHelper3();
		double perturbHelper4();

	public:
		StarFieldModel();
		~StarFieldModel();
		Model* factory() const;
		Model* clone() const;
		void copyFrom(const Model* other);

		void fromPrior();
		void calculateLogLikelihood();
		double perturb();
		double getValue();
		void print(std::ostream& out) const;
};
#endif

