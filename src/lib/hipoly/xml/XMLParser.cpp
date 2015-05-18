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
#include <ctype.h>
#include <ErrorSystem.hpp>
#include <XMLParser.hpp>
#include <XMLDeclaration.hpp>
#include <XMLUnknown.hpp>
#include <XMLComment.hpp>
#include <XMLElement.hpp>
#include <XMLText.hpp>
#include <XMLBinData.hpp>
#include <XMLBinary.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLParser::CXMLParser(void)
{
    Length = 0;
    Data = NULL;
    Pos = 0;
    Pointer = NULL;
    OwnData = false;
    OutputNode = NULL;
    SkipWhiteCharacters = true;
    TranscodeTextElement = false;
}

//------------------------------------------------------------------------------

CXMLParser::~CXMLParser(void)
{
    DestroyData();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLParser::SetOutputXMLNode(CXMLNode* p_node)
{
    OutputNode = p_node;
}

//------------------------------------------------------------------------------

void CXMLParser::EnableWhiteCharacters(bool set)
{
    SkipWhiteCharacters = !set;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLParser::Parse(const CSmallString& name)
{
    FILE* p_file;

    if( (p_file = fopen(name,"rb")) == NULL ) {
        CSmallString error;
        error << "unable to open file '" << name << "' for reading (" << strerror(errno) << ")";
        ES_ERROR(error);
        return(false);
    }

    if( Parse(p_file) == false ) {
        ES_ERROR("unable to parse XML from file");
        fclose(p_file);
        return(false);
    }

    fclose(p_file);

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::Parse(FILE* p_file)
{
    if( p_file == NULL ) {
        ES_ERROR("p_file is NULL");
        return(false);
    }

    // destroy previous data, if any ----------------
    DestroyData();

    // get size of file -----------------------------
    fseek(p_file,0,SEEK_END);
    Length = ftell(p_file);
    fseek(p_file,0,SEEK_SET);

    if( Length == 0 ) {
        ES_ERROR("XML file has zero size");
        return(false);
    }

    // allocate memory
    try {
        Data = new unsigned char[Length];
    } catch(...) {
        CSmallString error;
        error << "unable to allocate memory for file (size: " << CSmallString(Length) << ")";
        ES_ERROR(error);
        Length = 0;
        return(false);
    }

    if( fread(Data,sizeof(unsigned char),Length,p_file) != Length ) {
        DestroyData();
        ES_ERROR("unable to read data from XML file");
        return(false);
    }

    Pos = 0;
    Pointer = Data;
    OwnData = true;

    if( ParseXML() == false ) {
        ES_ERROR("unable to parse data");
        return(false);
    }

    // destroy previous data, if any ----------------
    DestroyData();

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::Parse(void* p_data,unsigned int length)
{
    if( p_data == NULL ) {
        ES_ERROR("p_data is NULL");
        return(false);
    }

    if( length == 0 ) {
        ES_ERROR("length is zero");
        return(false);
    }

    // destroy previous data, if any ----------------
    DestroyData();

    Length = length;
    Data = (unsigned char*)p_data;
    Pos = 0;
    Pointer = Data;
    OwnData = false;

    if( ParseXML() == false ) {
        ES_ERROR("unable to parse data");
        return(false);
    }

    // destroy previous data, if any ----------------
    DestroyData();

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLParser::DestroyData(void)
{
    Length = 0;
    if( (OwnData == true) && (Data != NULL) ) delete[] Data;
    Data = NULL;
    Pos = 0;
    Pointer = NULL;
    OwnData = false;
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseXML(void)
{
    if( OutputNode == NULL ) {
        ES_ERROR("OutputNode is NULL");
        return(false);
    }

    if( Pointer == NULL ) {
        ES_ERROR("Pointer is NULL");
        return(false);
    }

    // is it BXML?
    if( IsBXML() == true ) {
        return( ParseBXML() );
    } else {
        return( ParseTXML() );
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLParser::ParseTXML(void)
{
    // first parse XML as pure XML
    if( ParseTXMLNode(OutputNode) == false ) {
        ES_ERROR("unable to parse TXML document");
        return(false);
    }

    // then transform XMLElements with binary data to XMLBinData elements
    if( DecodeBinData(OutputNode) == false ) {
        ES_ERROR("unable to decode binary elements");
        return(false);
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseTXMLNode(CXMLNode* p_parent)
{
    if( p_parent == NULL ) {
        ES_ERROR("parent element is NULL");
        return(false);
    }

    while( Pos < Length ) {
        // skip white spaces
        if( SkipWhiteCharacters )  SkipWhiteSpaces();
        if( Pos >= Length ) return(true);   // end of stream

        // is it text element?
        if( *Pointer != '<' ) {
            CXMLText* p_text = p_parent->CreateChildText(!SkipWhiteCharacters);
            if( p_text == NULL ) {
                ES_ERROR("unable to create text element");
                return(false);
            }
            if( ParseTXMLText(p_text) == false ) {
                ES_ERROR("unable to parse text element");
                return (false);
            }
            continue;
        }

        // determine element type, move one character ahead
        Pointer++;
        Pos++;
        if( Pos >= Length ) {
            ES_ERROR("incomplete element tag specification");
            return(false);
        }

        switch( *Pointer ) {
        case '?': {
            // declaration
            CXMLDeclaration* p_declaration = p_parent->CreateChildDeclaration();
            if( p_declaration == NULL ) {
                ES_ERROR("unable to create declaration element");
                return(false);
            }
            if( ParseTXMLDeclaration(p_declaration) == false ) {
                ES_ERROR("unable to parse declaration element");
                return (false);
            }
        }
        break;
        case '!': {
            // parse comment or some else - for now this is only comment
            // do we have two more characters in the stream?
            if( (Pos + 2 < Length) && (strncmp((const char*)Pointer, "!--", 3) == 0) ) {
                Pointer += 3;
                Pos += 3;
                CXMLComment* p_comment = p_parent->CreateChildComment();
                if( p_comment == NULL ) {
                    ES_ERROR("unable to create comment element");
                    return(false);
                }
                if( ParseTXMLComment(p_comment) == false ) {
                    ES_ERROR("unable to parse comment element");
                    return (false);
                }
            } else {
                // some other element begining with <! - will be decoded as CXMLUnknown
                CXMLUnknown* p_unknown = p_parent->CreateChildUnknown();
                if( p_unknown == NULL ) {
                    ES_ERROR("unable to create unknown element");
                    return(false);
                }
                if( ParseTXMLUnknown(p_unknown) == false ) {
                    ES_ERROR("unable to parse unknown element");
                    return (false);
                }
            }
        }
        break;
        case '/':
            Pointer--;
            Pos--;
            return(true);
        default: {
            // other symbol implies classic element
            CXMLElement* p_element = p_parent->CreateChildElement("");
            if( p_element == NULL ) {
                ES_ERROR("unable to create element");
                return(false);
            }
            if( ParseTXMLElement(p_element) == false ) {
                ES_ERROR("unable to parse element");
                return (false);
            }
        }
        break;
        }
    }

    return (true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseTXMLDeclaration(CXMLDeclaration* p_declaration)
{
    if( p_declaration == NULL ) {
        ES_ERROR("p_declaration is null");
        return (false);
    }

    // read header
    if( (Length-Pos < 4) || strncmp((const char*)Pointer,"?xml",4) != 0) {
        ES_ERROR("declaration element has to contain ?xml");
        return (false);
    }
    Pointer += 4;
    Pos += 4;

    SkipWhiteSpaces();
    if( Pos >= Length ) {
        ES_ERROR("unexpected end of data");
        return false;
    }

    // decode attributes
    while( (Pos < Length) && (*Pointer != '?') ) {
        CSmallString    name;
        CSmallString    value;

        if( ParseAttribute(name,value) == false ) {
            ES_ERROR("unable to parse attribute");
            return (false);
        }

        // TODO: check for doubled attributes?
        if( name == "version" ) {
            p_declaration->Version = value;
        } else if( name == "encoding" ) {
            p_declaration->Encoding = value;
        } else if( name == "standalone" ) {
            p_declaration->Standalone = value;
        } else {
            CSmallString error;
            error << "'" << name << "' is unsupported attribute in declaration element";
            ES_ERROR(error);
            return (false);
        }
    }

    // finaly read end tag, white spaces were already skipped
    if( (Length-Pos < 2) || (strncmp((const char*)Pointer, "?>", 2) != 0) ) {
        ES_ERROR("declaration end tag is missing");
        return (false);
    }

    Pointer += 2;
    Pos += 2;

    return (true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseTXMLComment(CXMLComment* p_comment)
{
    if( p_comment == NULL ) {
        ES_ERROR("p_comment is NULL");
        return(false);
    }

    // pointer already points to first symbol after <!--
    unsigned char* p_begining = Pointer;
    unsigned int   initial_pos = Pos;

    // find the end of comment
    while( (Length - Pos >= 3) && (strncmp((const char*)Pointer, "-->", 3) != 0) ) {
        Pos++;
        Pointer++;
    }

    // end?
    if( Length - Pos < 3 ) {
        ES_ERROR("unexpected end of data");
        return (false);
    }

    // get as plain text
    if( PlainXMLText(p_begining,Pos-initial_pos,p_comment->Comment) == false ) {
        ES_ERROR("unable to copy XML text");
        return (false);
    }

    // skip final tag
    Pointer += 3;
    Pos += 3;

    return (true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseTXMLText(CXMLText* p_text)
{
    if( p_text == NULL ) {
        ES_ERROR("p_text is NULL");
        return(false);
    }

    unsigned char* p_begining = Pointer;
    unsigned int   initial_pos = Pos;
    unsigned int   length = 0;

    // find the end of text
    while( *Pointer != '<' ) {
        if( SkipWhiteCharacters ){
            if( ! IsWhite() ) length = Pos-initial_pos+1;
        } else {
            length = Pos-initial_pos+1;
        }
        Pos++;
        Pointer++;

        // end?
        if( Pos >= Length ) {
            if( ! SkipWhiteCharacters ) break;
            ES_ERROR("unexpected end of data");
            return (false);
        }
    }

    if( TranscodeTextElement ) {
        // decode as XML text
        if( DecodeXMLText(p_begining,length,p_text->Text) == false ) {
            ES_ERROR("unable to decode XML text");
            return (false);
        }
    } else {
        // decode as XML text
        if( PlainXMLText(p_begining,length,p_text->Text) == false ) {
            ES_ERROR("unable to decode XML text");
            return (false);
        }
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseTXMLElement(CXMLElement* p_element)
{
    // read element name
    unsigned char* p_begining = Pointer;
    unsigned int   initial_pos = Pos;

    // find end character
    while( ! (IsWhite() || *Pointer == '/' || *Pointer == '>') ) {
        Pointer++;
        Pos++;
        if( Pos >= Length ) {
            ES_ERROR("unexpected end of data");
            return false;
        }
    }

    // now decode element name to name
    if( PlainXMLText(p_begining,Pos-initial_pos,p_element->Name) == false ) {
        ES_ERROR("unable to decode XML text");
        return (false);
    }

    // read attributes if any -----------------------------------------------------
    SkipWhiteSpaces();
    if( Pos >= Length ) {
        ES_ERROR("unexpected end of data");
        return false;
    }

    while( (Pos < Length) && !((*Pointer == '>') || (*Pointer == '/')) ) {
        CSmallString    attr_name,attr_value;

        if( ParseAttribute(attr_name,attr_value) == false ) {
            ES_ERROR("unable to parse attribute");
            return false;
        }

        CXMLAttribute* p_attr = p_element->CreateAttribute();
        if( p_attr == NULL ) {
            ES_ERROR("unable to create attribute");
            return false;
        }

        p_attr->Name = attr_name;
        p_attr->Value = attr_value;
    }

    if( Pos >= Length ) {
        ES_ERROR("unexpected end of data");
        return false;
    }

    // does it contain any child elements?
    if( (Length - Pos >= 2) && (strncmp((const char*)Pointer,"/>",2) == 0) ) {
        // end of element
        Pointer += 2;
        Pos += 2;
        return(true);
    }

    if( *Pointer != '>' ) {
        CSmallString error;
        error << "> is required but " << (char)*Pointer << " was found";
        ES_ERROR(error);
        return(false);
    }

    // move after >
    Pointer++;
    Pos++;

    // construct end tag, end tag is: </name>
    CSmallString end_tag;
    end_tag << "</" << p_element->GetName() << ">";

    // parse child elements?
    while( (Length - Pos >= end_tag.GetLength()) &&
            (strncmp((const char*)Pointer,end_tag,end_tag.GetLength()) != 0) ) {

        // check for tag crossing
        if(*Pointer == '<' && *(Pointer+1) == '/' ) {
            CSmallString error;
            error << "tag crossing before " << end_tag;
            ES_ERROR(error);
            return(false);
        }
        if( ParseTXMLNode(p_element) == false ) {
            ES_ERROR("unable to parse child element");
            return(false);
        }
    }

    Pointer += end_tag.GetLength();
    Pos += end_tag.GetLength();

    return(true);
}

//-----------------------------------------------------------------------------

bool CXMLParser::ParseAttribute(CSmallString& name,CSmallString& value)
{
    // this is called when *Pointer is not white space

    // attribute name
    unsigned char* p_begining = Pointer;
    unsigned int   initial_pos = Pos;

    // find end character
    while( ! (IsWhite() || *Pointer == '=') ) {
        Pointer++;
        Pos++;
        if( Pos >= Length ) {
            ES_ERROR("unexpected end of data");
            return(false);
        }
    }

    // now decode element name to attribute name
    if( PlainXMLText(p_begining,Pos-initial_pos,name) == false ) {
        ES_ERROR("unable to decode XML text");
        return(false);
    }

    // skip white spaces between attr_name and =
    SkipWhiteSpaces();
    if( Pos >= Length ) {
        ES_ERROR("unexpected end of data");
        return(false);
    }

    // skip =
    if( *Pointer != '=' ) {
        CSmallString error;
        error << "= is required but " << (char)*Pointer << " was found";
        ES_ERROR(error);
        return(false);
    }
    Pointer++;
    Pos++;

    // skip white spaces between = and attr_value
    SkipWhiteSpaces();
    if( Pos >= Length ) {
        ES_ERROR("unexpected end of data");
        return(false);
    }

    char end_char;
    // is it "?
    if( ! (*Pointer == '"' || *Pointer == '\'')) {
        CSmallString error;
        error << "\" is required but " << (char)*Pointer << " was found";
        ES_ERROR(error);
        return(false);
    }
    end_char = *Pointer;
    Pointer++;
    Pos++;

    // attribute value
    p_begining = Pointer;
    initial_pos = Pos;

    // find end character
    while( ! (*Pointer == end_char) ) {
        Pointer++;
        Pos++;
        if( Pos >= Length ) {
            ES_ERROR("unexpected end of data");
            return(false);
        }
    }

    // now decode element value to attribute value
    if( DecodeXMLText(p_begining,Pos-initial_pos,value) == false ) {
        ES_ERROR("unable to decode XML text");
        return(false);
    }

    // move after "
    Pointer++;
    Pos++;

    SkipWhiteSpaces();

    return(true);
}

//-----------------------------------------------------------------------------

void CXMLParser::SkipWhiteSpaces(void)
{
    while( (Pos < Length) && IsWhite() == true ) {
        Pointer++;
        Pos++;
    }
}

//-----------------------------------------------------------------------------

bool CXMLParser::IsWhite(void)
{
    return( isspace(*Pointer) != 0 );
}

//-----------------------------------------------------------------------------

bool CXMLParser::PlainXMLText(unsigned char* p_string,unsigned int len,
        CSmallString& output)
{
    if( (p_string == NULL) || (len < 0 ) ) {
        ES_ERROR("illegal input data");
        return(false);
    }

    // set length
    output.SetLength(len);
    if( len == 0 ) return(true);

    // copy data
    memcpy(output.GetBuffer(),p_string,len);
    output[len] = '\0';

    return(true);
}

//-----------------------------------------------------------------------------

bool CXMLParser::DecodeXMLText(unsigned char* p_string,unsigned int len,
        CSmallString& output)
{
    /*! @RETURN decoded character or 0 for error
    &lt;    (<)
    &amp;   (&)
    &gt;    (>)
    &quot;  (")
    &apos;  (')
    &nbsp;
    &copy;
    &middot;
    */

    // calculate length of string
    unsigned int new_len = 0;

    for(unsigned int i = 0; i < len; i++) {
        if(p_string[i] == '&') {
            if( strncmp((const char*)&p_string[i],"&copy;",6) == 0) {
                new_len += 2;
                i += 5;
            } else {
                unsigned int j=i;
                bool found = true;
                do {
                    j++;
                    if( j > len ) {
                        found = false;
                        //ES_ERROR("cannot decode text, terminal ';' character was not found");
                        //return(false);
                    }
                } while(p_string[j] != ';');
                if( found ){
                    i = j;
                }
            }
        }
        new_len++;
    }

    // allocate output length
    output.SetLength(new_len);

    // decode and store in output
    unsigned int output_pos = 0;
    for(unsigned int i = 0; i < len; i++) {
        if (p_string[i] == '&') {
            if( (len - i >= 4) && (strncmp((const char*)&p_string[i],"&lt;",4) == 0) ) {
                output[output_pos] = '<';
                i+=4-1;/*one is added in cycle*/
            } else if( (len - i >= 4) && (strncmp((const char*)&p_string[i],"&gt;",4) == 0) ) {
                output[output_pos] = '>';
                i+=4-1;
            } else if( (len - i >= 5) && (strncmp((const char*)&p_string[i],"&amp;",5) == 0) ) {
                output[output_pos] = '&';
                i+=5-1;
            } else if( (len - i >= 6) && (strncmp((const char*)&p_string[i],"&quot;",6) == 0) ) {
                output[output_pos] = '"';
                i+=6-1;
            } else if( (len - i >= 6) && (strncmp((const char*)&p_string[i],"&apos;",6) == 0) ) {
                output[output_pos] = '\'';
                i+=6-1;
            } else if( (len - i >= 6) && (strncmp((const char*)&p_string[i],"&nbsp;",6) == 0) ) {
                output[output_pos] = ' ';
                i+=6-1;
            } else if( (len - i >= 6) && (strncmp((const char*)&p_string[i],"&copy;",6) == 0) ) {
                output[output_pos++] = '(';
                output[output_pos++] = 'c';
                output[output_pos] = ')';
                i+=6-1;
            } else if( (len - i >= 6) && (strncmp((const char*)&p_string[i],"&middot;",8) == 0) ) {
                output[output_pos] = '.';
                i+=8-1;
            } else {
                //ES_ERROR("can not decode - ilegal \'&\'");
                //return(false);
                output[output_pos] = p_string[i];
                // no hit but probably ilegal '&' - put it into the stream
            }
        } else {
            output[output_pos] = p_string[i];
        }
        output_pos++;
    }

    return(true);
}

//-----------------------------------------------------------------------------

/*! Unknown element structure: <!data space>
    Pointer is on entrance pointing to '!'
*/

bool CXMLParser::ParseTXMLUnknown(CXMLUnknown* p_unknown)
{
    // move back and include <!
    Pointer--;
    Pos--;

    // calculate data length that wil be copied to XMLUnknown
    int unknownLen = 0;
    while( Pointer[unknownLen] != '>' ) {
        unknownLen++;
        if(Pos + unknownLen >= Length) {
            ES_ERROR("unexpected end of data - probably \'>\' is missing ");
            return(false);
        }
    }

    // set pointer behind read block and behind final '>'
    unknownLen++;

    // copy data
    p_unknown->Unknown.SetLength(unknownLen);
    memcpy(p_unknown->Unknown.GetBuffer(),Pointer,unknownLen);

    Pointer += unknownLen;
    Pos += unknownLen;

    return (true);
}

//-----------------------------------------------------------------------------

bool CXMLParser::DecodeBinData(CXMLNode* p_node)
{
    if( p_node == NULL ) {
        ES_ERROR("p_node is NULL");
        return(false);
    }

    // search whole structure in p_node for elements saved
    // as XMLElement with bin_data="on" length="xx" type="xy"
    // and replace them by XMLBinData

    // is p_node XMLElement or XMLDocument?
    if( (p_node->GetNodeType() != EXNT_ELEMENT) && (p_node->GetNodeType() != EXNT_DOCUMENT) ) {
        // is neither XMLElement nor XMLDocument - return
        return(true);
    }

    // is element?
    if( p_node->GetNodeType() == EXNT_ELEMENT ) {
        // it is element -> is BinData?
        CSmallString value;
        CXMLElement* p_element = dynamic_cast<CXMLElement*>(p_node);
        if( p_element != NULL ) {
            p_element->GetAttribute("bin_data",value);
        }
        if( (p_element != NULL) && (value == "on") ) {
            // this element should be transformed to XMLBinData
            return(TransformToBinData(p_element));
        }
    }

    // XMLElement or XMLDocument without BinData -> check children recursively
    // for each child run DecodeBinData recursively
    CXMLNode* p_child =  p_node->GetFirstChildNode();
    CXMLNode* p_next;

    while( p_child != NULL ) {
        // DecodeBinData can delete p_node so we have to get next sibling node in advance
        p_next = p_child->GetNextSiblingNode();
        if( DecodeBinData(p_child) == false ) {
            CSmallString error;
            error << "node transformation was not successful";
            ES_ERROR(error);
            return(false);
        }
        p_child = p_next;
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::TransformToBinData(CXMLElement* p_element)
{
    if( p_element == NULL ) {
        ES_ERROR("p_element is NULL");
        return(false);
    }

    // 1) create new siblink XMLBinData element, copy attributes
    CXMLBinData* p_bin_data;
    p_bin_data = p_element->CreateSiblingBinData(p_element->GetName());

    if( p_bin_data == NULL ) {
        ES_ERROR("unable to create sibling BinData node");
        return(false);
    }

    // copy attributes
    CXMLAttribute* p_attr = p_element->GetFirstAttribute();
    while( p_attr != NULL ) {
        p_bin_data->SetAttribute(p_attr->Name, p_attr->Value);
        p_attr = p_attr->GetNextSiblingAttribute();
    }

    unsigned int length = 0;
    p_element->GetAttribute("length", length);

    // data are saved as single child CXMLText node
    // do we have them?
    if( (p_element->GetNumberOfChildNodes() != 1) ||
            (p_element->GetFirstChildNode()->GetNodeType() != EXNT_TEXT) ) {
        CSmallString error;
        error << "BinData element (" << p_element->GetName()
              << ") can contain only single XMLText node ("
              << p_element->GetNumberOfChildNodes() << ")";
        ES_ERROR(error);
        return(false);
    }


    const CXMLText* p_text_code = dynamic_cast<const CXMLText*>(p_element->GetFirstChildNode());
    const char* sourceText = p_text_code->GetText().GetBuffer();
    unsigned int sourceLen = p_text_code->GetText().GetLength();

    // calculate length of result b;
    unsigned int binLen = 0;
    for(unsigned int i = 0; i < sourceLen; i++) {
        if( isxdigit(sourceText[i]) != 0 ) {
            binLen++;
        }
    }

    // every byte is composed from two characters
    binLen = binLen / 2;

    // is this number same as length?
    if( length != binLen ) {
        CSmallString error;
        error << "length mismatch: attribute length = " << (int)length << "; data length = " << (int)binLen;
        ES_ERROR(error);
        return(false);
    }

    // allocate data
    try {
        p_bin_data->DataArray = new unsigned char[length];
    } catch(...) {
        ES_ERROR("unable to allocate array for bin data");
        return(false);
    }

    p_bin_data->Length = length;
    p_bin_data->Owner = true;
    p_bin_data->RestoreType();

    // decode data
    unsigned char a, b, result;
    unsigned int binPos = 0;

    for(unsigned int i = 0; i < sourceLen; ) {
        while( (i < sourceLen) && (isspace(sourceText[i]) != 0) ) {
            i++;
        }
        if( (i < sourceLen) && (isxdigit(sourceText[i]) != 0) ) {
            a = sourceText[i];
            // move to second symbol
            i++;
        } else {
            CSmallString error;
            error << "hexadecimal required, but '" << sourceText[i] << "', found (1)";
            ES_ERROR(error);
            return(false);
        }
        // skip white spaces
        while( (i < sourceLen) && (isspace(sourceText[i]) != 0) ) {
            i++;
        }
        // second symbol read
        if( (i < sourceLen) && (isxdigit(sourceText[i]) != 0) ) {
            b = sourceText[i];
            // move to other byte
            i++;
        } else {
            CSmallString error;
            error << "hexadecimal required, but '" << sourceText[i] << "', found (2)";
            ES_ERROR(error);
            return(false);
        }

        // 2) decode a & b to result
        // decode a
        a = toupper(a);
        b = toupper(b);
        if( isdigit(a) == 0 ) {
            result = (a-'A'+10)*16;
        } else {
            result = (a-'0')*16;
        }
        // decode b
        if( isdigit(b) == 0 ) {
            result += (b-'A'+10);
        } else {
            result += (b - '0');
        }
        // 3) write result to binOut
        p_bin_data->DataArray[binPos++] = result;
    }

    // finaly delete source node
    delete p_element;

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLParser::IsBXML(void)
{
    if( Length - Pos < sizeof(CBXMLHeader) ) return(false);

    // data are long enough, map them to CBXMLHeader
    // and check if it is BXML file
    CBXMLHeader* p_header = (CBXMLHeader*)Pointer;
    return( p_header->IsBXML() );
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXML(void)
{
    CBXMLHeader* p_header = (CBXMLHeader*)Pointer;

    // check BXML version
    if( p_header->GetVersion() != BXML_VERSION ) {
        CSmallString error;
        error << "BXML version mismatch (required: " << CSmallString(BXML_VERSION) << ", provided: " << CSmallString(p_header->GetVersion()) << ")";
        ES_ERROR(error);
        return(false);
    }

    // move in stream
    Pos += sizeof(CBXMLHeader);
    Pointer += sizeof(CBXMLHeader);

    return( ParseBXMLNode(OutputNode) );
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLNode(CXMLNode* p_parent)
{
    while(Pos < Length ) {
        Pos++;
        unsigned char node_code = *Pointer++;

        switch(node_code) {
        case BXML_DECLARATION_TAG: {
            CXMLDeclaration* p_declaration = p_parent->CreateChildDeclaration();
            if( p_declaration == NULL ) {
                ES_ERROR("unable to create declaration element");
                return(false);
            }
            if( ParseBXMLDeclaration(p_declaration) == false ) {
                ES_ERROR("unable to parse declaration element");
                return(false);
            }
        }
        break;
        case BXML_ELEMENT_STAG: {
            CXMLElement* p_element = p_parent->CreateChildElement("VOID");
            if( p_element == NULL ) {
                ES_ERROR("unable to create element");
                return(false);
            }
            if( ParseBXMLElement(p_element) == false ) {
                ES_ERROR("unable to parse element");
                return(false);
            }
        }
        break;
        case BXML_ELEMENT_ETAG:
            return(true);
        case BXML_BIN_DATA_STAG: {
            CXMLBinData* p_bindata = p_parent->CreateChildBinData("VOID");
            if( p_bindata == NULL ) {
                ES_ERROR("unable to create bin data element");
                return(false);
            }
            if( ParseBXMLBinData(p_bindata) == false ) {
                ES_ERROR("unable to parse bin data element");
                return(false);
            }
        }
        break;
        case BXML_UNKNOWN_TAG: {
            CXMLUnknown* p_unknown = p_parent->CreateChildUnknown();
            if( p_unknown == NULL ) {
                ES_ERROR("unable to create unknown element");
                return(false);
            }
            if( ParseBXMLUnknown(p_unknown) == false ) {
                ES_ERROR("unable to parse unknown element");
                return(false);
            }
        }
        break;
        case BXML_COMMENT_TAG: {
            CXMLComment* p_comment = p_parent->CreateChildComment();
            if( p_comment == NULL ) {
                ES_ERROR("unable to create comment element");
                return(false);
            }
            if( ParseBXMLComment(p_comment) == false ) {
                ES_ERROR("unable to parse comment element");
                return(false);
            }
        }
        break;
        case BXML_TEXT_TAG: {
            CXMLText* p_text = p_parent->CreateChildText();
            if( p_text == NULL ) {
                ES_ERROR("unable to create text element");
                return(false);
            }
            if( ParseBXMLText(p_text) == false ) {
                ES_ERROR("unable to parse text element");
                return(false);
            }
        }
        break;
        default:
            ES_ERROR("unknown element code");
            return(false);
        }
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLDeclaration(CXMLDeclaration* p_declaration)
{
    if( p_declaration == NULL ) return(false);

    if( Length - Pos < 3*sizeof(CBXMLUShort) ) {
        ES_ERROR("uncomplete XML declaration (sizes)");
        return(false);
    }

    CBXMLUShort*   p_vlength;
    CBXMLUShort*   p_elength;
    CBXMLUShort*   p_slength;
    unsigned int   total_length = 0;

    p_vlength = (CBXMLUShort*)Pointer;
    total_length += p_vlength->Get();
    Pointer += sizeof(CBXMLUShort);
    Pos += sizeof(CBXMLUShort);

    p_elength = (CBXMLUShort*)Pointer;
    total_length += p_elength->Get();
    Pointer += sizeof(CBXMLUShort);
    Pos += sizeof(CBXMLUShort);

    p_slength = (CBXMLUShort*)Pointer;
    total_length += p_slength->Get();
    Pointer += sizeof(CBXMLUShort);
    Pos += sizeof(CBXMLUShort);

    bool result = true;

    result &= p_declaration->Version.SetLength(p_vlength->Get());
    result &= p_declaration->Encoding.SetLength(p_elength->Get());
    result &= p_declaration->Standalone.SetLength(p_slength->Get());

    if( result == false ) {
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    if( Length - Pos < total_length ) {
        ES_ERROR("uncomplete XML declaration (data)");
        return(false);
    }

    memcpy(p_declaration->Version.GetBuffer(),Pointer,p_vlength->Get());
    Pointer += p_vlength->Get();
    Pos += p_vlength->Get();

    memcpy(p_declaration->Encoding.GetBuffer(),Pointer,p_elength->Get());
    Pointer += p_elength->Get();
    Pos += p_elength->Get();

    memcpy(p_declaration->Version.GetBuffer(),Pointer,p_slength->Get());
    Pointer += p_slength->Get();
    Pos += p_slength->Get();

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLUnknown(CXMLUnknown* p_unknown)
{
    if( p_unknown == NULL ) return(false);

    if( Length - Pos < sizeof(CBXMLUInt) ) {
        ES_ERROR("uncomplete XML unknown (sizes)");
        return(false);
    }

    CBXMLUInt*   p_length;

    p_length = (CBXMLUInt*)Pointer;
    Pointer += sizeof(CBXMLUInt);
    Pos += sizeof(CBXMLUInt);

    bool result = true;

    result &= p_unknown->Unknown.SetLength(p_length->Get());

    if( result == false ) {
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    if( Length - Pos < p_length->Get() ) {
        ES_ERROR("uncomplete XML unknown (data)");
        return(false);
    }

    memcpy(p_unknown->Unknown.GetBuffer(),Pointer,p_length->Get());
    Pointer += p_length->Get();
    Pos += p_length->Get();

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLComment(CXMLComment* p_comment)
{
    if( p_comment == NULL ) return(false);

    if( Length - Pos < sizeof(CBXMLUShort) ) {
        ES_ERROR("uncomplete XML comment (sizes)");
        return(false);
    }

    CBXMLUShort*   p_length;

    p_length = (CBXMLUShort*)Pointer;
    Pointer += sizeof(CBXMLUShort);
    Pos += sizeof(CBXMLUShort);

    bool result = true;

    result &= p_comment->Comment.SetLength(p_length->Get());

    if( result == false ) {
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    if( Length - Pos < p_length->Get() ) {
        ES_ERROR("uncomplete XML comment (data)");
        return(false);
    }

    memcpy(p_comment->Comment.GetBuffer(),Pointer,p_length->Get());
    Pointer += p_length->Get();
    Pos += p_length->Get();

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLElement(CXMLElement* p_element)
{
    if( p_element == NULL ) return(false);

    if( Length - Pos < sizeof(CBXMLUShort) ) {
        ES_ERROR("uncomplete XML element (name size)");
        return(false);
    }

    CBXMLUShort*   p_length;

    p_length = (CBXMLUShort*)Pointer;
    Pointer += sizeof(CBXMLUShort);
    Pos += sizeof(CBXMLUShort);

    bool result = true;

    result &= p_element->Name.SetLength(p_length->Get());

    if( result == false ) {
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    if( Length - Pos < p_length->Get() ) {
        ES_ERROR("uncomplete XML element (name data)");
        return(false);
    }

    memcpy(p_element->Name.GetBuffer(),Pointer,p_length->Get());
    Pointer += p_length->Get();
    Pos += p_length->Get();

    // process attributes ---------------------------------------------------------
    while( Pos < Length ) {
        unsigned char node_code = *Pointer;
        if( node_code != BXML_ATTRIBUTE_TAG ) break;    // no attribute

        // move to attribute definition
        Pointer++;
        Pos++;

        // create attribute
        CXMLAttribute* p_attr = p_element->CreateAttribute();
        if( p_attr == NULL ) {
            ES_ERROR("unable to create attribute");
            return(false);
        }

        if( Length - Pos < 2*sizeof(CBXMLUShort) ) {
            ES_ERROR("uncomplete XML element (attribute sizes)");
            return(false);
        }

        // get name and value sizes
        CBXMLUShort*   p_nlength;
        CBXMLUShort*   p_vlength;
        unsigned int   total_length = 0;

        p_nlength = (CBXMLUShort*)Pointer;
        total_length += p_nlength->Get();
        Pointer += sizeof(CBXMLUShort);
        Pos += sizeof(CBXMLUShort);

        p_vlength = (CBXMLUShort*)Pointer;
        total_length += p_vlength->Get();
        Pointer += sizeof(CBXMLUShort);
        Pos += sizeof(CBXMLUShort);

        bool result = true;

        // allocate strings
        result &= p_attr->Name.SetLength(p_nlength->Get());
        result &= p_attr->Value.SetLength(p_vlength->Get());

        if( result == false ) {
            ES_ERROR("unable to allocate memory");
            return(false);
        }

        if( Length - Pos < total_length ) {
            ES_ERROR("uncomplete XML element (attribute data)");
            return(false);
        }

        // copy data
        memcpy(p_attr->Name.GetBuffer(),Pointer,p_nlength->Get());
        Pointer += p_nlength->Get();
        Pos += p_nlength->Get();

        memcpy(p_attr->Value.GetBuffer(),Pointer,p_vlength->Get());
        Pointer += p_vlength->Get();
        Pos += p_vlength->Get();
    }

    if( Pos >= Length ) {
        ES_ERROR("element end is not specified");
        return(false);
    }

    unsigned char node_code = *Pointer;
    if( node_code == BXML_ELEMENT_ETAG ) {
        Pointer++;
        Pos++;
        return(true);
    }

    // now process subelements
    if( ParseBXMLNode(p_element) == false ) {
        ES_ERROR("unable to decode subelements");
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLText(CXMLText* p_text)
{
    if( p_text == NULL ) return(false);

    if( Length - Pos < sizeof(CBXMLUInt) ) {
        ES_ERROR("uncomplete XML text (sizes)");
        return(false);
    }

    CBXMLUInt*   p_length;

    p_length = (CBXMLUInt*)Pointer;
    Pointer += sizeof(CBXMLUInt);
    Pos += sizeof(CBXMLUInt);

    bool result = true;

    result &= p_text->Text.SetLength(p_length->Get());

    if( result == false ) {
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    if( Length - Pos < p_length->Get() ) {
        ES_ERROR("uncomplete XML text (data)");
        return(false);
    }

    memcpy(p_text->Text.GetBuffer(),Pointer,p_length->Get());
    Pointer += p_length->Get();
    Pos += p_length->Get();

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLParser::ParseBXMLBinData(CXMLBinData* p_bindata)
{
    if( p_bindata == NULL ) return(false);

    p_bindata->RemoveAllAttributes(); // remove deafault attributes

    if( Length - Pos < sizeof(CBXMLUShort) ) {
        ES_ERROR("uncomplete XML bin data (name size)");
        return(false);
    }

    CBXMLUShort*   p_length;

    p_length = (CBXMLUShort*)Pointer;
    Pointer += sizeof(CBXMLUShort);
    Pos += sizeof(CBXMLUShort);

    bool result = true;

    result &= p_bindata->Name.SetLength(p_length->Get());

    if( result == false ) {
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    if( Length - Pos < p_length->Get() ) {
        ES_ERROR("uncomplete XML bin data (name data)");
        return(false);
    }

    memcpy(p_bindata->Name.GetBuffer(),Pointer,p_length->Get());
    Pointer += p_length->Get();
    Pos += p_length->Get();

    // process attributes ---------------------------------------------------------
    while( Pos < Length ) {
        unsigned char node_code = *Pointer;
        if( node_code != BXML_ATTRIBUTE_TAG ) break;    // no attribute

        // move to attribute definition
        Pointer++;
        Pos++;

        // create attribute
        CXMLAttribute* p_attr = p_bindata->CreateAttribute();
        if( p_attr == NULL ) {
            ES_ERROR("unable to create attribute");
            return(false);
        }

        if( Length - Pos < 2*sizeof(CBXMLUShort) ) {
            ES_ERROR("uncomplete XML bin data (attribute sizes)");
            return(false);
        }

        // get name and value sizes
        CBXMLUShort*   p_nlength;
        CBXMLUShort*   p_vlength;
        unsigned int   total_length = 0;

        p_nlength = (CBXMLUShort*)Pointer;
        total_length += p_nlength->Get();
        Pointer += sizeof(CBXMLUShort);
        Pos += sizeof(CBXMLUShort);

        p_vlength = (CBXMLUShort*)Pointer;
        total_length += p_vlength->Get();
        Pointer += sizeof(CBXMLUShort);
        Pos += sizeof(CBXMLUShort);

        bool result = true;

        // allocate strings
        result &= p_attr->Name.SetLength(p_nlength->Get());
        result &= p_attr->Value.SetLength(p_vlength->Get());

        if( result == false ) {
            ES_ERROR("unable to allocate memory");
            return(false);
        }

        if( Length - Pos < total_length ) {
            ES_ERROR("uncomplete XML bin data (attribute data)");
            return(false);
        }

        // copy data
        memcpy(p_attr->Name.GetBuffer(),Pointer,p_nlength->Get());
        Pointer += p_nlength->Get();
        Pos += p_nlength->Get();

        memcpy(p_attr->Value.GetBuffer(),Pointer,p_vlength->Get());
        Pointer += p_vlength->Get();
        Pos += p_vlength->Get();
    }

    // attributes were read - restore data type
    p_bindata->RestoreType();

    // read binary data ---------------------------==
    if( Pos >= Length ) {
        ES_ERROR("binary data are missing");
        return(false);
    }

    unsigned char node_code = *Pointer++;
    Pos++;
    if( node_code != BXML_BIN_ARRAY_TAG ) {
        ES_ERROR("binary data section code is missing");
        return(false);
    }

    if( Length - Pos < sizeof(CBXMLUInt) ) {
        ES_ERROR("data length is not available");
        return(false);
    }

    CBXMLUInt*   p_dlength;

    p_dlength = (CBXMLUInt*)Pointer;
    Pointer += sizeof(CBXMLUInt);
    Pos += sizeof(CBXMLUInt);

    p_bindata->Length = p_dlength->Get();

    unsigned int control_length = 0;
    p_bindata->GetAttribute("length",control_length);

    if( control_length != p_bindata->Length ) {
        ES_ERROR("control length does not match");
        return(false);
    }

    try {
        p_bindata->DataArray = new unsigned char[p_bindata->Length];
    } catch(...) {
        p_bindata->Length = 0;
        ES_ERROR("unable to allocate memory");
        return(false);
    }

    p_bindata->Owner = true;

    if( Length - Pos < p_bindata->Length ) {
        ES_ERROR("data array is incomplete");
        return(false);
    }

    memcpy(p_bindata->DataArray,Pointer,p_bindata->Length);
    Pointer += p_bindata->Length;
    Pos += p_bindata->Length;

    if( Pos >= Length ) {
        ES_ERROR("element end is not specified");
        return(false);
    }

    // node termination -----------------------------
    node_code = *Pointer;
    if( node_code == BXML_ELEMENT_ETAG ) {
        Pointer++;
        Pos++;
        return(true);
    }

    ES_ERROR("binary element cannot have nested elements");
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

