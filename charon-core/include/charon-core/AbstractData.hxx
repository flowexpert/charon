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
/** \file AbstractData.hxx
 *  Declaration and implementation of the abstract class
 *  AbstractData.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *
 *  \date 10.04.2009
 */

#include <vector>

#ifndef _ABSTRACT_DATA_HXX_
#define _ABSTRACT_DATA_HXX_

/// Interface for data acces.
/** This is the common interface for parameters and
 *  slots.  Data access is only provided read only.
 */
template<typename T>
class AbstractROData {
public:
	virtual ~AbstractROData() {}

	/// Cast operator to get data (get copy)
	/** \returns data content (copy) */
	virtual operator T() const = 0;

	/// Call operator.
	/** \returns data content (const reference, no copy) */
	virtual const T& operator()() const = 0;
};

/// Interface for data assignment and readout.
/** This is the common interface for parameters and
 *  output slots. (Input slots are read only.)
 */
template<typename T>
class AbstractData : public AbstractROData<T> {
public:
	virtual ~AbstractData() {}

	/// data assignment operator
	/** \param B         data to assign
	 *  \returns updated object
	 */
	virtual T& operator=(const T& B) = 0;

	/// Call operator.
	/** Non const version
	 *  \returns data content (no copy) */
	virtual T& operator()() = 0;
};

/// Common interface for read-only access to multiple data members
/// like arrays etc.
template<typename T>
class AbstractMultiROData {
public:
	virtual ~AbstractMultiROData() {}

	/// Access to specific member (read-only).
	/** \param pos              data position
	 *  \returns data content at given position (read-only, no copy)
	 */
	virtual const T& operator[](std::size_t pos) const = 0;

	/// Access to number of members.
	/** For iterations using operator[]
	 *  \returns number of elements
	 */
	virtual std::size_t size() const = 0;
};

/// Common interface for read-write access to multiple data members
/// like arrays etc.
template<typename T>
class AbstractMultiData : public AbstractMultiROData<T> {
public:
	virtual ~AbstractMultiData() {}

	/// Access to specific member (read-write).
	/** \param pos              data position
	 *  \returns data content at given position (read-write, no copy)
	 */
	virtual T& operator[](std::size_t pos) = 0;
};

#endif // _ABSTRACT_DATA_HXX_

