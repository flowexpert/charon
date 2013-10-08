/*  Copyright (C) 2012, 2013
                  University of Heidelberg (IWR/HCI)

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

/** \file SimpleDiff.hxx
 *  Implementation of the parameter class SimpleDiff.
 *  \author Michael Baron
 *  \date 09.10.2012 - 09.01.2013
 */

#ifndef _SIMPLEDIFF_HXX_
#define _SIMPLEDIFF_HXX_

#include "SimpleDiff.h"
#include <charon/Warper.hxx>

#define ONLY_CHECK_BOUNDS -1

// cubic interpolation with mirroring at image borders
// derived from _cubic_atXY from CImg.h
template <typename T>
T SimpleDiff<T>::__cubic_atXY(cimg_library::CImg<T> img, const float fx, const float fy)
{
      const float
	nfx = fx<0?0:(fx>_width-1?_width-1:fx),
	nfy = fy<0?0:(fy>_height-1?_height-1:fy);
      const int x = (int)nfx, y = (int)nfy;
      const float dx = nfx - x, dy = nfy - y;
//    const int
//	px = x-1<0?0:x-1, nx = dx>0?x+1:x, ax = x+2>=_width?_width-1:x+2,
//	py = y-1<0?0:y-1, ny = dy>0?y+1:y, ay = y+2>=_height?_height-1:y+2;
      const int
	px = (x >= 1)          ? x-1 : 0,
	py = (y >= 1)          ? y-1 : 0,
	nx = (x <= _width-2)   ? x+1 : _width-1,
	ny = (y <= _height-2)  ? y+1 : _height-1,
	ax = (x <= _width-3)   ? x+2 : (x == _width-2)  ? _width-1  : _width-2,
	ay = (y <= _height-3)  ? y+2 : (y == _height-2) ? _height-1 : _height-2;
      const T
	Ipp = (T)img(px,py), Icp = (T)img(x,py), Inp = (T)img(nx,py), Iap = (T)img(ax,py),
	Ip = Icp + 0.5f*(dx*(-Ipp+Inp) + dx*dx*(2*Ipp-5*Icp+4*Inp-Iap) + dx*dx*dx*(-Ipp+3*Icp-3*Inp+Iap)),
	Ipc = (T)img(px,y),  Icc = (T)img(x, y), Inc = (T)img(nx,y),  Iac = (T)img(ax,y),
	Ic = Icc + 0.5f*(dx*(-Ipc+Inc) + dx*dx*(2*Ipc-5*Icc+4*Inc-Iac) + dx*dx*dx*(-Ipc+3*Icc-3*Inc+Iac)),
	Ipn = (T)img(px,ny), Icn = (T)img(x,ny), Inn = (T)img(nx,ny), Ian = (T)img(ax,ny),
	In = Icn + 0.5f*(dx*(-Ipn+Inn) + dx*dx*(2*Ipn-5*Icn+4*Inn-Ian) + dx*dx*dx*(-Ipn+3*Icn-3*Inn+Ian)),
	Ipa = (T)img(px,ay), Ica = (T)img(x,ay), Ina = (T)img(nx,ay), Iaa = (T)img(ax,ay),
	Ia = Ica + 0.5f*(dx*(-Ipa+Ina) + dx*dx*(2*Ipa-5*Ica+4*Ina-Iaa) + dx*dx*dx*(-Ipa+3*Ica-3*Ina+Iaa));
      return Ic + 0.5f*(dy*(-Ip+In) + dy*dy*(2*Ip-5*Ic+4*In-Ia) + dy*dy*dy*(-Ip+3*Ic-3*In+Ia));
}

template <typename T>
SimpleDiff<T>::SimpleDiff(const std::string& name) :
		TemplatedParameteredObject<T>("SimpleDiff", name,
			"This module performs differentiation like proposed by D. Sun within his "
			"Classic+NL algorithm<br>"
			"The differentiation process itself utilizes the 5 point filter mask, "
			"that optimally approximates the transfer function of an ideal differentiator, "
			"proposed by J&auml;hne in LNCS751. "
			"The process is additionally enhanced by some not straightforward boundary "
			"handling, <i>before</i> the warping step ist being performed. "
			"Therefore, this makes use of a so-called Warper module, in order to be able "
			"to process the original image data. "
		)
{
	ParameteredObject::_addInputSlot(
			img, "img",
			"image input", "CImgList<T>");
	ParameteredObject::_addInputSlot(
			warper, "warper",
			"warper input", "Warper<T>*");

	ParameteredObject::_addOutputSlot(
			dx, "dx", "derivative wrt x", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dy, "dy", "derivative wrt y", "CImgList<T>");
	ParameteredObject::_addOutputSlot(
			dt, "dt", "derivative wrt t", "CImgList<T>");

	ParameteredObject::_setTags("charon-flow;Differentiators;CImg");

	_width = 0;
	_height = 0;
}

