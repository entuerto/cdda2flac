// AudioTranscoder.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
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

#ifndef AUDIO_AUDIOTRANSCODER_H
#define AUDIO_AUDIOTRANSCODER_H

#include <orion/MemoryUtils.h>
#include <audio/AudioEncoderProfile.h>

namespace audio
{
//!
/*!
 */
class AudioTranscoder
{
public:
   DECLARE_POINTERS(AudioTranscoder)

   ~AudioTranscoder();

   //! Extracts a single track from a cd-rom
   void transcode_file(const std::string& file_name, AudioEncoderProfile::SharedPtr profile);


   static AudioTranscoder::SharedPtr create(); 

protected:
   AudioTranscoder();

};

} // namespace audio
#endif /* CDDATRACK_H */
