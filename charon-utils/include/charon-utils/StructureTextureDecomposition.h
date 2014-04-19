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
/** \file StructureTextureDecomposition.h
 *  Declaration of the parameter class StructureTextureDecomposition.
 *  \author <a href="mailto:gerald.mwangi@gmx.de">
 *      Gerald Mwangi</a>
 *  \date 13.06.2013
 */

#ifndef _STRUCTURETEXTUREDECOMPOSITION_H_
#define _STRUCTURETEXTUREDECOMPOSITION_H_

#ifdef _MSC_VER
#ifdef structuretexturedecomposition_EXPORTS
/// Visual C++ specific code
#define structuretexturedecomposition_DECLDIR __declspec(dllexport)
#else
#define structuretexturedecomposition_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define structuretexturedecomposition_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <vigra/multi_array.hxx>

/// Structure Texture Decomposition
/** Structure Texture Decomposition based on Deqing Suns implementation of
 *  Wedel et al: An Improved Algorithm for TV-L1 optical flow. By default the
 *  structure image u is estimated with the data term (u-data_image)^2. If
 *  conditional_image is given, the data term is a log conditional gaussian
 *  log(P(u|data_image,conditional_image)) in which loosely speaking, the
 *  input data_image and the structure image u are correlated to
 *  conditional_image. Thus the estimated structure image u contains both
 *  structure from data_image and conditional_image, but with intensity range
 *  from data_image. This is useful if one needs to estimate the structure
 *  image u of one modality (data_image) given the image of another modality
 *  (conditional_image). See Zhang, Y. :Spatial Resolution Enhancement for
 *  Hyperspectral Image Based on Wavelet Bayesian Fusion
 */
template <typename T>
class structuretexturedecomposition_DECLDIR StructureTextureDecomposition :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	StructureTextureDecomposition(const std::string& name = "");

	/// data image
	InputSlot< vigra::MultiArray<5,T> > data_image;
	/// optional conditional image
	InputSlot< vigra::MultiArray<5,T> > conditional_image;

	/// Estimated structure image
	OutputSlot< vigra::MultiArray<5,T> > structure_image;

	/// regularizer weight
	Parameter< double > theta;
	/// correlation data term weight
	Parameter< double > lambda;
	/// primal variable  iterations
	Parameter< double > primal_iters;
	/// dual variable iterations
	Parameter< double > dual_iters;

    Parameter< double > scale;


protected:
	/// Update object.
	virtual void execute();
    void computeDualVarDiv();
    void computeDualVarDiv2();
    vigra::MultiArray<2,T> computeFuncDeriv();
    T computeCovariance(vigra::MultiArray<2 ,T> left, vigra::MultiArray<2 ,T> right,T* leftmean=0,T* rightmean=0);
    double computeNoiseVariance(vigra::MultiArray<2 ,T> in);
private:
   vigra::MultiArray<2, T > mdata_image2D;
   vigra::MultiArray<2, T > mconditional_image2D;
   vigra::MultiArray<2, T > mresult_2D;
   vigra::MultiArray<2, T > mdualvar_x,mdualvar_y;
   vigra::MultiArray<2,T> mIiter;
   vigra::MultiArray<2,T> mdiv_dualvar;
   vigra::MultiArray<2, T > Ix,Iy;
   vigra::MultiArray<2, T > locIiter;
   vigra::MultiArray<2,T> divergence;
   vigra::MultiArray<2, T > reprojection;
};

#endif // _STRUCTURETEXTUREDECOMPOSITION_H_
