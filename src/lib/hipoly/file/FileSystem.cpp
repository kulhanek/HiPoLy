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

#include <FileSystem.hpp>
#include <ErrorSystem.hpp>
#include <errno.h>
#include <string.h>
#include <DirectoryEnum.hpp>

#if defined _WIN32 || defined __CYGWIN__
#include <windows.h>
#undef CopyFile
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <dirent.h>
#endif


//------------------------------------------------------------------------------

#if defined _WIN32 || defined __CYGWIN__
char *fi_strtok_r(char *str, const char *delim, char **saveptr)
{
    return(strtok(str,delim));
}
#else
char *fi_strtok_r(char *str, const char *delim, char **saveptr)
{
    return(strtok_r(str,delim,saveptr));
}
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CFileSystem::GetCurrentDir(CSmallString& currdir)
{
#if defined _WIN32 || defined __CYGWIN__
    int bufferlen = GetCurrentDirectory(0,NULL);
    if( bufferlen == 0 ) return(false);
    currdir.SetLength(bufferlen-1);
    return(GetCurrentDirectory(bufferlen,currdir.GetBuffer()));
#else
    CSmallString buffer;
    int buff_len = 1024;

    buffer.SetLength(buff_len);
    while( getcwd(buffer.GetBuffer(),buff_len) == NULL ) {
        if( errno != ERANGE ) {
            ES_ERROR(strerror(errno));
            return(false);
        }
        buff_len += 1024;
        buffer.SetLength(buff_len);
    }

    currdir = buffer.GetBuffer(); // copy only NULL terminated string
    return(true);
#endif
}

//------------------------------------------------------------------------------

bool CFileSystem::SetCurrentDir(const CSmallString& currdir)
{
#if defined _WIN32 || defined __CYGWIN__
    BOOL result = SetCurrentDirectory(currdir);
    if( result == TRUE ) return(true);
    ES_ERROR("SetCurrentDirectory failed");
#else
    int result = chdir(currdir);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
#endif

    return(false);
}

//------------------------------------------------------------------------------

bool CFileSystem::CreateDir(const CSmallString& dir)
{
#if defined _WIN32 || defined __CYGWIN__
    CSmallString    copy(dir);
    char*           p_subdir;
    char*           p_status;
    char            delim[2];
    delim[0] = CFileName::GetPathDelimiter();
    delim[1] = '\0';

    p_subdir = fi_strtok_r(copy.GetBuffer(),delim,&p_status);
    CFileName  whole_path;

    if( (dir != NULL) && (dir[0] == CFileName::GetPathDelimiter()) ) {
        whole_path /= p_subdir;
    } else {
        whole_path = p_subdir;
    }

    while( p_subdir != NULL ) {
        if( IsDirectory(whole_path) == false ) {
            int result = CreateDirectory(whole_path,NULL);
            if( result != 0 ) {
                ES_ERROR("CreateDir failed");
                return(false);
            }
        }
        p_subdir = fi_strtok_r(NULL,delim,&p_status);
        if( p_subdir ) whole_path = whole_path / p_subdir;
    }
    return(true);
#else
    CSmallString    copy(dir);
    char*           p_subdir;
    char*           p_status;
    char            delim[2];
    delim[0] = CFileName::GetPathDelimiter();
    delim[1] = '\0';

    p_subdir = fi_strtok_r(copy.GetBuffer(),delim,&p_status);
    CFileName  whole_path;

    if( (dir != NULL) && (dir[0] == CFileName::GetPathDelimiter()) ) {
        whole_path /= p_subdir;
    } else {
        whole_path = p_subdir;
    }

    while( p_subdir != NULL ) {
        if( IsDirectory(whole_path) == false ) {
            int result = mkdir(whole_path,0750);
            if( result != 0 ) {
                ES_ERROR(strerror(errno));
                return(false);
            }
        }
        p_subdir = fi_strtok_r(NULL,delim,&p_status);
        if( p_subdir ) whole_path = whole_path / p_subdir;
    }
    return(true);
#endif
}

//------------------------------------------------------------------------------

bool CFileSystem::RemoveDir(const CSmallString& dir,bool recursive)
{
#if defined _WIN32 || defined __CYGWIN__
    if( RemoveDirContents(dir,recursive) == false ) {
        return(false);
    }

    // and finaly remove directory
    int result = RemoveDirectory(dir);
    if( result == 0 ) return(true);
    ES_ERROR("RemoveDir failed");
#else
    if( RemoveDirContents(dir,recursive) == false ) {
        return(false);
    }

    // and finaly remove directory
    int result = rmdir(dir);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
#endif

    return(false);
}

//------------------------------------------------------------------------------

