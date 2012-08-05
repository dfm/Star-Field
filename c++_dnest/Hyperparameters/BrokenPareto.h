#ifndef _BrokenPareto_
#define _BrokenPareto_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere, but their fluxes are tied together
*/
class BrokenPareto:public Hyperparameters
{
	private:
		// For star positions
		double xMin, xMax, yMin, yMax;

		double x1, x2; // Lower cutoff and break-point of flux PDF
		double a1, a2; // Slopes

		double Z1, Z2; // Normalisers
		void calculateZ();

		// These are useful for perturbStar1 and 2
		double perturb_x();
		double perturb_a();

		// These are useful for perturbStar2
		double fluxCDF(double f) const;
		double fluxInvCDF(double u) const;
		double fluxLogPDF(double f) const;

	public:
		BrokenPareto();

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

