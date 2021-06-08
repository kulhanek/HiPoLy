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

#include <errno.h>
#include <string.h>
#include <XMLPrinter.hpp>
#include <ErrorSystem.hpp>
#include <XMLDocument.hpp>
#include <XMLDeclaration.hpp>
#include <XMLUnknown.hpp>
#include <XMLComment.hpp>
#include <XMLElement.hpp>
#include <XMLText.hpp>
#include <XMLBinData.hpp>
#include <XMLBinary.hpp>
#include <algorithm>

// NOTES: encoded parts are as follows:
//        a) attribute values
//        b) text elements

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLPrinter::CXMLPrinter(void)
{
    PrintedNode = NULL;
    OutputFormat = EXF_TEXT;
    UseTabs = true;
    UseSpacesInsteadOfTab = true;
    TabWidth = 4;
    TBinBlockSize = 40;
    PrintAsItIs = false;
    TranscodeTextElement = false;
}

//------------------------------------------------------------------------------

CXMLPrinter::~CXMLPrinter(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLPrinter::SetPrintedXMLNode(const CXMLNode* p_node)
{
    PrintedNode = p_node;
}

//------------------------------------------------------------------------------

void CXMLPrinter::SetOutputFormat(EXMLFormat format)
{
    OutputFormat = format;
}

//------------------------------------------------------------------------------

EXMLFormat CXMLPrinter::GetOutputFormat(void) const
{
    return(OutputFormat);
}

//------------------------------------------------------------------------------

void CXMLPrinter::SetPrintAsItIs(bool set)
{
    PrintAsItIs = set;
    UseTabs = !set;
}

//------------------------------------------------------------------------------

void CXMLPrinter::EnableTextElementTranscoding(bool set)
{
    TranscodeTextElement = set;
}

//------------------------------------------------------------------------------

void CXMLPrinter::ClearBlockElements(void)
{
    BlockElements.clear();
}

//------------------------------------------------------------------------------

void CXMLPrinter::RegisterBlockElement(const CSmallString& elem)
{
    BlockElements.push_back(elem);
}

//------------------------------------------------------------------------------

void CXMLPrinter::ClearLinearElements(void)
{
    LinearElements.clear();
}

//------------------------------------------------------------------------------

void CXMLPrinter::RegisterLinearElement(const CSmallString& elem)
{
    LinearElements.push_back(elem);
}

//------------------------------------------------------------------------------

void CXMLPrinter::ClearPreformatedElements(void)
{
    PreElements.clear();
}

//------------------------------------------------------------------------------

void CXMLPrinter::RegisterPreformatedElement(const CSmallString& elem)
{
    PreElements.push_back(elem);
}

//------------------------------------------------------------------------------

void CXMLPrinter::ClearTabResetElements(void)
{
    TabResetElements.clear();
}

//------------------------------------------------------------------------------

void CXMLPrinter::RegisterTabResetElement(const CSmallString& elem)
{
    TabResetElements.push_back(elem);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLPrinter::Print(const CSmallString& name)
{
    // copy memory stream to file
    FILE* p_file;

    if( (p_file = fopen(name,"w")) == NULL ) {
        CSmallString error;
        error << "unable to open file '" << name << "' for writing (" << strerror(errno) << ")";
        ES_ERROR(error);
        return(false);
    }

    if( Print(p_file) == false ) {
        ES_ERROR("unable to print XML to file");
        fclose(p_file);
        return(false);
    }

    fclose(p_file);
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLPrinter::Print(FILE* p_file)
{
    if( p_file == NULL ) {
        ES_ERROR("p_file is NULL");
        return(false);
    }

    // print XML to memory stream
    try {
        PrintXML();
    } catch(...) {
        ES_ERROR("unable to print file");
        return(false);
    }

    if( OutputFile.SaveToFile(p_file) == false ) {
        ES_ERROR("unable to save memory stream to file stream");
        return(false);
    }

    // destroy data in cache
    OutputFile.Clear();

    return(true);
}

//------------------------------------------------------------------------------

unsigned char*  CXMLPrinter::Print(unsigned int& length)
{
    // print XML to memory stream
    try {
        PrintXML();
    } catch(...) {
        ES_ERROR("unable to print file");
        return(NULL);
    }

    length = 0;
    unsigned char* p_data = OutputFile.GetDataAsOneBlock(length);
    if( p_data == NULL ) {
        ES_ERROR("unable to create one block data");
        return(NULL);
    }

    // destroy data in cache
    OutputFile.Clear();

    return(p_data);
}

//------------------------------------------------------------------------------

const CSmallString CXMLPrinter::Print(void)
{
    // print XML to memory stream
    try {
        PrintXML();
    } catch(...) {
        ES_ERROR("unable to print file");
        return("");
    }

    unsigned int length = 0;
    unsigned char* p_data = OutputFile.GetDataAsOneBlock(length);
    if( p_data == NULL ) {
        ES_ERROR("unable to create one block data");
        return("");
    }

    // destroy data in cache
    OutputFile.Clear();

    CSmallString buffer;
    buffer.SetLength(length);
    memcpy(buffer.GetBuffer(),p_data,length);
    delete[] p_data;

    return(buffer);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLPrinter::PrintXML(void)
{
    if( PrintedNode == NULL ) {
        LOGIC_ERROR("PrintedNode is NULL");
    }

    // remove previous contents, if any
    OutputFile.Clear();

    switch(OutputFormat) {
    case EXF_TEXT:
        PrintTXMLNode(PrintedNode,0);
        return;
    case EXF_BXML:
        PrintBXMLHeader();
        PrintBXMLNode(PrintedNode);
        return;
    default:
        LOGIC_ERROR("unknown output format");
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLPrinter::PrintTXMLNode(const CXMLNode* p_node,int level)
{
    if( p_node == NULL ){
        LOGIC_ERROR("p_node == NULL");
    }

    // determine type
    switch(p_node->GetNodeType()) {
    case EXNT_DOCUMENT: {
        const CXMLDocument* p_document = dynamic_cast<const CXMLDocument*>(p_node);
        PrintTXMLDocument(p_document,level);
    }
    break;
    case EXNT_DECLARATION: {
        const CXMLDeclaration* p_declaration = dynamic_cast<const CXMLDeclaration*>(p_node);
        PrintTXMLDeclaration(p_declaration,level);
    }
    break;
    case EXNT_UNKNOWN: {
        const CXMLUnknown* p_unknown = dynamic_cast<const CXMLUnknown*>(p_node);
        PrintTXMLUnknown(p_unknown,level);
    }
    break;
    case EXNT_COMMENT: {
        const CXMLComment* p_comment = dynamic_cast<const CXMLComment*>(p_node);
        PrintTXMLComment(p_comment,level);
    }
    break;
    case EXNT_ELEMENT: {
        const CXMLElement* p_element = dynamic_cast<const CXMLElement*>(p_node);
        PrintTXMLElement(p_element,level);
    }
    break;
    case EXNT_TEXT: {
        const CXMLText* p_text = dynamic_cast<const CXMLText*>(p_node);
        PrintTXMLText(p_text,level);
    }
    break;
    case EXNT_BIN_DATA: {
        const CXMLBinData* p_bindata = dynamic_cast<const CXMLBinData*>(p_node);
        PrintTXMLBinData(p_bindata,level);
    }
    break;
    default:
        LOGIC_ERROR("unknown XML node type");
    }
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLDocument(const CXMLDocument* p_document,int level)
{
    if( p_document == NULL ){
        LOGIC_ERROR("p_document == NULL");
    }

    // XML document does not hold any printable data,
    // it only owns child XML nodes

    CXMLNode* p_node = p_document->GetFirstChildNode();

    while( p_node != NULL ) {
        PrintTXMLNode(p_node,level);
        p_node = p_node->GetNextSiblingNode();
    }
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLDeclaration(const CXMLDeclaration* p_declaration,int level)
{
    if( p_declaration == NULL ){
        LOGIC_ERROR("p_declaration == NULL");
    }

    // indentation ----------------------------------
    if( UseTabs == true ) {
        SetTab(level);
    }

    // print comment --------------------------------
    OutputFile.WriteString("<?xml");

    // version
    if( p_declaration->GetVersion() != NULL ) {
        CSmallString output;
        EncodeXMLText(p_declaration->GetVersion(),output);
        OutputFile.WriteString(" version=\"" + output + "\"");
    }

    // encoding
    if( p_declaration->GetEncoding() != NULL ) {
        CSmallString output;
        EncodeXMLText(p_declaration->GetEncoding(),output);
        OutputFile.WriteString(" encoding=\"" + output + "\"");
    }

    // standalone flag
    if( p_declaration->GetStandalone() != NULL ) {
        CSmallString output;
        EncodeXMLText(p_declaration->GetStandalone(),output);
        OutputFile.WriteString(" standalone=\"" + output + "\"");
    }

    OutputFile.WriteString("?>");
    if( ! PrintAsItIs ) OutputFile.WriteString("\n");
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLUnknown(const CXMLUnknown* p_unknown,int level)
{
    if( p_unknown == NULL ){
        LOGIC_ERROR("p_unknown == NULL");
    }

    if( UseTabs == true ) {
        SetTab(level);
    }

    OutputFile.WriteString(p_unknown->GetUnknown());
    if( ! PrintAsItIs ) OutputFile.WriteString("\n");
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLComment(const CXMLComment* p_comment,int level)
{
    if( p_comment == NULL ){
        LOGIC_ERROR("p_comment == NULL");
    }

    // indentation ----------------------------------
    if( UseTabs == true ) {
        SetTab(level);
    }

    // print comment --------------------------------
    OutputFile.WriteString("<!--");
    if( p_comment->GetComment() != NULL ) {
        OutputFile.WriteString(p_comment->GetComment());
    }
    OutputFile.WriteString("-->");
    if( ! PrintAsItIs ) OutputFile.WriteString("\n");
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLElement(const CXMLElement* p_element,int level)
{
    if( p_element == NULL ){
        LOGIC_ERROR("p_comment == NULL");
    }

    CSmallString name = p_element->GetName();

    if( name == NULL ) {
        LOGIC_ERROR("element name is NULL");
    }

    bool _PrintAsItIs = true;
    bool _UseTabs = UseTabs;

    if( PrintAsItIs == false ){
        _PrintAsItIs = false;
        if( std::find(LinearElements.begin(), LinearElements.end(), name) != LinearElements.end() ){
            PrintAsItIs = true;
            UseTabs = false;
        }
    }

    if( std::find(PreElements.begin(), PreElements.end(), name) != PreElements.end() ){
        UseTabs = false;
    }

    if( UseTabs == true ) {
        SetTab(level);
    }

    bool BlockNode = false;

    if( PrintAsItIs == false ){
        if( std::find(BlockElements.begin(), BlockElements.end(), name) != BlockElements.end() ){
            PrintAsItIs = true;
            UseTabs = false;
            BlockNode = true;
        }
    }

    CSmallString output;

    // element name ---------------------------------
    OutputFile.WriteChar('<');
    OutputFile.WriteString(p_element->GetName());

    // do we have attributes? -----------------------
    if( p_element->HasAttributes() == true ) {
        const CXMLAttribute* p_attr = p_element->GetFirstAttribute();
        while( p_attr != NULL ) {
            OutputFile.WriteChar(' ');
            if( p_attr->Value != NULL ) {
                OutputFile.WriteString(p_attr->Name);
                OutputFile.WriteString("=\"");
                EncodeXMLText(p_attr->Value,output);
                OutputFile.WriteString(output);
                OutputFile.WriteString("\"");
            } else {
                OutputFile.WriteString(p_attr->Name);
                OutputFile.WriteString("=\"\"");
            }

            p_attr = p_attr->GetNextSiblingAttribute();
        }
    }

    int _level = level;
    if( std::find(TabResetElements.begin(), TabResetElements.end(), name) != TabResetElements.end() ){
        level = -1;
    }

    // do we have child nodes ? ---------------------
    if( p_element->HasChildNodes() == true ) {
        OutputFile.WriteString(">");
        if( ! PrintAsItIs ) OutputFile.WriteString("\n");

        CXMLNode* p_node = p_element->GetFirstChildNode();
        while( p_node != NULL ) {
            PrintTXMLNode(p_node,level+1);
            p_node = p_node->GetNextSiblingNode();
        }
    }

    level = _level;

    // terminate element block
    if( p_element->HasChildNodes() == true ) {
        if( UseTabs == true ) {
            SetTab(level);
        }
        OutputFile.WriteString("</");
        OutputFile.WriteString(p_element->GetName());
        OutputFile.WriteString(">");
        if( (! PrintAsItIs) || BlockNode ) OutputFile.WriteString("\n");
    } else {
        OutputFile.WriteString("/>");
        if( (! PrintAsItIs) || BlockNode ) OutputFile.WriteString("\n");
    }

    if( _PrintAsItIs == false ){
        PrintAsItIs = false;
    }

    UseTabs = _UseTabs;
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLText(const CXMLText* p_text,int level)
{
    if( p_text == NULL ){
        LOGIC_ERROR("p_text == NULL");
    }

    if( UseTabs == true ) {
        SetTab(level);
    }

    CSmallString output;
    if( TranscodeTextElement == true ){
        EncodeXMLText(p_text->GetText(),output);
    }
    output = p_text->GetText();
    OutputFile.WriteString(output);
    if( ! PrintAsItIs ) OutputFile.WriteString("\n");
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintTXMLBinData(const CXMLBinData* p_bindata,int level)
{
    if( p_bindata == NULL ){
        LOGIC_ERROR("p_bindata == NULL");
    }

    if( UseTabs == true ) {
        SetTab(level);
    }

    if( p_bindata->GetName() == NULL ) {
        LOGIC_ERROR("element name is NULL");
    }

    CSmallString output;

    // element name ---------------------------------
    OutputFile.WriteChar('<');
    OutputFile.WriteString(p_bindata->GetName());

    // do we have attributes? -----------------------
    if( p_bindata->HasAttributes() == true ) {
        const CXMLAttribute* p_attr = p_bindata->GetFirstAttribute();
        while( p_attr != NULL ) {
            OutputFile.WriteChar(' ');
            if( p_attr->Value != NULL ) {
                OutputFile.WriteString(p_attr->Name);
                OutputFile.WriteString("=\"");
                EncodeXMLText(p_attr->Value,output);
                OutputFile.WriteString(output);
                OutputFile.WriteString("\"");
            } else {
                OutputFile.WriteString(p_attr->Name);
                OutputFile.WriteString("=\"\"");
            }
            p_attr = p_attr->GetNextSiblingAttribute();
        }
    }

    // binary alement does not have child, but contains data
    OutputFile.WriteString(">");
    if( ! PrintAsItIs ) OutputFile.WriteString("\n");

    // now write data as XMLText

    unsigned char* p_pointer = (unsigned char*)p_bindata->GetData();

    for(unsigned int i=0; i<p_bindata->GetLength(); i++) {
        if( (UseTabs == true) && ( i % TBinBlockSize == 0 ) ) {
            SetTab(level+1);
        }
        unsigned char z = *p_pointer++;
        unsigned char a;
        a  = (z >> 4);
        if( a >= 10 ) a += 'A' - 10;
        else a += '0';
        OutputFile.WriteChar(a);
        a  = z & 0x0F;
        if( a >= 10 ) a += 'A' - 10;
        else a += '0';
        OutputFile.WriteChar(a);

        if( i % TBinBlockSize == TBinBlockSize - 1 ) {
            OutputFile.WriteChar('\n');
        }
    }

    if( (p_bindata->GetLength()-1) % TBinBlockSize != TBinBlockSize - 1 ) {
        OutputFile.WriteChar('\n');
    }

    // terminate element block
    if( UseTabs == true ) {
        SetTab(level);
    }
    OutputFile.WriteString("</");
    OutputFile.WriteString(p_bindata->GetName());
    OutputFile.WriteString(">\n");
}

//------------------------------------------------------------------------------

const CSmallString CXMLPrinter::EncodeXMLText(const CSmallString& input)
{
    CSmallString output;
    EncodeXMLText(input,output);
    return(output);
}

//------------------------------------------------------------------------------

void CXMLPrinter::EncodeXMLText(const CSmallString& input,CSmallString& output)
{
    /*! @RETURN decoded character or 0 for error
    &lt;    (<)
    &amp;   (&)
    &gt;    (>)
    &quot;  (")
    &apos;  (')
    */

    // calculate the length of encoded string
    unsigned int new_len = 0;

    for(unsigned int i = 0; i < input.GetLength(); i++) {
        switch(input[i]) {
        case '<':
            new_len += 4;
            break;
        case '&':
            new_len += 5;
            break;
        case '>':
            new_len += 4;
            break;
        case '"':
            new_len += 6;
            break;
        case '\'':
            new_len += 6;
            break;
        default:
            new_len++;
            break;
        }
    }

    // allocate output length
    output.SetLength(new_len);

    // decode and store in output
    unsigned int output_pos = 0;
    for(unsigned int i = 0; i < input.GetLength(); i++) {
        switch(input[i]) {
        case '<':
            memcpy(&output[output_pos],"&lt;",4);
            output_pos += 4;
            break;
        case '&' :
            memcpy(&output[output_pos],"&amp;",5);
            output_pos += 5;
            break;
        case '>' :
            memcpy(&output[output_pos],"&gt;",4);
            output_pos += 4;
            break;
        case '"' :
            memcpy(&output[output_pos],"&quot;",6);
            output_pos += 6;
            break;
        case '\'' :
            memcpy(&output[output_pos],"&apos;",6);
            output_pos += 6;
            break;
        default:
            output[output_pos] = input[i];
            output_pos++;
            break;
        }
    }
}

//------------------------------------------------------------------------------

void CXMLPrinter::SetTab(int level)
{
    if( UseSpacesInsteadOfTab == true ) {
        for(int i=0; i<level*TabWidth; i++) {
            OutputFile.WriteChar(' ');
        }
    } else {
        for(int i=0; i<level; i++) {
            OutputFile.WriteChar('\t');
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLPrinter::PrintBXMLHeader(void)
{
    CBXMLHeader  bxml_header;
    OutputFile.WriteDataBlock(&bxml_header,sizeof(bxml_header));
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLNode(const CXMLNode* p_node)
{
    if( p_node == NULL ){
        LOGIC_ERROR("p_node == NULL");
    }

    // determine type
    switch(p_node->GetNodeType()) {
    case EXNT_DOCUMENT: {
        const CXMLDocument* p_document = dynamic_cast<const CXMLDocument*>(p_node);
        PrintBXMLDocument(p_document);
    }
    break;
    case EXNT_DECLARATION: {
        const CXMLDeclaration* p_declaration = dynamic_cast<const CXMLDeclaration*>(p_node);
        PrintBXMLDeclaration(p_declaration);
    }
    break;
    case EXNT_UNKNOWN: {
        const CXMLUnknown* p_unknown = dynamic_cast<const CXMLUnknown*>(p_node);
        PrintBXMLUnknown(p_unknown);
    }
    break;
    case EXNT_COMMENT: {
        const CXMLComment* p_comment = dynamic_cast<const CXMLComment*>(p_node);
        PrintBXMLComment(p_comment);
    }
    break;
    case EXNT_ELEMENT: {
        const CXMLElement* p_element = dynamic_cast<const CXMLElement*>(p_node);
        PrintBXMLElement(p_element);
    }
    break;
    case EXNT_TEXT: {
        const CXMLText* p_text = dynamic_cast<const CXMLText*>(p_node);
        PrintBXMLText(p_text);
    }
    break;
    case EXNT_BIN_DATA: {
        const CXMLBinData* p_bindata = dynamic_cast<const CXMLBinData*>(p_node);
        PrintBXMLBinData(p_bindata);
    }
    break;
    default:
        LOGIC_ERROR("unknown XML node type");
    }
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLDocument(const CXMLDocument* p_document)
{
    if( p_document == NULL ){
        LOGIC_ERROR("p_document == NULL");
    }

    // XML document does not hold any printable data,
    // it only owns child XML nodes

    CXMLNode* p_node = p_document->GetFirstChildNode();

    while( p_node != NULL ) {
        PrintBXMLNode(p_node);
        p_node = p_node->GetNextSiblingNode();
    }

}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLDeclaration(const CXMLDeclaration* p_declaration)
{
    if( p_declaration == NULL ){
        LOGIC_ERROR("p_declaration == NULL");
    }

    unsigned int length;

    CBXMLUShort  version_length;
    length = p_declaration->GetVersion().GetLength();
    if( length > ((unsigned int)1 << (sizeof(version_length)*8-1)) ) {
        LOGIC_ERROR("version size overflow");
    }
    version_length.Set(length);

    CBXMLUShort  encoding_length;
    length = p_declaration->GetEncoding().GetLength();
    if( length > ((unsigned int)1  << (sizeof(encoding_length)*8-1)) ) {
        LOGIC_ERROR("encoding size overflow");
    }
    encoding_length.Set(length);

    CBXMLUShort  standalone_length;
    length = p_declaration->GetStandalone().GetLength();
    if( length > ((unsigned int)1  << (sizeof(standalone_length)*8-1)) ) {
        LOGIC_ERROR("standalone size overflow");
    }
    standalone_length.Set(length);

    OutputFile.WriteUChar(BXML_DECLARATION_TAG);
    OutputFile.WriteDataBlock(&version_length,sizeof(version_length));
    OutputFile.WriteDataBlock(&encoding_length,sizeof(encoding_length));
    OutputFile.WriteDataBlock(&standalone_length,sizeof(standalone_length));
    OutputFile.WriteString(p_declaration->GetVersion());
    OutputFile.WriteString(p_declaration->GetEncoding());
    OutputFile.WriteString(p_declaration->GetStandalone());
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLUnknown(const CXMLUnknown* p_unknown)
{
    if( p_unknown == NULL ){
        LOGIC_ERROR("p_unknown == NULL");
    }

    CBXMLUInt  unknown_length;
    unsigned int length = p_unknown->GetUnknown().GetLength();
    if( length > ((unsigned int)1  << (sizeof(unknown_length)*8-1)) ) {
        LOGIC_ERROR("unknown size overflow");
    }
    unknown_length.Set(length);

    OutputFile.WriteUChar(BXML_UNKNOWN_TAG);
    OutputFile.WriteDataBlock(&unknown_length,sizeof(unknown_length));
    OutputFile.WriteString(p_unknown->GetUnknown());
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLComment(const CXMLComment* p_comment)
{
    if( p_comment == NULL ){
        LOGIC_ERROR("p_comment == NULL");
    }

    CBXMLUShort  comment_length;
    unsigned int length = p_comment->GetComment().GetLength();
    if( length > ((unsigned int)1  << (sizeof(comment_length)*8-1)) ) {
        LOGIC_ERROR("comment size overflow");
    }
    comment_length.Set(length);

    OutputFile.WriteUChar(BXML_COMMENT_TAG);
    OutputFile.WriteDataBlock(&comment_length,sizeof(comment_length));
    OutputFile.WriteString(p_comment->GetComment());
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLElement(const CXMLElement* p_element)
{
    if( p_element == NULL ){
        LOGIC_ERROR("p_element == NULL");
    }

    unsigned int length;

    CBXMLUShort  name_length;
    length = p_element->GetName().GetLength();
    if( length > ((unsigned int)1  << (sizeof(name_length)*8-1)) ) {
        LOGIC_ERROR("element name size overflow");
    }
    name_length.Set(length);

    OutputFile.WriteUChar(BXML_ELEMENT_STAG);
    OutputFile.WriteDataBlock(&name_length,sizeof(name_length));
    OutputFile.WriteString(p_element->GetName());

    // attributes -----------------------------------
    if( p_element->HasAttributes() == true ) {
        const CXMLAttribute* p_attr = p_element->GetFirstAttribute();
        while( p_attr != NULL ) {
            CBXMLUShort  attr_name_length;
            length = p_attr->Name.GetLength();
            if( length > ((unsigned int)1  << (sizeof(attr_name_length)*8-1)) ) {
                LOGIC_ERROR("attribute name size overflow");
            }
            attr_name_length.Set(length);

            CBXMLUShort  attr_value_length;
            length = p_attr->Value.GetLength();
            if( length > ((unsigned int)1  << (sizeof(attr_value_length)*8-1)) ) {
                LOGIC_ERROR("attribute value size overflow");
            }
            attr_value_length.Set(length);

            OutputFile.WriteUChar(BXML_ATTRIBUTE_TAG);
            OutputFile.WriteDataBlock(&attr_name_length,sizeof(attr_name_length));
            OutputFile.WriteDataBlock(&attr_value_length,sizeof(attr_value_length));
            OutputFile.WriteString(p_attr->Name);
            OutputFile.WriteString(p_attr->Value);

            p_attr = p_attr->GetNextSiblingAttribute();
        }
    }

    // do we have child nodes ? ---------------------
    if( p_element->HasChildNodes() == true ) {
        CXMLNode* p_node = p_element->GetFirstChildNode();
        while( p_node != NULL ) {
            PrintBXMLNode(p_node);
            p_node = p_node->GetNextSiblingNode();
        }
    }

    // terminate element
    OutputFile.WriteUChar(BXML_ELEMENT_ETAG);
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLText(const CXMLText* p_text)
{
    if( p_text == NULL ){
        LOGIC_ERROR("p_text == NULL");
    }

    CBXMLUInt  text_length;
    unsigned int length = p_text->GetText().GetLength();
    if( length > ((unsigned int)1  << (sizeof(text_length)*8-1)) ) {
        LOGIC_ERROR("text size overflow 65535");
    }
    text_length.Set(length);

    OutputFile.WriteUChar(BXML_TEXT_TAG);
    OutputFile.WriteDataBlock(&text_length,sizeof(text_length));
    OutputFile.WriteString(p_text->GetText());
}

//------------------------------------------------------------------------------

void CXMLPrinter::PrintBXMLBinData(const CXMLBinData* p_bindata)
{
    if( p_bindata == NULL ){
        LOGIC_ERROR("p_bindata == NULL");
    }

    unsigned int length;

    CBXMLUShort  name_length;
    length = p_bindata->GetName().GetLength();
    if( length > ((unsigned int)1  << (sizeof(name_length)*8-1)) ) {
        LOGIC_ERROR("element name size overflow");
    }
    name_length.Set(length);

    OutputFile.WriteUChar(BXML_BIN_DATA_STAG);
    OutputFile.WriteDataBlock(&name_length,sizeof(name_length));
    OutputFile.WriteString(p_bindata->GetName());

    // attributes -----------------------------------
    if( p_bindata->HasAttributes() == true ) {
        const CXMLAttribute* p_attr = p_bindata->GetFirstAttribute();
        while( p_attr != NULL ) {
            CBXMLUShort  attr_name_length;
            length = p_attr->Name.GetLength();
            if( length > ((unsigned int)1  << (sizeof(attr_name_length)*8-1)) ) {
                LOGIC_ERROR("attribute name size overflow");
            }
            attr_name_length.Set(length);

            CBXMLUShort  attr_value_length;
            length = p_attr->Value.GetLength();
            if( length > ((unsigned int)1  << (sizeof(attr_value_length)*8-1)) ) {
                LOGIC_ERROR("attribute value size overflow");
            }
            attr_value_length.Set(length);

            OutputFile.WriteUChar(BXML_ATTRIBUTE_TAG);
            OutputFile.WriteDataBlock(&attr_name_length,sizeof(attr_name_length));
            OutputFile.WriteDataBlock(&attr_value_length,sizeof(attr_value_length));
            OutputFile.WriteString(p_attr->Name);
            OutputFile.WriteString(p_attr->Value);

            p_attr = p_attr->GetNextSiblingAttribute();
        }
    }

    // bin data does not have child nodes but only data array ------

    CBXMLUInt  data_length;
    length = p_bindata->GetLength();
    if( length > ((unsigned int)1  << (sizeof(data_length)*8-1)) ) {
        LOGIC_ERROR("data size overflow");
    }
    data_length.Set(length);

    OutputFile.WriteUChar(BXML_BIN_ARRAY_TAG);
    OutputFile.WriteDataBlock(&data_length,sizeof(data_length));
    OutputFile.WriteDataBlock(p_bindata->GetData(),p_bindata->GetLength());

    // terminate element
    OutputFile.WriteUChar(BXML_ELEMENT_ETAG);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
