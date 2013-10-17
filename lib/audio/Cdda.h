// Cdda.h
//
// Copyright 2009 tomas <tomasp@videotron.ca>
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


#ifndef AUDIO_CDDA_H
#define AUDIO_CDDA_H

#include <ostream>
#include <list>

#include <orion/Mem-utils.h>
#include <audio/Common.h>

namespace audio
{
class CddaTrack;
class AudioDuration;

//!
/*!
 */
class Cdda
{
public:
   DECLARE_POINTERS(Cdda)

   typedef std::list<CddaTrack*> TrackList;

   static const int32_t FRAMESIZE;
   static const int32_t FRAMESIZE_RAW;
   static const int32_t FRAMESAMPLES;

   static const int32_t PREGAP_SECTORS;
   static const int32_t POSTGAP_SECTORS;

   static const int32_t MIN_LSN;
   static const int32_t MAX_LSN;

public:
   virtual ~Cdda();

   //! Opens the audio disk.
   virtual void open() =0;

   //! Close the audio disk.
   virtual void close() =0;

   //! Indicates if the audio disk is open.
   virtual bool is_open() const =0;

   virtual std::string device_name() const =0;

   //!
   virtual TrackList tracks() const =0;

   //! The number of audio tracks of the disk.
   virtual int track_count() const =0;

   //! Audio disk first sector
   virtual int32_t first_sector() const =0;

   //! Audio disk last sector
   virtual int32_t last_sector() const =0;

   //! Total lenght of audio disk in hours, minutes, seconds, and milliseconds.
   virtual AudioDuration audio_length() const =0;

   static SharedPtr create(const std::string& device = "");

protected:
   Cdda();

};

//--------------------------------------------------------------------------
// Manipulators

struct Msf
{
   std::string value;
};

//! Converts a logical sector number into a string format of minutes:seconsd:frames
Msf msf(int32_t lsn);
Msf msf(uint32_t lsn);

std::ostream& operator<<(std::ostream& out, const Msf& m);

} // namespace audio
#endif /* CDDA_H */
