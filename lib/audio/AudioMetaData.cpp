// AudioMetaData.cpp
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

#include <audio/AudioMetaData.h>

#include <string>

namespace audio
{

class AudioMetaData::Private
{
public:
   Private() :
      title(),
      album(),
      track_number(),
      artist(),
      performer(),
      copyright(),
      license(),
      organization(),
      comment(),
      genre(),
      date(),
      location(),
      contact(),
      isrc(),
      album_art(NULL)
      {}

public:
   std::string title;
   std::string album;
   std::string track_number;
   std::string artist;
   std::string performer;
   std::string copyright;
   std::string license;
   std::string organization;
   std::string comment;
   std::string genre;
   std::string date;
   std::string location;
   std::string contact;
   std::string isrc;
   int8_t* album_art;
};

AudioMetaData::AudioMetaData() :
   _private(new Private)
{

}


AudioMetaData::~AudioMetaData()
{
}

/*!
   Returns the Track/Work name
 */
std::string AudioMetaData::title() const
{
   return _private->title;
}

/*!
   Sets the Track/Work name to \a value.
 */
void AudioMetaData::title(const std::string& value)
{
   _private->title = value;
}

/*!
   Returns the album name to which this track belongs
 */
std::string AudioMetaData::album() const
{
   return _private->album;
}

/*!
   Sets the album name to which this track belongs to \a value.
 */
void AudioMetaData::album(const std::string& value)
{
   _private->album = value;
}

/*!
   Returns the track number of this piece if part of a specific album
 */
std::string AudioMetaData::track_number() const
{
   return _private->track_number;
}

/*!
   Sets the track number of this piece if part of a specific album to \a value.
 */
void AudioMetaData::track_number(const std::string& value)
{
   _private->track_number = value;
}

/*!
   Returns the artist generally considered responsible for the work
 */
std::string AudioMetaData::artist() const
{
   return _private->artist;
}

/*!
   Sets the artist generally considered responsible for the work to \a value.
 */
void AudioMetaData::artist(const std::string& value)
{
   _private->artist = value;
}

/*!
   Returns the The artist(s) who performed the work
 */
std::string AudioMetaData::performer() const
{
   return _private->performer;
}

/*!
   Sets the The artist(s) who performed the work to \a value.
 */
void AudioMetaData::performer(const std::string& value)
{
   _private->performer = value;
}

/*!
   Returns the Copyright attribution, e.g., '2001 Nobody's Band' or '1999 Jack Moffitt'
 */
std::string AudioMetaData::copyright() const
{
   return _private->copyright;
}

/*!
   Sets the Copyright attribution, e.g., '2001 Nobody's Band' or '1999 Jack Moffitt' to \a value.
 */
void AudioMetaData::copyright(const std::string& value)
{
   _private->copyright = value;
}

/*!
   Returns the License information
 */
std::string AudioMetaData::license() const
{
   return _private->license;
}

/*!
   Sets the License information to \a value.
 */
void AudioMetaData::license(const std::string& value)
{
   _private->license = value;
}

/*!
   Returns the Name of the organization producing the track (i.e. the 'record label')
 */
std::string AudioMetaData::organization() const
{
   return _private->organization;
}

/*!
   Sets the Name of the organization producing the track (i.e. the 'record label') to \a value.
 */
void AudioMetaData::organization(const std::string& value)
{
   _private->organization = value;
}

/*!
   Returns the a short text description of the contents
 */
std::string AudioMetaData::comment() const
{
   return _private->comment;
}

/*!
   Sets a short text description of the contents to \a value.
 */
void AudioMetaData::comment(const std::string& value)
{
   _private->comment = value;
}

/*!
   Returns the A short text indication of music genre
 */
std::string AudioMetaData::genre() const
{
   return _private->genre;
}

/*!
   Sets a short text indication of music genre to \a value.
 */
void AudioMetaData::genre(const std::string& value)
{
   _private->genre = value;
}

/*!
   Returns the date the track was recorded
 */
std::string AudioMetaData::date() const
{
   return _private->date;
}

/*!
   Sets the date the track was recorded to \a value.
 */
void AudioMetaData::date(const std::string& value)
{
   _private->date = value;
}

/*!
   Returns the location where track was recorded
 */
std::string AudioMetaData::location() const
{
   return _private->location;
}

/*!
   Sets the location where track was recorded to \a value.
 */
void AudioMetaData::location(const std::string& value)
{
   _private->location = value;
}

/*!
   Returns the contact information for the creators or distributors of the track
 */
std::string AudioMetaData::contact() const
{
   return _private->contact;
}

/*!
   Sets the contact information for the creators or distributors of the track to \a value.
 */
void AudioMetaData::contact(const std::string& value)
{
   _private->contact = value;
}

/*!
   Returns the isrc
 */
std::string AudioMetaData::isrc() const
{
   return _private->isrc;
}

/*!
   Sets the isrc to \a value.
 */
void AudioMetaData::isrc(const std::string& value)
{
   _private->isrc = value;
}

/*!
   Returns the image of the cover of the album
 */
int8_t* AudioMetaData::album_art() const
{
   return _private->album_art;
}

/*!
   Sets the image of the cover of the album to \a value.
 */
void AudioMetaData::album_art(int8_t* value)
{
   _private->album_art = value;
}

AudioMetaData::SharedPtr AudioMetaData::create()
{
   return AudioMetaData::SharedPtr(new AudioMetaData);
}

} // end of namespace
