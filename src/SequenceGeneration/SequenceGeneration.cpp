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
/// @file SequenceGeneration.cpp
/// implements class SequenceGeneration
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 27.05.2009

#include "SequenceGeneration.h"

using namespace std;
using namespace cimg_library;

/*
CImg<> SequenceGeneration::generateImage(int x, int y)
{
	CImg<> r(x,y);
	cimg_forXY(r,i,j)
	{
		r(i,j) = (sin(i/10.0)+sin(j/10.0))*64.0+128.0;
	}
	//img = r;
	return r;
}//*/

float SequenceGeneration::imageAt(float x, float y, int t, const FlowFunctorInterface& flowfunc)
{
	float f[2];
	while (t>0)
	{
		flowfunc(x,y,f);
		x -= f[0];
		y -= f[1];
		t--;
	}
	return /*exp(-t1*0.02)**/((sin(x/10.0)+sin(y/10.0))*64.0+128.0);
}

float SequenceGeneration::imageAt3d(float x, float y, float z, int t, const FlowFunctorInterface& flowfunc)
{
	float f[3];
	while (t>0)
	{
		flowfunc.get3d(x,y,z,f);
		x -= f[0];
		y -= f[1];
		z -= f[2];
		t--;
	}
	return /*exp(-t1*0.02)**/((sin(x/10.0)+sin(y/10.0)+sin(z/10.0))*128.0/3.0+128.0);
}

 /* Veraltet: Durch FlowFunc �berholt
float* SequenceGeneration::getFlow(int x, int y)
{
	//float a1=0.34, a2=0.0, b11=0.0, b12=0.00, b21=0.0, b22= 0.0, \
			c1= 0.00, c2=0.00;
	float a1=0.5, a2=0.4, b11=0.001*3, b12=-0.002*3, b21=0.002*3, b22= 0.001*3, \
			c1= 0.0008*0, c2=0.00013*0;
	//float a1=0.0, a2=0.0, b11=0.000, b12=-0.000, b21=0.000, b22= 0.000,	\
			c1= 0.0008*0, c2=0.00013*0;
	float r[2];
	r[0] = a1 + b11*x + b12*y + x*x*c1 + x*y*c2;
	r[1] = a2 + b21*x + b22*y + x*y*c1 + y*y*c2;
	return r;
} // */


/* Veraltet: Jetzt �ber BrightnessFunctor implementiert
void SequenceGeneration::brightnessChange(CImg<>& sequence)
{
	// F�rs Exponentialmodel
	for(int t=1; t< sequence.depth; t++)
	{
		cimg_forXY(sequence,x,y)
		{
			sequence(x,y,t) *= exp(-0.0*t);
		}
	} 
	
	 // Diffusionsmodell
	for(int t=1; t< sequence.depth; t++)
		sequence.draw_image(0,0,t,
							sequence.get_slice(t).convolve(functions::gauss_filter(sqrt(2*0.5*t))));
	
}

*/



SequenceGeneration::SequenceGeneration(const std::string& name) :
		ParameteredObject("sequencegeneration",name,"generates Sequence for the models")
{
	_addInputSlot(flowFunctor,"flowfunctor","flowFunctor of MotionModel","FlowFunctorInterface*");
	_addInputSlot(brightFunctor,"brightfunctor","brightFunctor of BrightnessModel","BrightnessFunctorInterface*");
	_addOutputSlot(result,"sequence","generatet Sequence","CImgList");
	
	_addParameter(width,"width","width of sequence");
	_addParameter(height,"height","height of sequence");
	_addParameter(depth,"depth","depth of sequence");
	_addParameter(duration,"duration","duration of sequence");
	_addParameter(channels,"channels","number of channels of the sequence");
	
	_addParameter(xc,"xc","x-coordinate of center of motion");
	_addParameter(yc,"yc","y-coordinate of center of motion");
	_addParameter(zc,"zc","z-coordinate of center of motion");
	
	_addParameter(is3d,"is3d","set true to generate a 3d-sequence");
	
	// default parameters
	setParams(100,100,100,10,1,50,50,50);
	
}

void SequenceGeneration::update()
{
	generateSequence(width(),height(),depth(),duration(),channels(),xc(),yc(),zc(),
			 *flowFunctor(),*brightFunctor(),result());
	_outDataChanged(result);
}

inline void SequenceGeneration::setParams(int w, int h, int d, int t, int v, int xc, int yc, int zc)
{
	this->width = w;
	this->height = h;
	this->depth = d;
	this->duration = t;
	this->channels = v;
	this->xc = xc;
	this->yc = yc;
	this->zc = zc;
}
		

void SequenceGeneration::generateSequence(int const& width, int const& height, int const& depth,
					int const& t, int const& channels, int const& xc,
       					int const& yc, int const& zc, const FlowFunctorInterface& flowfunc,
					const BrightnessFunctorInterface& brfunc, CImgList<>& sequence)
{
	if (!is3d) //2d	
	{
		sequence=CImgList<>(1,width,height,t,channels);
		cimg_forXYZV(sequence[0],i,j,k,v)
			{
				sequence(0,i,j,k,v) = imageAt(i-xc,j-yc, k, flowfunc) / float(v+1);
			}
		brfunc(sequence[0]);
	}
	else //3d
	{
		sequence=CImgList<>(t,width,height,depth,channels);
		for (int i = 0; i<sequence.size;i++)
			cimg_forXYZV(sequence[i],x,y,z,v)
				sequence(i,x,y,z,v) = imageAt3d(x-xc,y-yc,z-zc,i,flowfunc) / float(v+1);
		brfunc.get3d(sequence);
	}
		
}

