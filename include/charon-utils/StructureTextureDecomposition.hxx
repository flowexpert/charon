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
/** \file StructureTextureDecomposition.hxx
 *  Implementation of the parameter class StructureTextureDecomposition.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 13.06.2013
 */

#ifndef _STRUCTURETEXTUREDECOMPOSITION_HXX_
#define _STRUCTURETEXTUREDECOMPOSITION_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "StructureTextureDecomposition.h"
#include <vigra/multi_math.hxx>
#include <vigra/multi_convolution.hxx>
#include <vigra/accumulator.hxx>
#include <time.h>
#include <sys/timeb.h>
using namespace vigra;

template <typename T>
StructureTextureDecomposition<T>::StructureTextureDecomposition(const std::string& name) :
		TemplatedParameteredObject<T>(
			"StructureTextureDecomposition", name,
			"<h2>Structure Texture Decomposition</h2><br>"
			"Structure Texture Decomposition based on Deqing Suns "
			"implementation of Wedel et al: An Improved Algorithm for TV-L1 "
			"optical flow. By default the structure image u is estimated "
			"with the data term (u-data_image)^2. If conditional_image is "
			"given, the data term is a log conditional gaussian "
			"log(P(u|data_image,conditional_image)) in which loosely "
			"speaking, the input data_image and the structure image u are "
			"correlated to  conditional_image. Thus the estimated structure "
			"image u contains both structure from data_image and "
			"conditional_image, but with intensity range from data_image. "
			"This is useful if one needs to estimate the structure image u "
			"of one modality (data_image) given the image of another "
			"modality (conditional_image). See Zhang, Y. :Spatial Resolution "
			"Enhancement for Hyperspectral Image Based on Wavelet Bayesian "
			"Fusion"
		),
		conditional_image(true,false)
{
	ParameteredObject::_addInputSlot(
		data_image, "data_image",
		"data image",
		"vigraArray5<T>");
	ParameteredObject::_addInputSlot(
		conditional_image, "conditional_image",
		"optional conditional image",
		"vigraArray5<T>");

	ParameteredObject::_addOutputSlot(
		structure_image, "structure_image",
		"Estimated structure image",
		"vigraArray5<T>");

	ParameteredObject::_addParameter< double >(
		theta, "theta",
		"regularizer weight",
		1, "double");
	ParameteredObject::_addParameter< double >(
		lambda, "lambda",
		"correlation data term weight",
		1, "double");
    ParameteredObject::_addParameter< double >(
        scale, "scale",
        "difference in scale between data_image and conditional_image",
        1, "double");
	ParameteredObject::_addParameter< double >(
		primal_iters, "primal_iters",
		"primal variable  iterations",
		10, "double");
	ParameteredObject::_addParameter< double >(
		dual_iters, "dual_iters",
		"dual variable iterations",
		10, "double");
}

