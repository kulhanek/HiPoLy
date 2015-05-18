#ifndef XMLUnknownH
#define XMLUnknownH
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

/// \brief define XML unknown node

class HIPOLY_PACKAGE CXMLUnknown : public CXMLNode {
public:
// constructor and destructor -------------------------------------------------
    CXMLUnknown(CXMLNode* p_parent);
    virtual ~CXMLUnknown(void);

// text operations ------------------------------------------------------------
    /// get text
    const CSmallString& GetUnknown(void) const;

    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);

// section of private data ----------------------------------------------------
private:
    CSmallString    Unknown;

    friend class CXMLParser;
};

// -----------------------------------------------------------------------------

#endif

