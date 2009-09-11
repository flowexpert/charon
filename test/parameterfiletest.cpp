/*  Copyright (C) 2009 Jens-Malte Gottfried

    This file is part of Charon.
    
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
/** @file   parameterfiletest.cpp
 *  @brief  Unit tests for ParameterFile class.
 *  @date   12.08.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <cstdlib>
#include "ParameterFile.hxx"

/// Test application for class ParameterFile.
int main(){
    try{
        sout.assign(std::cout);
        ParameterFile* paramtest = new ParameterFile();

        // test setting and getting of values
        paramtest->set("param1", "value1 test");
        paramtest->set("param2", 10);
        std::string	val1 = paramtest->get<std::string>("param1");
        int			val2 = paramtest->get<int>("param2");
        sout << "val1 = " << val1 << std::endl;
        sout << "val2 = " << val2 << std::endl;
        if((val1 != "value1 test") || (val2 != 10))
            throw std::string("Values do not match!");

        // modification
        paramtest->set("param2", 15);
        val2 = paramtest->get<int>("param2");
        sout << "val2 = " << val2 << std::endl;
        if (val2 != 15)
            throw std::string("Failure loading modified value!");

        // information about set/modified parameters
        sout << "Output from showSetParams():" << std::endl;
        paramtest->showSetParams ();
        sout << "Size: " << paramtest->getKeyList().size() << std::endl;
        if(paramtest->getKeyList().size() != 2)
            throw std::string("Failure loading KeyList!");

        // test save and reload
        sout << "Saving to file \"test.txt\"." << std::endl;
        paramtest->save("test.txt");
        delete paramtest;

        sout << "Reloading file" << std::endl;
        paramtest = new ParameterFile("test.txt");

        sout << "Checking values..." << std::flush;
        val1 = paramtest->get<std::string>("param1");
        val2 = paramtest->get<int>("param2");
        if((val1 != "value1 test") || (val2 != 15))
            throw std::string("reloaded values do not match!");
        sout << "ok" << std::endl;

        // test value deletion
        sout << "deleting param1..." << std::flush;
        paramtest->erase("param1");
        if(paramtest->isSet("param1"))
            throw std::string("deleted value still set!");
        if(paramtest->getKeyList().size() != 1)
            throw std::string("length is not 1!");
        sout << "ok" << std::endl;

        sout << "deleting param2..." << std::flush;
        paramtest->erase("param2");
        if(paramtest->isSet("param2"))
            throw std::string("deleted value still set!");
        if(paramtest->getKeyList().size() != 0)
            throw std::string("length is not 0!");
        sout << "ok" << std::endl;

        sout << "checking exceptions..." << std::flush;
        bool fail = true;
        try {
            val1 = paramtest->get<std::string>("param1");
        }
        catch(std::string reason) {
            if (reason.find("not set") != std::string::npos)
                fail = false;
        }
        if (fail)
            throw std::string("no exception from get caught!");

        fail = true;
        try {
            paramtest->erase("param2");
        }
        catch(std::string reason) {
            if (reason.find("not set") != std::string::npos)
                fail = false;
        }
        if (fail)
            throw std::string("no exception from erase caught!");

        fail = true;
        try {
            paramtest->getList<float>("bla");
        }
        catch(std::string reason) {
            if (reason.find("not set") != std::string::npos)
                fail = false;
        }
        if (fail)
            throw std::string("no exception from getList caught!");

        sout << "ok" << std::endl;
        delete paramtest;
    }
    catch(std::string message) {
        std::cerr << "\n\nFailure:" << std::endl;
        std::cerr << message << std::endl;
        return EXIT_FAILURE;
    }
    catch(...) {
        std::cerr << "Unhandled exception!" << std::endl;
        return EXIT_FAILURE;
    }

    // Tests passed
    return EXIT_SUCCESS;
}
