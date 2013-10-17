// AudioReader.h
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

#ifndef AUDIO_AUDIOREADER_H
#define AUDIO_AUDIOREADER_H

#include <string>

#include <orion/MemoryUtils.h>
#include <audio/Cdda.h>

namespace audio
{
//!
/*!
 */
class AudioReader
{
public:
   DECLARE_POINTERS(AudioReader)

   virtual ~AudioReader();

   //!
   virtual long read(uint8_t* buffer, uint32_t begin_sector, uint32_t sectors) =0;

   // Static functions
   static SharedPtr create(Cdda::SharedPtr audio_cd, const std::string& reader);

protected:
   AudioReader();

};

} // namespace audio
#endif /* AUDIOREADER_H */
