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
 *  @file incrementortest.cpp
 *  @brief implementation IncrementorCountUp test
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 23.09.2009
 */

#include <Incrementors/IncrementorCountUp.hxx>
#include <IncrementorParameter.hxx>
#include <vector>

int main()
{
	try
	{
		// initialisation
		IncrementorParameter<float> x("x");
		IncrementorParameter<float> y("y");
		IncrementorParameter<float> z("z");
		IncrementorParameter<float> t("t");
		IncrementorParameter<float> d("d");
		std::string namex = "x";
		x.name = namex;
		x.min = 0.f;
		x.max = 5.f;
		x.stepSize = 1.f;
		std::string namey = "y";
		y.name = namey;
		y.min = 0.f;
		y.max = 20.f;
		y.stepSize = 7.f;
		std::string namez = "z";
		z.name = namez;
		z.min = 0.f;
		z.max = 0.f;
		z.stepSize = 1.f;
		std::string namet = "t";
		t.name = namet;
		t.min = 0.f;
		t.max = 1.f;
		t.stepSize = 1.f;
		std::string named = "d";
		d.name = named;
		d.min = 3.5f;
		d.max = 5.6f;
		d.stepSize = 1.3f;
		IncrementorCountUp<float> inc("inc");
		inc.paramList.connect(&x.out);
		inc.paramList.connect(&y.out);
		inc.paramList.connect(&z.out);
		inc.paramList.connect(&t.out);
		inc.paramList.connect(&d.out);
		if (!inc.paramList.connected(&x.out)) 
		{
		std::cout << "parameter x is not connected" << std::endl; 
		return -1; 
		}
		if (!inc.paramList.connected(&y.out)) 
		{
		std::cout << "parameter y is not connected" << std::endl; 
		return -1; 
		}
		if (!inc.paramList.connected(&z.out)) 
		{
		std::cout << "parameter z is not connected" << std::endl; 
		return -1; 
		}
		if (!inc.paramList.connected(&t.out)) 
		{
		std::cout << "parameter t is not connected" << std::endl; 
		return -1; 
		}
		if (!inc.paramList.connected(&d.out)) 
		{
		std::cout << "parameter d is not connected" << std::endl; 
		return -1; 
		}
		inc.execute();

		// compute steps
		std::cout << "x: " << x.current << std::endl;
		std::cout << "y: " << y.current << std::endl;
		std::cout << "z: " << z.current << std::endl;
		std::cout << "t: " << t.current << std::endl;
		std::cout << "d: " << d.current << std::endl;
		std::cout << std::endl;
		int i = 0;
		inc.doStep();
		for (; !inc.doStep(); i++)
		{
			std::cout << "round: " << i << std::endl;
			std::cout << "x: " << x.current << std::endl;
			std::cout << "y: " << y.current << std::endl;
			std::cout << "z: " << z.current << std::endl;
			std::cout << "t: " << t.current << std::endl;
			std::cout << "d: " << d.current << std::endl;
			std::cout << std::endl;
		}
		std::vector<IncrementorParameter<float>*> ip = inc.getListOfParams();

		// check if stepend is correct
		for (unsigned int j = 0; j != ip.size(); j++)
		{
			std::cout << ip[j]->name << ": " << ip[j]->current << " !> ";
			if (ip[j]->name == "x")
			{
				std::cout << x.max << " ?" << std::endl;
				if (ip[j]->current > x.max || ip[j]->current + x.stepSize
						< x.max)
				{
					std::cout << "Error: wrong CountUp" << std::endl;
					return -1;
				}
			}
			else
			{
				if (ip[j]->getName() == "y")
				{
					std::cout << y.max << " ?" << std::endl;
					if (ip[j]->current > y.max || ip[j]->current + y.stepSize
							< y.max)
					{
						std::cout << "Error: wrong CountUp" << std::endl;
						return -1;
					}
				}
				else
				{
					if (ip[j]->getName() == "z")
					{
						std::cout << z.max << " ?" << std::endl;
						if (ip[j]->current > z.max || ip[j]->current
								+ z.stepSize < z.max)
						{
							std::cout << "Error: wrong CountUp" << std::endl;
							return -1;
						}
					}
					else
					{
						if (ip[j]->getName() == "t")
						{
							std::cout << t.max << " ?" << std::endl;
							if (ip[j]->current > t.max || ip[j]->current
									+ t.stepSize < t.max)
							{
								std::cout << "Error: wrong CountUp"
										<< std::endl;
								return -1;
							}
						}
						else
						{
							if (ip[j]->getName() == "d")
							{
								std::cout << d.max << " ?" << std::endl;
								if (ip[j]->current > d.max || ip[j]->current
										+ d.stepSize < d.max)
								{
									std::cout << "Error: wrong CountUp"
											<< std::endl;
									return -1;
								}
							}
						}
					}
				}
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

	// if the programm do not teminate until here it works correct
	std::cout << "Incrementor works correct. " << std::endl;
	return 0;
}
