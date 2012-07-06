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

/*  Declaration of the QuiverPlot.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 23.04.2011
 */

#ifndef QUIVERPLOT_H_
#define QUIVERPLOT_H_

#include <mgl2/window.h>
#include <mgl2/qt.h>
#include <mgl2/data.h>

#include <QtGui>

/// class QuiverDraw
/** wraps mglDraw, fixing alpha effects */
class QuiverDraw : public mglDraw
{
public:
	/// draw quiver graph
	int Draw( mglGraph *gr );
	/// \name data
	//  \{
	mglData a, b;
	//  \}
};

/// class QuiverPlot
class QuiverPlot : public QMathGL
{
	Q_OBJECT
public:
	/// constructor
	QuiverPlot( QWidget *parent=0 );
	~QuiverPlot();

	/// update plot
	void updatePlot( bool visual );

	/// refresh plot
	virtual void refresh();

	/// convert Graph to Pixmap
	void _mglConvertFromGraphAlpha(QPixmap &pic, mglCanvas *gr, uchar **buf);

	/// get pixmap
	QPixmap getPixmap();

	/// quiver draw
	QuiverDraw draw;

private:
	uchar *grBufAlpha;
};

#endif

