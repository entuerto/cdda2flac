// CddaReaderImpl.h
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

#ifndef AUDIO_CDDAREADERIMPL_H
#define AUDIO_CDDAREADERIMPL_H

#include <orion/NonCopyable.h>
#include <audio/AudioReader.h>
#include <audio/Cdda.h>

#include <audio/cdio/CddaImpl.h>

namespace audio
{
class CddaImpl;
//!
/*!
 */
class CddaReaderImpl :
   public AudioReader,
   orion::NonCopyable
{
public:
   CddaReaderImpl(Cdda::SharedPtr audio_cd);
   virtual ~CddaReaderImpl();

   //! Raw read of audio sectors (no paranoia). One sector is CD_FRAMESIZE_RAW = 2352
   virtual long read(uint8_t* buffer, uint32_t begin_sector, uint32_t sectors);

private:
   CddaImpl::SharedPtr _audio_cd;

};
}
#endif /* AUDIO_CddaREADERIMPL_H */
