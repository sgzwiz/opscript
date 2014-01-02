/*
 * OpScriptUnit.cpp - Implementation of Script Engine module objects.
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
#include "opscriptctrl.h"
#include "opscript.h"

/////////////////////////////////////////////////////////////////////////////
// COpError
COpError::COpError()
{
}

COpError::~COpError()
{
}

void COpError::InitError(CScriptErr *pErr)
{
  _m_source_text = pErr->_m_source.c_str();
  _m_error_text = pErr->_m_message.c_str();
  _m_line = pErr->_m_line;
}

STDMETHODIMP COpError::GetExceptionInfo(EXCEPINFO *pexcepinfo)
{
  memset(pexcepinfo,0,sizeof(EXCEPINFO));

  pexcepinfo->bstrSource = SysAllocString(_m_source_text);
  pexcepinfo->bstrDescription = SysAllocString(_m_error_text);
  pexcepinfo->bstrHelpFile = SysAllocString(L" ");
  pexcepinfo->scode = OLESCRIPT_E_SYNTAX;

  return S_OK;
}

STDMETHODIMP COpError::GetSourcePosition(
                  DWORD *pdwSourceContext,
                  ULONG *pulLineNumber,
                  LONG *plCharacterPosition)
{
  if(!IsBadWritePtr(pdwSourceContext,sizeof(DWORD)))*pdwSourceContext = 0;
  if(!IsBadWritePtr(pdwSourceContext,sizeof(ULONG)))*pulLineNumber = _m_line;
  if(!IsBadWritePtr(pdwSourceContext,sizeof(LONG)))*plCharacterPosition = 0;
  return S_OK;
}

STDMETHODIMP COpError::GetSourceLineText(BSTR *pbstrSourceLine)
{
 *pbstrSourceLine = SysAllocString(L" ");
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// COpScriptUnit

void CopScriptUnit::handle_error(CScriptErr *perr )
{
  IActiveScriptError *pErrInt;
  IActiveScriptSite *pSite;

  pSite = _m_manager->GetScriptSite();

  error_count++;

  HRESULT hr;

  if(pSite)
  {
    CComObject<COpError> *pOpError = new CComObject<COpError>();

    pOpError->SetVoid(0);
    pOpError->InternalFinalConstructAddRef();
    pOpError->FinalConstruct();
    pOpError->InitError(perr);
    pOpError->InternalFinalConstructRelease();
    pOpError->QueryInterface(IID_IActiveScriptError, (void**)&pErrInt);
  
    hr = pSite->OnScriptError(pErrInt);  
  }
}

int CopScriptUnit::parseSystem()
{
  CpascalParser pP;
  
  int rc;

  // load the system unit...
  HRSRC hSysData = FindResource(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDR_SCRIPT_SYSTEM), "SCRIPTS");
  HGLOBAL pSdata = LoadResource(_Module.GetResourceInstance(), hSysData);
  DWORD dwSize = SizeofResource(_Module.GetResourceInstance(), hSysData);
  char *pText = (char*)malloc(dwSize+1);

  memcpy(pText,(void*)pSdata,dwSize);
  pText[dwSize]=0;

  rc = pP.parseText(pText,&_m_syntaxTree,this);

  FreeResource(hSysData);
  free(pText);

  return rc;
}

void CopScriptUnit::InitSystem(void)
{
  loadNativeTypes();
}
