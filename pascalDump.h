/*
 * pascalDump.h - Declaration of objects used for dumping type library information.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __pascalDump_h__
#define __pascalDump_h__

#ifdef _WIN32

class CDumpPascalInfo;

class CTypeLibErr
{
public:
  CTypeLibErr(){};
  ~CTypeLibErr(){};
};

#define CHECK_HRESULT(x) do{if(FAILED(x)){throw CTypeLibErr();}}while(0)

class CTypeInfo
{
public:
  struct property_item
  {
    INT read;
    INT write;
  };

  ITypeInfo *_m_typeInfo;
  ITypeInfo *_m_RealTypeInfo;

  TYPEATTR *_m_typeAttr;

  UINT _m_index;
  UINT _m_fnindex;
  UINT _m_pindex;

  CDumpPascalInfo *_m_parent;
  
  CComBSTR TypeName;
  CComBSTR BaseName;
  CComBSTR Description;
  
  std::vector<property_item> _m_properties;
public:
  CTypeInfo(CDumpPascalInfo *pParent,UINT index, UINT posIndex,ITypeInfo *pInfo);
  ~CTypeInfo();

  TYPEATTR *get_TypeAttr(){return _m_typeAttr;}
};

struct CGlobalPascalItems
{
public:
  std::vector<CComBSTR> TypeLibs;
  std::string output_directory;
public:
  CGlobalPascalItems();
  ~CGlobalPascalItems();

  bool CheckTypeLib(ITypeLib *pLib);
};

class CDumpPascalInfo
{
private:
 FILE *_m_pf;
 UINT _m_index;
 ITypeInfo *_m_pTypeInfo;
 ITypeLib *_m_pTypeLib;
 TLIBATTR *_m_pLibAttr;
 thash<int> _m_keywords;

 CComBSTR _m_libName;

 std::vector<CTypeInfo*> _m_typeInfoArr;
 std::vector<ITypeLib*> _m_refLibArr;
 std::vector<int> _m_typeAddedArr;

 int _m_RecordCount;
 int _m_AliasCount;
 int _m_CoClassCount;
 int _m_InterfaceCount;
 int _m_EnumCount;

 void DumpTypeInfos(ITypeLib *pLib );
 void CheckRefInfo( HREFTYPE href, CTypeInfo *pInfo, bool resort );
 void CheckElement( CTypeInfo *pInfo, ELEMDESC *pElem );
 void CheckTypeDesc( CTypeInfo *pInfo, TYPEDESC *pDesc, bool resort );

 void TranslateType(VARTYPE vt, CComBSTR &val,int ptrCnt);
 void GetElementName(CTypeInfo *pInfo, TYPEDESC *pDesc, CComBSTR &val, bool addPtr = false, int ptrCnt = 0);
 
 void GetResultName(CTypeInfo *pInfo, FUNCDESC *pFDesc, CComBSTR &val);
 void GetDefaultInterface( CTypeInfo *pInfo, CComBSTR &val);
 
 void DumpCoClassInterfaces(CTypeInfo*pInfo);
 void DumpCoClassFDecl();
 void DumpCoClassDecl();
 void DumpCoClassImpl();
 void DumpPropertyParams( UINT index, FUNCDESC *pFDesc, CTypeInfo *pInfo );
 void DumpProperty(UINT index, CTypeInfo *pInfo );
 void DumpDispProperty(UINT index, CTypeInfo *pInfo);
 void DumpProcedure(UINT index, CTypeInfo *pInfo, bool dispid);
 void DumpGuids();
 void DumpEnums();
 void DumpInterfaces();
 void DumpRecords();
 bool InitPascalGlobals();
 
public:
  CDumpPascalInfo();
  ~CDumpPascalInfo();

  CTypeInfo *GetTypeInfo(UINT indx);
  void SetFile(FILE *pf);
  void LoadTypeLib(ITypeLib *pLib);
  void FreeTypeLib();
  void DumpDispInterfaceDecl(CTypeInfo *pInfo);
  void DumpInterfaceDecl(CTypeInfo *pInfo);
  void DumpTypeLib(const char *pfname, ITypeLib *pLib,CGlobalPascalItems *pItems);
  void CheckPascalKeyword( CComBSTR &var );


  friend CTypeInfo;
};

#endif//WIN32

#endif  