template <typename T>
void StructureTextureDecomposition<T>::execute() {
    vigra::MultiArrayShape<2>::type new_shape;

    new_shape[0]=data_image().shape()[0];
    new_shape[1]=data_image().shape()[1];
    mdualvar_x.reshape(new_shape);
    mdualvar_y.reshape(new_shape);
    mdualvar_x.init(0);
    mdualvar_y.init(0);
    mdata_image2D=MultiArray<2,T>(new_shape,data_image().data());
    mIiter=mdata_image2D;


    divergence.reshape(new_shape);
    reprojection.reshape(new_shape);
    Ix.reshape(new_shape);
    Iy.reshape(new_shape);
    locIiter.reshape(new_shape);
    if(!conditional_image.connected())
        computeDualVarDiv();
    else
    {
        computeDualVarDiv2();
//        using namespace vigra::multi_math;
//        double sc=(scale()-1)/4;
//        Kernel1D<T> smooth;
//        smooth.initGaussian(sc);
//        smooth.setBorderTreatment(BORDER_TREATMENT_REPEAT);

//        std::vector<Kernel1D<T> > smooth2d;
//        smooth2d.push_back(smooth);
//        smooth2d.push_back(smooth);

//        Kernel1D<T> smoothsq;
//        smoothsq.initGaussian(sc*std::sqrt(2));
//        smoothsq.setBorderTreatment(BORDER_TREATMENT_REPEAT);

//        std::vector<Kernel1D<T> > smooth2dsq;
//        smooth2dsq.push_back(smoothsq);
//        smooth2dsq.push_back(smoothsq);

//        MultiArray<2,T> condIm2d(new_shape,conditional_image().data());

//        T condImMean=0;
//        double condImCov=computeCovariance(condIm2d,condIm2d,&condImMean);
//        condIm2d=condIm2d-condImMean;
//        MultiArray<2,T> smCondIm(condIm2d.shape());
//        //gaussianSmoothMultiArray(srcMultiArrayRange(condIm2d),destMultiArray(smCondIm),sc);
//        separableConvolveMultiArray(srcMultiArrayRange(condIm2d),destMultiArray(smCondIm),smooth2d.begin());
//        //smCondIm=smCondIm-condImMean;
//        MultiArray<2,T> smIiter(mIiter.shape());

//        //gaussianSmoothMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smIiter),sc);
//        separableConvolveMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smIiter),smooth2d.begin());
//        T Iitermean=0;
//        double IsmCondImCov=computeCovariance(smIiter,condIm2d,&Iitermean);
//        smIiter=smIiter-Iitermean;
//        double IsmVar=computeCovariance(smIiter,smIiter);
//        double corrRatio=IsmCondImCov/IsmVar;
//        double noisevar=computeNoiseVariance(condIm2d);
//        double cond=condImCov-noisevar-corrRatio*IsmCondImCov;

//        double factor=noisevar/((cond+noisevar)*(cond+noisevar))*corrRatio;

//        MultiArray<2,T> smSqIiter(mIiter.shape());
//        //gaussianSmoothMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smSqIiter),sc*std::sqrt(2));
//        separableConvolveMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smSqIiter),smooth2dsq.begin());
//        smSqIiter=smSqIiter-Iitermean;

//        MultiArray<2,T> corrFuncDeriv=(smSqIiter*corrRatio-smCondIm)*factor;
//        mIiter=mdata_image2D-lambda()*theta()*corrFuncDeriv;

//        for(int ii=0;ii<primal_iters();ii++)
//        {
//            sout<<"+++++Primal iteration: "<<ii<<std::endl;
//            computeDualVarDiv();
//            //gaussianSmoothMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smIiter),sc);
//            separableConvolveMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smIiter),smooth2d.begin());
//            Iitermean=0;
//            IsmCondImCov=computeCovariance(smIiter,condIm2d,&Iitermean);
//            smIiter=smIiter-Iitermean;
//            IsmVar=computeCovariance(smIiter,smIiter);
//            corrRatio=IsmCondImCov/IsmVar;
//            cond=condImCov-noisevar-corrRatio*IsmCondImCov;
//            factor=noisevar/((cond+noisevar)*(cond+noisevar))*corrRatio;
//            //gaussianSmoothMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smSqIiter),sc*std::sqrt(2));
//            separableConvolveMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smSqIiter),smooth2dsq.begin());
//            smSqIiter=smSqIiter-Iitermean;
//            corrFuncDeriv=(smSqIiter*corrRatio-smCondIm)*factor;
//            mIiter=mIiter-lambda()*theta()*corrFuncDeriv;


//        }




    }
    structure_image()=MultiArray<5,T>(data_image().shape(),mIiter.data());

}
/*
UInt64 ClockGetTime()
{
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (UInt64)ts.tv_sec * 1000000LL + (UInt64)ts.tv_nsec / 1000LL;
}
*/
template<typename T, int N>
void computeDivergence(std::vector<MultiArray<N,T>* > &vectorField,MultiArray<N,T> &divergence,double std_dev)
{
    assert(vectorField.size()==N);
    assert((*(vectorField[0])).shape()==divergence.shape());
    using namespace vigra::multi_math;
    Kernel1D<T> smooth,deriv;
    smooth.initGaussian(std_dev);
    deriv.initGaussianDerivative(std_dev,1);
    std::vector<Kernel1D<T> > kernels;
    static MultiArray<N,T> tmpDeriv(divergence.shape());

    for(int k=0;k<N;k++)
        kernels.push_back(smooth);
    for(int k=0;k<N;k++)
    {
        kernels[k]=deriv;
        separableConvolveMultiArray(srcMultiArrayRange(*(vectorField[k])),destMultiArray(tmpDeriv),kernels.begin());
        kernels[k]=smooth;
       // convolveMultiArrayOneDimension(srcMultiArrayRange(*(vectorField[k])),destMultiArray(tmpDeriv),k,deriv);
        divergence=divergence+tmpDeriv;
    }
}

