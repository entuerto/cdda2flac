/*
 * CddaTrackImpl.cpp
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
#include <audio/cdio/CddaTrackImpl.h>

#include <audio/cdio/CddaImpl.h>
#include <audio/cdio/CdTextImpl.h>
#include <audio/AudioDuration.h>

namespace audio
{

/*!
    Construct an audio track from an AudioCd
 */
CddaTrackImpl::CddaTrackImpl(const CddaImpl* audio_cd, uint8_t track) :
   _cdio(audio_cd->_cdio),
   _cdrom(audio_cd->_cdrom),
   _track_number(track),
   _cdtext()
{
   if (audio_cd->_cdtext != NULL) 
   {
      _cdtext = CdText::SharedPtr(new CdTextImpl(audio_cd->_cdtext, track));
   }
}

/*!
    Free resources
 */
CddaTrackImpl::~CddaTrackImpl()
{
}

/*!
    Return the lsn for the start of track
 */
int32_t CddaTrackImpl::first_sector() const
{
   return cdda_track_firstsector(_cdrom, _track_number);
}

/*!
    Get last lsn of the track. This generally one less than the start
    of the next track. -1 is returned on error.
 */
int32_t CddaTrackImpl::last_sector() const
{
   return cdda_track_lastsector(_cdrom, _track_number);
}


/*!
    Return the number of channels in track: 2 or 4; -2 if not implemented
    or -1 for error. Not meaningful if track is not an audio track.
 */
int CddaTrackImpl::channels() const
{
   return cdio_get_track_channels(_cdio, _track_number);
}

/*!
    Return true is track has copy permit set, false otherwise.
 */
bool CddaTrackImpl::copy_permit() const
{
   return cdio_get_track_copy_permit(_cdio, _track_number) == CDIO_TRACK_FLAG_TRUE;
}

/*!
    Duration of the audio cd track.
 */
AudioDuration CddaTrackImpl::duration() const
{
   uint32_t off = offset();

   return AudioDuration(0,
                        off / CDIO_CD_FRAMES_PER_MIN,
                        (off / CDIO_CD_FRAMES_PER_SEC) % CDIO_CD_SECS_PER_MIN,
                        off % CDIO_CD_FRAMES_PER_SEC);
}

/*!
    Track number
 */
uint32_t CddaTrackImpl::track_number() const
{
   return _track_number;
}

/*!
    Offset of the track : last sector - fisrt sector + 1
 */
int32_t CddaTrackImpl::offset() const
{
   return last_sector() - first_sector() + 1;
}

/*!
 */
int32_t CddaTrackImpl::size() const
{
   return CDIO_CD_FRAMESIZE_RAW * offset();
}

/*!
    Return true is audio track has linear preemphasis set, false otherwise.
 */
bool CddaTrackImpl::preemphasis() const
{
   return cdio_get_track_preemphasis(_cdio, _track_number) == CDIO_TRACK_FLAG_TRUE;
}

CdText::SharedPtr CddaTrackImpl::text() const
{
   return _cdtext;
}

}
