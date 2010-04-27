/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file StructureTensor.hxx
/// Implementation of the parameter class StructureTensor
/// \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
/// \date 06.10.2009

#ifndef STRUCTURE_TENSOR_HXX_
#define STRUCTURE_TENSOR_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "StructureTensor.h"
#include <cassert>

template <typename T>
StructureTensor<T>::StructureTensor(const std::string& name) :
		TemplatedParameteredObject<T>("StructureTensor", name, 
			"Structure Tensor and Eigenvector analysis")
{
	ParameteredObject::_addInputSlot(in, "in", 
		"image input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(out, "out",
		"self pointer", "StructureTensor*");
	ParameteredObject::_addOutputSlot(tensor, "tensor",
		"structure tensor", "CImgList<T>");
	ParameteredObject::_addOutputSlot(eigenvalues, "eigenvalues",
		"eigenvalues", "CImgList<T>");
	ParameteredObject::_addOutputSlot(eigenvectors, "eigenvectors",
		"eigenvectors", "CImgList<T>");
	ParameteredObject::_addParameter(centralScheme, "centralScheme",
		"scheme for structure tensor calculation", false);
	out = this;
}

template <typename T>
void StructureTensor<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	// this can only handle sequences with one channel
	assert(in().size() == 1u);
	// we have to change the dimensions because CImg stores the
	// tensor elements into the v-channels.
	cimg_library::CImgList<T> temp(in()[0].spectrum(),in()[0].width(),
		in()[0].height(),in()[0].depth(),1u);
	cimg_library::CImg<T> mat, val, vec;
	if(in()[0].depth()==1u) {
		// 2D
		tensor().assign(3u,in()[0].width(),in()[0].height(),in()[0].depth(),
			in()[0].spectrum());
		eigenvalues().assign(2u,in()[0].width(),in()[0].height(),in()[0].depth(),
			in()[0].spectrum());
		eigenvectors().assign(4u,in()[0].width(),in()[0].height(),in()[0].depth(),
			in()[0].spectrum());
		mat.assign(2,2);
		val.assign(1,2);
		vec.assign(2,2);
	}
	else {
		// 3D
		tensor().assign(6u,in()[0].width(),in()[0].height(),in()[0].depth(),
			in()[0].spectrum());
		eigenvalues().assign(3u,in()[0].width(),in()[0].height(),in()[0].depth(),
			in()[0].spectrum());
		eigenvectors().assign(9u,in()[0].width(),in()[0].height(),in()[0].depth(),
			in()[0].spectrum());
		mat.assign(3,3);
		val.assign(1,3);
		vec.assign(3,3);
	}
	cimg_forXYZC(in()[0u], x,y,z,t)
		temp(t,x,y,z,0u) = in()(0u,x,y,z,t);
	cimglist_for(temp, t) {
		temp[t].structure_tensors(centralScheme());
		// change back dimensions
		cimg_forXYZ(temp[t],x,y,z) {
			cimg_forC(temp[t],v)
				tensor()(v,x,y,z,t) = temp(t,x,y,z,v);
			if(temp[t].spectrum()==3u) {
				// 2D
				mat.fill(
						temp(t,x,y,z,0),temp(t,x,y,z,1),
						temp(t,x,y,z,1),temp(t,x,y,z,2)
				);
			}
			else {
				// 3D
				assert(temp[t].spectrum()==6u);
				mat.fill(
						temp(t,x,y,z,0),temp(t,x,y,z,1),temp(t,x,y,z,2),
						temp(t,x,y,z,1),temp(t,x,y,z,3),temp(t,x,y,z,4),
						temp(t,x,y,z,2),temp(t,x,y,z,4),temp(t,x,y,z,5)
				);
			}
			// calculate eigenvectors and eigenvalues
			mat.symmetric_eigen(val,vec);
			// fill into output slots
			assert(val.width()==1);
			cimg_forY(val,j) {
				eigenvalues()(j,x,y,z,t)=val[j];
				// check if sorted
				assert((j==0) || (val[j] <= val[j-1]));
			}
			assert(vec.width()==vec.height());
			cimg_forXY(vec,i,j)
				eigenvectors()(i*vec.width()+j,x,y,z,t)=vec(i,j);
		}
	}
}

#endif // STRUCTURE_TENSOR_HXX_

