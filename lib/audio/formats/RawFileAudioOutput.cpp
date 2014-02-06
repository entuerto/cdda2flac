// RawFileRawFileAudioOutput.cpp
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

#include <audio/formats/RawFileAudioOutput.h>

#include <sys/fcntl.h>

namespace audio
{

/*
    Creates a new instance of the <code>RawFileAudioOutput</code> class.
 */
RawFileAudioOutput::RawFileAudioOutput() :
   _file_handle(0),
   _is_open(false)
{
}

/*
    Free allocated resources.
 */
RawFileAudioOutput::~RawFileAudioOutput()
{
   close();
}

/*
 */
bool RawFileAudioOutput::is_open() const
{
   return _is_open;
}

/*
    Opens the specific file and creates it.
    \param fileName File to open an create if it does not exist
 */
bool RawFileAudioOutput::open(const std::string& name)
{
   _file_handle = std::fopen(name.c_str(), "w");

   if (_file_handle == nullptr) 
   {
      return false;
   }
   _is_open = true;
   return true;
}

/*
    Write to the audio output stream
    \param buffer audio data to write
    \param len length of the audio in bytes
 */
int32_t RawFileAudioOutput::write(const uint8_t* buffer, uint32_t len)
{
   return std::fwrite(buffer, sizeof(uint8_t), len, _file_handle);
}

/*
    Flush the stream
 */
void RawFileAudioOutput::flush()
{
}

/*
    Close the audio stream
 */
bool RawFileAudioOutput::close()
{
   _is_open = false;
   return std::fclose(_file_handle) == 0;
}

} // end of name space
