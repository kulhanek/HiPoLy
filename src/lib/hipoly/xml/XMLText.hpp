#ifndef XMLTextH
#define XMLTextH
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
#include <XMLNode.hpp>
#include <SmallString.hpp>

// -----------------------------------------------------------------------------

/// \brief define XML text node

class HIPOLY_PACKAGE CXMLText : public CXMLNode {
public:
// constructor and destructor -------------------------------------------------
    CXMLText(CXMLNode* p_parent);
    virtual ~CXMLText(void);

// text operations ------------------------------------------------------------
    /// set text
    void SetText(const CSmallString& text);

    /// get text
    const CSmallString& GetText(void) const;

    /// get text
    const CSmallString GetTrimmedText(void) const;

    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);

// section of private data ----------------------------------------------------
private:
    CSmallString    Text;

    friend class CXMLParser;
};

// -----------------------------------------------------------------------------

#endif

