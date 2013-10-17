//
// OggVorbisAudioEncoder.cpp
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
//
#include <audio/encoders/OggVorbisAudioEncoder.h>

#include <orion/Exception.h>
#include <audio/AudioOutput.h>
#include <audio/AudioEncoderSettings.h>
#include <audio/AudioMetaData.h>
#include <audio/AudioEncoderException.h>

#include <time.h>

namespace audio
{

//-------------------------------------------------------------------------
// VorbisAudioEncoder

/*!
    Creates a new instance of the <code>OggVorbisAudioEncoder</code> class.
 */
OggVorbisAudioEncoder::OggVorbisAudioEncoder(AudioOutput* out) :
   AudioEncoder(),
   _output(out),
   _settings(NULL),
   _ogg_stream(),
   _ogg_page(),
   _ogg_packet(),
   _vorbis_info(),
   _vorbis_comment(),
   _vorbis_dsp_state(),
   _vorbis_block()
{

}

/*!
    Free allocated resources.
 */
OggVorbisAudioEncoder::~OggVorbisAudioEncoder()
{

}

/*
 */
std::string OggVorbisAudioEncoder::type() const
{
   return "Ogg Vorbis";
}

/*
 */
void OggVorbisAudioEncoder::setup(AudioEncoderSettings* settings, AudioMetaData* metadata, uint32_t /* data_size */)
{
   _settings = settings;

   // Encode setup
   vorbis_info_init(&_vorbis_info);

   // Add a comment
   vorbis_comment_init(&_vorbis_comment);
   vorbis_comment_add_tag(&_vorbis_comment, "ENCODER", "audio");

   set_tags(metadata);

   // Choose an encoding method
   switch (settings->encode_method()) {
      case ABR:
         init_abr();
         break;
      case CBR:
         init_cbr();
         break;
      case VBR:
         init_vbr();
         break;
      case QVBR:
         init_qvbr();
         break;
      default:
         THROW_EXCEPTION(AudioEncoderException, "Unsuported encoding method");
   }

   // set up the analysis state and auxiliary encoding storage
   vorbis_analysis_init(&_vorbis_dsp_state, &_vorbis_info);
   vorbis_block_init(&_vorbis_dsp_state, &_vorbis_block);

   // set up our packet->stream encoder
   // pick a random serial number; that way we can more likely build
   // chained streams just by concatenation
   srand(time(NULL));
   ogg_stream_init(&_ogg_stream, rand());

   // Vorbis streams begin with three headers; the initial header (with
   // most of the codec setup parameters) which is mandated by the Ogg
   // bitstream spec.  The second header holds any comment fields.  The
   // third header holds the bitstream codebook.  We merely need to
   // make the headers, then pass them to libvorbis one at a time;
   // libvorbis handles the additional Ogg bitstream constraints
   ogg_packet header;
   ogg_packet header_comm;
   ogg_packet header_code;
   vorbis_analysis_headerout(&_vorbis_dsp_state,
                             &_vorbis_comment,
                             &header,
                             &header_comm,
                             &header_code);

   // automatically placed in its own page
   ogg_stream_packetin(&_ogg_stream, &header);
   ogg_stream_packetin(&_ogg_stream, &header_comm);
   ogg_stream_packetin(&_ogg_stream, &header_code);

   // This ensures the actual audio data will start on a new page, as per spec
   while (ogg_stream_flush(&_ogg_stream, &_ogg_page)) {
      _output->write(_ogg_page.header, _ogg_page.header_len);
      _output->write(_ogg_page.body, _ogg_page.body_len);
   }
}

/*
 */
int32_t OggVorbisAudioEncoder::encode(int8_t* data, uint32_t len)
{
   int i;
   int channels = _settings->channels();

   if (len == 0) {
      return 0;
   }

   // expose the buffer to submit data
   float** an_buffer = vorbis_analysis_buffer(&_vorbis_dsp_state, len / 4);

   /* expose the buffer to submit data */
   /* uninterleave samples */
   for(i = 0; i < len / (channels * 2); i++) {
      for(int j = 0; j < channels; j++) {
         an_buffer[j][i] = ((data[i * 2 * channels + 2 * j  + 1] << 8) |
			                   (data[i * 2 * channels + 2 * j] & 0xff)) / 32768.0f;
	   }
   }

   // tell the library how much we actually submitted
   vorbis_analysis_wrote(&_vorbis_dsp_state, i);
   return process_block();
}

/*
 */
void OggVorbisAudioEncoder::tear_down()
{
   // send end-of-stream and process last block
   vorbis_analysis_wrote(&_vorbis_dsp_state, 0);
   process_block();

   // clean up and exit. vorbis_info_clear() must be called last
   ogg_stream_clear(&_ogg_stream);
   vorbis_block_clear(&_vorbis_block);
   vorbis_dsp_clear(&_vorbis_dsp_state);
   vorbis_comment_clear(&_vorbis_comment);
   vorbis_info_clear(&_vorbis_info);
}

/*
 */
void OggVorbisAudioEncoder::set_tags(AudioMetaData* metadata)
{
   //if (_vorbis_comment == NULL)
   //   throw AudioEncoderException("Vorbis comments not initialized.");

   std::string data = metadata->title();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "TITLE", data.c_str());
   }

   data =  metadata->album();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "ALBUM", data.c_str());
   }

   data = metadata->track_number();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "TRACK_NUMBER", data.c_str());
   }

   data = metadata->artist();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "ARTIST", data.c_str());
   }

   data = metadata->performer();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "PERFORMER", data.c_str());
   }

   data = metadata->copyright();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "COPYRIGHT", data.c_str());
   }

   data = metadata->license();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "LICENSE", data.c_str());
   }

   data = metadata->organization();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "ORGANIZATION", data.c_str());
   }

   data = metadata->comment();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "DESCRIPTION", data.c_str());
   }

   data = metadata->date();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "DATE", data.c_str());
   }

   data = metadata->location();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "LOCATION", data.c_str());
   }

   data = metadata->contact();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "CONTACT", data.c_str());
   }

   data = metadata->genre();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "GENRE", data.c_str());
   }

   data = metadata->isrc();
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "ISRC", data.c_str());
   }

   /*
   data = metadata-> ;
   if (not data.empty()) {
      vorbis_comment_add_tag(&_vorbis_comment, "ALBUM_ART", value.c_str());
   }
   */
}

