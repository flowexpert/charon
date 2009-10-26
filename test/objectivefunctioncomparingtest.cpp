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
 *  @file objectivefunctioncomparingtest.cpp
 *  @brief implementation objectivefunctioncomparingtest test
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 23.09.2009
 */

#include <ObjectiveFunctions/ObjectiveFunctionComparing.hxx>
#include <PixelSelections/ListedPixelSelection.hxx>
#include <IncrementorParameter.hxx>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>
#include <charon-utils/InterpolatorLinear.h>
#include <Pixel.hxx>
#include <BrightnessModels/Constant.hxx>
#include <FlowFunctor.cpp>
#include <MotionModels/LocalConstant.hxx>
#include <charon-utils/FileReader.h>
#include <charon-utils/LinearFilter.h>
#include <charon-utils/Mask1D.h>
#include <vector>

int main()
{
	try
	{
		// initialisation
		IncrementorParameter<float> incpar1("incpar1");
		incpar1.current=0.0;
		IncrementorParameter<float> incpar2("incpar2");
		incpar2.current=0.0;
		IncrementorParameter<float> incpar3("incpar3");
		incpar3.current=0.0;
		IncrementorParameter<float> incpar4("incpar4");
		incpar4.current=0.0;
		std::vector<IncrementorParameter<float>*> params;
		params.push_back(&incpar1);
		params.push_back(&incpar2);
		params.push_back(&incpar3);
		params.push_back(&incpar4);
		ListedPixelSelection<float> lps("lps");
		Roi<int> roi("roi");
		roi.xEnd = 2;
		roi.yEnd = 2;
		FileReader<float> rd("rd");
		rd.filename	= TESTFILE;
		lps.range.connect(&roi.out);
		lps.sequence.connect(&rd.out);
		lps.execute();
		if (!lps.sequence.connected(&rd.out))
		{
			std::cout << "PixelSelection: sequence is not connected. " << std::endl;
			return -1; 
		}
		if (!lps.range.connected(&roi.out))
		{
			std::cout << "PixelSelection: range is not connected. " << std::endl;
			return -1; 
		}
		BrightnessModels::Constant<float> con;
		MotionModels::LocalConstant<float> locon;
		InterpolatorLinear<float> ipol("ipol");
		ObjectiveFunctionComparing<float> ofc("ofc");
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
		ofc.sequence.connect(&rd.out);
		ofc.brightnessModel.connect(&con.out);
		ofc.motionModel.connect(&locon.out);
		ofc.interpolator.connect(&ipol.out);
		con.img.connect(&rd.out);
		locon.dx.connect(&derivX.out);
		locon.dy.connect(&derivY.out);
		locon.dz.connect(&derivX.out);
		locon.dt.connect(&derivY.out);
		if (!ofc.sequence.connected(&rd.out))
		{
			std::cout << "ObjectiveFunction: Sequence is not connected. "<< std::endl;
			return -1; 
		}
		if (!ofc.brightnessModel.connected(&con.out))
		{
			std::cout << "ObjectiveFunction: bightnessModel is not connected. "<< std::endl;
			return -1; 
		}
		if (!ofc.motionModel.connected(&locon.out))
		{
			std::cout << "ObjectiveFunction: motionModel is not connected. "<< std::endl;
			return -1; 
		}
		if (!ofc.interpolator.connected(&ipol.out))
		{
			std::cout << "ObjectiveFunction: interpolator is not connected. "<< std::endl;
			return -1; 
		}
		if (!con.img.connected(&rd.out))
		{
			std::cout << "BrightnessModel::Constant: img is not connected. " << std::endl; 
			return -1; 
		}
		if (!locon.dx.connected(&derivX.out))
		{
			std::cout << "MotionModel::LocalConstant: dx is not connected. " << std::endl; 
			return -1;
		}
		if (!locon.dy.connected(&derivY.out))
		{
			std::cout << "MotionModel::LocalConstant: dy is not connected. " << std::endl; 
			return -1;
		}
		if (!locon.dz.connected(&derivX.out))
		{
			std::cout << "MotionModel::LocalConstant: dz is not connected. " << std::endl; 
			return -1;
		}
		if (!locon.dt.connected(&derivY.out))
		{
			std::cout << "MotionModel::LocalConstant: dt is not connected. " << std::endl; 
			return -1;
		}

		ofc.execute();

		// run ObjectiveFunction test for correct intensitys
		std::vector<Pixel<float>*> pilist = lps.getListOfPixel();
		std::vector<float> comparerescor;
		for (unsigned int i = 0; i != pilist.size(); i++)
		{
			comparerescor.push_back(ofc.compare(pilist, params));
			std::cout << comparerescor[i] << std::endl;
		}

		// run ObjectiveFunction test for incorrect intensitys
		std::vector<float> compareresincor;
		for (unsigned int j = 0; j != pilist.size(); j++)
		{
			pilist[j]->insertIntensity(0, 15.0);
		}
		for (unsigned int k = 0; k != pilist.size(); k++)
		{
			compareresincor.push_back(ofc.compare(pilist, params));
			std::cout << compareresincor[k] << std::endl;
		}

		// test is ObjectiveFunction results are correct
		for (unsigned int l = 0; l != comparerescor.size(); l++)
		{
			if (comparerescor[l] == compareresincor[l])
			{
				std::cout << "Result for correct and incorrect Parameters are "
					"equivalent" << std::endl;
				std::cout << "error: " << comparerescor[l] << " = "
						<< compareresincor[l] << std::endl;
				return -1;
			}
		}

	} catch (std::string s)
	{
		std::cout << s << std::endl;
		return -1;
	} catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	std::cout << "ObjectiveFunktionComparing works correct. " << std::endl;
	return 0;
}

