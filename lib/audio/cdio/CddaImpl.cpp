// CddaImpl.cpp
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

#include "CddaImpl.h"

#include <cdio/logging.h>

#include <orion/Exception.h>
#include <orion/Logging.h>
#include <audio/AudioDuration.h>
#include <audio/CddaTrack.h>
#include <audio/cdio/CddaTrackImpl.h>
#include <audio/CddaException.h>

using namespace orion;
using namespace orion::logging;

namespace audio
{

static void log_handler(cdio_log_level_t level, const char message[])
{
   Logger& logger = Logger::get_logger();

   switch (level) 
   {
      case CDIO_LOG_DEBUG:
         LOG(Debug) << "CDIO:Debug: " << message;
         break;
      case CDIO_LOG_INFO:
         LOG(Info) << "CDIO:Info: " << message;
         break;
      case CDIO_LOG_WARN:
         LOG(Warning) << "CDIO:Warning: " << message;
         break;
      case CDIO_LOG_ERROR:
         LOG(Error) << "CDIO:Error: " << message;
         break;
      case CDIO_LOG_ASSERT:
         LOG(Error) << "CDIO:Assert: " << message;
         break;
   }
}


CddaImpl::CddaImpl(const std::string& device) :
   Cdda(),
   _device(device),
   _is_open(false),
   _cdio(NULL),
   _cdrom(NULL),
   _disk_id(0),
   _audio_length(0)
{
   cdio_log_set_handler(log_handler);
}

CddaImpl::~CddaImpl()
{
   Cdda::TrackList::iterator iter = _tracks.begin();
   Cdda::TrackList::iterator end  = _tracks.end();

   for ( ; iter != end; iter++) 
   {
      delete *iter;
   }
   close();
}

void CddaImpl::open()
{
   driver_id_t driver_id = DRIVER_UNKNOWN;

   if (_device.empty())
   {
   char **ppsz_cd_drives = cdio_get_devices_with_cap_ret(NULL,  
                                                         CDIO_FS_AUDIO, 
                                                         false,
                                                         &driver_id);
   if (ppsz_cd_drives && *ppsz_cd_drives)
      _device = ppsz_cd_drives[0];

   cdio_free_device_list(ppsz_cd_drives);
   }

   _cdio = cdio_open(_device.c_str(), driver_id);

   if (_cdio == NULL) 
   {
      THROW_EXCEPTION(CddaException, "Could not open device.");
   }

   // Detect empty drive   
   if (cdio_get_first_track_num(_cdio) == CDIO_INVALID_TRACK) 
   {
      close();
      return;
   }

   _cdrom = cdio_cddap_identify_cdio(_cdio, CDDA_MESSAGE_LOGIT, NULL);

   if (_cdrom == NULL) 
   {
      THROW_EXCEPTION(CddaException, "Could not read cdrom structure.");
   }

   cdio_cddap_verbose_set(_cdrom, CDDA_MESSAGE_LOGIT, CDDA_MESSAGE_LOGIT);

   int ret = cdio_cddap_open(_cdrom);
   if (ret != 0) 
   {
      THROW_EXCEPTION(CddaException, cdda_error_message(ret));
   }

   _is_open = true;

   read_toc();
   //read_cddb();
}

void CddaImpl::close()
{
   _is_open = false;

   // Close cd-rom connection
   // cddda_close destroys _cdio
   if (_cdrom != NULL) 
   {
      cdda_close(_cdrom);
   }
}

bool CddaImpl::is_open() const
{
   return _is_open;
}

std::string CddaImpl::device_name() const
{
   return cdio_get_default_device(_cdio);
}

Cdda::TrackList CddaImpl::tracks() const
{
   return _tracks;
}

int CddaImpl::track_count() const
{
   return _tracks.size();
}

int32_t CddaImpl::first_sector() const
{
   return cdda_disc_firstsector(_cdrom);
}

int32_t CddaImpl::last_sector() const
{
   return cdda_disc_lastsector(_cdrom);
}

AudioDuration CddaImpl::audio_length() const
{
   return AudioDuration(0,
                        _audio_length / CDIO_CD_FRAMES_PER_MIN,
                        (_audio_length / CDIO_CD_FRAMES_PER_SEC) % CDIO_CD_SECS_PER_MIN,
                        _audio_length % CDIO_CD_FRAMES_PER_SEC);
}

CdIo_t* CddaImpl::cdio() const
{
   return _cdio;
}

cdrom_drive_t* CddaImpl::cdrom() const
{
   return _cdrom;
}

void CddaImpl::read_toc()
{
   _cdtext = cdio_get_cdtext(_cdio);

   track_t tracks = cdda_tracks(_cdrom);

   for (int i = 1; i <= tracks; i++) 
   {
      if (not cdda_track_audiop(_cdrom, i))
         continue;

      CddaTrackImpl* cd_track = new CddaTrackImpl(this, i);

      _tracks.push_back(cd_track);

      _audio_length += cd_track->offset();
   }
}

void CddaImpl::read_cddb()
{
}

std::string CddaImpl::cdda_error_message(int code) const
{
   switch (code)
   {
      case -1:
         return "001: Unable to set CDROM to read audio mode";
      case -2:
         return "002: Unable to read table of contents lead-out";
      case -3:
         return "003: CDROM reporting illegal number of tracks";
      case -4:
         return "004: Unable to read table of contents header";
      case -5:
         return "005: Unable to read table of contents entry";
      case -6:
         return "006: Could not read any data from drive";
      case -7:
         return "007: Unknown, unrecoverable error reading data";
      case -8:
         return "008: Unable to identify CDROM model";
      case -9:
         return "009: CDROM reporting illegal table of contents";
      case -10:
         return "010: Unaddressable sector";
      case -100:
         return "100: Interface not supported";
      case -101:
         return "101: Drive is neither a CDROM nor a WORM device";
      case -102:
         return "102: Permision denied on cdrom (ioctl) device";
      case -103:
         return "103: Permision denied on cdrom (data) device";
      case -300:
         return "300: Kernel memory error";
      case -400:
         return "400: Device not open";
      case -401:
         return "401: Invalid track number";
      case -402:
         return "402: Track not audio data";
      case -403:
         return "403: No audio tracks on disc";
   }
   return "Error openning cd-rom drive.";
}

//--------------------------------------------------------------------------

std::string lba_to_msf(int32_t lsn)
{
   char* str = cdio_lba_to_msf_str(lsn);

   std::string tmp(str);

   free(str);

   return tmp;
}

#if 0
std::string AudioCdImpl::get_audio_device()
{
   driver_id_t driver_id;

   char** cd_drives = cdio_get_devices_with_cap_ret(NULL, CDIO_FS_AUDIO, false, &driver_id);

   std::string tmp = *cd_drives;

   //Don't need the list anymore
   cdio_free_device_list(cd_drives);

   return tmp;
}
#endif
}
