#ifndef DELEGATES_H
#define DELEGATES_H

#include <QStyledItemDelegate>

/// delegate using a line edit with regExp validator for string data
class ObjNameDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	/// default constructor
	ObjNameDelegate(QObject* parent = 0);

	/// Returns the widget used to edit the item
	/// specified by index for editing
	virtual QWidget* createEditor(
			QWidget* parent, const QStyleOptionViewItem& option,
			const QModelIndex& index) const;
};

/// delegate using comboBox providing typestrings for type input
class TypeDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	/// default constructor
	TypeDelegate(QObject* parent = 0);

	/// Returns the widget used to edit the item
	/// specified by index for editing
	virtual QWidget* createEditor(
			QWidget* parent, const QStyleOptionViewItem& option,
			const QModelIndex& index) const;

	/// Sets the data to be displayed and edited by the editor from the
	/// data model item specified by the model index
	virtual void setEditorData(
			QWidget* editor, const QModelIndex& index) const;

	/// Gets data from the editor widget and stores it in the specified
	/// model at the item index
	virtual void setModelData(
			QWidget* editor, QAbstractItemModel* model,
			const QModelIndex& index ) const;
};

/// delegate using a line edit with regExp validator for string data
class DefaultDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	/// default constructor
	DefaultDelegate(QObject* parent = 0);

	/// Returns the widget used to edit the item
	/// specified by index for editing
	virtual QWidget* createEditor(
			QWidget* parent, const QStyleOptionViewItem& option,
			const QModelIndex& index) const;
};

#endif // DELEGATES_H
