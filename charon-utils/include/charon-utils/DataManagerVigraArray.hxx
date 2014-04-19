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
/** \file DataManagerParameterFile.hxx
 *  Data manager based on parameter files.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 11.01.2011
 */

#ifndef DATAMANAGER_VIGRAARRAY_HXX
#define DATAMANAGER_VIGRAARRAY_HXX

#include <charon-core/Slots.hxx>
#include <charon-core/FileTool.h>
#include <vigra/multi_array.hxx>
#include <vigra/hdf5impex.hxx>

namespace DataManagers {
	/// parameter file based implementation of a data manager
	template <typename T>
	class DataManagerVigraArray : public Slot::DataManager<T> {
	private:
		/// paraneter file name
		static const std::string _filename;
		/// parameter name in the file
		std::string _dSetName;
	public:
		/// create manager
		/** \param slot   slot generate manager for
		 *  \param config config string
		 */
		DataManagerVigraArray(
			const Slot& slot, const std::string& config = "");
		virtual void setData(const T& data);
		virtual T getData();
		virtual std::string getConfig() const;
	};
}

template<typename T>
const std::string DataManagers::DataManagerVigraArray<T>::_filename
	= "lastRunVigraCache.h5";

template<typename T>
DataManagers::DataManagerVigraArray<T>::DataManagerVigraArray(
	const Slot& slot, const std::string& config) {
	if (config.size()) {
		_dSetName = config;
	}
	else {
		_dSetName = slot.getParent().getName() + "/" + slot.getName();
	}
}

template<typename T>
void DataManagers::DataManagerVigraArray<T>::setData(const T& data) {
	vigra::HDF5File file(_filename,vigra::HDF5File::Open);
	file.write(_dSetName, data);
}

template<typename T>
T DataManagers::DataManagerVigraArray<T>::getData() {
	vigra::HDF5File file(_filename,vigra::HDF5File::Open);
	const vigra::ArrayVector<hsize_t>& dShape = file.getDatasetShape(_dSetName);
	typename T::difference_type aShape;
	for (int ii=0; ii < T::actual_dimension; ii++) {
		aShape[ii] = dShape[ii];
	}
	T data(aShape);
	file.read(_dSetName,data);
	return data;
}

template<typename T>
std::string DataManagers::DataManagerVigraArray<T>::getConfig() const {
	return _dSetName;
}

/// \name factory specializations using the DataManagerParameterFile
// \{
template<>
Slot::DataManager< vigra::MultiArray<5,double> >*
	Slot::DataManagerFactory< vigra::MultiArray<5,double> >::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerVigraArray<
			vigra::MultiArray<5,double> >(slot, config);
}

template<>
Slot::DataManager< vigra::MultiArray<5,float> >*
	Slot::DataManagerFactory< vigra::MultiArray<5,float> >::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerVigraArray<
			vigra::MultiArray<5,float> >(slot, config);
}

template<>
Slot::DataManager< vigra::MultiArray<5,int> >*
	Slot::DataManagerFactory< vigra::MultiArray<5,int> >::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerVigraArray<
			vigra::MultiArray<5,int> >(slot, config);
}
// \}

#endif // DATAMANAGER_VIGRAARRAY_HXX
