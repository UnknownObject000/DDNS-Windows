#include <iostream>
#include <fstream>
#include <string>

class USON
{
	private:
		struct USON_D
		{
			std::string data_name;
			std::string data_value;
		};
		int data_count;
                USON_D ram,*rom;
		int FIND_DATA(std::string obj,std::string src);
		USON_D Decode(std::string item);
	public:
		~USON();
		typedef USON_D USONDATA;
		int ReadFile(std::string file);
		std::string GetData(std::string name);
		bool ChangeValue(std::string name,std::string new_value);
		bool ChangeName(std::string oldname,std::string newname);
		int WriteToFile(std::string file);
		std::string operator[](std::string name);
};

class USON_LIST
{
	private:
		struct USON_LIST_D
		{
			int count;
			std::string *data;
		};
		USON_LIST_D rom;
		std::string add(char a,char b);
		std::string add(std::string a,char b);
	public:
		~USON_LIST();
		int DecodeLists(int cnt,std::string src);
		std::string GetItem(int i);
		std::string operator[](int i);
};

class DOMAIN_ : private USON , private USON_LIST
{
	public:
		enum DNS_TYPE
		{
			A,
			AAAA,
			_NULL
		};
		struct INFO
		{
			std::string domain;
			int sub_count;
			std::string* sub_name;
			DNS_TYPE* dns_type;
		};
	private:
		int domain_count;
		INFO* rom;
		DNS_TYPE stod(std::string src);
		std::string dtos(DNS_TYPE src);
		std::string CreateQueryString(std::string s,int n);
	public:
		~DOMAIN_();
		int LoadFile(std::string file);
		INFO GetDomainInfo(std::string domain);
		std::string GetDNSType(std::string domain,std::string sub_name);
		std::string GetSubName(std::string domain,int index);
		std::string GetDomain(int index);
		int GetSubCount(std::string domain);
		int GetDomainCount();
};
