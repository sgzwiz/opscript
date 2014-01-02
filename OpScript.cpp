/* 
 * OpScript.cpp - Implementation of Scripting Engine and Named Objects.  
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
#include "Opscriptctrl.h"
#include "OpScript.h"
#include "utils.h"


const DWORD dwSupportedBits = 
      INTERFACESAFE_FOR_UNTRUSTED_CALLER | 
      INTERFACESAFE_FOR_UNTRUSTED_DATA;

const DWORD dwNotSupportedBits = ~dwSupportedBits;

bool _g_traceInfo = false;
DWORD _traceCount = 0;

BEGIN_ENUMHANDLER_MAP(CScripStateHelper,SCRIPTSTATE)
  ENUM_MEMBER(SCRIPTSTATE_UNINITIALIZED)
  ENUM_MEMBER(SCRIPTSTATE_INITIALIZED)
  ENUM_MEMBER(SCRIPTSTATE_STARTED)
  ENUM_MEMBER(SCRIPTSTATE_CONNECTED)
  ENUM_MEMBER(SCRIPTSTATE_DISCONNECTED)
  ENUM_MEMBER(SCRIPTSTATE_CLOSED)
END_ENUMHANDLER_MAP()

BEGIN_FLAGHANDLER_MAP(CAddNamedItemFlags, DWORD)
	FLAG_MEMBER(SCRIPTITEM_CODEONLY)
	FLAG_MEMBER(SCRIPTITEM_GLOBALMEMBERS   )
	FLAG_MEMBER(SCRIPTITEM_ISPERSISTENT  )
	FLAG_MEMBER(SCRIPTITEM_ISSOURCE   )
	FLAG_MEMBER(SCRIPTITEM_ISVISIBLE   )
	FLAG_MEMBER(SCRIPTITEM_NOCODE  )
END_FLAGHANDLER_MAP();

BEGIN_FLAGHANDLER_MAP(CParseScriptTextFlags,DWORD)
	FLAG_MEMBER(SCRIPTTEXT_ISEXPRESSION )
	FLAG_MEMBER(SCRIPTTEXT_ISPERSISTENT)
	FLAG_MEMBER(SCRIPTTEXT_ISVISIBLE  )
  FLAG_MEMBER(SCRIPTTEXT_DELAYEXECUTION)
  FLAG_MEMBER(SCRIPTTEXT_HOSTMANAGESSOURCE)
END_FLAGHANDLER_MAP()

BEGIN_FLAGHANDLER_MAP(CParseProcedureTextFlags,DWORD)
  FLAG_MEMBER(SCRIPTPROC_ISEXPRESSION)
  FLAG_MEMBER(SCRIPTPROC_IMPLICIT_THIS)
  FLAG_MEMBER(SCRIPTPROC_IMPLICIT_PARENTS) 
  FLAG_MEMBER(SCRIPTPROC_HOSTMANAGESSOURCE)
END_FLAGHANDLER_MAP()





/////////////////////////////////////////////////////////////////////////////
// COpNamedItem
COpNamedItem::COpNamedItem(COpScript *pManager,LPCOLESTR name,DWORD dwFlags)
{
  _m_ObjectName=name;
  _m_pTypeInfo=0;
  _m_pObject=0;
  _m_dwFlags=dwFlags;
  _m_Manager=pManager;
}

COpNamedItem::~COpNamedItem()
{
  RELEASE_INTERFACE(_m_pTypeInfo);
  RELEASE_INTERFACE(_m_pObject);
}

BSTR COpNamedItem::GetName()
{
  return _m_ObjectName.m_str;
}

HRESULT COpNamedItem::ConnectObject()
{
  IUnknown **pUnk;
  IActiveScriptSite *pSite;
  // try to connect the item...
  if(!_m_pObject)
  {
    pSite = _m_Manager->GetScriptSite();
    
    if(!pSite)
      return E_FAIL;
   
    pSite->GetItemInfo(_m_ObjectName.m_str,SCRIPTINFO_IUNKNOWN,&_m_pObject,0);

    // if we can't even get a pointer to the IUnknown object, then we are 
    // really in trouble.
    if(!_m_pObject)
      return E_FAIL;
  }

  if((!_m_pUnit->ds)||(_m_pUnit->error_count>0))
    return E_FAIL;

  pUnk=(IUnknown**)((static_cast<DWORD*>(_m_pUnit->ds))+_m_pVariable->offset);
  *pUnk = _m_pObject;

  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// COpScript

COpScript::COpScript()
{ 
	InitTracing();

  _m_pActiveSite = 0;
  _m_scriptState = SCRIPTSTATE_UNINITIALIZED;
  _m_pDumpItems.output_directory = ".";

  CopScriptUnit *pSystem = new CopScriptUnit();

  _m_units.push_back(pSystem);
  _m_unit_names.insert("system",0);

  {
    char tmpPath[MAX_PATH];

    // should throw an exception if this fails...
    GetTempPath(sizeof(tmpPath),tmpPath);

    _m_full_path = tmpPath;
  }

  pSystem->_m_name = "system";
  pSystem->_m_asmpath = _m_full_path + "system.as";
  pSystem->_m_fullpath = "system.pas";
  pSystem->_m_index = 0;
  pSystem->_m_manager = this;
  
  pSystem->InitSystem();

	InitTracing();
}

COpScript::~COpScript()
{
  CloseAll();
  RELEASE_INTERFACE(_m_pActiveSite);
	ClearCpu();

	CloseTracing();
}

IActiveScriptSite *COpScript::GetScriptSite()
{
  return _m_pActiveSite;
}

Cmodule *COpScript::get_unit(size_t l)
{
  return _m_units[l];
}

Cmodule *COpScript::get_unit(const char *p)
{
  size_t i = get_unit_index(p);

  if(i==-1)
    return 0;

  return _m_units[i];
}

size_t COpScript::get_unit_index(const char *p)
{
  tlist<size_t> l;

  if(!_m_unit_names.find(p,l))
    return -1;

  return *l.begin();
}

size_t COpScript::get_count()
{
  return _m_units.size();
}

Cmodule *COpScript::create_unit(const char *p)
{
  CopScriptUnit *pUnit = new CopScriptUnit();

  std::string sn,sp,sa;

  sn = p;

  // drop the extension, if one exists
  remove_extention(sn);

  sp = _m_full_path + sn + ".pas";

  // in debug mode, name the "as" file such that we can easily validate the 
	// pseudo-assembly generated.
#ifdef _DEBUG
  sa = _m_full_path + sn + ".as"; 
#else
	{
		char tmpPath[MAX_PATH];
		char tmpFileName[MAX_PATH];

		GetTempPath(sizeof(tmpPath),tmpPath);
		GetTempFileName(tmpPath,"opsasm",0,tmpFileName);
		sa = tmpFileName;
	}
#endif

  pUnit->_m_asmpath = sa;
  pUnit->_m_name = sn;
  pUnit->_m_fullpath = sp;

  pUnit->_m_index = (int)_m_units.size();
  pUnit->_m_manager  = this;

  _m_unit_names.insert(sn.c_str(),(long)_m_units.size());

  // add the system unit to all new units...
  pUnit->add_unit(_m_units[0]);

  _m_units.push_back(pUnit);

  return pUnit;
}

void COpScript::InitCpu()
{
  BYTE *pStack = (BYTE*)calloc(10*1024,1);// 10K stack (for now...)
  _m_script_cpu.cs = 0;
  _m_script_cpu.ds = 0;
  _m_script_cpu.stack = pStack;
  _m_script_cpu.saved_count = 0;
  _m_script_cpu.saved_regs = 0;
  _m_script_cpu.regs[0].pvchar=(char*)pStack;
  _m_script_cpu.regs[1].pvchar=(char*)pStack;
}

void COpScript::ClearCpu()
{
	if(_m_script_cpu.stack)
	  free(_m_script_cpu.stack);
}

void COpScript::ExecuteFunction(unsigned offset, CopScriptUnit *pUnit)
{
  BYTE *pOldCs,*pOldDs,*pOldRegDs;

  if(offset==-1)
    return;

  pOldCs=_m_script_cpu.cs;
  pOldDs=_m_script_cpu.ds;
  pOldRegDs=(BYTE*)_m_script_cpu.regs[2].pvchar;

	std::auto_ptr<Cmodule*> _units(new Cmodule*[_m_units.size()]);

	for(size_t i = 0;i<_m_units.size();i++){
	  _units.get()[i]=_m_units[i];
	}

  //_m_script_cpu.units=(Cmodule**)&_m_units[0];
  _m_script_cpu.units=_units.get();
	_m_script_cpu.regs[2].pvchar=(char*)pUnit->ds;
  _m_script_cpu.cs = (BYTE*)pUnit->cs;
  _m_script_cpu.ds = (BYTE*)pUnit->ds;
  _m_script_cpu.curr_unit = pUnit->get_index();

  exec_function(offset,&_m_script_cpu);

  _m_script_cpu.cs=pOldCs;
  _m_script_cpu.ds=pOldDs;
  _m_script_cpu.regs[2].pvchar=(char*)pOldRegDs;
}

void COpScript::Initialize(IActiveScriptSite *pSite)
{

}

void COpScript::Reset()
{
}

void COpScript::InitTracing()
{
	char currModuleName[MAX_PATH];
	char currLogName[MAX_PATH];
	char *fname;
  DWORD traceInfo = 0;
	HKEY key;

	if(ERROR_SUCCESS == RegOpenKey(HKEY_CURRENT_USER,"SOFTWARE\\Felix Deschamps\\OpScript\\1.0\\",&key))
	{
		DWORD cb = sizeof(traceInfo);
		DWORD type;
		RegQueryValueEx(key,"DebugTrace",NULL,&type,
			(BYTE*)&traceInfo,
			&cb);
		RegCloseKey(key);
	}

	if(traceInfo){
		GetModuleFileName(_Module.GetModuleInstance(),currModuleName,sizeof(currModuleName));
		GetFullPathName(currModuleName, sizeof(currLogName),currLogName,&fname);
		sprintf(fname,"logOpscript_PID%i_%i_.txt", GetCurrentProcessId(),++_traceCount);

		_m_traceFile=fopen(currLogName,"wt");
	}
	else
		_m_traceFile=0;
}

void COpScript::CloseTracing()
{
	if(_m_traceFile){
		fclose(_m_traceFile);
		_m_traceFile=0;
	}
}

void COpScript::Trace(const char *msg)
{
	if(!_m_traceFile)
		return;

	fprintf(_m_traceFile,"%s", msg);
}

HRESULT COpScript::_ParseScript(LPCOLESTR ItemName, 
                       LPCOLESTR pCode,     
                       ULONG ulStartingLineNumber,
                       DWORD dwFlags,
                       EXCEPINFO *pexcepinfo )
{
  USES_CONVERSION;
  BOOL fWasConnected = false;
  CComBSTR unitName;
  char uCountBuff[20];
  HRESULT hr = E_FAIL;
  size_t i;

  sprintf(uCountBuff,"__%04i",get_count()+1);
  unitName = L"__";
  unitName.Append(((ItemName)&&(ItemName[0]))?ItemName:L"gblUnit");
  unitName.Append(uCountBuff);
  
  // disconnect all events...
  if (_m_scriptState == SCRIPTSTATE_CONNECTED)
  {
    SetScriptState( SCRIPTSTATE_DISCONNECTED );
    fWasConnected = true;
  }

  // Create, parse and compile a unit for this code,
  // but do not execute any code
  {
    CopScriptUnit *pUnit;
		CpascalParser pParser;
		CsyntaxValidator pAnalyzer;
    
    // create the raw unit.
    pUnit=(CopScriptUnit*)create_unit(W2A(unitName.m_str));
    
    // parse the code.
    if(!pParser.parseText(W2A(pCode), &pUnit->_m_syntaxTree, pUnit, (int)ulStartingLineNumber))
    {
      // add previous units (including global units) to this one.
      // also set the appropriate namespace stack.
      for(i=0;i<_m_NamedUnits.size();i++)
        pUnit->add_unit(_m_NamedUnits[i]);

      for(i=0;i<_m_CodeUnits.size();i++)
        pUnit->add_unit(_m_CodeUnits[i]);

      // analyze the new unit
      if(pAnalyzer.validateTree(pUnit))
      {
        // if no errors occurred, compile any new units.(there may be other units added 
        // if they were recursively added from within the script.)
        CompileAll();

        // Wrap the unit around the COM code
        _m_CodeUnits.push_back(pUnit);
        hr = S_OK;
      }
      else
        hr = OLESCRIPT_E_SYNTAX;
    }
    else
      hr = OLESCRIPT_E_SYNTAX;
  }

  //If the script engine was connected to events before this call, then
  //reconnect it to those events
  if (fWasConnected)
    SetScriptState( SCRIPTSTATE_CONNECTED );

  //If the engine has been started, then execute any immediate code in
  //the code block
  switch(_m_scriptState)
  {
    case SCRIPTSTATE_STARTED:
    case SCRIPTSTATE_CONNECTED:
      ExecuteImmediateScripts();
    break;
  }

  return hr;
}

HRESULT COpScript::_ParseExpression(LPCOLESTR pstrItemName,
                           LPCOLESTR pstrCode,
                           IUnknown *punkContext,
                           DWORD dwSourceContextCookie,
                           VARIANT *pvarResult,
                           EXCEPINFO *pexcepinfo)
{
  return S_OK;
}

HRESULT COpScript::_ParseProcedure()
{
  return S_OK;
}

void COpScript::ExecuteImmediateScripts()
{
  size_t i;
  thash<COpNamedItem*>::iterator it;

  // make sure all NamedItems are connected and the objects 
  for(it=_m_NamedItems.begin();it!=_m_NamedItems.end();it++)
    (*it)->ConnectObject();

  for(i=0;i<_m_units.size();i++)
  {
    if((_m_units[i]->error_count==0)&&!_m_units[i]->is_executed())
    {
      ExecuteFunction(_m_units[i]->main_offset,_m_units[i]);
      _m_units[i]->set_executed(true);
    }
  }
}

void COpScript::DisconnectEvents()
{
}

void COpScript::ConnectEvents()
{
}

void COpScript::CloseAll()
{
  thash<COpNamedItem*>::iterator it;

  for(it=_m_NamedItems.begin();it!=_m_NamedItems.end();it++)
    delete (*it);

  _m_NamedItems.clear();


  std::vector<CopScriptUnit*>::iterator unitit;

  for(unitit=_m_units.begin();unitit!=_m_units.end();unitit++)
  {
    delete (*unitit);
  }

  _m_units.clear();
}

void COpScript::CompileAll()
{
  CmoduleCompiler compiler;

  for(size_t i=0;i<_m_units.size();i++)
  {
    if((_m_units[i]->error_count==0)&&!_m_units[i]->is_compiled())
    {
       if(compiler.compileModule(_m_units[i]))
         compile_unit(_m_units[i]);
    }
  }
}

void COpScript::BuildNamedUnit(COpNamedItem *pItem)
{
  USES_CONVERSION;
  std::string sCode;

  CComBSTR unitName;
  CComBSTR typeLibName;
  CComBSTR typeName;
  char uCountBuff[20];
  HRESULT hr = E_FAIL;
  UINT indx;
  HREFTYPE href;
  ITypeLib *pLib=0;

  // get the type info object...
  if(!pItem->_m_pTypeInfo)
  {
    // try asking the site for it...
    _m_pActiveSite->GetItemInfo(pItem->GetName(),SCRIPTINFO_ITYPEINFO,0,&pItem->_m_pTypeInfo);

    // try getting the type info directly from the IUnknown Pointer...
    if(!pItem->_m_pTypeInfo)
    {
      IDispatch *pDispatch=0;
      ITypeInfo *pRealInfo=0;

      _m_pActiveSite->GetItemInfo(pItem->GetName(),SCRIPTINFO_IUNKNOWN,&pItem->_m_pObject,0);

      // if we can't even get a pointer to the IUnknown object, then we are 
      // really in trouble.
      if(!pItem->_m_pObject)
        return;

      pItem->_m_pObject->QueryInterface(IID_IDispatch,(void**)&pDispatch);

      if(!pDispatch)
        goto disp_error;

      if(SUCCEEDED(pDispatch->GetTypeInfo(0,LOCALE_USER_DEFAULT,&pRealInfo)) && pRealInfo)
      {
        // let's see if there is a non-dispatch implementation of this interface...
        if(S_OK==pRealInfo->GetRefTypeOfImplType(-1,&href))
          pRealInfo->GetRefTypeInfo(href,&pItem->_m_pTypeInfo);

        if(!pItem->_m_pTypeInfo)
        {
          pItem->_m_pTypeInfo=pRealInfo;
          pItem->_m_pTypeInfo->AddRef();
        }
      }

disp_error:
     
      RELEASE_INTERFACE(pRealInfo);
      RELEASE_INTERFACE(pDispatch);
    }
  }

  // if we haven't been able to aquire a type info object, 
  // then we can't continue...
	/*
  if(pItem->_m_pTypeInfo){
    // first, see if it's an IDispatch. if it is then don't do anything...
    TYPEATTR *attr;

    pItem->_m_pTypeInfo->GetTypeAttr(&attr);

    if(attr->typekind == TKIND_INTERFACE){
      pItem->_m_pTypeInfo->GetContainingTypeLib(&pLib,&indx);

      if(pLib){
        // we probably can try using the object's  typeinfo directly, 
        // but i wont support that right now
        pLib->GetDocumentation(-1,&typeLibName,0,0,0);
        pLib->GetDocumentation(indx,&typeName,0,0,0);

        CDumpPascalInfo dumpInfo;
     
        dumpInfo.DumpTypeLib(W2A(typeLibName.m_str),pLib,&_m_pDumpItems);
	
			} else {
        typeName = "IDispatch";
			}
    } else {
      typeName = "IDispatch";
		}

    pItem->_m_pTypeInfo->ReleaseTypeAttr(attr);
	
	} else {
	*/
    typeName = "IDispatch";
	//}

  sprintf(uCountBuff,"__%04i",get_count()+1);
  unitName = L"__";
  unitName.Append(pItem->GetName());
  unitName.Append(uCountBuff);

  sCode = "\nunit ";
  sCode += W2A(unitName.m_str);
  sCode += ";\ninterface\n\n";

  if(typeLibName.Length()>0)
  {
    sCode += "uses ";
    sCode += W2A(typeLibName.m_str);
    sCode += "_TLB;\n\n";
  }

  sCode += "var\n";
  sCode += W2A(pItem->GetName());
  sCode += " : ";
  sCode += W2A(typeName.m_str);
  sCode += ";\n\nimplementation\nend.\n";

  RELEASE_INTERFACE(pLib);

  {
    CsyntaxValidator pAnalyzer;
    CpascalParser pParser;
    
    // create the raw unit.
    pItem->_m_pUnit=(CopScriptUnit*)create_unit(W2A(unitName.m_str));
    
    // parse the code.
    if(pParser.parseText(sCode.c_str(), &pItem->_m_pUnit->_m_syntaxTree,pItem->_m_pUnit))
      return;
    
    // analyze the new unit
    if(!pAnalyzer.validateTree(pItem->_m_pUnit))
        return;

    _m_NamedUnits.push_back(pItem->_m_pUnit);
    pItem->_m_pVariable = (sym_variable*)pItem->_m_pUnit->global.first_symbol;

  }
}

