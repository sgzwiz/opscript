/* 
 * opscriptctrl.cpp : Implementation of DLL Exports and ATL initialization.
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
#include "resource.h"
#include <initguid.h>
#include "opscriptctrl.h"

#include "opscriptctrl_i.c"
#include "OpScript.h"


CComModule _Module;
extern int pascaldebug;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_OpScript, COpScript)
END_OBJECT_MAP()



/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
		if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_OPSCRIPTCTRLLib);
        DisableThreadLibraryCalls(hInstance);

        #if defined(_DEBUG) && defined(_DEBUG_PARSER)
          pascaldebug = 1;
          freopen("parser_states.txt","w",stderr);
        #endif
    }
		else if (dwReason == DLL_PROCESS_DETACH){
        _Module.Term();
#ifdef _DEBUG
				::_CrtDumpMemoryLeaks();
#endif
		}
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


