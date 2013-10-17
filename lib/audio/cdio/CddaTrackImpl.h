/*
 * CddaTrackImpl.h
 *
 * Copyright 2009 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef AUDIO_CDDATRACKIMPL_H
#define AUDIO_CDDATRACKIMPL_H

#include <orion/NonCopyable.h>
#include <audio/Common.h>
#include <audio/CddaTrack.h>
#include <audio/CdText.h>

#include <cdio/cdio.h>
#include <cdio/cd_types.h>
#include <cdio/paranoia/cdda.h>

namespace audio
{
class CddaImpl;
class AudioDuration;

//! An audio cd track
/*!
    Wrapper class for an audio cd track.
 */
class CddaTrackImpl :
   public CddaTrack,
   orion::NonCopyable
{
public:
   //! Construct an audio track from an Cdda
   CddaTrackImpl(const CddaImpl* audio_cd, uint8_t track);

   virtual ~CddaTrackImpl();

   virtual int32_t first_sector() const;

   virtual int32_t last_sector() const;

   virtual int channels() const;

   virtual bool copy_permit() const;

   virtual AudioDuration duration() const;

   virtual uint32_t track_number() const;

   virtual int32_t offset() const;

   virtual int32_t size() const;

   virtual bool preemphasis() const;

   virtual CdText::SharedPtr text() const;

private:
   CdIo_t*        _cdio;
   cdrom_drive_t* _cdrom;
   uint8_t        _track_number;
   CdText::SharedPtr _cdtext;

};
}
#endif
