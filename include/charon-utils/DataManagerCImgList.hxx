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
/** \file DataManagerParameterFile.hxx
 *  Data manager based on parameter files.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.01.2011
 */

#ifndef DATAMANAGER_CIMGLIST_HXX
#define DATAMANAGER_CIMGLIST_HXX

#include "charon-core/Slots.hxx"
#include "charon-core/FileTool.h"
#include "CImg.h"

namespace DataManagers {
	/// parameter file based implementation of a data manager
	template <typename T>
	class charon_core_LOCAL DataManagerCImgList :
			public Slot::DataManager<T> {
	private:
		/// cimglist file name
		std::string _filename;
	public:
		/// create manager
		/** \param slot   slot generate manager for
		 *  \param config config string
		 */
		DataManagerCImgList(
			const Slot& slot, const std::string& config = "");
		virtual void setData(const T& data);
		virtual T getData();
		virtual std::string getConfig() const;
	};
}

template<typename T>
DataManagers::DataManagerCImgList<T>::DataManagerCImgList(
	const Slot& slot, const std::string& config) {
	if (config.size()) {


		_filename = config;//+".cimg";
	}
	else {
		_filename = slot.getParent().getName()+"-"+slot.getName()+".cimg";
	}
}

template<typename T>
void DataManagers::DataManagerCImgList<T>::setData(
		const T& data) {
	data.save_cimg(_filename.c_str());
}

template<typename T>
T DataManagers::DataManagerCImgList<T>::getData() {
	T data;
	data.load_cimg(_filename.c_str());
	return data;
}

template<typename T>
std::string DataManagers::DataManagerCImgList<T>::getConfig() const {
	return _filename;
}

/// \name factory specializations using the DataManagerParameterFile
// \{
template<>
Slot::DataManager< cimg_library::CImgList<int> >*
		Slot::DataManagerFactory< cimg_library::CImgList<int> >::getManager(
			const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerCImgList<
			cimg_library::CImgList<int> >(slot, config);
}

template<>
Slot::DataManager< cimg_library::CImgList<float> >*
		Slot::DataManagerFactory< cimg_library::CImgList<float> >::getManager(
			const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerCImgList<
			cimg_library::CImgList<float> >(slot, config);
}

template<>
Slot::DataManager< cimg_library::CImgList<double> >*
		Slot::DataManagerFactory< cimg_library::CImgList<double> >::getManager(
			const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerCImgList<
			cimg_library::CImgList<double> >(slot, config);
}
// \}

#endif // DATAMANAGER_CIMGLIST_HXX
