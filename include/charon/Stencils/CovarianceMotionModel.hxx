/*  Copyright (C) 2013 Gerald Mwangi

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
/** \file CovarianceMotionModel.hxx
 *  Implementation of the parameter class CovarianceMotionModel.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 08.03.2013
 */

#ifndef _COVARIANCEMOTIONMODEL_HXX_
#define _COVARIANCEMOTIONMODEL_HXX_

//#include <charon-core/ParameteredObject.hxx>
#include "../Stencil.hxx"
#include "CovarianceMotionModel.h"

template <typename T>
CovarianceMotionModel<T>::CovarianceMotionModel(const std::string& name) :
        Stencil::Base<T>(
			"CovarianceMotionModel", name,
			"<h2>Likelyhood for a motion field based on correlation of 2 "
			"images</h2><br>"
			"Likelyhood for a motion field based on correlation of 2 images"
        )
{
	ParameteredObject::_addInputSlot(
		images, "images",
		"images",
		"CImgList<T>");
    ParameteredObject::_addParameter(
            pUnknowns, "unknowns", "List of unknowns");
    ParameteredObject::_addParameter(
            blurfactorImg2, "blurfactorImg2", "Blurfactor for the high-resolution image Img2.",1.);


}

template <typename T>
void CovarianceMotionModel<T>::execute() {
    Stencil::Base<T>::execute();
    computeCovariances();
}
template <typename T>
void CovarianceMotionModel<T>::computeCovariances()
{
    cimg_library::CImg<T> gradImg2X,gradImg2Y;
    cimg_library::CImg<T> img1ZeroMean,img2ZeroMean;
    double cov12=0;
    double var2=0;
    double var1=0;
    double mean1=0;
    double mean2=0;
    CImg<T> img1=((CImg<T>&)images()[0]).get_channel(0);//thermo
    CImg<T> img2=((CImg<T>&)images()[0]).get_channel(1);//gray
    img2.blur(blurfactorImg2);

    assert(img1.width()==img2.width()&&img1.height()==img2.height());
        //raise("Dimension mismatch: The input images must have the same dimension!");



    cimg_forXY(img1,x,y)
    {
        mean1+=img1(x,y);
        mean2+=img2(x,y);
    }
    int size=img1.width()*img1.height();
    mean1=mean1/size;
    mean2=mean2/size;
    img1ZeroMean.assign(img1);
    img2ZeroMean.assign(img2);
     cimg_forXY(img1,x,y)
     {
         img1ZeroMean(x,y)-=mean1;
         img2ZeroMean(x,y)-=mean2;
         var1+=(img1(x,y)-mean1)*(img1(x,y)-mean1);
         var2+=(img2(x,y)-mean2)*(img2(x,y)-mean2);
         cov12+=(img1(x,y)-mean1)*(img2(x,y)-mean2);
     }

     T noise=img1ZeroMean.get_abs().median()/0.6745;

     var1=var1/(size-1);
     var2=var2/(size-1);
     cov12=cov12/(size-1);

     //noise=1;
     T fd=cov12/var2;
     T cond1on2=(var1-noise-fd*cov12);

     T factor=noise/((noise+cond1on2)*(noise+cond1on2))*(this->lambda());//...<-stepsize!!(updateRate in simpleiterator);

     //T factor=-1/(cond1on2+noise);
     rhsx=(img1ZeroMean-img2ZeroMean*fd).get_mul((img2ZeroMean.get_gradient("x")[0])*factor*fd);
     rhsy=(img1ZeroMean-img2ZeroMean*fd).get_mul((img2ZeroMean.get_gradient("y")[0])*factor*fd);

     std::cout<<"cond1on2 "<<cond1on2<<" noise "<<noise<<std::endl;

     std::cout<<"rhsx mean "<<rhsx.mean()<<std::endl;
     std::cout<<"rhsy mean "<<rhsy.mean()<<std::endl;
     std::vector<std::string>::iterator puIt;
     for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
         this->_unknowns.insert(*puIt);
     }
     cimg_library::CImg<T>     _dataMask;    ///< common data
     cimg_library::CImg<char>  _patternMask; ///< common pattern
     Point4D<int>              _center;      ///< common center

     _patternMask.assign(3,3,1,1);
     _dataMask.assign(3,3,1,1);

     _patternMask.fill(
              0,  0,  0,
              0,  0,  0,
              0,  0,  0);
     _dataMask.fill(
              0, 0,  0,
             0,  0, 0,
              0, 0,  0);
     _center = Point4D<int>(1,1,0,0);

     std::set<std::string>::iterator uIt;
     for(uIt=this->_unknowns.begin();uIt!=this->_unknowns.end();uIt++) {

         SubStencil<T> entry(1,1,1,1,_center);
         entry.pattern.assign(_patternMask);
         entry.data.assign(_dataMask);
         this->_subStencils[*uIt] = entry;
     }



}




template <typename T>
void CovarianceMotionModel<T>::updateStencil(
        const std::string& unknown,
        const Point4D<int>& p, const int& v)
{

    if(unknown=="u")
        this->_rhs=rhsx(p.x,p.y);
    else if(unknown=="v")
        this->_rhs=rhsy(p.x,p.y);


}
template <typename T>
cimg_library::CImg<T> CovarianceMotionModel<T>::apply(
        const cimg_library::CImgList<T>& seq,
        const unsigned int frame) const
{

}

#endif /* _COVARIANCEMOTIONMODEL_HXX_ */
