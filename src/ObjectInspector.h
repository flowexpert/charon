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
/** \file   ObjectInspector.h
 *  \brief  Declaration of class ObjectInspector.
 *  \date   13.08.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef OBJECTINSPECTOR_H_
#define OBJECTINSPECTOR_H_

#include <QWidget>

class ParameterFileModel;
class QTableView;
class QLineEdit;
class QFrame;
class QVBoxLayout;
class PrefixValidator;
class QTextEdit;
class QGroupBox;
class QMutex;

namespace Ui {
	class ObjectInspector;
}

/// Object inspector widget.
/** This widget contains a TableView to edit parameters used in a
 *  ParameterFileModel. Additional it adds the possibilty to add and
 *  delete parameters in this view and provides the needed editors.
 */
class ObjectInspector : public QWidget {
	Q_OBJECT

public:
	/// Default constructor, setting parent widget.
	/** @param  parent  parent widget
	 *  @param  model   model to use in ObjectInspector
	 */
	ObjectInspector(QWidget* parent = 0, ParameterFileModel* model = 0);

	/// default destructor
	virtual ~ObjectInspector();

	/// Get value of property _model
	ParameterFileModel* model() const;

	/// get inspector viewer widget
	QWidget* getViewer();

signals:
	/// Inform about changed model
	/// @param  model   new model
	void modelChanged(ParameterFileModel* model);

	/// Send status message.
	/** Replicates messages from given model.
	 *  \param  msg             message
	 *  \param  timeout         time to show the message
	 */
	void statusMessage(const QString& msg, int timeout = 3000);

public slots:
	/// Open given file.
	/** \param fName            file to open (empty: open file dialog)
	 */
	virtual void openFile(QString fName = QString());

	/// Open file containing metadata
	/** \param fName            file to open (empty: open file dialog)
	 */
	virtual void openMetaData(QString fName = QString());

	/// Save File to it's old location.
	virtual void saveFile() const;

	/// Show SaveAs-Dialog and save File to the selected location.
	virtual void saveFileAs() const;

	/// Change used model
	/// @param  model   new model to use
	virtual void setModel(ParameterFileModel* model);

	/// Enable/Disable structural editing
	/// @param on       set editing
	virtual void setEdit(bool on);

	/// delete parameter
	void delParam();

private slots:
	/// \name handle Gui events
	// \{
	/// Add new Parameter
	void on_addButton_clicked();

	/// Delete selected parameter
	void on_deleteButton_clicked();

	/// Clear model
	void on_clearButton_clicked();

	/// set priority
	void on_setPriorityButton_clicked();

	/// filter by priority
	void on_filterBox_activated(int index);

	/// reset filter
	void on_resetFilterButton_clicked();

	/// handle comment changes
	void on_comment_textChanged();

	/// handle prefix text field changes
	void on_prefix_textChanged(QString text);

	/// handle useMetadata checkbox changes
	void on_useMetadata_clicked(bool state);

	/// handle onlyParams checkbox changes
	void on_onlyParams_clicked(bool state);
	// \}

	/// \name handle model events
	/// (no on_... syntax to avoid connectSlotsByName misinterpretation)
	// \{
	/// update prefix & comment text
	void handle_model_prefixChanged(const QString& text);

	/// enable/disable useMetadata checkbox
	void handle_model_metaInfoChanged(bool state);
	
	/// update useMetadata checkbox state
	void handle_model_useMetaInfoChanged(bool state);

	/// update onlyParams checkbox state
	void handle_model_onlyParamsChanged(bool state);
	// \}

private:
	/// designer gui
	Ui::ObjectInspector* _ui;
	/// save settings like column widths
	void _storeViewSettings() const;
	/// restore settings like column widths
	void _loadViewSettings();

	ParameterFileModel* _model;             ///< Used model to display
	QMutex*             _commentFieldMutex; ///< lock for changes of comments
};

#endif /* OBJECTINSPECTOR_H_ */
