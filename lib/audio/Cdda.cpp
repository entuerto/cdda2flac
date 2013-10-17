// Cdda.cpp
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


#include <audio/Cdda.h>
#include <audio/cdio/CddaImpl.h>

namespace audio
{

const int32_t Cdda::FRAMESIZE      = 2048;
const int32_t Cdda::FRAMESIZE_RAW  = 2352;
const int32_t Cdda::FRAMESAMPLES   = Cdda::FRAMESIZE_RAW / 4;

const int32_t Cdda::PREGAP_SECTORS   = 150;
const int32_t Cdda::POSTGAP_SECTORS  = 150;

const int32_t Cdda::MIN_LSN = -450150;
const int32_t Cdda::MAX_LSN =  450150;

Cdda::Cdda()
{

}


Cdda::~Cdda()
{

}

Cdda::SharedPtr Cdda::create(const std::string& device)
{
   return std::shared_ptr<Cdda>(new CddaImpl(device));
}

//--------------------------------------------------------------------------
// Manipulators

Msf msf(int32_t lsn)
{
   Msf m = { lba_to_msf(lsn) };
   return m;
}

Msf msf(uint32_t lsn)
{
   Msf m = { lba_to_msf(lsn) };
   return m;
}

std::ostream& operator<<(std::ostream& out, const Msf& m)
{
   out << m.value;
   return out;
}

} // namespace audio
