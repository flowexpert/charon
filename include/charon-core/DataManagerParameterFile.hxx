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

#ifndef DATAMANAGERPARAMETERFILE_HXX
#define DATAMANAGERPARAMETERFILE_HXX

#include "Slots.hxx"
#include "FileTool.h"

namespace DataManagers {
	/// parameter file based implementation of a data manager
	template <typename T>
	class charon_core_LOCAL DataManagerParameterFile :
			public Slot::DataManager<T> {
	private:
		/// parameter file to store parameter
		ParameterFile pf;
		/// paraneter file name
		static const std::string filename;
		/// parameter name in the file
		std::string paramName;
	public:
		/// create manager
		/** \param slot   slot generate manager for
		 *  \param config config string
		 */
		DataManagerParameterFile(
			const Slot& slot, const std::string& config = "");
		virtual void setData(const T& data);
		virtual T getData();
		virtual std::string getConfig() const;
	};
}

template<typename T>
const std::string DataManagers::DataManagerParameterFile<T>::filename
	= "lastRunParameterCache.wrp";

template<typename T>
DataManagers::DataManagerParameterFile<T>::DataManagerParameterFile(
	const Slot& slot, const std::string& config) {
	if (!FileTool::exists(filename)) {
		pf.save(filename);
	}
	if (config.size()) {
		paramName = config;
	}
	else {
		paramName = slot.getParent().getName() + "." + slot.getName();
	}
}

template<typename T>
void DataManagers::DataManagerParameterFile<T>::setData(const T& data) {
	pf.load(filename);
	pf.set<T>(paramName, data);
	pf.save(filename);
	pf.clear();
}

template<typename T>
T DataManagers::DataManagerParameterFile<T>::getData() {
	pf.load(filename);
	return pf.get<T>(paramName);
	pf.clear();
}

template<typename T>
std::string DataManagers::DataManagerParameterFile<T>::getConfig() const {
	return paramName;
}

/// \name factory specializations using the DataManagerParameterFile
// \{
template<>
Slot::DataManager<int>* Slot::DataManagerFactory<int>::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerParameterFile<int>(slot, config);
}

template<>
Slot::DataManager<float>* Slot::DataManagerFactory<float>::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerParameterFile<float>(slot, config);
}

template<>
Slot::DataManager<double>* Slot::DataManagerFactory<double>::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerParameterFile<double>(slot, config);
}

template<>
Slot::DataManager<std::string>* Slot::DataManagerFactory<std::string>::getManager(
		const Slot& slot, const std::string& config) {
	return new DataManagers::DataManagerParameterFile<std::string>(slot, config);
}
// \}

#endif // DATAMANAGERPARAMETERFILE_HXX
