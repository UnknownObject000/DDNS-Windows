#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>


class IP
{
	private:
		static size_t downloadCallback(void *buffer, size_t sz, size_t nmemb, void *writer);
		static std::string GetWebString(std::string strUrl);
	public:
		static std::string IPv4();
		static std::string IPv6();
};
