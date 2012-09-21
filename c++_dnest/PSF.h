#ifndef _PSF_
#define _PSF_

// Pixel-convolved PSF
class PSF
{
	public:
		static const double edge; // How many sigmas out to set PSF to zero

	private:
		double sigma1, sigma2, pr1, pr2; // pr = 1/sigma^2
		double weight;

		double preFactor1, preFactor2;
		double rEdge, rsqEdge;

	public:
		PSF();
		PSF(double sigma1, double sigma2, double weight);
		double evaluate(double x, double y) const;
		void set(double sigma1, double sigma2, double weight);
		double get_rEdge() const { return rEdge; }
};

#endif

