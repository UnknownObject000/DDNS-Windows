#include "pch.h"
#include "uson.h"
#include <Windows.h>

int USON::FIND_DATA(std::string obj,std::string src)
{
	bool sign=true;
	bool *sub=new bool[obj.length()];
	for(int i=0;i<obj.length();i++)
		sub[i]=false;
	int end=(int)((int)(src.length()/obj.length())*((int)obj.length()));
	if(end==0)
	{
		delete[] sub;
		return -1;
	}
	for(int i=0;i<src.length();i++)
	{
		if(i>=end)
		{
			delete[] sub;
			return -2;
		}
		for(int j=0;j<obj.length();j++)
		{
			if(src[i+j]==obj[j])
			sub[j]=true;
		}
		for(int j=0;j<obj.length();j++)
		{
			sign=(sign && sub[j]);
		}
		if(sign==true)
		{
			delete[] sub;
			return i+1;
		}
		else
			sign=true;
	}
	delete[] sub;
	return -2;
}

USON::USON_D USON::Decode(std::string item)
{
	USON_D ret;
	std::string tmp;
	int pos_s=FIND_DATA("[-",item)+1;
	int pos_e=FIND_DATA("-]",item)-1;
	tmp=item.substr(pos_s,pos_e-pos_s);
	item=item.substr(FIND_DATA("=",item),item.length());
	ret.data_name=tmp;
	pos_s=FIND_DATA("[-",item)+1;
	pos_e=FIND_DATA("-]",item)-1;
	tmp=item.substr(pos_s,pos_e-pos_s);
	if(FIND_DATA("-]",tmp)>=0)
		tmp=tmp.substr(0,tmp.length()-2);
	ret.data_value=tmp;
	return ret;
}

USON::~USON()
{
	delete[] rom;
	return;
}

int USON::ReadFile(std::string file)
{
	std::string tmp;
	std::fstream fh(file.c_str(),std::ios::in);
	if(!fh.is_open())
		return GetLastError();//can not open file/file does not exist
	getline(fh,tmp);
	if(tmp=="[DOCTYPE USON]")
	{
		getline(fh,tmp);
		ram=Decode(tmp);
		if(ram.data_name=="DATA_COUNT")
		{
			data_count=std::stoi(ram.data_value);
			rom=new USON_D[std::stoi(ram.data_value)];
		}
		else
			return -2;
		for(int i=0;i<std::stoi(ram.data_value);i++)
		{
			getline(fh,tmp);
			if(tmp=="-END-")
				return -3;//unexpected end of file
			rom[i]=Decode(tmp);
			tmp="";
		}
		getline(fh,tmp);
		if(tmp=="-END-")
		{
			fh.close();

			return 0;//success
		}
		else
			return -3;
	}
	else
		return -2;//file format error
}

std::string USON::GetData(std::string name)
{
	for(int i=0;i<data_count;i++)
	{
		if(rom[i].data_name==name)
		return rom[i].data_value;
	}
	return "NO_MATCH_DATA_FOUND";
}

bool USON::ChangeValue(std::string name,std::string new_value)
{
	for(int i=0;i<data_count;i++)
	{
		if(rom[i].data_name==name)
		{
			rom[i].data_value=new_value;
			return true;
		}
	}
	return true;
}

bool USON::ChangeName(std::string oldname,std::string newname)
{
	for(int i=0;i<data_count;i++)
	{
		if(rom[i].data_name==oldname)
		{
			rom[i].data_name=newname;
			return true;
		}
	}
	return false;
}

int USON::WriteToFile(std::string file)
{
	std::fstream fw(file.c_str(),std::ios::out);
	if(!fw.is_open())
		return -1;//can not create file
	fw<<"[DOCTYPE USON]"<<std::endl;
	fw<<"[-DATA_COUNT-] = [-"<<data_count<<"-]"<<std::endl;
	for(int i=0;i<data_count;i++)
	{
		fw<<"[-"<<rom[i].data_name<<"-] = [-"<<rom[i].data_value<<"-]"<<std::endl;
	}
	fw<<"-END-"<<std::endl;
	fw.close();
	return 0;
}

std::string USON::operator[](std::string name)
{
	std::string empty="";
	for(int i=0;i<data_count;i++)
	{
	if(rom[i].data_name==name)
		return rom[i].data_value;
	}
	return empty;
}

USON_LIST::~USON_LIST()
{
	delete[] rom.data;
	return;
}

std::string USON_LIST::add(char a,char b)
{
	char str[3];
	str[0]=a;
	str[1]=b;
	str[2]='\0';
	return std::string(str);
}

