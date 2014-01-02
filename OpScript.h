/* 
 * OpScript.h : Active Scripting ATL classes
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __opscript_h__
#define __opscript_h__
//#define INITGUID
#include "resource.h"       // main symbols
#include <activscp.h>
#include "activsp_plus.h"

#define _UNICODE_LOOKUP

#include <opascalParser.h>
#include <moduleCompiler.h>
#include <interpreter.h>
#include "pascalDump.h"
#include <moduleimpl.h>

// Found at C:\Develop\SDKS\Platform\include\azroles.h, although it is supposed to be 
// SCRIPT_E_REPORTED, as per 
#ifndef OLESCRIPT_E_SYNTAX
#define OLESCRIPT_E_SYNTAX 0x80020101
#endif

#ifndef SCRIPT_E_REPORTED
#define SCRIPT_E_REPORTED 0x80020101
#endif


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations.
class COpScript;
class CopScriptUnit;
class COpNamedItem;
class COpError;


/////////////////////////////////////////////////////////////////////////////
// COpDispatch
class COpDispatch : 
  public IDispatch
{
  ULONG _m_dwRef;
  COpScript *_m_pEngine;
  Cmodule *_m_pCodeUnit;
  int _m_proc_offset;
  ITypeInfo *_m_pTInfo;
public:
  COpDispatch(COpScript *);
  COpDispatch(COpScript *, sym_procedure *pProc);
  ~COpDispatch();
  // IUnknown 
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release( void);

  // IDispatch
  STDMETHODIMP GetIDsOfNames( REFIID  riid,                  
                              OLECHAR FAR* FAR*  rgszNames,  
                              unsigned int  cNames,          
                              LCID  lcid,                   
                              DISPID FAR*  rgDispId);
  STDMETHODIMP GetTypeInfo( unsigned int  iTInfo,         
                            LCID  lcid,                   
                            ITypeInfo FAR* FAR*  ppTInfo);
  STDMETHODIMP GetTypeInfoCount(unsigned int FAR*  pctinfo);  
  STDMETHODIMP Invoke( DISPID  dispIdMember,      
                       REFIID  riid,              
                       LCID  lcid,                
                       WORD  wFlags,              
                       DISPPARAMS FAR*  pDispParams,  
                       VARIANT FAR*  pVarResult,  
                       EXCEPINFO FAR*  pExcepInfo,  
                       unsigned int FAR*  puArgErr);
};

/////////////////////////////////////////////////////////////////////////////
// COpNamedItem
class COpNamedItem
{
public:
  CComBSTR _m_ObjectName;
  ITypeInfo *_m_pTypeInfo;
  IUnknown *_m_pObject;
  DWORD _m_dwFlags;
  COpScript *_m_Manager;
  CopScriptUnit *_m_pUnit;
  sym_variable *_m_pVariable;
public:
  COpNamedItem(COpScript *pManager,LPCOLESTR name,DWORD dwFlags);
  ~COpNamedItem();

  void Reset();
  HRESULT ConnectObject();

  BSTR GetName();
};

class COpError : 
  public CComObjectRootEx<CComSingleThreadModel>,
  public IActiveScriptError
{
private:
  _bstr_t _m_error_text;
  _bstr_t _m_source_text;
  int _m_line;
public:
  COpError();
  ~COpError();
 
  void InitError(CScriptErr *pErr);

  BEGIN_COM_MAP(COpError)
    COM_INTERFACE_ENTRY(IActiveScriptError)
  END_COM_MAP()

  STDMETHODIMP GetExceptionInfo(EXCEPINFO *pexcepinfo);
  STDMETHODIMP GetSourcePosition(
                  DWORD *pdwSourceContext,
                  ULONG *pulLineNumber,
                  LONG *plCharacterPosition);
  STDMETHODIMP GetSourceLineText(BSTR *pbstrSourceLine);
};

/////////////////////////////////////////////////////////////////////////////
// COpScriptUnit
class CopScriptUnit : 
  public CmoduleImpl<CopScriptUnit,COpScript>
{
protected:
  void InitSystem(void);
public:
  CopScriptUnit(){};
  ~CopScriptUnit(){};
  
  void handle_error(CScriptErr *perr);
  int parseSystem();

  friend COpScript;
};

/////////////////////////////////////////////////////////////////////////////
// COpScript
class COpScript : 
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<COpScript, &CLSID_OpScript>,
  public CModuleManager,
  public ISupportErrorInfo,
  public IConnectionPointContainerImpl<COpScript>,
  public IDispatchImpl<IOpScript, &IID_IOpScript, &LIBID_OPSCRIPTCTRLLib>,
  public IActiveScript,
  public IActiveScriptParse,
  public IActiveScriptParseProcedure,
  public IActiveScriptSiteDebug,
  public IObjectSafety
{
protected:
// Data Members : 
  IActiveScriptSite *_m_pActiveSite;
  SCRIPTSTATE _m_scriptState;
  thash<COpNamedItem*> _m_NamedItems;
  std::string _m_full_path;
  CGlobalPascalItems _m_pDumpItems;
  thash<size_t,11> _m_unit_names;
  std::vector<CopScriptUnit*> _m_units;
  std::vector<CopScriptUnit*> _m_NamedUnits;
  std::vector<CopScriptUnit*> _m_CodeUnits;
  std::vector<CopScriptUnit*> _m_ProcedureUnits;
	FILE *_m_traceFile;
  
  cpu_state _m_script_cpu;

// CCodeManager Methods
public:
  virtual Cmodule *get_unit(size_t);
  virtual Cmodule *get_unit(const char *);
  virtual Cmodule *create_unit(const char *);
  size_t get_count();
  size_t get_unit_index(const char *);  
  void CompileAll();
  IActiveScriptSite *GetScriptSite();

protected:

// Debugging support 
	void InitTracing();
	void CloseTracing();
	void Trace(const char *msg);

// Helper Methods
  void Initialize(IActiveScriptSite *pSite);
  void Reset();
  void CloseAll();
  void InitCpu();
	void ClearCpu();

  HRESULT _ParseScript( LPCOLESTR pstrItemName, 
                        LPCOLESTR pstrCode,     
                        ULONG ulStartingLineNumber,
                        DWORD dwFlags,
                        EXCEPINFO *pexcepinfo );

  HRESULT _ParseExpression( LPCOLESTR pstrItemName,
                            LPCOLESTR pstrCode,
                            IUnknown *punkContext,
                            DWORD dwSourceContextCookie,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo );

  HRESULT _ParseProcedure();

  void AddItem(COpNamedItem *pItem);
  COpNamedItem *GetItem(LPCOLESTR pName);
  void ExecuteImmediateScripts();
  void DisconnectEvents();
  void ConnectEvents();
  void BuildNamedUnit(COpNamedItem *pItem);
public:
  COpScript();
  ~COpScript();

  void ExecuteFunction(unsigned offset, CopScriptUnit *pUnit);

static HRESULT WINAPI UpdateRegistry(BOOL bRegister);

DECLARE_NOT_AGGREGATABLE(COpScript)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COpScript)
  COM_INTERFACE_ENTRY(IOpScript)
  COM_INTERFACE_ENTRY(IActiveScript)
  COM_INTERFACE_ENTRY(IActiveScriptParse)
  COM_INTERFACE_ENTRY(IActiveScriptParseProcedure)
  COM_INTERFACE_ENTRY(IObjectSafety)
  COM_INTERFACE_ENTRY(IDispatch)
  COM_INTERFACE_ENTRY(ISupportErrorInfo)
  COM_INTERFACE_ENTRY(IConnectionPointContainer)
  COM_INTERFACE_ENTRY(IActiveScriptSiteDebug)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(COpScript)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IOpScript

// IActiveScript
  STDMETHODIMP SetScriptSite(IActiveScriptSite *pass);
  STDMETHODIMP GetScriptSite( REFIID riid,void **ppvObject);
  STDMETHODIMP SetScriptState( SCRIPTSTATE ss);
  STDMETHODIMP GetScriptState( SCRIPTSTATE *pssState);
  STDMETHODIMP Close( void);
  STDMETHODIMP AddNamedItem(LPCOLESTR pstrName,DWORD dwFlags);
  STDMETHODIMP AddTypeLib( REFGUID rguidTypeLib, DWORD dwMajor, DWORD dwMinor, DWORD dwFlags);
  STDMETHODIMP GetScriptDispatch( LPCOLESTR pstrItemName, IDispatch **ppdisp);
  STDMETHODIMP GetCurrentScriptThreadID( SCRIPTTHREADID *pstidThread);
  STDMETHODIMP GetScriptThreadID( DWORD dwWin32ThreadId, SCRIPTTHREADID *pstidThread);
  STDMETHODIMP GetScriptThreadState( SCRIPTTHREADID stidThread, SCRIPTTHREADSTATE *pstsState);
  STDMETHODIMP InterruptScriptThread( SCRIPTTHREADID stidThread, const EXCEPINFO *pexcepinfo, DWORD dwFlags);
  STDMETHODIMP Clone( IActiveScript **ppscript);

// IActiveScriptParse
  STDMETHODIMP InitNew( void);
  STDMETHODIMP AddScriptlet( 
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
     EXCEPINFO *pexcepinfo);
 
  STDMETHODIMP ParseScriptText( 
     LPCOLESTR pstrCode,
     LPCOLESTR pstrItemName,
     IUnknown *punkContext,
     LPCOLESTR pstrDelimiter,
     DWORD dwSourceContextCookie,
     ULONG ulStartingLineNumber,
     DWORD dwFlags,
     VARIANT *pvarResult,
     EXCEPINFO *pexcepinfo);

// IActiveScriptParseProcedure
STDMETHODIMP ParseProcedureText(
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrFormalParams,
            /* [in] */ LPCOLESTR pstrProcedureName,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ IUnknown *punkContext,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwSourceContextCookie,
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDispatch **ppdisp);

