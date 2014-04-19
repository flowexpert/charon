/*  This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/**	@file SplitStream.cpp
 *	Implementation of class SplitStream.
 *	@date 11.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <charon-core/SplitStream.h>
#include <cassert>
#include <climits>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <sstream>

SplitStreamBuf::SplitStreamBuf(const std::vector<std::streambuf*>& buffers) {
	if (buffers.size() < 1) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Buffer must be initialized with at least one streambuf!";
		throw std::invalid_argument(msg.str().c_str());
	}
	_buffers = buffers;
}

SplitStreamBuf::~SplitStreamBuf() {
}

int SplitStreamBuf::overflow(int c) {
	if (c != EOF){
		bool over = false;
		char const cchar = traits_type::to_char_type(c);
		for (unsigned int i=0; i < _buffers.size(); i++)
			over = over || (_buffers[i]->sputc(cchar) == EOF);
		if (over)
			return EOF;
	}
	return traits_type::not_eof(c);
}

std::streamsize SplitStreamBuf::xsputn(char const* str, std::streamsize size) {
		std::streamsize min = std::numeric_limits<std::streamsize>::max();
	for(unsigned int i=0; i < _buffers.size(); i++) {
		std::streamsize temp = _buffers[i]->sputn(str, size);
		if (temp < min)
			min = temp;
	}
	return min;
}

int SplitStreamBuf::sync() {
	bool fail = false;
	for (unsigned int i=0; i<_buffers.size(); i++) {
		bool temp = ( _buffers[i]->pubsync() == -1 );
		fail = fail || temp;
	}

	if (fail)
		return -1;
	return 0;
}

SplitStream::SplitStream() :
#ifdef UNIX
		std::ostream(std::cout.rdbuf()),
#else
		std::ostream(std::_Uninitialized()),
#endif
	_buffers(0)
{
}

SplitStream::SplitStream(std::ostream& stream) :
		std::ostream(stream.rdbuf()) {
	_buffers.push_back(stream.rdbuf());
	assert(_buffers.size() == 1);
	_buffer = new SplitStreamBuf(_buffers);
	rdbuf(_buffer);
	assert(rdbuf() == _buffer);
}

SplitStream::SplitStream(std::ostream& stream1, std::ostream& stream2) :
		std::ostream(std::cout.rdbuf()) {
	_buffers.push_back(stream1.rdbuf());
	_buffers.push_back(stream2.rdbuf());
	assert(_buffers.size() == 2);
	_buffer = new SplitStreamBuf(_buffers);
	rdbuf(_buffer);
	assert(rdbuf() == _buffer);
}

SplitStream::SplitStream(std::vector<std::ostream*>& streamList) :
		std::ostream(std::cout.rdbuf()) {
	for(unsigned int i=0; i<streamList.size();i++)
		_buffers.push_back(streamList[i]->rdbuf());
	assert(_buffers.size() == streamList.size());
	_buffer = new SplitStreamBuf(_buffers);
	rdbuf(_buffer);
	assert(rdbuf() == _buffer);
}

SplitStream::~SplitStream() {
	if (_buffer) {
		delete _buffer;
	}
}

void SplitStream::updateBuf(std::vector<std::streambuf*> buffers){
	SplitStreamBuf* temp = new SplitStreamBuf(buffers);
	rdbuf(temp);
	if (_buffer) {
		delete _buffer;
	}
	_buffer = temp;
	assert(rdbuf() == _buffer);
}

void SplitStream::assign(std::ostream& stream){
	_buffers.clear();
	init(stream.rdbuf());
	_buffers.push_back(stream.rdbuf());
	assert(_buffers.size() == 1);
}

void SplitStream::assign(std::ostream& stream1, std::ostream &stream2){
	_buffers.clear();
	init(stream1.rdbuf());
	_buffers.push_back(stream1.rdbuf());
	_buffers.push_back(stream2.rdbuf());
	assert(_buffers.size() == 2);
	updateBuf(_buffers);
}

void SplitStream::assign(std::vector<std::ostream*>& streamList){
	_buffers.clear();
	if (streamList.size() < 1) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "You have to assign at least one output stream!";
		throw std::invalid_argument(msg.str().c_str());
	}
	init(streamList[0]->rdbuf());
	for(unsigned int i=0; i<streamList.size();i++)
		_buffers.push_back(streamList[i]->rdbuf());
	assert(_buffers.size() == streamList.size());
	updateBuf(_buffers);
}

SplitStream sout;
