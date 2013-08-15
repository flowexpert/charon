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
            Scale_difference, "Scale_difference", "Scale difference between the input images. If this parameter is 0, the scale difference will be estimated",1.);


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
    if(Scale_difference>0)
        img2.blur(Scale_difference);

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


     //noise=1;

     T cond1on2=0;
     if(Scale_difference<=0)
     {
         cond1on2=estimateOptimalSmoothing(img1,img2,mean1,mean2,var1,noise);
         img2ZeroMean.assign(img2);
         img2ZeroMean=img2ZeroMean-mean2;
         cov12=((img1-mean1).get_mul((img2-mean2))).sum();
         var2=((img2-mean2).get_mul((img2-mean2))).sum();
     }

     var2=var2/(size-1);
     cov12=cov12/(size-1);
     T fd=cov12/var2;

     if(Scale_difference>0)
        cond1on2=(var1-noise-fd*cov12);
     //T factor=noise/((noise+cond1on2)*(noise+cond1on2))*(this->lambda());//...<-stepsize!!(updateRate in simpleiterator);
     T factor=1.0f/this->lambda()*((2.0f*cond1on2+noise)*pow((cond1on2/this->lambda()+noise),2.0f)-cond1on2*(cond1on2+noise)*2.0f/this->lambda()*(cond1on2/this->lambda()+noise));
     factor=factor/pow((cond1on2/this->lambda()+noise),4.0f);
     //T factor=-1/(cond1on2+noise);
     rhsx=(img1ZeroMean-img2ZeroMean*fd).get_mul((img2ZeroMean.get_gradient("x")[0])*factor*fd);
     rhsy=(img1ZeroMean-img2ZeroMean*fd).get_mul((img2ZeroMean.get_gradient("y")[0])*factor*fd);

     //rhsx=img1ZeroMean.get_mul(img2ZeroMean.get_gradient("x")[0])*(-this->lambda);
     //rhsy=img1ZeroMean.get_mul(img2ZeroMean.get_gradient("y")[0])*(-this->lambda);
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
T CovarianceMotionModel<T>::estimateOptimalSmoothing(CImg<T> &img1,CImg<T> &img2, T mean1, T mean2, T var1,T noise)
{
    T cond1on2=1e6;
    T res_sd=0;
    CImg<T> im2_sm;
    int size=img2.width()*img2.height();
  //  #pragma omp parallel num_threads (4)
    for(int i=3;i<sqrt(size)/2;i++)
    {
        double sd=(i-1)/4;
        im2_sm=img2.get_blur(sd);
        double var2=0;
        double cov12=0;
//        cimg_forXY(im2_sm,x,y)
//        {

//            var2+=(im2_sm(x,y)-mean2)*(im2_sm(x,y)-mean2);
//            cov12+=(img1(x,y)-mean1)*(im2_sm(x,y)-mean2);
//        }

        if(this->lambdaMask.connected())
        {
            var2=((im2_sm-mean2).get_mul((im2_sm-mean2))).get_mul(this->lambdaMask()[0]).sum();
            cov12=((img1-mean1).get_mul((im2_sm-mean2))).get_mul(this->lambdaMask()[0]).sum();
        }
        else
        {
            var2=((im2_sm-mean2).get_mul((im2_sm-mean2))).sum();
            cov12=((img1-mean1).get_mul((im2_sm-mean2))).sum();
        }



        var2=var2/(size-1);
        cov12=cov12/(size-1);

        //noise=1;
        T fd=cov12/var2;
        T cond1on2_tmp=(var1-noise-fd*cov12);
    //    #pragma omp critical
        if(cond1on2_tmp<cond1on2)
        {
            //sout<<"Cond1on2: "<<cond1on2_tmp<<" sd: "<<sd<<endl;
            cond1on2=cond1on2_tmp;
            res_sd=sd;
        }
    }
    sout<<"Optimal Sigma: "<<res_sd<<" in interval [0.5,"<<sqrt(size)/2.0f<<"]. Cond1on2: "<<cond1on2<<endl;
    img2.blur(res_sd);
    return cond1on2;

}




template <typename T>
void CovarianceMotionModel<T>::updateStencil(
        const std::string& unknown,
        const Point4D<int>& p, const int& v)
{
    this->_rhs=0;
    if(this->lambdaMask.connected())
    {
        if(unknown=="u")
            this->_rhs=rhsx(p.x,p.y)*this->lambdaMask()[0](p.x,p.y);
        else if(unknown=="v")
            this->_rhs=rhsy(p.x,p.y)*this->lambdaMask()[0](p.x,p.y);
    }
    else
    {
        if(unknown=="u")
            this->_rhs=rhsx(p.x,p.y);
        else if(unknown=="v")
            this->_rhs=rhsy(p.x,p.y);
    }



}
template <typename T>
cimg_library::CImg<T> CovarianceMotionModel<T>::apply(
        const cimg_library::CImgList<T>& seq,
        const unsigned int frame) const
{
	return seq[0] ;
}

#endif /* _COVARIANCEMOTIONMODEL_HXX_ */
