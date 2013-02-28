#ifndef _Array_
#define _Array_

#include <vector>

class Array
{
	protected:
		int ni, nj, size;
		std::vector< std::vector<double> > pixels;

	public:
		Array();
		Array(int ni, int nj);

		// Resize (destructive)
		void resize(int ni, int j);

		// Set all pixels to given value
		void set(double value);
		void increment(double value);
		void decrement(double value);

		// Const and non-const getter
		double operator() (int i, int j) const { return pixels[i][j]; }
		double& operator() (int i, int j) { return pixels[i][j]; }

};

#endif


