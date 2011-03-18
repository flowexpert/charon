#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>

namespace Ui {
	class Wizard;
}
class QTableWidget;

/// template generator wizard widget
class Wizard : public QWizard
{
	Q_OBJECT

public:
	/// standart constructor
	explicit Wizard(QWidget *parent = 0);
	virtual ~Wizard();

private slots:
	/// \name add and remove table rows
	// \{
	void _addInputSlot(
			QString name="",QString doc="",QString type="",
			bool optional=false, bool multi=false);
	void _addOutputSlot(
			QString name="",QString doc="",QString type="");
	void _addParameter(
			QString name="",QString doc="",QString type="",
			QString defVal="", bool list=false);
	void _delInputSlot();
	void _delOutputSlot();
	void _delParameter();
	// \}

protected:
	/// generate module
	virtual void done(int result);

private:
	/// designer GUI
	Ui::Wizard* _ui;
	/// delete selected row from table widget
	/** if no row is selected, delete last row
	 *  \param table     table widget to use
	 */
	void _delRow(QTableWidget* table);
};

#endif // WIZARD_H
