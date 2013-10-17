// Win32-utils.cpp
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

#include <orion/SystemInfo.h>

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <lmcons.h>   /* For UNLEN */
#include <process.h>

#include <psapi.h>
#include <sstream>
#include <locale>


namespace orion
{
namespace systeminfo
{

void get_loaded_modules(unsigned long process_id, ModuleList& modules)
{
   DWORD cbNeeded;

   // Get a list of all the modules in this process.
   HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION |
                                       PROCESS_VM_READ,
                                       FALSE, process_id);
   if (NULL == process_handle)
       return;

   HMODULE module_handles[1024];

   if (EnumProcessModules(process_handle, module_handles, sizeof(module_handles), &cbNeeded)) {
      for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
         TCHAR module_name[MAX_PATH];

         // Get the full path to the module's file.
         if (GetModuleFileNameEx(process_handle,
                                 module_handles[i],
                                 module_name, sizeof(module_name) / sizeof(TCHAR))) {
            // Print the module name and handle value.
            modules.push_back(module_name);
         }
      }
   }
   CloseHandle(process_handle);
}

std::vector<std::string> get_cpu_info()
{
                             // HARDWARE\DESCRIPTION\System\CentralProcessor contient une
                             // sous-clé pour chaque CPU, normalement nommées "0", "1", ...
                             // Le nom du CPU est donné par les valeurs
                             // ProcessorNameString, Identifier et/ou ~MHz de ces sous-clés.
                             // On produit ici une chaîne qui contient le nom de tous les CPUS,
                             // séparés par "; ".
   HKEY proc_key;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                    "HARDWARE\\DESCRIPTION\\System\\CentralProcessor",
                    0, KEY_ENUMERATE_SUB_KEYS, &proc_key) != ERROR_SUCCESS) {
      return std::vector<std::string>();
   }

   DWORD cpu_key_index = 0;
   char  cpu_key_name[16];
   DWORD cpu_key_nameSize = sizeof(cpu_key_name);
   _FILETIME dummy;

   std::vector<std::string> cpus;
   std::ostringstream cpu_name;

   while (RegEnumKeyEx(proc_key,
                       cpu_key_index,
                       cpu_key_name,
                       &cpu_key_nameSize,
                       NULL, NULL, NULL, &dummy) == ERROR_SUCCESS) {

      cpu_name << "CPU "
               << cpu_key_name
               << ": ";

      HKEY cpu_key;
      if (RegOpenKeyEx(proc_key, cpu_key_name, 0, KEY_QUERY_VALUE, &cpu_key) == ERROR_SUCCESS) {
         char cpuName[255];
         DWORD cpu_value_size = sizeof(cpuName);
         DWORD cpuValueType;
         if (RegQueryValueEx(cpu_key, "ProcessorNameString", NULL, &cpuValueType, (LPBYTE)&cpuName, &cpu_value_size) == ERROR_SUCCESS
             and cpuValueType == REG_SZ) {
            cpu_name << cpuName;
         } else {
            cpu_value_size = sizeof(cpuName);
            if (RegQueryValueEx(cpu_key, "Identifier", NULL, &cpuValueType, (LPBYTE)&cpuName, &cpu_value_size) == ERROR_SUCCESS
                and cpuValueType == REG_SZ)
               {
               cpu_name << cpuName;
               }
            DWORD cpu_speed;
            cpu_value_size = sizeof(cpu_speed);
            if (RegQueryValueEx(cpu_key, "~MHz", NULL, &cpuValueType, (LPBYTE)&cpu_speed, &cpu_value_size) == ERROR_SUCCESS
                and cpuValueType == REG_DWORD)
               {
               cpu_name << " "
                        << cpu_speed
                        << " MHz";
               }
         }
         RegCloseKey(cpu_key);

         cpus.push_back(cpu_name.str());

         cpu_name.str("");
      }
      cpu_key_index++;
      cpu_key_nameSize = sizeof(cpu_key_name);
   }
   RegCloseKey(proc_key);
   return cpus;
}

