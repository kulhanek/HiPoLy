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

#include <string.h>
#include <MemBlockFile.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CMemBlockFile::CMemBlockFile(void)
    : I(Blocks)
{
    MaxBlockSize = 4096;
    TotalSize = 0;
    ActiveBlock = NULL;
    BlockPosition = 0;
    AllocateNewBlock();
}

//---------------------------------------------------------------------------

CMemBlockFile::~CMemBlockFile(void)
{
    CSimpleIterator<char> I(Blocks);
    char* p_block;

    while((p_block = I.Current()) != NULL) {
        delete[] p_block;
        I++;
    }
    Blocks.RemoveAll();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CMemBlockFile::Clear(void)
{
    CSimpleIterator<char> I(Blocks);
    char* p_block;

    while((p_block = I.Current()) != NULL) {
        delete[] p_block;
        I++;
    }
    Blocks.RemoveAll();

    TotalSize = 0;
    ActiveBlock = NULL;
    BlockPosition = 0;
    AllocateNewBlock();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

char* CMemBlockFile::GetFirstMemBlock(unsigned int& size)
{
    I.SetToBegin();
    char* p_block = I.Current();
    if( p_block != Blocks.GetLast() ) size = MaxBlockSize;
    else   size = BlockPosition;
    I++;
    return(p_block);
}

//---------------------------------------------------------------------------

char* CMemBlockFile::GetNextMemBlock(unsigned int& size)
{
    char* p_block = I.Current();
    if( p_block != Blocks.GetLast() ) size = MaxBlockSize;
    else   size = BlockPosition;
    I++;
    return(p_block);
}

//---------------------------------------------------------------------------

void CMemBlockFile::AllocateNewBlock(void)
{
    ActiveBlock = new char[MaxBlockSize];
    Blocks.InsertToEnd(ActiveBlock);
    BlockPosition = 0;
}

//---------------------------------------------------------------------------

unsigned char* CMemBlockFile::GetDataAsOneBlock(unsigned int& length)
{
    if( TotalSize == 0 ) {
        length = 0;
        return(NULL);
    }

    unsigned char* p_data = new unsigned char[TotalSize];
    if( p_data == NULL ) {
        length = 0;
        return(NULL);
    }

    length = TotalSize;

    CSimpleIterator<char> I(Blocks);
    char* p_block;
    unsigned int   pos = 0;
    unsigned int   remlen = TotalSize;

    while((p_block = I.Current()) != NULL) {
        if( remlen > MaxBlockSize ) {
            memcpy(&p_data[pos],p_block,MaxBlockSize);
            remlen -= MaxBlockSize;
            pos += MaxBlockSize;
        } else {
            memcpy(&p_data[pos],p_block,remlen);
            return(p_data);
        }
        I++;
    }

    return(p_data);
}

//---------------------------------------------------------------------------

unsigned int CMemBlockFile::GetTotalLength(void)
{
    return(TotalSize);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool  CMemBlockFile::SaveToFile(FILE* p_file)
{
    if( TotalSize == 0 ) return(false);

    CSimpleIterator<char> I(Blocks);
    char*          p_block;
    unsigned int   remlen = TotalSize;

    while((p_block = I.Current()) != NULL) {
        if( remlen >= MaxBlockSize ) {
            fwrite(p_block,sizeof(char),MaxBlockSize,p_file);
            remlen -= MaxBlockSize;
        } else {
            fwrite(p_block,sizeof(char),remlen,p_file);
            return(true);
        }
        I++;
    }
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CMemBlockFile::WriteString(const CSmallString& str)
{
    if( str == NULL ){
        return;
    }
    int len = str.GetLength();
    WriteDataBlock((const char*)str,len);
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteString(const char* p_str)
{
    if( p_str == NULL ){
        return;
    }
    int len = strlen(p_str);
    WriteDataBlock(p_str,len);
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteChar(const char z)
{
    if( BlockPosition == MaxBlockSize ) {
        AllocateNewBlock();
    }
    ActiveBlock[BlockPosition++] = z;
    TotalSize++;
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteUChar(const unsigned char z)
{
    if( BlockPosition == MaxBlockSize ) {
        AllocateNewBlock();
    }
    ActiveBlock[BlockPosition++] = z;
    TotalSize++;
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteUShort(const unsigned short z)
{
    if( BlockPosition == MaxBlockSize ) {
        AllocateNewBlock();
    }
    if( MaxBlockSize - BlockPosition >= sizeof(z) ) {
        memcpy(&ActiveBlock[BlockPosition],&z,sizeof(z));
        BlockPosition += sizeof(z);
        TotalSize += sizeof(z);
    } else {
        int delimit = (MaxBlockSize - BlockPosition);
        char* p_mydata = (char*)&z;
        memcpy(&ActiveBlock[BlockPosition],&p_mydata[0],delimit);
        BlockPosition += delimit;
        TotalSize += delimit;
        if( BlockPosition == MaxBlockSize ) {
            AllocateNewBlock();
        }
        int rest = sizeof(z) - delimit;
        memcpy(&ActiveBlock[BlockPosition],&p_mydata[delimit],rest);
        BlockPosition += rest;
        TotalSize += rest;
    }
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteUInt(const unsigned int z)
{
    if( BlockPosition == MaxBlockSize ) {
        AllocateNewBlock();
    }
    if( MaxBlockSize - BlockPosition >= sizeof(z) ) {
        memcpy(&ActiveBlock[BlockPosition],&z,sizeof(z));
        BlockPosition += sizeof(z);
        TotalSize += sizeof(z);
    } else {
        int delimit = (MaxBlockSize - BlockPosition);
        char* p_mydata = (char*)&z;
        memcpy(&ActiveBlock[BlockPosition],&p_mydata[0],delimit);
        BlockPosition += delimit;
        TotalSize += delimit;
        if( BlockPosition == MaxBlockSize ) {
            AllocateNewBlock();
        }
        int rest = sizeof(z) - delimit;
        memcpy(&ActiveBlock[BlockPosition],&p_mydata[delimit],rest);
        BlockPosition += rest;
        TotalSize += rest;
    }
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteDataBlock(const void* p_idata,unsigned int length)
{
    unsigned char* p_data = (unsigned char*)p_idata;

    unsigned int pos = 0;
    while( pos < length ) {
        if( BlockPosition == MaxBlockSize ) {
            AllocateNewBlock();
        }
        unsigned int block = MaxBlockSize - BlockPosition;
        if( block > (length - pos ) ) {
            block = length - pos;
        }
        memcpy(&ActiveBlock[BlockPosition],&p_data[pos],block);
        pos += block;
        BlockPosition += block;
        TotalSize += block;
    }
}

//---------------------------------------------------------------------------

void CMemBlockFile::WriteDataBlockInHex(const void* p_idata,unsigned int length)
{
    unsigned char* p_pointer = (unsigned char*)p_idata;
    for(unsigned int i=0; i<length; i++) {
        unsigned char z = *p_pointer++;
        unsigned char a;
        a  = (z >> 4);
        if( a >= 10 ) a += 55;
        else a += '0';
        WriteChar(a);
        a  = z & 0x0F;
        if( a >= 10 ) a += 55;
        else a += '0';
        WriteChar(a);
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

