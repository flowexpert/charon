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
/** @file graphtest.cpp
 *  @brief Testing of graphviz libraries
 *  @date	27.08.2008
 *  @author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <graphviz/gvc.h>
#include <graphviz/graph.h>
#include <iostream>
#include <string>
#include <cstdlib>

/// Starting point of application.
int main(){
    GVC_t*	    gvc   = gvContext();
    graph_t*    graph = agopen(const_cast<char*>("g"), AGDIGRAPH);
    agraphattr( graph, const_cast<char*>("rankdir"), const_cast<char*>("LR"));
    agnodeattr( graph, const_cast<char*>("shape")  , const_cast<char*>("box"));
    Agnode_t*   A     = agnode(graph, const_cast<char*>("A"));
    Agnode_t*   B     = agnode(graph, const_cast<char*>("B"));
    agedge(graph, A, B);
    gvLayout(gvc, graph, const_cast<char*>("dot"));
    gvRenderFilename(gvc, graph, const_cast<char*>("xdot"), const_cast<char*>("test.dot"));
    gvRenderFilename(gvc, graph, const_cast<char*>("svg"),  const_cast<char*>("test.svg"));
    gvRenderFilename(gvc, graph, const_cast<char*>("png"),  const_cast<char*>("test.png"));
    gvFreeLayout(gvc, graph);

    // Positionen ausgeben
    char* pos = agget(A, const_cast<char*>("pos"));
    if(pos)
        std::cout << "Pos A: " << pos << std::endl;
    else
        return EXIT_FAILURE;

    pos = agget(B, const_cast<char*>("pos"));
    if(pos)
        std::cout << "Pos B: " << pos << std::endl;
    else
        return EXIT_FAILURE;

    agclose(graph);
    gvFreeContext(gvc);

    return EXIT_SUCCESS;
}
