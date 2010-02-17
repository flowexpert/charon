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
 *  @file RobustnessTerm.h
 *  @brief declaration of class RobustnessTerm used by the PsiSolver
 *  @author <a href="mailto:techfreaq@web.de">Nina Hernitschek</a>
 *  @date 14.12.2009
 */

#ifndef ROBUSTNESSTERM_H_
#define ROBUSTNESSTERM_H_

#include <math.h>

template<typename T>
class RobustnessTerm
{
protected:
	double e;

public:
	/// standard constructor and destructor
	RobustnessTerm();
	~RobustnessTerm();

	/// standard set method for parameter e
	void setE(double e);

	/// standard get function for parameter e
	double getE();
	
	///calculates robustness term, Psi
	virtual double Psi(double s)=0;

	//calculates derivative of robustness term, DPsi
	virtual double DPsi(double s)=0;

};

#endif /* ROBUSTNESSTERM_H_ */
