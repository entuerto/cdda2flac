/*
 * AudioDuration.cpp
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
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
#include <audio/AudioDuration.h>

namespace audio
{
/*!
    Construct an audio duration object specifyinig the seconds
 */
AudioDuration::AudioDuration(uint32_t sec) :
   _seconds(sec),
   _milli(0)
{
}

/*!
    Construct an audio duration object specifyinig the hours, minutes, seconds,
    and milliseconds.
 */
AudioDuration::AudioDuration(uint32_t h, uint32_t m, uint32_t s, uint32_t mi) :
   _seconds(0), _milli(mi)
{
   _seconds = (h * 3600) + (m * 60) + s;
}

/*!
    Free resources
 */
AudioDuration::~AudioDuration()
{
}

/*!
    Copy constructor
 */
AudioDuration::AudioDuration(const AudioDuration& rhs) :
   _seconds(rhs._seconds),
   _milli(rhs._milli)
{
}

/*!
    Assignment operator
 */
AudioDuration& AudioDuration::operator=(const AudioDuration& rhs)
{
   if ( &rhs == this )
      return *this;

   _seconds = rhs._seconds;
   _milli   = rhs._milli;

   return *this;
}

/*!
    Hours of the duration
 */
uint32_t AudioDuration::hours() const
{
   return _seconds / 3600;
}

/*!
    Minutes of the duration
 */
uint32_t AudioDuration::minutes() const
{
   return (_seconds % 3600) / 60;
}

/*!
    Seconds of the duration
 */
uint32_t AudioDuration::seconds() const
{
   return _seconds % 60;
}

/*!
    Milliseconds of the duration
 */
uint32_t AudioDuration::millisec() const
{
   return _milli;
}

}
