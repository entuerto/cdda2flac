// AudioEncoder.cpp
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

#include "AudioEncoder.h"

#include <config.h>
#include <audio/AudioOutput.h>
#include <audio/AudioEncoderException.h>
#include <audio/encoders/FlacAudioEncoder.h>
#include <audio/encoders/WavAudioEncoder.h>
#include <audio/encoders/Mp3LameAudioEncoder.h>
#include <audio/encoders/OggVorbisAudioEncoder.h>

namespace audio
{

AudioEncoder::AudioEncoder()
{

}


AudioEncoder::~AudioEncoder()
{

}

AudioEncoder::SharedPtr AudioEncoder::create(const std::string& encoder, AudioOutput::SharedPtr out)
{
   if (encoder == "wav") 
   {
      return AudioEncoder::SharedPtr(new WavAudioEncoder(out));
   } 
#if HAVE_FLAC
   else if (encoder == "FLAC") 
   {
      return AudioEncoder::SharedPtr(new FlacAudioEncoder(out));
   }
#endif
#if HAVE_MP3LAME
   else if (encoder == "mp3lame" or encoder == "mp3") 
   {
      return AudioEncoder::SharedPtr(new Mp3LameAudioEncoder(out));
   }
#endif
#if HAVE_VORBIS
   else if (encoder == "ogg-vorbis" or encoder == "ogg") 
   {
      return AudioEncoder::SharedPtr(new OggVorbisAudioEncoder(out));
   }
#endif
   THROW_EXCEPTION(AudioEncoderException, "Unkwon encoder: " + encoder) 
}

} // namespace audio
