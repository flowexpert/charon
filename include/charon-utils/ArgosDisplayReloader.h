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

/*  Declaration of the ArgosDisplay's reloader.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.03.2011
 */

#ifndef _ARGOSDISPLAY_RELOADER_H
#define _ARGOSDISPLAY_RELOADER_H

#include <QtCore/QTimer>

#include <charon-core/ParameteredObject.h>

class ArgosDisplayReloader
	: public QTimer
{
	Q_OBJECT
public:
	ArgosDisplayReloader( ParameteredObject *argosDisplay );
	void setTimeout( int msec );
public slots:
	void reloadArgosDisplay();
private:
	ParameteredObject *_argosDisplay;
	int _msec;
};

#endif

