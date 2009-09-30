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

//  The filename editor widgets are based on CMake code:
//
//  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
//
//  See CMakeCopyright.txt or http://www.cmake.org/HTML/Copyright.html for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notices for more information.

#include "InspectorDelegate.h"
#include "ParameterFileModel.h"
#include "MetaData.h"
#include <QDirModel>
#include <QFileInfo>
#include <QFileDialog>
#include <QToolButton>
#include <QResizeEvent>
#include <QDebug>

InspectorDelegate::InspectorDelegate(QObject* p) :
		QStyledItemDelegate(p) {
}

QWidget* InspectorDelegate::createEditor(QWidget* p,
		const QStyleOptionViewItem& opt, const QModelIndex& ind) const {

	// detect parameter type
	const ParameterFileModel* model =
			qobject_cast<const ParameterFileModel*>(ind.model());
	if (model && model->onlyParams()) {
		std::string param = model->data(
				ind.sibling(ind.row(),0)).toString().toAscii().constData();
		if (!model->prefix().isEmpty())
			param = std::string(model->prefix().toAscii().constData())
					+ "." + param;
		std::string type = model->metaInfo()->getType(param, model->getClass(param));
		if (type == "filename") {
			InspectorFilePathEditor* editor =
					new InspectorFilePathEditor(p, param.c_str());
			connect(editor, SIGNAL(fileDialogExists(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type == "path") {
			InspectorPathEditor* editor =
					new InspectorPathEditor(p, param.c_str());
			connect(editor, SIGNAL(fileDialogExists(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
	}
	return QStyledItemDelegate::createEditor(p,opt,ind);
}

void InspectorDelegate::_setFileDialogFlag(bool f) {
	_fileDialogFlag = f;
}

bool InspectorDelegate::editorEvent(QEvent* e, QAbstractItemModel* model,
       const QStyleOptionViewItem& option, const QModelIndex& index)
{
	Qt::ItemFlags flags = model->flags(index);
	if (!(flags & Qt::ItemIsUserCheckable) || !(option.state & QStyle::State_Enabled)
			|| !(flags & Qt::ItemIsEnabled))
		return false;

	QVariant value = index.data(Qt::CheckStateRole);
	if (!value.isValid())
		return false;

	if ((e->type() == QEvent::MouseButtonRelease)
			|| (e->type() == QEvent::MouseButtonDblClick)) {
		// eat the double click events inside the check rect
		if (e->type() == QEvent::MouseButtonDblClick)
			return true;
	}
	else if (e->type() == QEvent::KeyPress) {
		if(static_cast<QKeyEvent*>(e)->key() != Qt::Key_Space &&
			static_cast<QKeyEvent*>(e)->key() != Qt::Key_Select)
				return false;
	}
	else
		return false;

	Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) ==
		Qt::Checked ? Qt::Unchecked : Qt::Checked);
	return model->setData(index, state, Qt::CheckStateRole);
}

bool InspectorDelegate::eventFilter(QObject* object, QEvent* ev)
{
	if(ev->type() == QEvent::FocusOut && _fileDialogFlag)
		return false;
	return QStyledItemDelegate::eventFilter(object, ev);
}


InspectorFileEditor::InspectorFileEditor(QWidget* p, const QString& var) :
		QLineEdit(p), _variable(var) {
	_toolButton = new QToolButton(this);
	_toolButton->setText("...");
	_toolButton->setCursor(QCursor(Qt::ArrowCursor));
	QObject::connect(_toolButton, SIGNAL(clicked(bool)),
						this, SLOT(chooseFile()));
}

void InspectorFileEditor::resizeEvent(QResizeEvent* e) {
	// make the tool button fit on the right side
	int h = e->size().height();
	// move the line edit to make room for the tool button
	setContentsMargins(0, 0, h, 0);
	// put the tool button in its place
	_toolButton->resize(h, h);
	_toolButton->move(width() - h, 0);
}

InspectorFilePathEditor::InspectorFilePathEditor(QWidget* p, const QString& var)
		: InspectorFileEditor(p, var) {
	setCompleter(new InspectorFileCompleter(this, false));
}

void InspectorFilePathEditor::chooseFile() {
	// choose a file and set it
	QString path;
	QFileInfo info(text());
	QString title;
	if (_variable.isEmpty())
		title = tr("Select File");
	else
		title = tr("Select File for %1").arg(_variable);

	emit fileDialogExists(true);
	qDebug() << QString("current path: %1").arg(info.absoluteFilePath());
	path = QFileDialog::getOpenFileName(this, title, info.absoluteFilePath());
	emit fileDialogExists(false);

	if (!path.isEmpty())
		this->setText(QDir::fromNativeSeparators(path));
}

InspectorPathEditor::InspectorPathEditor(QWidget* p, const QString& var)
		: InspectorFileEditor(p, var) {
	setCompleter(new InspectorFileCompleter(this, true));
}

void InspectorPathEditor::chooseFile() {
	// choose a file and set it
	QString path;
	QString title;
	if (_variable.isEmpty())
		title = tr("Select Path");
	else
		title = tr("Select Path for %1").arg(_variable);

	emit fileDialogExists(true);
	path = QFileDialog::getExistingDirectory(this, title, text());
	emit fileDialogExists(false);
	if (!path.isEmpty())
		setText(QDir::fromNativeSeparators(path));
}

// use same QDirModel for all completers
static QDirModel* fileDirModel() {
	static QDirModel* m = NULL;
	if (!m)
		m = new QDirModel();
	return m;
}

static QDirModel* pathDirModel() {
	static QDirModel* m = NULL;
	if (!m) {
		m = new QDirModel();
		m->setFilter(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot);
	}
	return m;
}

InspectorFileCompleter::InspectorFileCompleter(QObject* o, bool dirs)
		: QCompleter(o) {
	QDirModel* m = dirs ? pathDirModel() : fileDirModel();
	this->setModel(m);
}

QString InspectorFileCompleter::pathFromIndex(const QModelIndex& idx) const {
	return QDir::fromNativeSeparators(QCompleter::pathFromIndex(idx));
}

#include "InspectorDelegate.moc"
