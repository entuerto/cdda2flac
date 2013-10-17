// AudioCdReader.cpp
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

#include "AudioReader.h"

#include <audio/cdio/CddaReaderImpl.h>
#include <audio/cdio/ParanoiaReaderImpl.h>


namespace audio
{

AudioReader::AudioReader()
{
}

AudioReader::~AudioReader()
{
}

AudioReader::SharedPtr AudioReader::create(Cdda::SharedPtr audio_cd, const std::string& reader)
{
   if ("raw" == reader) 
   {
      return AudioReader::SharedPtr(new CddaReaderImpl(audio_cd));
   }
   if ("paranoia" == reader) 
   {
      return AudioReader::SharedPtr(new ParanoiaReaderImpl(audio_cd));
   }
   return AudioReader::SharedPtr(NULL);
}

} // namespace audio
