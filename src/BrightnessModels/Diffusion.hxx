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
/// @file Diffusion.cpp
/// implements class Diffusion
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#ifndef _Diffusion_hxx_
#define _Diffusion_hxx_

#include "Diffusion.h"
#include <vector>

using namespace std;
using namespace cimg_library;

template <typename T>
BrightnessModels::Diffusion<T>::Diffusion(const string& name) :
	BrightnessModel<T>("brightnessmodels_diffusion",name)
{
	_addInputSlot(dxx, "dxx","2nd derivative in x-direction","CImgList");
	_addInputSlot(dyy, "dyy","2nd derivative in y-direction","CImgList");
	_addInputSlot(dzz, "dzz","2nd derivative in z-direction","CImgList");
	
	this->brightnessFunctor = &functor;
	
	this->setFunctorParams(1.5);
}

template <typename T>
std::set<std::string>& BrightnessModels::Diffusion<T>::getUnknowns()
{
	this->unknowns.insert("d");
	return this->unknowns;
}

template <class T>
void BrightnessModels::Diffusion<T>::compute(	const int xs, const int ys, const int zs,
											const int t, const int v,
											std::map<std::string, T>& term,
											T& rhs)
{
	//assert(term.width == 1);
	if (dxx().size == 1) // 2D
		term["d"] += dxx()(0,xs,ys,t,v) + dyy()(0,xs,ys,t,v);
	else // 3D
		term["d"] += dxx()(t,xs,ys,zs,v) + dyy()(t,xs,ys,zs,v) + dzz()(t,xs,ys,zs,v);
}

template <class T>
void BrightnessModels::Diffusion<T>::Functor::operator ()(cimg_library::CImg<T> &sequence) const
{
	for(unsigned int t=1; t< sequence.depth; t++)
		sequence.draw_image(0,0,t,
				sequence.get_slice(t).convolve(Functions::gauss_filter(sqrt(2*(this->d)*t))));
}

template <class T>
void BrightnessModels::Diffusion<T>::Functor::get3d(cimg_library::CImgList<T> &sequence) const
{
	for(unsigned int t=1; t< sequence.size; t++)
	{
		cout << t << endl;
		sequence(t).convolve(Functions::gauss_filter3d(sqrt(2.0*(this->d)*t)));
	}
}

template <class T>
void BrightnessModels::Diffusion<T>::setFunctorParams(float d)
{
	this->functor.d = d;
}

template <class T>
void BrightnessModels::Diffusion<T>::apply(const std::vector<Parameter<T>* > & modifier)
{
/* std::string d, x, y, z, t;
    x = modifier[0];
    y = modifier[1];
    z = modifier[2];
    d = modifier[3];
    T red, green, blue;
    ///@TODO könnte falsch sein
    red 	= image.get_crop(atoi(x.c_str()), atoi(y.c_str()), atoi(z.c_str()),
    		                 atoi(t.c_str()), 0);
    green	= image.get_crop(atoi(x.c_str()), atoi(y.c_str()), atoi(z.c_str()),
    		                 atoi(t.c_str()), 1);
    blue	= image.get_crop(atoi(x.c_str()), atoi(y.c_str()), atoi(z.c_str()),
    		                 atoi(t.c_str()), 2);
    float D;
    D = atof(d.c_str());
//    red 	= D*(((zeichen^2*g)/(zeichen*x^2))+((zeichen^2*g)/(zeichen*y^2))); ///@TODO change to something funktional
//    green 	= D*(((zeichen^2*g)/(zeichen*x^2))+((zeichen^2*g)/(zeichen*y^2))); ///@TODO change to something funktional
//    blue	= D*(((zeichen^2*g)/(zeichen*x^2))+((zeichen^2*g)/(zeichen*y^2))); ///@TODO change to something funktional
*/
}

#endif
