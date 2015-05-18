#ifndef XMLBinDataH
#define XMLBinDataH
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

// -----------------------------------------------------------------------------

enum EXMLBinDataType {
    EXBDT_UNKNOWN,
    EXBDT_CHAR,
    EXBDT_SHORT,
    EXBDT_INT,
    EXBDT_LONG,
    EXBDT_FLOAT,
    EXBDT_DOUBLE,
    EXBDT_LONG_DOUBLE
};

// -----------------------------------------------------------------------------

/// \brief define XML binary data node

class HIPOLY_PACKAGE CXMLBinData : public CXMLElement {
public:
// constructor and destructor -------------------------------------------------
    CXMLBinData(CXMLNode* p_parent);
    virtual ~CXMLBinData(void);

// get methods ----------------------------------------------------------------
    /// return type of data
    EXMLBinDataType GetDataType(void) const;

    /// return length of data (length in unsigned char)
    long unsigned int GetLength(void) const;

    /// return length of data of type
    template <class type>
    inline long unsigned int GetLength(void) const;

    /// return true if own data
    bool IsOwnerOfData(void) const;

    /// return pointer to binary data
    void* GetData(void) const;

    /// return pointer to binary data
    template <class type>
    inline type* GetData(void) const;

    /// return pointer to copy of binary data, use delete[] to destroy this block
    unsigned char* GetDataAsCopy(void) const;

    /// return pointer to binary data, use delete[] to destroy this block
    unsigned char* ReleaseData(void);

// set methods ----------------------------------------------------------------
    /// allocate new data array, previous data are desroyed (length in unsigned char)
    void SetLength(long unsigned int length,EXMLBinDataType type);

    /// set binary data as copy, previous data are destroyed (length in unsigned char)
    void CopyData(void* p_data,long unsigned int length,
                    EXMLBinDataType type=EXBDT_UNKNOWN);

    /// set binary data, previous data are destroyed (length in unsigned char)
    void SetData(void* p_data,long unsigned int length,
                    bool own = false,
                    EXMLBinDataType type=EXBDT_UNKNOWN);

    /// destroy data
    void FreeData(void);

// section of private data ----------------------------------------------------
private:
    long unsigned int   Length;     // data length in unsigned char
    bool                Owner;      // is owner of block
    EXMLBinDataType     Type;       // data type
    unsigned char*      DataArray;  // binary data

    // update attributes
    void UpdateHeader(void);

    // restore type
    void RestoreType(void);

    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);

    friend class CXMLParser;
};

// -----------------------------------------------------------------------------

template <class type>
inline long unsigned int CXMLBinData::GetLength(void) const
{
    if(sizeof(type) == 0 ) return(Length);
    return(Length/sizeof(type));
}

// -----------------------------------------------------------------------------

template <class type>
inline type* CXMLBinData::GetData(void) const
{
    return((type*)DataArray);
}

// -----------------------------------------------------------------------------

#endif

