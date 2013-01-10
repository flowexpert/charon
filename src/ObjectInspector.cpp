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
#include "PriorityDialog.h"
#include "PriorityBox.h"

#include "ObjectInspector.moc"
#include "ui_ObjectInspector.h"

ObjectInspector::ObjectInspector(
			QWidget* myParent, ParameterFileModel* newModel, bool hideTools) :
	QWidget(myParent), _model(0)
{
	_ui = new Ui::ObjectInspector;
	_ui->setupUi(this);

	_commentFieldMutex = new QMutex(QMutex::NonRecursive);

	// use temporary model to set up view
	ParameterFileModel tempModel;
	setModel(&tempModel);

	// set up delegates
	InspectorDelegate* delegate1 = new InspectorDelegate(this);
	_ui->view->setItemDelegateForColumn(1, delegate1);

	QStyledItemDelegate* delegate2 = new QStyledItemDelegate(this);
	QItemEditorCreatorBase* creator =
		new QStandardItemEditorCreator<PriorityBox>();
	QItemEditorFactory* ef = new QItemEditorFactory(
		*QItemEditorFactory::defaultFactory());
	ef->registerEditor(QVariant::UInt, creator);
	delegate2->setItemEditorFactory(ef);
	_ui->view->setItemDelegateForColumn(2, delegate2);

	// unregister temporary model
	setModel(newModel);

	// disable prefix editing and hide buttonFrame
	setEdit(false);

	if (hideTools) {
		_ui->tools->hide();
	}
}

ObjectInspector::~ObjectInspector() {
	_storeViewSettings();
	delete _commentFieldMutex;
	delete _ui;
}

QWidget* ObjectInspector::getViewer() {
	return _ui->view;
}

void ObjectInspector::openFile(QString fName) {
	if (!_model)
		return;
	_model->load(fName);
}

void ObjectInspector::saveFile() const {
	if (!_model || !isEnabled())
		return;
	if (_model->fileName().isEmpty())
		saveFileAs();
	else
		_model->save();
}

void ObjectInspector::saveFileAs() const {
	if (!_model || !isEnabled())
		return;
	QString oldPath = _model->fileName();
	if (oldPath.isEmpty())
		oldPath = QDir::currentPath();
	QString fileName = QFileDialog::getSaveFileName(0, tr("Save File"),
			oldPath, tr("ParameterFile (*.wrp)"));
	if (!fileName.isEmpty())
		_model->save(fileName);
}

void ObjectInspector::_storeViewSettings() const {
	if (_ui->view && _ui->view->model()) {
		QSettings settings;
		settings.beginGroup("ObjectInspector");

		// store column widths
		settings.setValue("column0", _ui->view->columnWidth(0));
		settings.setValue("column1", _ui->view->columnWidth(1));
		settings.setValue("column2", _ui->view->columnWidth(2));

		settings.endGroup();
	}
}

void ObjectInspector::_loadViewSettings() {
	if (_ui->view->model()) {
		QSettings settings;
		settings.beginGroup("ObjectInspector");

		_ui->view->setColumnWidth(0, settings.value("column0", 250).toInt());
		_ui->view->setColumnWidth(1, settings.value("column1", 250).toInt());
		_ui->view->setColumnWidth(2, settings.value("column2", 90).toInt());

		settings.endGroup();
	}
}

void ObjectInspector::setModel(ParameterFileModel* newModel) {
	if (newModel == _model)
		return;

	if (_model) {
		// store view settings
		_storeViewSettings();

		// disconnect everything from the old model
		disconnect(_model, 0, this, 0);
	}

	_ui->view->setModel(newModel);

	// commit changes
	_model = newModel;
	setEnabled(_model);

	if (_model) {
		// init view
		_loadViewSettings();

		// update validator
		PrefixValidator* newValidator =
				new PrefixValidator(_model->parameterFile(),_ui->prefix);
		QValidator* oldValidator = (QValidator*) _ui->prefix->validator();
		_ui->prefix->setValidator(newValidator);
		if (oldValidator) {
			oldValidator->deleteLater();
		}

		// update textEdit content
		handle_model_prefixChanged(_model->prefix());
		_ui->filterBox->setCurrentIndex(_model->minPriority());

		// update values
		_ui->useMetadata->setEnabled(_model->metaInfoValid());
		_ui->useMetadata->setChecked(_model->useMetaInfo());
		_ui->onlyParams->setChecked(_model->onlyParams());

		// update connections
		connect(_model, SIGNAL(statusMessage(QString)),
			SIGNAL(statusMessage(QString)));
		connect(_model, SIGNAL(prefixChanged(QString)),
			SLOT(handle_model_prefixChanged(QString)));
		connect(_model, SIGNAL(metaInfoChanged(bool)),
			SLOT(handle_model_metaInfoChanged(bool)));
		connect(_model, SIGNAL(useMetaInfoChanged(bool)),
			SLOT(handle_model_useMetaInfoChanged(bool)));
		connect(_model, SIGNAL(onlyParamsChanged(bool)),
			SLOT(handle_model_onlyParamsChanged(bool)));
	}
	else {
		_ui->prefix->clear();
		_ui->comment->clear();
	}

	emit modelChanged(_model);
}

