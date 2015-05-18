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

#include <XMLText.hpp>
#include <ErrorSystem.hpp>
#include <ctype.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLText::CXMLText(CXMLNode* p_parent)
    : CXMLNode(p_parent)
{
}

//------------------------------------------------------------------------------

CXMLText::~CXMLText(void)
{
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLText::SetText(const CSmallString& text)
{
    Text = text;
}

//------------------------------------------------------------------------------

const CSmallString&  CXMLText::GetText(void) const
{
    return(Text);
}

//------------------------------------------------------------------------------

const CSmallString CXMLText::GetTrimmedText(void) const
{
    unsigned int beg;
    unsigned int end;
    // scan beggining
    for(beg = 0; beg < Text.GetLength(); beg++) {
        if( isspace(Text[beg]) == 0 ) break;
    }
    // scan end
    for(end = Text.GetLength(); end >= 0; end--) {
        if( isspace(Text[end]) == 0 ) break;
    }

    if( end < beg ) return("");
    return(Text.GetSubStringFromTo(beg,end));
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLText::CopyData(CXMLNode* p_src)
{
    CXMLText* p_rsrc = dynamic_cast<CXMLText*>(p_src);
    if( p_rsrc == NULL ) {
        LOGIC_ERROR("incompatible source node");
    }
    Text = p_rsrc->Text;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

