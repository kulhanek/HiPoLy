#ifndef MemBlockFileH
#define MemBlockFileH
//==============================================================================
//    Copyright 2003,2004,2005 Petr Kulhanek
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
#include <SimpleList.hpp>
#include <SimpleIterator.hpp>

//------------------------------------------------------------------------------

/*! \brief CMemBlockFile - fast memory stream
 *
 *  Fast memory stream for data write operations. It is block orientated.
 */

class HIPOLY_PACKAGE CMemBlockFile {
public:
    /// Constructor
    /*! Init first memory block for writing.
    */
    CMemBlockFile(void);
    /// Destructor
    /*! Release allocated memory blocks.
    */
    ~CMemBlockFile(void);

    /// get first memory block and its size
    char* GetFirstMemBlock(unsigned int& size);

    /// get next memory block and its size
    char* GetNextMemBlock(unsigned int& size);

    /// return pointer to one memory block contaning data
    /*! user is responsible to free alocated memory with delete[] operator
    */
    unsigned char* GetDataAsOneBlock(unsigned int& length);

    /// release allocated memory blocks and prepare for new writing
    void Clear(void);

    /// returns the total length of stream
    unsigned int GetTotalLength(void);

    /// write data to file
    bool  SaveToFile(FILE* p_file);

    // --------------------------------------------------------------------

    /// write whole string (CSmallString) to stream
    void WriteString(const CSmallString& string);

    /// write whole string (char*) to stream
    void WriteString(const char* p_str);

    /// write one character to stream
    void WriteChar(const char z);

    /// write one character to stream
    void WriteUChar(const unsigned char z);

    /// write short binary number to stream
    void WriteUShort(const unsigned short z);

    /// write short binary number to stream
    void WriteUInt(const unsigned int z);

    /// write binary data in HEX ASCII coding
    void WriteDataBlockInHex(const void* p_idata,unsigned int length);

    /// write binary data to stream
    void WriteDataBlock(const void* p_idata,unsigned int length);

// section of private data ----------------------------------------------------
private:
    unsigned int            MaxBlockSize;
    unsigned int            TotalSize;
    char*                   ActiveBlock;
    unsigned int            BlockPosition;

    CSimpleList<char>       Blocks;
    CSimpleIterator<char>   I;

    void AllocateNewBlock(void);
};

//---------------------------------------------------------------------------

#endif
