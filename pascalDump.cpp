/* 
 * pascalDump.cpp - Type library object implementation.  
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
#include "utils.h"
#include "pascaldump.h"


bool CDumpPascalInfo::InitPascalGlobals()
{
  _m_keywords.insert("dispid",0);
  _m_keywords.insert("threadvar",0);
  _m_keywords.insert("assembler",0);
  _m_keywords.insert("abstract",0);
  _m_keywords.insert("finalization",0);
  _m_keywords.insert("initialization",0);
  _m_keywords.insert("as",0);
  _m_keywords.insert("in",0);
  _m_keywords.insert("with",0);
  _m_keywords.insert("on",0);
  _m_keywords.insert("try",0);
  _m_keywords.insert("except",0);
  _m_keywords.insert("finally",0);
  _m_keywords.insert("while",0);
  _m_keywords.insert("is",0);
  _m_keywords.insert("implementation",0);
  _m_keywords.insert("unit",0);
  _m_keywords.insert("object",0);
  _m_keywords.insert("interface",0);
  _m_keywords.insert("default",0);
  _m_keywords.insert("stored",0);
  _m_keywords.insert("nodefault",0);
  _m_keywords.insert("implements",0);
  _m_keywords.insert("property",0);
  _m_keywords.insert("protected",0);
  _m_keywords.insert("public",0);
  _m_keywords.insert("private",0);
  _m_keywords.insert("published",0);
  _m_keywords.insert("class",0);
  _m_keywords.insert("goto",0);
  _m_keywords.insert("inherited",0);
  _m_keywords.insert("set",0);
  _m_keywords.insert("packed",0);
  _m_keywords.insert("file",0);
  _m_keywords.insert("out",0);
  _m_keywords.insert("forward",0);
  _m_keywords.insert("const",0);
  _m_keywords.insert("type",0);
  _m_keywords.insert("var",0);
  _m_keywords.insert("array",0);
  _m_keywords.insert("of",0);
  _m_keywords.insert("record",0);
  _m_keywords.insert("end",0);
  _m_keywords.insert("repeat",0);
  _m_keywords.insert("until",0);
  _m_keywords.insert("begin",0);
  _m_keywords.insert("if",0);
  _m_keywords.insert("then",0);
  _m_keywords.insert("else",0);
  _m_keywords.insert("for",0);
  _m_keywords.insert("case",0);
  _m_keywords.insert("do",0);
  _m_keywords.insert("to",0);
  _m_keywords.insert("downto",0);
  _m_keywords.insert("shl",0);
  _m_keywords.insert("shr",0);
  _m_keywords.insert("at",0);
  _m_keywords.insert("div",0);
  _m_keywords.insert("raise",0);
  _m_keywords.insert("mod",0);
  _m_keywords.insert("and",0);
  _m_keywords.insert("or",0);
  _m_keywords.insert("xor",0);
  _m_keywords.insert("program",0);
  _m_keywords.insert("function",0);
  _m_keywords.insert("procedure",0);
  _m_keywords.insert("uses",0);
  _m_keywords.insert("cdecl",0);
  _m_keywords.insert("register",0);
  _m_keywords.insert("dynamic",0);
  _m_keywords.insert("virtual",0);
  _m_keywords.insert("export",0);
  _m_keywords.insert("external",0);
  _m_keywords.insert("far",0);
  _m_keywords.insert("override",0);
  _m_keywords.insert("overload",0);
  _m_keywords.insert("pascal",0);
  _m_keywords.insert("message",0);
  _m_keywords.insert("reintroduce",0);
  _m_keywords.insert("safecall",0);    
  _m_keywords.insert("stdcall",0);     
  _m_keywords.insert("true",0);        
  _m_keywords.insert("false",0);       
  _m_keywords.insert("nil",0);
  _m_keywords.insert("not",0);
  _m_keywords.insert("constructor",0);
  _m_keywords.insert("destructor",0);
  _m_keywords.insert("message",0);
  _m_keywords.insert("string",0);

  return true;
}

void CDumpPascalInfo::CheckPascalKeyword( CComBSTR &var )
{
  USES_CONVERSION;
  char *p = W2A(var.m_str);

  str_to_lower(p);

  tlist<int> l;

  if(_m_keywords.find(p,l))
    var.Append(L"_");
}

bool CanDisplayItem(CTypeInfo *pInfo)
{
  USES_CONVERSION;
  if(pInfo->_m_typeAttr->guid==IID_IUnknown)
    return false;
  if(pInfo->_m_typeAttr->guid==IID_IDispatch)
    return false;
  if(strcmpi(W2A(pInfo->TypeName.m_str),"idispatch")==0)
    return false;
  if(strcmpi(W2A(pInfo->TypeName.m_str),"tagSafeArrayBound")==0)
    return false;
  
  return true;
}

CTypeInfo::CTypeInfo(CDumpPascalInfo *pParent, UINT index, UINT posIndex, ITypeInfo *pInfo)
{
  UINT i;
  size_t j;
  ITypeInfo *pImplType=0;
  _m_typeInfo=pInfo;
  _m_RealTypeInfo=0;
  _m_index=index;
  _m_parent=pParent;
  HREFTYPE href;
  MEMBERID mid;
  FUNCDESC *pFDesc;
  CComBSTR currName;
  UINT count;
  TYPEATTR *pattr;
  thash<size_t> property_names;
  tlist<size_t> pl;

  _m_pindex = posIndex;
  _m_parent->_m_typeAddedArr[_m_index] = -2; // just so we don't recursively try to re-add this type...
  CHECK_HRESULT(_m_typeInfo->GetTypeAttr(&_m_typeAttr));
  CHECK_HRESULT(_m_typeInfo->GetDocumentation(-1,&TypeName,&Description,0,0));
  _m_parent->CheckPascalKeyword(TypeName);

  _m_fnindex=0;

  if(_m_typeAttr->cImplTypes)
  {
    if(_m_typeAttr->wTypeFlags & TYPEFLAG_FDUAL)
    {
      ITypeInfo *pImplType2;
      
      CHECK_HRESULT(_m_typeInfo->GetRefTypeOfImplType(-1,&href));
      CHECK_HRESULT(_m_typeInfo->GetRefTypeInfo(href,&pImplType));
      CHECK_HRESULT(pImplType->GetRefTypeOfImplType(0,&href));
      CHECK_HRESULT(pImplType->GetRefTypeInfo(href,&pImplType2));
      _m_RealTypeInfo=pImplType;
      _m_RealTypeInfo->AddRef();
      RELEASE_INTERFACE(pImplType);
      pImplType=pImplType2;

      CHECK_HRESULT(_m_RealTypeInfo->GetTypeAttr(&pattr));
      _m_fnindex = _m_typeAttr->cFuncs - pattr->cFuncs;
      _m_RealTypeInfo->ReleaseTypeAttr(pattr);
    }
    else
    {
      bool found=false;
     
      CHECK_HRESULT(_m_typeInfo->GetRefTypeOfImplType(0,&href));
      CHECK_HRESULT(_m_typeInfo->GetRefTypeInfo(href,&pImplType));
    
      pImplType->GetTypeAttr(&pattr);

      if(pattr->cFuncs)
      {
        CHECK_HRESULT(pImplType->GetFuncDesc(pattr->cFuncs-1,&pFDesc));

        mid = pFDesc->memid;

        pImplType->ReleaseFuncDesc(pFDesc);
        pImplType->ReleaseTypeAttr(pattr);
    
        for(_m_fnindex=_m_typeAttr->cFuncs;_m_fnindex>0;_m_fnindex--)
        {
          CHECK_HRESULT(_m_typeInfo->GetFuncDesc(_m_fnindex-1,&pFDesc));
          found=(mid==pFDesc->memid);
          _m_typeInfo->ReleaseFuncDesc(pFDesc);

          if(found)
            break;
        }
      }
    }
  }

  if(pImplType)
  {
    CHECK_HRESULT(pImplType->GetDocumentation(-1,&BaseName,0,0,0));
    _m_parent->CheckPascalKeyword(BaseName);
  }
  
  RELEASE_INTERFACE(pImplType);

  // first check the implemented types...
  for(i=0;i<_m_typeAttr->cImplTypes;i++)
  {
    CHECK_HRESULT(_m_typeInfo->GetRefTypeOfImplType(i,&href));
    _m_parent->CheckRefInfo(href,this,true);
  }

  // check the functions...
  property_item item;

  for(i=_m_fnindex;i<_m_typeAttr->cFuncs;i++)
  {
    CHECK_HRESULT(_m_typeInfo->GetFuncDesc(i,&pFDesc));
    CHECK_HRESULT(_m_typeInfo->GetNames(pFDesc->memid,&currName,1,&count));
  
    _m_parent->CheckPascalKeyword(currName);

    if(pFDesc->invkind!=INVOKE_FUNC)
    {
      pl.clear();

      if(!property_names.find(currName.m_str,pl))
      {
        item.read = -1;
        item.write = -1;
        j = _m_properties.size();
        property_names.insert(currName.m_str,j);
        _m_properties.push_back(item);
      }
      else
        j = *pl.begin();
      
      switch(pFDesc->invkind)
      {
        case INVOKE_PROPERTYGET:
          _m_properties[j].read = i;
        break;
        case INVOKE_PROPERTYPUT:
        case INVOKE_PROPERTYPUTREF:
          _m_properties[j].write = i;
        break;
      }
    }

    _m_parent->CheckElement(this,&pFDesc->elemdescFunc);

    for(SHORT k=0;k<pFDesc->cParams;k++)
      _m_parent->CheckElement(this,&pFDesc->lprgelemdescParam[k]);

    _m_typeInfo->ReleaseFuncDesc(pFDesc);
  }

  VARDESC *pVDesc;

  for(i=0;i<_m_typeAttr->cVars;i++)
  {
    CHECK_HRESULT(_m_typeInfo->GetVarDesc(i,&pVDesc));
    _m_parent->CheckElement(this,&pVDesc->elemdescVar);
    _m_typeInfo->ReleaseVarDesc(pVDesc);
  }

  switch(_m_typeAttr->typekind)
  {
    case TKIND_ALIAS:
      _m_parent->_m_AliasCount++;
    break;
    case TKIND_RECORD:
      _m_parent->_m_RecordCount++;
    break;
    case TKIND_ENUM:
      _m_parent->_m_EnumCount++;
    break;
    case TKIND_INTERFACE:
    case TKIND_DISPATCH:
      _m_parent->_m_InterfaceCount++;
    break;
    case TKIND_COCLASS:
      _m_parent->_m_CoClassCount++;
    break;
  }

  _m_parent->_m_typeAddedArr[_m_index] = _m_pindex;
  _m_parent->_m_typeInfoArr[_m_pindex] = this;
}

CTypeInfo::~CTypeInfo()
{
  _m_typeInfo->ReleaseTypeAttr(_m_typeAttr);
  RELEASE_INTERFACE(_m_typeInfo);
  RELEASE_INTERFACE(_m_RealTypeInfo);
}

//  std::vector<CComBSTR> TypeLibs;
CGlobalPascalItems::CGlobalPascalItems()
{
}

CGlobalPascalItems::~CGlobalPascalItems()
{
}

bool CGlobalPascalItems::CheckTypeLib(ITypeLib *pLib)
{
  CComBSTR libName;
  size_t i;

  pLib->GetDocumentation(-1,&libName,0,0,0);

  for(i=0;i<TypeLibs.size();i++)
  {
    if(wcscmp(libName,TypeLibs[i])==0)
      return false;
  }

  TypeLibs.push_back(libName);
  return true;
}

CDumpPascalInfo::CDumpPascalInfo()
{
	InitPascalGlobals();
}

CDumpPascalInfo::~CDumpPascalInfo()
{
}

void CDumpPascalInfo::DumpTypeInfos(ITypeLib *pLib )
{
}

void CDumpPascalInfo::CheckTypeDesc( CTypeInfo *pInfo, TYPEDESC *pDesc, bool resort )
{
  switch(pDesc->vt)
  {
    case VT_PTR:
      CheckTypeDesc(pInfo,pDesc->lptdesc, false);
    break;
    case VT_USERDEFINED:
      CheckRefInfo(pDesc->hreftype, pInfo, resort );
    break;
  }
}

void CDumpPascalInfo::CheckElement( CTypeInfo *pInfo, ELEMDESC *pElem )
{
  CheckTypeDesc(pInfo,&pElem->tdesc, true);
}

void CDumpPascalInfo::CheckRefInfo( HREFTYPE href, CTypeInfo *pInfo, bool resort )
{
  ITypeInfo *pRefInfo=0;
  ITypeLib *pLib=0;
  UINT iIndex;
  UINT i;
  CComBSTR libName;
  // we are checking to see if this type references another type library,
  // and add it to the list of referenced libraries.
  CHECK_HRESULT(pInfo->_m_typeInfo->GetRefTypeInfo(href,&pRefInfo));

  if(pRefInfo)
    CHECK_HRESULT(pRefInfo->GetContainingTypeLib(&pLib,&iIndex));

  if(pLib)
  {
    pLib->GetDocumentation(-1, &libName,0,0,0);

    if((pLib==_m_pTypeLib)||(wcscmp(_m_libName,libName)==0))
    {
      // check to see if the reference type is declared later in the 
      // file than our current type. if that is the case, we 
      // have to move it up.
      if((resort) && (iIndex>pInfo->_m_index)&&(_m_typeAddedArr[iIndex]==-1) &&
         (pInfo->_m_typeAttr->typekind != TKIND_INTERFACE)&&
         (pInfo->_m_typeAttr->typekind != TKIND_DISPATCH))
      {
         TYPEATTR *pattr;
         CTypeInfo *pNewInfo;

         pRefInfo->GetTypeAttr(&pattr);

         if((pattr->typekind != TKIND_INTERFACE)&&(pattr->typekind != TKIND_DISPATCH))
         {
           // readd it here ...
           pRefInfo->AddRef();
           pNewInfo = new CTypeInfo(this,iIndex,pInfo->_m_pindex,pRefInfo);
           pInfo->_m_pindex = pNewInfo->_m_pindex + 1;
         }

         pRefInfo->ReleaseTypeAttr(pattr);
      }
      
      RELEASE_INTERFACE(pRefInfo);
      RELEASE_INTERFACE(pLib);
      return;
    }

    if(wcscmp(libName,L"stdole")==0)
    {
      RELEASE_INTERFACE(pRefInfo);
      RELEASE_INTERFACE(pLib);
      return;
    }

    //check the list of existing items...
    for(i=0;i<_m_refLibArr.size();i++)
    {
      if(pLib==_m_refLibArr[i])
        break;
    }

    if(i==_m_refLibArr.size())
    {
      pLib->AddRef();
      _m_refLibArr.push_back(pLib);
    }
  }

  RELEASE_INTERFACE(pLib);
  RELEASE_INTERFACE(pRefInfo);
}

void CDumpPascalInfo::DumpGuids()
{
  UINT i;
  // block comments...
  fprintf(_m_pf,"// *********************************************************************//\n");
  fprintf(_m_pf,"// GUIDS declared in the TypeLibrary. Following prefixes are used:        \n");
  fprintf(_m_pf,"//   Type Libraries     : LIBID_xxxx                                      \n");
  fprintf(_m_pf,"//   CoClasses          : CLASS_xxxx                                      \n");
  fprintf(_m_pf,"//   DISPInterfaces     : DIID_xxxx                                       \n");
  fprintf(_m_pf,"//   Non-DISP interfaces: IID_xxxx                                        \n");
  fprintf(_m_pf,"// *********************************************************************//\n");
  
  // dump the version information 
  fprintf(_m_pf,"const\n");
  fprintf(_m_pf,"  // TypeLibrary Major and minor versions\n");
  fwprintf(_m_pf,L"  %sMajorVersion = %i;\n", _m_libName.m_str,_m_pLibAttr->wMajorVerNum);
  fwprintf(_m_pf,L"  %sMinorVersion = %i;\n\n", _m_libName.m_str,_m_pLibAttr->wMinorVerNum);

  // dump the library guid
  fwprintf(_m_pf,L"  LIBID_%s : TGUID = '%s';\n\n", _m_libName.m_str, 
           CComBSTRFromIID(_m_pLibAttr->guid).m_str);

  // dump the guids of CoClasses and interfaces
  for(i=0;i<_m_typeInfoArr.size();i++)
  {
    switch(_m_typeInfoArr[i]->_m_typeAttr->typekind)
    {
      case TKIND_DISPATCH:
        if(!CanDisplayItem(_m_typeInfoArr[i]))
          continue;
        // may be dual...
        if(TYPEFLAG_FDUAL & _m_typeInfoArr[i]->_m_typeAttr->wTypeFlags)
          fwprintf(_m_pf,L"  IID_%s : TGUID = '%s';\n", _m_typeInfoArr[i]->TypeName.m_str,
                   CComBSTRFromIID(_m_typeInfoArr[i]->_m_typeAttr->guid));
        else
          fwprintf(_m_pf,L"  DIID_%s : TGUID = '%s';\n", _m_typeInfoArr[i]->TypeName.m_str,
                   CComBSTRFromIID(_m_typeInfoArr[i]->_m_typeAttr->guid));
      break;
      case TKIND_INTERFACE:
        if(!CanDisplayItem(_m_typeInfoArr[i]))
          continue;
        fwprintf(_m_pf,L"  IID_%s : TGUID = '%s';\n", _m_typeInfoArr[i]->TypeName.m_str,
                   CComBSTRFromIID(_m_typeInfoArr[i]->_m_typeAttr->guid));
      break;
      case TKIND_COCLASS:
        fwprintf(_m_pf,L"  CLASS_%s : TGUID = '%s';\n", _m_typeInfoArr[i]->TypeName.m_str,
                   CComBSTRFromIID(_m_typeInfoArr[i]->_m_typeAttr->guid));
      break;
    }
  }

  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::TranslateType(VARTYPE vt, CComBSTR &val,int ptrCnt)
{
  val = L"";

  for(int i=0;i<ptrCnt;i++)
    val.Append(L"^");

  switch(vt)
  {
    case VT_BSTR    : val.Append(L"WideString"); break;
    case VT_VARIANT : val.Append(L"OleVariant"); break;
    case VT_BOOL : val.Append(L"WordBool"); break;
    case VT_DISPATCH : val.Append(L"IDispatch"); break;
    case VT_UNKNOWN : val.Append(L"IUnknown"); break;
    case VT_ERROR : val.Append(L"HResult");break;
    case VT_I2 : val.Append(L"shortint"); break;
    case VT_I4 : val.Append(L"Integer"); break;
    case VT_R4 : val.Append(L"real"); break;
    case VT_R8 : val.Append(L"extended"); break;
    case VT_CY : val.Append(L"Currency"); break;
    case VT_DATE : val.Append(L"TDateTime"); break;
    case VT_DECIMAL : val.Append(L""); break;
    case VT_I1 : val.Append(L"smallint"); break;
    case VT_UI1 : val.Append(L"byte"); break;
    case VT_UI2 : val.Append(L"Word"); break;
    case VT_UI4 : val.Append(L"LongWord"); break;
    case VT_I8 : val.Append(L"Int64"); break;
    case VT_UI8 : val.Append(L"Int64"); break;
    case VT_INT : val.Append(L"Integer"); break;
    case VT_UINT : val.Append(L"SYSUINT"); break;
    case VT_HRESULT : val.Append(L"HResult"); break;
    case VT_CARRAY : val.Append(L"<unknown5>"); break;
    case VT_LPSTR : val.Append(L"PChar"); break;
    case VT_LPWSTR : val.Append(L"PWideChar"); break;
    case VT_FILETIME : val.Append(L"FILETIME"); break;
    case VT_BLOB : val.Append(L"<unknown2>"); break;
    case VT_STREAM : val.Append(L"IStream"); break;
    case VT_STORAGE : val.Append(L"IStorage"); break;
    case VT_STREAMED_OBJECT : val.Append(L"IStream"); break;
    case VT_STORED_OBJECT : val.Append(L"IStorage"); break;
    case VT_VOID: val.Append(L"pointer");break;
    case VT_BLOB_OBJECT : val.Append(L"<unknown3>"); break;
    case VT_CF : val.Append(L"<unknown4>"); break;
    case VT_CLSID: val.Append(L"TGUID"); break;
    case VT_SAFEARRAY :val.Append(L"PSafeArray");break;
    default:
      char a[20];
      sprintf(a,"0x%04X",vt);
      val.Append(L"<unknown : ");
      val.Append(a);
      val.Append(">");
  }
}

