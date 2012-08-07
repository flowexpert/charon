/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file FileReader.hxx
/// Implementation of the parameter class FileReader.
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
/// @date 20.08.2009

#ifndef _FILEREADER_HXX_
#define _FILEREADER_HXX_

#include <QRegExp>
#include <QStringList>
#include "FileReader.h"

template <typename T>
FileReader<T>::FileReader(const std::string& name) :
		TemplatedParameteredObject<T>("FileReader", name,
			"read image from image file using cimg"),
		_fileReaderWatcher(0)
{
	ParameteredObject::_addParameter (filename, "filename",
		"Filename to read image from", "fileopen");

	ParameteredObject::_addParameter (watchable, "watchable",
		"if set, files are monitored for changes", false);

	ParameteredObject::_addParameter (fileList, "fileList",
		"If this is not empty, the list will be added to the CImgList "
		"according to the paramter value of frameDimension.<br>"
		"Mixes with filename.", "fileopen");

	ParameteredObject::_addParameter< std::string >(
		frameDimension, "frameDimension",
		"Depending on application conventions the list of frames "
		"will be stored in the CImgList-entries or one of the last "
		"two dimensions of a single CImg." , "list",
		"{list;z;c}");

	ParameteredObject::_addParameter(
		fileNameIsPattern, "fileNameIsPattern",
		"Is filename a pattern?<br/>"
		"Currently only a simple pattern with one integer is allowed, "
		"such as \"%6_0.pgm\".<br>"
		"Pattern Syntax is QRegExp(\"(.*)%([0-9]+)(.*)\").<br>"
		"Prefix and tail are kept, the number after the percent sign "
		"is the field width, fillup character is zero.",
		false);

	ParameteredObject::_addParameter(
		start, "start", "Pattern parameter start", 0u);

	ParameteredObject::_addParameter(
		step, "step", "Pattern parameter step", 1u);

	ParameteredObject::_addParameter(
		end, "end",
		"Pattern parameter end (using 'i < end' rather than 'i <=end')",
		1u);

	ParameteredObject::_addOutputSlot(out, "out",
		"image output", "CImgList<T>");
}

template <typename T>
FileReader<T>::~FileReader() {
	if (_fileReaderWatcher) {
		delete _fileReaderWatcher;
	}
}

template <typename T>
void FileReader<T>::_append(const cimg_library::CImgList<T>& tmp) {
	cimg_library::CImgList<T>& o = out();
	bool first = o.size() > 0;
	if (tmp.size() == 0) {
		// nothing to do
		return;
	}
	else if (frameDimension() == "list") {
		// add to list always works
		sout << "(DD) \t\tappending " << tmp.size()
				<< " entries to CImgList." << std::endl;
		o.push_back(tmp);
	}
	else if (tmp.size() > 1) {
		// add list to image is difficult (not implemented)
		ParameteredObject::raise(
			"Cannot append CImgList of length > 1 to dimension"
				+ frameDimension() + "! (not implemented)");
	}
	// from here on we have exactly one CImg
	else if (frameDimension() == "z" || frameDimension() == "c") {
		if (first) {
			o = tmp;
			sout << "(DD) first append: image size is (x,y,z,c)=("
					<< tmp[0].width() << ","
					<< tmp[0].height() << "," << tmp[0].depth() << ","
					<< tmp[0].spectrum() << ")" << std::endl;
		}
		else {
			if (!o[0].is_sameXY(tmp[0])) {
				sout << "(WW) image of different (x,y)-size: ("
						<< tmp[0].width() << ","
						<< tmp[0].height() << ")" << std::endl;
			}
			if (frameDimension() == "z" && !o[0].is_sameC(tmp[0])) {
				sout << "(WW) image of different spectrum "
						<< tmp[0].spectrum() << std::endl;
			}
			if (frameDimension() == "c" && !o[0].is_sameZ(tmp[0])) {
				sout << "(WW) image of different depth "
						<< tmp[0].depth() << std::endl;
			}
			try {
				o[0].append(tmp[0], frameDimension()[0]);
			}
			catch(const cimg_library::CImgException& err) {
				ParameteredObject::raise(
					std::string("Append failed:\n\t") + err.what());
			}
		}
	}
	else {
		ParameteredObject::raise(
			"Unknown frameDimension '"  + frameDimension() + "'!");
	}
}

template <typename T>
void FileReader<T>::_mem() {
	size_t cnt = 0;
	const cimg_library::CImgList<T>& o = out();
	cimglist_for(o, i) {
		cnt +=o[i].size()*sizeof(T);
	}
	sout << "(DD) \t\tcurrent memory consumption for output-Slot: "
		<< cnt/1024.0f/1024.0f << " MB" << std::endl;
}

template <typename T>
void FileReader<T>::execute() {
	if (filename() == "" && fileList.size() == 0) {
		ParameteredObject::raise(
			"filename or fileList or both have to be set!");
	}

	// determine files to load
	QStringList files;

	// filename with possible pattern matching
	if (!filename().empty()) {
		if (fileNameIsPattern()) {
			// parse pattern with regexp
			QString head, tail;
			uint width;
			QRegExp pattern("(.*)%(\\d+)(.*)");
			if (pattern.exactMatch(QString::fromStdString(filename()))) {
				head = pattern.cap(1);
				width = pattern.cap(2).toUInt();
				tail = pattern.cap(3);
			}
			else {
				ParameteredObject::raise(
					"invalid pattern: %[0-9]+ missing");
			}
			for(size_t i = start(); i < end(); i += step()) {
				files << QString("%1%2%3")
					.arg(head).arg(i,width,10,QChar('0')).arg(tail);
			}
		}
		else {
			files << QString::fromStdString(filename());
		}
	}
	// list of filenames
	for (size_t i=0; i < fileList.size(); ++i) {
		files << QString::fromStdString(fileList[i]);
	}

	// install watcher if neccessary
	if (watchable()) {
		if (!_fileReaderWatcher) {
			_fileReaderWatcher = new FileReaderWatcher(this);
			_fileReaderWatcher->addPaths(files);
		}
	}

	// load
	out().assign();
	try {
		foreach (QString cur, files) {
			sout << "(II) \tloading file: " << cur.toStdString() << std::endl;
			cimg_library::CImgList<T> tmp(cur.toLocal8Bit().constData());
			_append(tmp);
			_mem();
		}
	}
	catch (const cimg_library::CImgException& err) {
		ParameteredObject::raise(
			std::string("Could not read file\n\t") + err.what());
	}
}

#endif /* _FILEREADER_HXX_ */
