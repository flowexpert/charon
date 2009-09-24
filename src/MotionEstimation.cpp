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
/// @file MotionEstimation.cpp
/// implements class MotionEstimation
/// @author <a href="mailto:Steinbruegge@stud.uni-heidelberg.de">René Steinbrügge</a>
/// @date 07.07.2009

//Haupt-Klasse

#include "MotionEstimation.h"

using namespace std;
using namespace cimg_library;


/*void MotionEstimation::initialise()
{
	model->img = image;
	model->deriv = deriv;
	model->calculateDerivatives();
	brightModel->img = image;
	brightModel->initialize();
	
} //*/


MotionEstimation::MotionEstimation(const std::string& name ) :
		ParameteredObject("motionestimation", name ,"Computes Tensor")
{
	_addInputSlot(model,"motionmodel","Pointer to MotionModel","MotionModel*");
	_addInputSlot(brightModel,"brightnessmodel","Pointer to BrightnessModel","BrightnessModel*");
	_addInputSlot(assembler,"dataassembler","Pointer to DataAssembler","DataAssembler*");
	_addInputSlot(fitter,"fitter","Pointer to Fitter","Fitter*");
	_addInputSlot(roi,"roi","Pointer to Range of Interest","roi");
	_addOutputSlot(result,"result","computed parameters","CImgList");
	
	_addParameter(tbegin,"starttime","start time of RoI at which flow is computed");
	_addParameter(tend,"endtime","end time of RoI at which flow is computed");
	

	_addParameter(centerrelative, "centerrealtive","if true center of motion is relative to current pixel, otherwise its absolute");
	_addParameter(xc,"xc","x-coordinate of center of motion");
	_addParameter(yc,"yc","y-coordinate of center of motion");
	_addParameter(zc,"zc","z-coordinate of center of motion");

}

void MotionEstimation::update()
{
	CImg<> terms,params;
	if (brightModel()->img().size == 1) // 2D
	{
		result = CImgList<>(1, roi()->getWidth(),roi()->getHeight(),tend-tbegin,
				   model()->count() + (*brightModel).count()-1);
		for(int t=tbegin; t<tend; t++)
		{ forRoiXY(*roi(), x, y)
		{
			//cout << x << " , " << y<< " , " << t << endl;
			this->computeTerms(x,y,0,t,terms);
			//cout << "terms" << endl;
			//Functions::printMatrix(terms);
			fitter()->computeParams(terms,params);
			//cout << "params" << endl;
			//Functions::printMatrix(params);
			cimg_forV(result()[0],v)
			{
				//cout << v << " p: " << params(0,v) << endl;
				//result2d().print("t");
				//cout << x-roi()->left() <<  "," << y-roi()->top()<<  "," <<t-tbegin<<  "," <<v << endl;
				result()[0](x-roi()->left(),y-roi()->top(),t-tbegin,v) = params(0,v);
				//cout << v << " d: " << result2d()(x-roi()->left(),y-roi()->top(),t-tbegin,v) << endl;
				//cout << "0" << " d: " << result2d()(0,0,0,0) << endl;
			}
		}}
		//result2d().print("moest");
		//cout << "lll: " << result2d()(0,0,0,0) << endl;
	}
	else // 3D
	{
		result = CImgList<>(tend-tbegin,roi()->getWidth(),roi()->getHeight(),roi()->getDepth(),
				  model()->count() + (*brightModel).count()-1);
		for(int t=tbegin; t<tend; t++)
		{ forRoiXYZ(*roi(), x, y, z)
		{
			this->computeTerms(x,y,z,t,terms);
			fitter()->computeParams(terms,params);
			//cout << "params" << endl;
			//Functions::printMatrix(params);
			cimg_forV(result()[0],v)
			{
				result()[t-tbegin](x-roi()->left(),y-roi()->top(),z-roi()->front(),v) = params(0,v);
			}
		}}
	}
	_outDataChanged(result);
}

void MotionEstimation::computeTerms(const int x, const int y, const int z, const int t, CImg<>& res)
{
	
	int xca,yca,zca; // absolute values of center of motion
	if (centerrelative)
	{
		xca = x+xc;
		yca = y+yc;
		zca = z+zc;
	}
	else
	{
		xca = xc;
		yca = yc;
		zca = zc;
	}
	res = CImg<>(((MotionModel*)model)->count() + (*brightModel).count(),
		       assembler()->count());
	CImg<> motionTerm(model()->count(),1);
	CImg<> brightTerm(brightModel()->count(),1);
	int i=0;
	for ((*assembler).init(x,y,z); !(*assembler).end(); (*assembler).next())
	{
// 		cout << " ,i " << i<< " , " << (*assembler).getX()<< " , " << (*assembler).getY()<< " , " <<
// 			assembler()->getZ()<< " , " << t << " , " << assembler()->getV()<< " , " << x<< " , " << y<< " , "<< z << endl ;
		// check if current point is in the image
		if ( 
		    ( // 3D
		     (brightModel()->img().size != 1) &&
		     (!brightModel()->img()[0].containsXYZV(assembler()->getX(),assembler()->getY(),assembler()->getZ(),assembler()->getV()))
		    ) ||
		    ( // 2D
		     (brightModel()->img().size == 1) &&
		     ( 
		      (assembler()->getZ() != 0) ||
		      (!brightModel()->img()[0].containsXYZV(assembler()->getX(),assembler()->getY(),t,assembler()->getV()))
		     )
		    )
		   )
		{
			continue;
		}
		model()->compute((*assembler).getX(),(*assembler).getY(),assembler()->getZ(),
					t ,assembler()->getV(),xca,yca,zca, motionTerm);
		res.draw_image(0,i,motionTerm);
		if ((*brightModel).count() > 0)
		{
			brightModel()->compute((*assembler).getX(),(*assembler).getY(),assembler()->getZ(),
				    		t ,assembler()->getV(), brightTerm);
			res.draw_image((*model).count(),i,brightTerm);
		}
		i++;
	}
	res.crop(0,0,res.width-1,i-1);
}

/* Scalarberechnung durch neue Klasse ?!
float MotionEstimation::getScalar(int x, int y, int t, const CImg<>& params)
{
	CImg<> r(model->count()+brightModel->count(),1);
	r.draw_image(0,0,model->compute(x,y,t,x,y));
	if (brightModel->count() > 0)
		r.draw_image(model->count(),0,brightModel->compute(x,y,t,x,y)); //TODO 2. x,y durch xc,yc ersetzen?
	//cout << "params"; functions::printMatrix(params);
	//cout << "r";
	//functions::printMatrix(r);
	float res = r(0,0); 
	r.crop(1,r.width-1);
	//cout << "r new";
	//functions::printMatrix(r);
	r *= params;
	return res + r(0,0);
}

void MotionEstimation::getScalars(int x, int y, int t, const cimg_library::CImg<>& params, cimg_library::CImg<>& result)
{
	CImg<> p(1,params.height+1);
	p(0,0) = 1.0;
	p.draw_image(0,1,params);
	result = this->result * p;
} //*/

/*
CImg<> MotionEstimation::getTensor(int x, int y, int t)
{
  computeTensor(x,y,t);
  return result;
}//*/

/*
// Achtung: Tensor muss vorher berechnet werden!
CImg<float> MotionEstimation::getParams()
{
	return fitter->computeParams(result);
} //*/
