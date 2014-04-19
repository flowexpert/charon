/*  Copyright (C) 2012 The regents of the University of Heidelberg

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file ToggleTimeout.cpp
 *  Definition of the Toggle's timeout.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 10.04.2011
 */

#include <charon-utils/ToggleTimeout.h>

#include <charon-core/ParameteredObject.h>
#include <charon-utils/Toggle.h>
#include <QTimer>

ToggleTimeout::ToggleTimeout( ParameteredObject* toggle )
	: _toggle(toggle)
{
	_timer = new QTimer( this );
	_timer->setSingleShot( true );
	connect(_timer, SIGNAL(timeout()), SLOT(toggleToggle()));
}

void ToggleTimeout::setTimeout( int msec )
{
	_timer->setInterval(msec);
}

void ToggleTimeout::start()
{
	// skip singleshot if zero timeout
	if (_timer->interval() > 0) {
		_timer->start();
	}
}

void ToggleTimeout::toggleToggle()
{
	dynamic_cast<ToggleInterface*>(_toggle)->toggleToggle();
}

