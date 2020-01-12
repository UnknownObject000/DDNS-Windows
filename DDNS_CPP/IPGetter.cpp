#include "pch.h"
#include "IPGetter.h"

size_t IP::downloadCallback(void *buffer, size_t sz, size_t nmemb, void *writer)
{
	std::string* psResponse = (std::string*) writer;
	size_t size = sz * nmemb;
	psResponse->append((char*) buffer, size); 
	return sz * nmemb;
}
 
std::string IP::GetWebString(std::string strUrl)
{
	std::string strTmpStr;
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downloadCallback); 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strTmpStr);
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	std::string strRsp;
	if (res != CURLE_OK)
	{
		strRsp = "error";
	}
	else
	{
		strRsp = strTmpStr;
	}
	return strRsp;
}

std::string IP::IPv4()
{
	return GetWebString("http://api.ipify.org");
}

std::string IP::IPv6()
{
	return GetWebString("http://v6.ident.me");
}
