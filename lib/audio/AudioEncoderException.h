// AudioEncoderException.h
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

#ifndef AUDIO_AUDIOENCODEREXCEPTION_H
#define AUDIO_AUDIOENCODEREXCEPTION_H

#include <cstdint>
#include <string>

#include <orion/ErrorMacros.h>
#include <orion/Exception.h>

namespace audio
{
//! Signals an AudioEncoder error
/*!
    This exception is throwned when an error occures
    in during audio encoding.
 */
class AudioEncoderException : public orion::Exception
{
public:
   AudioEncoderException(const std::string& text);

   AudioEncoderException(const std::string& text,
                         const std::string& file_name,
                               int32_t      line_number);

   AudioEncoderException(const std::string& text,
                         const std::string& file_name,
                              int32_t      line_number,
                         const std::string& function);

   AudioEncoderException(const AudioEncoderException& other);

   virtual ~AudioEncoderException() throw();

   AudioEncoderException& operator=(const AudioEncoderException& other);
};

}
#endif // AUDIO_AUDIOENCODEREXCEPTION_H
