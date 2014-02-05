
#include <audio/Cdda.h>
#include <audio/AudioReader.h>
#include <audio/CddaRipper.h>
#include <audio/CddaTrack.h>
#include <audio/CddaException.h>
#include <audio/AudioDuration.h>
#include <audio/AudioEncoder.h>
#include <audio/AudioEncoderException.h>
#include <audio/AudioEncoderProfile.h>
#include <audio/AudioMetaData.h>
#include <audio/AudioOutput.h>
#include <orion/Logging.h>
#include <orion/StringUtils.h>

#include <getopt.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace orion;
using namespace orion::logging;
using namespace audio;

static struct option long_options[] =
{
    { "help",   no_argument,       0, 'h' },
    { "track",  required_argument, 0, 't' },
    { "format", required_argument, 0, 'f' },
    { 0, 0, 0, 0 }
};

void usage()
{
   std::cout << "Usage: track-read [options] \n"
             << "Options: \n"
             << "      -h, --help      Print this message and exit. \n"
             << "      -t, --track     Choose a track to extract \n"
             << "      -f, --format    Choose an encoding format wav, ogg, or mp3 \n"
             << std::endl;
}

int parse_cmd_line(int argc, char* argv[], int& track, std::string& format)
{
   int opt;
   int opt_index = 0;

   while ((opt = getopt_long (argc, argv, "ht:f:", long_options, &opt_index)) != -1) 
   {
      switch (opt) 
      {
         case 'h':
            usage();
            return 1;
         case 't':
            track = orion::convert_to<int>(optarg);
            break;
         case 'f':
            format = optarg;
            break;
         default:
            usage();
            return 1;
      }
   }
   return 0;
}

void setup_logger(std::fstream& file_stream)
{
   StreamOutputHandler::SharedPtr cout_handler = StreamOutputHandler::create(std::cout);
   StreamOutputHandler::SharedPtr file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Logger::Warning);
   logger.output_handlers().push_back(cout_handler);
   logger.output_handlers().push_back(file_handler);
}

void extract_track(Cdda::SharedPtr cd, const std::string& enc_name)
{
   std::cout << "\nExtracting track: 1"
             << std::endl;

   std::cout << "track        length       copy pre ch\n"
             << "======================================\n";

   Cdda::TrackList tracks = cd->tracks();

   CddaTrack* track = tracks.front();

   std::cout << std::setw(3) << static_cast<int>(track->track_number()) << "."
             << std::setw(9) << track->offset()
             << " [" << std::setw(8) << msf(track->offset()) << "]"
             << std::setw(5) << (track->copy_permit() ? "yes" : "no")
             << std::setw(5) << (track->preemphasis() ? "yes" : "no")
             << std::setw(3) << track->channels()
             << "\n\n";

   std::cout << "Extracting track 1\n";

   try 
   {
      AudioEncoderProfile::SharedPtr enc_profile = AudioEncoderProfile::create("FLAC");

      CddaRipper::SharedPtr ripper = CddaRipper::create(cd);

      ripper->rip_track(track, enc_profile); 

   }
   catch (CddaException& ae) 
   {
      LOG_EXCEPTION(ae);
      return;
   }

}

int main(int argc, char* argv[])
{

   int track_number = 1;
   std::string enc_name("wav");

   if (parse_cmd_line(argc, argv, track_number, enc_name))
      return EXIT_FAILURE;

   std::fstream fout("track-read.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   Cdda::SharedPtr cd = Cdda::create();

   try 
   {
      cd->open();

      if (cd->is_open())
         extract_track(cd, enc_name);

      cd->close();
   } 
   catch (CddaException& ae) 
   {
      LOG_EXCEPTION(ae);
   }
  
   return EXIT_SUCCESS;
}
