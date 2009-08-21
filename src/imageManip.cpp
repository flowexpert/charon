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
/**	@file	imageManip.cpp
 *	@brief	implementation of image manipulation methods
 *
 *	@date	24.09.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "imageManip.h"
 
QImage imageManip::crop(const QImage& image) {
	int h = image.height();
	int w = image.width();
	
	QRgb tl = image.pixel(0, 0);

	int top = -1;
	for(int y = 0; top < 0 && y < h; y++)
		for(int x = 0; x < w; x++) {
			if (image.pixel(x, y) != tl) {
				top = y;
				break;
			}
		}
			
	int bottom = -1;
	for(int y = h - 1; bottom < 0 && y >= 0; y--)
		for(int x = w - 1; x >= 0; x--) {
			if (image.pixel(x, y) != tl) {
				bottom = y;
				break;
			}
		}
		
	int left = -1;
	for(int x = 0; left < 0 && x < w; x++)
		for(int y = 0; y < h; y++) {
			if (image.pixel(x, y) != tl) {
				left = x;
				break;
			}
		}
		
	int right = -1;
	for(int x = w - 1; right < 0 && x >= 0; x--)
		for(int y = h - 1; y >= 0; y--) {
			if (image.pixel(x, y) != tl) {
				right = x;
				break;
			}
		}
	
	if(top < 0)
		top = 0;
	if(bottom < 0)
		bottom = h-1;
	if(left < 0)
		left = 0;
	if(right < 0)
		right = w-1;
		
	QPoint topLeft(left, top);
	QPoint bottomRight(right, bottom);
	QRect region(topLeft, bottomRight);
	region.adjust(0, 0, -1, -1);
	
	return image.copy(region);
}
