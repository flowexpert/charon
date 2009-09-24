/*  Copyright (C) 2009 René Steinbrügge

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file LeastSquare.cpp
/// implements class LeastSquare
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "LeastSquare.h"

using namespace std;
using namespace cimg_library;

using namespace Fitters;

void LeastSquare::computeParams(const cimg_library::CImg<> &img, CImg<>& res)
{
	//cout << "übergeben";
	//functions::printMatrix(img);
	CImg<> m = img.get_transpose() * img;
	CImg<> val, vec;
	m.symmetric_eigen(val,vec);
// 	cout << "Eigenvectors";
// 	Functions::printMatrix(vec);
// 	cout << "Eigenvalues";
// 	Functions::printMatrix(val);
	int i=1;
	while ((vec(vec.width-i,0) == 0) && (i<vec.width) ) i++; //TODO ==0 ?!
	res = vec.get_column(vec.width-i);
	//cout << " r first:"<<res.first() << endl;
	res *= (1.0/res.first());
	res.crop(0,1,0,0,res.height-1,0);
}

