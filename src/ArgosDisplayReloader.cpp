/*  Copyright (C) 2012 The regents of the University of Heidelberg

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file ArgosDisplayReloader.cpp
 *  Definition of the ArgosDisplay's reloader.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.03.2011
 */

#include <QTimer>
#include <ParameteredObject.h>
#include <charon-utils/ArgosDisplayReloader.h>

ArgosDisplayReloader::ArgosDisplayReloader(
		ParameteredObject* argosDisplay)
	: _argosDisplay(argosDisplay)
{
	_timer = new QTimer(this);
	_timer->setSingleShot(true);
	connect(_timer, SIGNAL(timeout()), SLOT(reloadArgosDisplay()));
}

void ArgosDisplayReloader::setTimeout( int msec ) {
	_timer->setInterval(msec);
}

void ArgosDisplayReloader::start() {
	// skip singleshot if zero timeout
	if (_timer->interval() > 0) {
		_timer->start();
	}
}

void ArgosDisplayReloader::reloadArgosDisplay() {
	_argosDisplay->run();
}

