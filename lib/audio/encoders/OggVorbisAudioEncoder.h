//
// VorbisAudioEncoder.h
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
#ifndef AUDIO_OGGVORBISAUDIOENCODER_H
#define AUDIO_OGGVORBISAUDIOENCODER_H

//include files
#include <string>

#include <orion/NonCopyable.h>
#include <audio/AudioEncoder.h>

#include <vorbis/vorbisenc.h>

namespace audio
{
//! Ogg Vorbis encoder
/*!
    Vorbis is the name of a specific audio compression scheme that's
    designed to be contained in Ogg.
 */
class OggVorbisAudioEncoder :
   public AudioEncoder,
   orion::NonCopyable
{
public:
   //! Construct an Ogg vorbis audio encoder
   OggVorbisAudioEncoder(AudioOutput::SharedPtr out);

   //! Free the encoders resources
   virtual ~OggVorbisAudioEncoder();

   virtual std::string type() const;

   virtual void setup(AudioEncoderProfile::SharedPtr profile, AudioMetaData::SharedPtr metadata, uint32_t data_size);

   virtual int32_t encode(const int8_t* data, uint32_t len);

   virtual void tear_down();

protected:
   void init_abr();
   void init_cbr();
   void init_vbr();
   void init_qvbr();
   uint32_t process_block();

   void set_tags(AudioMetaData::SharedPtr metadata);

private:
   AudioOutput::SharedPtr _output;
   AudioEncoderProfile::SharedPtr _profile;

   ogg_stream_state _ogg_stream; // take physical pages, weld into a logical stream of packets
   ogg_page         _ogg_page;   // one Ogg bitstream page.  Vorbis packets are inside
   ogg_packet       _ogg_packet; // one raw packet of data for decode

   vorbis_info      _vorbis_info;    // struct that stores all the static vorbis bitstream settings
   vorbis_comment   _vorbis_comment; // struct that stores all the user comments

   vorbis_dsp_state _vorbis_dsp_state; // central working state for the packet->PCM decoder
   vorbis_block     _vorbis_block;     // local working space for packet->PCM decode

};

}
#endif
