#ifndef XMLCommentH
#define XMLCommentH
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

/// \brief define XML comment element

class HIPOLY_PACKAGE CXMLComment : public CXMLNode {
public:
// constructor and destructor -------------------------------------------------
    CXMLComment(CXMLNode* p_parent);
    virtual ~CXMLComment(void);

// comment methods ------------------------------------------------------------
    /// set commnent
    void SetComment(const CSmallString& comment);

    /// get comment
    const CSmallString& GetComment(void) const;

    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);

// section of private data ----------------------------------------------------
private:
    CSmallString    Comment;

    friend class CXMLParser;
};

// -----------------------------------------------------------------------------

#endif

