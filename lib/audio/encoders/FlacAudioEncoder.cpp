// FlacAudioEncoder.cpp
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

#include <audio/encoders/FlacAudioEncoder.h>

#include <audio/AudioOutput.h>
#include <audio/AudioEncoderException.h>
#include <audio/AudioEncoderProfile.h>
#include <audio/AudioMetaData.h>

#include <cdio/sector.h>

namespace audio
{
static const int SAMPLES_BUF_SIZE = 1024 * 2;

static FLAC__StreamEncoderWriteStatus write_callback(const FLAC__StreamEncoder *, const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned currentFrame, void* clientData)
{
   FlacAudioEncoder* flac_encoder = static_cast<FlacAudioEncoder*>(clientData);

   int32_t result = flac_encoder->write_to_output(const_cast<FLAC__byte*>(buffer), static_cast<uint32_t>(bytes));

   if (result == static_cast<ssize_t>(bytes))
      return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;

   return FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR;

}
//-------------------------------------------------------------------------
// FlacAudioEncoder

/*
    Constructor
 */
FlacAudioEncoder::FlacAudioEncoder(AudioOutput::SharedPtr out) :
   AudioEncoder(),
   _output(out),
   _profile(nullptr),
   _encoder(nullptr)
{
}

/*
    Free resources
 */
FlacAudioEncoder::~FlacAudioEncoder()
{
}

/*
 */
std::string FlacAudioEncoder::type() const
{
   return "FLAC";
}

/*
 */
void FlacAudioEncoder::setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr /* metadata */, uint32_t data_size)
{
   _profile = profile;

   // allocate the encoder 
   _encoder = FLAC__stream_encoder_new();

   if (_encoder == nullptr) 
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: allocating encoder");
      return;
   }

   if (not FLAC__stream_encoder_set_channels(_encoder, profile->channels()))
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: could not set channel count");
      return;
   }

   if (not FLAC__stream_encoder_set_sample_rate(_encoder, profile->sample_rate()))
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: could not set sample rate");
      return;
   }

   if (not FLAC__stream_encoder_set_bits_per_sample(_encoder, profile->bits_per_sample()))
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: could not set sample width");
      return;
   }

   if (not FLAC__stream_encoder_set_compression_level(_encoder, profile->quality_level()))
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: could not set compression level");
      return;
   }

   FLAC__stream_encoder_set_verify(_encoder, true);
   //FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples);

   FLAC__StreamEncoderInitStatus init_status = FLAC__stream_encoder_init_stream(_encoder,
                                                                                write_callback,
                                                                                nullptr,
                                                                                nullptr,
                                                                                nullptr,
		                                                                this);
   if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) 
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: could not initialize FLAC encoder");
   }

}

/*
 */
int32_t FlacAudioEncoder::encode(int8_t* data,  uint32_t len)
{
   FLAC__int32 buffer[SAMPLES_BUF_SIZE];

   int32_t channels =  _profile->channels(); 
   int32_t left_samples = len / channels; 

   while (left_samples > 0)
   {
      int32_t samples = left_samples > SAMPLES_BUF_SIZE ? SAMPLES_BUF_SIZE : left_samples;

      // convert the packed little-endian 16-bit PCM samples into an 
      // interleaved FLAC__int32 buffer for libFLAC
      for (int i = 0; i < samples; i++)
      { 
         // inefficient but simple and works on big- or little-endian machines.
         buffer[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)data[2 * i + 1] << 8) | (FLAC__int16)data[2 * i]);
      }

      // feed samples to encoder
      if (not FLAC__stream_encoder_process_interleaved(_encoder, buffer, samples / 2))
      {
         THROW_EXCEPTION(AudioEncoderException, "ERROR: FLAC__stream_encoder_process_interleaved error");
      }
      
      left_samples -= samples;
      data += samples * 2; // skip processed samples      
   }

   return 1;
}

/*
 */
void FlacAudioEncoder::tear_down()
{
   if (not FLAC__stream_encoder_finish(_encoder))
   {
      THROW_EXCEPTION(AudioEncoderException, "ERROR: could not finish encoding");
   }

   if (_encoder)
   {
      FLAC__stream_encoder_delete(_encoder);
      _encoder = nullptr;
   }
}

/*
 */
int32_t FlacAudioEncoder::write_to_output(uint8_t* buffer, uint32_t len)
{
   return _output->write(buffer, len);
}

} // end of namespace
