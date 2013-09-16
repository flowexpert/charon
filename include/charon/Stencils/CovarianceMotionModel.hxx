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
    ParameteredObject::_addInputSlot(
        flow, "flow",
        "flow",
        "CImgList<T>");
    ParameteredObject::_addOutputSlot(rhs_out,"rhs","Compute right hand side","CImgList<T>");
    ParameteredObject::_addParameter(
            pUnknowns, "unknowns", "List of unknowns");
    ParameteredObject::_addParameter(
            Scale_difference, "Scale_difference", "For global model: Scale difference between the input images. If this parameter is 0, the scale difference will be estimated",1.);

    ParameteredObject::_addParameter(
                local,"localModal","Compute covariances at every pixel in a window of size windowsize",true);
    ParameteredObject::_addParameter(
                windowsize,"windowsize","Size of local window for local model (see local parameter)",11);
    ParameteredObject::_addParameter(
                update_on_nth_iter,"update_on_nth_iter","Update covariances in stencil after n iterations",5);

    count=0;
}

template <typename T>
void CovarianceMotionModel<T>::execute() {
    Stencil::Base<T>::execute();
    //computeCovariances();
    cout<<"Update after "<<update_on_nth_iter()<<" iterations."<<endl;
    cout<<"Current iteration: "<<count<<endl;
    if(!(count%update_on_nth_iter))
    {
        if(!local)
            computeCovariances();
        else
            computeLocalCovariances();
        count=0;
    }
    count++;
    //computeLocalCovariancesHist();
}

