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
/** @file ExceptionHandler.h
 *  Declaration of class WorkflowExecutor
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date 2010-02-11
 */

#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

/// Routine to simplify exception handling during workflow executions.
namespace ExceptionHandler {
	/// Execute given function and catch exceptions.
	/** Exceptions are caught, error messages are printed to std::cerr.
	 *  Exception type is given in error message, on gcc, typename
	 *  demangling is performed.
	 *  You may use this as return value of the main() routine.
	 *  \param[in] method   execution routine
	 *  \returns return value of the given function
	 */
	int run(int (&method)());

	/// Execute given function and catch exceptions.
	/** Exceptions are caught, error messages are printed to std::cerr.
	 *  Exception type is given in error message, on gcc, typename
	 *  demangling is performed.
	 *  You may use this as return value of the main() routine.
	 *  \param[in] method   execution routine
	 *  \return success if no exceptions, fail otherwise
	 */
	int run(void (&method)());

	/// execute given function and return EXIT_SUCCESS on exception catch
	/** Exceptions are caught, error message is printed to sout.
	 *  \param[in] method   execution routine
	 *  \return success on exception catch, fail otherwise
	 */
	int checkRaise(void (&method)());
}

#endif // EXCEPTIONHANDLER_H
