// AudioEncoderSettings.h
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

#ifndef AUDIO_AUDIOENCODERSETTINGS_H
#define AUDIO_AUDIOENCODERSETTINGS_H

#include <orion/MemoryUtils.h>
#include <orion/NonCopyable.h>
#include <audio/Common.h>

namespace audio
{
enum EncodeMethod {
   CBR,   //!< Constant bit rate
   ABR,   //!< Average bit rate
   VBR,   //!< Variable bit rate
   QVBR   //!< Quality based variable bit rate
};

//! Settings of the audio encoder
/*!
   This class configures the audio encoder. By setting
   the various properties the audio encoder will
   encode the audio in various methods.
 */
class AudioEncoderSettings :
   orion::NonCopyable
{
public:
   DECLARE_POINTERS(AudioEncoderSettings)

   AudioEncoderSettings();
   virtual ~AudioEncoderSettings();

   //! Return the sample rate ex: 44100 hz
   uint32_t sample_rate() const;

   //! Sets the sample rate
   void sample_rate(uint32_t value);

   //! Returns the bits per sample ex: 16
   uint32_t bits_per_sample() const;

   //! Sets the bits per sample
   void bits_per_sample(uint32_t value);

   //! Returns the number of audio channels ex: 2 stereo, 1 mono
   uint8_t channels() const;

   //! Sets the number of audio channels
   void channels(uint8_t value);

   //! Returns the quality level asked
   int32_t quality_level() const;

   //! Sets the quality level
   void quality_level(int32_t value);

   //! Returns the audio mode ex: stereo, mono, etc.
   std::string audio_mode() const;

   //! Sets the audio mode
   void audio_mode(const std::string& value);

   //! Returns the encoding metho. ex: avg bitrate, constant bitrate, etc
   EncodeMethod encode_method() const;

   //! Sets the encoding method
   void encode_method(EncodeMethod value);

   //! Returns the minimum bitrate.
   int32_t min_bitrate() const;

   //! Sets the minimum bitrate
   void min_bitrate(int32_t value);

   //! Returns the average bitrate.
   int32_t avg_bitrate() const;

   //! Sets the average bitrate
   void avg_bitrate(int32_t value);

   //! Returns the maximum bitrate.
   int32_t max_bitrate() const;

   //! Sets the maximum birate
   void max_bitrate(int32_t value);

   //! Sets bitrates
   void set_bitrates(int32_t min_bitrate, int32_t avg_bitrate, int32_t max_bitrate);


   static SharedPtr create();

   //! \todo Create advance options functions for audio encoders

private:
   class Private;

   const std::auto_ptr<Private> _private;
};

} // end of namespace
#endif /* AUDIOENCODERSETTINGS_H */