void CDumpPascalInfo::GetElementName(CTypeInfo *pInfo, TYPEDESC *pDesc, CComBSTR &val, bool addPtr, int ptrCnt)
{
  ITypeInfo *pRefType;
  CComBSTR arr;
  CComBSTR name;
  
  switch(pDesc->vt)
  {
    case VT_PTR:
      // we can ignore this for now...
      GetElementName(pInfo,pDesc->lptdesc,val,addPtr,(addPtr)?ptrCnt+1:0);
    break;
    case VT_CARRAY:
      SAFEARRAYBOUND *pBounds;
      char num[20];
      int i;
      
      GetElementName(pInfo,&pDesc->lpadesc->tdescElem,name);

      arr = "array [";
      
      for(i=0;i<pDesc->lpadesc->cDims;i++)
      {
        pBounds=&pDesc->lpadesc->rgbounds[i];
        if(i>0)arr.Append(',');
        itoa(pBounds->lLbound,num,10);
        arr.Append(num);
        arr.Append("..");
        itoa(pBounds->cElements-1+pBounds->lLbound,num,10);
        arr.Append(num);
      }

      arr.Append("] of ");
      arr.Append(name);

      val = arr;
    break;
    case VT_USERDEFINED:
      TYPEATTR *pattr;

      CHECK_HRESULT((pInfo->_m_typeInfo->GetRefTypeInfo(pDesc->hreftype,&pRefType)));
      
      // if it's a disp interface, we have to add Disp at the end...
      CHECK_HRESULT(pRefType->GetDocumentation(-1,&val,0,0,0));
      CheckPascalKeyword(val);
      
      if(ptrCnt)
      {
        CHECK_HRESULT(pRefType->GetTypeAttr(&pattr));
        if((pattr->typekind==TKIND_INTERFACE)||(pattr->typekind==TKIND_DISPATCH))
          ptrCnt--;
        for(int i=0;i<ptrCnt;i++)
          arr.Append(L"^");
        arr.Append(val);
        val = arr;
        pRefType->ReleaseTypeAttr(pattr);
      }
      RELEASE_INTERFACE(pRefType);
    break;
    default:
      TranslateType(pDesc->vt,val,ptrCnt);
  }
}

