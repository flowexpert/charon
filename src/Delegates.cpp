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

TypeDelegate::TypeDelegate(QObject* pp) :
		QStyledItemDelegate(pp) {
}

QWidget* TypeDelegate::createEditor(
			QWidget* pp, const QStyleOptionViewItem& opt,
			const QModelIndex& idx) const {
	if (idx.model()->data(idx).type() == QVariant::String) {
		QComboBox* ed = new QComboBox(pp);
		ed->addItem("");
		ed->setInsertPolicy(QComboBox::InsertAlphabetically);
		ed->addItems(ParamSlotModel::typeMap.uniqueKeys());
		ed->setEditable(true);
		return ed;
	}
	return QStyledItemDelegate::createEditor(pp, opt, idx);
}

void TypeDelegate::setEditorData(
		QWidget* ed, const QModelIndex& idx) const {
	QComboBox* box = qobject_cast<QComboBox*>(ed);
	if(box)
		box->setItemText(0,idx.model()->data(idx).toString());
	else
		QStyledItemDelegate::setEditorData(ed,idx);
}

void TypeDelegate::setModelData(
		QWidget* ed, QAbstractItemModel* md, const QModelIndex& idx) const {
	QComboBox* box = qobject_cast<QComboBox*>(ed);
	if(box)
		md->setData(idx,box->currentText());
	else
		QStyledItemDelegate::setModelData(ed,md,idx);
}

DefaultDelegate::DefaultDelegate(QObject* pp) :
		QStyledItemDelegate(pp) {
}

QWidget* DefaultDelegate::createEditor(
			QWidget* pp, const QStyleOptionViewItem& opt,
			const QModelIndex& idx) const {
	QWidget* res = QStyledItemDelegate::createEditor(pp, opt, idx);
	if(idx.model()->data(idx).type() == QVariant::Double) {
		QDoubleSpinBox* ed = qobject_cast<QDoubleSpinBox*>(res);
		Q_ASSERT(ed);
		ed->setDecimals(6);
	}
	return res;
}
