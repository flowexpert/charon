/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   WorkflowExecutor.h
 *  @brief  Declaration of class WorkflowExecutor.
 *  @date   15.04.2010
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef WORKFLOWEXECUTOR_H
#define WORKFLOWEXECUTOR_H

#include <QObject>
#include <QAction>
class PluginManager;
class ObjectInspector;

/// Management of workflow execution
/** This class provides the run workflow action to add to toolbars and menus.
 *  It manages execution state etc.
 */
class WorkflowExecutor : public QObject
{
	Q_OBJECT

private:
	/// Pointer to execution action
	QAction* _runAction;
	/// update action icon
	void _updateIcon();
	/// pointer to plugin manager
	PluginManager* _manager;
	/// log filename
	std::string _logFileName;
	/// log writer
	std::ofstream* _log;
	/// backup of cwd
	QString _pathBak;
	/// pointer to ObjectInspector holding model information
	ObjectInspector* _inspector;
	/// execute workflow
	void _run();
	/// cleanup plugins
	void _cleanup();

public:
	/// standard constructor
	explicit WorkflowExecutor(
			ObjectInspector* inspector  /** [in] object inspector widget*/,
			QObject* parent = 0          /** [in] parent parent object*/);
	virtual ~WorkflowExecutor();

	/// get pointer to the action for usage in menus or toolbars
	QAction* runAction();

public slots:
	/// Execute current workflow
	/** The executed workflow corresponds to the currently active flow
	 *  widget and is determined using the object inspector.
	 *
	 *  The behaviour of the workflow execution is influenced by the
	 *  QSettings property <it>waitAfterExecute<it>.
	 *
	 *  If this property is set to <tt>true</tt>, a message box shows up after
	 *  execution termination, all plugins stay loaded.
	 *  This is e.g. useful for interactive workflows.
	 *  Plugin cleanup is performed on the next call to this function.
	 *
	 *  If <it>waitAfterExecute<it> is set to false, a plugin cleanup is
	 *  performed on execution termination which unloads all plugins.
	 */
	void run();
};

#endif // WORKFLOWEXECUTOR_H