/*
 */
void OggVorbisAudioEncoder::init_abr()
{
   /*
      Encoding using an average bitrate mode (ABR).
      example: 44kHz stereo coupled, average 128kbps ABR
   */

   int ret = vorbis_encode_init(&_vorbis_info,
                                _settings->channels(),
                                _settings->sample_rate(),
                                -1,
                                _settings->avg_bitrate(),
                                -1);
   /*
      do not continue if setup failed; this can happen if we ask for a
      mode that libVorbis does not support (eg, too low a quality mode, etc,
      will return 'OV_EIMPL')
   */

   THROW_IF3(ret, AudioEncoderException, "Error initialising encoder in ABR.");
}

/*
 */
void OggVorbisAudioEncoder::init_cbr()
{
   /*
      Encoding using a constant bitrate mode (CBR).
      example: 44kHz stereo coupled, average 128kbps CBR
   */

   int ret = vorbis_encode_init(&_vorbis_info,
                               _settings->channels(),
                               _settings->sample_rate(),
                               _settings->min_bitrate(),
                               _settings->avg_bitrate(),
                               _settings->max_bitrate());
   /*
      do not continue if setup failed; this can happen if we ask for a
      mode that libVorbis does not support (eg, too low a quality mode, etc,
      will return 'OV_EIMPL')
   */

   THROW_IF3(ret, AudioEncoderException, "Error initialising encoder in CBR.");
}

/*
 */
void OggVorbisAudioEncoder::init_vbr()
{
   /*
      Encode using a quality mode, but select that quality mode by asking for
      an approximate bitrate.  This is not ABR, it is true VBR, but selected
      using the bitrate interface, and then turning bitrate management off:
   */

   int ret = (vorbis_encode_setup_managed(&_vorbis_info,
                                          _settings->channels(),
                                          _settings->sample_rate(),
                                          -1,
                                          _settings->avg_bitrate(),
                                          -1)
              or vorbis_encode_ctl(&_vorbis_info, OV_ECTL_RATEMANAGE2_SET, NULL)
              or vorbis_encode_setup_init(&_vorbis_info));



  /*
      do not continue if setup failed; this can happen if we ask for a
      mode that libVorbis does not support (eg, too low a quality mode, etc,
      will return 'OV_EIMPL')
  */

   THROW_IF3(ret, AudioEncoderException, "Error initialising encoder in VBR.");
}

/*
 */
void OggVorbisAudioEncoder::init_qvbr()
{
   /*
      Encoding using a VBR quality mode.  The usable range is -.1
      (lowest quality, smallest file) to 1.0 (highest quality, largest file).
      Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR
   */

   int ret = vorbis_encode_init_vbr(&_vorbis_info,
                                    _settings->channels(),
                                    _settings->sample_rate(),
                                    _settings->quality_level() * .1);

  /*
      do not continue if setup failed; this can happen if we ask for a
      mode that libVorbis does not support (eg, too low a quality mode, etc,
      will return 'OV_EIMPL')
  */

   THROW_IF3(ret, AudioEncoderException, "Error initialising encoder in VBR.");
}

/*
 */
uint32_t OggVorbisAudioEncoder::process_block()
{
   uint32_t processed(0);
   // vorbis does some data preanalysis, then divvies up blocks for
   // more involved (potentially parallel) processing.  Get a single
   // block for encoding now
   while (vorbis_analysis_blockout(&_vorbis_dsp_state, &_vorbis_block) == 1) {
      // Do the main analysis, creating a packet
      vorbis_analysis(&_vorbis_block, 0);
      vorbis_bitrate_addblock(&_vorbis_block);

      while (vorbis_bitrate_flushpacket(&_vorbis_dsp_state, &_ogg_packet)) {
         // weld the packet into the bitstream
         ogg_stream_packetin(&_ogg_stream, &_ogg_packet);

         // write out pages (if any)
         while (ogg_stream_pageout(&_ogg_stream, &_ogg_page)) {
            _output->write(_ogg_page.header, _ogg_page.header_len);
            _output->write(_ogg_page.body, _ogg_page.body_len);

            processed += _ogg_page.header_len + _ogg_page.body_len;

            if (ogg_page_eos(&_ogg_page))
               break;
         }
      }
   }
   return processed;
}

} // End of namespace
