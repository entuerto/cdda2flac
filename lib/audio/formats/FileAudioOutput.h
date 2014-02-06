// FileAudioOutput.h
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

#ifndef AUDIO_FILEAUDIOOUTPUT_H
#define AUDIO_FILEAUDIOOUTPUT_H

#include <orion/NonCopyable.h>
#include <audio/AudioOutput.h>

namespace audio
{

//! An audio file output stream
/*!
    Base class to create audio file output streams.
 */
class FileAudioOutput :
   public AudioOutput,
   orion::NonCopyable
{
public:
    FileAudioOutput();           // default constructor
    virtual ~FileAudioOutput();  // destructor

    virtual bool is_open() const;

    virtual bool open(const std::string& name);

    virtual int32_t write(const uint8_t* buffer, uint32_t len);

    virtual void flush();

    virtual bool close();

    virtual uint64_t position();

    virtual uint64_t position(int64_t offset);

private:
    struct Private;
    Private* _impl;
};

}
#endif
