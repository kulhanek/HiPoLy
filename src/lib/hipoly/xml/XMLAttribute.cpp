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

#include <XMLAttribute.hpp>
#include <XMLElement.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLAttribute::CXMLAttribute(CXMLElement* p_parent)
{
    ParentElement = p_parent;
    NextSiblingAttribute = NULL;
}

//------------------------------------------------------------------------------

CXMLAttribute::~CXMLAttribute(void)
{
    if( ParentElement != NULL ) {
        // remove attribute from the list
        CXMLAttribute* p_attr = ParentElement->FirstAttribute;
        CXMLAttribute* p_prev = NULL;

        // find attribute
        while( p_attr != NULL ) {
            if( p_attr == this ) break;
            p_prev = p_attr;
            p_attr = p_attr->GetNextSiblingAttribute();
        }

        if( p_attr != NULL ) {
            // remove it from list
            if( p_prev == NULL ) {
                ParentElement->FirstAttribute = NextSiblingAttribute;
            } else {
                p_prev->NextSiblingAttribute = NextSiblingAttribute;
            }
            if( ParentElement->LastAttribute == this ){
                ParentElement->LastAttribute = p_prev;
            }
        }
        if( ParentElement->NextToTry == this ){
            ParentElement->NextToTry = NULL;
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLElement*  CXMLAttribute::GetParentElement(void) const
{
    return(ParentElement);
}

//------------------------------------------------------------------------------

CXMLAttribute* CXMLAttribute::GetNextSiblingAttribute(void) const
{
    return(NextSiblingAttribute);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================



