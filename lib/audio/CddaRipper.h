// CddaRipper.h
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

#ifndef AUDIO_CDDARIPPER_H
#define AUDIO_CDDARIPPER_H

#include <orion/MemoryUtils.h>
#include <audio/Cdda.h>
#include <audio/AudioEncoderProfile.h>
#include <audio/AudioReader.h>

namespace audio
{
//!
/*!
 */
class CddaRipper
{
public:
   DECLARE_POINTERS(CddaRipper)

   ~CddaRipper();

   //! Extracts a single track from a cd-rom
   void rip_track(CddaTrack* track, AudioReader::SharedPtr reader,
                                    AudioEncoderProfile::SharedPtr profile);

   //! Extracts a all the tracks from a cd-rom
   void rip_disk(Cdda::SharedPtr cd);


   static CddaRipper::SharedPtr create(); 

protected:
   CddaRipper();

};

} // namespace audio
#endif /* CDDATRACK_H */
