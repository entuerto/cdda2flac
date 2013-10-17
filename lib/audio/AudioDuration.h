/*
 * AudioDuration.h
 *
 * Copyright 2009 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef AUDIO_AUDIODURATION_H
#define AUDIO_AUDIODURATION_H

#include <cstdint>

namespace audio
{
//! Audio duration
/*!
    Holds the duration of audio
 */
class AudioDuration
{
public:
   //! Construct an audio duration object
   AudioDuration(uint32_t sec);

   //! Construct an audio duration object
   AudioDuration(uint32_t h, uint32_t m, uint32_t s, uint32_t mi);

   //! Copy constructor
   AudioDuration(const AudioDuration& rhs);

   //! Free resources
   ~AudioDuration();

   //! Hours of the duration
   uint32_t hours() const;

   //! Minutes of the duration
   uint32_t minutes() const;

   //! Seconds of the duration
   uint32_t seconds() const;

   //! Milliseconds of the duration
   uint32_t millisec() const;

   //! Assignment operator
   AudioDuration& operator=(const AudioDuration& rhs);

private:
   uint32_t _seconds;
   uint32_t _milli;
};

}
#endif
