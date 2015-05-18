#ifndef XMLNodeH
#define XMLNodeH
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
#include <SimpleList.hpp>
#include <SmallString.hpp>

// -----------------------------------------------------------------------------

class CXMLDeclaration;
class CXMLComment;
class CXMLUnknown;
class CXMLElement;
class CXMLText;
class CXMLBinData;

// -----------------------------------------------------------------------------

/// \brief recognized XML node types

enum EXMLNodeType {
    EXNT_DOCUMENT,      // document node
    EXNT_DECLARATION,   // declaration of XML file
    EXNT_UNKNOWN,       // unknown element
    EXNT_COMMENT,       // <!-- comment -->
    EXNT_ELEMENT,       // <ELEMENT at1="value"></ELEMENT>
    EXNT_TEXT,          // <B>text element</B>
    EXNT_BIN_DATA       // <BIN_DATA name="myname>FFFFFFFFFFF</BIN_DATA>
};

// -----------------------------------------------------------------------------

/// \brief define XML node, base class of all elements

class HIPOLY_PACKAGE CXMLNode {
public:
// constructor and destructor --------------------------------------------------
    CXMLNode(CXMLNode* p_parent);
    virtual ~CXMLNode(void);

// node properties -------------------------------------------------------------
    /// return type of node
    EXMLNodeType GetNodeType(void) const;

    /// has child nodes
    bool HasChildNodes(void) const;

    /// return number of child nodes
    int GetNumberOfChildNodes(void) const;

//==============================================================================

// create new child nodes ------------------------------------------------------
    /// create new declaration
    CXMLDeclaration* CreateChildDeclaration(void);

    /// create new unknown element
    CXMLUnknown*    CreateChildUnknown(void);

    /// create new comment
    CXMLComment*    CreateChildComment(void);

    /// create new comment
    CXMLComment*    CreateChildComment(const CSmallString& comment);

    /// create new element
    CXMLElement*    CreateChildElement(const CSmallString& name);

    /// create new element by path
    CXMLElement*    CreateChildElementByPath(const CSmallString& path);

    /// create new text node
    CXMLText*       CreateChildText(bool allow_all=false);

    /// create new text node
    CXMLText*       CreateChildText(const CSmallString& contents,bool allow_all=false);

    /// create new binary data
    CXMLBinData*    CreateChildBinData(const CSmallString& name);

//==============================================================================

// create new sibling nodes ----------------------------------------------------
    /// create new comment
    CXMLComment*    CreateSiblingComment(void);

    /// create new comment
    CXMLComment*    CreateSiblingComment(const CSmallString& comment);

    /// create new element
    CXMLElement*    CreateSiblingElement(const CSmallString& name);

    /// create new element by path
    CXMLElement*    CreateSiblingElementByPath(const CSmallString& path);

    /// create new text node
    CXMLText*       CreateSiblingText(bool allow_all=false);

    /// create new text node
    CXMLText*       CreateSiblingText(const CSmallString& contents);

    /// create new binary data
    CXMLBinData*    CreateSiblingBinData(const CSmallString& name);

//==============================================================================

// sibling elements ------------------------------------------------------------
    /// return next element on the same level
    CXMLComment* GetNextSiblingComment(void) const;

// child elements --------------------------------------------------------------
    /// return the first child element
    CXMLComment* GetFirstChildComment(void) const;

    /// return the last child element
    CXMLComment* GetLastChildComment(void) const;

//==============================================================================

// sibling elements ------------------------------------------------------------
    /// return next element on the same level
    CXMLText* GetNextSiblingText(void) const;

// child elements --------------------------------------------------------------
    /// return the first child element
    CXMLText* GetFirstChildText(void) const;

    /// return the last child element
    CXMLText* GetLastChildText(void) const;

//==============================================================================

// sibling elements ------------------------------------------------------------
    /// return next element on the same level
    CXMLElement* GetNextSiblingElement(void) const;

    /// return next element with name on the same level
    CXMLElement* GetNextSiblingElement(const CSmallString& name) const;

// child elements --------------------------------------------------------------
    /// return the first child element
    CXMLElement* GetFirstChildElement(void) const;

    /// return the first child element with name
    CXMLElement* GetFirstChildElement(const CSmallString& name) const;

    /// return the last child element
    CXMLElement* GetLastChildElement(void) const;

    /// return the last child element with name
    CXMLElement* GetLastChildElement(const CSmallString& name) const;

    /// open child element by path
    CXMLElement* GetChildElementByPath(const CSmallString& path,bool create=false);

    /// open child element by path
    CXMLElement* GetChildElementByPath(const CSmallString& path) const;

//==============================================================================

// sibling elements ------------------------------------------------------------
    /// return next element on the same level
    CXMLBinData* GetNextSiblingBinData(void) const;

    /// return next element with name on the same level
    CXMLBinData* GetNextSiblingBinData(const CSmallString& name) const;

// child elements --------------------------------------------------------------
    /// return the first child element
    CXMLBinData* GetFirstChildBinData(void) const;

    /// return the first child element with name
    CXMLBinData* GetFirstChildBinData(const CSmallString& name) const;

    /// return the last child element
    CXMLBinData* GetLastChildBinData(void) const;

    /// return the last child element with name
    CXMLBinData* GetLastChildBinData(const CSmallString& name) const;

//==============================================================================

// node methods ----------------------------------------------------------------
    /// return parent node
    CXMLNode* GetParentNode(void) const;

    /// return next sibling node
    CXMLNode* GetNextSiblingNode(void) const;

    /// return first child node
    CXMLNode* GetFirstChildNode(void) const;

    /// return last child node
    CXMLNode* GetLastChildNode(void) const;

//==============================================================================

    /// remove one sibling node
    void RemoveSiblingNode(void);

    /// remove one sibling node
    void RemoveAllSiblingNodes(void);

    /// remove all child nodes
    void RemoveAllChildNodes(void);

//==============================================================================

    /// move node up in the list
    void MoveNodeUp(void);

    /// move node down in the list
    void MoveNodeDown(void);

//==============================================================================

    /// add all child nodes from p_src to this node
    void CopyChildNodesFrom(CXMLNode* p_src);

    /// add all child nodes and contents of current node from p_src to this node
    void CopyContentsFrom(CXMLNode* p_src);

    /// duplicate node and insert it to p_parent if it is not NULL
    CXMLNode* DuplicateNode(CXMLNode* p_parent);

//==============================================================================
    /// set user data associated with the node
    void SetUserData(void* p_data);

    /// get user data associated with the node
    void* GetUserData(void) const;

// section of private data -----------------------------------------------------
private:
    CXMLNode*   ParentNode;
    CXMLNode*   FirstChildNode;
    CXMLNode*   NextSiblingNode;
    CXMLNode*   LastChildNode;
    void*       UserData;

// section of public data ------------------------------------------------------
public:
    // if true - SetAttribute does not check if attribute was already set
    // the value is inherrited from parent node or it can be manually set
    // NOTE: potentially dangerous to use if the value is set to true
    bool        FastSetAttribute;

protected:
    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);

    friend class CXMLIterator;
};

// -----------------------------------------------------------------------------

#endif

