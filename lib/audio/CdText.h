// CdText.h
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

#ifndef AUDIO_CDTEXT_H
#define AUDIO_CDTEXT_H

#include <string>

#include <orion/Mem-utils.h>
#include <audio/Common.h>

namespace audio
{

//!
/*!
 */
class CdText
{
public:
   DECLARE_POINTERS(CdText)

   virtual ~CdText() {}

   virtual std::string arranger() const =0;

   virtual std::string composer() const =0;

   virtual std::string disc_id() const =0;

   virtual std::string genre() const =0;

   virtual std::string message() const =0;

   virtual std::string isrc() const =0;

   virtual std::string performer() const =0;

   virtual std::string song_writer() const =0;

   virtual std::string title() const =0;

   virtual std::string upc_ean() const =0;

protected:
   CdText() {}

};

} // namespace audio
#endif /* AUDIO_CDTEXT_H */
