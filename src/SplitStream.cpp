/*  This file is part of Charon.

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
/**	@file SplitStream.cpp
 *	Implementation of class SplitStream.
 *	@date 11.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "SplitStream.h"
#include <cassert>
#include <climits>
#include <cstdio>

SplitStreamBuf::SplitStreamBuf(const std::vector<std::streambuf*>& buffers) {
	if (buffers.size() < 1)
		throw "Buffer must be initialized with at least one streambuf!";
	buffers_ = buffers;
}

SplitStreamBuf::~SplitStreamBuf() {
}

int SplitStreamBuf::overflow(int c) {
	if (c != EOF){
		bool over = false;
		char const cchar = traits_type::to_char_type(c);
		for (unsigned int i=0; i < buffers_.size(); i++)
			over = over || (buffers_[i]->sputc(cchar) == EOF);
		if (over)
			return EOF;
	}
	return traits_type::not_eof(c);
}

int SplitStreamBuf::xsputn(char const* str, int size) {
	unsigned int min = UINT_MAX;
	for(unsigned int i=0; i < buffers_.size(); i++) {
		unsigned int temp = buffers_[i]->sputn(str, size);
		if (temp < min)
			min = temp;
	}
	return min;
}

int SplitStreamBuf::sync() {
	bool fail = false;
	for (unsigned int i=0; i<buffers_.size(); i++)
		fail = fail || ( buffers_[i]->pubsync() == -1 );

	if (fail)
		return -1;
	return 0;
}

SplitStream::SplitStream(std::ostream& stream) :
		std::ostream(std::cout.rdbuf()) {
	buffers_.push_back(stream.rdbuf());
	assert(buffers_.size() == 1);
	buffer_ = new SplitStreamBuf(buffers_);
	rdbuf(buffer_);
	assert(rdbuf() == buffer_);
}

SplitStream::SplitStream(std::ostream& stream1, std::ostream& stream2) :
		std::ostream(std::cout.rdbuf()) {
	buffers_.push_back(stream1.rdbuf());
	buffers_.push_back(stream2.rdbuf());
	assert(buffers_.size() == 2);
	buffer_ = new SplitStreamBuf(buffers_);
	rdbuf(buffer_);
	assert(rdbuf() == buffer_);
}

SplitStream::SplitStream(std::vector<std::ostream*>& streamList) :
		std::ostream(std::cout.rdbuf()) {
	for(unsigned int i=0; i<streamList.size();i++)
		buffers_.push_back(streamList[i]->rdbuf());
	assert(buffers_.size() == streamList.size());
	buffer_ = new SplitStreamBuf(buffers_);
	rdbuf(buffer_);
	assert(rdbuf() == buffer_);
}

SplitStream::~SplitStream(){
	delete buffer_;
}

void SplitStream::updateBuf(std::vector<std::streambuf*> buffers){
	SplitStreamBuf* temp = new SplitStreamBuf(buffers);
	rdbuf(temp);
	delete buffer_;
	buffer_ = temp;
	assert(rdbuf() == buffer_);
}

void SplitStream::assign(std::ostream& stream){
	buffers_.clear();
	buffers_.push_back(stream.rdbuf());
	assert(buffers_.size() == 1);
}

void SplitStream::assign(std::ostream& stream1, std::ostream &stream2){
	buffers_.clear();
	buffers_.push_back(stream1.rdbuf());
	buffers_.push_back(stream2.rdbuf());
	assert(buffers_.size() == 2);
	updateBuf(buffers_);
}

void SplitStream::assign(std::vector<std::ostream*>& streamList){
	buffers_.clear();
	for(unsigned int i=0; i<streamList.size();i++)
		buffers_.push_back(streamList[i]->rdbuf());
	assert(buffers_.size() == streamList.size());
	updateBuf(buffers_);
}

bool SplitStream::isZeroRank(){
	return true;
}

SplitStream sout;
