// CdTextImpl.cpp
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

#include <audio/cdio/CdTextImpl.h>

namespace audio
{

CdTextImpl::CdTextImpl(cdtext_t* cdtext, uint8_t track) :
   CdText(),
   _cdtext(cdtext),
   _track(track)
{

}

CdTextImpl::~CdTextImpl()
{

}

std::string CdTextImpl::arranger() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_ARRANGER, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::composer() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_COMPOSER, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::disc_id() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_DISCID, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::genre() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_GENRE, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::message() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_MESSAGE, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::isrc() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_ISRC, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::performer() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_PERFORMER, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::song_writer() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_SONGWRITER, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::title() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_TITLE, _track);
   return value != NULL ? value : "";
}

std::string CdTextImpl::upc_ean() const
{
   const char* value = cdtext_get_const(_cdtext, CDTEXT_FIELD_UPC_EAN, _track);
   return value != NULL ? value : "";
}

}
