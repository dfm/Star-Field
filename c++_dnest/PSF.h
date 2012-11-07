#ifndef _PSF_
#define _PSF_

#include <ostream>

// Pixel-convolved PSF
class PSF
{
	public:
		static const double edge; // How many sigmas out to set PSF to zero

	private:
		// Fundamental parameters
		double sigma1, sigma2, weight;

		// Derived parameters
		double pr1, pr2; // pr = 1/sigma^2
		double preFactor1, preFactor2;
		double rEdge, rsqEdge;

		// Keep derived parameters in sync with fundamental ones
		void sync();

	public:
		PSF();
		PSF(double sigma1, double sigma2, double weight);
		double evaluate(double x, double y) const;
		double get_rEdge() const { return rEdge; }

		// Can infer PSF!
		void fromPrior();
		double perturb();

		friend std::ostream& operator << (std::ostream& out,
							const PSF& psf);
};

#endif

