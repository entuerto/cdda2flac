// RawFileAudioOutput.h
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

#ifndef AUDIO_RAWFILEAUDIOOUTPUTWIN32_H
#define AUDIO_RAWFILEAUDIOOUTPUTWIN32_H

//include files
#include <string>

#include <orion/NonCopyable.h>
#include <audio/AudioOutput.h>

#include <windows.h>

namespace audio
{

//! An audio output stream
/*!
    Base class to create audio output streams.

    At the moment it output to a file.
 */
class RawFileAudioOutput :
   public AudioOutput,
   orion::NonCopyable
{
public:
    RawFileAudioOutput();           // default constructor
    virtual ~RawFileAudioOutput();  // destructor

    virtual bool is_open() const;

    virtual bool open(const std::string& name);

    virtual int32_t write(const uint8_t* buffer, uint32_t len);

    virtual void flush();

    virtual bool close();

private:
    HANDLE _file_handle;

};

}
#endif