template <typename T>
void StructureTextureDecomposition<T>::computeDualVarDiv()
{
    using namespace vigra::multi_math;

//    long long begin_t=0,end_t=0;
    double delta = 1.0/(4.0*theta());


    double std_dev=0.05;

    Kernel1D<T> smooth,deriv;
    smooth.initGaussian(std_dev);
    deriv.initGaussianDerivative(std_dev,1);
    std::vector<Kernel1D<T> > xderiv_k,yderiv_k;
    xderiv_k.push_back(deriv);
    xderiv_k.push_back(smooth);
    yderiv_k.push_back(smooth);
    yderiv_k.push_back(deriv);

    for(int diter=0;diter<dual_iters();diter++)
    {
        sout<<"Dual iteration: "<<diter<<std::endl;
        std::vector<MultiArray<2,T>* > vectorField;
        vectorField.push_back(&mdualvar_x);
        vectorField.push_back(&mdualvar_y);
        //begin_t=ClockGetTime();
        computeDivergence<T,2>(vectorField,divergence,std_dev);
//        end_t=ClockGetTime();
//        sout<<"computeDivergence: "<<end_t-begin_t<<std::endl;

//        begin_t=ClockGetTime();
        locIiter=mIiter+theta()*divergence;

//        end_t=ClockGetTime();
//        sout<<"locIiter=mdata_image2D+theta()*divergence;: "<<end_t-begin_t<<std::endl;

//        begin_t=ClockGetTime();
        separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Ix),xderiv_k.begin());
//        end_t=ClockGetTime();
//        sout<<"separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Ix),xderiv_k.begin());: "<<end_t-begin_t<<std::endl;
//        begin_t=ClockGetTime();
        separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Iy),yderiv_k.begin());
//        end_t=ClockGetTime();
//        sout<<"separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Iy),yderiv_k.begin());: "<<end_t-begin_t<<std::endl;

        mdualvar_x=mdualvar_x+delta*Ix;
        mdualvar_y=mdualvar_y+delta*Iy;
        reprojection=max(1.0,sqrt(pow(mdualvar_x,2)+pow(mdualvar_y,2)));
        mdualvar_x=mdualvar_x/reprojection;
        mdualvar_y=mdualvar_y/reprojection;




    }
    std::vector<MultiArray<2,T>* > vectorField;
    vectorField.push_back(&mdualvar_x);
    vectorField.push_back(&mdualvar_y);
    computeDivergence<T,2>(vectorField,divergence,std_dev);
    mIiter=mIiter+theta()*divergence;


}

template <class T>
T StructureTextureDecomposition<T>::computeCovariance(vigra::MultiArray<2 ,T> left, vigra::MultiArray<2 ,T> right,T* leftmean,T* rightmean)
{

    using namespace vigra::multi_math;

    int s=left.size();

    T lmean=left.sum<T>()/s;
    T rmean=right.sum<T>()/s;
    left=left-lmean;
    right=right-rmean;

    if(leftmean)
        *leftmean=lmean;
    if(rightmean)
        *rightmean=rmean;



    vigra::MultiArray<2,T> prod=left*right;

    return prod.sum<T>()/(s-1);

}
template <class T>
double StructureTextureDecomposition<T>::computeNoiseVariance(MultiArray<2 ,T> in)
{
//    const vigra::MultiArrayView<5, T>& i = in;
//    int numbins=100;
//    T max=0,min=0;
    using namespace vigra::acc;
    using namespace vigra::multi_math;
    AccumulatorChain<T,Select<StandardQuantiles<AutoRangeHistogram<0> > > > accum;
    HistogramOptions hist_opt;
    hist_opt=hist_opt.setBinCount(500);
    accum.setHistogramOptions(hist_opt);
    double bla=0,mean=0;
    in.meanVariance(&mean,&bla);
    in=abs(in-mean);
    extractFeatures(in.begin(),in.end(),accum);

    double median=get<StandardQuantiles<AutoRangeHistogram<0> > >(accum)[3];
//    MultiArray<1,double> hist=get<AutoRangeHistogram<0> >(accum);




//    const vigra::MultiArrayView<1,double> &hView=hist;
//    T min=0,max=0;

//    in.minmax(&min,&max);
//    double res=0;

//    for(int x=0;x<hView.size(1);x++)
//    {
//        res+=(x+min-mean)*(x+min-mean)*hView(0,x);
//    }
//    res=res/in.size();


    return median/0.6745;


}

