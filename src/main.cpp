// main.cpp
//
// Copyright 2013 tomas <tomasp@videotron.ca>
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

#include <getopt.h>
#include <iostream>
#include <fstream>

#include <orion/Logging.h>
#include <audio/Cdda.h>
#include <audio/CddaException.h>

using namespace orion;
using namespace orion::logging;
using namespace audio;

static struct option long_options[] =
{
    { "help",        no_argument,       0, 'h' },
    { "json-output", no_argument,       0, 'j' },
    { "no-metadata", no_argument,       0, 'n' },
    { "track",       required_argument, 0, 't' },
    { 0, 0, 0, 0 }
};

struct OptionValues 
{
   bool metadata;
   bool json_output;
   int  compression_level;
   int  chanels;
   int  bits_per_sample;
   int  sample_rate;
};

void usage()
{
   std::cout << "Usage: cdda2flac [options] \n"
             << "Options: \n"
             << "      -h, --help          Print this message and exit. \n"
             << "      -t, --track         Choose a track to extract \n"
             << "      -n, --no-metadata   Do not query for meta data \n"
             << "      -j, --json-output   Output to json format \n"
             << std::endl;
}

void init_options(OptionValues& option_values)
{
   option_values.metadata = true;
   option_values.json_output = false;
   option_values.compression_level = 5;
   option_values.chanels = 2;
   option_values.bits_per_sample = 16;
   option_values.sample_rate = 44000;
}

bool parse_cmd_line(int argc, char* argv[], OptionValues& option_values)
{
   int opt;
   int opt_index = 0;

   while ((opt = getopt_long(argc, argv, "ht:nj", long_options, &opt_index)) != -1) 
   {
      switch (opt) 
      {
         case 'h':
            usage();
            return false;
         case 't':
            //track = orion::convert_to<int>(optarg);
            break;
         case 'n':
            option_values.metadata = false;
            break;
         case 'j':
            option_values.json_output = true;
            break;
         default:
            usage();
            return false;
      }
   }
   return true;
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

void extract_tracks(Cdda::SharedPtr cd, const OptionValues& option_values)
{
/*
   FLAC__bool ok = true;
   FLAC__StreamEncoderInitStatus init_status;

   // allocate the encoder 
   FLAC__StreamEncoder* encoder = FLAC__stream_encoder_new();

   if (encoder == nullptr) 
   {
      // TODO: Change to exception
      LOG(Error) << "ERROR: allocating encoder\n";
      return;
   }

   if (not FLAC__stream_encoder_set_channels(encoder, option_values.chanels))
   {
      LOG(Error) << "ERROR: could not set channel count\n";
      return;
   }

   if (not FLAC__stream_encoder_set_sample_rate(encoder, sample_rate))
   {
      LOG(Error) << "ERROR: could not set sample rate\n";
      return;
   }

   if (not FLAC__stream_encoder_set_bits_per_sample(encoder, option_values.bits_per_sample))
   {
      LOG(Error) << "ERROR: could not set sample width\n";
      return;
   }
*/
   //FLAC__stream_encoder_set_verify(encoder, true);
   //FLAC__stream_encoder_set_compression_level(encoder, option_values.compression_level);
   //FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples);


}

int main (int argc, char** argv)
{
   OptionValues option_values;

   init_options(option_values);

   if (not parse_cmd_line(argc, argv, option_values))
      return EXIT_FAILURE;

   std::fstream fout("cdda2flac.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   Cdda::SharedPtr cd = Cdda::create();

   try 
   {
      cd->open();

      if (cd->is_open())
         extract_tracks(cd, option_values);

      cd->close();
   } 
   catch (CddaException& ae) 
   {
      LOG_EXCEPTION(ae);
   }

   LOG_END();
   return EXIT_SUCCESS;
}
