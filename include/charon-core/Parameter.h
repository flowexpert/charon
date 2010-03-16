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
/** @file Parameter.h
 *  Declaration of the abstract class
 *  AbstractParameter and template classes Parameter and ParameterList.
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  Original file written by Daniel Kondermann, modified by Jens-Malte
 *  Gottfried.
 *
 *  @date 11.12.2008
 *
 *  This file does only contain the (prototyped) interface of the classes
 *  - AbstractParameter
 *  - Parameter
 *  - ParameterList
 *
 *  The implementation of the prototypes is found in Parameter.hxx
 *
 *  In most cases, you want not to include this file but ParameteredObject.hxx
 *
 *  Changes:
 *  -   added documentation
 *  -   redesigned parameter handling
 *  -   split up interface and implementation (due to cyclic dependencies)
 */

#ifndef _Parameter_H
#define _Parameter_H

#include "AbstractData.hxx"
#include "ParameterFile.hxx"

#ifdef MSVC
#ifdef charon_core_EXPORTS
#define charon_core_DECLDIR __declspec(dllexport)
#else
#define charon_core_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define charon_core_DECLDIR
#endif

class ParameteredObject;

/// Parameter object handling.
/// Provides assignment and setters/getters.
/// Load and save routines are purely virtual.
class AbstractParameter {
private:
    /// Forbid copying.
    AbstractParameter(const AbstractParameter&);

protected:
    /// Parameter name.
    std::string _name;

    /// Parent ParameteredObject.
    ParameteredObject* _parent;

public:
    AbstractParameter();

    /// Initialize new parameter.
    /// Assign name, parent etc.
    /// Set these variables to their final values, they can not be changed
    /// later because this would mess up the parameter store
    /// @param parent           parent object
    /// @param name             parameter name
    /// @throws std::string     Error message if invalid parent set.
    void init(ParameteredObject* parent, const std::string& name);

    virtual ~AbstractParameter();

    /// get parent object
    ParameteredObject& getParent();

    /// get parent object (const version)
    const ParameteredObject& getParent() const;

    /// Get parameter name.
    std::string getName() const;

    /// Try to guess type
    virtual std::string guessType() const = 0;

    /// Save to ParameterFile.
    /// This stores the value to the paremeter name
    /// parent_name._name in the given parameter file.
    /// If the parameter has not been changed (i.e. has the
    /// same value as its default value), the parameter
    /// string will not be written to the parameter file.
    /// @param pf               ParameterFile to write to.
    virtual void save(ParameterFile& pf) const = 0;

    /// Load from ParameterFile.
    /// This restores the value from the paremeter named
    /// "name"."_name" in the given parameter file.
    /// @param pf               ParameterFile to load from.
    virtual void load(const ParameterFile& pf) = 0;

    /// Get default value.
    /// This string representation will be saved in the
    /// metadata parameter file.
    virtual std::string getDefaultString() = 0;

    /**
     * Inserts the value of the Parameter at the end of a stream.
     * Overloaded in class Parametery<T>.
     *
     * @see Parameter::intoStream()
     * @param os Stream ot insert the value in
     */
    virtual void intoStream(std::ostream & os) const = 0;
};

/**
 * Stream output operator
 */
std::ostream charon_core_DECLDIR &operator<<(std::ostream & os, const AbstractParameter & param);

/// Implementation of the AbstractParameter interface for one single
/// parameter.
template <typename T>
class Parameter : public AbstractParameter, public AbstractData<T> {
private:
    /// default value
    T _defaultValue;

    /// current value
    T _value;

public:
    /// Create parameter.
    /// _defaultValue and _value are set to the given
    /// defaultValue.
    /// @param defaultValue     New default and current value.
    Parameter(T defaultValue = T());

    virtual ~Parameter();

    /// Assignment operator for internal value.
    /// This modifies only the current value.
    /// @param B				Value to assign.
    virtual T& operator=(const T& B);

    /// Assign new current and default value
    /// by copying an existing parameter (which has not to be assigned
    /// to some object)
    /// @param B                Copy source
    virtual Parameter<T>& operator=(const Parameter<T>& B);

    /// Cast operator to get internal value.
    virtual operator T() const;

    /// Set default value
    /// @param value            New default value
    void    setDefault(const T& value);

    // overload inherited functions
    virtual std::string guessType() const;
    virtual const T& operator()() const;
    virtual T& operator()();
    virtual void save(ParameterFile& pf) const;
    virtual void load(const ParameterFile& pf);
    virtual std::string getDefaultString();

    /**
     * Inserts the value of the Parameter at the end of a stream.
     *
     * @see AbstractParameter::intoStream()
     * @param os Stream ot insert the value in
     */
    virtual void intoStream(std::ostream & os) const;
};

/// Implementation of the AbstractParameter interface for a list of
/// parameters.
template <typename T>
class ParameterList : public AbstractParameter {
private:
    /// Internal value (list).
    std::vector<T> _value;

    /// Default value, string representation, separated by ";".
    std::string _defaultValue;

public:
    /// Create parameter list
    /// Assign parameter name, description and content.
    /// Internal value will be set to defaultValue, the given string
    /// representation will be split up at ";".
    /// @param defaultValue     Default value.
    ParameterList(std::string defaultValue = "");

    virtual ~ParameterList();

    /// Set default value
    /// @param value            New default value
    void    setDefault(const std::string& value);

    /// Assign parameter content.
    /// Default value remains unchanged.
    /// @param B				Copy source.
    virtual std::vector<T>& operator=(const std::vector<T>& B);

    /// Assign parameter content.
    /// Default value remains unchanged.
    /// @param B                Copy source.
    virtual ParameterList<T>& operator=(const ParameterList<T>& B);

    /// Cast operator to get copy of internal value.
    operator std::vector<T>() const;

    /// Call operator to get a reference to the parameter list content.
    std::vector<T>& operator()();

    /// Call operator to get a const reference to the parameter list content.
    const std::vector<T>& operator()() const;

    virtual std::string guessType() const;
    virtual void save(ParameterFile& pf) const;
    virtual void load(const ParameterFile& pf);
    virtual std::string getDefaultString();

    /**
     * Inserts the value of the ParameterList at the end of a stream.
     *
     * @see AbstractParameter::intoStream()
     * @param os Stream ot insert the value in
     */
    virtual void intoStream(std::ostream & os) const;
};

#endif // _Parameter_H
