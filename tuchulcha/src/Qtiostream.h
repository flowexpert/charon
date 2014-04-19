/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file   Qtiostream.h
 *  \brief  Useful Qt I/O functions
 *
 *  \date   23.09.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef QTIOSTREAM_H_
#define QTIOSTREAM_H_

#include <QPointF>
#include <QRectF>
#include <iostream>

/// Output of QPointF to std::ostream
/** \param os       reference to current stream
 *  \param point    reference to point to output
 *  \returns        modified os
 */
std::ostream& operator<<(std::ostream& os, const QPointF& point);

/// Output of QPointF to std::ostream
/** \param os       reference to current stream
 *  \param rect     reference to rect to output
 *  \returns        modified os
 */
std::ostream& operator<<(std::ostream& os, const QRectF& rect);

/// Read a QPointF from a std::istream
/** \param is       reference to input stream
 *  \param point    reference to output point
 *  \returns        modified os
 */
std::istream& operator>>(std::istream& is, QPointF& point);

/// Read a QRectF from a std::istream
/** \param is       reference to input stream
 *  \param rect     reference to output rect
 *  \returns        modified os
 */
std::istream& operator>>(std::istream& is, QRectF& rect);
#endif /*QTIOSTREAM_H_*/
