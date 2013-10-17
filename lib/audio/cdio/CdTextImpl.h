// CdTextImpl.h
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

#ifndef AUDIO_CDTEXTIMPL_H
#define AUDIO_CDTEXTIMPL_H

#include <cdio/cdtext.h>

#include <orion/NonCopyable.h>
#include <audio/CdText.h>

namespace audio
{

//!
/*!
 */
class CdTextImpl :
   public CdText,
   orion::NonCopyable
{
public:
   CdTextImpl(cdtext_t* cdtext, uint8_t track);
   virtual ~CdTextImpl();

   virtual std::string arranger() const;

   virtual std::string composer() const;

   virtual std::string disc_id() const;

   virtual std::string genre() const;

   virtual std::string message() const;

   virtual std::string isrc() const;

   virtual std::string performer() const;

   virtual std::string song_writer() const;

   virtual std::string title() const;

   virtual std::string upc_ean() const;

private:
   cdtext_t*  _cdtext;
   uint8_t    _track;

};
}
#endif /* CDTEXTIMPL_H */
