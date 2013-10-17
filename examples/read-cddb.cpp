
#include <audio/Cdda.h>
#include <audio/CddaTrack.h>
#include <audio/AudioDuration.h>
#include <audio/CddaException.h>

#include <string>
#include <iostream>
#include <memory>

using namespace audio;

int main(int argc, char* argv[])
{
   std::auto_ptr<Cdda> cd(Cdda::create("\\\\.\\F:"));

   try {
      cd->open();
   } catch (CddaException& ae) {
      std::cout << ae.what()
                << std::endl;
      return EXIT_FAILURE;
   }


   std::cout << "\nTracks: "
             << int(cd->track_count())
             << std::endl;

   std::cout << "\nTable of contents (audio tracks only):\n"
             << "track        length       copy pre ch\n"
             << "======================================\n";

   Cdda::TrackList tracks = cd->tracks();

   Cdda::TrackList::iterator iter = tracks.begin();
   Cdda::TrackList::iterator end  = tracks.end();

   char buffer[256];
   for ( ; iter != end; iter++) {
      CddaTrack* t = *iter;

      AudioDuration dur(t->duration());


      sprintf(buffer,
              "%3d.  %7d [%02d:%02d.%02d] %s %s  %d",
              t->track_number(),
              t->offset(),
              int(dur.minutes()),
              int(dur.seconds()),
              int(dur.millisec()),
              t->copy_permit() ? "  OK" : "  no",
              t->preemphasis() ? " yes" : "  no",
              t->channels());

      std::cout << buffer << "\n";
   }

   AudioDuration total(cd->audio_length());

   sprintf(buffer, "TOTAL %7d [%02d:%02d:%02d.%02d]    (audio only)",
	   cd->last_sector() - cd->first_sector() + 1,
	   int(total.hours()),
	   int(total.minutes()),
	   int(total.seconds()),
	   int(total.millisec()));
   std::cout << buffer << "\n";

   cd->close();

   return EXIT_SUCCESS;
}
