/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest.
*
* DNest is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "Start.h"
#include "Data.h"
#include "StarFieldModel.h"
#include "Hyperparameters/BrokenPareto.h"

using namespace std;

int main(int argc, char** argv)
{
	Data::load_data("SimulatedData/100.txt");
	DNest3::start_mt< StarFieldModel<BrokenPareto> >(argc, argv);
	return 0;
}

