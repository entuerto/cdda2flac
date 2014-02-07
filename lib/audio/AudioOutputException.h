/*
 * AudioOutputException.h
 *
 * Copyright 2014 tomas <tomasp@videotron.ca>
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

#ifndef AUDIO_AUDIOOUTPUTEXCEPTION_H
#define AUDIO_AUDIOOUTPUTEXCEPTION_H

#include <string>

#include <orion/ErrorMacros.h>
#include <orion/Exception.h>

namespace audio
{
//! Brief description of AudioOutputException
/*!
    Long description of AudioOutputException
 */
class AudioOutputException : public orion::Exception
{
public:
   AudioOutputException(const std::string& text);

   AudioOutputException(const std::string& text,
                        const std::string& file_name,
                              int32_t      line_number);

   AudioOutputException(const std::string& text,
                        const std::string& file_name,
                              int32_t      line_number,
                        const std::string& function);

   AudioOutputException(const AudioOutputException& other);

   virtual ~AudioOutputException() throw();

   AudioOutputException& operator=(const AudioOutputException& other);
};

}
#endif
