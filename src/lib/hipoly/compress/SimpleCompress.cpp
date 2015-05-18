//==============================================================================
//    Copyright 2008 Petr Kulhanek
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

#include <SimpleCompress.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

unsigned char* CSimpleCompress::CompressData(unsigned char* p_in,
        unsigned int in_length,
        unsigned int& out_length)
{
    out_length = in_length;
    return(p_in);

    /* // compress data
     lzo_byte *p_wrkmem;
     lzo_byte *p_out;
     lzo_uint in_len = in_length;
     lzo_uint out_len = in_length + in_length/64 + 16 +3;

     // vlastn� komprese - inicializace buffer�
     p_wrkmem = new lzo_byte[LZO1X_1_MEM_COMPRESS];
     if( p_wrkmem == NULL ){
        delete[] p_in;
        ES_ERROR("memory problem","CCommand::CompressData");
        return(NULL);
        }

     p_out = new lzo_byte[out_len];

     if( p_out == NULL ){
        delete[] p_wrkmem;
        delete[] p_in;
        ES_ERROR("memory problem","CCommand::CompressData");
        return(NULL);
        }

     // inicializace
     if(lzo_init() != LZO_E_OK){
        delete[] p_wrkmem;
        delete[] p_in;
        ES_ERROR("lzo_init failed","CCommand::CompressData");
        return(NULL);
        }

     bool compress = false;
     if(lzo1x_1_compress((lzo_byte*)p_in,in_len,p_out,&out_len,p_wrkmem) == LZO_E_OK){
        compress &= out_len <= in_len; // success
        }

     // complete data
     delete[] p_wrkmem;

     if( compress == true ){
        out_length = out_len;
        delete[] p_in;
        return((char*)p_out);
        }
      else{
        out_length = in_length;
        delete[] p_out;
        return(p_in);
        }*/
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

unsigned char* CSimpleCompress::DecompressData(unsigned char* p_in,
        unsigned int in_length,
        unsigned int out_length)
{
    return(p_in);

    //  if( in_length == out_length) return(p_in);
    //
    //  if(lzo_init() != LZO_E_OK){
    //     ES_ERROR("lzo_init failed","CCommand::DecompressData");
    //     delete[] p_in;
    //     return(NULL);
    //     }
    //
    //  lzo_byte* p_newdata = new lzo_byte[out_length];
    //  if( p_newdata == NULL ){
    //     ES_ERROR("memory problem","CCommand::DecompressData");
    //     delete[] p_in;
    //     return(NULL);
    //     }
    //
    //  unsigned int out_len;
    //  if( lzo1x_decompress((lzo_byte*)p_in,in_length,p_newdata,&out_len,NULL) != LZO_E_OK){
    //     ES_ERROR("decompression problem","CCommand::DecompressData");
    //     delete[] p_in;
    //     delete[] p_newdata;
    //     return(NULL);
    //     }
    //
    //  if( out_len != out_length ){
    //     ES_ERROR("decompressed length does not match requested length","CCommand::DecompressData");
    //     delete[] p_in;
    //     delete[] p_newdata;
    //     return(NULL);
    //     }
    //
    //  delete[] p_in;
    //  return((char*)p_newdata);

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
