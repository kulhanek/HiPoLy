#ifndef XMLIteratorH
#define XMLIteratorH
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

#include <HiPoLyMainHeader.hpp>
#include <SmallString.hpp>
#include <SimpleIterator.hpp>
#include <XMLElement.hpp>
#include <XMLAttribute.hpp>

//------------------------------------------------------------------------------

/// \brief define iterator over XML elements and attributes within parent element

class HIPOLY_PACKAGE CXMLIterator {
public:
    CXMLIterator(void);
    CXMLIterator(CXMLElement* p_ele);

    /// chnage root element for iteration
    void ChangeRootElement(CXMLElement* p_ele);

    //--------------------------------------------------------------------------
    /// set to first element
    CXMLElement* SetToFirstChildElement(void);

    /// set to specific child element
    CXMLElement* SetToChildElement(int index);

    /// set to specific child element
    CXMLElement* SetToChildElement(const CSmallString& name,int index);

    /// get next sub element
    CXMLElement* GetNextChildElement(void);

    /// get next sub element
    CXMLElement* GetNextChildElement(const CSmallString& name);

    /// get number of elements
    int GetNumberOfChildElements(const CSmallString& name);

    //--------------------------------------------------------------------------
    /// set to first attribute
    void SetToFirstAttribute(void);

    /// get next attribute
    bool GetNextAttributeName(CSmallString& name);

// section of private data ----------------------------------------------------
private:
    CXMLElement*    RootElement;
    CXMLElement*    NextElement;
    CXMLAttribute*  NextAttribute;
};

//------------------------------------------------------------------------------

#endif