void COpScript::AddItem(COpNamedItem *pItem)
{
  _m_NamedItems.insert(pItem->GetName(),pItem);
}

COpNamedItem *COpScript::GetItem(LPCOLESTR pName )
{
  tlist<COpNamedItem*> l;

  if(_m_NamedItems.find(pName,l))
    return *l.begin();

  return 0;
}

HRESULT WINAPI COpScript::UpdateRegistry(BOOL bRegister)
{
  USES_CONVERSION;
  _ATL_REGMAP_ENTRY entries[3];
  char ScriptPath[MAX_PATH];
  char CmdScriptPath[MAX_PATH];
  char *SName;

  if(!SearchPath(0,"WScript.exe",0,MAX_PATH,ScriptPath,&SName))
    strcpy(ScriptPath,"C:\\Windows\\WScript.exe");

  if(!SearchPath(0,"CScript.exe",0,MAX_PATH,CmdScriptPath,&SName))
    strcpy(ScriptPath,"C:\\Windows\\CScript.exe");

  entries[0].szKey = L"SCRIPT_HOST";
  entries[0].szData = A2W(ScriptPath);
  entries[1].szKey  = L"CMD_SCRIPT_HOST";
  entries[1].szData= A2W(CmdScriptPath);
  entries[2].szData = entries[2].szKey = 0;
  
  return _Module.UpdateRegistryFromResource(IDR_OPSCRIPT, bRegister, entries);
}

