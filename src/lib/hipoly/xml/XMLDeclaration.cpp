//==============================================================================
//    Copyright 2000,2004,2008,2011 Petr Kulhanek
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

#include <XMLDeclaration.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLDeclaration::CXMLDeclaration(CXMLNode* p_parent)
    : CXMLNode(p_parent)
{
    Version = "1.0";
    Encoding = "UTF-8";
    Standalone = "";
}

//------------------------------------------------------------------------------

CXMLDeclaration::~CXMLDeclaration(void)
{
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const CSmallString& CXMLDeclaration::GetVersion(void) const
{
    return(Version);
}

//------------------------------------------------------------------------------

const CSmallString& CXMLDeclaration::GetEncoding(void) const
{
    return(Encoding);
}

//------------------------------------------------------------------------------

const CSmallString& CXMLDeclaration::GetStandalone(void) const
{
    return(Standalone);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLDeclaration::CopyData(CXMLNode* p_src)
{
    CXMLDeclaration* p_rsrc = dynamic_cast<CXMLDeclaration*>(p_src);
    if( p_rsrc == NULL ) {
        LOGIC_ERROR("incompatible source node");
    }
    Version = p_rsrc->Version;
    Encoding = p_rsrc->Encoding;
    Standalone = p_rsrc->Standalone;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================



