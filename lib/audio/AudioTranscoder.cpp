// AudioTranscoder.cpp
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

#include <audio/AudioTranscoder.h>

#include <orion/Exception.h>
#include <orion/Logging.h>
#include <audio/AudioEncoder.h>
#include <audio/AudioEncoderException.h>
#include <audio/AudioOutput.h>

#include <windows.h>

using namespace orion;
using namespace orion::logging;

namespace audio
{

/*!
*/
AudioTranscoder::AudioTranscoder() 
{
}

/*!
*/
AudioTranscoder::~AudioTranscoder()
{
}

/*!
*/
void AudioTranscoder::transcode_file(const std::string& file_name, AudioEncoderProfile::SharedPtr profile)
{
   HANDLE file_handle = CreateFile(file_name.c_str(),
                                   GENERIC_READ, 
                                   0, 
                                   NULL, 
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL, 
                                   NULL);

   THROW_IF(INVALID_HANDLE_VALUE == file_handle, Exception);

   try 
   {
      AudioOutput::SharedPtr  output  = AudioOutput::create("raw");
      AudioEncoder::SharedPtr encoder = AudioEncoder::create(profile->encoder(), output);

      output->open("file1." + profile->encoder());

      AudioMetaData::SharedPtr metadata = AudioMetaData::create();

      metadata->title("track number N");

      encoder->setup(profile, metadata, 1000000);

      int8_t read_buffer[1024];

      DWORD bytes_read = 0;
      while (ReadFile(file_handle, read_buffer, 1024, &bytes_read, NULL)) 
      {
         encoder->encode(read_buffer, bytes_read);
      }
      
      encoder->tear_down();

   }
   catch (AudioEncoderException& aee) 
   {
      LOG_EXCEPTION(aee);
   }
   CloseHandle(file_handle);
}

/*!
*/
AudioTranscoder::SharedPtr AudioTranscoder::create() 
{
   return AudioTranscoder::SharedPtr(new AudioTranscoder);
}

} // namespace audio