bool CFileSystem::RemoveDirContents(const CSmallString& dir,bool recursive)
{
    // go through the directory contents
    CDirectoryEnum dir_enum(dir);

    dir_enum.StartFindFile("*");

    CFileName dir_item;

    while( dir_enum.FindFile(dir_item) ) {
        if( (dir_item == ".") || (dir_item == "..") ) continue;

        CFileName whole_name;
        whole_name = dir / dir_item;

        if( (CFileSystem::IsDirectory(whole_name)) && recursive ) {
            if( CFileSystem::RemoveDir(whole_name,true) == false ) {
                return(false);
            }
        }

        if( CFileSystem::IsFile(whole_name) ) {
            if( CFileSystem::RemoveFile(whole_name) == false ) {
                return(false);
            }
        }

    }
    dir_enum.EndFindFile();

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CFileSystem::CopyFile(const CSmallString& source,
                                        const CSmallString& dest,
                                        bool overwrite)
{
    if( ! overwrite ){
        if( IsFile(dest) == true ){
            ES_ERROR("file already exist");
            return(false);
        }
    }


#if defined _WIN32 || defined __CYGWIN__
    return(false);
#else
    int     fd_to, fd_from;
    char    buf[4096];
    ssize_t nread;

    fd_from = open(source, O_RDONLY);
    if( fd_from < 0 ){
        ES_ERROR("source does not exist or is not file");
        return(false);
    }

    fd_to = open(dest, O_WRONLY | O_CREAT , 0644);
    if( fd_to < 0 ){
        close(fd_from);
        ES_ERROR("unable to create destination file");
        return(false);
    }

    while( nread = read(fd_from, buf, sizeof buf), nread > 0 ) {
        char    *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0) {
                nread -= nwritten;
                out_ptr += nwritten;
            } else if (errno != EINTR) {
                close(fd_from);
                close(fd_to);
                return(false);
            }
        } while (nread > 0);
    }

    close(fd_from);
    close(fd_to);

    return(true);
#endif
}

//------------------------------------------------------------------------------

bool CFileSystem::RemoveFile(const CSmallString& name,bool silent)
{
#if defined _WIN32 || defined __CYGWIN__
    BOOL result = DeleteFile(name);
    if( result == 0 ) return(true);
    ES_ERROR("DeleteFile failed");
#else
    int result = unlink(name);
    if( result == 0 ) return(true);
    if( ! silent ) {
        CSmallString error;
        error << "unable to remove file '" << name << "' (" << strerror(errno) << ")";
        ES_ERROR(error);
    }
#endif

    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CFileSystem::Rename(const CSmallString& old_name,
                                      const CSmallString& new_name)
{
    int result = rename(old_name,new_name);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
}

//------------------------------------------------------------------------------

bool CFileSystem::Copy(const CSmallString& source,
                                    const CSmallString& dest,
                                    bool overwrite,
                                    bool recursive)
{
    ES_ERROR("not implemented");
    return(false);
}

//------------------------------------------------------------------------------

bool CFileSystem::Remove(const CSmallString& file,
                                      bool recursive)
{
    ES_ERROR("not implemented");
    return(false);
}

//------------------------------------------------------------------------------

bool CFileSystem::IsDirectory(const CSmallString& file)
{
#if defined _WIN32 || defined __CYGWIN__
    HANDLE fhandle;
    bool   result = false;

    fhandle = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
    if( fhandle != INVALID_HANDLE_VALUE ) {
        BY_HANDLE_FILE_INFORMATION handle_info;
        if( GetFileInformationByHandle(fhandle,&handle_info) == TRUE ) {
            result = (handle_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        } else {
            result = false;
        }
        CloseHandle(fhandle);
    } else {
        printf("unable to open dir %s\n",(const char*)file);
    }

    return(result);
#else
    struct stat my_stat;
    if( stat(file,&my_stat) != 0 ) {
        return(false);
    }
    return( S_ISDIR(my_stat.st_mode) );
#endif
}

//------------------------------------------------------------------------------

bool CFileSystem::IsFile(const CSmallString& file)
{
#if defined _WIN32 || defined __CYGWIN__
    HANDLE fhandle;
    bool   result;

    fhandle = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    result = fhandle != INVALID_HANDLE_VALUE;
    if( fhandle != INVALID_HANDLE_VALUE ) CloseHandle(fhandle);

    return(result);
#else
    struct stat my_stat;
    if( stat(file,&my_stat) != 0 ) {
        return(false);
    }
    return( S_ISREG(my_stat.st_mode) );
#endif
}

//------------------------------------------------------------------------------

bool CFileSystem::IsSocket(const CSmallString& file)
{
#if defined _WIN32 || defined __CYGWIN__
    return(false);
#else
    struct stat my_stat;
    if( stat(file,&my_stat) != 0 ) {
        return(false);
    }
    return( S_ISSOCK(my_stat.st_mode) );
#endif
}

//------------------------------------------------------------------------------

bool CFileSystem::SetPosixMode(const CSmallString& file,int mode)
{
#if defined _WIN32 || defined __CYGWIN__
    ES_ERROR("not implemented");
    return(false);
#else
    if( chmod(file,mode & 0x1FF) == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
#endif
}

//------------------------------------------------------------------------------

int  CFileSystem::GetPosixMode(const CSmallString& file)
{
#if defined _WIN32 || defined __CYGWIN__
    ES_ERROR("not implemented");
    return(false);
#else
    struct stat my_stat;
    if( stat(file,&my_stat) != 0 ) {
        ES_ERROR(strerror(errno));
        return(0);
    }
    return( my_stat.st_mode & 0x1FF );
#endif
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
