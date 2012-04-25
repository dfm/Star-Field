#ifndef _PSF_
#define _PSF_

#include "Image.h"

// Pixel-convolved PSF
class PSF
{
	public:
		static const double edge; // How many sigmas out to set PSF to zero

	private:
		double sigma1, sigma2;
		double weight;

		double preFactor1, preFactor2;

	public:
		PSF(double sigma1, double sigma2, double weight);
		double evaluate(double x, double y) const;
};

#endif

