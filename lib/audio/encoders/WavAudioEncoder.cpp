// WavAudioEncoder.cpp
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

#include <audio/encoders/WavAudioEncoder.h>

#include <audio/AudioOutput.h>
#include <audio/AudioEncoderProfile.h>
#include <audio/AudioMetaData.h>

#include <cdio/sector.h>

namespace audio
{

//-------------------------------------------------------------------------
// WavAudioEncoder

/*
    Constructor
 */
WavAudioEncoder::WavAudioEncoder(AudioOutput::SharedPtr out) :
   AudioEncoder(),
   _output(out)
{
}

/*
    Free resources
 */
WavAudioEncoder::~WavAudioEncoder()
{
}

/*
 */
std::string WavAudioEncoder::type() const
{
   return "Wav";
}

/*
 */
void WavAudioEncoder::setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr /* metadata */, uint32_t data_size)
{
   uint8_t  riff_id[4]          = {'R','I','F','F'};  //'RIFF'
   uint32_t riff_size           = data_size + 44 - 8; // FileSize-8
   uint8_t  riff_type[4]        = {'W','A','V','E'};  // 'WAVE'

   uint8_t  fmt_id[4]           = {'f','m','t',' '};  // 'FMT '
   uint32_t fmt_length          = 16;                 // 16
   uint16_t fmt_format          = 1;                       // 1 = WAVE_FORMAT_PCM
   uint16_t fmt_channels        = profile->channels();    // 1 = mono, 2 = stereo
   uint32_t fmt_sampling_rate   = profile->sample_rate(); // 44100
   uint32_t fmt_data_rate       = profile->channels() *
                                  profile->bits_per_sample() * profile->sample_rate() / 8;
   uint16_t fmt_block_align     = profile->channels()* profile->bits_per_sample() / 8;
   uint16_t fmt_bits_per_sample = profile->bits_per_sample();  // 8 or 16

   uint8_t  data_id[4]          = {'d','a','t','a'};  // 'data'

   _output->write(riff_id, 4);
   _output->write((uint8_t*)&riff_size, 4);
   _output->write(riff_type, 4);

   _output->write(fmt_id, 4);
   _output->write((uint8_t*)&fmt_length, 4);
   _output->write((uint8_t*)&fmt_format, 2);
   _output->write((uint8_t*)&fmt_channels, 2);
   _output->write((uint8_t*)&fmt_sampling_rate, 4);
   _output->write((uint8_t*)&fmt_data_rate, 4);
   _output->write((uint8_t*)&fmt_block_align, 2);
   _output->write((uint8_t*)&fmt_bits_per_sample, 2);

   _output->write(data_id, 4);
   _output->write((uint8_t*)&data_size, 4);
}

/*
 */
int32_t WavAudioEncoder::encode(int8_t* data,  uint32_t len)
{
   //! \todo should we check the endianess of the data.

   return _output->write(reinterpret_cast<uint8_t*>(data), len);
}

/*
 */
void WavAudioEncoder::tear_down()
{
}

} // end of namespace
