// FlacAudioEncoder.h
//
// Copyright 2013 Tomas <Tomas@JUANITO>
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

#ifndef AUDIO_FLACAUDIOENCODER_H
#define AUDIO_FLACAUDIOENCODER_H

//include files
#include <string>

#include <FLAC/all.h>

#include <orion/NonCopyable.h>
#include <audio/AudioEncoder.h>

namespace audio
{
//! Audio encoder of the FLAC format
/*!
    This is the audio encoder to create FLAC files. 
 */
class FlacAudioEncoder :
   public AudioEncoder,
   orion::NonCopyable
{
public:
   FlacAudioEncoder(AudioOutput::SharedPtr out);
   virtual ~FlacAudioEncoder();  // destructor

   virtual std::string type() const;

   virtual void setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr metadata, uint32_t data_size);

   virtual int32_t encode(const int8_t* data,  uint32_t len);

   virtual void tear_down();

   int32_t write_to_output(const uint8_t* buffer, uint32_t len);
   uint64_t seek(int64_t offset);
   uint64_t tell();


protected:
   void set_tags(AudioMetaData::SharedPtr metadata);
   bool set_tag_value(FLAC__StreamMetadata* metadata, const std::string& key, const std::string& value); 

private:
   AudioOutput::SharedPtr _output;
   AudioEncoderProfile::SharedPtr _profile;

   FLAC__StreamEncoder* _encoder;
   FLAC__StreamMetadata* _metadata[2];

};

}
#endif