template <typename T>
void CovarianceMotionModel<T>::warpIm2(CImg<T>& img2)
{
    CImg<T> src=((CImg<T>&)images()[0]).get_channel(1);
    img2.assign(src);
    cimg_forXY(src,x,y)
    {
        float xw=x+flow()[0].atXY(x,y);
        float yw=y+flow()[1].atXY(x,y);
        img2(x,y)=src.linear_atXY(xw,yw);
    }
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
    CImg<T> img2;//=((CImg<T>&)images()[0]).get_channel(1);//gray
    warpIm2(img2);
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
     rhs_out().assign(rhsx,rhsy,img2);

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
void CovarianceMotionModel<T>::computeLocalCovariancesHist()
{
    int windowsize=31;
    CImg<T> window=computeGaussianWindow(windowsize);
    CImg<T> img1=((CImg<T>&)images()[0]).get_channel(0);//thermo
    CImg<T> img2=((CImg<T>&)images()[0]).get_channel(1);//gray
    img1ZeroMean.assign(img1);
    img2ZeroMean.assign(img2);
//#pragma omp parallel num_threads (4)
    cimg_forXY(img1,x,y)
    {
        int x0=x-(windowsize-1)/2;
        int y0=y-(windowsize-1)/2;
        int x1=x+(windowsize-1)/2;
        int y1=y+(windowsize-1)/2;
        img1ZeroMean(x,y)=img1ZeroMean(x,y)-getLocalMeanHist(window,img1.get_crop(x0,y0,x1,y1,true));
        //img2ZeroMean(x,y)=img2ZeroMean(x,y)-getLocalMeanHist(window,img2.get_crop(x0,y0,x1,y1,true));

    }
    img2grad.clear();
    img2grad=img2ZeroMean.get_gradient("xy");

    rhsx.assign(img1,"xy",0);
    rhsy.assign(img1,"xy",0);

// num_threads (4)
    {
        int hist_size=100;
        CImg<float> hist(hist_size,hist_size,1,1,0), hist_deriv(hist_size,hist_size,1,1,0);

        int x,y;
        CImg<T> im1,  im2_warp, im2window_warp;
        CImgList<T> grad_im2_warp,  grad_im2window_warp;
        CImg<T> hist12,hist22,hist12_deriv,hist22_deriv,hist11,hist11_deriv;
#pragma omp parallel for private(y)
        for(y=0;y<rhsx.height();y++)
#pragma omp parallel for private(im1,  im2_warp, im2window_warp,grad_im2_warp,  grad_im2window_warp,hist12,hist22,hist12_deriv,hist22_deriv,x)
            for(x=0;x<rhsx.width();x++)
            {


                getImagePatches(x,y,window,im1,im2_warp,im2window_warp,grad_im2_warp,grad_im2window_warp);
                computeLocalHistogram(im1,im2_warp,window,hist12,hist12_deriv,hist,hist_deriv);
                computeLocalHistogram(im2_warp,im2_warp,window,hist22,hist22_deriv,hist,hist_deriv);
                computeLocalHistogram(im1,im1,window,hist11,hist11_deriv,hist,hist_deriv);
                T cov12=0;
                T cov22=0;
                T cov11=0;
                T integral1x=0;
                T integral1y=0;
                T integral2x=0;
                T integral2y=0;
                int xw,yw;
//#pragma omp parallel for reduction(+:cov12,cov22,integral1x,integral1y,integral2x,integral2y) private(xw) //num_threads (4)
                for(xw=0;xw<im1.width();xw++)
//#pragma omp parallel for reduction(+:cov12,cov22,integral1x,integral1y,integral2x,integral2y)
                    for(yw=0;yw<im1.height();yw++)
                    {
                        cov11+=hist11(xw,yw)*im1(xw,yw)*im1(xw,yw);
                        cov12+=hist12(xw,yw)*im1(xw,yw)*im2_warp(xw,yw);//*window(xw,yw);
                        cov22+=hist22(xw,yw)*im2_warp(xw,yw)*im2_warp(xw,yw);//*window(xw,yw);
                        integral1x+=hist12_deriv(xw,yw)*grad_im2window_warp[0](xw,yw)*im1(xw,yw)*im2_warp(xw,yw)+hist12(xw,yw)*im1(xw,yw)*grad_im2_warp[0](xw,yw);//*window(xw,yw);
                        integral1y+=hist12_deriv(xw,yw)*grad_im2window_warp[1](xw,yw)*im1(xw,yw)*im2_warp(xw,yw)+hist12(xw,yw)*im1(xw,yw)*grad_im2_warp[1](xw,yw);//*window(xw,yw);

                        integral2x+=hist22_deriv(xw,yw)*grad_im2window_warp[0](xw,yw)*im2_warp(xw,yw)*im2_warp(xw,yw)+hist22(xw,yw)*im2_warp(xw,yw)*grad_im2_warp[0](xw,yw);//*window(xw,yw);
                        integral2x+=hist22_deriv(xw,yw)*grad_im2window_warp[1](xw,yw)*im2_warp(xw,yw)*im2_warp(xw,yw)+hist22(xw,yw)*im2_warp(xw,yw)*grad_im2_warp[1](xw,yw);//*window(xw,yw);
                    }
                cov11/=(windowsize*windowsize-1);
                cov12/=(windowsize*windowsize-1);
                cov22/=(windowsize*windowsize-1);
                integral1x/=(windowsize*windowsize-1);
                integral1y/=(windowsize*windowsize-1);
                integral2x/=(windowsize*windowsize-1);
                integral2y/=(windowsize*windowsize-1);


                if(cov22==0)
                {
                    rhsx(x,y)=0;
                    rhsy(x,y)=0;
                }
                else
                {
                    T cond_mean=cov12/cov22*im2_warp((windowsize-1)/2,(windowsize-1)/2);
                    T cond_var=cov11-cov12*cov12/cov22;
                    //#pragma omp critical
                    //rhsx(x,y)=(-cov12/cov22*integral1x+cov12*cov12/(cov22*cov22)*integral2x)*this->lambda;
                    //#pragma omp critical
                    //rhsy(x,y)=(-cov12/cov22*integral1y+cov12*cov12/(cov22*cov22)*integral2y)*this->lambda;

                    rhsx(x,y)=-this->lambda*((-1/(cond_var*cond_var)*(-cov12/cov22*integral1x+cov12*cov12/(cov22*cov22)*integral2x)*(img1ZeroMean(x,y)-cond_mean)*(img1ZeroMean(x,y)-cond_mean))
                            +1/cond_var*(img1ZeroMean(x,y)-cond_mean)*(integral1x/cov22*im2_warp((windowsize-1)/2,(windowsize-1)/2)-cov12*2.0f*integral2x/(cov22*cov22)*im2_warp((windowsize-1)/2,(windowsize-1)/2)+cov12/cov22*grad_im2_warp[0]((windowsize-1)/2,(windowsize-1)/2)));
                    rhsy(x,y)=-this->lambda*((-1/(cond_var*cond_var)*(-cov12/cov22*integral1y+cov12*cov12/(cov22*cov22)*integral2y)*(img1ZeroMean(x,y)-cond_mean)*(img1ZeroMean(x,y)-cond_mean))
                            +1/cond_var*(img1ZeroMean(x,y)-cond_mean)*(integral1y/cov22*im2_warp((windowsize-1)/2,(windowsize-1)/2)-cov12*2.0f*integral2y/(cov22*cov22)*im2_warp((windowsize-1)/2,(windowsize-1)/2)+cov12/cov22*grad_im2_warp[1]((windowsize-1)/2,(windowsize-1)/2)));
                }

                // cout<<"cov12 "<<cov12<<" cov22 "<<cov22<<endl;
                if(hist12.is_nan())
                    cout<<"hist12"<<endl;
                if(hist22.is_nan())
                    cout<<"hist22"<<endl;
                if(im1.is_nan())
                    cout<<"im1"<<endl;
                if(im2_warp.is_nan())
                    cout<<"im2_warp"<<endl;
                if(std::isnan(cov22))
                    cout<<"cov22"<<endl;
                if(std::isnan(cov12))
                    cout<<"cov12"<<endl;
                if(std::isnan(integral1x))
                    cout<<"integral1x"<<endl;
                if(std::isnan(integral1y))
                    cout<<"integral1y"<<endl;
                if(std::isnan(integral2x))
                    cout<<"integral2x"<<endl;
                if(std::isnan(integral2x))
                    cout<<"integral2x"<<endl;

                if(hist12.is_inf())
                    cout<<"hist12 isinf"<<endl;
                if(hist22.is_inf())
                    cout<<"hist22 isinf"<<endl;
                if(im1.is_inf())
                    cout<<"im1 isinf"<<endl;
                if(im2_warp.is_inf())
                    cout<<"im2_warp isinf"<<endl;
                if(std::isinf(cov22))
                    cout<<"cov22 isinf"<<endl;
                if(std::isinf(cov12))
                    cout<<"cov12 isinf"<<endl;
                if(std::isinf(integral1x))
                    cout<<"integral1x isinf"<<endl;
                if(std::isinf(integral1y))
                    cout<<"integral1y isinf"<<endl;
                if(std::isinf(integral2x))
                    cout<<"integral2x isinf"<<endl;
                if(std::isinf(integral2x))
                    cout<<"integral2x isinf"<<endl;


            }
    }
    if(rhsx.is_nan())
        cout<<"rhsx nan"<<endl;
    if(rhsx.is_inf())
        cout<<"rhsx inf"<<endl;
    if(rhsy.is_nan())
        cout<<"rhsy nan"<<endl;
    if(rhsy.is_inf())
        cout<<"rhsy inf"<<endl;
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
void CovarianceMotionModel<T>::computeLocalCovariances()
{

    CImg<T> window=computeGaussianWindow(windowsize);
    CImg<T> img1=((CImg<T>&)images()[0]).get_channel(0);//thermo
    CImg<T> img2=((CImg<T>&)images()[0]).get_channel(1);//gray
    //img1=(img1-img1.min())/((img1.max()-img1.min()))*1000;
    T a=(img2.max()-img2.min())/(img1.max()-img1.min());
    //img2=(img2-img2.min())/a+img1.min();
    img1ZeroMean.assign(img1);
    img2ZeroMean.assign(img2);
    CImg<T> residualenergy(img1),cond_var_im(img1),cond_mean_im(img1),im2_warp_im(img1);
//#pragma omp parallel num_threads (4)
    cimg_forXY(img1,x,y)
    {
        int x0=x-(windowsize-1)/2;
        int y0=y-(windowsize-1)/2;
        int x1=x+(windowsize-1)/2;
        int y1=y+(windowsize-1)/2;
        img1ZeroMean(x,y)=img1ZeroMean(x,y)-img1.get_crop(x0,y0,x1,y1,true).mean();
       // img2ZeroMean(x,y)=img2ZeroMean(x,y)-img2.get_crop(x0,y0,x1,y1,true).mean();

    }
    img2grad.clear();
    img2grad=img2ZeroMean.get_gradient("xy");

    rhsx.assign(img1,"xy",0);
    rhsy.assign(img1,"xy",0);

// num_threads (4)
    {
        //int hist_size=100;
        //CImg<float> hist(hist_size,hist_size,1,1,0), hist_deriv(hist_size,hist_size,1,1,0);

        int x,y;
        CImg<T> im1,  im2_warp, im2window_warp;
        CImgList<T> grad_im2_warp,  grad_im2window_warp;
        CImg<T> hist12,hist22,hist12_deriv,hist22_deriv,hist11,hist11_deriv;
//#pragma omp parallel for private(y)
        for(y=0;y<rhsx.height();y++)
//#pragma omp parallel for private(im1,  im2_warp, im2window_warp,grad_im2_warp,  grad_im2window_warp,cond_var_im,cond_mean_im,im2_warp_im,residualenergy,x)
            for(x=0;x<rhsx.width();x++)
            {


                getImagePatches(x,y,window,im1,im2_warp,im2window_warp,grad_im2_warp,grad_im2window_warp);
                //computeLocalHistogram(im1,im2_warp,window,hist12,hist12_deriv,hist,hist_deriv);
                //computeLocalHistogram(im2_warp,im2_warp,window,hist22,hist22_deriv,hist,hist_deriv);
                //computeLocalHistogram(im1,im1,window,hist11,hist11_deriv,hist,hist_deriv);
                T cov12=0;
                T cov22=0;
                T cov11=0;
                T cov1_grad2x=0;
                T cov1_grad2y=0;
                T cov2_grad2x=0;
                T cov2_grad2y=0;
                int xw,yw;
                T a1_loc=(im2_warp.max()-im2_warp.min())/(im1.max()-im1.min());
                T b1_loc=im2_warp.min();
                T im1min=im1.min();
                T a2_loc=(im1.max()-im1.min())/(im2_warp.max()-im2_warp.min());
                T b2_loc=im1.min();
                T im2min=im2_warp.min();

                //if(1)//a1_loc>=1)
                {
                    a2_loc=1;
                    b2_loc=0;
                    im2min=0;
                }
                //else
                {
                    a1_loc=1;
                    b1_loc=0;
                    im1min=0;
                }
//#pragma omp parallel for reduction(+:cov11,cov12,cov22,cov1_grad2x,cov1_grad2y,cov2_grad2x,cov2_grad2y) private(xw) //num_threads (4)
                for(xw=0;xw<im1.width();xw++)
//#pragma omp parallel for reduction(+:cov11,cov12,cov22,cov1_grad2x,cov1_grad2y,cov2_grad2x,cov2_grad2y) private(yw)
                    for(yw=0;yw<im1.height();yw++)
                    {
                        cov11+=((im1(xw,yw)-im1min)*a1_loc+b1_loc)*((im1(xw,yw)-im1min)*a1_loc+b1_loc);//*window(xw,yw);
                        cov12+=((im1(xw,yw)-im1min)*a1_loc+b1_loc)*((im2_warp(xw,yw)-im2min)*a2_loc+b2_loc);//*window(xw,yw);
                        cov22+=((im2_warp(xw,yw)-im2min)*a2_loc+b2_loc)*((im2_warp(xw,yw)-im2min)*a2_loc+b2_loc);//*window(xw,yw);
                        cov1_grad2x+=((im1(xw,yw)-im1min)*a1_loc+b1_loc)*grad_im2_warp[0](xw,yw)*a2_loc;//*window(xw,yw);
                        cov1_grad2y+=((im1(xw,yw)-im1min)*a1_loc+b1_loc)*grad_im2_warp[1](xw,yw)*a2_loc;//*window(xw,yw);

                        cov2_grad2x+=((im2_warp(xw,yw)-im2min)*a2_loc+b2_loc)*grad_im2_warp[0](xw,yw)*a2_loc;//*window(xw,yw);
                        cov2_grad2y+=((im2_warp(xw,yw)-im2min)*a2_loc+b2_loc)*grad_im2_warp[1](xw,yw)*a2_loc;//*window(xw,yw);
                    }
                cov11/=(windowsize*windowsize-1);
                cov12/=(windowsize*windowsize-1);
                cov22/=(windowsize*windowsize-1);
                cov1_grad2x/=(windowsize*windowsize-1);
                cov1_grad2y/=(windowsize*windowsize-1);
                cov2_grad2x/=(windowsize*windowsize-1);
                cov2_grad2y/=(windowsize*windowsize-1);
                T cond_var=cov22-cov12*cov12/cov11;

                if((cond_var==0)||cov11==0||std::isnan(a1_loc)||std::isnan(a2_loc)||std::isinf(a1_loc)||std::isinf(a2_loc)
                        ||std::isnan(-a1_loc)||std::isnan(-a2_loc)||std::isinf(-a1_loc)||std::isinf(-a2_loc))
                {
                    rhsx(x,y)=0;
                    rhsy(x,y)=0;
                    residualenergy(x,y)=0;
                    cond_var_im(x,y)=0;
                    cond_mean_im(x,y)=0;


                }
                else
                {
//                    T cond_mean=cov12/cov22*im2_warp((windowsize-1)/2,(windowsize-1)/2);
//                    T cond_var=cov11-cov12*cov12/cov22;
//                    //#pragma omp critical
//                    rhsx(x,y)=(-cov12/cov22*integral1x+cov12*cov12/(cov22*cov22)*integral2x)*this->lambda;
//                    //#pragma omp critical
//                    rhsy(x,y)=(-cov12/cov22*integral1y+cov12*cov12/(cov22*cov22)*integral2y)*this->lambda;

//                    rhsx(x,y)=-this->lambda*((-1.0f/(cond_var*cond_var)*(-cov12/cov22*integral1x+cov12*cov12/(cov22*cov22)*integral2x)*(img1ZeroMean(x,y)-cond_mean)*(img1ZeroMean(x,y)-cond_mean))
//                            +1.0f/cond_var*(img1ZeroMean(x,y)-cond_mean)*(integral1x/cov22*im2_warp((windowsize-1)/2,(windowsize-1)/2)-cov12*2.0f*integral2x/(cov22*cov22)*im2_warp((windowsize-1)/2,(windowsize-1)/2)+cov12/cov22*grad_im2_warp[0]((windowsize-1)/2,(windowsize-1)/2)));
//                    rhsy(x,y)=-this->lambda*((-1.0f/(cond_var*cond_var)*(-cov12/cov22*integral1y+cov12*cov12/(cov22*cov22)*integral2y)*(img1ZeroMean(x,y)-cond_mean)*(img1ZeroMean(x,y)-cond_mean))
//                            +1.0f/cond_var*(img1ZeroMean(x,y)-cond_mean)*(integral1y/cov22*im2_warp((windowsize-1)/2,(windowsize-1)/2)-cov12*2.0f*integral2y/(cov22*cov22)*im2_warp((windowsize-1)/2,(windowsize-1)/2)+cov12/cov22*grad_im2_warp[1]((windowsize-1)/2,(windowsize-1)/2)));
//                    residualenergy(x,y)=1.0f/cond_var*(img1ZeroMean(x,y)-cond_mean)*(img1ZeroMean(x,y)-cond_mean);
                    T im1_center=((im1((windowsize-1)/2,(windowsize-1)/2)-im1min)*a1_loc+b1_loc);
                    T cond_mean=cov12/cov11*im1_center;


                    T im2w_center=((im2_warp((windowsize-1)/2,(windowsize-1)/2)-im2min)*a2_loc+b2_loc);

                    T im2gradx=grad_im2_warp[0]((windowsize-1)/2,(windowsize-1)/2);
                    T im2grady=grad_im2_warp[1]((windowsize-1)/2,(windowsize-1)/2);
                    rhsx(x,y)=-this->lambda*(-1.0f/(cond_var*cond_var)*(cov2_grad2x-cov12/cov11*cov1_grad2x)*(im2w_center-cond_mean)*(im2w_center-cond_mean)
                                             +1.0f/cond_var*(im2w_center-cond_mean)*(im2gradx-cov1_grad2x/cov11*im1_center));
                    rhsy(x,y)=-this->lambda*(-1.0f/(cond_var*cond_var)*(cov2_grad2y-cov12/cov11*cov1_grad2y)*(im2w_center-cond_mean)*(im2w_center-cond_mean)
                                             +1.0f/cond_var*(im2w_center-cond_mean)*(im2grady-cov1_grad2y/cov11*im1_center));
                    cond_var_im(x,y)=cond_var;
                    cond_mean_im(x,y)=cond_mean;

                    residualenergy(x,y)=this->lambda/2.0f*(1.0f/cond_var*(im2w_center-cond_mean)*(im2w_center-cond_mean));



                    im2_warp_im(x,y)=(im2_warp((windowsize-1)/2,(windowsize-1)/2)-im2min)*a2_loc+b2_loc;
                    if(std::isinf(rhsx(x,y))||std::isnan(rhsx(x,y))||std::isinf(rhsy(x,y))||std::isnan(rhsy(x,y))
                            ||std::isinf(-rhsx(x,y))||std::isnan(-rhsx(x,y))||std::isinf(-rhsy(x,y))||std::isnan(-rhsy(x,y)))
                    {
                        rhsx(x,y)=0;
                        rhsy(x,y)=0;
                        residualenergy(x,y)=0;
                        cond_var_im(x,y)=0;
                        cond_mean_im(x,y)=0;
                    }
                }


                // cout<<"cov12 "<<cov12<<" cov22 "<<cov22<<endl;

                if(im1.is_nan())
                    cout<<"im1"<<endl;
                if(im2_warp.is_nan())
                    cout<<"im2_warp"<<endl;
                if(std::isnan(cov22))
                    cout<<"cov22"<<endl;
                if(std::isnan(cov12))
                    cout<<"cov12"<<endl;
                if(std::isnan(cov1_grad2x))
                    cout<<"integral1x"<<endl;
                if(std::isnan(cov1_grad2y))
                    cout<<"integral1y"<<endl;
                if(std::isnan(cov2_grad2x))
                    cout<<"integral2x"<<endl;
                if(std::isnan(cov2_grad2x))
                    cout<<"integral2x"<<endl;


                if(im1.is_inf())
                    cout<<"im1 isinf"<<endl;
                if(im2_warp.is_inf())
                    cout<<"im2_warp isinf"<<endl;
                if(std::isinf(cov22))
                    cout<<"cov22 isinf"<<endl;
                if(std::isinf(cov12))
                    cout<<"cov12 isinf"<<endl;
                if(std::isinf(cov1_grad2x))
                    cout<<"integral1x isinf"<<endl;
                if(std::isinf(cov1_grad2y))
                    cout<<"integral1y isinf"<<endl;
                if(std::isinf(cov2_grad2x))
                    cout<<"integral2x isinf"<<endl;
                if(std::isinf(cov2_grad2x))
                    cout<<"integral2x isinf"<<endl;


            }
    }
    if(rhsx.is_nan())
        cout<<"rhsx nan"<<endl;
    if(rhsx.is_inf())
        cout<<"rhsx inf"<<endl;
    if(rhsy.is_nan())
        cout<<"rhsy nan"<<endl;
    if(rhsy.is_inf())
        cout<<"rhsy inf"<<endl;
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

    rhs_out().assign(rhsx,rhsy,residualenergy,cond_var_im,cond_mean_im,img1ZeroMean,im2_warp_im);


}

template <typename T>
CImg<T> CovarianceMotionModel<T>::computeGaussianWindow(int size,float sd_in=0)
{
    CImg<T> res(size,size);
    float sd=(size-1)/4;
    if(sd_in)
        sd=sd_in;
    int center=(size-1)/2;

    cimg_forXY(res,x,y)
    {
        res(x,y)=exp(-((x-center)*(x-center)+(y-center)*(y-center))/(2.0f*sd));
    }
    //res/=res.sum();
    return res;
}
template <typename T>
void CovarianceMotionModel<T>::computeLocalHistogram(CImg<T> im1w,CImg<T> im2w, CImg<T> &window, CImg<T> &hist_xy, CImg<T> &hist_deriv_xy,CImg<float>& hist,CImg<float>& hist_deriv)
{

    int width=window.width();
    int height=window.height();
    im1w.mul(window);
    im2w.mul(window);
    T max1=im1w.max();
    T min1=im1w.min();
    T max2=im2w.max();
    T min2=im2w.min();
    int hist_size=hist.width();
    float binsize1=(max1-min1)/(hist_size-1);
    float binsize2=(max2-min2)/(hist_size-1);
   // int size1=std::floor((max1-min1)/binsize)+1;

   // int size2=std::floor((max2-min2)/binsize)+1;
    //hist.fill(0.0f);
    memset(hist.data(),0,sizeof(float)*hist_size*hist_size);

    cimg_forXY(im1w,x1,y1)
    {
        int i1=floor((im1w(x1,y1)-min1)/binsize1);
        cimg_forXY(im2w,x2,y2)
        {
            int i2=floor((im2w(x2,y2)-min2)/binsize2);
            hist(i1,i2)=hist(i1,i2)+1;
        }
    }
    hist=hist/hist.sum();

    hist_deriv=hist.get_gradient()[1];

    hist_xy.assign(im1w,"xy",0);
    hist_deriv_xy.assign(im1w,"xy",0);

    cimg_forXY(hist_xy,x,y)
    {
        int i1=floor((im1w(x,y)-min1)/binsize1);
        int i2=floor((im2w(x,y)-min2)/binsize2);
        hist_xy(x,y)=hist(i1,i2);
        hist_deriv_xy(x,y)=hist_deriv(i1,i2);
    }







}

template <typename T>
void CovarianceMotionModel<T>::computeLocalGaussianProb(CImg<T> im1w, CImg<T> im2w, CImg<T> &window, CImg<T> &hist_xy, CImg<T> &hist_deriv_xy, CImg<float> &hist, CImg<float> &hist_deriv)
{


    T max1=im1w.max();
    T min1=im1w.min();
    T max2=im2w.max();
    T min2=im2w.min();
    int hist_size=hist.width();
    float binsize1=(max1-min1)/(hist_size-1);
    float binsize2=(max2-min2)/(hist_size-1);
   // int size1=std::floor((max1-min1)/binsize)+1;

   // int size2=std::floor((max2-min2)/binsize)+1;
    //hist.fill(0.0f);
    memset(hist.data(),0,sizeof(float)*hist_size*hist_size);

    cimg_forXY(im1w,x1,y1)
    {
        int i1=floor((im1w(x1,y1)-min1)/binsize1);
        cimg_forXY(im2w,x2,y2)
        {
            int i2=floor((im2w(x2,y2)-min2)/binsize2);
            hist(i1,i2)=hist(i1,i2)+1;
        }
    }
    hist.convolve(window);

    hist_deriv=hist.get_gradient()[1];

    hist_xy.assign(im1w,"xy",0);
    hist_deriv_xy.assign(im1w,"xy",0);

    cimg_forXY(hist_xy,x,y)
    {
        int i1=floor((im1w(x,y)-min1)/binsize1);
        int i2=floor((im2w(x,y)-min2)/binsize2);
        hist_xy(x,y)=hist(i1,i2);
        hist_deriv_xy(x,y)=hist_deriv(i1,i2);
    }
}

template <typename T>
void CovarianceMotionModel<T>::getImagePatches(int x, int y, CImg<T> &window, CImg<T> &im1, CImg<T> &im2_warp,CImg<T> &im2window_warp, CImgList<T> &grad_im2_warp, CImgList<T> &grad_im2window_warp)
{
    int width=window.width();
    int height=window.height();
    int x0=x-(width-1)/2;
    int y0=y-(height-1)/2;
    int x1=x+(width-1)/2;
    int y1=y+(height-1)/2;
    im1.assign(width,height);
    //im1=img1ZeroMean.get_crop(x0,y0,x1,y1,true);
    im2_warp.assign(im1);
    grad_im2_warp.assign(im1,im1);
    //grad_im2window_warp.assign(grad_im2_warp);
    cimg_forXY(im2_warp,i,j)
    {

        float xw=x0+i+flow()[0].atXY(x,y);
        float yw=y0+j+flow()[1].atXY(x,y);
        im1(i,j)=img1ZeroMean._linear_atXY(x0+i,y0+j,0,0);
        im2_warp(i,j)=img2ZeroMean._linear_atXY(xw,yw,0,0);
        grad_im2_warp[0](i,j)=img2grad[0]._linear_atXY(xw,yw,0,0);
        grad_im2_warp[1](i,j)=img2grad[1]._linear_atXY(xw,yw,0,0);
    }
    im2_warp=im2_warp-im2_warp.mean();
    grad_im2_warp[0]=grad_im2_warp[0]-grad_im2_warp[0].mean();
    grad_im2_warp[1]=grad_im2_warp[1]-grad_im2_warp[1].mean();
    im2window_warp=im2_warp.get_mul(window);
    grad_im2window_warp.assign(grad_im2_warp);
    grad_im2window_warp[0].mul(window);
    grad_im2window_warp[1].mul(window);


//    im1.mul(window);
//    im2_warp.mul(window);
//    grad_im2_warp[0].mul(window);
//    grad_im2_warp[1].mul(window);

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
T CovarianceMotionModel<T>::getLocalMeanHist(const CImg<T> &window, const CImg<T> im)
{
    CImg<T> imw=im.get_mul(window);
    T max1=imw.max();
    T min1=imw.min();
    CImg<T> hist=imw.get_histogram(std::floor(max1-min1)+1);
    hist=hist/hist.sum();
    T mean=0;
    cimg_forXY(imw,x,y)
    {
        int i=std::floor(imw(x,y)-min1);
        mean+=hist(i)*im(x,y);
    }
    return mean;

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
