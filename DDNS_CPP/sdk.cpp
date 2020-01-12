#include "pch.h"
#include "sdk.h"

int SDK::GetRecordID(string domain_name,string record_name,string &id,string &oldvalue)
{
	AlibabaCloud::InitializeSdk();
	ClientConfiguration configuration("cn-beijing");
	AlidnsClient client(akid,aksr,configuration);
	Model::DescribeDomainRecordsRequest request;
	request.setPageSize(500);
	request.setRRKeyWord(record_name);
	request.setDomainName(domain_name);
	auto outcome = client.describeDomainRecords(request);
	if (!outcome.isSuccess()) 
	{
		sdk_error = outcome.error().errorCode();
		AlibabaCloud::ShutdownSdk();
		opt_error="SDK_CANNOT_GET_INFO";
		return SDK_INIT_FALIURE;
	}
	else
	{
		sdk_error = "";
		std::vector<Model::DescribeDomainRecordsResult::Record> records;
		records=outcome.result().getDomainRecords();
		if(records.size()>1)
		{
			AlibabaCloud::ShutdownSdk();
			for(int i=0;i<records.size();i++)
			{
				if(records[i].rR==record_name)
				{
					id = records[i].recordId;
					oldvalue = records[i].value;
					sdk_error="";
					opt_error="";
					return REQUEST_SUCCESS;
				}
			}
			return MUTI_RECORD_FOUND;
		}
		else if(records.size()<=0)
		{
			AlibabaCloud::ShutdownSdk();
			opt_error="SDK_FOUND_NOTHING";
			return NO_RECORD_FOUND;
		}
		else
		{
			AlibabaCloud::ShutdownSdk();
			id = records[0].recordId;
			oldvalue = records[0].value;
			sdk_error="";
			opt_error="";
			return REQUEST_SUCCESS;
		}
	}
	opt_error="THE_PROGRAM_IS_CRAZY";
	return UNKNOWN_ERROR;
}

int SDK::ChangeRecordValue(string domain_name,string record_name,string record_type,string newvalue)
{
	string record_id,oldvalue;
	int gid=GetRecordID(domain_name,record_name,record_id,oldvalue);
	if(gid!=0)
	{
		opt_error+=" & CANNOT_GET_RECORD_ID";
		return gid;
	}
	AlibabaCloud::InitializeSdk();
	ClientConfiguration configuration("cn-beijing");
	AlidnsClient client(akid,aksr,configuration);
	Model::UpdateDomainRecordRequest request;
	request.setRR(record_name);
	request.setType(record_type);
	request.setRecordId(record_id);
	request.setValue(newvalue);
	old_value=oldvalue;
	auto outcome=client.updateDomainRecord(request);
	if (!outcome.isSuccess())
	{
		sdk_error = outcome.error().errorCode();
		AlibabaCloud::ShutdownSdk();
		opt_error="SDK_CANNOT_SET_VALUE";
		return SDK_INIT_FALIURE;
	}
	sdk_error="";
	opt_error="";
	return REQUEST_SUCCESS;
}

string SDK::LogString_Success(string domain_name,string record_name,string oldvalue,string newvalue)
{
	time_t ti;
	time(&ti);
	tm* lt=localtime(&ti);
	int year=lt->tm_year+1900;
	int month=lt->tm_mon+1;
	int day=lt->tm_mday;
	int hour=lt->tm_hour;
	int minute=lt->tm_min;
	int second=lt->tm_sec;
	int size=60+10+domain_name.length()+record_name.length()+oldvalue.length()+newvalue.length();
	char* recive=new char[size];
	sprintf(recive,"DNS Record updated at %04d-%02d-%02d %02d:%02d:%02d\nDomain:%s.%s\nIP: %s ---> %s\n",year,month,day,hour,minute,second,record_name.c_str(),domain_name.c_str(),oldvalue.c_str(),newvalue.c_str());
	string ret=recive;
	delete[] recive;
	return ret;
}

string SDK::LogString_Faliure(string domain_name,string record_name,string oldvalue,string newvalue,int ecode)
{
	time_t ti;
	time(&ti);
	tm* lt=localtime(&ti);
	int year=lt->tm_year+1900;
	int month=lt->tm_mon+1;
	int day=lt->tm_mday;
	int hour=lt->tm_hour;
	int minute=lt->tm_min;
	int second=lt->tm_sec;
	int size=127+10+record_name.length()+domain_name.length()+newvalue.length()+oldvalue.length()+std::to_string(ecode).length()+sdk_error.length()+opt_error.length();
	char* recv=new char[size];
	sprintf(recv,"DNS Record update faliure! Current time: %04d-%02d-%02d %02d:%02d:%02d\nDomain: %s.%s\nCurrent IP:%s Recored IP:%s\nErrorCode:%s OptError:%s SDKError:%s\n",year,month,day,hour,minute,second,record_name.c_str(),domain_name.c_str(),newvalue.c_str(),oldvalue.c_str(),std::to_string(ecode).c_str(),opt_error.c_str(),sdk_error.c_str());
	string ret=recv;
	delete[] recv;
	return ret;
}

SDK::SDK()
{
	sdk_error="";
	opt_error="";
	return;
}

SDK::SDK(string id,string sr)
{
	sdk_error="";
	opt_error="";
	akid=id;
	aksr=sr;
	return;
}

SDK::SDK(string id,string sr,string lf)
{
	sdk_error="";
	opt_error="";
	akid=id;
	aksr=sr;
	logfile=lf;
	return;
}

void SDK::SetAccessKeyID(string id)
{
	akid=id;
	return;
}

void SDK::SetAccessKeySecret(string secret)
{
	aksr=secret;
	return;
}

void SDK::SetLogFile(string filepath)
{
	logfile=filepath;
	return;
}

string SDK::GetSDKError()
{
	return sdk_error;
}

string SDK::GetOptError()
{
	return opt_error;
}

bool SDK::UpdateDomainRecord(string Domain,string Record,string Type,string NewValue)
{
	int ret = ChangeRecordValue(Domain,Record,Type,NewValue);
	fstream file(logfile.c_str(),ios::out|ios::app);
	string ls = "";
	if(!file.is_open())
	{
		opt_error="CANNOT_OPEN_LOG_FILE";
		return false;
	}
	if(sdk_error=="" && opt_error=="")
	{
		ls=LogString_Success(Domain,Record,old_value,NewValue);
		file<<ls<<endl;
		file.close();
		return true;
	}
	else
	{
		ls=LogString_Faliure(Domain,Record,old_value,NewValue,ret);
		file<<ls<<endl;
		file.close();
		return false;
	}
	opt_error="THE_PROGRAM_IS_CRAZY";
	return false;
}
