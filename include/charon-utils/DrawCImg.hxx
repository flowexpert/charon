/*  Copyright (C) 2011 Julian Coordts

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
/** \file DrawCImg.hxx
 *  Implementation of the class EmptyCImg.
 *  \author Julian Coordts
 *  \date 13.01.2012
 */

#ifndef _DRAWCIMG_HXX_
#define _DRAWCIMG_HXX_

#include "DrawCImg.h"
using namespace cimg_library ;

template <typename T>
DrawCImg<T>::DrawCImg(const std::string& name) :
        TemplatedParameteredObject<T>("drawcimg", name,
			"draws one cimg into another")
{
	ParameteredObject::_addParameter (_posX, "posx", 
		"the x coordinate where the source should be placed on target", 0) ;
	
	ParameteredObject::_addParameter (_posY, "posy", 
		"the y coordinate where the source should be placed on target", 0) ;

	ParameteredObject::_addParameter (_opacity, "opacity", 
		"the city where opa lives", 0.0f) ;

	ParameteredObject::_addParameter (_skipBlack, "skipBlack", 
		"doesn't copies black pixels", false) ;

	ParameteredObject::_addInputSlot(_inputTarget, "inputTarget", 
		"input target image", "CImgList<T>") ;

	ParameteredObject::_addInputSlot(_inputSource, "inputSource", 
		"input source image (that what get's painted)", "CImgList<T>") ;

	ParameteredObject::_addOutputSlot(_output, "output", 
		"output image", "CImgList<T>"); 
}

template <typename T>
void DrawCImg<T>::execute() {

	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const CImgList<T>& inputTarget = _inputTarget() ; 
	const CImgList<T>& inputSource = _inputSource() ;
	CImgList<T>& output = _output() ; //similiar for output slots
	
	output.assign(inputTarget.size(),
		inputTarget[0].width(), inputTarget[0].height(), inputTarget[0].depth(),inputTarget[0].spectrum());
	//check if input is valid
	cimglist_for(inputSource,l)
	{
		output(l).assign(inputTarget(l));
		output(l).draw_image(_posX(),_posY(),0,0,inputSource(l),_opacity());
	}

}

#endif /* _DrawCImg_HXX_ */
