

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jan 10 20:02:49 2011
 */
/* Compiler settings for opscriptctrl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __opscriptctrl_h__
#define __opscriptctrl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOpScript_FWD_DEFINED__
#define __IOpScript_FWD_DEFINED__
typedef interface IOpScript IOpScript;
#endif 	/* __IOpScript_FWD_DEFINED__ */


#ifndef ___IOpScriptEvents_FWD_DEFINED__
#define ___IOpScriptEvents_FWD_DEFINED__
typedef interface _IOpScriptEvents _IOpScriptEvents;
#endif 	/* ___IOpScriptEvents_FWD_DEFINED__ */


#ifndef __OpScript_FWD_DEFINED__
#define __OpScript_FWD_DEFINED__

#ifdef __cplusplus
typedef class OpScript OpScript;
#else
typedef struct OpScript OpScript;
#endif /* __cplusplus */

#endif 	/* __OpScript_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IOpScript_INTERFACE_DEFINED__
#define __IOpScript_INTERFACE_DEFINED__

/* interface IOpScript */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IOpScript;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C5267F85-759C-41EA-8872-5A037E1B5145")
    IOpScript : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IOpScriptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOpScript * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOpScript * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOpScript * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOpScript * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOpScript * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOpScript * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOpScript * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IOpScriptVtbl;

    interface IOpScript
    {
        CONST_VTBL struct IOpScriptVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOpScript_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOpScript_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOpScript_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOpScript_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOpScript_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOpScript_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOpScript_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOpScript_INTERFACE_DEFINED__ */



#ifndef __OPSCRIPTCTRLLib_LIBRARY_DEFINED__
#define __OPSCRIPTCTRLLib_LIBRARY_DEFINED__

/* library OPSCRIPTCTRLLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_OPSCRIPTCTRLLib;

#ifndef ___IOpScriptEvents_DISPINTERFACE_DEFINED__
#define ___IOpScriptEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IOpScriptEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IOpScriptEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E93EE80D-2EA3-426D-9F9F-9CB235FB28D1")
    _IOpScriptEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IOpScriptEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IOpScriptEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IOpScriptEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IOpScriptEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IOpScriptEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IOpScriptEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IOpScriptEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IOpScriptEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IOpScriptEventsVtbl;

    interface _IOpScriptEvents
    {
        CONST_VTBL struct _IOpScriptEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IOpScriptEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IOpScriptEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IOpScriptEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IOpScriptEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IOpScriptEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IOpScriptEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IOpScriptEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IOpScriptEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_OpScript;

#ifdef __cplusplus

class DECLSPEC_UUID("7F770D4B-2A9C-41B5-8AD0-E217721E1DD9")
OpScript;
#endif
#endif /* __OPSCRIPTCTRLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


