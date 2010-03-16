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
/** @file SplitStream.h
 *  Declaration of class SplitStream.
 *  @author  <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date    11.08.2008
 *  @remark  Used Interface from SplitStream Class of Daniel Kondermann
 */

#ifndef _SPLIT_STREAM_H_
#define _SPLIT_STREAM_H_

#ifdef WINDOWS
#ifdef DLLEX
#undef DLLEX
#endif // DLLEX
#ifdef CREATE_SHARED
#define DLLEX __declspec(dllexport)
#else  // CREATE_SHARED
#define DLLEX __declspec(dllimport)
#endif // CREATE_SHARED
#else  // WINDOWS
#define DLLEX
#endif // WINDOWS

#include <iostream>
#include <vector>

/** Buffer to split output on several streams.
 */
class DLLEX SplitStreamBuf : public std::streambuf {
private:
	/// pointer to stream buffers
	std::vector<std::streambuf*> buffers_;
	/// forbid copying
	SplitStreamBuf(SplitStreamBuf const &);
	/// forbid assignment
	void operator= (SplitStreamBuf const &);

public:
	/// Constructor initializing the buffers array
	SplitStreamBuf(const std::vector<std::streambuf*>& buffers);
	virtual ~SplitStreamBuf();

	/** Write character in the case of overflow.
	 *  @param c        char value to check
	 *  @return         EOF if overflow occured
	 */
	int overflow(int c);

	/** Write sequence of characters.
	 *  @param str      char array to put
	 *  @param size     number of characters
	 *  @return         number of written characters
	 */
	std::streamsize xsputn(char const * str, std::streamsize size);

	/** Sync stream buffers.
	 *  @return         EOF on failure
	 */
	int sync();
};

/** This class wraps a list of output streams and
 *  pipes output made to a splitstream instance to
 *  all of these output streams.
 */
class DLLEX SplitStream : public std::ostream {
public:
	/// Default constructor
	/** Use no output stream.<br>
	 *  Make sure you call assign() before using this SplitStream.
	 */
	SplitStream();

	/** Assign one stream to output list.
	 *  @param stream       stream to pipe output to
	 */
	SplitStream(std::ostream& stream);

	/** Assign two streams to output list.
	 *  @param stream1      fist output stream to assign
	 *  @param stream2      second stream to assign
	 */
	SplitStream(std::ostream& stream1, std::ostream& stream2);

	/** Assing a whole list of streams.
	 *  @param streamList   list of streams to assign
	 */
	SplitStream(std::vector<std::ostream*>& streamList);

	/// Default destructor
	virtual ~SplitStream();

	///@name Stream assignment (see constructors for details)
	//@{
	void assign(std::ostream& stream = std::cout);
	void assign(std::ostream& stream1, std::ostream &stream2);
	void assign(std::vector<std::ostream*>& streamsList);
	//@}

	/** Dummy function to fulfill interface
	 *  \deprecated         This function is not used in current plugins,
	 *                      check if it's still needed.
	 *  \return             always true
	 */
	bool isZeroRank();

private:
	std::vector<std::streambuf*> buffers_;	///< buffer array
	SplitStreamBuf* buffer_;			///< pointer to output stream buffer

	/**	Set new buffers.
	 *	@param buffers		new buffers to set
	 */
	void updateBuf(std::vector<std::streambuf*> buffers);
};

/// Dummy instance for usage in other files (for interface too).
extern DLLEX SplitStream sout;

#endif /* _SPLIT_STREAM_H_ */

