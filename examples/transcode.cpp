
#include <audio/AudioEncoderProfile.h>
#include <audio/AudioTranscoder.h>
#include <orion/Exception.h>
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
    { "format", required_argument, 0, 'f' },
    { 0, 0, 0, 0 }
};

void usage()
{
   std::cout << "Usage: transcode [options] file\n"
             << "Options: \n"
             << "      -h, --help      Print this message and exit. \n"
             << "      -f, --format    Choose an encoding format wav, ogg, or mp3 \n"
             << std::endl;
}

int parse_cmd_line(int argc, char* argv[], std::string& file_name, std::string& format)
{
   int opt;
   int opt_index = 0;

   while ((opt = getopt_long(argc, argv, "h:f:", long_options, &opt_index)) != -1) 
   {
      switch (opt) 
      {
         case 'h':
            usage();
            return 1;
         case 'f':
            format = optarg;
            break;
         case '?':
            file_name = optarg;
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

void transcode(const std::string& file_name, const std::string& enc_name)
{
   std::cout << "Transcoding file: "
             << file_name
             << "\n";

   AudioEncoderProfile::SharedPtr enc_profile = AudioEncoderProfile::create(enc_name);
   AudioTranscoder::SharedPtr trans = AudioTranscoder::create();

   trans->transcode_file(file_name, enc_profile);
}

int main(int argc, char* argv[])
{
   std::string enc_name("FLAC");
   std::string file_name("track1.wav");

   if (parse_cmd_line(argc, argv, file_name, enc_name))
      return EXIT_FAILURE;

   std::fstream fout("transcode.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   try 
   {
      transcode(file_name, enc_name);
   } 
   catch (Exception& e) 
   {
      LOG_EXCEPTION(e);
   }
  
   return EXIT_SUCCESS;
}