ParameterFileModel* ObjectInspector::model() const {
	return _model;
}

void ObjectInspector::setEdit(bool on) {
	_ui->prefix->setEnabled(on);
	_ui->buttonGroupBox->setVisible(on);
}

void ObjectInspector::openMetaData(QString fname) {
	if (_model) {
		if (!QFileInfo(fname).exists()) {
			fname = QFileDialog::getOpenFileName(0, tr("Open File"),
					FileManager::instance().classesFile(),
					tr("ParameterFile (*.*)"));
		}
		_model->loadMetaInfo(fname);
	}
}

void ObjectInspector::delParam() {
	on_deleteButton_clicked();
}

void ObjectInspector::on_addButton_clicked() {
	if (_model) {
		_model->insertRow(model()->rowCount(QModelIndex()), QModelIndex());
	}
}

void ObjectInspector::on_deleteButton_clicked() {
	if (_model) {
		QItemSelectionModel* selectionModel = _ui->view->selectionModel();
		uint rows = _model->rowCount();

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
			_model->removeRow(rowStack.top(), QModelIndex());
			rowStack.pop();
		}
	}
}

void ObjectInspector::on_clearButton_clicked() {
	if (_model) {
		QMessageBox mbox(QMessageBox::Question, tr("confirm delete"), tr(
				"Do you really want to delete the model content?"));
		mbox.addButton(QMessageBox::Yes);
		mbox.addButton(QMessageBox::No);
		mbox.setDefaultButton(QMessageBox::No);
		mbox.setEscapeButton(QMessageBox::No);
		mbox.exec();
		if (mbox.result() == QMessageBox::Yes) {
			_model->clear();
			emit statusMessage(tr("cleared model content"));
		}
	}
}

void ObjectInspector::on_setPriorityButton_clicked() {
	if (_model) {
		QItemSelectionModel* selectionModel = _ui->view->selectionModel();
		uint rows = _ui->view->model()->rowCount();

		// collect selected rows
		std::stack<int> rowStack;
		for (uint i = 0; i < rows; i++)
			if (selectionModel->rowIntersectsSelection(i, QModelIndex()))
				rowStack.push(i);

		if (!rowStack.size()) {
			return;
		}

		// old value, if only one row selected
		int before = 0;
		if (rowStack.size() == 1) {
			before = _model->data(_model->index(rowStack.top(), 2), Qt::EditRole).toInt();
		}

		// open dialog
		PriorityDialog dlg(this, before);

		if (dlg.exec() == QDialog::Rejected)
			return;

		int priority = dlg.selection();

		// determine if change needed
		if (priority != before || rowStack.size() > 1) {
			// change priority
			while (rowStack.size() > 0) {
				_model->setData(_model->index(rowStack.top(), 2), QVariant(priority));
				rowStack.pop();
			}
		}
	}
}

void ObjectInspector::on_filterBox_activated(int index) {
	if (_model) {
		_model->setMinPriority(index);
	}
}

void ObjectInspector::on_resetFilterButton_clicked() {
	if (_model) {
		_ui->filterBox->setCurrentIndex(0);
		_model->setMinPriority(0);
		_model->setPrefix("");
	}
}

void ObjectInspector::on_comment_textChanged() {
	if (_model && isEnabled() && _commentFieldMutex->tryLock()) {
		QString comment = _ui->comment->toPlainText();
		comment.replace(QRegExp("\n"), "<br/>");
		_model->setValue(_model->prefix()+".editorcomment",comment);
		_commentFieldMutex->unlock();
	}
}

void ObjectInspector::handle_model_prefixChanged(const QString& prefix) {
	// update comment text area
	QMutexLocker locker (_commentFieldMutex);
	_ui->prefix->setText(prefix);
	QString comment = _model->getValue(prefix + ".editorcomment");
	comment.replace(QRegExp("<br/?>"), "\n");
	_ui->comment->setPlainText(comment);
	_ui->commentBox->setEnabled(!prefix.isEmpty() && _model->prefixValid());
	locker.unlock();
}

void ObjectInspector::on_prefix_textChanged(QString text) {
	if (_model) {
		_model->setPrefix(text);
	}
}

void ObjectInspector::on_useMetadata_toggled(bool state) {
	if (!state) {
		_ui->onlyParams->setChecked(false);
	}
	if (_model) {
		_model->setUseMetaInfo(state);
	}
	_ui->onlyParams->setEnabled(state);
}

void ObjectInspector::on_onlyParams_toggled(bool state) {
	if (_model) {
		_model->setOnlyParams(state);
	}
}

void ObjectInspector::handle_model_metaInfoChanged(bool state) {
	_ui->useMetadata->setEnabled(state);
}

void ObjectInspector::handle_model_useMetaInfoChanged(bool state) {
	_ui->useMetadata->setChecked(state);
}

void ObjectInspector::handle_model_onlyParamsChanged(bool state) {
	_ui->onlyParams->setChecked(state);
}
