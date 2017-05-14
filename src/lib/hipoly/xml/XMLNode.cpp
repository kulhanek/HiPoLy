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

#include <string.h>
#include <XMLNode.hpp>
#include <XMLDeclaration.hpp>
#include <XMLComment.hpp>
#include <XMLElement.hpp>
#include <XMLText.hpp>
#include <XMLBinData.hpp>
#include <XMLDocument.hpp>
#include <XMLUnknown.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#if defined _WIN32 || defined __CYGWIN__
char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    return(strtok(str,delim));
}
#else
char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    return(strtok_r(str,delim,saveptr));
}
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLNode::CXMLNode(CXMLNode* p_parent)
{
    ParentNode = p_parent;
    FirstChildNode = NULL;
    LastChildNode = NULL;
    NextSiblingNode = NULL;
    UserData = NULL;
    if( p_parent ){
        FastSetAttribute = p_parent->FastSetAttribute;
    } else {
        FastSetAttribute = false;
    }
}

//------------------------------------------------------------------------------

CXMLNode::~CXMLNode(void)
{
    if( ParentNode != NULL ) {
        // find previous node
        CXMLNode* p_prev = ParentNode->FirstChildNode;
        while( p_prev != NULL ) {
            if( p_prev->NextSiblingNode == this ) break;
            p_prev = p_prev->NextSiblingNode;
        }
        if( p_prev == NULL ) {
            // it is the first child node
            ParentNode->FirstChildNode = NextSiblingNode;
        } else {
            p_prev->NextSiblingNode = NextSiblingNode;
        }
        if( ParentNode->LastChildNode == this ){
            ParentNode->LastChildNode = p_prev;
        }
        NextSiblingNode = NULL;
    }

    ParentNode = NULL;

    // delete child nodes
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        CXMLNode* p_node_del = p_node;
        p_node = p_node->NextSiblingNode;
        p_node_del->ParentNode = NULL;
        delete p_node_del;
    }

    FirstChildNode = NULL;
    LastChildNode = NULL;
    NextSiblingNode = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

EXMLNodeType CXMLNode::GetNodeType(void) const
{
    if( dynamic_cast<const CXMLDocument*>(this) != NULL ) {
        return(EXNT_DOCUMENT);
    }
    if( dynamic_cast<const CXMLDeclaration*>(this) != NULL ) {
        return(EXNT_DECLARATION);
    }
    if( dynamic_cast<const CXMLUnknown*>(this) != NULL ) {
        return(EXNT_UNKNOWN);
    }
    if( dynamic_cast<const CXMLComment*>(this) != NULL ) {
        return(EXNT_COMMENT);
    }
    if( dynamic_cast<const CXMLText*>(this) != NULL ) {
        return(EXNT_TEXT);
    }

    // ORDER is important since CXMLBinData is child of CXMLElement

    if( dynamic_cast<const CXMLBinData*>(this) != NULL ) {
        return(EXNT_BIN_DATA);
    }
    if( dynamic_cast<const CXMLElement*>(this) != NULL ) {
        return(EXNT_ELEMENT);
    }

    return(EXNT_UNKNOWN);
}

//------------------------------------------------------------------------------

bool CXMLNode::HasChildNodes(void) const
{
    return( FirstChildNode != NULL );
}

//------------------------------------------------------------------------------

