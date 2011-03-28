#ifndef WIZARDPAGESLOTS_H
#define WIZARDPAGESLOTS_H

#include <QWizardPage>
#include <QAbstractTableModel>

namespace Ui {
	class WizardPageSlots;
}
class QTableWidget;

/// class providing validators and initialization for wizard page
class WizardPageSlots : public QWizardPage
{
	Q_OBJECT

public:
	/// default constructor
	explicit WizardPageSlots(QWidget *parent = 0);
	virtual ~WizardPageSlots();
	/// return complete state
	virtual bool isComplete() const;

private slots:
	/// \name add and remove table rows
	// \{
	void _addInputSlot(
			QString name="",QString doc="",QString type="",
			bool optional=false, bool multi=false);
	void _addOutputSlot(
			QString name="",QString doc="",QString type="");
	void _delInputSlot();
	void _delOutputSlot();
	// \}

private:
	/// model used for parameter edit table
	class SlotModel : public QAbstractTableModel {
	public:
		/// suppress optional and multi flags (for output slots)
		bool skipFlags;

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
		QList<bool> optional;
		QList<bool> multi;
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
	Ui::WizardPageSlots* _ui;
	/// input slots
	SlotModel _inputSlots;
	/// output slots
	SlotModel _outputSlots;
};

#endif // WIZARDPAGESLOTS_H
