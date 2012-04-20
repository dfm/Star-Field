#ifndef _Image_
#define _Image_

#include <ostream>
#include <istream>
#include <Eigen/Core>
#include "Array.h"

/*
* Implement an image as a 2D array of doubles, with bounding
* information. Pixels need not be square, but you probably want
* them to be most of the time.
*/
class Image
{
	private:
		// Fundamental image parameters
		int nI, nJ;	// Number of pixels in y and x dir respectively
		double xMin, xMax, yMin, yMax; // The domain, extending to the edges

		// Derived image parameters
		double xRange, yRange;	// Total width and height of the image
		double dx, dy;	// Pixel widths
		double dA;	// Pixel area

		// The actual pixel values
		Array pixels;

		// Compute derived image parameters from fundamental ones
		void compute();

	public:
		/*
		* Main constructor: set up image size (number of pixels)
		* and image domain
		*/
		Image(int nI, int nJ, double xMin, double xMax, double yMin, double yMax);

		/*
		* Another constructor: same as main constructor but set all
		* pixels to a single value, the last parameter
		*/
		Image(int nI, int nJ, double xMin, double xMax, double yMin, double yMax, double value);

		/*
		* Set all pixels to the specified value
		*/
		void setConstant(double value);

		/*
		* Getters
		*/
		int get_nI() const { return nI; }
		int get_nJ() const { return nJ; }
		double get_xMin() const { return xMin; }
		double get_xMax() const { return xMax; }
		double get_yMin() const { return yMin; }
		double get_yMax() const { return yMax; }
		double get_xRange() const { return xRange; }
		double get_yRange() const { return yRange; }
		double get_dx() const { return dx; }
		double get_dy() const { return dy; }
		double get_dA() const { return dA; }

		/*
		* MEMBER OPERATOR OVERLOADS
		*/

		// Read/write pixel access
		double& operator () (int i, int j);

		/*
		* Increment this image's pixels by another image.
		* Image sizes and domain must match!
		*/
		Image& operator += (const Image& other);

		/*
		* Decrement this image's pixels by another image.
		* Image sizes and domain must match!
		*/
		Image& operator -= (const Image& other);


		/*
		* DECLARE NON-MEMBER OPERATOR OVERLOADS AS FRIEND
		*/

		/*
		* Outputs image to stream
		* with the size+domain info on line 1 after a # character,
		* followed by the pixel values.
		*/
		friend std::ostream& operator << (std::ostream& out, const Image& image);

		/*
		* Read image from a stream in the same format that <<
		* uses for output. Behaviour is undefined for incorrect
		* formats.
		*/
		friend std::istream& operator >> (std::istream& in, Image& image);

		/*
		* Check for total equality of the images
		* No need to check derived image parameters
		*/
		friend bool operator == (const Image& image1, const Image& image2);
};

#endif

