#ifndef XMLPrinterH
#define XMLPrinterH
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
#include <MemBlockFile.hpp>
#include <vector>

// -----------------------------------------------------------------------------

class CXMLNode;
class CXMLDocument;
class CXMLDeclaration;
class CXMLUnknown;
class CXMLComment;
class CXMLElement;
class CXMLText;
class CXMLBinData;

// -----------------------------------------------------------------------------

enum EXMLFormat {
    EXF_TEXT,
    EXF_BXML
};

// -----------------------------------------------------------------------------

/// \brief print XML document

class HIPOLY_PACKAGE CXMLPrinter {
public:
// constructor and destructor -------------------------------------------------
    CXMLPrinter(void);
    virtual ~CXMLPrinter(void);

// setup methods --------------------------------------------------------------
    /// set printed XMLNode
    void SetPrintedXMLNode(const CXMLNode* p_node);

    /// set output format
    void SetOutputFormat(EXMLFormat format);

    /// get output format
    EXMLFormat GetOutputFormat(void) const;

    /// set print form
    void  SetPrintAsItIs(bool set);

    /// how to transcode text elements
    void EnableTextElementTranscoding(bool set);

// print methods --------------------------------------------------------------
    /// save data to array, array has to be deallocated with delete[]
    unsigned char*  Print(unsigned int& length);

    /// save data to file with name
    bool Print(const CSmallString& name);

    /// save data to file
    bool Print(FILE* p_file);

    /// save data to string
    const CSmallString Print(void);

    /// encode XML text
    static const CSmallString EncodeXMLText(const CSmallString& input);

    /// clear linear elements
    void ClearLinearElements(void);

    /// register linear element
    void RegisterLinearElement(const CSmallString& elem);

    /// clear pre-formated elements
    void ClearPreformatedElements(void);

    /// register  pre-formated element
    void RegisterPreformatedElements(const CSmallString& elem);

// section of private data ----------------------------------------------------
private:
    CMemBlockFile   OutputFile;             // output memory stream
    const CXMLNode* PrintedNode;            // printed node
    EXMLFormat      OutputFormat;           // output format
    bool            PrintAsItIs;            // print as it is
    bool            TranscodeTextElement;
    bool            UseTabs;                // use tabs to highlight level
    bool            UseSpacesInsteadOfTab;  // use spaces instead of tabs
    int             TabWidth;               // tab width in characters
    unsigned int    TBinBlockSize;          // size of bin block per line
    // linear elements
    std::vector<CSmallString>   LinearElements;
    std::vector<CSmallString>   PreElements;

// section of private methods -------------------------------------------------
    void PrintXML(void);

    void PrintTXMLNode(const CXMLNode* p_node,int level);
    void PrintTXMLDocument(const CXMLDocument* p_document,int level);
    void PrintTXMLDeclaration(const CXMLDeclaration* p_declaration,int level);
    void PrintTXMLUnknown(const CXMLUnknown* p_unknown,int level);
    void PrintTXMLComment(const CXMLComment* p_comment,int level);
    void PrintTXMLElement(const CXMLElement* p_element,int level);
    void PrintTXMLText(const CXMLText* p_text,int level);
    void PrintTXMLBinData(const CXMLBinData* p_bindata,int level);
    static void EncodeXMLText(const CSmallString& input,CSmallString& output);
    void SetTab(int level);

    void PrintBXMLHeader(void);
    void PrintBXMLNode(const CXMLNode* p_node);
    void PrintBXMLDocument(const CXMLDocument* p_document);
    void PrintBXMLDeclaration(const CXMLDeclaration* p_declaration);
    void PrintBXMLUnknown(const CXMLUnknown* p_unknown);
    void PrintBXMLComment(const CXMLComment* p_comment);
    void PrintBXMLElement(const CXMLElement* p_element);
    void PrintBXMLText(const CXMLText* p_text);
    void PrintBXMLBinData(const CXMLBinData* p_bindata);
};

// -----------------------------------------------------------------------------

#endif
