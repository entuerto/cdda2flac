/*
 * AudioCDException.h
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

#ifndef AUDIO_CDDAEXCEPTION_H
#define AUDIO_CDDAEXCEPTION_H

#include <string>

#include <orion/ErrorMacros.h>
#include <orion/Exception.h>

namespace audio
{
//! Brief description of AudioCDException
/*!
    Long description of AudioCDException
 */
class CddaException : public orion::Exception
{
public:
   CddaException(const std::string& text);

   CddaException(const std::string& text,
                 const std::string& file_name,
                       int32_t      line_number);

   CddaException(const std::string& text,
                 const std::string& file_name,
                       int32_t      line_number,
                 const std::string& function);

   CddaException(const CddaException& other);

   virtual ~CddaException() throw();

   CddaException& operator=(const CddaException& other);
};

}
#endif
