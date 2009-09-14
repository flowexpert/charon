/*  Copyright (C) 2009 Jens-Malte Gottfried

    This file is part of Tuchulcha.
    
    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   MetaData.h
 *  @brief  Implementation of class ParameterFileModel.
 *  @date   10.10.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef METADATA_H_
#define METADATA_H_

#include <string>
#include <vector>

class ParameterFile;

/// Handle metadata management for ParameterFileModel classes.
class MetaData
{
public:
    /// Load metadata from given file
    MetaData(const std::string& fileName);
    virtual ~MetaData();

    /// Check metadata status
    bool valid() const;

    /// @name Data queries
    //  @{
    /// get classes contained in this metadata file
    const std::vector<std::string>& getClasses() const;

    /// get input slots of some given object
    /// @param className        object class name
    ///                         (use ParameterFileModel::getClass to get this)
    std::vector<std::string> getOutputs(const std::string& className) const;

    /// get output slots of some given object
    /// @param className        object class name
    ///                         (use ParameterFileModel::getClass to get this)
    std::vector<std::string> getInputs(const std::string& className) const;

    /// get parameters of some given object
    /// @param className        object class name
    ///                         (use ParameterFileModel::getClass to get this)
    std::vector<std::string> getParameters(const std::string& className) const;

    /// Get type of some parameter or slot.
    /// Use GraphModel::getType if there are template arguments and you need to
    /// know the type in the specific instance.
    /// @param parName          name of the parameter
    /// @param className        object class name
    ///                         (use ParameterFileModel::getClass to get this)
    std::string getType(std::string parName,
                                           const std::string& className) const;

    /// get default value for some editable parameter
    /// @param parName          name of the parameter
    /// @param className        object class name
    /// @return                 empty string, if no default given
    std::string getDefault(std::string parName,
                                           const std::string& className) const;

    /// get docstring for some parameter or some class.
    /// @param parName          name of the parameter
    /// @param className        object class name
    /// @return                 empty string, if no docstring given
    std::string getDocString(std::string parName,
                                           const std::string& className) const;

    /// get filename for some parameter or class documentation.
    /// @param parName          name of the parameter
    /// @param className        object class name
    /// @return                 empty string, if no docfile given
    std::string getDocFile(std::string parName,
                                           const std::string& className) const;

    /// Check if some parameter/slot is a parameter.
    /// @param  name            name of the parameter/slot
    /// @param  className       object class name
    bool isParameter(std::string name,
                                           const std::string& className) const;

    /// Check if some parameter/slot is an input slot.
    /// @param  name            name of the slot
    /// @param  className       object class name
    bool isInputSlot(std::string name,
                                           const std::string& className) const;

    /// Check if some parameter/slot is an output slot.
    /// @param  name            name of the slot
    /// @param  className       object class name
    bool isOutputSlot(std::string name,
                                           const std::string& className) const;

    /// Check if some slot is optional.
    /// @param  slotName        name of the slot
    /// @param  className       object class name
    /// @throws std::string     error message if slot is invalid
    ///                         (i.e. does not exists, etc.)
    bool isOptionalSlot(std::string slotName,
                                           const std::string& className) const;

    /// Check if some slot is a multi slot.
    /// @param  slotName        name of the slot
    /// @param  className       object class name
    /// @throws std::string     error message if slot is invalid
    ///                         (i.e. does not exists, etc.)
    bool isMultiSlot(std::string slotName,
                                           const std::string& className) const;
    //  @}  // queries

private:
    /// check metadata file
    /// @throws std::string     error message
    void _check();

    /// contained classes
    std::vector<std::string> _classes;

    /// used ParameterFile (read only)
    const ParameterFile* _data;
    /// validity status
    bool _valid;
};

#endif /*METADATA_H_*/
