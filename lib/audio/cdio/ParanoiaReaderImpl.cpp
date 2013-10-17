// ParanoiaReaderImpl.cpp
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

#include "ParanoiaReaderImpl.h"

#include <cdio/cdio.h>
#include <cdio/cd_types.h>
#include <cdio/paranoia/cdda.h>

#include <orion/Exception.h>
#include <audio/cdio/CddaImpl.h>
#include <audio/CddaException.h>

namespace audio
{

ParanoiaReaderImpl::ParanoiaReaderImpl(Cdda::SharedPtr audio_cd, int max_retries, int mode)
	: AudioReader(),
     _audio_cd(),
     _max_retries(max_retries), // 20
     _mode(mode), // PARANOIA_MODE_FULL ^ PARANOIA_MODE_NEVERSKIP
     _cdrom_paranoia(NULL)
{
   _audio_cd = orion::iface_cast<CddaImpl>(audio_cd);

   _cdrom_paranoia = cdio_paranoia_init(_audio_cd->cdrom());
}


ParanoiaReaderImpl::~ParanoiaReaderImpl()
{
   cdio_paranoia_free(_cdrom_paranoia);
}

/*!

 */
long ParanoiaReaderImpl::read(uint8_t* buffer, uint32_t begin_sector, uint32_t sectors)
{
   if (not _audio_cd->is_open()) 
   {
      THROW_EXCEPTION(CddaException, "AudioCd is closed");
   }

   cdio_paranoia_modeset(_cdrom_paranoia, PARANOIA_MODE_FULL ^ PARANOIA_MODE_NEVERSKIP);

   cdio_paranoia_seek(_cdrom_paranoia, begin_sector, SEEK_SET);

   int16_t* read_buffer = cdio_paranoia_read_limited(_cdrom_paranoia, NULL, _max_retries);

   //! \todo Use another system to free returned messages
   char* err = cdio_cddap_errors(_audio_cd->cdrom());
   //char* mes = cdio_cddap_messages(_audio_cd->cdrom());

   if (err) 
   {
      std::string tmp = err;
      delete [] err;
      THROW_EXCEPTION(CddaException, tmp);
   }

   buffer = reinterpret_cast<uint8_t*>(read_buffer);

   return sectors;
}

}
