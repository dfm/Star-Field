#ifndef _PSF_
#define _PSF_

#include "Array.h"

// Pixel-convolved PSF
class PSF
{
	public:
		static const double edge; // How many sigmas out to set PSF to zero

	private:
		double sigma1, sigma2;
		double weight;
		double prefactor1, prefactor2;
		double norm;

	public:
		PSF(double sigma1, double sigma2, double weight);
		double evaluate(double x, double y) const;
		double evaluate(double rsq) const;
		Array evaluate(const Array& x, const Array& y) const;
		Array evaluate(const Array& rsq) const;
};

#endif

