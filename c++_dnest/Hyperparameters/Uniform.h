#ifndef _Uniform_
#define _Uniform_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere, but their fluxes are tied together
*/
class Uniform:public Hyperparameters
{
	private:
		static const double logMinMu, logMaxMu;

		double onFraction; // Fraction of stars that are on
		double mu; // Mean flux of stars

	public:
		// Generate hyperparameters from the prior
		void fromPrior();

		// Metropolis-Hastings proposal
		double perturb();

		/*
		* Transform U(0, 1) latent variables into positions 
		* and fluxes
		*/
		void transform(double u_x, double u_y, double u_f,
				double& x, double& y, double& f) const;

		/* Print the hyperparameters */
		void print(std::ostream& out) const;
};

#endif

