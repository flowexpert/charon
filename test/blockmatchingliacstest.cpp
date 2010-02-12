/*  This file is part of Charon.

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
/**
 *  @file blockmatchingliacstest.cpp
 *  @brief implementation blockmatchingliacstest test
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 26.09.2009
 */

#include <ObjectiveFunctions/ObjectiveFunctionComparing.h>
#include <PixelSelections/ListedPixelSelection.h>
#include <Incrementors/IncrementorCountUp.h>
#include <SurfaceAnalysis/SurfaceAnalysisMinChange.h>
#include <IncrementorParameter.h>
#include <BlockMatching/BlockMatchingLIACS.h>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>
#include <charon-utils/InterpolatorLinear.h>
#include <Pixel.hxx>
#include <BrightnessModels/Constant.h>
#include <FlowFunctor.cpp>
#include <MotionModels/LocalConstant.h>
#include <charon-utils/FileReader.h>
#include <charon-utils/FileWriter.h>
#include <vector>
#include <charon-utils/LinearFilter.h>
#include <charon-utils/Mask1D.h>

#ifndef TESTDIR
/// global testing directory
#define TESTDIR ""
#error TESTDIR not defined!
#endif
// make sure that assert works
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

int main()
{
	try
	{
		// initialisation
		ListedPixelSelection<float> lps("lps");
		Roi<int> roi("roi");
		roi.xEnd = 15;
		roi.yEnd = 18;
		FileReader<float> rd("rd");
		rd.filename = TESTDIR "/test.cimg";
		lps.range.connect(&roi.out);
		if (!lps.range.connected(&roi.out))
		{
			std::cout << "PixelSelection: range is not connected. "
					<< std::endl;
			return -1;
		}
		lps.sequence.connect(&rd.out);
		if (!lps.sequence.connected(&rd.out))
		{
			std::cout << "PixelSelection: sequence is not connected. "
					<< std::endl;
			return -1;
		}
		lps.execute();
		BrightnessModels::Constant<float> con;
		MotionModels::LocalConstant<float> locon;
		InterpolatorLinear<float> ipol("ipol");
		ObjectiveFunctionComparing<float> ofc("ofc");
		ofc.sequence.connect(&rd.out);
		if (!ofc.sequence.connected(&rd.out))
		{
			std::cout << "ObjectiveFunction: sequence is not connected. "
					<< std::endl;
			return -1;
		}
		ofc.brightnessModel.connect(&con.out);
		if (!ofc.brightnessModel.connected(&con.out))
		{
			std::cout
					<< "ObjectiveFunction: brightnessModel is not connected. "
					<< std::endl;
			return -1;
		}
		ofc.motionModel.connect(&locon.out);
		if (!ofc.motionModel.connected(&locon.out))
		{
			std::cout << "ObjectiveFunction: motionModel is not connected. "
					<< std::endl;
			return -1;
		}
		ofc.interpolator.connect(&ipol.out);
		if (!ofc.interpolator.connected(&ipol.out))
		{
			std::cout << "ObjectiveFunction: interpolator is not connected. "
					<< std::endl;
			return -1;
		}
		con.img.connect(&rd.out);
		if (!con.img.connected(&rd.out))
		{
			std::cout << "BirghtnessModel: Constant: img is not connected. "
					<< std::endl;
			return -1;
		}
		// replacement for sobel filter
		LinearFilter<float> derivX("derivX"), derivY("derivY");
		Mask1D<float> dx("dx"), dy("dy"), sx("sx"), sy("sy");
		ParameterList<float> dVals("-1;0;1"), sVals("3;10;3");
		dx.values = dVals;
		dx.normalize = true;
		sy.values = sVals;
		sy.normalize = true;
		sy.dir = 1;
		derivX.in.connect(&rd.out);
		derivX.masks.connect(&dx.out);
		derivX.masks.connect(&sy.out);
		assert(derivX.in.connected(&rd.out));
		assert(derivX.masks.connected(&dx.out));
		assert(derivX.masks.connected(&sy.out));
		dy.values = dVals;
		dy.normalize = true;
		dy.dir = 1;
		sx.values = sVals;
		sx.normalize = true;
		derivY.in.connect(&rd.out);
		derivY.masks.connect(&dy.out);
		derivY.masks.connect(&sx.out);
		assert(derivY.in.connected(&rd.out));
		assert(derivY.masks.connected(&dy.out));
		assert(derivY.masks.connected(&sx.out));
		derivX.execute();
		derivY.execute();
		locon.dx.connect(&derivX.out);
		if (!locon.dx.connected(&derivX.out))
		{
			std::cout << "MotionModel: LocalConstant: dx is not connected. "
					<< std::endl;
			return -1;
		}
		locon.dy.connect(&derivY.out);
		if (!locon.dy.connected(&derivY.out))
		{
			std::cout << "MotionModel: LocalConstant: dy is not connected. "
					<< std::endl;
			return -1;
		}
		locon.dz.connect(&derivX.out);
		if (!locon.dz.connected(&derivX.out))
		{
			std::cout << "MotionModel: LocalConstant: dz is not connected. "
					<< std::endl;
			return -1;
		}
		locon.dt.connect(&derivY.out);
		if (!locon.dt.connected(&derivY.out))
		{
			std::cout << "MotionModel: LocalConstant: dt is not connected. "
					<< std::endl;
			return -1;
		}
		ofc.execute();
		IncrementorParameter<float> x("x");
		IncrementorParameter<float> y("y");
		IncrementorParameter<float> z("z");
		IncrementorParameter<float> t("t");
		IncrementorParameter<float> d("d");
		std::string namex = "x";
		x.name = namex;
		x.min = 0;
		x.max = 5;
		x.stepSize = 1;
		std::string namey = "y";
		y.name = namey;
		y.min = 0;
		y.max = 5;
		y.stepSize = 1;
		std::string namez = "z";
		z.name = namez;
		z.min = 0;
		z.max = 1;
		z.stepSize = 1;
		std::string namet = "t";
		t.name = namet;
		t.min = 0;
		t.max = 1;
		t.stepSize = 1;
		std::string named = "d";
		d.name = named;
		d.min = 0;
		d.max = 1;
		d.stepSize = 1;
		IncrementorCountUp<float> inc("inc");
		inc.paramList.connect(&x.out);
		if (!inc.paramList.connected(&x.out))
		{
			std::cout << "Incremenor: x is not connected. " << std::endl;
			return -1;
		}
		inc.paramList.connect(&y.out);
		if (!inc.paramList.connected(&y.out))
		{
			std::cout << "Incremenor: y is not connected. " << std::endl;
			return -1;
		}
		inc.paramList.connect(&z.out);
		if (!inc.paramList.connected(&z.out))
		{
			std::cout << "Incremenor: z is not connected. " << std::endl;
			return -1;
		}
		inc.paramList.connect(&t.out);
		if (!inc.paramList.connected(&t.out))
		{
			std::cout << "Incremenor: t is not connected. " << std::endl;
			return -1;
		}
		inc.paramList.connect(&d.out);
		if (!inc.paramList.connected(&d.out))
		{
			std::cout << "Incremenor: d is not connected. " << std::endl;
			return -1;
		}
		//		inc.execute();
		SurfaceAnalysisMinChange<float> samc("samc");
		BlockMatchingLIACS<float> bm("bm");
		bm.sequence.connect(&rd.out);
		if (!bm.sequence.connected(&rd.out))
		{
			std::cout << "BlockMatching: sequence is not connected. "
					<< std::endl;
			return -1;
		}
		bm.pixelList.connect(&lps.out);
		if (!bm.sequence.connected(&rd.out))
		{
			std::cout << "BlockMatching: sequence is not connected. "
					<< std::endl;
			return -1;
		}
		bm.newParams.connect(&inc.out);
		if (!bm.newParams.connected(&inc.out))
		{
			std::cout << "BlockMatching: newParams is not connected. "
					<< std::endl;
			return -1;
		}
		bm.changes.connect(&ofc.out);
		if (!bm.changes.connected(&ofc.out))
		{
			std::cout << "BlockMatching: changes is not connected. "
					<< std::endl;
			return -1;
		}
		bm.bestParam.connect(&samc.out);
		if (!bm.bestParam.connected(&samc.out))
		{
			std::cout << "BlockMatching: bestParam is not connected. "
					<< std::endl;
			return -1;
		}
		FileWriter<float> wr("wr");
		wr.in.connect(&bm.flow);
		if (!wr.in.connected(&bm.flow))
		{
			std::cout << "FileWriter: in is not connected. " << std::endl;
			return -1;
		}

		// testing BlockMatchingLIACS
		bm.execute();
		bm.findFlow();
		wr.filename
				= "flow.cimg";
		wr.execute();
	} catch (std::string s)
	{
		std::cout << s << std::endl;
		return -1;
	} catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	std::cout << "BlockMatchinLIACS works correct. " << std::endl;
	return 0;
}
