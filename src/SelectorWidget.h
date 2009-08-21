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
/** @file   SelectorWidget.h
 *  @brief  Declaration of class SelectorWidget
 *
 *  @date   06.11.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef SELECTORWIDGET_H_
#define SELECTORWIDGET_H_

#include <QWidget>
#include <QPointer>

class ParameterFileModel;
class GraphModel;
class GraphBuilder;
class QUrl;

/// Manage selector of template items for node edition
class SelectorWidget : public QWidget {
    Q_OBJECT

public:
    /// default constructor
    /// @param parent	parent widget
    /// @param classesFile    classes file to use, use file manager if empty
    SelectorWidget(QWidget* parent = 0, QString classesFile = "");

    /// default destructor
    virtual ~SelectorWidget();

public slots:
    /// set graph model to edit
    /// @param model	new model to use for node addition
    void setModel(ParameterFileModel* model);

signals:
    /// show documentation page
    /// @param fileName	fileName of doc page
    void showDocPage(const QString& fileName);

    /// show documentation page
    /// @param doc		strint containing help page (html)
    void showClassDoc(const QString& doc);

    /// @name inform about property changes
    //  @{
    void matchingOnlyChanged(bool);
    void searchChanged(const QString&);
    //  @}

protected slots:
    /// request to add new node of given kind
    /// @param className	name of the class to create a new node
    void _reqAddNode(const QString& className) const;

    /// change property _matchingOnly
    /// @param value		new value
    void _setMatchingOnly(bool value);

    /// set search string
    /// @param str			new search string
    void _setSearch(const QString& str);

    /// reset class filtering
    void _resetFilters();

    /// if only one object is left (due to filtering)
    /// and return is pressed, this object will be added.
    void _addShown();

    /// update population when selection has changed
    void _updateMatching();

private:
    /// model to edit
    QPointer<GraphModel> _editmodel;

    /// this model stores dummy instances of all avaiable classes
    GraphModel* _model;

    /// populate _model with dummy instances
    void _populate();

    /// only show selectable items
    bool _matchingOnly;

    /// string containing search entry
    QString _search;
};

#endif /*SELECTORWIDGET_H_*/
