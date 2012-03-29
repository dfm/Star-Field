#ifndef _Array_
#define _Array_

#include <vector>
#include <ostream>

class Array
{
	private:
		int ni, nj;
		std::vector< std::vector<double> > values;

	public:
		Array(int ni, int nj);
		Array(int ni, int nj, double value);

		void zero();
		double sum() const;
		void normalise();
		int getNi() const;
		int getNj() const;

		const std::vector<double>& operator [] (int i) const; // For element access
		std::vector<double>& operator [] (int i); // Non-const case
		Array& operator += (const Array& other);

		static std::vector<Array> meshgrid(int ni, int nj, double xMin, double xMax, double yMin, double yMax);
		friend std::ostream& operator << (std::ostream& out, const Array& array);
};

#endif

