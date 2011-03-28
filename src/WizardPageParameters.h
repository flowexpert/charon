#ifndef WIZARDPAGEPARAMETERS_H
#define WIZARDPAGEPARAMETERS_H

#include <QWizardPage>
#include <QAbstractTableModel>
class QTableWidget;

namespace Ui {
	class WizardPageParameters;
}

/// class providing validators and initialization for wizard page
class WizardPageParameters : public QWizardPage
{
	Q_OBJECT

public:
	/// default constructor
	explicit WizardPageParameters(QWidget *parent = 0);
	~WizardPageParameters();
	/// return complete state
	virtual bool isComplete() const;

private slots:
	/// \name add and remove table rows
	// \{
	void _addParameter(
			QString name="",QString doc="",QString type="",
			QString defVal="", bool list=false);
	void _delParameter();
	// \}

private:
	/// model used for parameter edit table
	class ParamModel : public QAbstractTableModel {
	public:
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
		QList<bool> list;
		// \}

		/// \name add and remove table rows
		// \{
		virtual bool insertRows(
				int row, int count, const QModelIndex& parent = QModelIndex());
		virtual bool removeRows(
				int row, int count, const QModelIndex& parent = QModelIndex());
		// \}
	};

	/// designer GUI
	Ui::WizardPageParameters* _ui;
	/// parameter table model
	ParamModel _model;
};

#endif // WIZARDPAGEPARAMETERS_H
