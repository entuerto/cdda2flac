// AudioEncoderSettings.cpp
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

#include <audio/AudioEncoderSettings.h>

#include <string>

namespace audio
{

class AudioEncoderSettings::Private
{
public:
   Private() :
      sample_rate(44100),
      bits_per_sample(16),
      channels(2),
      quality_level(-1),
      audio_mode("stereo"),
      encode_method(CBR),
      min_bitrate(-1),
      avg_bitrate(-1),
      max_bitrate(-1)
      {}

public:
   uint32_t sample_rate;
   uint32_t bits_per_sample;
   uint8_t  channels;
   int32_t  quality_level;
   std::string audio_mode;
   EncodeMethod encode_method;
   int32_t  min_bitrate;
   int32_t  avg_bitrate;
   int32_t  max_bitrate;

};

AudioEncoderSettings::AudioEncoderSettings() :
   _private(new Private)
{

}


AudioEncoderSettings::~AudioEncoderSettings()
{
}

/*!
   \return The sample rate ex: 44100 hz
 */
uint32_t AudioEncoderSettings::sample_rate() const
{
   return _private->sample_rate;
}

/*!
   Sets the sample rate. This will usaly be 44100.
   \param value new sample rate
 */
void AudioEncoderSettings::sample_rate(uint32_t value)
{
   _private->sample_rate = value;
}

/*!
   \return The bits per sample ex: 16
 */
uint32_t AudioEncoderSettings::bits_per_sample() const
{
   return _private->bits_per_sample;
}

/*!
   Returns the bits per sample ex: 16

   Bits per sample used in audio files 8, 16, and 24.

   \param value new bits per sample to use
 */
void AudioEncoderSettings::bits_per_sample(uint32_t value)
{
   _private->bits_per_sample = value;
}

/*!
   \return The number of audio channels ex: 2 stereo, 1 mono
 */
uint8_t AudioEncoderSettings::channels() const
{
   return _private->channels;
}

/*!
   Sets the number of audio channels

   \param value new amount for channels 2 (stereo) or 1 (mono)
 */
void AudioEncoderSettings::channels(uint8_t value)
{
   _private->channels = value;
}

/*!
   \return The quality level asked
 */
int32_t AudioEncoderSettings::quality_level() const
{
   return _private->quality_level;
}

/*!
   Sets the quality level usually 0 to 10.

   \param value quality level to use
 */
void AudioEncoderSettings::quality_level(int32_t value)
{
   _private->quality_level = value;
}

/*!
   \return The audio mode ex: stereo, mono, etc.
 */
std::string AudioEncoderSettings::audio_mode() const
{
   return _private->audio_mode;
}

/*!
   Sets the audio mode

   - For the mp3 encoder this can be stereo, joint, mono, etc
   - For the wav or ogg encoders this has no effect

   \param value the new audio mode
 */
void AudioEncoderSettings::audio_mode(const std::string& value)
{
   _private->audio_mode = value;
}

/*!
   \return The encoding method. ex: avg bitrate, constant bitrate, etc
 */
EncodeMethod AudioEncoderSettings::encode_method() const
{
   return _private->encode_method;
}

/*!
   Sets the encoding method of the audio encoder.

   \param value Methos to use
 */
void AudioEncoderSettings::encode_method(EncodeMethod value)
{
   _private->encode_method = value;
}

/*!
   \return The minimum bitrate.
 */
int32_t AudioEncoderSettings::min_bitrate() const
{
   return _private->min_bitrate;
}

/*!
   Sets the minimum bitrate

   \param value sets the minimum bitrate for the encoder
 */
void AudioEncoderSettings::min_bitrate(int32_t value)
{
   _private->min_bitrate = value;
}

/*!
   \return the average bitrate.
 */
int32_t AudioEncoderSettings::avg_bitrate() const
{
   return _private->avg_bitrate;
}

/*!
   Sets the average bitrate

   \param value sets the average bitrate for the encoder
 */
void AudioEncoderSettings::avg_bitrate(int32_t value)
{
   _private->avg_bitrate = value;
}

/*!
   \return the maximum bitrate.
 */
int32_t AudioEncoderSettings::max_bitrate() const
{
   return _private->max_bitrate;
}

/*!
   Sets the maximum bitrate

   \param value sets the maximum bitrate for the encoder
 */
void AudioEncoderSettings::max_bitrate(int32_t value)
{
   _private->max_bitrate = value;
}

/*!
   Sets bitrates for the encoder.
 */
void AudioEncoderSettings::set_bitrates(int32_t min_bitrate, int32_t avg_bitrate, int32_t max_bitrate)
{
   _private->min_bitrate = min_bitrate;
   _private->avg_bitrate = avg_bitrate;
   _private->max_bitrate = max_bitrate;
}

AudioEncoderSettings::SharedPtr AudioEncoderSettings::create()
{
   return AudioEncoderSettings::SharedPtr(new AudioEncoderSettings);
}

} // end of namespace
