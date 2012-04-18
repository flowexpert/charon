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

/*  Declaration of the Toggle's timeout.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 10.04.2011
 */

#ifndef _TOGGLE_TIMEOUT_H
#define _TOGGLE_TIMEOUT_H

#include <QObject>
class ParameteredObject;
class QTimer;

/// Timeout to toggle Toggle on timeout
class ToggleTimeout : public QObject
{
	Q_OBJECT
public:
	/// set up ToggleTimeout and connect to given Toggle
	ToggleTimeout( ParameteredObject *toggle );
	/// set timeout
	void setTimeout( int msec );
public slots:
	/// handle timeout event
	void toggleToggle();
	/// start timeout (if timeout > 0)
	void start();
private:
	/// connection to parent Toggle
	ParameteredObject *_toggle;
	/// timer object
	QTimer *_timer;
};

#endif

