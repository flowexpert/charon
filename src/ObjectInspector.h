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
/** @file   ObjectInspector.h
 *  @brief  Declaration of class ObjectInspector.
 *  @date   13.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef OBJECTINSPECTOR_H_
#define OBJECTINSPECTOR_H_

#include <QWidget>

class ParameterFileModel;
class QTableView;
class QLineEdit;
class QFrame;
class QVBoxLayout;
class PrefixValidator;

/** This widget contains a TableView to edit parameters used in a
 *  ParameterFileModel. Additional it adds the possibilty to add and
 *  delete parameters in this view and provides the needed editors.
 */
class ObjectInspector : public QWidget {
    Q_OBJECT

public:
    /** Default constructor, setting parent widget.
     *  @param  parent  parent widget
     *  @param  model   model to use in ObjectInspector
     */
    ObjectInspector(QWidget* parent = 0, ParameterFileModel* model = 0);

    /// default destructor
    virtual ~ObjectInspector();

    /// Get value of property _model
    ParameterFileModel* model() const;

signals:
    /// Inform about changed model
    /// @param  model   new model
    void modelChanged(ParameterFileModel* model);

    /// Send status message.
    /// Replicates messages from given model.
    /// @param  msg             message
    /// @param  timeout         time to show the message
    void statusMessage(const QString& msg, int timeout = 3000);

public slots:
    /// Show OpenFile-Dialog and display file content.
    virtual void openFile();

    /// Open file containing metadata
    virtual void openMetaData();

    /// Save File to it's old location.
    virtual void saveFile() const;

    /// Show SaveAs-Dialog and save File to the selected location.
    virtual void saveFileAs() const;

    /// Change used model
    /// @param  model   new model to use
    virtual void setModel(ParameterFileModel* model);

    /// Enable/Disable structural editing
    /// @param on       set editing
    virtual void setEdit(bool on);

    /// Add new Parameter
    void addParam();

    /// Delete selected parameter
    void delParam();

    /// Clear model
    void clear();

protected:
    QVBoxLayout*        _mainLayout;    ///< GUI main layout
    QTableView*         _view;          ///< View widget (tree view)

private:
    /// setup widget layout
    void init();

    PrefixValidator*    _validator;     ///< prefix validator
    QLineEdit*          _prefix;        ///< prefix modification widget
    ParameterFileModel* _model;         ///< Used model to display
    bool                _ownModel;      ///< Class has Ownership of _model
};

#endif /* OBJECTINSPECTOR_H_ */
