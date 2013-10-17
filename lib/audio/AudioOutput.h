// AudioOutput.h
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

#ifndef AUDIO_AUDIOOUTPUT_H
#define AUDIO_AUDIOOUTPUT_H

//include files
#include <string>

#include <orion/Mem-utils.h>
#include <audio/Common.h>

namespace audio
{
//! An audio output stream
/*!
    Base class to create audio output streams.

    At the moment it output to a file.
 */
class AudioOutput
{
public:
   DECLARE_POINTERS(AudioOutput)

   virtual ~AudioOutput();  // destructor

   //! Indicates if the output stream is open.
   virtual bool is_open() const =0;

   //! Opens the audio output stream
   virtual bool open(const std::string& name) =0;

   //! Write to the audio stream
   virtual int32_t write(uint8_t* buffer, uint32_t len) =0;

   //! Flush the stream
   virtual void flush() =0;

   //! Close the audio stream
   virtual bool close() =0;

   static SharedPtr create(const std::string& format);

protected:
   AudioOutput();                                  //!< Default constructor

};

} // namespace audio
#endif
