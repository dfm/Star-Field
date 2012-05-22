#ifndef _Pareto_
#define _Pareto_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere, but their fluxes are tied together
*/
class Pareto:public Hyperparameters
{
	private:
		double minLogMu, maxLogMu, rangeLogMu;
		double xMin, xMax, yMin, yMax;

		double mu; // Mean flux of stars

		// These are useful for perturbStar2
		double fluxCDF(double f) const;
		double fluxInvCDF(double u) const;

	public:
		Pareto();

		// Generate hyperparameters from the prior
		void fromPrior();

		// Metropolis-Hastings proposal - move stars
		double perturb1(std::vector<Star>& stars);

		// Metropolis-Hastings proposal - don't move stars
		double perturb2(const std::vector<Star>& stars);

		/* Print the hyperparameters */
		void print(std::ostream& out) const;

		// Generate a star
		Star generateStar() const;

		// Perturb a star (position)
		double perturbStar1(Star& star, double scale) const;

		// Perturb a star (flux)
		double perturbStar2(Star& star, double scale) const;
};

#endif

