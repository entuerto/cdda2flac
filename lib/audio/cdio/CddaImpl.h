// CddaImpl.h
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

#ifndef AUDIO_CDDAIMPL_H
#define AUDIO_CDDAIMPL_H

#include <orion/MemoryUtils.h>
#include <orion/NonCopyable.h>
#include <audio/Cdda.h>

#include <cdio/cdio.h>
#include <cdio/cd_types.h>
#include <cdio/cdtext.h>
#include <cdio/paranoia/cdda.h>

namespace audio
{

class CddaImpl :
   public Cdda,
   orion::NonCopyable
{
public:
   DECLARE_POINTERS(CddaImpl)

   CddaImpl(const std::string& device);
   virtual ~CddaImpl();

   virtual void open();

   virtual void close();

   virtual bool is_open() const;

   virtual std::string device_name() const;

   virtual TrackList tracks() const;

   virtual int track_count() const;

   virtual int32_t first_sector() const;

   virtual int32_t last_sector() const;

   virtual AudioDuration audio_length() const;

   CdIo_t*        cdio() const;
   cdrom_drive_t* cdrom() const;

protected:
   void read_toc();
   void read_cddb();

   std::string cdda_error_message(int code) const;

private:
   friend class CddaTrackImpl;

   // Main info
   std::string    _device;
   bool           _is_open;
   CdIo_t*        _cdio;
   cdrom_drive_t* _cdrom;
   cdtext_t*      _cdtext;

   // Current CD
   TrackList _tracks;

   // CDDB information
   uint32_t _disk_id;

   // Misc
   uint32_t _audio_length;

};


std::string lba_to_msf(int32_t lsn);

}
#endif /* AUDIO_CDDAIMPL_H */
