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
/** @file ParameterFile.h
 *  Declaration of class ParameterFile.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">
 *     Cornelius Ratsch</a>
 *
 *  Original file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried.
 *
 *  @date 23.05.2009
 *
 *  This file does only contain the interface! In most cases, you want to
 *  include ParameterFile.hxx which does contain the template instantiations
 *
 *  Changes:
 *  -   Added some useful default values to get and set functions
 *  -   Added possibilty to remove parameters.
 *  -   Removed setting of default values on get-functions
 *  -   added const version of get-functions
 *  -   throwing exceptions when erase called with an unset parameter name
 *  -   added parName and objName to get parts of parameter descriptors
 */
#ifndef _ParameterFile_H
#define _ParameterFile_H

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

#include <vector>
#include <map>
#include <iostream>

/** This class serves to store parameters used within the Charon Project.
 *  Parameters of different types can be stored and loaded from a plain text
 *  file.
 *
 *  The parameters are stored in an std::map<std::string, std::string>.
 *  So you can only store parameter types which can be transformed
 *  into string representations (using the ostream operator<<())
 *  and be restored (using the istream operator>>()). A second map
 *  serves to save information which parameters have been newly set
 *  or modified. These information can be retrieved using showSetParams().
 *  All other function operate only on the real parameter store.
 *  You can reset this second list (without losing the stored parameters)
 *  by calling resetSetParams().
 *
 *  @remark The names of the parameters are case insensitive.
 *  @remark You can reassign SplitStream sout to redirect debugging
 *          messages into logfiles etc.
 */
class DLLEX ParameterFile
{
private:
    /** Store a string value to the parameter list.
     *  @param  parameter   Name of the parameter to store
     *  @param  value       String representation of the value.
     *  @throw  std::string error-string when called with empty parameter name
     *  @remark             Modified parameter (when overwriting an existing
     *                      value) is marked as modified. */
    void _set(std::string parameter, std::string value);

    /** Get the lowercase version of a string.
     * @param  input        Input string
     * @return              string transformed to lowercase */
    inline void _toLower(std::string& input) const;

    /// @name Parameter store
    //  @{

    /// this vector keeps a copy of all parameters in order to preserve their
    /// ordering in the actual file	it is used for saving the file in the same
    /// order and appending new parameters at the end of the file
    std::vector<std::string> _parameterLines;

    /// maps parameters to their respective values
    std::map<std::string, std::string> _params;

    /// stores modified and new parameters in a seperate map
    /// (see showSetParams())
    std::map<std::string, std::string> _setParams;

    //  @}

    /// @name Properties to change behaviour
    //  @{

    /// warn if parameter was not found
    bool _noFoundWarnings;

    /// convert linux to windows slashes and vice versa
    /// (depends on define CHARON_LINUX/CHARON_WINDOWS in StringTool)
    bool _convertSlashes;

    /// delimiter for lists of values (default is ';')
    char _delimiter;

    //  @}

public:
    /// Default constructor
    ParameterFile();

    /// Initialize Parameterfile reading content of an existing file.
    /// @param  fileName    Name of the plaintext-file to read.
    ParameterFile(std::string fileName);

    /// Default destructor
    ~ParameterFile();

    /// get parameter component of a full name
    /// @param  fullName    full name descriptor (e.g. "obj1.par1")
    /// @return             parameter name part  (e.g. "par1")
    static std::string parName(std::string fullName);

    /// get object name component of a full name
    /// @param  fullName    full name descriptor (e.g. "obj1.par1")
    /// @return             object name part     (e.g. "obj1")
    static std::string objName(std::string fullName);

    /// Change property delimiter
    /// @param  delimiter       New value
    void setDelimiter(char delimiter);

    /// Change property convertSlashes
    /// @param convertSlashes	New value
    void setConvertSlashes(bool convertSlashes);

    /// Check if a givem parameter has already been set.
    /// @param  parameter       Name of the parameter to check
    /// @return                 True if value already present
    /// @throws std::string     error-string when called with empty
    ///                         parameter name
    bool isSet(std::string parameter) const;

    /// Set a parameter to the given single value.
    /// @param  parameter   Name of the parameter to set
    /// @param  value       Value to set
    template<class T>
    void set(std::string parameter, const T& value = T());

    /// Set a parameter to multiple values.
    /// Use getList to restore this list of values.
    /// @param  parameter   Name of the parameter to set
    /// @param  value       List containing the values to set
    template<class T>
    void set(std::string parameter, const std::vector<T>& value);

    /// Get the value of a specified parameter.
    /// @param parameter    Name of parameter to get
    /// @return             Found value
    /// @throws std::string error message in case of unset parameter
    template<class T>
    T get(std::string parameter) const;

    /// Get the value of a specified parameter.
    /// @param  parameter       Name of the parameter to get
    /// @param  defaultValue    value to set if parameter unset
    /// @return                 Found value
    template<class T>
    T get(std::string parameter, T defaultValue);

    /// If multiple values are set, return a list containing these values.
    /// The values are sored separated by the delimiter stored in
    /// thec orresponding property.
    /// @param parameter        Name of the parameter to look for
    /// @return                 List containing the found values
    /// @throws std::string     Error message when parameter not set
    template<class T>
    std::vector<T> getList(std::string parameter) const;

    /// If multiple values are set, return a list containing these values.
    /// The values are sored separated by the delimiter stored in
    /// thec orresponding property.
    /// @param parameter        Name of the parameter to look for
    /// @param defaultValue     Value to set in case of unset parameter
    /// @return                 List containing the found values
    template<class T>
    inline std::vector<T> getList(std::string parameter,
            std::string defaultValue);

    /// Look for parameters beginning with a given string.
    /// @param  beginsWith  Beginning of parameter name
    /// @return             List containing the found parameters.
    ///                     This vector has the same sorting
    ///                     as the lines in the textfile.
    std::vector<std::string> getKeyList(std::string beginsWith = "") const;

    /**
     * Look for parameters ignoring the instance name.
     * For example, getEveryParameter("type") will return a vector every "type"
     * parameter in this ParameterFile.
     * @param param Parameter name to look for
     * @return             List containing the found parameters.
     *                     This vector has the same sorting
     *                     as the lines in the textfile.
     */
    std::vector<std::string> getEveryParameter(const std::string & param) const;

    /// Print a list of all set parameters to sout.
    void showSetParams() const;

    /// Clear parameter list.
    void clear();

    /// Delete a parameter from the parameter list.
    /// @param  parameter   Name of the parameter to remove
    /// @throws std::string error-string describing the failure if parameter
    ///                     is not set
    void erase(std::string parameter);

    /// Clear list of modifications.
    /// This does not change the stored parameters.
    void resetSetParams();

    /// Set property noFoundWarnings.
    /// @param  noFoundWarnings New value
    void setNotFoundWarningsOn(bool noFoundWarnings);

    /// Save parameters and values to a plain text file.
    /// @param  fileName    Name of the file to write to.
    ///                     The file will be overwritten.
    /// @return             True on a successful write
    /// @throws std::string error string on file write-error
    bool save(std::string fileName) const;

    /// Load parameters from the given file.
    /// @param  fileName    Name of the file to read from
    /// @return             True on successful read
    /// @throws std::string error string on read errors
    bool load(std::string fileName);

    /// Save parameters in the same order as they where inserted.
    /// @param  strm        Stream to write to
    void toStream(std::ostream &strm) const;

    /// Restore parameters reading from the given stream.
    /// @param strm        Input stream to read from
    void fromStream(std::istream& strm);
};

#endif // _ParameterFile_H