// IObjectSafety
  STDMETHODIMP SetInterfaceSafetyOptions(REFIID,DWORD,DWORD);
  STDMETHODIMP GetInterfaceSafetyOptions(REFIID,DWORD *, DWORD *);
// IActiveScriptSiteDebug
  virtual HRESULT STDMETHODCALLTYPE GetDocumentContextFromPosition(
      DWORD dwSourceContext,// As provided to ParseScriptText
                            // or AddScriptlet
      ULONG uCharacterOffset,// character offset relative
                             // to start of script block or scriptlet
      ULONG uNumChars,// Number of characters in context
                      // Returns the document context corresponding to this character-position range.
      IDebugDocumentContext **ppsc) 
   {
     return E_NOTIMPL;
   }
   // Returns the debug application object associated with this script site.Provides
   // a means for a smart host to define what application object each scriptbelongs to.
   // Script engines should attempt to call this method to get theircontaining application
   // and resort to IProcessDebugManager::GetDefaultApplication if this fails.
   virtual HRESULT STDMETHODCALLTYPE GetDefaultApplication( IDebugApplication **ppda) 
   {
     return E_NOTIMPL;
   }
   // Gets the application node under which script documents should be added 
   // can return NULL if script documents should be top-level. 
   virtual HRESULT STDMETHODCALLTYPE GetRootApplicationNode( IDebugApplicationNode **ppdanRoot)  
   {
     return E_NOTIMPL;
   }

   // Allows a smart host to control the handling of runtime errors 
   virtual HRESULT STDMETHODCALLTYPE OnScriptErrorDebug( 
      // the runtime error that occurred 
      IActiveScriptErrorDebug *pErrorDebug, 
      // whether to pass the error to the debugger to do JIT debugging 
      BOOL*pfEnterDebugger, 
      // whether to call IActiveScriptSite::OnScriptError() when the user 
      // decides to continue without debugging 
      BOOL *pfCallOnScriptErrorWhenContinuing) 
   {
     return E_NOTIMPL;
   }

public:
};

#endif //__OPSCRIPT_H_
