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
 *  @file Pixel.hxx
 *  @brief implementation of class Pixel
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 14.09.2009
 */

#ifndef PIXEL_HXX_
#define PIXEL_HXX_

#include "Pixel.h"

template<typename T>
Pixel<T>::Pixel()
{

}

template<typename T>
void Pixel<T>::setX(const T& newX)
{
	this->x = newX;
}

template<typename T>
const T& Pixel<T>::getX() const
{
	return this->x;
}

template<typename T>
void Pixel<T>::setY(const T& newY)
{
	this->y = newY;
}

template<typename T>
const T& Pixel<T>::getY() const
{
	return this->y;
}

template<typename T>
void Pixel<T>::setZ(const T& newZ)
{
	this->z = newZ;
}

template<typename T>
const T& Pixel<T>::getZ() const
{
	return this->z;
}

template<typename T>
void Pixel<T>::setT(const T& newT)
{
	this->t = newT;
}

template<typename T>
const T& Pixel<T>::getT() const
{
	return this->t;
}

template<typename T>
void Pixel<T>::pushBackIntensity(const T& newI)
{
	this->intensity.push_back(newI);
}

template<typename T>
void Pixel<T>::insertIntensity(const int pos, const T& newI)
{
	this->intensity[pos] = newI;
}

template<typename T>
const std::vector<T>& Pixel<T>::getIntensity() const
{
	return this->intensity;
}

template<typename T>
Pixel<T>::Pixel(const Pixel<T>& rhs)
{
	this->x(rhs.getX());
	this->y(rhs.getY());
	this->z(rhs.getZ());
	this->t(rhs.getT());
	std::vector<T>* tempListOfIntensity = rhs.getIntensity();
	for (unsigned int i = 0; i != tempListOfIntensity.size() - 1; i++)
	{
		if (i <= this->intensity.size() - 1)
		{
			this->insertIntensity(i, tempListOfIntensity[i]);
		}
		else
		{
			this->pushBackIntensity(tempListOfIntensity[i]);
		}
	}
}

template<typename T>
bool Pixel<T>::operator==(const Pixel<T> &rhs) const
{
	bool ret = false;
	if (this->x == rhs.getX())
	{
		if (this->y == rhs.getY())
		{
			if (this->z == rhs.getZ())
			{
				if (this->t == rhs.getT())
				{

					if (this->intensity.size() == rhs.intensity.size())
					{
						for (unsigned int i = 0; i != this->intensity.size(); i++)
						{
							if (this->intensity[i] == rhs->intensity[i])
							{
								ret = true;
							}
							else
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return ret;
}

template<typename T>
Pixel<T>& Pixel<T>::operator=(const Pixel<T>& rhs)
{
	if (&rhs == this)
	{
		return *this;
	}
	this->x = rhs.getX();
	this->y = rhs.getY();
	this->z = rhs.getZ();
	this->t = rhs.getT();
	std::vector<T> tempListOfIntensity = rhs.getIntensity();
	for (unsigned int i = 0; i != tempListOfIntensity.size() - 1; i++)
	{
		if (i <= this->intensity.size() - 1)
		{
			this->insertIntensity(i, tempListOfIntensity[i]);
		}
		else
		{
			this->pushBackIntensity(tempListOfIntensity[i]);
		}
	}
	return *this;
}

#endif /* PIXEL_HXX_ */
