#include "Hyperparameters.h"

Star Hyperparameters::generateStar(double u_x, double u_y, double u_f) const
{
	double x, y, f;
	transform(u_x, u_y, u_f, x, y, f);
	return Star(x, y, f);
}

