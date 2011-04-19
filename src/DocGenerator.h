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
/** \file DocGenerator.h
 *  \brief Declaration of class DocGenerator
 *
 *  \date 26.11.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef DOCGENERATOR_H_
#define DOCGENERATOR_H_

#include <QObject>
#include <vector>

class QWebView;
class QUrl;

/// handle documentation page generation
class DocGenerator : public QObject {
	Q_OBJECT

public:
	/// Default constructor.
	/** \param viewer       pointer to help browser display
	 *  \param parent       pointer to parent object
	 */
	DocGenerator(QWebView* viewer, QObject* parent = 0);

	virtual ~DocGenerator();

public slots:
	/// show help page in help browser
	void showHelp();

	/// show introduction message in help browser
	void showIntro();

	/// Show documentation page.
	/** The file can use html markup but has to omit the document
	 *  frame. Only the content between \<body\>\</body\> has to be passed.
	 *  \param fileName     name of the html file to display
	 */
	void showDocPage(const QString& fileName);

	/// show documentation page
	/** \param className    class name to generate documentation for */
	void showClassDoc(const QString& className);

	/// Show the given html-formatted string.
	/** This string will be passed to the body of an html document
	 *  using the global stylesheet.
	 *  \param doc          string containing help page (html)
	 */
	void showDocString(const QString& doc);

protected:
	/// Generate documentation for the given list of parameters.
	/** The documentation is arranged in a bullte point list.
	 *  Each parameter is shown with its type separated by a colon.
	 *  \param parList      list of parameters to document
	 *  \param className    class name to generate documentation for
	 *  \param slotType     slot type (if needed), can be "in" or "out"
	 *  \returns            html formatted docstring
	 */
	QString _docList(const std::vector<std::string>& parList,
		const std::string& className, const std::string& slotType = "") const;

	/// pointer to used viewer instance
	QWebView* _viewer;

	/// stylesheet for help doc
	QString _stylesheet;

	/// footer for doc pages
	QString _footer;

	/// current docstring in help browser
	QString _helpDoc;
};

#endif /*DOCGENERATOR_H_*/
