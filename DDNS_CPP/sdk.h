#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/alidns/AlidnsClient.h>
#include <alibabacloud/alidns/AlidnsExport.h>

using namespace AlibabaCloud;
using namespace AlibabaCloud::Alidns;
using std::string;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;

class SDK
{
	private:
		const int REQUEST_SUCCESS = 0;
		const int SDK_INIT_FALIURE = -10023;
		const int MUTI_RECORD_FOUND = -10057;
		const int NO_RECORD_FOUND = -10093;
		const int UNKNOWN_ERROR = -11111;
		const int FILE_IO_ERROR = -20597;
		const int ARGUMENTS_IS_NULL = -20976;
	private:
		string akid;
		string aksr;
		string sdk_error;
		string opt_error;
		string logfile;
		string old_value;
		string new_value;
		int GetRecordID(string domain_name,string record_name,string &id,string &oldvalue);
		int ChangeRecordValue(string domain_name,string record_name,string record_type,string newvalue);
		string LogString_Success(string domain_name,string record_name,string oldvalue,string newvalue);
		string LogString_Faliure(string domain_name,string record_name,string oldvalue,string newvalue,int ecode);
	public:
		SDK();
		SDK(string id,string sr);
		SDK(string id,string sr,string lf);
		void SetAccessKeyID(string id);
		void SetAccessKeySecret(string secret);
		void SetLogFile(string filepath);
		string GetSDKError();
		string GetOptError();
		bool UpdateDomainRecord(string Domain,string Record,string Type,string NewValue);
};
