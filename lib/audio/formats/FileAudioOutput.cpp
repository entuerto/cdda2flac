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

#include <sys/fcntl.h>
#include <cerrno>
#include <cstdio>

#include <audio/AudioOutputException.h>

namespace audio
{
struct FileAudioOutput::Private
{
   FILE*  _file_handle;
   bool   _is_open;
};



/*
    Creates a new instance of the <code>FileAudioOutput</code> class.
 */
FileAudioOutput::FileAudioOutput() :
   _impl(new Private)
{
   _impl->_file_handle = 0;
   _impl->_is_open = false;
}

/*
    Free allocated resources.
 */
FileAudioOutput::~FileAudioOutput()
{
   close();

   delete _impl;
}

/*
 */
bool FileAudioOutput::is_open() const
{
   return _impl->__is_open;
}

/*
    Opens the specific file and creates it.
    \param fileName File to open an create if it does not exist
 */
bool FileAudioOutput::open(const std::string& name)
{
   _impl->__file_handle = std::fopen(name.c_str(), "w");

   if (_impl->__file_handle == nullptr) 
   {
      return false;
   }
   _impl->__is_open = true;
   return true;
}

/*
    Write to the audio output stream
    \param buffer audio data to write
    \param len length of the audio in bytes
 */
int32_t FileAudioOutput::write(const uint8_t* buffer, uint32_t len)
{
   return std::fwrite(buffer, sizeof(uint8_t), len, _impl->__file_handle);
}

/*
    Flush the stream
 */
void FileAudioOutput::flush()
{
}

/*
    Close the audio stream
 */
bool FileAudioOutput::close()
{
   _impl->__is_open = false;
   return std::fclose(_impl->__file_handle) == 0;
}

uint64_t FileAudioOutput::position()
{
   int32_t ret = std::ftell(_impl->__file_handle);

   THROW_IF3(ret == -1, AudioOutputException, std::strerror(errno));

   return ret;
}

uint64_t FileAudioOutput::position(int64_t offset)
{
   if (std::fseek(_impl->__file_handle, offset, SEEK_SET) != 0)
      THROW(AudioOutputException, std::strerror(errno));

   return position();
}
} // end of name space
