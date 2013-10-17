// ParanoiaReaderImpl.h
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

#ifndef AUDIO_PARANOIAREADERIMPL_H
#define AUDIO_PARANOIAREADERIMPL_H

#include <cdio/paranoia/paranoia.h>

#include <orion/NonCopyable.h>
#include <audio/AudioReader.h>
#include <audio/Cdda.h>

#include <audio/cdio/CddaImpl.h>

namespace audio
{
//!
/*!
 */
class ParanoiaReaderImpl :
   public AudioReader,
   orion::NonCopyable
{
public:
   ParanoiaReaderImpl(Cdda::SharedPtr audio_cd, int max_retries = 20, int mode = PARANOIA_MODE_FULL ^ PARANOIA_MODE_NEVERSKIP);
   virtual ~ParanoiaReaderImpl();

   //!
   virtual long read(uint8_t* buffer, uint32_t begin_sector, uint32_t sectors);

private:
   CddaImpl::SharedPtr _audio_cd;
   int       _max_retries;
   int32_t   _mode;

   cdrom_paranoia_t* _cdrom_paranoia;
};
}
#endif /* AUDIO_PARANOIAREADERIMPL_H */
