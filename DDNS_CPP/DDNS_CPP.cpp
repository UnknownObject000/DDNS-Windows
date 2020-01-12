// DDNS_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "IPGetter.h"
#include "sdk.h"
#include "uson.h"
#include <Windows.h>
#pragma comment(lib, "Advapi32")
constexpr auto SLEEP_TIME = 30000;

USON config;
DOMAIN_ dnsinfo;
string IPv4, IPv6;
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

enum IPChange
{
	ipv4,
	ipv6,
	both,
	none
};

void ReadConfig()
{
	int ret = config.ReadFile("C:\\Windows\\System32\\config.uson");
	if (ret != 0)
	{
		cout << "Cannot load 'config.uson'" << endl;
		exit(ret);
	}
	ret = dnsinfo.LoadFile(config["DNS_FILE_PATH"]);
	if (ret != 0)
	{
		cout << "Cannot load '" << config["DNS_FILE_PATH"] << "'" << endl;
		exit(ret);
	}
	return;
}

IPChange UpdateIP()
{
	string v4, v6;
	v4 = IP::IPv4();
	v6 = IP::IPv6();
	if (IPv4 == v4 && IPv6 == v6)
	{
		return none;
	}
	else if (IPv4 != v4 && IPv6 != v6)
	{
		IPv4 = v4;
		IPv6 = v6;
		return both;
	}
	else if (IPv4 != v4)
	{
		IPv4 = v4;
		return ipv4;
	}
	else
	{
		IPv6 = v6;
		return ipv6;
	}
	return none;
}

string SwitchIP(string type)
{
	if (type == "A")
		return IPv4;
	else if (type == "AAAA")
		return IPv6;
	return IPv4;
}

string IPNotChangeLog()
{
	time_t ti;
	time(&ti);
	tm* lt = localtime(&ti);
	int year = lt->tm_year + 1900;
	int month = lt->tm_mon + 1;
	int day = lt->tm_mday;
	int hour = lt->tm_hour;
	int minute = lt->tm_min;
	int second = lt->tm_sec;
	int size = 60 + IPv4.length() + IPv6.length();
	char* recv = new char[size];
	sprintf(recv, "IP Doesn't change at %04d-%02d-%02d %02d:%02d:%02d.\nIPv4:%s\nIPv6%s\n", year, month, day, hour, minute, second, IPv4.c_str(), IPv6.c_str());
	string ret = recv;
	delete[] recv;
	return ret;
}

int UpdateDomainRecord()
{
	IPChange ipc;
	ipc = UpdateIP();
	if (ipc == none)
		return -1;//IP_NOT_CHANGE
	string domain, sub, dns;
	SDK sdk(config["ACCESS_KEY_ID"], config["ACCESS_KEY_SERCET"], config["LOG_FILE_PATH"]);
	if (ipc == both)
	{
		for (int i = 0; i < dnsinfo.GetDomainCount(); i++)
		{
			domain = dnsinfo.GetDomain(i);
			for (int j = 0; j < dnsinfo.GetSubCount(domain); j++)
			{
				sub = dnsinfo.GetSubName(domain, j);
				dns = dnsinfo.GetDNSType(domain, sub);
				sdk.UpdateDomainRecord(domain, sub, dns, SwitchIP(dns));
			}
		}
		return 0;//BOTH_IP_CHANGED
	}
	else if (ipc == ipv4)
	{
		for (int i = 0; i < dnsinfo.GetDomainCount(); i++)
		{
			domain = dnsinfo.GetDomain(i);
			for (int j = 0; j < dnsinfo.GetSubCount(domain); j++)
			{
				sub = dnsinfo.GetSubName(domain, j);
				dns = dnsinfo.GetDNSType(domain, sub);
				if (dns == "AAAA")
					continue;
				sdk.UpdateDomainRecord(domain, sub, dns, SwitchIP(dns));
			}
		}
		return 4;//IPV4_CHANGED
	}
	else if (ipc == ipv6)
	{
		for (int i = 0; i < dnsinfo.GetDomainCount(); i++)
		{
			domain = dnsinfo.GetDomain(i);
			for (int j = 0; j < dnsinfo.GetSubCount(domain); j++)
			{
				sub = dnsinfo.GetSubName(domain, j);
				dns = dnsinfo.GetDNSType(domain, sub);
				if (dns == "A")
					continue;
				sdk.UpdateDomainRecord(domain, sub, dns, SwitchIP(dns));
			}
		}
		return 6;//IPV6_CHANGED
	}
	else
		return -2;
	return -3;
}

void Service_Main()
{
	int res = UpdateDomainRecord();
	if (res == -1)
	{
		fstream f(config["LOG_FILE_PATH"].c_str(), ios::out | ios::app);
		f << IPNotChangeLog() << endl;
		f.close();
	}
	return;
}

void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;
	default:
		break;
	}
	// Report current status
	SetServiceStatus(hStatus, &ServiceStatus);
	return;
}

void ServiceMain(int argc, char** argv)
{
	int error;

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;


	hStatus = RegisterServiceCtrlHandler(L"UnknownObjectDDNSService", (LPHANDLER_FUNCTION)ControlHandler);
	if (hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		// Registering Control Handler failed
		return;
	}
	// Initialize Service 
	error = true;
	if (!error)
	{
		// Initialization failed
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;
	}
	// We report the running status to SCM. 
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);
	// The worker loop of a service
	while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		//what your service do
		Service_Main();
		Sleep(SLEEP_TIME);
	}
	return;
}


int main()
{
	ReadConfig();
	fstream f(config["LOG_FILE_PATH"].c_str(), ios::in);
	if (!f.is_open())
	{
		f.close();
		f.open(config["LOG_FILE_PATH"].c_str(), ios::out);
		f << "DDNS Version 1.0.0 Operation Log" << endl << endl;
		f.close();
	}
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = (LPWSTR)(L"UnknownObjectDDNSService"); //Change to your service name
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	StartServiceCtrlDispatcher(ServiceTable);
	/*
	while (true)
	{
		Service_Main();
		cout << "Running......" << endl;
		Sleep(SLEEP_TIME);
	}
	*/
	exit(-1);
	return -32767;
}
