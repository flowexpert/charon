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
 *  @file listedpixelselection_test.cpp
 *  @brief Implementation of a test for ListedPixelSelection
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 21.09.2009
 */

#include <PixelSelections/ListedPixelSelection.hxx>
#include <charon-utils/Roi.h>
#include <charon-utils/FileReader.h>
#include <Pixel.hxx>
#include <vector>
#include <stdexcept>

#ifndef TESTDIR
/// global testing directory
#define TESTDIR ""
#error TESTDIR not defined!
#endif

int main()
{
	bool test1 = false, test2 = false;
	try
	{
		/// Test one: compare Size of original Roi and the size of the ListOfPixel
		FileReader<float> rd("rd");
		Roi<int> roi("roi");
		ListedPixelSelection<float> lps("lps");
		std::vector<Pixel<float>*> list;

		roi.xBegin = 29;
		roi.xEnd = 44;
		roi.yBegin = 0;
		roi.yEnd = 18;

		rd.filename = TESTDIR "/Penguin.cimg";

		lps.range.connect(&roi.out);
		lps.sequence.connect(&rd.out);
		if (!lps.range.connected(&roi.out))
		{
			std::cout << "range is not connected. " << std::endl;
			return -1;
		}
		if (!lps.sequence.connect(&rd.out))
		{
			std::cout << "sequence is not connected. " << std::endl;
			return -1;
		}

		lps.execute();
		list = lps.getListOfPixel();

		std::cout << "test 1: test, if size of pixellist is correct. "
				<< std::endl;
		if (list.size() != 15 * 18)
		{
			std::cout << "list has wrong size (" << list.size() << " != " << 15
					* 18 << ")" << std::endl;
		}
		else
		{
			std::cout << "[50%] List has correct size. of: " << list.size()
					<< std::endl;
			test1 = true;
		}
		lps.range.disconnect(&roi.out);
		lps.sequence.disconnect(&rd.out);

		/// Test two: test the saved intensity of all pixels inside the list
		std::cout << "test 2: test, if saved pixel are correct. " << std::endl;
		FileReader<float> rdt("rdt");
		Roi<int> roit("roit");
		ListedPixelSelection<float> lpst("lpst");
		rdt.filename = TESTDIR "sequence.cimg";
		lpst.range.connect(&roit.out);
		lpst.sequence.connect(&rdt.out);
		roit.xEnd = 60;
		roit.yEnd = 60;
		lpst.range.connect(&roit.out);
		lpst.sequence.connect(&rdt.out);
		if (!lpst.range.connected(&roit.out))
		{
			std::cout << "range is not connected. " << std::endl;
			return -1;
		}
		if (!lpst.sequence.connect(&rdt.out))
		{
			std::cout << "sequence is not connected. " << std::endl;
			return -1;
		}

		lpst.execute();
		std::vector<Pixel<float>*> listt = lpst.getListOfPixel();
		Pixel<float> pix = *(listt[0]);
		std::vector<float> intens = pix.getIntensity();
		unsigned int vMax = intens.size();
		bool fehler = 0;

		for (unsigned int i = 0; i < lpst.getListOfPixel().size(); i++)
		{
			Pixel<float> temp = *(lpst.getListOfPixel()[i]);
			for (unsigned int v = 1; v <= vMax; v++)
			{
				std::vector<float> intensi;
				for (unsigned int it = 0; it < temp.getIntensity().size(); it++)
				{
					intensi.push_back(rdt.out()(it, (unsigned int) temp.getX(),
							(unsigned int) temp.getY(),
							(unsigned int) temp.getZ(),
							(unsigned int) temp.getT()));
				}
				for (unsigned int iter = 0; iter < temp.getIntensity().size(); iter++)
				{
					if (intensi[iter] != temp.getIntensity()[iter])
					{
						std::cout << "original intensity: " << rdt.out()[0](
								(unsigned int) temp.getX(),
								(unsigned int) temp.getY(),
								(unsigned int) temp.getZ(),
								(unsigned int) temp.getT()) << std::endl;
						std::cout << "saved Pixel: "
								<< temp.getIntensity()[iter] << std::endl;
						std::cout << "pixel (" << (unsigned int) temp.getX()
								<< ", " << (unsigned int) temp.getY() << ", "
								<< (unsigned int) temp.getZ() << ", "
								<< (unsigned int) temp.getT() << ") is wrong. "
								<< std::endl;
						fehler = true;
					}
				}

			}
		}
		if (fehler)
		{
			std::cout << "There where errors while pixel comparing."
					<< std::endl;
			test2 = false;
			return -1;
		}
		else
		{
			test2 = true;
			if (test1 == true)
			{
				if (test2 == true)
				{
					std::cout << "[100%]Test pixel comparing was successfull."
							<< std::endl;
				}
				else
				{
					std::cout << "[50%]Test pixel comparing was successfull."
							<< std::endl;
					return -1;
				}
			}
			if (test2 == true && test1 == false)
			{
				std::cout << "[50%]Test pixel comparing was successfull."
						<< std::endl;
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
	if (test1 == true && test2 == true)
		return 0;
}
