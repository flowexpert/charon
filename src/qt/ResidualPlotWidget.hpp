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
/** \file ResidualPlotWidget.hpp
 *  Declaration of class ResidualPlotWidget.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
 *  \date 11.04.2012
 */

#ifndef RESIDUALPLOTWIDGET_HPP_
#define RESIDUALPLOTWIDGET_HPP_

#include <qwt_data.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <QDockWidget>
#include "charonwidgets.h"
#include <charon-utils/ResidualPlot.h>
#include <string>

class QLabel ;
class QLineEdit ;
class QwtPlot ;

/// class representing curve data
class CurveData : public QwtData
{
public:
	virtual QwtData *copy() const
	{
		return new CurveData;
	}
	virtual size_t size() const { return d_x.size(); }
	virtual double x(size_t i) const { return d_x[i]; }
	virtual double y(size_t i) const { return d_y[i]; }

	void append(double x, double y)
	{
		d_x.push_back( x );
		d_y.push_back( y );
	}

	std::vector<double> d_x;
	std::vector<double> d_y;
};

/// class for plotting incremental plots
class IncrementalPlot : public QwtPlot
{
	Q_OBJECT
public:
	IncrementalPlot(QWidget *parent = NULL);
	virtual ~IncrementalPlot();

	void appendData(double x, double y);
	void removeData();

private:
	CurveData *d_data;
	QwtPlotCurve *d_curve;
};

/// Widget for Plotting Residual Energy
class charonwidgets_DECLDIR ResidualPlotWidget : public QDockWidget
{
	Q_OBJECT
public:
	/// default constructor
	ResidualPlotWidget(const std::string& title = "", QWidget* parent = 0) ;
	/// default destructor
	virtual ~ResidualPlotWidget() {};
	
	/// update residual
	void updateResidual( int executionCount, double residualEnergy ) ;

private:
	/// central widget
	IncrementalPlot *_plot;

} ; // class ResidualPlotWidget

#endif // RESIDUALPLOTWIDGET_HPP_

