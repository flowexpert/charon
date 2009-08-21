/*	Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.
    
    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/**	@file	Qtiostream.cpp
 *	@brief	Implementation of Qt I/O functions
 *
 *	@date	23.09.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#include "Qtiostream.h"
#include <QDebug>

std::ostream& operator<<(std::ostream& os, const QPointF& point) {
	os << point.x() << "," << point.y();
	return os;
}

std::ostream& operator<<(std::ostream& os, const QRectF& rect) {
	os << rect.topLeft() << "," << rect.bottomRight();
	return os;
}

std::istream& operator>>(std::istream& is, QPointF& point) {
	double x, y;
	char ch;
	is >> x;				// get x value
	is.get(ch);
	Q_ASSERT(ch == ',');	// check separator
	is >> y;				// get y value
	
	point = QPointF(x, y);
	return is;
}

std::istream& operator>>(std::istream& is, QRectF& rect) {
	QPointF tl, br;
	char ch;
	is >> tl;				// get top left point
	is.get(ch);
	Q_ASSERT(ch == ',');	// check separator
	is >> br;				// get bottom right point
	
	rect = QRectF(tl, br);
	return is;
}
