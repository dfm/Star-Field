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
		const double minLogMu, maxLogMu, rangeLogMu;

		double mu; // Actually, minimum flux
		double alpha; // Slope

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

};

#endif

