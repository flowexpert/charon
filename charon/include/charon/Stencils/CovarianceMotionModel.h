/*  Copyright (C) 2013 Gerald Mwangi

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file CovarianceMotionModel.h
 *  Declaration of the parameter class CovarianceMotionModel.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 08.03.2013
 */

#ifndef _COVARIANCEMOTIONMODEL_H_
#define _COVARIANCEMOTIONMODEL_H_

#ifdef _MSC_VER
#ifdef covariancemotionmodel_EXPORTS
/// Visual C++ specific code
#define covariancemotionmodel_DECLDIR __declspec(dllexport)
#else
#define covariancemotionmodel_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define covariancemotionmodel_DECLDIR
#endif
#include "../Stencil.h"
#include <charon-utils/CImg.h>

/// Likelyhood for a motion field based on correlation of 2 images
/** Likelyhood for a motion field based on correlation of 2 images
 */
using namespace std;
using namespace cimg_library;
template <typename T>
class covariancemotionmodel_DECLDIR CovarianceMotionModel :
        public Stencil::Mask<T> {
public:
	/// default constructor
	/// \param name          Instance name
	CovarianceMotionModel(const std::string& name = "");

	/// images
	InputSlot< cimg_library::CImgList<T> > images;
    OutputSlot< cimg_library::CImgList<T> > rhs_out;
    /// images
    InputSlot< cimg_library::CImgList<T> > flow;
    /// ParameterList containing all unknowns of the Stencil.
    ParameterList<std::string> pUnknowns;

    Parameter< double > Scale_difference;
    virtual void updateStencil(
        const std::string& unknown,
        const Point4D<int>& p=Point4D<int>(), const int& v=0);

    virtual cimg_library::CImg<T> apply(
            const cimg_library::CImgList<T>& seq,
            const unsigned int frame) const;
    unsigned int count;
    Parameter<int> update_on_nth_iter;
    Parameter<int> windowsize;
    Parameter<bool> local;



protected:
	/// Update object.
	virtual void execute();

    void warpIm2(CImg<T> &img2);
    void computeCovariances();
    void computeLocalCovariances();
    void computeLocalCovariancesHist();
    CImg<T> computeGaussianWindow(int size, float sd_in);
    void computeLocalHistogram(CImg<T> im1w,CImg<T> im2w,CImg<T>& window, CImg<T>& hist_xy,CImg<T>& hist_deriv_xy,CImg<float>& hist,CImg<float>& hist_deriv);
    void computeLocalGaussianProb(CImg<T> im1w,CImg<T> im2w,CImg<T>& window, CImg<T>& hist_xy,CImg<T>& hist_deriv_xy,CImg<float>& hist,CImg<float>& hist_deriv);
    void getImagePatches(int x,int y, CImg<T>& window, CImg<T>& im1,CImg<T>& im2_warp,CImg<T> &im2window_warp,CImgList<T>& grad_im2_warp,CImgList<T>& grad_im2window_warp);
    T estimateOptimalSmoothing(CImg<T> &img1,CImg<T> &img2,T mean1,T mean2, T var1, T noise);
    T getLocalMeanHist(const CImg<T>& window,const CImg<T> im);

    cimg_library::CImg<T> img1ZeroMean,img2ZeroMean;
    cimg_library::CImgList<T> img2grad;
    cimg_library::CImg<T> flowx,flowy;
    CImg<T> rhsx;
    CImg<T> rhsy;


};

#endif // _COVARIANCEMOTIONMODEL_H_
