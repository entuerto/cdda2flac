// AudioEncoder.h
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

#ifndef AUDIO_AUDIOENCODER_H
#define AUDIO_AUDIOENCODER_H

#include <cstdint>
#include <string>

#include <orion/MemoryUtils.h>
#include <audio/AudioOutput.h>
#include <audio/AudioEncoderSettings.h>
#include <audio/AudioMetaData.h>

namespace audio
{
//! Encodes an audio stream
/*!
   This class specifies an interface
 */
class AudioEncoder
{
public:
   DECLARE_POINTERS(AudioEncoder)

   virtual ~AudioEncoder();

   //! Generic type of the encoder
   /*!
       Returns the generic user string type/name of this encoder
       Examples: "MP3", "Ogg Vorbis", "Wav", "Flac", etc
     */
   virtual std::string type() const = 0;

   //! Configure the audio encoder
   /*!
       The encoder can initialize specific information for
       the audio stream that we are about to encode.
       \param settings controls the encoders settings
       \param metadata contains the tag information when encoding audio
       \param data_size size of the audio stream to encode
     */
   virtual void setup(AudioEncoderSettings::SharedPtr settings, AudioMetaData::SharedPtr metadata, uint32_t data_size) = 0;

   //! Encode some audio
   /*!
       Encode a number of frames of audio.
       \param buffer audio data
       \param len length of buffer of audio data to encode
       \return the byte of encoded audio, -1 if error
     */
   virtual int32_t encode(int8_t* data, uint32_t len) = 0;

   //! Cleanup the encoding process after completion
   /*!
       We have finish encoding and we tell the encoder to
       flush any pending data.
     */
   virtual void tear_down() = 0;


   static SharedPtr create(const std::string& encoder, AudioOutput::SharedPtr out);

protected:
   AudioEncoder();

};

} // namespace audio
#endif /* AUDIO_AUDIOENCODER_H */
