/*
 * utils.h - Miscellaneous functions.
 * 
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __utils_h__
#define __utils_h__

void remove_extention(std::string &s);
char *str_to_lower(char *p);

#ifdef _WIN32
#ifdef __ATLBASE_H__
 // declarations dependent on the atl librar
CComBSTR CComBSTRFromIID(GUID);
void DumpSupportedInterfaces(IUnknown *pUnk,bool);

#endif//__ATLBASE_H__

template< class T>
class CParamHelper
{
public:
	static std::string getValue(T &value)
	{
		std::stringstream ss;
		std::string s;

		ss << value;
		ss >> s;

		return s;
	}
};

class CTraceHelper
{
protected:
	std::string _m_params;
	std::string _m_method;
	FILE *_m_pFile;
	bool exitTraced;
public:
	CTraceHelper(FILE *pFile);
	~CTraceHelper();

	template <class T>
	void addParameter(const char *name, T value )
	{
		_m_params += "\t" + std::string(name) + " = ";
		_m_params += CParamHelper<T>::getValue(value) + "\n";
	}

	template < class M, class T>
	void addParameter_(const char *name, T value )
	{
		_m_params += "\t" + std::string(name) + " = ";
		_m_params += M::getValue(value) + "\n";
	}

	void addMethodName(const char *name);
	void traceEntry();
	void traceExit();
};

template <>
class CParamHelper<IID>
{
public:
	static std::string getValue(IID &value);
};

template <>
class CParamHelper<const WCHAR*>
{
public:
	static std::string getValue(const WCHAR *&value);
};

#define BEGIN_TRACE()  if(_m_traceFile) { CTraceHelper th(_m_traceFile); th.addMethodName(__FUNCTION__); 

#define ADD_PARAM(x)  		th.addParameter(#x,x); 
#define ADD_PARAM_HANDLER(x,H) th.addParameter_< H >(#x,x);

#define END_TRACE()			th.traceEntry(); 	}

#define SIMPLE_TRACE()   BEGIN_TRACE() \
				END_TRACE()

#define ENUM_MEMBER(x) if(value==x){return #x;} 
#define FLAG_ADD_OR(v) do{if(v.size()!=0) v+= " | ";}while(0)
#define FLAG_MEMBER(x) if(value & x) { FLAG_ADD_OR(fvalue); fvalue += #x; value &= ~x; }

#define BEGIN_ENUMHANDLER_MAP(X,Y) class X { public: static std::string getValue(Y value)  {
#define END_ENUMHANDLER_MAP() 	return "<Unknown>";  } }; 

#define BEGIN_FLAGHANDLER_MAP(X,Y) class X \
{ public: \
	static std::string getValue(Y value) \
	{ \
		std::string fvalue;

#define END_FLAGHANDLER_MAP()  if(value){\
			std::stringstream ss; \
			std::string ts; \
			FLAG_ADD_OR(fvalue); \
			fvalue += "0x"; ss << std::hex << value ; ss >> ts; \
			fvalue += ts;  } 	return fvalue;	}  };



#endif//WIN32

#endif//__ATLBASE_H__