// AudioOutput.cpp
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

#include <audio/AudioOutput.h>

#include <audio/formats/FileAudioOutput.h>

namespace audio
{

/*!
    Creates a new instance of the <code>AudioOutput</code> class.
 */
AudioOutput::AudioOutput()
{
}

/*!
    Free allocated resources.
 */
AudioOutput::~AudioOutput()
{
}

/*!
    Creates a new instance of an <code>AudioOutput</code> class.
 */
AudioOutput::SharedPtr AudioOutput::create(const std::string& format)
{
   if ("wav" == format or "file" == format) 
   {
      return AudioOutput::SharedPtr(new FileAudioOutput);
   }
   return AudioOutput::SharedPtr(NULL);
}

} // namespace audio