STDMETHODIMP COpScript::InterfaceSupportsErrorInfo(REFIID riid)
{
  BEGIN_TRACE()
		ADD_PARAM(riid)
	END_TRACE()

  static const IID* arr[] = 
  {
    &IID_IOpScript
  };
  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++){
		if(*arr[i]==riid)
      return S_OK;
  }
  return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IActiveScript Implementation.
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP COpScript::SetScriptSite(IActiveScriptSite *pass)
{
  BEGIN_TRACE()
	END_TRACE()


  RELEASE_INTERFACE(_m_pActiveSite);
  _m_pActiveSite = pass;
  pass->AddRef();   

  Initialize(pass);
  _m_pActiveSite->OnStateChange(_m_scriptState);
  return S_OK;
}

STDMETHODIMP COpScript::GetScriptSite( REFIID riid,void **ppvObject)
{
	 BEGIN_TRACE()
		 ADD_PARAM(riid)
		 ADD_PARAM(ppvObject)
	 END_TRACE()

  return _m_pActiveSite->QueryInterface(riid,ppvObject);
}



STDMETHODIMP COpScript::SetScriptState(SCRIPTSTATE ss)
{
	 BEGIN_TRACE()
		 ADD_PARAM_HANDLER(ss,CScripStateHelper )
	 END_TRACE()

   //If we are already in the requested state, do nothing and return S_OK;
   if (ss == _m_scriptState)
      return S_OK;

   //If we are in a Closed state, then this method shouldn't have been called
   //so return E_UNEXPECTED.
   if ( _m_scriptState == SCRIPTSTATE_CLOSED)
      return E_UNEXPECTED;
   
   HRESULT hr = S_OK;

   //Otherwise, transition from the current state to the requested state.  
   //Switch statements allow us to fall through intermediate states to the
   //desired state.
   switch(ss) {
   case SCRIPTSTATE_UNINITIALIZED:
      switch(_m_scriptState) {
         case SCRIPTSTATE_CONNECTED:
            //Disconnect from any event sinks
            DisconnectEvents();
         case SCRIPTSTATE_DISCONNECTED:
         case SCRIPTSTATE_STARTED:
            //Reset the engine
            Reset();
         case SCRIPTSTATE_INITIALIZED:
            //Release the IActiveScriptSite interface
            RELEASE_INTERFACE(_m_pActiveSite);
            _m_scriptState = SCRIPTSTATE_UNINITIALIZED;
            hr = S_OK;
            break;
      }
      break;

   case SCRIPTSTATE_INITIALIZED:
      switch(_m_scriptState) {
         case SCRIPTSTATE_CONNECTED:
            //Disconnect from any event sinks
            DisconnectEvents();
         case SCRIPTSTATE_DISCONNECTED:
         case SCRIPTSTATE_STARTED:
            //Reset the engine
            Reset();
         case SCRIPTSTATE_UNINITIALIZED:
            _m_scriptState = SCRIPTSTATE_INITIALIZED;
            _m_pActiveSite->OnStateChange( _m_scriptState );
            hr = S_OK;
            break;
      }
      break;
      
   case SCRIPTSTATE_STARTED:
      switch(_m_scriptState) {
         case SCRIPTSTATE_CONNECTED:
            //Disconnect from any event sinks
            DisconnectEvents();
         case SCRIPTSTATE_DISCONNECTED:
            //Release any interface pointers
         case SCRIPTSTATE_UNINITIALIZED:
         case SCRIPTSTATE_INITIALIZED:
            //Connect interface pointers
            //Execute immediate scripts
            ExecuteImmediateScripts();
            _m_scriptState = SCRIPTSTATE_STARTED;
            _m_pActiveSite->OnStateChange( _m_scriptState );
            hr = S_OK;
            break;
      }
      break;

   case SCRIPTSTATE_CONNECTED:
      switch(_m_scriptState) {
         case SCRIPTSTATE_UNINITIALIZED:
         case SCRIPTSTATE_INITIALIZED:
         case SCRIPTSTATE_STARTED:
            //Connect interface pointers
            //Execute immediate scripts
            ExecuteImmediateScripts();
         case SCRIPTSTATE_DISCONNECTED:
            //Connect any event sinks
            ConnectEvents();
            _m_scriptState = SCRIPTSTATE_CONNECTED;
            _m_pActiveSite->OnStateChange( _m_scriptState );
            hr = S_OK;
            break;
      }
      break;
   
   case SCRIPTSTATE_DISCONNECTED:
      switch(_m_scriptState) {
         case SCRIPTSTATE_CONNECTED:
            //Disconnect from any event sinks
            DisconnectEvents();
         case SCRIPTSTATE_STARTED:
         case SCRIPTSTATE_INITIALIZED:
         case SCRIPTSTATE_UNINITIALIZED:
            _m_scriptState = SCRIPTSTATE_DISCONNECTED;
            _m_pActiveSite->OnStateChange( _m_scriptState );
            hr = S_OK;
            break;
      }
      break;
   
   case SCRIPTSTATE_CLOSED:
      switch(_m_scriptState) {
         case SCRIPTSTATE_CONNECTED:
            //Disconnect from any event sinks
            DisconnectEvents();
         case SCRIPTSTATE_DISCONNECTED:
         case SCRIPTSTATE_STARTED:
            //Release any interface pointers
         case SCRIPTSTATE_INITIALIZED:
         case SCRIPTSTATE_UNINITIALIZED:
            hr = Close();
            break;
      }
      break;
   
   default:
      break;
   }
   
  return hr;
}

