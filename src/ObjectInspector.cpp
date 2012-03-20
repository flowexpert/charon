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

ObjectInspector::ObjectInspector(QWidget* myParent,
		ParameterFileModel* newModel) :
	QWidget(myParent), _validator(0), _ownModel(true) {
	// set up main layout
	_mainLayout = new QVBoxLayout(this);
	_view = new QTableView(this);
	_prefix = new QLineEdit(this);
	_comment = new QTextEdit(this);

	// init model
	_model = new ParameterFileModel("", this);
	setModel(newModel);

	// init GUI
	init();

	// disable prefix editing
	setEdit(false);
	on_model_prefixChanged(_model->prefix());
}

void ObjectInspector::init() {
	// init prefix widget
	QFrame* prefixFrame = new QFrame();
	prefixFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
	QGridLayout* prefixLayout = new QGridLayout(prefixFrame);
	QLabel* label1 = new QLabel(tr("&Prefix"));
	label1->setBuddy(_prefix);
	prefixLayout->addWidget(label1, 0, 0);
	prefixLayout->addWidget(_prefix, 0, 1);
	_mainLayout->addWidget(prefixFrame);

	// init comment widget
	_commentBox = new QGroupBox(tr("&Comment"));
	QHBoxLayout* commentLayout = new QHBoxLayout(_commentBox);
	commentLayout->addWidget(_comment);
	_mainLayout->addWidget(_commentBox);

	// init view widget
	_view->setAlternatingRowColors(true);
	_view->setEditTriggers(QAbstractItemView::AllEditTriggers);
	_view->verticalHeader()->hide();
	_view->setCornerButtonEnabled(false);
	_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_view->setColumnWidth(0, 120);
	_view->setColumnWidth(1, 120);
	_view->horizontalHeader()->setStretchLastSection(true);
	InspectorDelegate* delegate = new InspectorDelegate(this);
	_view->setItemDelegateForColumn(1, delegate);
	_view->setMouseTracking(true);
	_mainLayout->addWidget(_view);
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
		disconnect(_prefix, 0, _model, 0);
	}

	if (newModel) {
		_view->setModel(newModel);

		// delete model if class has ownership
		if (_ownModel && _model)
			delete _model;
		_ownModel = false;
	} else {
		// create empty model
		newModel = new ParameterFileModel();
		_view->setModel(newModel);

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
			new PrefixValidator(_model->parameterFile(),_prefix);
	_prefix->setValidator(newValidator);

	if (_validator)
		delete _validator;
	_validator = newValidator;

	// update textEdit content
	_prefix->setText(_model->prefix());

	// empty comment text area
	// text will be updated again, when an item is selected
	_comment->setText("");

	// update connections
	connect(_model, SIGNAL(prefixChanged(QString)), _prefix, SLOT(setText(
			QString)));
	connect(_prefix, SIGNAL(textEdited(QString)), _model, SLOT(setPrefix(
			QString)));
	connect(_model, SIGNAL(statusMessage(QString)),
			SIGNAL(statusMessage(QString)));
	connect(_comment, SIGNAL(textChanged()), SLOT(on_comment_textChanged()));
	connect(_model, SIGNAL(prefixChanged(QString)),
			SLOT(on_model_prefixChanged(QString)));

	emit modelChanged(_model);
}

ParameterFileModel* ObjectInspector::model() const {
	return _model;
}

void ObjectInspector::setEdit(bool on) {
	_prefix->setEnabled(on);
}

void ObjectInspector::openMetaData(QString fname) {
	if (!QFileInfo(fname).exists()) {
		fname = QFileDialog::getOpenFileName(0, tr("Open File"),
				FileManager::instance().classesFile(),
				tr("ParameterFile (*.*)"));
	}
	_model->loadMetaInfo(fname);
}

void ObjectInspector::addParam() {
	model()->insertRow(model()->rowCount(QModelIndex()), QModelIndex());
}

void ObjectInspector::delParam() {
	QItemSelectionModel* selectionModel = _view->selectionModel();
	uint rows = _view->model()->rowCount();

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

void ObjectInspector::clear() {
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
	bool valSet = false;
	_model->setOnlyParams(false);

	// check if "editorcomment" entry already exists
	for (int i = 0; i < _model->rowCount(); i++) {
		QString str = _model->data(_model->index(i,0)).toString();
		if (_model->data(_model->index(i,0)).toString() == "editorcomment") {
			_model->setData(_model->index(i,1), _comment->toPlainText());
			valSet = true;
			break;
		}
	}

	// if not, insert row
	if (!valSet && !_comment->toPlainText().isEmpty()) {
		_model->insertRow(_model->rowCount());
		_model->setData(_model->index(_model->rowCount()-1,0), "editorcomment");
		_model->setData(_model->index(_model->rowCount()-1,1), _comment->toPlainText());
	}

	_model->setOnlyParams(true);
}

void ObjectInspector::on_model_prefixChanged(const QString& prefix) {
	// update comment text area
	_comment->setPlainText(_model->parameterFile().get(prefix + ".editorcomment"));
	_commentBox->setEnabled(!prefix.isEmpty());
}
