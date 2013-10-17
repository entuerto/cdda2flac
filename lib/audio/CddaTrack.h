// CddaTrack.h
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

#ifndef AUDIO_CDDATRACK_H
#define AUDIO_CDDATRACK_H

#include <orion/MemoryUtils.h>
#include <audio/CdText.h>

namespace audio
{
class AudioDuration;

//!
/*!
 */
class CddaTrack
{
public:
   DECLARE_POINTERS(CddaTrack)

   virtual ~CddaTrack() {}

   //! Return the lsn for the start of track
   virtual int32_t first_sector() const =0;

   //!  Get last logical sector number of the track
   virtual int32_t last_sector() const =0;

   //! Returns the number of channels in the track: 2 or 4
   virtual int channels() const =0;

   //! Indicates if track has copy permit set
   virtual bool copy_permit() const =0;

   //! Track duration
   virtual AudioDuration duration() const =0;

   //! Track number
   virtual uint32_t track_number() const =0;

   //! Offset of the track
   virtual int32_t offset() const =0;

   //! size of the track in bytes
   virtual int32_t size() const =0;

   //! Indicates if audio track has linear preemphasis set
   virtual bool preemphasis() const =0;

   //! The cd-text information kept in the audio cd
   virtual CdText::SharedPtr text() const =0;

protected:
   CddaTrack() {}

};

} // namespace audio
#endif /* CDDATRACK_H */
