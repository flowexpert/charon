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
			"read image from image file using cimg<br>"
			"Native supported file types:<ul>"
			"<li>BMP</li>"
			"<li>HDR (Radiance)</li>"
			"<li>TXT/ASC (comma separated, one line per scanline)</li>"
			"<li>PPM/PGM</li>"
			"<li>CIMG</li>"
			"</ul><br>To support the following formats, additional libaries need to be linked:<ul>"
			"<li>PNG (libpng)</li>"
			"<li>TIFF (libtiff)</li>"
			"<li>JPEG/JPG(libjpeg)</li>"
			"</ul>"),
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

//utility functions for reading radiance hdr images
//hdr code by Igor Kravtchenko (igor@obraz.net)
namespace hdr {

	int  MINELEN	= 8;		// minimum scanline length for encoding
	int  MAXELEN	= 0x7fff;	// maximum scanline length for encoding

	int R = 0;
	int G = 1;
	int B = 2;
	int E = 3;
	typedef unsigned char RGBE[4];

	static void workOnRGBE(RGBE *scan, int len, float *cols);
	static bool decrunch(RGBE *scanline, int len, FILE *file);
	static bool oldDecrunch(RGBE *scanline, int len, FILE *file);

	float convertComponent(int expo, int val)
	{
		float v = val / 256.0f;
		float d = (float) pow(2.0f, float(expo));
		return v * d;
	}

	void workOnRGBE(RGBE *scan, int len, float *cols)
	{
		while (len-- > 0) {
			int expo = scan[0][E] - 128;
			cols[0] = convertComponent(expo, scan[0][R]);
			cols[1] = convertComponent(expo, scan[0][G]);
			cols[2] = convertComponent(expo, scan[0][B]);
			cols += 3;
			scan++;
		}
	}

	bool decrunch(RGBE *scanline, int len, FILE *file)
	{
		int  i, j;
					
		if (len < MINELEN || len > MAXELEN)
			return oldDecrunch(scanline, len, file);

		i = fgetc(file);
		if (i != 2) {
			fseek(file, -1, SEEK_CUR);
			return oldDecrunch(scanline, len, file);
		}

		scanline[0][G] = fgetc(file);
		scanline[0][B] = fgetc(file);
		i = fgetc(file);

		if (scanline[0][G] != 2 || scanline[0][B] & 128) {
			scanline[0][R] = 2;
			scanline[0][E] = i;
			return oldDecrunch(scanline + 1, len - 1, file);
		}

		// read each component
		for (i = 0; i < 4; i++) {
			for (j = 0; j < len; ) {
				unsigned char code = fgetc(file);
				if (code > 128) { // run
					code &= 127;
					unsigned char val = fgetc(file);
					while (code--)
						scanline[j++][i] = val;
				}
				else  {	// non-run
					while(code--)
						scanline[j++][i] = fgetc(file);
				}
			}
		}

		return feof(file) ? false : true;
	}

	bool oldDecrunch(RGBE *scanline, int len, FILE *file)
	{
		int i;
		int rshift = 0;
	
		while (len > 0) {
			scanline[0][R] = fgetc(file);
			scanline[0][G] = fgetc(file);
			scanline[0][B] = fgetc(file);
			scanline[0][E] = fgetc(file);
			if (feof(file))
				return false;

			if (scanline[0][R] == 1 &&
				scanline[0][G] == 1 &&
				scanline[0][B] == 1) {
				for (i = scanline[0][E] << rshift; i > 0; i--) {
					memcpy(&scanline[0][0], &scanline[-1][0], 4);
					scanline++;
					len--;
				}
				rshift += 8;
			}
			else {
				scanline++;
				len--;
				rshift = 0;
			}
		}
		return true;
	}
	

} // namespace hdr

template <typename T>
cimg_library::CImgList<T> FileReader<T>::_readHDR(const std::string& filename)
{
	int i;
	char str[200];
	FILE *file;

	file = fopen(filename.c_str(), "rb");
	if (!file)
		ParameteredObject::raise(std::string("Could not open ")+filename) ;

	fread(str, 10, 1, file);
	if (memcmp(str, "#?RADIANCE", 10)) {
		fclose(file);
		ParameteredObject::raise(filename + " is not an Radiance HDR file!") ;
	}

	fseek(file, 1, SEEK_CUR);

	char cmd[200];
	i = 0;
	char c = 0, oldc;
	while(true) {
		oldc = c;
		c = fgetc(file);
		if (c == 0xa && oldc == 0xa)
			break;
		cmd[i++] = c;
	}
	cimg_library::cimg::unused(cmd);

	char reso[200];
	i = 0;
	while(true) {
		c = fgetc(file);
		reso[i++] = c;
		if (c == 0xa)
			break;
	}

	long int w, h;
	if (!sscanf(reso, "-Y %ld +X %ld", &h, &w)) {
		fclose(file);
		ParameteredObject::raise("File could be corrupt or is no HDR file!") ;
	}

	float *data = new float[w * h * 3];
	float* cols = data ;
	hdr::RGBE *scanline = new hdr::RGBE[w];
	if (!scanline) {
		fclose(file);
		ParameteredObject::raise("File could be corrupt or is no HDR file!") ;
	}

	// convert image 
	for (int y = h - 1; y >= 0; y--) {
		if (hdr::decrunch(scanline, w, file) == false)
			break;
		hdr::workOnRGBE(scanline, w, cols);
		cols += w * 3;
	}

	delete [] scanline;
	fclose(file);

	///deinterleave rgb channels and copy to CImgList
	cimg_library::CImgList<T> result(3,w,h,1,1,0.0) ;
	float* src = data ;
	float* end = data + w * h * 3 ;
	T* dstR = result[0].data() ;
	T* dstG = result[1].data();
	T* dstB = result[2].data();

	while(src < end)
	{
		*(dstR++) = *(src++) ;
		*(dstG++) = *(src++) ;
		*(dstB++) = *(src++) ;
	}
	delete[] data ;
	return result ;
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
			cimg_library::CImgList<T> tmp ;
			if(cur.endsWith("hdr", Qt::CaseInsensitive))
				tmp = _readHDR(cur.toStdString());
			else
				tmp.load(cur.toLocal8Bit().constData());
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
