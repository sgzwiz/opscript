/*
 * common.h 
 * Headers used throughout project. Most headers are here (instead on the stdafx.h)
 * since some of the modules don't support precompiled headers.
 * 
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __common_h__
#define __common_h__

#pragma warning(disable : 4786 )

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <hash_map>

#ifndef BYTE 
#define BYTE unsigned char
#endif
#ifndef WORD 
#define WORD unsigned short
#endif

#define RELEASE_INTERFACE(x) do{ if(x){x->Release();x=0;} }while(0)


#endif//_COMMON_FILES_H_