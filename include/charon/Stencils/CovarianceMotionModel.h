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
    /// ParameterList containing all unknowns of the Stencil.
    ParameterList<std::string> pUnknowns;

    Parameter< double > Scale_difference;
    virtual void updateStencil(
        const std::string& unknown,
        const Point4D<int>& p=Point4D<int>(), const int& v=0);

    virtual cimg_library::CImg<T> apply(
            const cimg_library::CImgList<T>& seq,
            const unsigned int frame) const;


protected:
	/// Update object.
	virtual void execute();


    void computeCovariances();
    T estimateOptimalSmoothing(CImg<T> &img1,CImg<T> &img2,T mean1,T mean2, T var1, T noise);
    CImg<T> rhsx;
    CImg<T> rhsy;


};

#endif // _COVARIANCEMOTIONMODEL_H_