STDMETHODIMP COpScript::GetScriptState( SCRIPTSTATE *pssState)
{
	SIMPLE_TRACE()

  *pssState = _m_scriptState;
  return S_OK;
}

STDMETHODIMP COpScript::Close( void)
{
	SIMPLE_TRACE()

  if(_m_scriptState == SCRIPTSTATE_CLOSED)
    return S_OK;

  CloseAll();

  _m_scriptState = SCRIPTSTATE_CLOSED;
  if(_m_pActiveSite)
    _m_pActiveSite->OnStateChange(_m_scriptState);
  RELEASE_INTERFACE(_m_pActiveSite);
  return S_OK;
}

STDMETHODIMP COpScript::AddNamedItem(LPCOLESTR pstrName,DWORD dwFlags)
{
	BEGIN_TRACE()
		 ADD_PARAM(pstrName)
		 ADD_PARAM_HANDLER(dwFlags,CAddNamedItemFlags)
	END_TRACE()

  USES_CONVERSION;
  IUnknown *pUnk=0;

  //First, check the flags to make sure we don't have conflicts.  We don't
  //support SCRIPTITEM_ALL_FLAGS
  if ((dwFlags & SCRIPTITEM_CODEONLY) && (dwFlags & (SCRIPTITEM_GLOBALMEMBERS |
    SCRIPTITEM_ISSOURCE | SCRIPTITEM_ISVISIBLE | SCRIPTITEM_NOCODE)))
    //CODEONLY conflicts with everything but ISPERSISTENT
    return E_INVALIDARG;

  if ((dwFlags & SCRIPTITEM_ISSOURCE) && (dwFlags & SCRIPTITEM_NOCODE))
    //ISSOURCE conflicts with NOCODE
    return E_INVALIDARG;

  if (dwFlags & SCRIPTITEM_NOCODE)
  {
    //We don't want to create a NamedItem here because the item may not be
    //visible.  Although NOCODE without ISVISIBLE doesn't make much sense,
    //its not actually a conflict.
  }
  else if (dwFlags & (SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISVISIBLE))
  {
    // first, make sure the item is not there already...
    if(!GetItem(pstrName))
    {
      COpNamedItem *pItem = new COpNamedItem(this,pstrName,dwFlags);
      AddItem(pItem);
      BuildNamedUnit(pItem);
    }
  }

  if ((dwFlags & SCRIPTITEM_ISSOURCE) || !(dwFlags & SCRIPTITEM_NOCODE))
  {
  }

  // If dwFlags contains SCRIPTITEM_ISSOURCE, then we need to create an
  // event handler for this named item.
  if (dwFlags & SCRIPTITEM_ISSOURCE)
  {
  }

  return S_OK;
}

