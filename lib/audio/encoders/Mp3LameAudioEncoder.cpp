// Mp3LameAudioEncoder.cpp
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

#include <audio/encoders/Mp3LameAudioEncoder.h>

#include <orion/Exception.h>
#include <audio/AudioEncoderException.h>
#include <audio/AudioEncoderProfile.h>
#include <audio/AudioMetaData.h>
#include <audio/AudioOutput.h>

#define OUT_BUFFER_SIZE 16384

namespace audio
{

/*!
    Creates a new instance of the <code>Mp3LameAudioEncoder</code> class.
 */
Mp3LameAudioEncoder::Mp3LameAudioEncoder(AudioOutput::SharedPtr out) :
   AudioEncoder(),
   _output(out),
   _profile(nullptr)
{
}

/*!
    Free allocated resources.
 */
Mp3LameAudioEncoder::~Mp3LameAudioEncoder()
{

}

/*
 */
std::string Mp3LameAudioEncoder::type() const
{
   return "Mp3 Lame";
}

/*
 */
void Mp3LameAudioEncoder::setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr metadata, uint32_t /* data_size */)
{
   _profile = profile;

   _gf = lame_init();
   if (_gf == 0) 
   {
      THROW_EXCEPTION(AudioEncoderException, "Error initialising lame library");
   }

   // Set the format of the input data
   lame_set_in_samplerate(_gf, _profile->sample_rate());
   lame_set_num_channels(_gf, _profile->channels());

   // Set the sample rate of the output
   lame_set_out_samplerate(_gf, _profile->sample_rate());

   // Choose an encoding method
   switch (profile->encode_method()) 
   {
      case ABR:
         lame_set_VBR(_gf, vbr_abr);
	 lame_set_VBR_mean_bitrate_kbps(_gf, _profile->avg_bitrate());
         break;
      case CBR:
         lame_set_VBR(_gf, vbr_off);
         lame_set_brate(_gf, _profile->avg_bitrate());
         break;
      case VBR:
         lame_set_VBR(_gf, vbr_mtrh);
         lame_set_VBR_min_bitrate_kbps(_gf, _profile->min_bitrate());
         lame_set_VBR_max_bitrate_kbps(_gf, _profile->max_bitrate());
         break;
      case QVBR:
         lame_set_VBR(_gf, vbr_mtrh);
         // Set a VBR rate (0 High, ... , 9 low)
         lame_set_VBR_q(_gf, _profile->quality_level());

         //lame_set_brate(_gf, _profile->avg_bitrate());
         lame_set_VBR_min_bitrate_kbps(_gf, _profile->min_bitrate());

         //lame_set_lowpassfreq(_gf, int);
         break;
      default:
         THROW_EXCEPTION(AudioEncoderException, "Unsuported encoding method");
   }

   if ("stereo" == _profile->audio_mode()) 
   {
      lame_set_mode(_gf, STEREO);
   }
   else if ("joint" == _profile->audio_mode()) 
   {
      lame_set_mode(_gf, JOINT_STEREO);
   }
   else if ("mono" == _profile->audio_mode()) 
   {
      lame_set_mode(_gf, MONO);
   }

   // Now that all the options are set, lame needs to analyze them and
   // set some more internal options and check for problems.
   if (lame_init_params(_gf) < 0) 
   {
       THROW_EXCEPTION(AudioEncoderException, "Error initialising lame parameters");
   }
   lame_print_config(_gf);
   lame_print_internals(_gf);

   // Add tags to mp3 file
   set_tags(metadata);
}

/*
 */
int32_t Mp3LameAudioEncoder::encode(const int8_t* data,  uint32_t len)
{
   uint8_t out_buffer[OUT_BUFFER_SIZE];

   // encode
   int out_bytes = lame_encode_buffer_interleaved(_gf,
                                                  reinterpret_cast<int16_t*>(const_cast<int8_t*>(data)),
                                                  len / 4,
                                                  out_buffer,
                                                  OUT_BUFFER_SIZE);
   _output->write(out_buffer, out_bytes);

   return out_bytes;
}

/*
 */
void Mp3LameAudioEncoder::tear_down()
{
   uint8_t buffer[OUT_BUFFER_SIZE];

   int out_bytes = lame_encode_flush(_gf, buffer, 0);

   _output->write(buffer, out_bytes);

   lame_close(_gf);
}

/*
 */
void Mp3LameAudioEncoder::set_tags(AudioMetaData::SharedPtr metadata)
{
   std::string data = metadata->title();
   if (not data.empty()) 
   {
      id3tag_set_title(_gf, data.c_str());
   }

   data = metadata->artist();
   if (not data.empty()) 
   {
      id3tag_set_artist(_gf, data.c_str());
   }

   data = metadata->album();
   if (not data.empty()) 
   {
      id3tag_set_album(_gf, data.c_str());
   }

   data = metadata->comment();
   if (not data.empty()) 
   {
      id3tag_set_comment(_gf, data.c_str());
   }

   data = metadata->date();
   if (not data.empty()) 
   {
      id3tag_set_year(_gf, data.c_str());
   }

   data = metadata->track_number();
   if (not data.empty()) 
   {
      id3tag_set_track(_gf, data.c_str());
   }

   data = metadata->genre();
   if (not data.empty()) 
   {
      if (id3tag_set_genre(_gf, data.c_str()) == -1)
         THROW_EXCEPTION(AudioEncoderException, "Genre number out of range.");
   }

   if (lame_init_params(_gf) < 0)
      THROW_EXCEPTION(AudioEncoderException, "lame_init_params failed when adding a tag.");
}

} // end of namespace
