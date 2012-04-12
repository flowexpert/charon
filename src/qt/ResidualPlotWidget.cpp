/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

	This file is part of Charon.

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
/** \file ResidualPlotWidget.cpp
 *  Declaration of class ResidualPlotWidget.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 11.04.2011
 */

#include <QtGui>
#include "ResidualPlotWidget.hpp"

ResidualPlotWidget::ResidualPlotWidget(const std::string& title,QWidget* parent) : 
	QDockWidget(parent)
{
	//create tab widget with one tab for each input image
	this->setObjectName(QString("Residual Energy")) ;

	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea) ;
	
	this->setWindowTitle(QString("ResidualEnergy [") + QString::fromStdString(title.c_str()) + QString("]") ) ;
	this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable) ;

	_plot = new IncrementalPlot(this);
	this->setWidget(_plot);
}

void ResidualPlotWidget::updateResidual( int executionCount, double residualEnergy )
{
	double cnt = (double)executionCount;
	double res = (double)residualEnergy;

	_plot->appendData( cnt, res );
	_plot->replot();
}

/////////////////////////////////////////////////////////////////////

IncrementalPlot::IncrementalPlot(QWidget *parent):
    QwtPlot(parent),
    d_data(NULL),
    d_curve(NULL)
{
    setAutoReplot(false);
}

IncrementalPlot::~IncrementalPlot()
{
    delete d_data;
}

void IncrementalPlot::appendData(double x, double y)
{
    if ( d_data == NULL )
        d_data = new CurveData;

    if ( d_curve == NULL )
    {
        d_curve = new QwtPlotCurve("Residual Curve");
        d_curve->setStyle(QwtPlotCurve::Lines);
        d_curve->setPaintAttribute(QwtPlotCurve::PaintFiltered);

        const QColor &c = Qt::black;
        d_curve->setSymbol(QwtSymbol(QwtSymbol::XCross,
            QBrush(c), QPen(c), QSize(5, 5)) );

        d_curve->attach(this);
    }

    d_data->append(x, y);
    d_curve->setData(d_data->d_x.data(), d_data->d_y.data(), d_data->size());

    const bool cacheMode =
        canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);

#if QT_VERSION >= 0x040000 && defined(Q_WS_X11)
    // Even if not recommended by TrollTech, Qt::WA_PaintOutsidePaintEvent 
    // works on X11. This has an tremendous effect on the performance..

    canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif

    canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    d_curve->draw(d_curve->dataSize() - d_data->size(), d_curve->dataSize() - 1);
    canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, cacheMode);

#if QT_VERSION >= 0x040000 && defined(Q_WS_X11)
    canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, false);
#endif
}

void IncrementalPlot::removeData()
{
    delete d_curve;
    d_curve = NULL;

    delete d_data;
    d_data = NULL;

    replot();
}