STDMETHODIMP COpScript::AddTypeLib( REFGUID rguidTypeLib, DWORD dwMajor, DWORD dwMinor, DWORD dwFlags )
{
   BEGIN_TRACE()
		 ADD_PARAM(rguidTypeLib)
		 ADD_PARAM(dwMajor)
		 ADD_PARAM(dwMinor)
		 ADD_PARAM(dwFlags )
	 END_TRACE()
  return E_NOTIMPL;
}

STDMETHODIMP COpScript::GetScriptDispatch( LPCOLESTR pstrItemName, IDispatch **ppdisp)
{
	HRESULT hr;

  BEGIN_TRACE()
		ADD_PARAM(pstrItemName)
	END_TRACE()

	

  COpDispatch *	pDispatch = new COpDispatch(this,0);
        //*ppdisp = pDispatch; 

  hr = pDispatch->QueryInterface(IID_IDispatch,(void**)ppdisp);

  return hr;
}

STDMETHODIMP COpScript::GetCurrentScriptThreadID( SCRIPTTHREADID *pstidThread)
{
	SIMPLE_TRACE()

  return E_NOTIMPL;
}

STDMETHODIMP COpScript::GetScriptThreadID( DWORD dwWin32ThreadId, SCRIPTTHREADID *pstidThread)
{
  BEGIN_TRACE()
		ADD_PARAM(dwWin32ThreadId)
	END_TRACE()

  return E_NOTIMPL;
}

