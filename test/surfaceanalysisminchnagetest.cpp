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
 *  @file surfaceanalysisminchnageetest.cpp
 *  @brief implementation surfaceanalysisminchnageetest test
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 26.09.2009
 */

#include <SurfaceAnalysisMinChange.hxx>
#include <IncrementorParameter.hxx>
#include <iostream>
#include <vector>
#include <stdexcept>

int main()
{
	try
	{
		// initialisation
		SurfaceAnalysisMinChange<float> sa("sa");
		sa.execute();
		std::vector<float> changes;
		changes.push_back(1.0);
		changes.push_back(5.36);
		changes.push_back(6.3);
		changes.push_back(0.53);
		std::vector<IncrementorParameter<float>*> params1;
		std::vector<IncrementorParameter<float>*> params2;
		std::vector<IncrementorParameter<float>*> params3;
		std::vector<IncrementorParameter<float>*> params4;

		/*std::vector<T>& differences, std::vector<std::vector<
		 IncrementorParameter<T>*> >& parameters)*/

		std::vector<std::vector<IncrementorParameter<float>*> > insertparams;
		IncrementorParameter<float>* x1 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* y1 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* x2 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* y2 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* x3 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* y3 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* x4 = new IncrementorParameter<float> ;
		IncrementorParameter<float>* y4 = new IncrementorParameter<float> ;
		x1->current = 5.0;
		y1->current = 3.5;
		x2->current = 4.0;
		y2->current = 6.2;
		x3->current = 4.3;
		y3->current = 1.4;
		x4->current = 0.0;
		y4->current = 100.3;
		params1.push_back(x1);
		params1.push_back(y1);
		params2.push_back(x2);
		params2.push_back(y2);
		params3.push_back(x3);
		params3.push_back(y3);
		params4.push_back(x4);
		params4.push_back(y4);
		insertparams.push_back(params1);
		insertparams.push_back(params2);
		insertparams.push_back(params3);
		insertparams.push_back(params4);

		// execution of min change search
		std::cout << "changes.size()" << changes.size() << std::endl;
		for (unsigned int j = 0; j != changes.size(); j++)
		{
			std::cout << "changes[" << j << "]: " << changes[j] << std::endl;
		}
		std::cout << "insertparams" << insertparams.size() << std::endl;
		for (unsigned int k = 0; k != insertparams.size(); k++)
		{
			for (unsigned int l = 0; l != insertparams[k].size(); l++)
			{
				std::cout << "insertparams[" << k << "][" << l << "]: "
						<< insertparams[k][l]->current << std::endl;
			}
		}
		std::vector<IncrementorParameter<float>*> erg;
		erg = sa.findMinChange(changes, insertparams);
		for (unsigned int i = 0; i != erg.size(); i++)
		{
			std::cout << "erg[" << i << "]: " << erg[i]->current << std::endl;
			if (erg[i]->current != params4[i]->current)
			{
				std::cout << "Error: erg[" << i << "] do not match: "
						<< params4[i]->current << " != " << erg[i]->current
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
	std::cout << "SurfaceAnalysisMinChange works correct. " << std::endl;
	return 0;

}
