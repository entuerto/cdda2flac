// CddaReaderImpl.cpp
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

#include <audio/cdio/CddaReaderImpl.h>

#include <cdio/cdio.h>
#include <cdio/cd_types.h>
#include <cdio/paranoia/cdda.h>
#include <cdio/paranoia/paranoia.h>

#include <orion/Exception.h>
#include <audio/cdio/CddaImpl.h>
#include <audio/CddaException.h>


namespace audio
{

CddaReaderImpl::CddaReaderImpl(Cdda::SharedPtr audio_cd)
	: AudioReader(),
     _audio_cd()
{
   _audio_cd = orion::iface_cast<CddaImpl>(audio_cd);
}


CddaReaderImpl::~CddaReaderImpl()
{

}

/*!
   Raw read of audio sectors (no paranoia). One sector is CD_FRAMESIZE_RAW = 2352
 */
long CddaReaderImpl::read(uint8_t* buffer, uint32_t begin_sector, uint32_t sectors)
{
   if (not _audio_cd->is_open()) 
   {
      THROW_EXCEPTION(CddaException, "Cdda is closed");
   }

   long sectors_read = cdio_cddap_read(_audio_cd->cdrom(), buffer, begin_sector, sectors);

   char* err = cdio_cddap_errors(_audio_cd->cdrom());

   if (err) 
   {
      std::string tmp = err;
      delete [] err;
      THROW_EXCEPTION(CddaException, tmp);
   }
   return sectors_read;
}

}