STDMETHODIMP COpScript::GetScriptThreadState( SCRIPTTHREADID stidThread, SCRIPTTHREADSTATE *pstsState)
{
  BEGIN_TRACE()
		ADD_PARAM(stidThread)
	END_TRACE()

  return E_NOTIMPL;
}

STDMETHODIMP COpScript::InterruptScriptThread( SCRIPTTHREADID stidThread, const EXCEPINFO *pexcepinfo, DWORD dwFlags)
{
  BEGIN_TRACE()
		ADD_PARAM(stidThread)
		ADD_PARAM(dwFlags)
	END_TRACE()

  return E_NOTIMPL;
}

STDMETHODIMP COpScript::Clone( IActiveScript **ppscript)
{
  BEGIN_TRACE()
	END_TRACE()

  return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////////
// IActiveScriptParse Implementation.
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP COpScript::InitNew(void)
{
	SIMPLE_TRACE();

  _m_scriptState=SCRIPTSTATE_INITIALIZED;
  if(_m_pActiveSite)
    _m_pActiveSite->OnStateChange(_m_scriptState);

  InitCpu();

  return S_OK;
}

STDMETHODIMP COpScript::AddScriptlet( 
     LPCOLESTR pstrDefaultName,
     LPCOLESTR pstrCode,
     LPCOLESTR pstrItemName,
     LPCOLESTR pstrSubItemName,
     LPCOLESTR pstrEventName,
     LPCOLESTR pstrDelimiter,
     DWORD dwSourceContextCookie,
     ULONG ulStartingLineNumber,
     DWORD dwFlags,
     BSTR *pbstrName,
     EXCEPINFO *pexcepinfo)
{
  BEGIN_TRACE()
	END_TRACE()

	return S_OK;
}

STDMETHODIMP COpScript::ParseScriptText( 
     LPCOLESTR pCode,
     LPCOLESTR ItemName,
     IUnknown *Context,
     LPCOLESTR Delimiter,
     DWORD dwCtxCookie,
     ULONG lineNo,
     DWORD dwFlags,
     VARIANT *pResult,
     EXCEPINFO *pExceptInfo)
{
	BEGIN_TRACE()
    ADD_PARAM(pCode)
    ADD_PARAM(ItemName)
    ADD_PARAM(Context)
    ADD_PARAM(Delimiter)
    ADD_PARAM(dwCtxCookie)
    ADD_PARAM(lineNo)
    ADD_PARAM_HANDLER(dwFlags,CParseScriptTextFlags)
	END_TRACE()

  HRESULT hr;

  hr = (dwFlags & SCRIPTTEXT_ISEXPRESSION)? _ParseExpression(ItemName,pCode,Context,dwCtxCookie,pResult,pExceptInfo) :
                                            _ParseScript(ItemName,pCode,lineNo,dwFlags,pExceptInfo);
  return hr;
}

///////////////////////////////////////////////////////////////////////////////
// IObjectSafety Implementation.
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP COpScript::SetInterfaceSafetyOptions(REFIID id,DWORD op,DWORD d)
{
	BEGIN_TRACE()
		ADD_PARAM(id)
		ADD_PARAM(op)
		ADD_PARAM(d)
	END_TRACE()

  return S_OK;
}

STDMETHODIMP COpScript::GetInterfaceSafetyOptions(REFIID id,DWORD *pso, DWORD *peo)
{
  BEGIN_TRACE()
		ADD_PARAM(id)
	END_TRACE()

	*pso = INTERFACESAFE_FOR_UNTRUSTED_CALLER  | INTERFACE_USES_DISPEX | INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACE_USES_SECURITY_MANAGER;
  *peo = INTERFACESAFE_FOR_UNTRUSTED_CALLER  | INTERFACE_USES_DISPEX | INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACE_USES_SECURITY_MANAGER;
  return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// IActiveScriptParseProcedure Implementation.
///////////////////////////////////////////////////////////////////////////////
STDMETHODIMP COpScript::ParseProcedureText(
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrFormalParams,
            /* [in] */ LPCOLESTR pstrProcedureName,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ IUnknown *punkContext,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwSourceContextCookie,
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDispatch **ppdisp)
{
  BEGIN_TRACE()
		ADD_PARAM(pstrCode)
		ADD_PARAM(pstrFormalParams)
		ADD_PARAM(pstrProcedureName)
		ADD_PARAM(pstrItemName)
		ADD_PARAM(punkContext)
		ADD_PARAM(pstrDelimiter)
		ADD_PARAM(dwSourceContextCookie)
		ADD_PARAM(ulStartingLineNumber)
		ADD_PARAM_HANDLER(dwFlags,CParseProcedureTextFlags)
	END_TRACE()

  //COpScriptUnit *pUnit = 
  //m_ProcedureUnits
  *ppdisp = 0;

  USES_CONVERSION;
  CComBSTR unitName;
  CComBSTR unitCode;
  char uCountBuff[20];
  HRESULT hr = E_FAIL;
  size_t i;
  std::string proc_name;

  sprintf(uCountBuff,"__%04i",get_count()+1);
  unitName = L"__";
  unitName.Append(((pstrItemName)&&(pstrItemName[0]))?pstrItemName:L"gblUnit");
  unitName.Append(uCountBuff);


  unitCode.Append("procedure ");
  unitCode.Append(unitName);
  unitCode.Append("_proc; begin ");
  unitCode.Append(pstrCode);
  unitCode.Append(" \nend;");
  
  // Create, parse and compile a unit for this code,
  // but do not execute any code
  {
    CopScriptUnit *pUnit;
    COpDispatch *pDispatch;
    CsyntaxValidator pAnalyzer;
    CpascalParser pParser;

    // create the raw unit.
    pUnit=(CopScriptUnit*)create_unit(W2A(unitName.m_str));
    
    // parse the code.
    if(!pParser.parseText(W2A(unitCode.m_str), &pUnit->_m_syntaxTree,pUnit, (int)ulStartingLineNumber))
    {
      // add previous units (including global units) to this one.
      // also set the appropriate namespace stack.
      for(i=0;i<_m_NamedUnits.size();i++)
        pUnit->add_unit(_m_NamedUnits[i]);

      for(i=0;i<_m_CodeUnits.size();i++)
        pUnit->add_unit(_m_CodeUnits[i]);

      // analyze the new unit
      if(pAnalyzer.validateTree(pUnit))
      {
        // if no errors occurred, compile any new units.(there may be other units added 
        // if they were recursively added from within the script.)
        CompileAll();

        // Wrap the unit around the COM code
        _m_ProcedureUnits.push_back(pUnit);

        pDispatch = new COpDispatch(this,(sym_procedure*)pUnit->global.first_symbol);
        //*ppdisp = pDispatch; 

        hr = pDispatch->QueryInterface(IID_IDispatch,(void**)ppdisp);
      }
      else
        hr = OLESCRIPT_E_SYNTAX;
    }
    else
      hr = OLESCRIPT_E_SYNTAX;
  }

  return hr;
}

///////////////////////////////////////////////////////////////////////////////
// COpDispatch Implementation.
///////////////////////////////////////////////////////////////////////////////
COpDispatch::COpDispatch(COpScript *pEng, sym_procedure *pProc)
{
  _m_pEngine = pEng;
	_m_proc_offset = (pProc)? pProc->proc_offset : 0;
	_m_pCodeUnit = (pProc)? _m_pEngine->get_unit(pProc->unit_index) : 0;
  _m_dwRef = 0;
  _m_pTInfo = 0;
}

COpDispatch::~COpDispatch()
{
  RELEASE_INTERFACE(_m_pTInfo);
}

STDMETHODIMP COpDispatch::QueryInterface(REFIID riid, void **ppvObject)
{
  HRESULT hr=E_NOINTERFACE;

  *ppvObject = 0;

  if(riid == IID_IUnknown || riid == IID_IDispatch)
   *ppvObject = this;

  if(*ppvObject)
  {
    AddRef();
    hr = S_OK;
  }

  return hr;
}

STDMETHODIMP_(ULONG) COpDispatch::AddRef(void)
{
  return InterlockedIncrement((LONG*)&_m_dwRef);;
}

STDMETHODIMP_(ULONG) COpDispatch::Release( void)
{
  ULONG r = InterlockedDecrement((LONG*)&_m_dwRef);
  if(r==0)
    delete this;
  return r;
}

STDMETHODIMP COpDispatch::GetIDsOfNames( REFIID  riid,                  
                              OLECHAR FAR* FAR*  rgszNames,  
                              unsigned int  cNames,          
                              LCID   lcid,                   
                              DISPID FAR*  rgDispId)
{
  return E_NOTIMPL;
}

STDMETHODIMP COpDispatch::GetTypeInfo( unsigned int  iTInfo,         
                            LCID  lcid,                   
                            ITypeInfo FAR* FAR*  ppTInfo)
{
  return E_NOTIMPL;
}

STDMETHODIMP COpDispatch::GetTypeInfoCount(unsigned int FAR*  pctinfo)
{
  return E_NOTIMPL;
}

STDMETHODIMP COpDispatch::Invoke( DISPID  dispIdMember,      
                       REFIID  riid,              
                       LCID  lcid,                
                       WORD  wFlags,              
                       DISPPARAMS FAR*  pDispParams,  
                       VARIANT FAR*  pVarResult,  
                       EXCEPINFO FAR*  pExcepInfo,  
                       unsigned int FAR*  puArgErr)
{
  switch(dispIdMember)
  {
    case 0:
      _m_pEngine->ExecuteFunction((unsigned int)_m_proc_offset,(CopScriptUnit*)_m_pCodeUnit);
      return S_OK;
    break;
  }  

  return S_OK;
}