std::string get_os_version()
{
   OSVERSIONINFOEX osvi;
   SYSTEM_INFO si;

   ZeroMemory(&si, sizeof(SYSTEM_INFO));
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   bool os_version_info_ex = GetVersionEx((OSVERSIONINFO*) &osvi);

   if (not os_version_info_ex) {
      osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (not GetVersionEx((OSVERSIONINFO*) &osvi))
         return "";
   }

   GetSystemInfo(&si);

   std::ostringstream os_version;

   switch (osvi.dwPlatformId)
   {
      // Test for the Windows NT product family.
      case VER_PLATFORM_WIN32_NT:
         // Test for the specific product.
         if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
            if (osvi.wProductType == VER_NT_WORKSTATION)
               os_version << "Windows Vista ";
            else
               os_version << "Windows Server \"Longhorn\" ";
         }

         if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
            if (GetSystemMetrics(SM_SERVERR2)) {
               os_version << "Microsoft Windows Server 2003 \"R2\" ";
            } else if(osvi.wProductType == VER_NT_WORKSTATION
                      and si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
               os_version << "Microsoft Windows XP Professional x64 Edition ";
            } else {
               os_version << "Microsoft Windows Server 2003, ";
            }
         }

         if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            os_version << "Microsoft Windows XP ";

         if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
            os_version << "Microsoft Windows 2000 ";

         if (osvi.dwMajorVersion <= 4)
            os_version << "Microsoft Windows NT ";

         // Test for specific product on Windows NT 4.0 SP6 and later.
         if (os_version_info_ex) {
            // Test for the workstation type.
            if (osvi.wProductType == VER_NT_WORKSTATION &&
                si.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64) {
               if (osvi.dwMajorVersion == 4)
                  os_version << "Workstation 4.0 ";
               else if(osvi.wSuiteMask & VER_SUITE_PERSONAL)
                  os_version << "Home Edition ";
               else
                  os_version << "Professional ";
            }
         }

         // Display service pack (if any) and build number.
         if (osvi.dwMajorVersion == 4 &&
             lstrcmpi(osvi.szCSDVersion, TEXT("Service Pack 6")) == 0) {
            HKEY hKey;
            LONG lRet;

            // Test for SP6 versus SP6a.
            lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
                                0, KEY_QUERY_VALUE, &hKey);
            if (lRet == ERROR_SUCCESS)
               os_version << "Service Pack 6a (Build "
                          << (osvi.dwBuildNumber & 0xFFFF)
                          << ")";
            else { // Windows NT 4.0 prior to SP6a
               os_version << osvi.szCSDVersion
                          << " (Build "
                          << (osvi.dwBuildNumber & 0xFFFF)
                          << ")";
            }

            RegCloseKey( hKey );
         }
         else {// not Windows NT 4.0
            os_version << osvi.szCSDVersion
                       << " (Build "
                       << (osvi.dwBuildNumber & 0xFFFF)
                       << ")";
         }
         break;

      // Test for the Windows Me/98/95.
      case VER_PLATFORM_WIN32_WINDOWS:
         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
            os_version << "Microsoft Windows 95 ";
            if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
               os_version << "OSR2 ";
         }
         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
            os_version << "Microsoft Windows 98 ";
            if (osvi.szCSDVersion[1] == 'A' || osvi.szCSDVersion[1] == 'B')
               os_version << "SE ";
         }

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
            os_version << "Microsoft Windows Millennium Edition";
         }
         break;

      case VER_PLATFORM_WIN32s:
         os_version << "Microsoft Win32s";
         break;
   }
   return os_version.str();
}

std::string get_host_name()
{
   char hostname[100];
   DWORD size = sizeof (hostname);
   bool hostname_fail = not GetComputerName(hostname, &size);

   return hostname_fail ? "localhost" : hostname;
}

std::string get_user_name()
{
/*
   uint32_t len = UNLEN + 1;
   wchar_t  buffer[UNLEN + 1];
    
   if (GetUserNameW(buffer, (LPDWORD) &len))
   {
      // wide to UTF-8
      std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
      return conv.to_bytes(buffer);
   }
*/   
   uint32_t len = UNLEN + 1;
   char buffer[UNLEN + 1];
    
   if (GetUserName(buffer, (LPDWORD) &len))
   {
      return buffer;
   }
   return "";
}

int get_process_id()
{
   return getpid();
}

std::string get_program_name()
{
   HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION |
                                       PROCESS_VM_READ,
                                       FALSE, getpid());
   if (NULL == process_handle)
       return "";

   TCHAR module_name[MAX_PATH];

   GetModuleFileNameEx(process_handle,
                           NULL,
                           module_name, sizeof(module_name) / sizeof(TCHAR)); 

   CloseHandle(process_handle);

   return module_name;
}

} // namespace systeminfo
} // namespace orion
