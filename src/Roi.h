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
/** @file Roi.h
 *  Declaration of template class Roi.
 *
 *  This file does only contain the interface. Implementation of the template
 *  functions is in Roi.hxx. Use the hxx file if you want to use Roi.
 *
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 21.04.2009
 */

#ifndef _Roi_H_
#define _Roi_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef roi_EXPORTS
///Visual C++ specific code
#define roi_DECLDIR __declspec(dllexport)
#else
#define roi_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define roi_DECLDIR
#endif

#include "ParameteredObject.hxx"
#include <iostream>

/// ROI - Region of Interest.
/// This class can be used to select the parts of images/image sequences
/// where you are interested in.
template<typename T>
class roi_DECLDIR Roi: public TemplatedParameteredObject<T> {
private:
	/// Initialize this object.
	/// To be called from various constructors.
	/// This assigns the parameters to the parametered object.
	void _init();

	/// dummy function to get copy constructor compiled into dll
	Roi<T> createCopy(const Roi<T>& rhs) const;

public:
	///\name roi borders, "begin" is included, "end" is past the last element
	//@{
	Parameter<T> xBegin;
	Parameter<T> yBegin;
	Parameter<T> zBegin;
	Parameter<T> tBegin;
	Parameter<T> vBegin;
	Parameter<T> xEnd;
	Parameter<T> yEnd;
	Parameter<T> zEnd;	
	Parameter<T> tEnd;
	Parameter<T> vEnd;	
	//@}

	OutputSlot<Roi<T>*> out; ///< roi output slot

	/// Create empty Roi with default limits.
	/// @param name         roi name
	Roi(std::string name = "");

	/// Create Roi and load values from parameter file.
	/// @param pf           parameter file to load from
	/// @param name         roi name
	Roi(const ParameterFile& pf, std::string name = "");

	/// Create Roi with the given limits.
	/// @param xBegin,xEnd  x limits
	/// @param yBegin,yEnd  y limits
	/// @param zBegin,zEnd  z limits
	/// @param tBegin,tEnd  t limits
	/// @param vBegin,vEnd	channel limits
	/// @param name         roi name
	Roi(const T& xBegin, const T& xEnd,
		const T& yBegin = T(0), const T& yEnd = T(1),
		const T& zBegin = T(0), const T& zEnd = T(1),
		const T& tBegin = T(0), const T& tEnd = T(1),
		const T& vBegin = T(0), const T& vEnd = T(1),
		std::string name = "");

	/// Copy constructor
	Roi(const Roi<T>& rhs /** [in] copy source */);

	virtual ~Roi();

	T getWidth()    const; ///< get region width
	T getHeight()   const; ///< get region height
	T getDepth()    const; ///< get region depth
	T getDuration() const; ///< get region duration
	T getChannels()	const; ///< get region channel number

	/// Set region width.
	/// Modifies xEnd, keeps xBegin.
	/// @param w            new width
	void setWidth(T w);

	/// Set region height.
	/// Modifies yEnd, keeps yBegin.
	/// @param h            new height
	void setHeight(T h);

	/// Set region depth.
	/// Modifies zEnd, keeps zBegin.
	/// @param d            new depth
	void setDepth(T d);

	/// Set region length.
	/// Modifies tEnd, keeps tBegin.
	/// @param d            new duration
	void setDuration(T d);

	/// Set channel number.
	/// Modifies vEnd, keeps vBegin.
	/// @param v			new channel number
	void setChannels(T v);

	/// get region volume
	T getVolume() const;

	/// assign region limits
	/// @param xBegin,xEnd  x limits
	/// @param yBegin,yEnd  y limits
	/// @param zBegin,zEnd  z limits
	/// @param tBegin,tEnd  t limits
	/// @param vBegin,vEnd	channel limits
	void assign(const T& xBegin, const T& xEnd,
		const T& yBegin = T(0), const T& yEnd = T(1),
		const T& zBegin = T(0), const T& zEnd = T(1),
		const T& tBegin = T(0), const T& tEnd = T(1),
		const T& vBegin = T(0), const T& vEnd = T(1));

	/// Assign values from another region.
	/// This does copy the region limits.
	/// @param rhs          copy source
	Roi<T>& operator=(const Roi<T>& rhs);

	/// Enlarge region so that all elements of this region and
	/// of the rhs are contained. (The cubic structure is kept, so that
	/// more than the mathematical union is contained after this operation.)
	/// @param rhs          right hand side
	void unionWith(const Roi<T>& rhs);

	/// Shrink region so that only elements of this region that are also
	/// element of the rhs are contained.
	/// @param rhs          right hand side
	void intersectionWith(const Roi<T>& rhs);

	/// Check if a given point is inside the region.
	/// \param x,y,z,t,v	point coordinates
	bool isInside(T x, T y = 0, T z = 0, T t = 0, T v = 0) const;

	/// Load region parameters from the given parameter file.
	/// @param pf           ParameterFile to load from
	virtual void loadParameters(const ParameterFile& pf);

	/// print roi to given stream
	void print(std::ostream& strm = sout) const;
};

/// Simplify roi output to some std::ostream.
/// @param strm             output stream
/// @param roi              roi instance to print
template<typename T>
roi_DECLDIR std::ostream& operator<<(std::ostream& strm, const Roi<T>& roi);

/// for loop over the x dimension
#define forRoiX(roi,x)		for(int x=(int)((roi).xBegin); x<(int)((roi).xEnd); ++x)
/// for loop over the y dimension
#define forRoiY(roi,y)		for(int y=(int)((roi).yBegin); y<(int)((roi).yEnd); ++y)
/// for loop over the z dimension
#define forRoiZ(roi,z)		for(int z=(int)((roi).zBegin); z<(int)((roi).zEnd); ++z)
/// for loop over the t dimesnion
#define forRoiT(roi,t)		for(int t=(int)((roi).tBegin); t<(int)((roi).tEnd); ++t)
/// for loop over the v dimension
#define forRoiV(roi,v)		for(int v=(ind)((roi).vBegin); v<(int)((roi).vEnd); ++v)
/// for loop over the xy plane
#define forRoiXY(roi,x,y)    forRoiY(roi,y) forRoiX(roi,x)
/// for loop over the xz plane
#define forRoiXZ(roi,x,z)    forRoiZ(roi,z) forRoiX(roi,x)
/// for loop over the xt plane
#define forRoiXT(roi,x,t)    forRoiT(roi,t) forRoiX(roi,x)
/// for loop over the yz plane
#define forRoiYZ(roi,y,z)    forRoiZ(roi,z) forRoiY(roi,y)
/// for loop over the yt plane
#define forRoiYT(roi,y,t)    forRoiT(roi,t) forRoiY(roi,y)
/// for loop over the zt plane
#define forRoiZT(roi,z,t)    forRoiT(roi,t) forRoiZ(roi,z)
/// for loop over the xyz volume
#define forRoiXYZ(roi,x,y,z) forRoiZ(roi,z) forRoiXY(roi,x,y)
/// for loop over the xyzt volume
#define forRoiXYZT(roi,x,y,z,t) forRoiT(roi,t) forRoiXYZ(roi,x,y,z)
/// for loop over the xyztv volume
#define forRoiXYZTV(roi,x,y,z,t,v) forRoiV(roi,v) forRoiXYZT(roi,x,y,z,t)

#endif // _Roi_H_
