// main.cpp
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

#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <sstream>

#if WIN32
#include <process.h>
#else
#include <unistd.h>
#endif


#include <core/env.h>
#include <nodus/Logging.h>
#include <nodus/SystemInfo.h>
#include <core/MainWindow.h>

using namespace std;
using namespace nodus;
using namespace nodus::logging;
using namespace mamut;

void setup_logger(std::fstream& file_stream)
{
   StreamOutputHandler::SharedPtr cout_handler = StreamOutputHandler::create(std::cout);
   StreamOutputHandler::SharedPtr file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Logger::Warning);
   logger.output_handlers().push_back(cout_handler);
   logger.output_handlers().push_back(file_handler);
}

std::string log_header()
{
   std::ostringstream stream_info;

   stream_info << "System Information\n"
               << "  Name:       " << Glib::get_user_name() << " (" << Glib::get_user_name() << ")\n"
               << "  Host name:  " << g_get_host_name() << "\n"
               << "  Process ID: " << getpid() << "\n"
               << "  Sytem:      " << get_os_version() << "\n"
               << "  CPU:        " <<  get_cpu() << "\n"
               << "\n"
               << "Program Information\n"
               << "  " << Glib::get_prgname() << "\n"
               << "\n"
               << "Setup Information\n"
               << "  User:\n"
               << "    Config dir: " << get_user_config_dir() << "\n"
               << "    Data dir:   " << get_user_data_dir() << "\n"
               << "    Plugin dir: " << get_user_plugins_dir() << "\n\n"
               << "  System:\n"
               << "    Config dir: " << get_system_config_dir() << "\n"
               << "    Data dir:   " << get_system_data_dir() << "\n"
               << "    Plugin dir: " << get_system_plugins_dir() << "\n";

   return stream_info.str();
}

int main (int argc, char** argv)
{
   // Help with uncaught exceptions in g++
   std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

   Gtk::Main kit(argc, argv);

   Glib::set_application_name("mamut");

   fstream fout("mamut.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START(sigc::ptr_fun(&log_header));

   Glib::RefPtr<Gtk::Builder> builder;
   try {
      std::string filename = build_path_to_glade_file("MamutGui.glade");

      LOG(Debug) << "Mamut main glade file: " << filename;

      builder = Gtk::Builder::create_from_file(filename);
   }
   catch (const Gtk::BuilderError& ex) {
      LOG_EXCEPTION(ex);
      return EXIT_FAILURE;
   }
   catch (Glib::FileError& fe) {
      LOG_EXCEPTION(fe);
      return EXIT_FAILURE;
   }
   catch (std::exception& e) {
      LOG_EXCEPTION(e);
      return EXIT_FAILURE;
   }


   MainWindow* main_window = NULL;

   builder->get_widget_derived("MainWindow", main_window);

   if (main_window != NULL) {
      kit.run(*main_window);
   }

   delete main_window;

   LOG_END();
   return EXIT_SUCCESS;
}
