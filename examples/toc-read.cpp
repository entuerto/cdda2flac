
#include <orion/Logging.h>
#include <audio/Cdda.h>
#include <audio/CddaTrack.h>
#include <audio/AudioDuration.h>
#include <audio/CddaException.h>

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace orion;
using namespace orion::logging;
using namespace audio;

void setup_logger(std::fstream& file_stream)
{
   StreamOutputHandler::SharedPtr cout_handler = StreamOutputHandler::create(std::cout);
   StreamOutputHandler::SharedPtr file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Logger::Warning);
   logger.output_handlers().push_back(cout_handler);
   logger.output_handlers().push_back(file_handler);
}

void print_toc(Cdda::SharedPtr cd)
{
   std::cout << "\nTracks: "
             << int(cd->track_count())
             << std::endl;

   std::cout << "\nTable of contents (audio tracks only):\n"
             << "track        length       copy pre ch\n"
             << "======================================\n";

   Cdda::TrackList tracks = cd->tracks();

   Cdda::TrackList::iterator iter = tracks.begin();
   Cdda::TrackList::iterator end  = tracks.end();

   for ( ; iter != end; iter++) 
   {
      CddaTrack* track = *iter;

      std::cout << std::setw(3) << static_cast<int>(track->track_number()) << "."
                << std::setw(9) << track->offset()
                << " [" << std::setw(8) << msf(track->offset()) << "]"
                << std::setw(5) << (track->copy_permit() ? "yes" : "no")
                << std::setw(5) << (track->preemphasis() ? "yes" : "no")
                << std::setw(3) << track->channels()
                << "\n";
   }

   int32_t offset = cd->last_sector() - cd->first_sector() + 1;
   std::cout << "TOTAL "
             << std::setw(7) << offset
             << " [" << msf(offset) << "]    (audio only)"
             << std::endl;
}

int main(int argc, char* argv[])
{
   std::fstream fout("toc-read.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   Cdda::SharedPtr cd = Cdda::create();

   try 
   {
      cd->open();

      if (cd->is_open())
         print_toc(cd);

      cd->close();
   } 
   catch (CddaException& ae) 
   {
      LOG_EXCEPTION(ae);
   }

   LOG_END();
   return EXIT_SUCCESS;
}
