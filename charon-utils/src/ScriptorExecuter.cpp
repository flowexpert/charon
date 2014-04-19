/*  Copyright (C) 2012 mox

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file Scriptor_Executor.cpp
 *  This file is needed for class ScriptorBase to work as a charon plugin.
 *  \author <a href="mailto:de_mox@zoho.com">
 *      mox</a>
 *  \date 29.02.2012
 */


#include <charon-utils/Scriptor/ScriptorExecuter.h>
#include <QStringList>
#include <QListIterator>
#include <QDir>
#include <charon-core/SplitStream.h>

ScriptorExecuter::ScriptorExecuter(QString executable,
									QStringList arguments,
									QString workDir):
	QObject(), _proc(this), _executable(executable), _arguments(arguments)
{

	//Import environment of mother process
//	QStringList env = QProcess::systemEnvironment();
//	env.replaceInStrings(QRegExp("^LANG=(.*)"), "LANG=C");
//	_proc.setEnvironment(env);

	// if _workDir is set and sane, use ist, else skip it
	if(QDir(workDir).exists())
	{
		_proc.setWorkingDirectory(workDir);
	}

	// make the readerfunctions start, when there is output to read
	connect(&_proc, SIGNAL(readyReadStandardOutput()), this,
					SLOT(_readStandardOut()));
	connect(&_proc, SIGNAL(readyReadStandardError()), this,
					SLOT(_readErrorOut()));
}

int ScriptorExecuter::run()
{
	sout << "Calling executable or interpreter:\n\t" << _executable.toStdString() << std::endl ;
	sout << "with arguments:\n\t" << _arguments.join("\n\t").toStdString() << std::endl ;
	sout << "Script output: " << std::endl ;
	_proc.start(_executable, _arguments);
	//we never write to the process Stdin, close the channel as the process won't exit otherwise
	_proc.closeWriteChannel() ;
	_proc.waitForFinished(-1);
	_exitCode = _proc.exitCode();
	return _exitCode;
}

void ScriptorExecuter::_readStandardOut()
{
	QString str(_proc.readAllStandardOutput());
	QStringList output = str.split("\n");

	// write each line of output to sout
	QStringList::const_iterator constIterator;
	for(constIterator = output.constBegin(); constIterator != output.constEnd();
			++constIterator)
	{
		sout << constIterator->toLocal8Bit().constData() << std::endl;
	}
}

void ScriptorExecuter::_readErrorOut()
{
	QString str(_proc.readAllStandardError());
	QStringList errout = str.split("\n");

	// write each line of output to sout
	QStringList::const_iterator constIterator;
	for(constIterator = errout.constBegin(); constIterator != errout.constEnd();
			++constIterator)
	{
		sout << "(EE) " << constIterator->toLocal8Bit().constData() << std::endl;
	}
}

int ScriptorExecuter::exitCode()
{
	return _exitCode;
}
