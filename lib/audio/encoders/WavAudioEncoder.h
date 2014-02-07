// WavAudioEncoder.h
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

#ifndef AUDIO_WAVAUDIOENCODER_H
#define AUDIO_WAVAUDIOENCODER_H

//include files
#include <string>

#include <orion/NonCopyable.h>
#include <audio/AudioEncoder.h>

namespace audio
{
//! Audio encoder of the wav format
/*!
    This is the audio encoder to create vaw files. This is
    a simple format.
 */
class WavAudioEncoder :
   public AudioEncoder,
   orion::NonCopyable
{
public:
   WavAudioEncoder(AudioOutput::SharedPtr out);
   virtual ~WavAudioEncoder();  // destructor

   virtual std::string type() const;

   virtual void setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr metadata, uint32_t data_size);

   virtual int32_t encode(const int8_t* data,  uint32_t len);

   virtual void tear_down();

private:
   AudioOutput::SharedPtr _output;

};

}
#endif
