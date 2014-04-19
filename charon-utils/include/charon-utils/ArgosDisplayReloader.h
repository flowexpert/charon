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

/*  Declaration of the ArgosDisplay's reloader.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.03.2011
 */

#ifndef _ARGOSDISPLAY_RELOADER_H
#define _ARGOSDISPLAY_RELOADER_H

#include <QObject>
class ParameteredObject;
class QTimer;

/// handle automatic dislpay refresh
class ArgosDisplayReloader : public QObject
{
	Q_OBJECT
public:
	/// set up timer object, linking to parent display
	ArgosDisplayReloader( ParameteredObject* argosDisplay );
	/// timeout ( set to zero to disable timer )
	void setTimeout( int msec );
public slots:
	/// handle timeout
	void reloadArgosDisplay();
	/// start timer (if timeout > 0)
	void start();
private:
	/// parent display
	ParameteredObject *_argosDisplay;
	/// timer object
	QTimer* _timer;
};

#endif

