#ifndef XMLParserH
#define XMLParserH
//==============================================================================
//    Copyright 2011 Petr Kulhanek
//    Copyright 2008            Jakub Stepan
//    Copyright 2000,2004,2008  Petr Kulhanek
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
#include <XMLNode.hpp>
#include <MemBlockFile.hpp>

// -----------------------------------------------------------------------------

class CXMLNode;
class CXMLDeclaration;
class CXMLUnknown;
class CXMLComment;
class CXMLElement;
class CXMLText;
class CXMLBinData;

// -----------------------------------------------------------------------------

/// \brief parse XML document

class HIPOLY_PACKAGE CXMLParser {
public:
// constructor and destructor -------------------------------------------------
    CXMLParser(void);
    virtual ~CXMLParser(void);

// setup methods --------------------------------------------------------------
    /// set printed XMLNode
    void SetOutputXMLNode(CXMLNode* p_node);

    /// enable white characters as XMLText
    void EnableWhiteCharacters(bool set);

    /// how to transcode text elements
    void EnableTextElementTranscoding(bool set);

// parse methods --------------------------------------------------------------
    /// parse data from file
    bool Parse(const CSmallString& name);

    /// parse data from file
    bool Parse(FILE* p_file);

    /// parse data from array
    bool Parse(void* p_data,unsigned int length);

// section of private data ----------------------------------------------------
private:
    bool                SkipWhiteCharacters;
    bool                TranscodeTextElement;
    unsigned int        Length;                 // data length
    unsigned char*      Data;                   // data for parsing
    unsigned int        Pos;                    // current position
    unsigned char*      Pointer;                // pointer to current character
    bool                OwnData;                // am I data owner?
    CXMLNode*           OutputNode;             // output node

    void DestroyData(void);
    bool ParseXML(void);

    bool ParseTXML(void);
    bool ParseTXMLNode(CXMLNode* p_parent);
    bool ParseTXMLDeclaration(CXMLDeclaration* p_declaration);
    bool ParseTXMLUnknown(CXMLUnknown* p_unknown);
    bool ParseTXMLComment(CXMLComment* p_comment);
    bool ParseTXMLElement(CXMLElement* p_element);
    bool ParseTXMLText(CXMLText* p_text);
    void SkipWhiteSpaces(void);
    bool IsWhite(void);
    bool ParseAttribute(CSmallString& name,CSmallString& value);
    bool DecodeXMLText(unsigned char* p_string,unsigned int len,CSmallString& output);
    bool PlainXMLText(unsigned char* p_string,unsigned int len,CSmallString& output);
    bool DecodeBinData(CXMLNode* p_parent);
    bool TransformToBinData(CXMLElement* p_element);


    bool IsBXML(void);
    bool ParseBXML(void);
    bool ParseBXMLNode(CXMLNode* p_parent);
    bool ParseBXMLDeclaration(CXMLDeclaration* p_declaration);
    bool ParseBXMLUnknown(CXMLUnknown* p_unknown);
    bool ParseBXMLComment(CXMLComment* p_comment);
    bool ParseBXMLElement(CXMLElement* p_element);
    bool ParseBXMLText(CXMLText* p_text);
    bool ParseBXMLBinData(CXMLBinData* p_bindata);
};

// -----------------------------------------------------------------------------

#endif
