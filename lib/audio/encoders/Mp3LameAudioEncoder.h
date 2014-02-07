//
// Mp3LameAudioEncoder.h
//
// Copyright 2009 tomas <tomasp@videotron.ca>
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

#ifndef AUDIO_MP3LAMEAUDIOENCODER_H
#define AUDIO_MP3LAMEAUDIOENCODER_H

//include files
#include <string>

#include <orion/NonCopyable.h>
#include <audio/AudioEncoder.h>

#include <lame/lame.h>

namespace audio
{
//! Audio encoder for mp3 lame
/*!
    This is the audio encoder to create mp3 files with the lame mp3
    encoder library.
 */
class Mp3LameAudioEncoder :
   public AudioEncoder,
   orion::NonCopyable
{
public:
   //! Construct an mp3 audio encoder
   Mp3LameAudioEncoder(AudioOutput::SharedPtr out);

   //! Free the audio encoder resources
   virtual ~Mp3LameAudioEncoder();

   virtual std::string type() const;

   virtual void setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr metadata, uint32_t data_size);

   virtual int32_t encode(const int8_t* data, uint32_t len);

   virtual void tear_down();

protected:
   void set_tags(AudioMetaData::SharedPtr metadata);

private:
   AudioOutput::SharedPtr _output;
   AudioEncoderProfile::SharedPtr _profile;

   lame_global_flags* _gf;
};

}
#endif
