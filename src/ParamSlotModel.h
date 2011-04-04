#ifndef PARAMSLOTMODEL_H
#define PARAMSLOTMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

/// model used for parameter and slots edit table
class ParamSlotModel : public QAbstractTableModel
{
	Q_OBJECT
private:
	/// QVariant type mapping usied in types
	static QMap<QString, QVariant::Type> _typeMap;

public:
	ParamSlotModel();

	/// \name implement model interface
	// \{
	virtual int rowCount(
			const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(
			const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(
			const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags(
			const QModelIndex& index) const;
	virtual bool setData(
			const QModelIndex& index, const QVariant& value,
			int role = Qt::EditRole);
	// \}

	/// \name store parameter config
	// \{
	QStringList names;
	QStringList docs;
	QStringList types;
	QStringList defaults;
	QList<bool> optional;
	QList<bool> multi;
	QList<bool> list;
	// \}

	/// \name add and remove table rows
	// \{
	virtual bool insertRows(
			int row, int count, const QModelIndex& parent = QModelIndex());
	virtual bool removeRows(
			int row, int count, const QModelIndex& parent = QModelIndex());
	// \}

	/// check if all required fields are filled
	bool isValid() const;

	/// list conversion to string list (for convenience)
	static QStringList toStringList(const QList<bool>& list);
	/// list conversion to bool list (for convenience)
	static QList<bool> toBoolList(const QStringList& list);

	/// map access
	static const QMap<QString, QVariant::Type>& typeMap;
	/// type map lookup with string default
	static QVariant::Type typeLookup(QString type);

	/// user types
	enum Type {
		FileOpen = 257,
		FileWrite = 258,
		Path = 259
	};
};

#endif // PARAMSLOTMODEL_H
