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

#include <ObjectiveFunctionComparing.hxx>
#include <ListedPixelSelection.hxx>
#include <IncrementorParameter.hxx>
#include <CImg.h>
#include <Roi.h>
#include <InterpolatorLinear.hxx>
#include <Pixel.hxx>
#include <Constant.hxx>
#include <FlowFunctor.cpp>
#include <LocalConstant.hxx>
#include <FileReader.hxx>
#include <Sobel.hxx>
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
		roi.left = 0;
		roi.right = 2;
		roi.top = 0;
		roi.bottom = 2;
		roi.front = 0;
		roi.back = 1;
		roi.before = 0;
		roi.after = 1;
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
		Sobel<float> der("der");
		der.img.connect(&rd.out);
		der.execute();
		ofc.sequence.connect(&rd.out);
		ofc.brightnessModel.connect(&con.out);
		ofc.motionModel.connect(&locon.out);
		ofc.interpolator.connect(&ipol.out);
		con.img.connect(&rd.out);
		locon.dx.connect(&der.dx);
		locon.dy.connect(&der.dy);
		locon.dz.connect(&der.dx);
		locon.dt.connect(&der.dy);
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
		if (!der.img.connected(&rd.out))
		{
			std::cout << "Derivatives: img is not connected. " << std::endl; 
			return -1; 
		}
		if (!con.img.connected(&rd.out))
		{
			std::cout << "BrightnessModel::Constant: img is not connected. " << std::endl; 
			return -1; 
		}
		if (!locon.dx.connected(&der.dx))
		{
			std::cout << "MotionModel::LocalConstant: dx is not connected. " << std::endl; 
			return -1;
		}
		if (!locon.dy.connected(&der.dy))
		{
			std::cout << "MotionModel::LocalConstant: dy is not connected. " << std::endl; 
			return -1;
		}
		if (!locon.dz.connected(&der.dx))
		{
			std::cout << "MotionModel::LocalConstant: dz is not connected. " << std::endl; 
			return -1;
		}
		if (!locon.dt.connected(&der.dy))
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

