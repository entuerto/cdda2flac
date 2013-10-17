
#include <orion/Logging.h>
#include <audio/Cdda.h>
#include <audio/CddaTrack.h>
#include <audio/CdText.h>
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

int main(int argc, char* argv[])
{
   std::fstream fout("read-cdtext.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   Cdda::SharedPtr cd = Cdda::create();

   try 
   {
      cd->open();
   } 
   catch (CddaException& ae) 
   {
      LOG_EXCEPTION(ae);
      return EXIT_FAILURE;
   }

   std::cout << "\nTracks: "
             << cd->track_count()
             << std::endl;

   Cdda::TrackList tracks = cd->tracks();

   Cdda::TrackList::iterator iter = tracks.begin();
   Cdda::TrackList::iterator end  = tracks.end();

   for ( ; iter != end; iter++) 
   {
      CdText::SharedPtr cdtext = (*iter)->text();

      if (cdtext == NULL)
         continue;

      std::cout << "Track:     " << (*iter)->track_number() << "\n"
                << "Arranger:  " << cdtext->arranger() << "\n"
                << "Composer:  " << cdtext->composer() << "\n"
                << "Disc Id:   " << cdtext->disc_id() << "\n"
                << "Genre:     " << cdtext->genre() << "\n"
                << "Message:   " << cdtext->message() << "\n"
                << "Isrc:      " << cdtext->isrc() << "\n"
                << "Performer: " << cdtext->performer() << "\n"
                << "Song writer: " << cdtext->song_writer() << "\n"
                << "Title:     " << cdtext->title() << "\n" 
                << "-------------------------------------------" << "\n"
                << std::endl;
   }

   cd->close();

   LOG_END();
   return EXIT_SUCCESS;
}
