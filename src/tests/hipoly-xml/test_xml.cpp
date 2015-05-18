//==============================================================================
//    Copyright 1998,1999,2000,2001,2005 Petr Kulhanek
//
//    This file is part of HiPoLy library.
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor,
//    Boston, MA  02110-1301  USA
//==============================================================================

#include "XMLDocument.hpp"
#include "XMLElement.hpp"
#include "XMLBinData.hpp"
#include "XMLPrinter.hpp"
#include "XMLParser.hpp"
#include "ErrorSystem.hpp"

#include <stdio.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int main(int argc, char* argv[])
{

    CXMLDocument   xmldoc;

    //xmldoc.CreateChildDeclaration();
    //xmldoc.CreateChildComment("my comment");
    /*CXMLElement* p_ele = xmldoc.CreateChildElement("EL1");
    p_ele->SetAttribute("test","value");
    p_ele->SetAttribute("test2","value2");
    p_ele->CreateChildElement("EL2");
    p_ele->CreateChildText("tak toto je text mezi elementy");
    p_ele->CreateChildComment("another comment");
    CXMLBinData* p_bd = p_ele->CreateChildBinData("ARRAY");
    p_ele->CreateChildElement("EL3");
    char array[100];
    for(int i=0; i < 100; i++) array[i] = 0;
    p_bd->CopyData(array,100);

    printer.Print(stdout);

    printer.SetOutputFormat(EXF_BXML);

    printer.Print("test.bxml");
    */

    // parse
    CXMLParser  parser;

    xmldoc.RemoveAllChildNodes();

    parser.SetOutputXMLNode(&xmldoc);
    parser.Parse("test_txml.in");

    //print
    CXMLPrinter    printer;

    printer.SetPrintedXMLNode(&xmldoc);
    printer.SetOutputFormat(EXF_TEXT);

    printer.Print("test_txml.out");
    printer.Print(stdout);

    ErrorSystem.PrintErrors(stdout);

    return(0);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