template <class T>
MultiArray<2,T> StructureTextureDecomposition<T>::computeFuncDeriv()
{
    vigra::MultiArrayShape<2>::type new_shape;

    new_shape[0]=data_image().shape()[0];
    new_shape[1]=data_image().shape()[1];
    double sc=(scale()-1)/4;
    Kernel1D<T> smooth;
    smooth.initGaussian(sc);
    smooth.setBorderTreatment(BORDER_TREATMENT_REPEAT);

    std::vector<Kernel1D<T> > smooth2d;
    smooth2d.push_back(smooth);
    smooth2d.push_back(smooth);

    Kernel1D<T> smoothsq;
    smoothsq.initGaussian(sc*std::sqrt(2.0));
    smoothsq.setBorderTreatment(BORDER_TREATMENT_REPEAT);

    std::vector<Kernel1D<T> > smooth2dsq;
    smooth2dsq.push_back(smoothsq);
    smooth2dsq.push_back(smoothsq);
    using namespace vigra::multi_math;

    ConvolutionOptions<2> opt;

    MultiArray<2,T> condIm2d(new_shape,conditional_image().data());
    double noisevar=computeNoiseVariance(condIm2d);
    T condImMean=0;
    double condImCov=computeCovariance(condIm2d,condIm2d,&condImMean);
    MultiArray<2,T> smCondIm(condIm2d.shape());
    condIm2d=condIm2d-condImMean;
    gaussianSmoothMultiArray(srcMultiArrayRange(condIm2d),destMultiArray(smCondIm),sc);

    MultiArray<2,T> smIiter(mIiter.shape());

    gaussianSmoothMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smIiter),sc);
    T Iitermean=0;
    double IsmCondImCov=computeCovariance(smIiter,condIm2d,&Iitermean);
    double IsmVar=computeCovariance(smIiter,smIiter);
    double corrRatio=IsmCondImCov/IsmVar;

    double cond=condImCov-noisevar-corrRatio*IsmCondImCov;

    double factor=noisevar/((cond+noisevar)*(cond+noisevar))*corrRatio;

    MultiArray<2,T> smSqIiter(mIiter.shape());
    gaussianSmoothMultiArray(srcMultiArrayRange(mIiter),destMultiArray(smSqIiter),sc*std::sqrt(2.0));
    smSqIiter=smSqIiter-Iitermean;

    return factor*(corrRatio*smSqIiter-smCondIm);
}

template <class T>
void StructureTextureDecomposition<T>::computeDualVarDiv2()
{
    using namespace vigra::multi_math;

//    long long begin_t=0,end_t=0;
    double delta = 1.0/(4.0*theta());


    double std_dev=0.05;

    Kernel1D<T> smooth,deriv;
    smooth.initGaussian(std_dev);
    deriv.initGaussianDerivative(std_dev,1);
    std::vector<Kernel1D<T> > xderiv_k,yderiv_k;
    xderiv_k.push_back(deriv);
    xderiv_k.push_back(smooth);
    yderiv_k.push_back(smooth);
    yderiv_k.push_back(deriv);

    for(int diter=0;diter<dual_iters();diter++)
    {
        sout<<"Dual iteration: "<<diter<<std::endl;
        std::vector<MultiArray<2,T>* > vectorField;
        vectorField.push_back(&mdualvar_x);
        vectorField.push_back(&mdualvar_y);
        //begin_t=ClockGetTime();
        computeDivergence<T,2>(vectorField,divergence,std_dev);
//        end_t=ClockGetTime();
//        sout<<"computeDivergence: "<<end_t-begin_t<<std::endl;

//        begin_t=ClockGetTime();
        MultiArray<2,T> funcderiv=computeFuncDeriv();
        locIiter=mdata_image2D+theta()*divergence-funcderiv*theta()*lambda();

//        end_t=ClockGetTime();
//        sout<<"locIiter=mdata_image2D+theta()*divergence;: "<<end_t-begin_t<<std::endl;

//        begin_t=ClockGetTime();
        separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Ix),xderiv_k.begin());
//        end_t=ClockGetTime();
//        sout<<"separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Ix),xderiv_k.begin());: "<<end_t-begin_t<<std::endl;
//        begin_t=ClockGetTime();
        separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Iy),yderiv_k.begin());
//        end_t=ClockGetTime();
//        sout<<"separableConvolveMultiArray(srcMultiArrayRange(locIiter),destMultiArray(Iy),yderiv_k.begin());: "<<end_t-begin_t<<std::endl;

        mdualvar_x=mdualvar_x+delta*Ix;
        mdualvar_y=mdualvar_y+delta*Iy;
        reprojection=max(1.0,sqrt(pow(mdualvar_x,2)+pow(mdualvar_y,2)));
        mdualvar_x=mdualvar_x/reprojection;
        mdualvar_y=mdualvar_y/reprojection;

        std::vector<MultiArray<2,T>* > vectorField2;
        vectorField2.push_back(&mdualvar_x);
        vectorField2.push_back(&mdualvar_y);
        //begin_t=ClockGetTime();
        computeDivergence<T,2>(vectorField2,divergence,std_dev);
        mIiter=mdata_image2D+theta()*divergence-funcderiv*theta()*lambda();



    }

}

#endif /* _STRUCTURETEXTUREDECOMPOSITION_HXX_ */
