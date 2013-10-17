// AudioMetaData.h
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

#ifndef AUDIO_AUDIOMETADATA_H
#define AUDIO_AUDIOMETADATA_H

#include <string>

#include <orion/MemoryUtils.h>
#include <orion/NonCopyable.h>

namespace audio
{
//! An interface to audio meta data fields
/*!
   This a basic class to set the meta data of the audio
   encoder. Use this class when encoding audio.
 */
class AudioMetaData :
   orion::NonCopyable
{
public:
   DECLARE_POINTERS(AudioMetaData)

   AudioMetaData();
   virtual ~AudioMetaData();

   //! Returns the Track/Work name
   std::string title() const;

   //! Sets the Track/Work name to \a value.
   void title(const std::string& value);

   //! Returns the album name to which this track belongs
   std::string album() const;

   //! Sets the album name to which this track belongs to \a value.
   void album(const std::string& value);

   //! Returns the track number of this piece if part of a specific album
   std::string track_number() const;

   //! Sets the track number of this piece if part of a specific album to \a value.
   void track_number(const std::string& value);

   //! Returns the artist generally considered responsible for the work
   std::string artist() const;

   //! Sets the artist generally considered responsible for the work to \a value.
   void artist(const std::string& value);

   //! Returns the The artist(s) who performed the work
   std::string performer() const;

   //! Sets the The artist(s) who performed the work to \a value.
   void performer(const std::string& value);

   //! Returns the Copyright attribution, e.g., '2001 Nobody's Band' or '1999 Jack Moffitt'
   std::string copyright() const;

   //! Sets the Copyright attribution, e.g., '2001 Nobody's Band' or '1999 Jack Moffitt' to \a value.
   void copyright(const std::string& value);

   //! Returns the License information
   std::string license() const;

   //! Sets the License information to \a value.
   void license(const std::string& value);

   //! Returns the Name of the organization producing the track (i.e. the 'record label')
   std::string organization() const;

   //! Sets the Name of the organization producing the track (i.e. the 'record label') to \a value.
   void organization(const std::string& value);

   //! Returns the a short text description of the contents
   std::string comment() const;

   //! Sets a short text description of the contents to \a value.
   void comment(const std::string& value);

   //! Returns the A short text indication of music genre
   std::string genre() const;

   //! Sets a short text indication of music genre to \a value.
   void genre(const std::string& value);

   //! Returns the date the track was recorded
   std::string date() const;

   //! Sets the date the track was recorded to \a value.
   void date(const std::string& value);

   //! Returns the location where track was recorded
   std::string location() const;

   //! Sets the location where track was recorded to \a value.
   void location(const std::string& value);

   //! Returns the contact information for the creators or distributors of the track
   std::string contact() const;

   //! Sets the contact information for the creators or distributors of the track to \a value.
   void contact(const std::string& value);

   //! Returns the isrc
   std::string isrc() const;

   //! Sets the isrc to \a value.
   void isrc(const std::string& value);

   //! Returns the image of the cover of the album
   int8_t* album_art() const;

   //! Sets the image of the cover of the album to \a value.
   void album_art(int8_t* value);

   static SharedPtr create();

private:
   class Private;

   const std::unique_ptr<Private> _private;
};

} // end of namespace
#endif /* AUDIO_AUDIOMETADATA_H */