void CDumpPascalInfo::GetResultName(CTypeInfo *pInfo, FUNCDESC *pFDesc, CComBSTR &val)
{
  ELEMDESC *pDesc;

  switch(pFDesc->invkind)
  {
    case INVOKE_FUNC:
    case INVOKE_PROPERTYGET:
      pDesc = &pFDesc->elemdescFunc;
    break;
    case INVOKE_PROPERTYPUT:
    case INVOKE_PROPERTYPUTREF:
      pDesc = &pFDesc->lprgelemdescParam[pFDesc->cParams-1];
    break;
  }

  GetElementName(pInfo,&pDesc->tdesc,val,true);
}

void CDumpPascalInfo::DumpProperty(UINT index, CTypeInfo *pInfo)
{
  CTypeInfo::property_item item;
  UINT fn_indx;

  item = pInfo->_m_properties[index];

  if(item.read == -1){
    // writeonly
    fn_indx = item.write;
  }
  else
  if(item.write == -1){
    // readonly
    fn_indx = item.read;
  }
  else{
    // read / write
    fn_indx = min(item.write,item.read);
  }

  FUNCDESC *pDesc;
  CComBSTR name;
  UINT count;
  CComBSTR propType;
  int params;

  CHECK_HRESULT(pInfo->_m_typeInfo->GetFuncDesc(fn_indx,&pDesc));
  CHECK_HRESULT(pInfo->_m_typeInfo->GetNames(pDesc->memid,&name,1,&count));
  CheckPascalKeyword(name);
  
  GetResultName(pInfo,pDesc,propType);

  switch(pDesc->invkind)
  {
    case INVOKE_PROPERTYGET:
      if(pInfo->_m_RealTypeInfo)
        params = pDesc->cParams;
      else
        params=(pDesc->cParams)?pDesc->cParams-1:0;
    break;
    case INVOKE_PROPERTYPUT:
    case INVOKE_PROPERTYPUTREF:
      params = (pDesc->cParams)?pDesc->cParams-1:0;
    break;
  }

  if(params)
  {
    fwprintf(_m_pf,L"    property %s", name.m_str );
    DumpPropertyParams(fn_indx,pDesc,pInfo);
    fwprintf(_m_pf,L" : %s ", propType.m_str);
  }
  else
    fwprintf(_m_pf,L"    property %s : %s ", name.m_str, propType.m_str);

  if(item.read != -1 )
    fwprintf(_m_pf,L"read Get_%s", name.m_str);

  if(item.write != -1 )
    fwprintf(_m_pf,L" write Set_%s", name.m_str);

  if((params)&&(pDesc->memid == 0))
    fprintf(_m_pf,"; default");

  fprintf(_m_pf,";\n");

  pInfo->_m_typeInfo->ReleaseFuncDesc(pDesc);
}