int CXMLNode::GetNumberOfChildNodes(void) const
{
    CXMLNode*  p_node = FirstChildNode;
    int        count = 0;

    while( p_node != NULL ) {
        count++;
        p_node = p_node->NextSiblingNode;
    }
    return(count);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLDeclaration* CXMLNode::CreateChildDeclaration(void)
{
    if( GetNodeType() != EXNT_DOCUMENT ) {
        LOGIC_ERROR("XMLDeclaration can be nested only into XMLDocument");
    }

    CXMLDeclaration* p_ele = NULL;
    p_ele = new CXMLDeclaration(this);

    CXMLNode* p_last = GetLastChildNode();
    if( p_last == NULL ) {
        FirstChildNode = p_ele;
    } else {
        p_last->NextSiblingNode = p_ele;
    }
    LastChildNode = p_ele;

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLUnknown* CXMLNode::CreateChildUnknown(void)
{
    if( (GetNodeType() != EXNT_DOCUMENT) && (GetNodeType() != EXNT_ELEMENT) ) {
        LOGIC_ERROR("XMLUnknown can be nested only into XMLDocument or XMLElement");
    }

    CXMLUnknown* p_ele = new CXMLUnknown(this);

    CXMLNode* p_last = GetLastChildNode();
    if( p_last == NULL ) {
        FirstChildNode = p_ele;
    } else {
        p_last->NextSiblingNode = p_ele;
    }
    LastChildNode = p_ele;

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLComment* CXMLNode::CreateChildComment(void)
{
    if( (GetNodeType() != EXNT_DOCUMENT) && (GetNodeType() != EXNT_ELEMENT) ) {
        LOGIC_ERROR("XMLComment can be nested only into XMLDocument or XMLElement");
    }

    CXMLComment* p_ele = new CXMLComment(this);

    CXMLNode* p_last = GetLastChildNode();
    if( p_last == NULL ) {
        FirstChildNode = p_ele;
    } else {
        p_last->NextSiblingNode = p_ele;
    }
    LastChildNode = p_ele;

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLComment* CXMLNode::CreateChildComment(const CSmallString& comment)
{
    CXMLComment* p_ele = CreateChildComment();
    p_ele->SetComment(comment);
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::CreateChildElement(const CSmallString& name)
{
    if( (GetNodeType() != EXNT_DOCUMENT) && (GetNodeType() != EXNT_ELEMENT) ) {
        LOGIC_ERROR("XMLElement can be nested only into XMLDocument or XMLElement");
    }

    CXMLElement* p_ele = new CXMLElement(this);
    p_ele->SetName(name);

    CXMLNode* p_last = GetLastChildNode();
    if( p_last == NULL ) {
        FirstChildNode = p_ele;
    } else {
        p_last->NextSiblingNode = p_ele;
    }
    LastChildNode = p_ele;

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::CreateChildElementByPath(const CSmallString& path)
{
    CSmallString   tmp(path);

    char* strtok_state;
    char* p_dirname = my_strtok_r(tmp.GetBuffer(),"/",&strtok_state);

    if( p_dirname == NULL ) {
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }
    if( p_dirname == NULL ){
        INVALID_ARGUMENT("path is empty");
    }

    CXMLElement* p_ele = CreateChildElement(p_dirname);

    p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    CXMLElement* p_sele = p_ele;

    while( p_dirname != NULL ) {
        p_sele = p_ele->CreateChildElement(p_dirname);
        p_ele = p_sele;
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }

    return(p_sele);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::CreateChildText(bool allow_all)
{
    if( ! allow_all ) {
        if( GetNodeType() != EXNT_ELEMENT ) {
            LOGIC_ERROR("XMLText can be nested only into XMLElement");
        }
    }

    CXMLText* p_ele = new CXMLText(this);

    CXMLNode* p_last = GetLastChildNode();
    if( p_last == NULL ) {
        FirstChildNode = p_ele;
    } else {
        p_last->NextSiblingNode = p_ele;
    }
    LastChildNode = p_ele;

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::CreateChildText(const CSmallString& contents,bool allow_all)
{
    CXMLText* p_ele = CreateChildText(allow_all);
    p_ele->SetText(contents);
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::CreateChildText(const CSmallString& contents)
{
    CXMLText* p_ele = CreateChildText(false);
    p_ele->SetText(contents);
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::CreateChildText(const char* contents)
{
    CXMLText* p_ele = CreateChildText(false);
    p_ele->SetText(contents);
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::CreateChildBinData(const CSmallString& name)
{
    if( (GetNodeType() != EXNT_ELEMENT) && (GetNodeType() != EXNT_ELEMENT) ) {
        LOGIC_ERROR("XMLBinData can be nested only into XMLDocument or XMLElement");
        return(NULL);
    }

    CXMLBinData* p_ele = new CXMLBinData(this);
    p_ele->SetName(name);

    CXMLNode* p_last = GetLastChildNode();
    if( p_last == NULL ) {
        FirstChildNode = p_ele;
    } else {
        p_last->NextSiblingNode = p_ele;
    }
    LastChildNode = p_ele;

    return(p_ele);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLComment* CXMLNode::CreateSiblingComment(void)
{
    CXMLComment* p_ele = new CXMLComment(ParentNode);

    p_ele->NextSiblingNode = NextSiblingNode;
    NextSiblingNode = p_ele;
    if( ParentNode ){
        if( ParentNode->LastChildNode == this ){
            ParentNode->LastChildNode = p_ele;
        }
    }

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLComment* CXMLNode::CreateSiblingComment(const CSmallString& comment)
{
    CXMLComment* p_ele = CreateSiblingComment();
    p_ele->SetComment(comment);
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::CreateSiblingElement(const CSmallString& name)
{
    CXMLElement* p_ele = new CXMLElement(ParentNode);
    p_ele->SetName(name);

    p_ele->NextSiblingNode = NextSiblingNode;
    NextSiblingNode = p_ele;
    if( ParentNode ){
        if( ParentNode->LastChildNode == this ){
            ParentNode->LastChildNode = p_ele;
        }
    }

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::CreateSiblingElementByPath(const CSmallString& path)
{
    CSmallString   tmp(path);

    char* strtok_state;
    char* p_dirname = my_strtok_r(tmp.GetBuffer(),"/",&strtok_state);

    CXMLElement* p_ele;

    if( p_dirname == NULL ) {
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }
    if( p_dirname == NULL ){
        INVALID_ARGUMENT("illegal path");
    }

    p_ele = CreateSiblingElement(p_dirname);

    p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    CXMLElement* p_sele = p_ele;

    while( p_dirname != NULL ) {
        p_sele = p_ele->CreateChildElement(p_dirname);
        p_ele = p_sele;
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }

    return(p_sele);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::CreateSiblingText(bool allow_all)
{
    if( ! allow_all ) {
        if( GetNodeType() == EXNT_TEXT ) {
            LOGIC_ERROR("XMLText can not be the sibling of another XMLText");
        }
    }

    CXMLText* p_ele = new CXMLText(ParentNode);

    p_ele->NextSiblingNode = NextSiblingNode;
    NextSiblingNode = p_ele;
    if( ParentNode ){
        if( ParentNode->LastChildNode == this ){
            ParentNode->LastChildNode = p_ele;
        }
    }

    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::CreateSiblingText(const CSmallString& contents)
{
    CXMLText* p_ele = CreateSiblingText();
    p_ele->SetText(contents);
    return(p_ele);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::CreateSiblingBinData(const CSmallString& name)
{
    CXMLBinData* p_ele = new CXMLBinData(ParentNode);
    p_ele->SetName(name);

    p_ele->NextSiblingNode = NextSiblingNode;
    NextSiblingNode = p_ele;
    if( ParentNode ){
        if( ParentNode->LastChildNode == this ){
            ParentNode->LastChildNode = p_ele;
        }
    }

    return(p_ele);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLComment* CXMLNode::GetFirstChildComment(void) const
{
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_COMMENT ) {
            return(dynamic_cast<CXMLComment*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLComment* CXMLNode::GetNextSiblingComment(void) const
{
    CXMLNode* p_node = NextSiblingNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_COMMENT ) {
            return(dynamic_cast<CXMLComment*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLComment* CXMLNode::GetLastChildComment(void) const
{
    CXMLComment*   p_last = NULL;
    CXMLNode*      p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_COMMENT ) {
            p_last = dynamic_cast<CXMLComment*>(p_node);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(p_last);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLText* CXMLNode::GetFirstChildText(void) const
{
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_TEXT ) {
            return(dynamic_cast<CXMLText*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::GetNextSiblingText(void) const
{
    CXMLNode* p_node = NextSiblingNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_TEXT ) {
            return(dynamic_cast<CXMLText*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLText* CXMLNode::GetLastChildText(void) const
{
    CXMLText*   p_last = NULL;
    CXMLNode*      p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_TEXT ) {
            p_last = dynamic_cast<CXMLText*>(p_node);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(p_last);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLElement* CXMLNode::GetFirstChildElement(void) const
{
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_ELEMENT ) {
            return(dynamic_cast<CXMLElement*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetFirstChildElement(const CSmallString& name) const
{
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_ELEMENT ) {
            CXMLElement* p_ele = dynamic_cast<CXMLElement*>(p_node);
            if( p_ele->GetName() == name ) return(p_ele);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetNextSiblingElement(void) const
{
    CXMLNode* p_node = NextSiblingNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_ELEMENT ) {
            return(dynamic_cast<CXMLElement*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetNextSiblingElement(const CSmallString& name) const
{
    CXMLNode* p_node = NextSiblingNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_ELEMENT ) {
            CXMLElement* p_ele = dynamic_cast<CXMLElement*>(p_node);
            if( p_ele->GetName() == name ) return(p_ele);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetLastChildElement(void) const
{
    CXMLElement*   p_last = NULL;
    CXMLNode*      p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_ELEMENT ) {
            p_last = dynamic_cast<CXMLElement*>(p_node);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(p_last);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetLastChildElement(const CSmallString& name) const
{
    CXMLElement*   p_last = NULL;
    CXMLNode*      p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_ELEMENT ) {
            CXMLElement* p_ele = dynamic_cast<CXMLElement*>(p_node);
            if( p_ele->GetName() == name ) p_last = p_ele;
        }
        p_node = p_node->NextSiblingNode;
    }

    return(p_last);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetChildElementByPath(
    const CSmallString& path,bool create)
{
    CSmallString   tmp(path);

    char* strtok_state;
    char* p_dirname = my_strtok_r(tmp.GetBuffer(),"/",&strtok_state);

    CXMLElement* p_ele;

    if( p_dirname == NULL ) {
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }
    if( p_dirname == NULL ) return(NULL);

    if( (p_ele = GetFirstChildElement(p_dirname)) == NULL ) {
        if( create == true ) {
            p_ele = CreateChildElement(p_dirname);
        }
    }
    if( p_ele == NULL ) return(NULL);

    p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    CXMLElement* p_sele = p_ele;

    while( p_dirname != NULL ) {
        p_sele = p_ele->GetFirstChildElement(p_dirname);
        if( (p_sele == NULL) && (create == true) ) {
            p_sele = p_ele->CreateChildElement(p_dirname);
        }
        if( p_sele == NULL ) return(NULL);
        p_ele = p_sele;
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }

    return(p_sele);
}

//------------------------------------------------------------------------------

CXMLElement* CXMLNode::GetChildElementByPath(const CSmallString& path) const
{
    CSmallString   tmp(path);

    char* strtok_state;
    char* p_dirname = my_strtok_r(tmp.GetBuffer(),"/",&strtok_state);

    CXMLElement* p_ele;

    if( p_dirname == NULL ) {
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }
    if( p_dirname == NULL ) return(NULL);

    p_ele = GetFirstChildElement(p_dirname);
    if( p_ele == NULL ) return(NULL);

    p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    CXMLElement* p_sele = p_ele;

    while( p_dirname != NULL ) {
        p_sele = p_ele->GetFirstChildElement(p_dirname);
        if( p_sele == NULL ) return(NULL);
        p_ele = p_sele;
        p_dirname = my_strtok_r(NULL,"/",&strtok_state);
    }

    return(p_sele);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLBinData* CXMLNode::GetFirstChildBinData(void) const
{
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_BIN_DATA ) {
            return(dynamic_cast<CXMLBinData*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::GetFirstChildBinData(const CSmallString& name) const
{
    CXMLNode* p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_BIN_DATA ) {
            CXMLBinData* p_ele = dynamic_cast<CXMLBinData*>(p_node);
            if( p_ele->GetName() == name ) return(p_ele);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::GetNextSiblingBinData(void) const
{
    CXMLNode* p_node = NextSiblingNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_BIN_DATA ) {
            return(dynamic_cast<CXMLBinData*>(p_node));
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::GetNextSiblingBinData(const CSmallString& name) const
{
    CXMLNode* p_node = NextSiblingNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_BIN_DATA ) {
            CXMLBinData* p_ele = dynamic_cast<CXMLBinData*>(p_node);
            if( p_ele->GetName() == name ) return(p_ele);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(NULL);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::GetLastChildBinData(void) const
{
    CXMLBinData*   p_last = NULL;
    CXMLNode*      p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_BIN_DATA ) {
            p_last = dynamic_cast<CXMLBinData*>(p_node);
        }
        p_node = p_node->NextSiblingNode;
    }
    return(p_last);
}

//------------------------------------------------------------------------------

CXMLBinData* CXMLNode::GetLastChildBinData(const CSmallString& name) const
{
    CXMLBinData*   p_last = NULL;
    CXMLNode*      p_node = FirstChildNode;
    while( p_node != NULL ) {
        if( p_node->GetNodeType() == EXNT_BIN_DATA ) {
            CXMLBinData* p_ele = dynamic_cast<CXMLBinData*>(p_node);
            if( p_ele->GetName() == name ) p_last = p_ele;
        }
        p_node = p_node->NextSiblingNode;
    }
    return(p_last);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLNode* CXMLNode::GetParentNode(void) const
{
    return(ParentNode);
}

//------------------------------------------------------------------------------

CXMLNode* CXMLNode::GetNextSiblingNode(void) const
{
    return(NextSiblingNode);
}

//------------------------------------------------------------------------------

CXMLNode* CXMLNode::GetFirstChildNode(void) const
{
    return(FirstChildNode);
}

//------------------------------------------------------------------------------

CXMLNode* CXMLNode::GetLastChildNode(void) const
{
    return(LastChildNode);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLNode::RemoveSiblingNode(void)
{
    CXMLNode* p_next_next_sinling = NULL;

    if( NextSiblingNode != NULL ) {
        p_next_next_sinling = NextSiblingNode->NextSiblingNode;
    }

    if( NextSiblingNode != NULL ) {
        if( NextSiblingNode->ParentNode ){
            if( NextSiblingNode->ParentNode->LastChildNode == NextSiblingNode ){
                NextSiblingNode->ParentNode->LastChildNode =  NextSiblingNode->NextSiblingNode;
            }
        }
        NextSiblingNode->ParentNode = NULL;
        NextSiblingNode->NextSiblingNode = NULL;
        delete NextSiblingNode;
    }

    NextSiblingNode = p_next_next_sinling;
}

//------------------------------------------------------------------------------

void CXMLNode::RemoveAllSiblingNodes(void)
{
    CXMLNode*      p_node = NextSiblingNode;
    CXMLNode*      p_last;

    while( p_node != NULL ) {
        p_last = p_node;
        p_node = p_node->NextSiblingNode;

        p_last->ParentNode = NULL;
        p_last->NextSiblingNode = NULL;
        delete p_last;
    }

    NextSiblingNode = NULL;
    if( ParentNode ){
        ParentNode->LastChildNode = this;
    }
}

//------------------------------------------------------------------------------

void CXMLNode::RemoveAllChildNodes(void)
{
    CXMLNode*      p_node = FirstChildNode;
    CXMLNode*      p_last;

    while( p_node != NULL ) {
        p_last = p_node;
        p_node = p_node->NextSiblingNode;

        p_last->ParentNode = NULL;
        p_last->NextSiblingNode = NULL;
        delete p_last;
    }

    FirstChildNode = NULL;
    LastChildNode = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLNode::MoveNodeUp(void)
{

}

//------------------------------------------------------------------------------

void CXMLNode::MoveNodeDown(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLNode::CopyChildNodesFrom(CXMLNode* p_src)
{
    if( p_src == NULL ) {
        INVALID_ARGUMENT("p_src is NULL");
    }

    CXMLNode*      p_node = p_src->FirstChildNode;

    while( p_node != NULL ) {
        // duplicate node
        p_node->DuplicateNode(this);
        p_node = p_node->NextSiblingNode;
    }
}

//------------------------------------------------------------------------------

void CXMLNode::CopyContentsFrom(CXMLNode* p_src)
{
    if( p_src == NULL ) {
        INVALID_ARGUMENT("p_src is NULL");
    }

    // copy data
    CopyData(p_src);
    CopyChildNodesFrom(p_src);
}

//------------------------------------------------------------------------------

CXMLNode* CXMLNode::DuplicateNode(CXMLNode* p_parent)
{
    // create object
    CXMLNode* p_dupl = NULL;

    switch(GetNodeType()) {
        case EXNT_DOCUMENT:
            p_dupl = new CXMLDocument();
            break;
        case EXNT_DECLARATION:
            p_dupl = new CXMLDeclaration(p_parent);
            break;
        case EXNT_UNKNOWN:
            p_dupl = new CXMLUnknown(p_parent);
            break;
        case EXNT_COMMENT:
            p_dupl = new CXMLComment(p_parent);
            break;
        case EXNT_ELEMENT:
            p_dupl = new CXMLElement(p_parent);
            break;
        case EXNT_TEXT:
            p_dupl = new CXMLText(p_parent);
            break;
        case EXNT_BIN_DATA:
            p_dupl = new CXMLBinData(p_parent);
            break;
        default:
            LOGIC_ERROR("not implemented");
    }

    // copy contents
    p_dupl->CopyContentsFrom(this);

    // insert duplicated node to parent list
    if( p_parent != NULL ) {
        CXMLNode* p_last = p_parent->LastChildNode;
        if( p_last == NULL ) {
            p_parent->FirstChildNode = p_dupl;
        } else {
            p_last->NextSiblingNode = p_dupl;
        }
        p_parent->LastChildNode = p_dupl;
    }

    return(p_dupl);
}

//------------------------------------------------------------------------------

void CXMLNode::CopyData(CXMLNode* p_src)
{
    // nothing here
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLNode::SetUserData(void* p_data)
{
    UserData = p_data;
}

//------------------------------------------------------------------------------

void* CXMLNode::GetUserData(void) const
{
    return(UserData);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================



