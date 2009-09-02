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
/// @file functions.cpp
/// implements namespace Functions
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "functions.h"

using namespace std;
using namespace cimg_library;

CImg<> Functions::gauss_filter(float sigma)
{
	int sigma3 = (int)(sigma*3.0);
	CImg<> r(sigma3*2+1,sigma3*2+1,1,1,0.0);
	float foo[] = {1.0/(2.0*cimg::valuePI*sigma*sigma)};
	r.draw_gaussian(sigma3,sigma3,sigma,foo);
	return r;
}

CImg<> Functions::gauss_filter3d(float sigma)
{
	int sigma3 = (int)(sigma*3.0);
	CImg<> r(sigma3*2+1,sigma3*2+1,sigma3*2+1,1,0.0);
	float foo[] = {1.0/(2.0*cimg::valuePI*sigma*sigma*sqrt(2.0*cimg::valuePI*sigma*sigma))};
	//float foo[] = {1};
	r.draw_gaussian(sigma3,sigma3,sigma3,sigma,foo);
	return r;
}

/*void functions::assemblertest(DataAssembler* assembler)
{
	for (assembler->init(5,19); !assembler->end(); assembler->next())
	{
		cout << assembler->getX() << "," << assembler->getY() << endl;
	}
} */

CImg<> Functions::falte_filter(const CImg<float>& f1, const CImg<float>& f2)
{
	CImg<> r(f1.width+f2.width-1 ,f1.height+f2.height-1 ,1,1,0.0);
	cimg_forXY(f1,x1,y1)
		cimg_forXY(f2,x2,y2)
			{
				r(x1+x2,y1+y2) += f1(x1,y1)*f2(x2,y2);
			}
	return r;
}

void Functions::printMatrix(const CImg<>& m)
{
	cout << endl;
	for(unsigned int i = 0; i< m.height; i++)
		{
		cout << m(0,i);
		for(unsigned int j = 1; j< m.width; j++)
			{
				cout << "\t" << m(j,i) ;
			}
		cout << endl;
		}
}
