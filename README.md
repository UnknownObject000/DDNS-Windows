English | [简体中文](./README-CN.md)

# DDNS-Windows 
This is a DDNS program written in C++.It can change **Aliyun** domain record automatically,IPv4 and IPv6 are both supported.This is the windows version.**Warning:Windows XP/Windows Server 2003 or lower are not supported.**

## Build
To bulid this program,you need:
  - Visual Studio 2017 or newer (with c++11 support)
  - ***core*** and ***alidns*** module of <a href="https://github.com/aliyun/aliyun-openapi-cpp-sdk">Aliyun-openapi-cpp-sdk</a>
  - libcurl installed.
  - SrvanyUI installed.

Bulid Step:
  1. Clone code to your device by execuing following command.
  ```bash
    git clone https://github.com/UnknownObject000/DDNS-Windows.git
  ```
  2. Install SrvanyUI.
    Run the 'install.exe' witch inside '3rd' folder to install it.(Administrator required.)
    
  3. Build and install.
      + Open Visual Studio.
      + Open this project and choose your platform(x86 for 32bit system,x64 for 64bit system)
      + Click Bulid->Bulid DDNS_CPP to bulid.
  4. Write Config
  
   **config.uson**
    
   config.uson recored your accesskey info and file path info,it must be putted in the same directory of main program. (the default is C:/Windows/System32)
    
   You can see the 'config.uson' in source code to fill it with your data.('<' and '>' should not included)
    
   **dns.uson**
    
   dns.uson recorded your domain/dns record info.You can tell the program where it is by editing 'config.uson'.
   
   You can see the 'dns.uson' in source code to fill it with your data.('<' and '>' should not included).Make sure that all domains in that file is belong to one account.If you have only one domain,you must set the 'DOMAIN_COUNT to 1 and all '<no.>' to 1.If you had more than one domain,set this from 1 to n.
   
   **log**
   
   The name of a log file can be whatever you want.What you just need to make sure is the directory that store log file exist and needn't to create a log file by yourself.
   
  4. Copy dll.
  
    Copy all dll files in 'dll' folder into the folder where the program are.
    
  5. Install service.
      - Open srvanyUI
      - Click ➕ and choose 'system service',witch is default.
      - Choose DDNS_CPP.exe as the server program.
      - Input service name,service description.
      - Save.
 ## Useage
  The service will start after you installed.You can check and manage it in system's server manager.
