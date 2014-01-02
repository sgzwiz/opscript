/* 
 * utils.cpp - Miscellaneous utility functions.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#include "stdafx.h"
#include <string>
#include "PascalDump.h"
#include "utils.h"

void remove_extention(std::string &s)
{
  size_t i = s.size();

  if(i<4)
    return;

  if(s[s.size()-4] == '.')
    s.erase(s.size()-4,4);
}

char *str_to_lower(char *p)
{
  char *ptmp=p;
  for(;*ptmp;ptmp++)
  {
    if(((*ptmp)>='A')&&((*ptmp)<='Z'))
      *ptmp=tolower(*ptmp);
  }
  return p; 
}

#ifdef _WIN32

CComBSTR CComBSTRFromIID(GUID id)
{
  CComBSTR b;
  WCHAR *p;

  StringFromIID(id,&p);
  b=p;
  CoTaskMemFree(p);
  return b;
}

void DumpInterfaceInfo(IUnknown *pInt, FILE *pfile)
{
  IDispatch *pDisp;
  ITypeInfo *pInfo;
  ITypeLib *pLib;
  UINT indx;

  pInt->QueryInterface(IID_IDispatch,(void**)&pDisp);

  if(!pDisp)
    return;

  pDisp->GetTypeInfo(0,LOCALE_USER_DEFAULT,&pInfo);

  if(!pInfo)
  {
    RELEASE_INTERFACE(pDisp);
    return;
  }

  pInfo->GetContainingTypeLib(&pLib,&indx);

  if(pLib)
  {
    // Dump the type library...
    CDumpPascalInfo dInfo;

    dInfo.SetFile(pfile);
    dInfo.LoadTypeLib(pLib);
 
    CTypeInfo *pAInfo = dInfo.GetTypeInfo(indx);

    if((pAInfo->get_TypeAttr()->typekind == TKIND_DISPATCH)&&
       !(pAInfo->get_TypeAttr()->wTypeFlags & TYPEFLAG_FDUAL ))
      dInfo.DumpDispInterfaceDecl(pAInfo);
    else
      dInfo.DumpInterfaceDecl(pAInfo);

    RELEASE_INTERFACE(pLib);
  }


  RELEASE_INTERFACE(pInfo);
  RELEASE_INTERFACE(pDisp);
}


void DumpSupportedInterfaces(IUnknown *pUnk, bool dump_int=false)
{
  FILE *pfile = fopen("c:\\supinter.txt","wt");
  IUnknown *pDeclUnk;
  char iid_name[MAX_PATH];
  char int_name[MAX_PATH];
  WCHAR iid_wname[MAX_PATH];
  IID id;
  LONG len;

  HKEY k1,k2;
  DWORD i;

  RegOpenKey(HKEY_CLASSES_ROOT,"interface",&k1);

  for(i=0;ERROR_SUCCESS==RegEnumKey(k1,i,iid_name,MAX_PATH);i++)
  {
    MultiByteToWideChar(CP_ACP,0,iid_name,-1,iid_wname,MAX_PATH);
    IIDFromString(iid_wname,&id);
    pDeclUnk=0;
    pUnk->QueryInterface(id,(void**)&pDeclUnk);

    if(pDeclUnk)
    {
      len = MAX_PATH;
      RegOpenKey(k1,iid_name,&k2);
       RegQueryValue(k2,0,int_name,&len);
       RegCloseKey(k2);
      fprintf(pfile,"%s\n",int_name);

      if(dump_int)
      {
        DumpInterfaceInfo(pDeclUnk,pfile);
      }
  
      pDeclUnk->Release();
    }
  }


  RegCloseKey(k1);

  fclose(pfile);
}


#endif//WIN32

CTraceHelper::CTraceHelper(FILE *pFile)
{
	_m_pFile = pFile;
	exitTraced = false;
}

CTraceHelper::~CTraceHelper()
{
	traceExit();
}

void CTraceHelper::traceEntry()
{
	if(!_m_pFile)
		return;

	fprintf(_m_pFile,"Entering %s():\n", _m_method.c_str());
	fprintf(_m_pFile,"%s\n", _m_params.c_str());
	fflush(_m_pFile);
}

void CTraceHelper::traceExit()
{
	if(exitTraced)
		return;

	exitTraced=true;
}

void CTraceHelper::addMethodName(const char *name)
{
	_m_method = name;
}

// Parameter helpers
std::string CParamHelper<IID>::getValue(IID &value)
{
	USES_CONVERSION;
	WCHAR *text;
	std::string param;

	StringFromIID(value,&text);
	param = std::string(W2A(text));
	CoTaskMemFree(text);
	return param;
}

std::string CParamHelper<const WCHAR*>::getValue(const WCHAR *&value)
{
	if(value==NULL)
		return "<null>";

	USES_CONVERSION;
	std::string param;

	param += "\"";
	param += std::string(W2A(value));
	param += "\"";

	return param;
}