void CDumpPascalInfo::DumpDispProperty(UINT index, CTypeInfo *pInfo)
{
  CTypeInfo::property_item item;
  std::string options;
  UINT fn_indx;
  UINT prev_indx;

  if(index==0)
    prev_indx = pInfo->_m_fnindex;
  else
  {
    item = pInfo->_m_properties[index-1];

    if(item.read == -1)
      prev_indx = item.write;
    else
    if(item.write == -1)
      prev_indx = item.read;
    else
      prev_indx = max(item.write,item.read);
    prev_indx++;
  }

  item = pInfo->_m_properties[index];

  if(item.read == -1){
    // writeonly
    options = "writeonly ";
    fn_indx = item.write;
  }
  else
  if(item.write == -1){
    // readonly
    options = "readonly ";
    fn_indx = item.read;
  }
  else{
    // read / write
    options = "";
    fn_indx = min(item.write,item.read);
  }

  // we need to output the functions in between the properties...
  for(;prev_indx<fn_indx;prev_indx++)
    DumpProcedure(prev_indx,pInfo,true);

  FUNCDESC *pDesc;
  CComBSTR name;
  UINT count;
  CComBSTR propType;
  int params;

  CHECK_HRESULT(pInfo->_m_typeInfo->GetFuncDesc(fn_indx,&pDesc));
  CHECK_HRESULT(pInfo->_m_typeInfo->GetNames(pDesc->memid,&name,1,&count));
  CheckPascalKeyword(name);

  GetResultName(pInfo,pDesc,propType);

  switch(pDesc->invkind)
  {
    case INVOKE_PROPERTYGET:
      if(pInfo->_m_RealTypeInfo)
        params = pDesc->cParams;
      else
        params=(pDesc->cParams)?pDesc->cParams-1:0;
    break;
    case INVOKE_PROPERTYPUT:
    case INVOKE_PROPERTYPUTREF:
      params = (pDesc->cParams)?pDesc->cParams-1:0;
    break;
  }

  if(params)
  {
    fwprintf(_m_pf,L"    property %s", name.m_str );
    DumpPropertyParams(fn_indx,pDesc,pInfo);
    fwprintf(_m_pf,L" : %s ", propType.m_str);
  }
  else
    fwprintf(_m_pf,L"    property %s : %s ", name.m_str, propType.m_str);

  fprintf(_m_pf,"%s", options.c_str());
  fprintf(_m_pf,"dispid %i",pDesc->memid);
  if((params)&&(pDesc->memid == 0))
    fprintf(_m_pf,"; default");
  fprintf(_m_pf,";\n");

  pInfo->_m_typeInfo->ReleaseFuncDesc(pDesc);

  if(pInfo->_m_properties.size()-1 == index)
  {
    fn_indx++;
    for(;fn_indx<pInfo->_m_typeAttr->cFuncs;fn_indx++)
      DumpProcedure(fn_indx,pInfo,true);
  }
}

void CDumpPascalInfo::DumpPropertyParams( UINT index, FUNCDESC *pDesc, CTypeInfo *pInfo )
{
  UINT params;
  UINT i,c;
  CComBSTR typeName;
  CComBSTR paramName;

  fprintf(_m_pf,"[");

  switch(pDesc->invkind)
  {
    case INVOKE_PROPERTYGET:
      if(pInfo->_m_RealTypeInfo)
        params = pDesc->cParams;
      else
        params=(pDesc->cParams)?pDesc->cParams-1:0;
    break;
    case INVOKE_PROPERTYPUT:
    case INVOKE_PROPERTYPUTREF:
      params = (pDesc->cParams)?pDesc->cParams-1:0;
    break;
  }

  BSTR *names = new BSTR[1+pDesc->cParams];
  
  if(pInfo->_m_RealTypeInfo && (pDesc->invkind != INVOKE_PROPERTYGET))
  {
    FUNCDESC *pDesc2;
    CHECK_HRESULT(pInfo->_m_RealTypeInfo->GetFuncDesc(index-pInfo->_m_fnindex,&pDesc2));
    CHECK_HRESULT(pInfo->_m_RealTypeInfo->GetNames(pDesc2->memid,names,1+pDesc->cParams,&c));
    pInfo->_m_RealTypeInfo->ReleaseFuncDesc(pDesc2);
  }
  else
    CHECK_HRESULT(pInfo->_m_typeInfo->GetNames(pDesc->memid,names,1+pDesc->cParams,&c));

  for(i=0;i<params;i++)
  {
     if(i!=0)
       fprintf(_m_pf,"; ");
      if(pDesc->lprgelemdescParam[i].paramdesc.wParamFlags & PARAMFLAG_FOUT )
      {
        if(pDesc->funckind == FUNC_DISPATCH)
          fprintf(_m_pf,"var ");
        else
          fprintf(_m_pf,"out ");
      }
      else
      {
        if(pDesc->lprgelemdescParam[i].tdesc.vt == VT_PTR)
        {
          TYPEDESC *ptDesc=pDesc->lprgelemdescParam[i].tdesc.lptdesc;
          ITypeInfo *pRefType;
          TYPEATTR *tRefAttr;

          // if it's not an interface, we should declare it as var...
          switch(ptDesc->vt)
          {
            case VT_USERDEFINED:
              if(SUCCEEDED(pInfo->_m_typeInfo->GetRefTypeInfo(ptDesc->hreftype,&pRefType)))
              {
                CHECK_HRESULT(pRefType->GetTypeAttr(&tRefAttr));
                switch(tRefAttr->typekind)
                {
                  case TKIND_DISPATCH:
                  case TKIND_INTERFACE:
                  break;
                  default:
                    fprintf(_m_pf," var");
                }
                pRefType->ReleaseTypeAttr(tRefAttr);
                RELEASE_INTERFACE(pRefType);
              }
            break;
            case VT_UNKNOWN:
            case VT_DISPATCH:
            case VT_STREAM:
            case VT_STORAGE:
            case VT_VOID:
            break;
            default:
              fprintf(_m_pf,"var ");
          }
        }
      }

      GetElementName(pInfo,&pDesc->lprgelemdescParam[i].tdesc,typeName);
      paramName=names[i+1];
      CheckPascalKeyword(paramName);
      fwprintf(_m_pf,L"%s : %s", paramName.m_str,typeName.m_str);
  }

  fprintf(_m_pf,"]");

  delete [] names;
}

