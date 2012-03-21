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
/** @file   ObjectInspector.cpp
 *  @brief  Implementation of class ObjectInspector
 *  @date   13.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QtGui>
#include <QDir>
#include <QFile>
#include <set>
#include <stack>
#include "ObjectInspector.h"
#include "ParameterFileModel.h"
#include "SplitStream.h"
#include "PrefixValidator.h"
#include "FileManager.h"
#include "InspectorDelegate.h"
#include "QParameterFile.h"

#include "ObjectInspector.moc"
#include "ui_ObjectInspector.h"

ObjectInspector::ObjectInspector(QWidget* myParent,
		ParameterFileModel* newModel) :
	QWidget(myParent), _validator(0), _ownModel(true) {
	_ui = new Ui::ObjectInspector;
	_ui->setupUi(this);

	// init model
	_model = new ParameterFileModel("", this);
	setModel(newModel);

	// init view
	_ui->view->setColumnWidth(0, 120);
	_ui->view->setColumnWidth(1, 120);
	InspectorDelegate* delegate = new InspectorDelegate(this);
	_ui->view->setItemDelegateForColumn(1, delegate);

	// disable prefix editing and hide buttonFrame
	setEdit(false);
}

ObjectInspector::~ObjectInspector() {
	if (_ownModel)
		delete _model;
}

void ObjectInspector::openFile(QString fName) {
	_model->load(fName);
}

void ObjectInspector::saveFile() const {
	if (!isEnabled())
		return;
	if (_model->fileName().isEmpty())
		saveFileAs();
	else
		_model->save();
}

void ObjectInspector::saveFileAs() const {
	if (!isEnabled())
		return;
	QString oldPath = _model->fileName();
	if (oldPath.isEmpty())
		oldPath = QDir::currentPath();
	QString fileName = QFileDialog::getSaveFileName(0, tr("Save File"),
			oldPath, tr("ParameterFile (*.wrp)"));
	if (!fileName.isEmpty())
		_model->save(fileName);
}

void ObjectInspector::setModel(ParameterFileModel* newModel) {
	if (newModel == _model)
		return;

	if (_model) {
		// disconnect everything from the old model
		disconnect(_model, 0, 0, 0);
		disconnect(_ui->prefix, 0, _model, 0);
		disconnect(_ui->useMetadata, 0, _model, 0);
		disconnect(_ui->onlyParams, 0, _model, 0);
	}

	if (newModel) {
		_ui->view->setModel(newModel);

		// delete model if class has ownership
		if (_ownModel && _model)
			delete _model;
		_ownModel = false;
	} else {
		// create empty model
		newModel = new ParameterFileModel();
		_ui->view->setModel(newModel);

		// delete old model if class has ownership
		if (_ownModel)
			delete _model;
		_ownModel = true;
	}

	// commit changes
	_model = newModel;

	Q_ASSERT(_model);

	// update validator
	PrefixValidator* newValidator =
			new PrefixValidator(_model->parameterFile(),_ui->prefix);
	_ui->prefix->setValidator(newValidator);

	if (_validator)
		delete _validator;
	_validator = newValidator;

	// update textEdit content
	_ui->prefix->setText(_model->prefix());

	// empty comment text area
	// text will be updated again, when an item is selected
	_ui->comment->setText("");

	// update values
	_ui->useMetadata->setEnabled(model()->metaInfo());
	_ui->useMetadata->setChecked(model()->useMetaInfo());
	_ui->onlyParams->setChecked(model()->onlyParams());

	// update connections
	connect(_model, SIGNAL(prefixChanged(QString)), _ui->prefix, SLOT(setText(
			QString)));
	connect(_ui->prefix, SIGNAL(textEdited(QString)), _model, SLOT(setPrefix(
			QString)));
	connect(_model, SIGNAL(statusMessage(QString)),
			SIGNAL(statusMessage(QString)));
	connect(_model, SIGNAL(prefixChanged(QString)),
			SLOT(on_model_prefixChanged(QString)));

	// connect additional widgets
	connect(model(), SIGNAL(metaInfoChanged(bool)),
		_ui->useMetadata, SLOT(setEnabled(bool)));
	connect(_ui->useMetadata, SIGNAL(clicked(bool)),
		model(), SLOT(setUseMetaInfo(bool)));
	connect(model(), SIGNAL(useMetaInfoChanged(bool)),
		_ui->useMetadata, SLOT(setChecked(bool)));
	connect(_ui->onlyParams, SIGNAL(clicked(bool)),
		model(), SLOT(setOnlyParams(bool)));
	connect(model(), SIGNAL(onlyParamsChanged(bool)),
		_ui->onlyParams, SLOT(setChecked(bool)));

	on_model_prefixChanged(_model->prefix());

	emit modelChanged(_model);
}

ParameterFileModel* ObjectInspector::model() const {
	return _model;
}

void ObjectInspector::setEdit(bool on) {
	_ui->prefix->setEnabled(on);
	_ui->buttonFrame->setVisible(on);
}

void ObjectInspector::openMetaData(QString fname) {
	if (!QFileInfo(fname).exists()) {
		fname = QFileDialog::getOpenFileName(0, tr("Open File"),
				FileManager::instance().classesFile(),
				tr("ParameterFile (*.*)"));
	}
	_model->loadMetaInfo(fname);
}

void ObjectInspector::on_addButton_clicked() {
	model()->insertRow(model()->rowCount(QModelIndex()), QModelIndex());
}

void ObjectInspector::on_deleteButton_clicked() {
	QItemSelectionModel* selectionModel = _ui->view->selectionModel();
	uint rows = _ui->view->model()->rowCount();

	// collect selected rows
	std::stack<int> rowStack;
	for (uint i = 0; i < rows; i++)
		if (selectionModel->rowIntersectsSelection(i, QModelIndex()))
			rowStack.push(i);

	if (!rowStack.size()) {
		QMessageBox::warning(this, tr("delete failed"), tr(
				"Please select an item to delete first."));
		return;
	}

	// delete rows, reversed to avoid row movement below
	while (rowStack.size() > 0) {
		model()->removeRow(rowStack.top(), QModelIndex());
		rowStack.pop();
	}
}

void ObjectInspector::on_clearButton_clicked() {
	QMessageBox mbox(QMessageBox::Question, tr("confirm delete"), tr(
			"Do you really want to delete the model content?"));
	mbox.addButton(QMessageBox::Yes);
	mbox.addButton(QMessageBox::No);
	mbox.setDefaultButton(QMessageBox::No);
	mbox.setEscapeButton(QMessageBox::No);
	mbox.exec();
	if (mbox.result() == QMessageBox::Yes) {
		model()->clear();
		emit statusMessage(tr("cleared model content"));
	}
}

void ObjectInspector::on_comment_textChanged() {
	_model->setEditorComment(_ui->comment->toPlainText());
}

void ObjectInspector::on_model_prefixChanged(const QString& prefix) {
	// update comment text area
	_ui->comment->setPlainText(_model->parameterFile().get(prefix + ".editorcomment"));
	_ui->commentBox->setEnabled(!prefix.isEmpty() && _model->prefixValid());
}
