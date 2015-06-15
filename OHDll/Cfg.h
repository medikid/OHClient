#ifndef CFG_H
#define CFG_H


#include <iostream>
#include <fstream>
#include <string>
#include "CfgProtocol.h"
#include "CfgPrsr.h"

namespace TCP{


class CONFIG{
public:
	string s_logFile;
	PROTOCOL protocol;
	SERVER server;
	CLIENT client;

	CONFIG(string cfgFile){
		s_logFile = cfgFile;
		defaultSettings();
		Load();
	}

	void defaultSettings(){
		protocol.Section.Set(tSECTION, "protocol", "PROTOCOL");
		protocol.Version.Set(tDOUBLE, "version", "1.0");

		server.Section.Set(tSECTION, "server", "SERVER");
		server.Address.Set(tSTRING, "address", "localhost");
		server.Port.Set(tINT, "port", "1000");

		client.Section.Set(tSECTION, "client", "CLIENT");
		client.Port.Set(tINT, "port", "10001");
	}

	bool Load(){
		Read();
		Print();
		return true;
	}
	bool Write(){
		const char* c_logFile = s_logFile.c_str();
		string s_logTxt = "; Test config file for ini_example.c and INIReaderTest.cpp \n";		
		s_logTxt.append(protocol.Section.Write());
		s_logTxt.append(protocol.Version.Write());

		s_logTxt.append(server.Section.Write());
		s_logTxt.append(server.Address.Write());
		s_logTxt.append(server.Port.Write());

		s_logTxt.append(client.Section.Write());
		s_logTxt.append(client.Port.Write());

		const char* c_logTxt = s_logTxt.c_str();

		FILE* pFile = fopen( c_logFile, "a");
		fprintf(pFile, "%s\n", c_logTxt);
		fclose(pFile);

		//delete pFile;
		//delete c_logTxt;
		return true;
	}

	void Read(){
		ConfigParser *cfgPrsr = new ConfigParser(s_logFile);
		protocol.Version.Read(cfgPrsr,protocol.Section.value_);

		server.Address.Read(cfgPrsr,server.Section.value_);
		server.Port.Read(cfgPrsr,server.Section.value_);

		client.Port.Read(cfgPrsr,server.Section.value_);
	}

	void Print(){
		std::cout << protocol.Section.value_ << ": "<<protocol.Version.name_ <<" = "<<protocol.Version.value_<<std::endl;

		std::cout << server.Section.value_ << ": "<<server.Address.name_ <<" = "<<server.Address.value_<<std::endl;
		std::cout << server.Section.value_ << ": "<<server.Port.name_ <<" = "<<server.Port.value_<<std::endl;

		std::cout << client.Section.value_ << ": "<<client.Port.name_ <<" = "<<client.Port.value_<<std::endl;

	}


};

}//tcp

#endif //CFG_H