template <typename T>
void SimpleDiff<T>::execute() {
	sout << "\tcalculating derivatives:" << std::endl;

	Warper<T> *_warper = warper();

	_width = img()[0].width();
	_height = img()[0].height();
	cimg_library::CImg<T> tmp( _width, _height );
	cimg_library::CImg<T> tmp1( _width, _height );

	if(dx.connected()) {
		sout << "\t\twrt. dx" << std::endl;
		dx().assign(img());
		cimglist_for( img(), kk )
		cimg_forC( img()[kk], c ) {
			cimg_forXYZ( img()[kk], x, y, z ) {
				tmp.atXYZC(x,y,z,c) = img()[kk].atXYZC(x,y,z,c);
			}
			cimg_forXY( tmp, x, y )
			{
				if ( _warper->getX(ONLY_CHECK_BOUNDS,x,y,x,y) && _warper->getY(ONLY_CHECK_BOUNDS,x,y,x,y) ) {
					dx().atNXYZC(0,x,y,0,c) = (   __cubic_atXY( tmp, _warper->getX(c,x-2,y,x,y), _warper->getY(c,x,y,x,y) )
					                          - 8*__cubic_atXY( tmp, _warper->getX(c,x-1,y,x,y), _warper->getY(c,x,y,x,y) )
				        	                  + 8*__cubic_atXY( tmp, _warper->getX(c,x+1,y,x,y), _warper->getY(c,x,y,x,y) )
				                	          -   __cubic_atXY( tmp, _warper->getX(c,x+2,y,x,y), _warper->getY(c,x,y,x,y) ) ) / T(12.0);
				} else {
					dx().atNXYZC(0,x,y,0,c) = T(0);
				}
			}
		}
		cimg_forXY( img()[0], x, y )
		{
			dx().atNXYZC(0,x,y,0,0) += dx().atNXYZC(0,x,y,0,1) ;
			dx().atNXYZC(0,x,y,0,0) /= T(2.0) ;
			dx().atNXYZC(0,x,y,0,1) =  dx().atNXYZC(0,x,y,0,0) ;
		}
	}
	if(dy.connected()) {
		sout << "\t\twrt. dy" << std::endl;
		dy().assign(img());
		cimglist_for( img(), kk )
		cimg_forC( img()[kk], c ) {
			cimg_forXYZ( img()[kk], x, y, z ) {
				tmp.atXYZC(x,y,z,c) = img()[kk].atXYZC(x,y,z,c);
			}
			cimg_forXY( tmp, x, y )
			{
				if ( _warper->getX(ONLY_CHECK_BOUNDS,x,y,x,y) && _warper->getY(ONLY_CHECK_BOUNDS,x,y,x,y) ) {
					dy().atNXYZC(0,x,y,0,c) = (   __cubic_atXY( tmp, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y-2,x,y) )
					                          - 8*__cubic_atXY( tmp, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y-1,x,y) )
					                          + 8*__cubic_atXY( tmp, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y+1,x,y) )
				        	                  -   __cubic_atXY( tmp, _warper->getX(c,x,y,x,y), _warper->getY(c,x,y+2,x,y) ) ) / T(12.0);
				} else {
					dy().atNXYZC(0,x,y,0,c) = T(0);
				}
			}
		}
		cimg_forXY( img()[0], x, y )
		{
			dy().atNXYZC(0,x,y,0,0) += dy().atNXYZC(0,x,y,0,1) ;
			dy().atNXYZC(0,x,y,0,0) /= T(2.0) ;
			dy().atNXYZC(0,x,y,0,1) =  dy().atNXYZC(0,x,y,0,0) ;
		}
	}
	if(dt.connected()) {
		sout << "\t\twrt. dt" << std::endl;
		dt().assign(img());
		cimglist_for( img(), kk )
		cimg_forC( img()[kk], c ) {
			cimg_forXYZ( img()[kk], x, y, z ) {
				tmp1.atXYZC(x,y,z,c) = img()[kk].atXYZC(x,y,z,1);
				tmp.atXYZC(x,y,z,c) = img()[kk].atXYZC(x,y,z,0);
			}
			cimg_forXY( img()[0], x, y )
			{
				if ( _warper->getX(ONLY_CHECK_BOUNDS,x,y,x,y) && _warper->getY(ONLY_CHECK_BOUNDS,x,y,x,y) ) {
					dt().atNXYZC(0,x,y,0,c) = __cubic_atXY( tmp1, _warper->getX(1,x,y,x,y), _warper->getY(1,x,y,x,y) )
				        	                - __cubic_atXY( tmp,  _warper->getX(0,x,y,x,y), _warper->getY(0,x,y,x,y) );
				} else {
					dt().atNXYZC(0,x,y,0,c) = T(0);
				}
			}
		}
	}
}
#endif /* _SIMPLEDIFF_HXX_ */

