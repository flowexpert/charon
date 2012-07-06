/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

    This file is part of charon-free.

    charon-free is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    charon-free is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with charon-free.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file QuiverPlot.cpp
 *  Definition of the class QuiverPlot.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 23.04.2012
 */

#include <ParameteredObject.h>
#include <charon-utils/QuiverPlot.h>

int QuiverDraw::Draw( mglGraph *gr )
{
	gr->SetPlotFactor(1.0);
	gr->Alpha(true);
	gr->SetAlphaDef(0);
	gr->Light(true);
	gr->SetTranspType(0);
	gr->Clf();
//	gr->Box();
	gr->Vect( a, b, "=" );
	return 0;
}

QuiverPlot::QuiverPlot( QWidget *parent )
	: QMathGL(parent)
{
	grBufAlpha = 0;
	this->setAlpha(true);
	setPalette(Qt::transparent);
}

QuiverPlot::~QuiverPlot()
{
	if (grBufAlpha) delete[] grBufAlpha;
}

void QuiverPlot::updatePlot( bool visual )
{
	this->autoResize = true;
	this->setDraw(&draw);
	this->update();
	if (visual)
		this->show();
}

void QuiverPlot::refresh()
{
        gr->Zoom(x1,y1,x2,y2);  gr->View(phi,tet);      gr->Perspective(per);
        _mglConvertFromGraphAlpha(pic, gr, &grBufAlpha);
        if(pic.size()!=size())  setSize(pic.width(), pic.height());
        repaint();
}

void QuiverPlot::_mglConvertFromGraphAlpha(QPixmap &pic, mglCanvas *gr, uchar **buf)
{
        const uchar *bb = gr->GetBits();
        register long i,w=gr->GetWidth(), h=gr->GetHeight();
        if(*buf)        delete [](*buf);
        *buf = new uchar[4*w*h];
        for(i=0;i<w*h;i++)
        {
                (*buf)[4*i]   = bb[3*i+2];
                (*buf)[4*i+1] = bb[3*i+1];
                (*buf)[4*i+2] = bb[3*i];
                (*buf)[4*i+3] = 255;
        }
        QImage img(*buf, w, h, QImage::Format_ARGB32);

        int imgWidth = img.width();
        int imgHeight = img.height();
        QRgb old_color, new_color;
        new_color = qRgba(0,0,0,0);
        for (int w=0; w<imgWidth; ++w)
        for (int h=0; h<imgHeight; ++h)
        {
                old_color = img.pixel(w,h);
//                new_color = qRgba(255,0,0, 255 - qRed(old_color));  //  red
                new_color = qRgba(0,255,0, 255 - qGreen(old_color)); //  green
//                new_color = qRgba(0,0,255, 255 - qBlue(old_color)); //  blue
                img.setPixel(w, h, new_color);
        }
        pic = QPixmap::fromImage(img);
}

QPixmap QuiverPlot::getPixmap()
{
	return pic;
}

