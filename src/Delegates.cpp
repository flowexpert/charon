#include "Delegates.h"
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QRegExpValidator>
#include "ParamSlotModel.h"

ObjNameDelegate::ObjNameDelegate(QObject* pp) :
		QStyledItemDelegate(pp) {
}

QWidget* ObjNameDelegate::createEditor(
			QWidget* pp, const QStyleOptionViewItem& opt,
			const QModelIndex& idx) const {
	QWidget* res = QStyledItemDelegate::createEditor(pp, opt, idx);
	if(idx.model()->data(idx).type() == QVariant::String) {
		QLineEdit* ed = qobject_cast<QLineEdit*>(res);
		Q_ASSERT(ed);
		ed->setValidator(new QRegExpValidator(
				QRegExp("[a-z][a-zA-Z0-9_]*"),ed));
	}
	return res;
}

TypeDelegate::TypeDelegate(QString t, QObject* pp) :
		QItemDelegate(pp),
		_typePrefix(t) {
}

QWidget* TypeDelegate::createEditor(
			QWidget* pp, const QStyleOptionViewItem&,
			const QModelIndex& idx) const {
	Q_ASSERT(idx.model()->data(idx).type() == QVariant::String);
	QSettings s(":/templates/cppTypeMap.ini", QSettings::IniFormat);
	QComboBox* ed = new QComboBox(pp);
	ed->addItem("");
	ed->addItems(s.childKeys());
	if (!_typePrefix.isEmpty()) {
		s.beginGroup(_typePrefix);
		ed->addItems(s.childKeys());
	}
	ed->setEditable(true);
	if(_typePrefix == "Parameters") {
		ed->setValidator(new QRegExpValidator(
				QRegExp("((\\w|<\\s*|\\s*>)*|\\{[\\w;]*\\})"),ed));
	}
	else {
		ed->setValidator(new QRegExpValidator(
				QRegExp("(\\w|<\\s*|\\s*>)*"),ed));
	}
	return ed;
}

void TypeDelegate::setEditorData(
		QWidget* ed, const QModelIndex& idx) const {
	QComboBox* box = qobject_cast<QComboBox*>(ed);
	Q_ASSERT(box);
	box->setItemText(0,idx.model()->data(idx).toString());
}

void TypeDelegate::setModelData(
		QWidget* ed, QAbstractItemModel* md, const QModelIndex& idx) const {
	QComboBox* box = qobject_cast<QComboBox*>(ed);
	Q_ASSERT(box);
	md->setData(idx,box->currentText());
}

DefaultDelegate::DefaultDelegate(QObject* pp) :
		QStyledItemDelegate(pp) {
}

QWidget* DefaultDelegate::createEditor(
			QWidget* pp, const QStyleOptionViewItem& opt,
			const QModelIndex& idx) const {
	QVariant::Type dType = idx.model()->data(idx,Qt::EditRole).type();
	QString mType = idx.model()->data(idx,Qt::UserRole).toString();

	if (dType == QVariant::String &&
			 mType.contains(QRegExp("^\\s*\\{\\s*\\w.*\\}\\s*$"))) {
		QComboBox* editor = new QComboBox(pp);
		editor->setObjectName("selectBox");
		QStringList options = mType.trimmed().mid(
				1,mType.length()-2).trimmed().split(
						";",QString::SkipEmptyParts);
		editor->addItems(options);
		return editor;
	}
	QWidget* res = QStyledItemDelegate::createEditor(pp, opt, idx);
	if (dType == QVariant::Double) {
		QDoubleSpinBox* ed = qobject_cast<QDoubleSpinBox*>(res);
		Q_ASSERT(ed);
		ed->setDecimals(6);
		return res;
	}
	return res;
}

void DefaultDelegate::setEditorData(
		QWidget* ed, const QModelIndex& idx) const {
	QComboBox* box = qobject_cast<QComboBox*>(ed);
	if(box && box->objectName() == "selectBox") {
		QString cur = idx.model()->data(idx, Qt::EditRole).toString();
		int curInd = box->findText(
				cur,(Qt::MatchFlags)Qt::MatchExactly);
		if (curInd >= 0)
			box->setCurrentIndex(curInd);
	}
	else
		QStyledItemDelegate::setEditorData(ed,idx);
}

void DefaultDelegate::setModelData(
		QWidget* ed, QAbstractItemModel* md, const QModelIndex& idx) const {
	QComboBox* box = qobject_cast<QComboBox*>(ed);
	if (box && box->objectName() == "selectBox")
		md->setData(idx, box->currentText());
	else
		QStyledItemDelegate::setModelData(ed,md,idx);
}

