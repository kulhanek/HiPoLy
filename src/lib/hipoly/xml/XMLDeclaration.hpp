#ifndef XMLDeclarationH
#define XMLDeclarationH
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
#include <XMLElement.hpp>
#include <SmallString.hpp>

// -----------------------------------------------------------------------------

/// \brief define XML declaration node

class HIPOLY_PACKAGE CXMLDeclaration : public CXMLNode {
public:
// constructor and destructor -------------------------------------------------
    CXMLDeclaration(CXMLNode* p_parent);
    virtual ~CXMLDeclaration(void);

    /// get first attribute, used only for printing
    const CXMLAttribute* GetFirstAttribute(void) const;

// access methods -------------------------------------------------------------
    /// return XML version
    const CSmallString& GetVersion(void) const;

    /// return document encoding
    const CSmallString& GetEncoding(void) const;

    /// return standalone flag
    const CSmallString& GetStandalone(void) const;

    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);

// section of private data ----------------------------------------------------
private:
    CSmallString        Version;
    CSmallString        Encoding;
    CSmallString        Standalone;

    friend class CXMLParser;
};

// -----------------------------------------------------------------------------

#endif

