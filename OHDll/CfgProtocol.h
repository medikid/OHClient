#ifndef CFG_PROTOCOL_H
#define CFG_PROTOCOL_H


#include <string>
#include "CfgPrsr.h"

using namespace std;

enum	CfgType{tSECTION, tSTRING, tCHAR, tINT, tSHORT, tFLOAT, tDOUBLE};
class  CfgItem{
	public:
	CfgType type_;
	string name_;
	string value_;

	void Set(CfgType type, string name, string value){
		type_= type;
		name_= name;
		value_=value;
	}

	string Write(){	
		string s_txt="";
			switch(type_){
			case tSECTION:
				s_txt.append("\n");
				s_txt.append("[");s_txt.append(value_);s_txt.append("]");
				s_txt.append("\n");
				return s_txt;
			break;
			default:
				//s_txt.append("	");
				s_txt.append(name_);s_txt.append(" = ");s_txt.append(value_);
				s_txt.append("\n");
				return s_txt;
			break;
		}
	}

	bool Read(ConfigParser *prsr, string SectionName){	
		string value = prsr->Get(SectionName, name_, "UNKNOWN");
		if (value != "UNKNOWN"){
			value_ = value;
			return true;
		} else return false;
	}

};
struct  PROTOCOL{
public:
	CfgItem Section;
	CfgItem Version;
};

struct SERVER{
public:
	CfgItem Section;
	CfgItem Address;
	CfgItem Port;
};

struct CLIENT{
public:
	CfgItem Section;
	CfgItem Port;
};


#endif //CFG_PROTOCOL_H