void CDumpPascalInfo::DumpProcedure(UINT index, CTypeInfo *pInfo, bool dispid)
{
  FUNCDESC *pDesc;
  BSTR *names;
  UINT c,i;
  bool function;
  CComBSTR typeName;
  CComBSTR paramName;
  CComBSTR propName;
  
  CHECK_HRESULT(pInfo->_m_typeInfo->GetFuncDesc(index,&pDesc));
  
  names = new BSTR[1+pDesc->cParams];
  
  if(pInfo->_m_RealTypeInfo && (pDesc->invkind != INVOKE_PROPERTYGET))
  {
    FUNCDESC *pDesc2;
    TYPEATTR *pAttr;
    CComBSTR  libPath;

    QueryPathOfRegTypeLib(_m_pLibAttr->guid,_m_pLibAttr->wMajorVerNum,_m_pLibAttr->wMinorVerNum, _m_pLibAttr->lcid,&libPath);

    CHECK_HRESULT(pInfo->_m_RealTypeInfo->GetTypeAttr(&pAttr));
    CHECK_HRESULT(pInfo->_m_RealTypeInfo->GetFuncDesc(index-pInfo->_m_fnindex,&pDesc2));
    CHECK_HRESULT(pInfo->_m_RealTypeInfo->GetNames(pDesc2->memid,names,1+pDesc->cParams,&c));
    pInfo->_m_RealTypeInfo->ReleaseFuncDesc(pDesc2);
    pInfo->_m_RealTypeInfo->ReleaseTypeAttr(pAttr);
  }
  else
    CHECK_HRESULT(pInfo->_m_typeInfo->GetNames(pDesc->memid,names,1+pDesc->cParams,&c));
  
  fprintf(_m_pf,"    ");

  propName=names[0];
  CheckPascalKeyword(propName);

  switch(pDesc->invkind)
  {
    case INVOKE_PROPERTYGET:    
      fwprintf(_m_pf,L"function Get_%s",propName.m_str);
      function = true;
    break;
    case INVOKE_PROPERTYPUT:    
    case INVOKE_PROPERTYPUTREF:
      fwprintf(_m_pf,L"procedure Set_%s",propName.m_str);
      function = false;
    break;
    default:
      switch(pDesc->elemdescFunc.tdesc.vt)
      {
        case VT_EMPTY:
        case VT_NULL:
        case VT_VOID:
          fwprintf(_m_pf,L"procedure "); 
          function = false;
        break;
        default:
          function = true;
          fwprintf(_m_pf,L"function "); 
      }

      fwprintf(_m_pf,L"%s",propName.m_str);
  }

  if(pDesc->cParams>0)
  {
    fprintf(_m_pf,"(");

    for(SHORT i=0;i<pDesc->cParams;i++)
    {
      if(i!=0)
        fprintf(_m_pf,";");
      if(pDesc->lprgelemdescParam[i].paramdesc.wParamFlags & PARAMFLAG_FOUT )
      {
        if(pDesc->funckind == FUNC_DISPATCH)
          fprintf(_m_pf," var");
        else
          fprintf(_m_pf," out");
      }
      else
      {
        if(pDesc->lprgelemdescParam[i].tdesc.vt == VT_PTR)
        {
          TYPEDESC *ptDesc=pDesc->lprgelemdescParam[i].tdesc.lptdesc;
          ITypeInfo *pRefType;
          TYPEATTR *tRefAttr;

          // if it's not an interface, we should declare it as var...
          switch(ptDesc->vt)
          {
            case VT_USERDEFINED:
              if(SUCCEEDED(pInfo->_m_typeInfo->GetRefTypeInfo(ptDesc->hreftype,&pRefType)))
              {
                pRefType->GetTypeAttr(&tRefAttr);
                switch(tRefAttr->typekind)
                {
                  case TKIND_DISPATCH:
                  case TKIND_INTERFACE:
                  break;
                  default:
                    fprintf(_m_pf," var");
                }
                pRefType->ReleaseTypeAttr(tRefAttr);
                RELEASE_INTERFACE(pRefType);
              }
            break;
            case VT_UNKNOWN:
            case VT_DISPATCH:
            case VT_STREAM:
            case VT_STORAGE:
            case VT_VOID:
            break;
            default:
              fprintf(_m_pf," var");
          }
        }
      }

      GetElementName(pInfo,&pDesc->lprgelemdescParam[i].tdesc,typeName);

      if(((UINT)(i+1)<c)&&names[i+1])
        paramName=names[i+1];
      else
      {
        char num[20];

        itoa(i+1,num,10);
        paramName = "param";
        paramName.Append(num);
      }

      CheckPascalKeyword(paramName);
      fwprintf(_m_pf,L" %s : %s",paramName.m_str,typeName.m_str);
    }

    fprintf(_m_pf,")");
  }

  // output the return type...
  if(function)
  {
    CComBSTR retType;
    GetResultName(pInfo,pDesc,retType);

    fwprintf(_m_pf,L" : %s",retType.m_str);
  }

  if(dispid)
    fprintf(_m_pf,"; dispid %i",pDesc->memid);
  else
  {
    if(pDesc->funckind == FUNC_DISPATCH)
      fprintf(_m_pf,"; safecall");
    else
      fprintf(_m_pf,"; stdcall");
  }

  for(i=0;i<c;i++)
    SysFreeString(names[i]);

  delete [] names;

  pInfo->_m_typeInfo->ReleaseFuncDesc(pDesc);

  fprintf(_m_pf,";\n");
}

