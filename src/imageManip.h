/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/** \file   imageManip.h
 *  \brief  useful methods for image manipulation
 *
 *  \date   24.09.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef IMAGEMANIP_H_
#define IMAGEMANIP_H_

#include <QImage>
#include <QList>
#include <QRectF>

/// Useful methods for image manipulation.
namespace imageManip {
	/// Remove the border of an image.
	/** \param image    image to use
	 *  \returns        image with borders removed (size decreased)
	 */
	QImage crop(const QImage& image);

	/// Find bounding rectangle for given list of rectangles
	/** \param rectList rectangle list
	 *  \returns        bounding rectangle
	 */
	inline QRectF boundingRect(const QList<QRectF>& rectList) {
		QList<QRectF>::const_iterator rectIter = rectList.begin();
		QRectF rect = *rectIter;
		for(; rectIter != rectList.end(); rectIter++) {
			rect |= *rectIter;
		}
		return rect;
	}
}

#endif /*IMAGEMANIP_H_*/
