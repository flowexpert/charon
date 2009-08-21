/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file	HierarchyGraphView.h
 *	@brief	Declaration of class GraphView
 *
 *	@date	21.11.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef HIERARCHYGRAPHVIEW_H_
#define HIERARCHYGRAPHVIEW_H_

#include "GraphView.h"

class QAbstractGraphicsShapeItem;

/// Graph view with drag & drop handling specialized for display of
/// flow visualizations.
class HierarchyGraphView : public GraphView {
	Q_OBJECT

public:
	/// Default constructor.
	/// @param parent		parent widget
	HierarchyGraphView(QWidget* parent = 0);
	
	/// default destructor
	virtual ~HierarchyGraphView();

protected:
	/// Render graph and update display
	///	@param graph	Graph to render
	virtual void _renderGraph(graph_t* graph);
		
	/// @name Input device handling.
	//	@{

	/// Mouse press event handler.
	/// This manages item selection and saves the event position to be able
	/// to recognize dragging.
	/// @param event		mouse event
	virtual void mousePressEvent(QMouseEvent* event);
	
	/// Mouse press event handler.
	/// Graph nodes can be renamed by double clicking on it.
	/// @param event		mouse event
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	
	/// Mouse move event handler.
	/// When the mouse is moved with pressed left button and the distance
	/// is beyond the system defined start drag distance, a drag is emitted
	/// with the current slot (if there is any).
	/// @param event		mouse event
	virtual void mouseMoveEvent(QMouseEvent* event);
	
	/// Key press event handler.
	/// This performs the following actions on the selected node:
	/// -	\<arrow left/right\>: select previous/next node
	/// -	\<del\>: delete node
	/// -	\<F2\>: rename node (same as double click)
	/// @param event		key event
	virtual void keyPressEvent (QKeyEvent* event);

	//	@} /* input device handling */
	
	/// @name drag&drop handling
	//	@{

	/// Drag enter event handler.
	/// Checks that the entered drag contains data that can be processed
	/// and ignores it otherwise.
	/// @param event		entered drag event
	virtual void dragEnterEvent(QDragEnterEvent* event);
	
	/// Drag leave event handler.
	/// Reset all highlighted slots.
	/// @param event		left drag event
	virtual void dragLeaveEvent(QDragLeaveEvent* event);
	
	/// Drag move event handler.
	/// Check if drag has reached or left an input/output slot and
	/// highlight it, if a connection can be established.
	/// @param event		moved drag event
	virtual void dragMoveEvent(QDragMoveEvent* event);
	
	/// Drop event handler.
	/// Establish slot connection if possible.
	/// @param event		drop event
	virtual void dropEvent(QDropEvent* event);

	//	@} /* drag&drop handling */
	
	/// Check if drop could establish connection at the current position.
	/// @param event		drop event
	bool _match(QDropEvent* event) const;
	
	/// Highlight some QGraphicsRect in green color.
	/// @param item			item to highlight
	/// @param colour		colour to use to highlight
	void _highlight(QAbstractGraphicsShapeItem* item, QColor colour);
	
	/// Clear highlighted items
	void _clearHighlight();
	
	/// Highlighted item
	QList<QAbstractGraphicsShapeItem*> _highlighted;
};

#endif /*HIERARCHYGRAPHVIEW_H_*/
