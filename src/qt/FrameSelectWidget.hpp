/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file FrameSelectWidget.h
 *  Declaration of class FrameSelectWidget.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#ifndef FRAMESELECTWIDGET_H
#define FRAMESELECTWIDGET_H

#include <QDockWidget>
#include <charon-core/ParameteredObject.h>
#include "charonwidgets.h"

namespace Ui {
	class FrameSelectWidget;
}

class charonwidgets_DECLDIR FrameSelectWidget : public QDockWidget
{
	Q_OBJECT

public:
	explicit FrameSelectWidget(
			ParameteredObject* parentParamObj,
			Parameter<bool>& cropV,
			Parameter<uint>& z,
			Parameter<uint>& t,
			Parameter<uint>& v,
			QWidget* parentWidget = 0);
	~FrameSelectWidget();
	void setDisplay(ParameteredObject* display);
	void setShape(uint dz, uint dt, uint dv);

public slots:
	void setCropV(bool val);
	void setDim2(int val);
	void setDim3(int val);
	void setDim4(int val);

private:
	Ui::FrameSelectWidget* _ui;
	ParameteredObject* _parent;
	ParameteredObject* _display;
	Parameter<bool>& _cropV;
	Parameter<uint>& _z;
	Parameter<uint>& _t;
	Parameter<uint>& _v;
	void _update();
};

#endif // FRAMESELECTWIDGET_H
