/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file   GraphModel.h
 *  \brief  Declaration of class GraphModel
 *
 *  \date   10.09.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef GRAPHMODEL_H_
#define GRAPHMODEL_H_

#include "ParameterFileModel.h"

/// This model wraps a ParameterFile instance and provides
/// access to the data interpreted as a (directed) graph.
class GraphModel: public ParameterFileModel {
	Q_OBJECT

public:
	/// Constructor initializing the model with the given file.
	/** \param metaFile     ParameterFile describing classes and in-/outputs
	 *  \param fileName     filename of the textfile to load.
	 *  \param parent       parent QObject passed to inherited constructor.
	 */
	GraphModel(QString fileName = "", QObject* parent = 0,
		QString metaFile = "");

	/// Clear ParameterFile content.
	/** \param draw         emit display refresh signal if true */
	virtual void clear(bool draw);

	/// Clear ParameterFile content with redraw.
	/** This has to be set up separately to override the parent class
	 *  method.
	 */
	virtual void clear();

	/// Checks if the given node is a valid object in the ParameterFile.
	/** \param name node name
	 *  \retval true        node is valid
	 */
	bool nodeValid(const QString& name) const;

	/// Check if given connection is valid and established.
	/** This can also be used, to check if a connection could be established
	 *  (using an exception as hint, that something goes wrong).
	 *  \param source       source slot ("\<object\>.\<slot\>")
	 *  \param target       target slot ("\<object\>.\<slot\>")
	 *  \throws runtime_error if connection is not valid
	 *  \retval true        nodes are connected
	 */
	bool connected(QString source, QString target) const;


	/// Get nodes in current graph
	QStringList nodes() const;

	virtual bool setData (
			const QModelIndex& index, const QVariant& value,
			int role = Qt::EditRole);
	virtual bool removeRows (
			int row, int count,
			const QModelIndex& parent = QModelIndex());

public slots:
	/// select next item
	/** \param back         select previous item, if back is true
	 */
	void selectNext(bool back = false);

	/// add new node of given kind
	/** \param className    name of the class to create a new node
	 *  \param draw         emit display refresh signal if true
	 *  \returns QString    name of new node (entered by user)
	 */
	QString addNode(QString className, bool draw = true);

	/// rename node
	/** \param nodename     name of the node to rename
	 *  \param draw         emit display refresh signal if true
	 */
	void renameNode(QString nodename, bool draw = true);

	/// delete node
	/** \param nodename     name of the node to delete
	 *  \param draw         emit display refresh signal if true
	 */
	bool deleteNode(QString nodename, bool draw = true);

	/// connect slots
	/** \param source       source slot ("\<object\>.\<slot\>")
	 *  \param target       target slot ("\<object\>.\<slot\>")
	 *  \param draw         emit display refresh signal if true
	 */
	void connectSlot(QString source, QString target, bool draw = true);

	/// disconnect slots
	/** \param source       source slot ("\<object\>.\<slot\>")
	 *  \param target       target slot ("\<object\>.\<slot\>"),
	 *                      disconnect from all targets, if empty
	 *  \param draw         emit display refresh signal if true
	 */
	void disconnectSlot(QString source, QString target = QString(), bool draw = true);

	/// disconnect all slots of given node
	/** \param node         node name ("\<object\>")
	 *  \param draw         emit display refresh signal if true
	 */
	void disconnectAllSlots(QString node, bool draw = true);
	//  \}

	/// load metaFile
	/** \param fileName         filename to load from */
	virtual void loadMetaFile(QString fileName);

	/// emit graph changed signal to update displays
	/** this has to be done, when the ParameterFileModel content
	 *  has been changed so that the graph structure has to be redrawn
	 */
	void reDraw();

protected slots:
	/// Load data without showing OpenFile Dialog.
	virtual bool _load();

signals:
	/// Initiate graph rebuild and Display refresh
	void graphChanged();
};

#endif /* GRAPHMODEL_H_ */
