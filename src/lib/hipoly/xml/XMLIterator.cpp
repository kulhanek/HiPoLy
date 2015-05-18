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

#include <XMLIterator.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLIterator::CXMLIterator(void)
{
    RootElement = NULL;
    NextElement = NULL;
    NextAttribute = NULL;
}

//------------------------------------------------------------------------------

CXMLIterator::CXMLIterator(CXMLElement* p_ele)
{
    RootElement = p_ele;
    NextElement = NULL;
    NextAttribute = NULL;
    if( RootElement != NULL ) {
        NextElement = RootElement->GetFirstChildElement();
        NextAttribute = RootElement->GetFirstAttribute();
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLIterator::ChangeRootElement(CXMLElement* p_ele)
{
    RootElement = p_ele;
    NextElement = NULL;
    NextAttribute = NULL;
    if( RootElement != NULL ) {
        NextElement = RootElement->GetFirstChildElement();
        NextAttribute = RootElement->GetFirstAttribute();
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLElement* CXMLIterator::SetToFirstChildElement(void)
{
    if( RootElement != NULL ) {
        NextElement = RootElement->GetFirstChildElement();
    } else {
        NextElement = NULL;
    }
    return(NextElement);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLIterator::SetToChildElement(int index)
{
    if( RootElement != NULL ) {
        NextElement = RootElement->GetFirstChildElement();
    } else {
        NextElement = NULL;
    }
    int count = 0;
    while( NextElement != NULL ) {
        if( index == count ) return(NextElement);
        count++;
        NextElement = NextElement->GetNextSiblingElement();
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLIterator::SetToChildElement(const CSmallString& name,int index)
{
    if( RootElement != NULL ) {
        NextElement = RootElement->GetFirstChildElement();
    } else {
        NextElement = NULL;
    }
    int count = 0;
    while( NextElement != NULL ) {
        if( NextElement->GetName() == name ) {
            if( index == count ) return(NextElement);
            count++;
        }
        NextElement = NextElement->GetNextSiblingElement();
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLIterator::GetNextChildElement(void)
{
    CXMLElement* p_ele = NextElement;
    if( NextElement != NULL ) {
        NextElement = NextElement->GetNextSiblingElement();
    }
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLIterator::GetNextChildElement(const CSmallString& name)
{
    CXMLElement* p_ele = NULL;

    while( NextElement != NULL ) {
        if( NextElement->GetName() == name ) {
            p_ele = NextElement;
            NextElement = NextElement->GetNextSiblingElement();
            return(p_ele);
        }
        NextElement = NextElement->GetNextSiblingElement(name);
    }
    return(p_ele);
}

//------------------------------------------------------------------------------

int CXMLIterator::GetNumberOfChildElements(const CSmallString& name)
{
    CXMLElement*   p_ele = NULL;
    int            count = 0;

    if( RootElement != NULL ) {
        p_ele = RootElement->GetFirstChildElement(name);
    }
    while( p_ele != NULL ) {
        count++;
        p_ele = p_ele->GetNextSiblingElement(name);
    }

    return(count);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLIterator::SetToFirstAttribute(void)
{
    if( RootElement != NULL ) {
        NextAttribute = RootElement->GetFirstAttribute();
    } else {
        NextAttribute = NULL;
    }
}

//------------------------------------------------------------------------------

bool CXMLIterator::GetNextAttributeName(CSmallString& name)
{
    CXMLAttribute* p_attr = NextAttribute;
    if( NextAttribute != NULL ) {
        NextAttribute = NextAttribute->GetNextSiblingAttribute();
    }
    if( p_attr == NULL ) return(false);

    name = p_attr->Name;
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


