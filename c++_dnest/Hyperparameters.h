#ifndef _Hyperparameters_
#define _Hyperparameters_

/*
* An abstract base class for describing
* models for the spatial and flux distribution
* of the stars.
*/

#include <vector>
#include <iostream>

class Hyperparameters
{
	protected:
		// Parameters go here when you inherit

	public:

		/* Stuff that derived classes must implement */

		// Generate hyperparameters from the prior
		virtual void fromPrior() = 0;

		// Metropolis-Hastings proposal
		virtual double perturb() = 0;

		/*
		* Transform U(0, 1) latent variables into positions 
		* and fluxes
		*/
		virtual void transform(double u_x, double u_y, double u_f,
				double& x, double& y, double& f) const = 0;

		/* Print the hyperparameters */
		virtual void print(std::ostream& out) const = 0;
};

#endif

