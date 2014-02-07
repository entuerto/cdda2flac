// CddaRipper.cpp
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

#include <audio/CddaRipper.h>

#include <orion/Logging.h>
#include <audio/AudioEncoder.h>
#include <audio/AudioEncoderException.h>
#include <audio/AudioOutput.h>
#include <audio/CddaTrack.h>

using namespace orion;
using namespace orion::logging;

namespace audio
{

/*!
*/
CddaRipper::CddaRipper(Cdda::SharedPtr cd) :
   _cd(cd)
{
}

/*!
*/
CddaRipper::~CddaRipper()
{
}

/*!
*/
void CddaRipper::rip_track(CddaTrack* track, AudioEncoderProfile::SharedPtr profile)
{
   try 
   {
      AudioReader::SharedPtr  reader  = AudioReader::create(_cd, "raw");
      AudioOutput::SharedPtr  output  = AudioOutput::create("file");
      AudioEncoder::SharedPtr encoder = AudioEncoder::create(profile->encoder(), output);

      output->open("track1." + profile->encoder());

      AudioMetaData::SharedPtr metadata = AudioMetaData::create();

      metadata->title("track number N");

      encoder->setup(profile, metadata, track->size());

      uint8_t read_buffer[Cdda::FRAMESIZE_RAW];

      for (uint32_t cursor = track->first_sector(); cursor <= track->last_sector(); ++cursor) 
      {
         // read a sector
         long sectors_read = reader->read(read_buffer, cursor, 1);

         encoder->encode(reinterpret_cast<int8_t*>(read_buffer), sectors_read * Cdda::FRAMESIZE_RAW);
      }
      encoder->tear_down();

   }
   catch (AudioEncoderException& aee) 
   {
      LOG_EXCEPTION(aee);
      return;
   }
}

/*!
*/
void CddaRipper::rip_disk(AudioEncoderProfile::SharedPtr profile)
{
/*
   try 
   {
      AudioOutput::SharedPtr output  = AudioOutput::create("file");
      AudioReader::SharedPtr reader  = AudioReader::create(cd, "raw");


      output->open("track1." + enc_name);

      AudioMetaData::SharedPtr metadata = AudioMetaData::create();

      metadata->title("track number N");

      encoder->setup(enc_settings, metadata, track->size());

      CddaRipper::SharedPtr ripper = CddaRipper::create();

      ripper->rip_track(track, reader, encoder, output); 

   }
   catch (CddaException& ae) 
   {
      LOG_EXCEPTION(ae);
      return;
   }
*/   
}

/*!
*/
CddaRipper::SharedPtr CddaRipper::create(Cdda::SharedPtr cd) 
{
   return CddaRipper::SharedPtr(new CddaRipper(cd));
}

} // namespace audio

