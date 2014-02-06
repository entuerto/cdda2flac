// FileAudioOutput.cpp
//
// Copyright 2009 Tomas <Tomas@JUANITO>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include <audio/formats/FileAudioOutput.h>

#include <windows.h>

namespace audio
{
struct FileAudioOutput::Private
{
   HANDLE _file_handle;
   bool  _is_open;
};

/*
    Creates a new instance of the <code>FileAudioOutput</code> class.
 */
FileAudioOutput::FileAudioOutput() :
   _impl(new Private)
{
   _impl->_file_handle = NULL;
}

/*
    Free allocated resources.
 */
FileAudioOutput::~FileAudioOutput()
{
   close();
}

/*
 */
bool FileAudioOutput::is_open() const
{
   return _impl->_file_handle != NULL;
}

/*
    Opens the specific file and creates it.
    \param fileName File to open an create if it does not exist
 */
bool FileAudioOutput::open(const std::string& name)
{
   _impl->_file_handle = CreateFile(name.c_str(),
                                    GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL, NULL );

   if (INVALID_HANDLE_VALUE == _impl->_file_handle) 
   {
      return false;
   }

   return true;
}

/*
    Write to the audio output stream
    \param buffer audio data to write
    \param len length of the audio in bytes
 */
int32_t FileAudioOutput::write(const uint8_t* buffer, uint32_t len)
{
   DWORD bytes_written = 0;
   WriteFile(_impl->_file_handle, buffer, len, &bytes_written, NULL);
   return bytes_written;
}

/*
    Flush the stream
 */
void FileAudioOutput::flush()
{
   FlushFileBuffers(_impl->_file_handle);
}

/*
    Close the audio stream
 */
bool FileAudioOutput::close()
{
   flush();

   bool ret = CloseHandle(_impl->_file_handle);
   _impl->_file_handle = NULL;
   return ret;
}

uint64_t FileAudioOutput::position()
{
   return SetFilePointer(_impl->_file_handle, 0, 0, FILE_CURRENT);
}

uint64_t FileAudioOutput::position(int64_t offset)
{
   return SetFilePointer(_impl->_file_handle, offset, 0, FILE_BEGIN);
}

} // end of name space