std::string USON_LIST::add(std::string a,char b)
{
	char twstr[2];
	twstr[0]=b;
	twstr[1]='\0';
	std::string ret=a+std::string(twstr);
	return ret;
}

int USON_LIST::DecodeLists(int cnt,std::string src)
{
	rom.count=cnt;
	rom.data=new std::string[cnt];
	std::string tmp="";
	for(int i=0;i<src.length();i++)
	{
		if(src[i]==',')
		{
			rom.data[rom.count-cnt]=tmp;
			tmp="";
			cnt--;
		}
		else
		{
			tmp=add(tmp,src[i]);
		}
	}
	rom.data[rom.count-1]=tmp;
	return 0;
}

std::string USON_LIST::GetItem(int i)
{
	if(i<rom.count)
		return rom.data[i];
	else
		return "";
}

std::string USON_LIST::operator[](int i)
{
	std::string empty="";
	if(i<rom.count)
		return rom.data[i];
	else
		return empty;
}

DOMAIN_::~DOMAIN_()
{
	for(int i=0;i<domain_count;i++)
	{
		delete[] rom[i].sub_name;
		delete[] rom[i].dns_type;
	}
	delete[] rom;
	return;
}

DOMAIN_::DNS_TYPE DOMAIN_::stod(std::string src)
{
	DNS_TYPE ret;
	if(src=="A")
		ret=A;
	else if(src=="AAAA")
		ret=AAAA;
	else
		ret=_NULL;
	return ret;
}

std::string DOMAIN_::dtos(DOMAIN_::DNS_TYPE src)
{
	std::string ret;
	if(src==A)
		ret="A";
	else if(src==AAAA)
		ret="AAAA";
	else
		ret="";
	return ret;
}

std::string DOMAIN_::CreateQueryString(std::string s,int n)
{
	std::string tmp="DOMAIN_";
	tmp=tmp+std::to_string(n);
	if(s!="")
		tmp=tmp+"_";
	tmp=tmp+s;
	return tmp;
}

int DOMAIN_::LoadFile(std::string file)
{
	int rf=ReadFile(file);
	if(rf!=0)
		return rf;
	domain_count=std::stoi(GetData("DOMAIN_COUNT"));
	rom=new INFO[domain_count];
	for(int i=0;i<domain_count;i++)
	{
		rom[i].domain=GetData(CreateQueryString("",i+1));
		rom[i].sub_count=std::stoi(GetData(CreateQueryString("SUB_COUNT",i+1)));
		rom[i].sub_name=new std::string[rom[i].sub_count];
		rom[i].dns_type=new DNS_TYPE[rom[i].sub_count];
		DecodeLists(rom[i].sub_count,GetData(CreateQueryString("SUB_NAME_LIST",i+1)));
		for(int j=0;j<rom[i].sub_count;j++)
		{
			rom[i].sub_name[j]=GetItem(j);
		}
		DecodeLists(rom[i].sub_count,GetData(CreateQueryString("SUB_NAME_TYPE_LIST",i+1)));
		for(int j=0;j<rom[i].sub_count;j++)
		{
			rom[i].dns_type[j]=stod(GetItem(j));
		}
	}
	return 0;
}

DOMAIN_::INFO DOMAIN_::GetDomainInfo(std::string domain)
{
	INFO empty;
	for(int i=0;i<domain_count;i++)
		if(rom[i].domain==domain)
			return rom[i];
	return empty;
}

std::string DOMAIN_::GetDNSType(std::string domain,std::string sub_name)
{
	for(int i=0;i<domain_count;i++)
                if(rom[i].domain==domain)
			for(int j=0;j<rom[i].sub_count;j++)
				if(rom[i].sub_name[j]==sub_name)
					return dtos(rom[i].dns_type[j]);
	return "ERROR_NO_DOMAIN_MATCHED";
}

std::string DOMAIN_::GetSubName(std::string domain,int index)
{

	for(int i=0;i<domain_count;i++)
                if(rom[i].domain==domain)
			return rom[i].sub_name[index];
	return "ERROR_NO_DOMAIN_MATCHED";
}

std::string DOMAIN_:: GetDomain(int index)
{
	if(index<domain_count)
		return rom[index].domain;
	else
		return "ERROR_OUT_OF_RANGE";
}

int DOMAIN_::GetSubCount(std::string domain)
{
	for(int i=0;i<domain_count;i++)
                if(rom[i].domain==domain)
			return rom[i].sub_count;
	return 0;
}

int DOMAIN_::GetDomainCount()
{
	return domain_count;
}
