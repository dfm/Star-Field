#ifndef _Uniform_
#define _Uniform_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere
*/
class Uniform:public Hyperparameters
{
	private:
		double mu; // Mean flux

		double xMin, xMax, yMin, yMax;
		static double minMu, maxMu;

	public:

		/* Implement undefined methods */
		void fromPrior();
		double perturb1(const std::vector<Star>& stars);
		double perturb2(std::vector<Star>& stars); // Take stars along for the ride

		// Generate a star from the prior
		// given the hyperparameters
		Star generateStar() const;
		double perturbPosition(Star& star, double scale) const;
		double perturbFlux(Star& star, double scale) const;

		// Evaluate the probability density
		// for a star given the hyperparameters
		double _logp(const Star& star) const;

		friend std::ostream& operator << (std::ostream& out, const Uniform& i);

};

#endif

