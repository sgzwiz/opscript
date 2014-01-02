

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IOpScript,0xC5267F85,0x759C,0x41EA,0x88,0x72,0x5A,0x03,0x7E,0x1B,0x51,0x45);


MIDL_DEFINE_GUID(IID, LIBID_OPSCRIPTCTRLLib,0xB4DF1D75,0x5469,0x4227,0xA8,0x2E,0xA2,0xE3,0xDC,0xC1,0x2E,0x40);


MIDL_DEFINE_GUID(IID, DIID__IOpScriptEvents,0xE93EE80D,0x2EA3,0x426D,0x9F,0x9F,0x9C,0xB2,0x35,0xFB,0x28,0xD1);


MIDL_DEFINE_GUID(CLSID, CLSID_OpScript,0x7F770D4B,0x2A9C,0x41B5,0x8A,0xD0,0xE2,0x17,0x72,0x1E,0x1D,0xD9);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