void CDumpPascalInfo::DumpInterfaceDecl(CTypeInfo *pInfo)
{
  CComBSTR docInfo;
  UINT i;

  if(!CanDisplayItem(pInfo))
    return;
  
  CHECK_HRESULT(pInfo->_m_typeInfo->GetDocumentation(-1,0,&docInfo,0,0));

  fprintf(_m_pf,   "// *********************************************************************//\n");
  fwprintf(_m_pf, L"// Interface:   %s\n",pInfo->TypeName.m_str);
  fprintf(_m_pf,   "// Flags:       \n");
  fwprintf(_m_pf,  L"// GUID:       %s\n",CComBSTRFromIID(pInfo->_m_typeAttr->guid));
  if(docInfo.Length()!=0)
  fwprintf(_m_pf,  L"// Helpstring: %s\n",docInfo.m_str);
  fprintf(_m_pf,   "// *********************************************************************//\n");

  if(pInfo->BaseName.Length())
    fwprintf(_m_pf,L"  %s = interface(%s)\n", pInfo->TypeName.m_str,pInfo->BaseName.m_str);
  else
    fwprintf(_m_pf,L"  %s = interface\n", pInfo->TypeName.m_str);

  fwprintf(_m_pf,L"    ['%s']\n",CComBSTRFromIID(pInfo->_m_typeAttr->guid));
  // print all the procedures...
  for(i=pInfo->_m_fnindex;i<pInfo->_m_typeAttr->cFuncs;i++)
    DumpProcedure(i,pInfo,false);
  for(i=0;i<pInfo->_m_properties.size();i++)
    DumpProperty(i,pInfo);

  fwprintf(_m_pf,L"  end;\n");
  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::DumpCoClassInterfaces(CTypeInfo*pInfo)
{
  CComBSTR val;
  UINT i;
  int flags;
  for(i=0;i<pInfo->_m_typeAttr->cImplTypes;i++)
  {
    pInfo->_m_typeInfo->GetImplTypeFlags(i,&flags);
    
    if(flags & IMPLTYPEFLAG_FDEFAULT)
      fprintf(_m_pf,"// [default] ");
    else
      fprintf(_m_pf,"//           ");

    HREFTYPE href;
    ITypeInfo *pRefInfo;
    CHECK_HRESULT(pInfo->_m_typeInfo->GetRefTypeOfImplType(i,&href));
    CHECK_HRESULT(pInfo->_m_typeInfo->GetRefTypeInfo(href,&pRefInfo));
    CHECK_HRESULT(pRefInfo->GetDocumentation(-1,&val,0,0,0));
    CheckPascalKeyword(val);
    RELEASE_INTERFACE(pRefInfo);
    fwprintf(_m_pf,L"%s\n",val.m_str);
  }
}

void CDumpPascalInfo::GetDefaultInterface( CTypeInfo *pInfo, CComBSTR &val)
{
  UINT i;
  int flags;
  for(i=0;i<pInfo->_m_typeAttr->cImplTypes;i++)
  {
    pInfo->_m_typeInfo->GetImplTypeFlags(i,&flags);
    
    if(flags & IMPLTYPEFLAG_FDEFAULT)
      break;
  }

  if(i!=pInfo->_m_typeAttr->cImplTypes)
  {
    HREFTYPE href;
    ITypeInfo *pRefInfo;
    TYPEATTR *pattr;
    CHECK_HRESULT(pInfo->_m_typeInfo->GetRefTypeOfImplType(i,&href));
    CHECK_HRESULT(pInfo->_m_typeInfo->GetRefTypeInfo(href,&pRefInfo));
    pRefInfo->GetTypeAttr(&pattr);
    
    if(pattr->typekind == TKIND_DISPATCH)
    {
      if(pattr->wTypeFlags & TYPEFLAG_FDUAL)
      {
        ITypeInfo *pRealRefInfo;

        CHECK_HRESULT(pRefInfo->GetRefTypeOfImplType(-1,&href));
        CHECK_HRESULT(pRefInfo->GetRefTypeInfo(href,&pRealRefInfo));
        pRealRefInfo->GetDocumentation(-1,&val,0,0,0);
        RELEASE_INTERFACE(pRealRefInfo);      
      }
      else
      {
        pRefInfo->GetDocumentation(-1,&val,0,0,0);
        val.Append("Disp");
      }
    }
    else
      pRefInfo->GetDocumentation(-1,&val,0,0,0);

    pRefInfo->ReleaseTypeAttr(pattr);
    CheckPascalKeyword(val);
    RELEASE_INTERFACE(pRefInfo);
  }
  else
    val = L"IUnknown";
}

void CDumpPascalInfo::DumpCoClassFDecl()
{
  if(_m_CoClassCount==0)
    return;

  fprintf(_m_pf,"// *********************************************************************//\n");
  fprintf(_m_pf,"// Declaration of CoClasses defined in Type Library                       \n");
  fprintf(_m_pf,"// (NOTE: Here we map each CoClass to its Default Interface)              \n");
  fprintf(_m_pf,"// *********************************************************************//\n");
  fprintf(_m_pf,"type\n\n");


  UINT i;
  CTypeInfo *pInfo;
  CComBSTR typeName;
  CComBSTR defaultInt;

  for(i=0;i<_m_typeInfoArr.size();i++)
  {
    pInfo=_m_typeInfoArr[i];

    switch(pInfo->_m_typeAttr->typekind)
    {
      case TKIND_COCLASS:
        pInfo->_m_typeInfo->GetDocumentation(-1,&typeName,0,0,0);
        CheckPascalKeyword(typeName);
        GetDefaultInterface(pInfo,defaultInt);
        fwprintf(_m_pf,L"  %s = %s;\n",typeName.m_str,defaultInt.m_str);
      break;
    }
  }

  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::DumpCoClassDecl()
{
  UINT i;
  CTypeInfo *pInfo;
  CComBSTR typeName;
  CComBSTR defaultInt;
  CComBSTR docInfo;

  if(_m_CoClassCount==0)
    return;

  for(i=0;i<_m_typeInfoArr.size();i++)
  {
    pInfo=_m_typeInfoArr[i];

    switch(pInfo->_m_typeAttr->typekind)
    {
      case TKIND_COCLASS:
        pInfo->_m_typeInfo->GetDocumentation(-1,&typeName,&docInfo,0,0);
        CheckPascalKeyword(typeName);

        fprintf(_m_pf,"// *********************************************************************//\n");
        if(docInfo.Length())
          fwprintf(_m_pf,L"// %s\n",docInfo.m_str);
        fprintf(_m_pf,"// Implemented Interfaces:\n");
        DumpCoClassInterfaces(pInfo);
        fprintf(_m_pf,"// *********************************************************************//\n");

        GetDefaultInterface(pInfo,defaultInt);

        fwprintf(_m_pf,L"  Co%s = class\n",typeName.m_str);
        fwprintf(_m_pf,L"    class function Create : %s;\n",defaultInt.m_str);
        fwprintf(_m_pf,L"    class function CreateRemote( const MachineName : string ) : %s;\n",defaultInt.m_str);
        fprintf(_m_pf,"  end;\n\n");
      break;
    }
  }

  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::DumpCoClassImpl()
{
  UINT i;
  CTypeInfo *pInfo;
  CComBSTR typeName;
  CComBSTR defaultInt;

  if(_m_CoClassCount==0)
    return;

  for(i=0;i<_m_typeInfoArr.size();i++)
  {
    pInfo=_m_typeInfoArr[i];

    switch(pInfo->_m_typeAttr->typekind)
    {
      case TKIND_COCLASS:
        pInfo->_m_typeInfo->GetDocumentation(-1,&typeName,0,0,0);
        CheckPascalKeyword(typeName);
        GetDefaultInterface(pInfo,defaultInt);

        fwprintf(_m_pf,L"class function Co%s.Create: %s;\n",typeName.m_str,defaultInt.m_str);
        fwprintf(_m_pf,L"begin\n");
        fwprintf(_m_pf,L"  Result := CreateComObject(CLASS_%s) as %s;\n",typeName.m_str,defaultInt.m_str);
        fwprintf(_m_pf,L"end;\n");
        fwprintf(_m_pf,L"\n");
        fwprintf(_m_pf,L"class function Co%s.CreateRemote(const MachineName: string): %s;\n",typeName.m_str,defaultInt.m_str);
        fwprintf(_m_pf,L"begin\n");
        fwprintf(_m_pf,L"  Result := CreateRemoteComObject(MachineName, CLASS_%s) as %s;\n",typeName.m_str,defaultInt.m_str);
        fwprintf(_m_pf,L"end;\n\n");
      break;
    }
  }

  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::DumpDispInterfaceDecl(CTypeInfo *pInfo)
{
  CComBSTR docInfo;
  UINT i;

  if(!CanDisplayItem(pInfo))
    return ;
  
  pInfo->_m_typeInfo->GetDocumentation(pInfo->_m_index,0,&docInfo,0,0);

  fprintf(_m_pf,   "// *********************************************************************//\n");
  fwprintf(_m_pf, L"// Interface:   %sDisp\n",pInfo->TypeName.m_str);
  fprintf(_m_pf,   "// Flags:       \n");
  fwprintf(_m_pf,  L"// GUID:       %s\n",CComBSTRFromIID(pInfo->_m_typeAttr->guid));
  if(docInfo.Length()!=0)
  fwprintf(_m_pf,  L"// Helpstring: %s\n",docInfo.m_str);
  fprintf(_m_pf,   "// *********************************************************************//\n");

  fwprintf(_m_pf,L"  %sDisp = dispinterface\n", pInfo->TypeName.m_str);
  fwprintf(_m_pf,L"    ['%s']\n",CComBSTRFromIID(pInfo->_m_typeAttr->guid));
  
  if(pInfo->_m_properties.size()>0)
  {
    for(i=0;i<pInfo->_m_properties.size();i++)
      DumpDispProperty(i,pInfo);
  }
  else
  {
    for(i=pInfo->_m_fnindex;i<pInfo->_m_typeAttr->cFuncs;i++)
      DumpProcedure(i,pInfo,true);
  }
  fwprintf(_m_pf,L"  end;\n");
  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::DumpInterfaces()
{
  UINT i;
  
  // first dump the forward declarations...
  if(_m_InterfaceCount>0)
  {
    fprintf(_m_pf,"type\n\n");
  
    fprintf(_m_pf,"// *********************************************************************//\n");
    fprintf(_m_pf,"// Forward declaration of types defined in TypeLibrary                    \n");
    fprintf(_m_pf,"// *********************************************************************//\n");

    int j=0;

    for(i=0;i<_m_typeInfoArr.size();i++)
    {
      switch(_m_typeInfoArr[i]->_m_typeAttr->typekind)
      {
        case TKIND_DISPATCH:
          if(!CanDisplayItem(_m_typeInfoArr[i]))
            continue;
          // may be dual...
          if(TYPEFLAG_FDUAL & _m_typeInfoArr[i]->_m_typeAttr->wTypeFlags)
            fwprintf(_m_pf,L"  %s = interface;\n", _m_typeInfoArr[i]->TypeName.m_str);
          fwprintf(_m_pf,L"  %sDisp = dispinterface;\n", _m_typeInfoArr[i]->TypeName.m_str );
        break;
        case TKIND_INTERFACE:
          if(!CanDisplayItem(_m_typeInfoArr[i]))
            continue;
          fwprintf(_m_pf,L"  %s = interface;\n", _m_typeInfoArr[i]->TypeName.m_str);
        break;
      }
    }

    if(i>0)
      fprintf(_m_pf,"\n");
  }

  // dump forward declarations for the coclasses...
  DumpCoClassFDecl();

  // dump the enumerations
  DumpEnums();

  // dump all structures...
  DumpRecords();

  if(_m_InterfaceCount>0)
  {
    // dump the interfaces...
    fprintf(_m_pf,"type\n");

    for(i=0;i<_m_typeInfoArr.size();i++)
    {
      switch(_m_typeInfoArr[i]->_m_typeAttr->typekind)
      {
        case TKIND_DISPATCH:
          if(TYPEFLAG_FDUAL & _m_typeInfoArr[i]->_m_typeAttr->wTypeFlags)
            DumpInterfaceDecl(_m_typeInfoArr[i]);
          DumpDispInterfaceDecl(_m_typeInfoArr[i]);
        break;
        case TKIND_INTERFACE:
          DumpInterfaceDecl(_m_typeInfoArr[i]);
        break;
      }
    }

    if(i>0)
      fprintf(_m_pf,"\n");
  }


  // dump the coclasses...
  DumpCoClassDecl();
}

void CDumpPascalInfo::DumpEnums()
{
  UINT i,j;
  CComBSTR enum_name;
  CComBSTR docInfo;
  CComBSTR var_name;
  CComBSTR var_value;
  VARDESC *pDesc;
  CTypeInfo *pInfo;

  if(_m_EnumCount==0)
    return;

  for(i=0;i<this->_m_typeInfoArr.size();i++)
  {
    if(_m_typeInfoArr[i]->_m_typeAttr->typekind == TKIND_ENUM)
    {
      // dump the name...
      pInfo = _m_typeInfoArr[i];
      pInfo->_m_typeInfo->GetDocumentation(-1,&enum_name,&docInfo,0,0);
      CheckPascalKeyword(enum_name);
      
      if(docInfo.Length())
        fwprintf(_m_pf,L"//%s \ntype\n",docInfo.m_str);
      else
        fwprintf(_m_pf,L"// Constants for enum %s\ntype\n", enum_name.m_str);
      fwprintf(_m_pf,L"  %s = TOleEnum;\n",enum_name.m_str);
      fprintf(_m_pf,"const\n");
      for(j=0;j<pInfo->_m_typeAttr->cVars;j++)
      {
        // dump the consts...
        CHECK_HRESULT(pInfo->_m_typeInfo->GetVarDesc(j,&pDesc));
        pInfo->_m_typeInfo->GetDocumentation(pDesc->memid,&var_name,0,0,0);
        CheckPascalKeyword(var_name);
        VarFormat(pDesc->lpvarValue,L"#",0,0,0,&var_value);
        pInfo->_m_typeInfo->ReleaseVarDesc(pDesc);

        fwprintf(_m_pf,L"  %s = $%08X;\n",var_name.m_str,_wtoi(var_value.m_str));
      }

      fprintf(_m_pf,"\n");
    }
  }

  fprintf(_m_pf,"\n");
}

void CDumpPascalInfo::DumpRecords()
{
  UINT i,j;
  CTypeInfo *pInfo;
  CComBSTR typeName,baseName;
  VARDESC *pDesc;
  TYPEKIND last=TKIND_MAX;

  if(_m_RecordCount+_m_AliasCount==0)
    return;

  fprintf(_m_pf,"// *********************************************************************//\n");
  fprintf(_m_pf,"// Declaration of structures, unions and aliases.                         \n");
  fprintf(_m_pf,"// *********************************************************************//\n");

  fprintf(_m_pf,"\ntype\n");

  for(i=0;i<_m_typeInfoArr.size();i++)
  {
    pInfo=_m_typeInfoArr[i];

    if(!CanDisplayItem(pInfo))
      continue;

    switch(pInfo->_m_typeAttr->typekind)
    {
      case TKIND_RECORD:
      case TKIND_UNION:     
        if(last==TKIND_ALIAS)
          fwprintf(_m_pf,L"\n");

        pInfo->_m_typeInfo->GetDocumentation(-1,&typeName,0,0,0);
        CheckPascalKeyword(typeName);
        fwprintf(_m_pf,L"  %s = packed record\n", typeName.m_str);
        
        if(pInfo->_m_typeAttr->typekind==TKIND_UNION)
          fprintf(_m_pf,"    case Integer of \n");
        for(j=0;j<pInfo->_m_typeAttr->cVars;j++)
        {
          CHECK_HRESULT(pInfo->_m_typeInfo->GetVarDesc(j,&pDesc));
          pInfo->_m_typeInfo->GetDocumentation(pDesc->memid,&typeName,0,0,0);
          CheckPascalKeyword(typeName);
          GetElementName(pInfo,&pDesc->elemdescVar.tdesc,baseName,true);
          pInfo->_m_typeInfo->ReleaseVarDesc(pDesc);

          if(pInfo->_m_typeAttr->typekind==TKIND_UNION)
            fprintf(_m_pf,"      %i : (",j);
          else
            fprintf(_m_pf,"    ");

          fwprintf(_m_pf,L"%s : %s",typeName.m_str,baseName.m_str);

          if(pInfo->_m_typeAttr->typekind==TKIND_UNION)
            fprintf(_m_pf,");\n");
          else
            fprintf(_m_pf,";\n");
        }

        fwprintf(_m_pf,L"  end;\n\n");
        last=pInfo->_m_typeAttr->typekind;
      break;
      case TKIND_ALIAS:
        pInfo->_m_typeInfo->GetDocumentation(-1,&typeName,0,0,0);
        CheckPascalKeyword(typeName);
        GetElementName(pInfo,&pInfo->_m_typeAttr->tdescAlias,baseName,true);
        fwprintf(_m_pf,L"  %s = %s;\n",typeName.m_str,baseName.m_str);
        last=pInfo->_m_typeAttr->typekind;
      break;
    }
  }

  fwprintf(_m_pf,L"\n");
}

void CDumpPascalInfo::LoadTypeLib(ITypeLib *pLib)
{
  std::string s;
  UINT icount,i,pindex;
  CTypeInfo *pInfo;
  ITypeInfo *piInfo;
  CComBSTR tmpName;

  try
  {
    _m_pTypeLib=pLib;
    CHECK_HRESULT(_m_pTypeLib->GetLibAttr(&_m_pLibAttr));
    _m_pTypeLib->GetDocumentation(-1,&_m_libName,0,0,0);

    icount=_m_pTypeLib->GetTypeInfoCount();

    // build type info array...
    _m_typeInfoArr.resize(icount,NULL);
    _m_typeAddedArr.resize(icount,-1);

    for(i=0,pindex=0;i<icount;i++)
    {
      if(_m_typeAddedArr[i]==-1)
      {
        CHECK_HRESULT(_m_pTypeLib->GetTypeInfo(i,&piInfo));
        pInfo = new CTypeInfo(this,i,pindex,piInfo);
        pindex = pInfo->_m_pindex+1;
      }
    }
  }
  catch(CTypeLibErr &err)
  {
    err;// to avoid
  }
}

void CDumpPascalInfo::FreeTypeLib()
{
  UINT icount,i;
  
  icount=_m_pTypeLib->GetTypeInfoCount();

  try
  {
    // clear the array of type infos...
    for(i=0;i<icount;i++)
    {
      if(_m_typeInfoArr[i])
        delete _m_typeInfoArr[i];
    }

    // remove the list of contained libraries...
    for(i=0;i<_m_refLibArr.size();i++)
      RELEASE_INTERFACE(_m_refLibArr[i]);

    _m_pTypeLib->ReleaseTLibAttr(_m_pLibAttr);
  }
  catch(...)
  {
  }

  _m_typeInfoArr.clear();
  _m_refLibArr.clear();
}

CTypeInfo *CDumpPascalInfo::GetTypeInfo(UINT indx)
{
  return _m_typeInfoArr[_m_typeAddedArr[indx]];
}

void CDumpPascalInfo::SetFile(FILE *pf)
{
  _m_pf = pf;
}

void CDumpPascalInfo::DumpTypeLib(const char *pfname, ITypeLib *pLib,CGlobalPascalItems *pItems)
{
  USES_CONVERSION;
  std::string s;
  UINT icount,i,pindex;
  CTypeInfo *pInfo;
  ITypeInfo *piInfo;
  CComBSTR tmpName;
  CComBSTR libPath;

  if(!pItems->CheckTypeLib(pLib))
    return;

	pItems->output_directory = "c:\\Windows\\temp";

  s += pItems->output_directory + "\\";
  s += pfname;
  s += "_TLB.pas";

  _m_pf = fopen(s.c_str(),"wt");

	if(!_m_pf)
	{
		return;
	}

  try
  {
    // Dump the directory where the typelib resides...
    _m_pTypeLib=pLib;
    _m_RecordCount=0;
    _m_AliasCount=0;
    _m_CoClassCount=0;
    _m_InterfaceCount=0;
    _m_EnumCount=0;

    _m_typeInfoArr.clear();
    _m_refLibArr.clear();
    _m_typeAddedArr.clear();


    CHECK_HRESULT(_m_pTypeLib->GetLibAttr(&_m_pLibAttr));
    _m_pTypeLib->GetDocumentation(-1,&_m_libName,0,0,0);

    QueryPathOfRegTypeLib(_m_pLibAttr->guid,_m_pLibAttr->wMajorVerNum,_m_pLibAttr->wMinorVerNum, _m_pLibAttr->lcid,&libPath);

    fprintf(_m_pf,"// *********************************************************************//\n");
    fwprintf(_m_pf,L"// %s\n",libPath.m_str);
    fprintf(_m_pf,"// *********************************************************************//\n");

    fprintf(_m_pf,"unit %s_TLB;\n\n", pfname);
    fprintf(_m_pf,"interface\n\n");

    icount=_m_pTypeLib->GetTypeInfoCount();

    // build type info array...
    _m_typeInfoArr.resize(icount,NULL);
    _m_typeAddedArr.resize(icount,-1);

    for(i=0,pindex=0;i<icount;i++)
    {
      if(_m_typeAddedArr[i]==-1)
      {
        CHECK_HRESULT(_m_pTypeLib->GetTypeInfo(i,&piInfo));
        pInfo = new CTypeInfo(this,i,pindex,piInfo);
        pindex = pInfo->_m_pindex+1;
      }
    }    

    // output the uses clause...
    if(_m_refLibArr.size()>0)
    {
      CDumpPascalInfo *tmpDump; 

      fprintf(_m_pf,"uses ");
      for(i=0;i<_m_refLibArr.size()-1;i++)
      {
        
        _m_refLibArr[i]->GetDocumentation(-1,&tmpName,0,0,0);

        fprintf(_m_pf,"%s_tlb,",W2A(tmpName.m_str));

        if(pItems->CheckTypeLib(_m_refLibArr[i]))
        {
          tmpDump = new CDumpPascalInfo;
          tmpDump->DumpTypeLib(W2A(tmpName.m_str),_m_refLibArr[i],pItems);
          delete tmpDump;
        }
      }

      _m_refLibArr[i]->GetDocumentation(-1,&tmpName,0,0,0);
      fprintf(_m_pf,"%s_tlb",W2A(tmpName.m_str));
    
      if(pItems->CheckTypeLib(_m_refLibArr[i]))
      {
        tmpDump = new CDumpPascalInfo;
        tmpDump->DumpTypeLib(W2A(tmpName.m_str),_m_refLibArr[i],pItems);
        delete tmpDump;
      }

      fprintf(_m_pf,";\n\n");
    }

    DumpGuids();
    DumpInterfaces();

    fprintf(_m_pf,"implementation\n\n");

    DumpCoClassImpl();

    fprintf(_m_pf,"end.");
    fclose(_m_pf);
  }
  catch(CTypeLibErr &err)
  {
    err;// to avoid
    fclose(_m_pf);
    DeleteFile(s.c_str());
  }

  try
  {
    // clear the array of type infos...
    for(i=0;i<icount;i++)
    {
      if(_m_typeInfoArr[i])
        delete _m_typeInfoArr[i];
    }

    // remove the list of contained libraries...
    for(i=0;i<_m_refLibArr.size();i++)
    {
      RELEASE_INTERFACE(_m_refLibArr[i]);
    }

    _m_pTypeLib->ReleaseTLibAttr(_m_pLibAttr);
  }
  catch(...)
  {
  }
  
  _m_typeAddedArr.clear();
  _m_typeInfoArr.clear();
  _m_refLibArr.clear();
}


