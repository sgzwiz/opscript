/* 
 * interpreter.cpp - Implementation of the code interpreter.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#include <atlbase.h>
#include <atlconv.h>
#include <comutil.h>
#include "common.h"
#include "moduleObjects.h"
#include "interpreter.h"


#pragma warning(disable : 4267)
#pragma warning(disable : 4786)
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)


//************************************************************
//*** local definitions
//************************************************************

#define INST(x,y) y, #x , i_##x, (sizeof(BYTE) + (sizeof(operand)*y))
#define BIFUNCTION(x) "$_"#x, x

#define GET_STRDATA(x) ((string_data*)(((DWORD)(x))-offsetof(string_data,strdata)))

#define SP_BOOL(x) regs[ESP].pvchar--; bool x = (bool)(*regs[ESP].pvchar!=0);
#define SP_CHAR(x) regs[ESP].pvchar--; char x = *regs[ESP].pvchar;
#define SP_SHORT(x) regs[ESP].pvshort--; short x = *regs[ESP].pvshort;
#define SP_INT(x) regs[ESP].pvint--; int x = *regs[ESP].pvint; 
#define SP_FLOAT(x) regs[ESP].pfloat--; float x = *regs[ESP].pfloat;
#define SP_DOUBLE(x) regs[ESP].vint-=8; double x = *regs[ESP].pvdbl;
#define SP_CHAR_PTR(x) regs[ESP].vint-=4; char *x = (char*)(*regs[ESP].pvint);
#define SP_VOID_PTR(x) regs[ESP].pvint--; void *x = (void*)*regs[ESP].pvint;
#define SP_BSTR(x) regs[ESP].vint-=4; BSTR x = (BSTR)(*regs[ESP].pvint);
#define SP_IID(x) regs[ESP].vint-=sizeof(IID);IID x=*((IID*)regs[ESP].pvint);
#define SP_VARIANT(x) regs[ESP].vint-=sizeof(VARIANT); VARIANT x=*((VARIANT*)regs[ESP].pvint);
#define SP_IDISPATCH(x) regs[ESP].vint-=sizeof(void*); IDispatch *x=(IDispatch*)(*regs[ESP].pvint);
#define SP_IUNKNOWN(x) regs[ESP].vint-=sizeof(void*); IUnknown *x=(IUnknown*)(*regs[ESP].pvint);
#define SP_SHORT_PTR(x) regs[ESP].vint-=4; short *x = (short*)*regs[ESP].pvint;
#define SP_INT_PTR(x) regs[ESP].vint-=4; int *x = (int*)*regs[ESP].pvint;
#define SP_FLOAT_PTR(x) regs[ESP].vint-=4; float *x = (float*)*regs[ESP].pvint;
#define SP_DOUBLE_PTR(x) regs[ESP].vint-=4; double *x = (double*)*regs[ESP].pvint;
#define SP_CHAR_PTRPTR(x) regs[ESP].vint-=4; char **x = (char**)*regs[ESP].pvint;
#define SP_VOID_PTRPTR(x) regs[ESP].pvint--; void **x = (void**)*regs[ESP].pvint;
#define SP_BSTR_PTR(x) regs[ESP].vint-=4; BSTR *x = (BSTR*)*regs[ESP].pvint;
#define SP_VARIANT_PTR(x) regs[ESP].vint-=sizeof(4); VARIANT *x=*((VARIANT**)regs[ESP].pvint);
#define SP_RESULT(x,y) x &y=*((x*)(regs[ESP].vint-sizeof(x)));

union value
{
  char   *pvchar;
  short  *pvshort;
  int    *pvint;
  float  *pfloat;
  double *pvdbl;
};

struct code_location
{
  unsigned offset;
  std::vector<unsigned> update_locs;
};

typedef std::map<int,code_location> lbl_map;
typedef std::map<std::string,code_location> fn_map;
typedef std::map<std::string,int> builtin_map;


//************************************************************
//*** globals 
//************************************************************
struct _builtin_fn
{
  char *name;
  _BUILTIN_FN fnptr;
}
_builtin_fns[]=
{
  BIFUNCTION(read),
  BIFUNCTION(writeb),
  BIFUNCTION(writec),
  BIFUNCTION(writey),
  BIFUNCTION(writem),
  BIFUNCTION(writei),
  BIFUNCTION(writef),
  BIFUNCTION(writer),
  BIFUNCTION(writes),
  BIFUNCTION(writew),
  BIFUNCTION(writev),
  BIFUNCTION(writelnb),
  BIFUNCTION(writelnc),
  BIFUNCTION(writelny),
  BIFUNCTION(writelnm),
  BIFUNCTION(writelni),
  BIFUNCTION(writelnf),
  BIFUNCTION(writelnr),
  BIFUNCTION(writelns),
  BIFUNCTION(writelnw),
  BIFUNCTION(writelnv),
  BIFUNCTION(showmessage),
  BIFUNCTION(GetMem),
  BIFUNCTION(FreeMem),
  BIFUNCTION(mcpy),
  BIFUNCTION(mset),
  BIFUNCTION(dispcall),
  BIFUNCTION(interface_thunk),
  BIFUNCTION(safecall_thunk),
  BIFUNCTION(create_object),
  BIFUNCTION(createcomobjects),
  BIFUNCTION(createcomobjectetguid29),
  BIFUNCTION(varclear),
  BIFUNCTION(varcopy),
  BIFUNCTION(vartobool),
  BIFUNCTION(vartochar),
  BIFUNCTION(vartobyte),
  BIFUNCTION(vartoshort),
  BIFUNCTION(vartoint),
  BIFUNCTION(vartofloat),
  BIFUNCTION(vartodouble),
  BIFUNCTION(vartostring),
  BIFUNCTION(vartowidestr),
  BIFUNCTION(vartodispatch),
  BIFUNCTION(booltovar),
  BIFUNCTION(chartovar),
  BIFUNCTION(bytetovar),
  BIFUNCTION(shorttovar),
  BIFUNCTION(inttovar),
  BIFUNCTION(floattovar),
  BIFUNCTION(doubletovar),
  BIFUNCTION(stringtovar),
  BIFUNCTION(widestrtovar),
  BIFUNCTION(interfacetovar),
  BIFUNCTION(strtowide),
  BIFUNCTION(free_widestring),
  BIFUNCTION(_cpystr),
  BIFUNCTION(_cpyregstr),
  BIFUNCTION(_freestr),
  BIFUNCTION(_catstr),
  BIFUNCTION(_cpybstr),
  BIFUNCTION(_cpyregbstr),
  BIFUNCTION(_freebstr),
  BIFUNCTION(_catbstr),
  BIFUNCTION(_cnvbstrtostr),
  BIFUNCTION(_cnvstrtobstr),
  BIFUNCTION(_catbstrtostr),
  BIFUNCTION(_catstrtobstr),
  BIFUNCTION(inci),
  BIFUNCTION(deci),
};

struct instruction
{
  BYTE  count;
  char *name;

  BYTE  inst;
  BYTE  size;
}
insts[] =
{
  INST(mov,2),
  INST(push,1),
  INST(pop,1),
  INST(push_rgs,1),
  INST(pop_rgs,1),
  INST(mov_ds,2),
  INST(mv_sg,1),
  // arithmetic
  INST(add,2),
  INST(sub,2),
  INST(mult,2),
  INST(div,2),
  INST(mod,2),
  INST(inc,1),
  INST(dec,1),
  // flow control
  INST(call,1),
  INST(calln,1),
  INST(callu,2),
  INST(jmp,1),
  INST(ret,1),
  // boolean functinality
  INST(lt,2),
  INST(le,2),
  INST(ge,2),
  INST(gt,2),
  INST(eq,2),
  INST(ne,2),
  // bit operations
  INST(shl,2),
  INST(shr,2),
  INST(and,2),
  INST(or,2),
  INST(xor,2),
};

//*******************
// built in functions 
//*******************
void read(_register *regs)
{
  int type;
  regs[ESP].pvint--;
  type=*regs[ESP].pvint;
  regs[ESP].pvint--;
  
  switch(type)
  {
    case 0:
      scanf("%c",*regs[ESP].pvint);
    break;
    case 1:
      scanf("%i",*regs[ESP].pvint);
    break;
    case 2:
      scanf("%f",*regs[ESP].pvint);
    break;
  }
}

void writeb(_register *regs)
{
  SP_BOOL(val);
  printf((val)?"True":"False");
}

void writec(_register *regs)
{
  SP_CHAR(val);
  printf("%c",val);
}

void writey(_register *regs)
{
  SP_CHAR(val);
  printf("%i",val);
}

void writem(_register *regs)
{
  SP_SHORT(val);
  printf("%i",val);
}

void writei(_register *regs)
{
  SP_INT(val);
  printf("%i",val);
}

void writef(_register *regs)
{
  SP_FLOAT(val);
  printf("%f",val);
}

void writer(_register *regs)
{
  SP_DOUBLE(val);
  printf("%f",val);
}

void writes(_register *regs)
{
  SP_CHAR_PTR(val);
  string_data *str;

  if(val)
  {
    str=GET_STRDATA(val);
    printf("%s",val);
    if(str->types)
      free(str);
  }
}

void writew(_register *regs)
{
  SP_BSTR(val);
#ifdef _WIN32
  if(val)
  {
    wprintf(L"%s",val);
    SysFreeString(val);
  }
#endif
}

#ifdef _WIN32
void writev(_register *regs)
{
  SP_VARIANT(v);
  VARIANT v2;

  VariantInit(&v2);
  if(S_OK==VariantChangeType(&v2,&v,VARIANT_ALPHABOOL|VARIANT_LOCALBOOL,VT_BSTR))
  {
    wprintf(L"%s",v2.bstrVal);
    VariantClear(&v2);
  }
}
#endif

void writelnb(_register *regs)
{
  SP_BOOL(val);
  printf((val)?"True\n":"Talse\n");
}

void writelnc(_register *regs)
{
  SP_CHAR(val);
  printf("%c\n",val);
}

void writelny(_register *regs)
{
  SP_CHAR(val);
  printf("%i\n",val);
}

void writelnm(_register *regs)
{
  SP_SHORT(val);
  printf("%i\n",val);
}

void writelni(_register *regs)
{
  SP_INT(val);
  printf("%i\n",val);
}

void writelnf(_register *regs)
{
  SP_FLOAT(val);
  printf("%f\n",val);
}

void writelnr(_register *regs)
{
  SP_DOUBLE(val);
  printf("%f\n",val);
}

void writelns(_register *regs)
{
  SP_CHAR_PTR(val);
  string_data *str;

  if(val)
  {
    str=GET_STRDATA(val);
    printf("%s\n",val);
    if(str->types)
      free(str);
  }
  else
    printf("\n");
}

void writelnw(_register *regs)
{
  SP_BSTR(val);
#ifdef _WIN32
  if(val)
  {
    wprintf(L"%s\n",val);
    SysFreeString(val);
  }
  else
#endif
    printf("\n");

}

#ifdef _WIN32
void writelnv(_register *regs)
{
  SP_VARIANT(v);
  VARIANT v2;

  VariantInit(&v2);
  if(S_OK==VariantChangeType(&v2,&v,VARIANT_ALPHABOOL|VARIANT_LOCALBOOL,VT_BSTR))
  {
    wprintf(L"%s\n",v2.bstrVal);
    VariantClear(&v2);
  }
  else
    wprintf(L"\n");
}

void showmessage(_register *regs)
{
  SP_CHAR_PTR(val);
  string_data *str;

  if(val)
  {
    str=GET_STRDATA(val);
    MessageBox(GetForegroundWindow(),val,"",MB_OK);
    if(str->types)
       free(str);
  }
  else
    MessageBox(GetForegroundWindow(),"","",MB_OK);
}
#endif

void GetMem(_register *regs)
{
  SP_INT(size);
  SP_VOID_PTRPTR(p);
  *p = calloc(size,1);
}

void FreeMem(_register *regs)
{
  SP_VOID_PTR(p);
  free(p);
}

void mcpy(_register *regs)
{
  SP_VOID_PTR(pdest);
  SP_VOID_PTR(psrc);
  SP_INT(len);
  
  memcpy(pdest,psrc,len);
}

void mset(_register *regs)
{
  SP_VOID_PTR(pdest);
  SP_INT(value);
  SP_INT(len);
  
  memset(pdest,value,len);
}

#ifdef _WIN32
void dispcall(_register *regs)
{
  USES_CONVERSION;
  VARIANT *pVar;
  SP_INT(flags);
  SP_INT(param_count);
  
  if(param_count)
  {
    regs[ESP].vint -= sizeof(VARIANT)*param_count;  
    pVar = (VARIANT*)regs[ESP].vint;
  }
  else
    pVar = 0;
  
  SP_BSTR(name);
  SP_IDISPATCH(val);
  SP_RESULT(VARIANT,result);

  DISPID id;
  EXCEPINFO info;
  unsigned int argerr;
  DISPPARAMS params;
  DISPID putid=DISPID_PROPERTYPUT;
  HRESULT hr;
  //USES_CONVERSION;
  
  VariantInit(&result);

  params.cArgs = param_count;
  params.rgvarg = pVar;

  if(flags!=DISPATCH_PROPERTYPUT)
  {

    params.cNamedArgs = 0;
    params.rgdispidNamedArgs = 0;
  }
  else
  {
    params.cNamedArgs = 1;
    params.rgdispidNamedArgs = &putid;
  }  

  hr = val->GetIDsOfNames(IID_NULL,&name, 1, LOCALE_USER_DEFAULT, &id );

  if(FAILED(hr))
  {
    throw CRuntimeErr(1,"Interpreter","member not found");
  }

  hr = val->Invoke(id,IID_NULL,LOCALE_USER_DEFAULT,flags,&params,&result,&info,&argerr);
 
  if(FAILED(hr))
  {
		bstr_t source;
		bstr_t description;
        
    switch(hr)
    {
      case DISP_E_EXCEPTION:
				source=(info.bstrSource)?info.bstrSource:L"IDispatch::Invoke()";
				description=(info.bstrDescription)?info.bstrDescription:L"Call failed";

				throw CRuntimeErr(1,W2A(source),W2A(description));
      break;
			default:
				throw CRuntimeErr(1,"dispcall","IDispatch::Invoke() failed");
    }
  }
}

void createcomobjectetguid29(_register *regs)
{
  SP_IID(id);
  SP_RESULT(void*,res);
  res = 0;

  CoCreateInstance(id,0,CLSCTX_ALL,IID_IUnknown,&res);
}

void createcomobjects(_register *regs)
{
  USES_CONVERSION;
  IID id;

  SP_CHAR_PTR(val);
  SP_RESULT(void*,res);

  res = 0;
  
  string_data *str;

  if(val)
  {
    str=GET_STRDATA(val);

    CLSIDFromProgID(A2W(val),&id);
    CoCreateInstance(id,0,CLSCTX_ALL,IID_IUnknown,&res);
    
    if(str->types)
      free(str);
  }
}

void create_object(_register *regs)
{
  USES_CONVERSION;
  regs[ESP].pvint--;
  char *s_id=(char*)(*regs[ESP].pvint);
  regs[ESP].pvint--;
  char *s_inter=(char*)(*regs[ESP].pvint);
  void *obj;
  GUID id;
  GUID inter;
  
  if(s_id[0]=='{')
    IIDFromString(A2W(s_id),&id);
  else
  {
    HRESULT hr = CLSIDFromProgID(A2W(s_id),&id);
    if(FAILED(hr))
    {
      *(regs[ESP].pvint-1)=0;
      return;
    }
  }
  
  IIDFromString(A2W(s_inter),&inter);

  CoCreateInstance(id,0,CLSCTX_INPROC_SERVER,inter,&obj);
  *(regs[ESP].pvint-1)=(int)obj;
}

void safecall_thunk(_register *regs)
{
  unsigned index;
  unsigned ret_size;
  unsigned stack_size;
  void *object;
  void *stack;
  unsigned *presult;
  
  regs[ESP].pvint--;
  index=(*regs[ESP].pvint)*4;
  regs[ESP].pvint--;
  ret_size=*regs[ESP].pvint;
  regs[ESP].pvint--;
  stack_size=*regs[ESP].pvint;
  regs[ESP].vint-=stack_size;
  object=(void*)*regs[ESP].pvint;
  stack=regs[ESP].pvint;
  presult=(unsigned*)(((unsigned)stack)-4);

  __asm
  {
    push ecx
    push edx
    push eax
    push esi
    push edi
    sub  esp,stack_size
    mov  edi,esp
    mov  esi,stack
    mov  ecx,stack_size
    rep movsb
    mov eax,object
    mov edx,[eax]
    add edx,index
    call [edx]
    mov edx,presult
    mov [edx],eax
    pop edi
    pop esi
    pop eax
    pop edx
    pop ecx
  }

  if(FAILED(*presult))
  {
    USES_CONVERSION;
    IErrorInfo *pErrInfo;
    BSTR pSource;
    BSTR pDesc;

    GetErrorInfo(0,&pErrInfo);
    pErrInfo->GetDescription(&pDesc);
    pErrInfo->GetSource(&pSource);

    RELEASE_INTERFACE(pErrInfo);

    throw CRuntimeErr(1,W2A(pSource),W2A(pDesc));
  }
}

void interface_thunk(_register *regs)
{
  unsigned index;
  unsigned ret_size;
  unsigned stack_size;
  void *object;
  void *stack;
  unsigned *presult;
  
  regs[ESP].pvint--;
  index=(*regs[ESP].pvint)*4;
  regs[ESP].pvint--;
  ret_size=*regs[ESP].pvint;
  regs[ESP].pvint--;
  stack_size=*regs[ESP].pvint;
  regs[ESP].vint-=stack_size;
  object=(void*)*regs[ESP].pvint;
  stack=regs[ESP].pvint;
  presult=(unsigned*)(((unsigned)stack)-4);

  __asm
  {
    push ecx
    push edx
    push eax
    push esi
    push edi
    sub  esp,stack_size
    mov  edi,esp
    mov  esi,stack
    mov  ecx,stack_size
    rep movsb
    mov eax,object
    mov edx,[eax]
    add edx,index
    call [edx]
    mov edx,presult
    mov [edx],eax
    pop edi
    pop esi
    pop eax
    pop edx
    pop ecx
  }
}

void interfacetovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_IUNKNOWN(psrc);

  // try to get dispatch...
  psrc->QueryInterface(IID_IDispatch,(void**)&pdest->pdispVal);

  if(pdest->pdispVal)
    pdest->vt = VT_DISPATCH;
  else
  {
    // get unknown...
    pdest->punkVal = psrc;
    pdest->punkVal->AddRef();
    pdest->vt = VT_UNKNOWN;
  }
}

void varcmp(_register *regs)
{
  SP_VARIANT_PTR(pvar1);
  SP_VARIANT_PTR(pvar2);
  SP_RESULT(int,res);
}

void varclear(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  VariantClear(psrc);
}

void varcopy(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_VARIANT_PTR(psrc);

   VariantCopy(pdest,psrc);
}

void vartobool(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(char,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_BOOL);
  val=(v.boolVal)?1:0;
  VariantClear(&v);
}

void vartochar(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(char,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_I1);
  val=v.cVal;
  VariantClear(&v);
}

void vartobyte(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(char,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_I1);
  val=v.cVal;
  VariantClear(&v);
}

void vartoshort(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(short,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_I2);
  val=v.iVal;
  VariantClear(&v);
}

void vartoint(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(int,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_I4);
  val=v.intVal;
  VariantClear(&v);
}

void vartofloat(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(float,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_R4);
  val=v.fltVal;
  VariantClear(&v);
}

void vartodouble(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(double,val);
  VARIANT v;
  VariantInit(&v);
  VariantChangeType(&v,psrc,0,VT_R8);
  val=v.dblVal;
  VariantClear(&v);
}

void vartostring(_register *regs)
{
  USES_CONVERSION;
  SP_VARIANT_PTR(psrc);
  SP_RESULT(char *,val);
  VARIANT v;
  VariantInit(&v);
  if(S_OK==VariantChangeType(&v,psrc,0,VT_BSTR))
  {
      string_data *str;
      int len;

      len=(int)wcslen(v.bstrVal);
      if(len>0)
      {
        str=(string_data*)malloc(sizeof(string_data)+len);
        str->len = len;
        str->delphi_ref = -1;
        str->types = 1;
        memcpy(str->strdata,W2A(v.bstrVal),len+1);      
        val=str->strdata;
      }
      else
        val = 0;
  }
  else
    val = 0;
  VariantClear(&v);
}

void vartowidestr(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(BSTR,val);
  VARIANT v;
  VariantInit(&v);
  if(S_OK==VariantChangeType(&v,psrc,0,VT_BSTR))
    val=v.bstrVal;
  else
    val=0;
}

void vartodispatch(_register *regs)
{
  SP_VARIANT_PTR(psrc);
  SP_RESULT(IDispatch*,val);
  

  switch(psrc->vt)
  {
    case VT_DISPATCH:
      val = psrc->pdispVal;
    break;
    case VT_UNKNOWN:
			throw CRuntimeErr(-1,"Interpreter","Runtime Error");
    break;
    // default...
  }
}

void booltovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_CHAR(src);
  //VariantClear(pdest);
  pdest->vt = VT_BOOL;
  pdest->boolVal = (src)?VARIANT_TRUE:VARIANT_FALSE;
}

void chartovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_CHAR(src);
  //VariantClear(pdest);
  pdest->vt = VT_I1;
  pdest->cVal = src;
}

void bytetovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_CHAR(src);
  //VariantClear(pdest);
  pdest->vt = VT_I1;
  pdest->cVal = src;
}


void shorttovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_SHORT(src);
  //VariantClear(pdest);
  pdest->vt = VT_I2;
  pdest->iVal = src;
}

void inttovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_INT(psrc);
  //VariantClear(pdest);
  pdest->vt = VT_I4;
  pdest->intVal = psrc;
}

void floattovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_FLOAT(psrc);
  //VariantClear(pdest);
  pdest->vt = VT_R4;
  pdest->fltVal = psrc;
}

void doubletovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_DOUBLE(psrc);
  //VariantClear(pdest);
  pdest->vt = VT_R8;
  pdest->dblVal = psrc;
}

void stringtovar(_register *regs)
{
  USES_CONVERSION;
  SP_VARIANT_PTR(pdest);
  SP_CHAR_PTR(psrc);
  //VariantClear(pdest);
  pdest->vt = VT_BSTR;
  pdest->bstrVal= SysAllocString(A2W(psrc));
}

void widestrtovar(_register *regs)
{
  SP_VARIANT_PTR(pdest);
  SP_BSTR(psrc);
  //VariantClear(pdest);
  pdest->vt = VT_BSTR;
  pdest->bstrVal= SysAllocString(psrc);
}

#endif

void strtowide(_register *regs)
{
  USES_CONVERSION;
  char *str;
  BSTR *pbstr;

  regs[ESP].pvint--;
  pbstr=(BSTR*)(*regs[ESP].pvint);
  regs[ESP].pvint--;
  str=(char*)(*regs[ESP].pvint);

  *pbstr=SysAllocString(A2W(str));   
}

void free_widestring(_register *regs)
{
  BSTR pbstr;

  regs[ESP].pvint--;
  pbstr=(BSTR)(*regs[ESP].pvint);
 
  SysFreeString(pbstr);
}

void strcmp(_register *regs)
{
  SP_CHAR_PTR(pstr1);
  SP_CHAR_PTR(pstr2);
  SP_RESULT(int,res);
}

void wstrcmp(_register *regs)
{
  SP_BSTR(pstr1);
  SP_BSTR(pstr2);
  SP_RESULT(int,res);
}

void _cpystr(_register *regs)
{
  SP_CHAR_PTRPTR(p);
  string_data *pdata,*ptmpdata;
  
  if(*p)
  {
    pdata=GET_STRDATA(*p);

    if(pdata->types)
    {
      ptmpdata=(string_data*)malloc(sizeof(string_data)+pdata->len);
      memcpy(ptmpdata,pdata,pdata->len + sizeof(string_data));
      *p=ptmpdata->strdata;
    }
  }
}

void _cpyregstr(_register *regs)
{
  SP_INT(r);
  string_data *pdata,*ptmpdata;
  
  if(regs[r].pvchar)
  {
    pdata=GET_STRDATA(regs[r].vint);

    if(pdata->types)
    {
      ptmpdata=(string_data*)malloc(sizeof(string_data)+pdata->len);
      memcpy(ptmpdata,pdata,pdata->len + sizeof(string_data));
      regs[r].pvchar=ptmpdata->strdata;
    }
  }
}

void _freestr(_register *regs)
{
  SP_CHAR_PTR(p);

  if(p)
  {
    string_data *pdata=GET_STRDATA(p);
    if(pdata->types)
      free(pdata);
  }
}

void _catstr(_register *regs)
{
  SP_CHAR_PTR(p);
  SP_INT(r);
  int len;
  
  string_data *ptmpdata=0;
  string_data *pdata=0;

  // add the info to the current string...
  if(regs[r].pvchar)
  {
    pdata = GET_STRDATA(regs[r].vint);

    if(p)
    {
      len = strlen(p);
      ptmpdata=(string_data*)malloc(sizeof(string_data)+pdata->len + len);
      
      memcpy(ptmpdata,pdata,pdata->len + sizeof(string_data));
      memcpy(&ptmpdata->strdata[pdata->len],p,len+1);
      ptmpdata->len += len;
      ptmpdata->types = 1;

      if(pdata->types)
        free(pdata);
    }
    else
      ptmpdata=pdata;
  }
  else
  {
    // allocate a new one...
    if(p)
    {
      len=strlen(p);
      ptmpdata=(string_data*)malloc(sizeof(string_data)+len);
      ptmpdata->len = len;
      ptmpdata->delphi_ref = -1;
      ptmpdata->types = 1;
      memcpy(ptmpdata->strdata,p,len+1);      
    }
  }

  if(ptmpdata)
    regs[r].pvchar=ptmpdata->strdata;
}

void _cpybstr(_register *regs)
{
  SP_BSTR_PTR(p);

  if(*p)
    *p = SysAllocString(*p);
}

void _cpyregbstr(_register *regs)
{
  SP_INT(r);
  if(regs[r].pvchar)
    regs[r].pvchar=(char*)SysAllocString((BSTR)regs[r].pvchar);
}

void _freebstr(_register *regs)
{
  SP_BSTR(p);

  if(p)
    SysFreeString(p);
}

void _catbstr(_register *regs)
{

  SP_BSTR(p);
  SP_INT(r);

#ifdef _WIN32
  size_t len1,len2;
  BSTR regstr;
  BSTR pstr=0;
  WCHAR *tmp_str;
  
  // add the info to the current string...
  if(regs[r].pvchar)
  {
    if(p)
    {
      regstr = (BSTR)regs[r].pvchar;
      len1 = wcslen(regstr);
      len2 = wcslen(p);
      tmp_str=(BSTR)malloc((len1+len2+1)*sizeof(WCHAR));
      swprintf(tmp_str,L"%s%s",regstr,p);
      SysFreeString(regstr);
      pstr=SysAllocString(tmp_str);
      free(tmp_str);
    }
  }
  else
  {
    // allocate a new one...
    if(p)
      pstr = SysAllocString(p);
  }

  if(pstr)
    regs[r].pvchar=(char*)pstr;
#endif
}

void _cnvbstrtostr(_register *regs)
{
  USES_CONVERSION;
  SP_CHAR_PTRPTR(ps);
  SP_INT(fval);
  BSTR *pb=(BSTR*)ps;
  char *str;
  size_t len;
  string_data *pdata;

  if(*ps)
  {
    str = W2A(*pb);
    len = strlen(str);
    pdata = (string_data*)malloc(sizeof(string_data) + len );
    pdata->types = 1;
    pdata->len = len;
    pdata->delphi_ref = -1;
    strcpy(pdata->strdata,str);
    if(fval)
      SysFreeString(*pb);
    *ps = pdata->strdata;
  }
}

void _cnvstrtobstr(_register *regs)
{
  USES_CONVERSION;
  SP_CHAR_PTRPTR(ps);
  SP_INT(fval);
  BSTR *pb=(BSTR*)ps;
  BSTR  pout;
  string_data *pdata;

  if(*ps)
  {
    pout = SysAllocString(A2W(*ps));
    if(fval)
    {
      pdata = GET_STRDATA(*ps);
      if(pdata->types)
        free(pdata);
    }
    *pb = pout;
  }
}

void _catbstrtostr(_register *regs)
{
  USES_CONVERSION;
  SP_BSTR(p);
  SP_INT(r);
  int len;
#ifdef _WIN32  
  string_data *ptmpdata=0;
  string_data *pdata=0;

  // add the info to the current string...
  if(regs[r].pvchar)
  {
    pdata = GET_STRDATA(regs[r].vint);

    if(p)
    {
      len = wcslen(p);
      ptmpdata=(string_data*)malloc(sizeof(string_data)+pdata->len + len);
      
      memcpy(ptmpdata,pdata,pdata->len + sizeof(string_data));
      memcpy(&ptmpdata->strdata[pdata->len],W2A(p),len+1);
      ptmpdata->len += len;
      ptmpdata->types = 1;

      if(pdata->types)
        free(pdata);
    }
    else
      ptmpdata=pdata;
  }
  else
  {
    // allocate a new one...
    if(p)
    {
      len=wcslen(p);
      ptmpdata=(string_data*)malloc(sizeof(string_data)+len);
      ptmpdata->len = len;
      ptmpdata->delphi_ref = -1;
      ptmpdata->types = 1;
      memcpy(ptmpdata->strdata,W2A(p),len+1);      
    }
  }

  if(ptmpdata)
    regs[r].pvchar=ptmpdata->strdata;
#endif
}

void _catstrtobstr(_register *regs)
{
  USES_CONVERSION;
  SP_CHAR_PTR(ps);
  SP_INT(r);

#ifdef _WIN32

  int len1,len2;
  BSTR regstr;
  BSTR pstr=0;
  WCHAR *tmp_str;
  
  // add the info to the current string...
  if(regs[r].pvchar)
  {
    if(ps)
    {
      regstr = (BSTR)regs[r].pvchar;
      len1 = wcslen(regstr);
      len2 = strlen(ps);
      tmp_str=(BSTR)malloc((len1+len2+1)*sizeof(WCHAR));
      swprintf(tmp_str,L"%s%s",regstr,A2W(ps));
      SysFreeString(regstr);
      pstr=SysAllocString(tmp_str);
      free(tmp_str);
    }
  }
  else
  {
    // allocate a new one...
    if(ps)
      pstr = SysAllocString(A2W(ps));
  }

  if(pstr)
    regs[r].pvchar=(char*)pstr;
#endif
}

void inci(_register *regs)
{
  SP_INT_PTR(i);
  (*i)++;
}

void deci(_register *regs)
{
  SP_INT_PTR(i);
  (*i)--;
}

int get_instruction(const char *name)
{
  int i;
  for(i=0;i<sizeof(insts)/sizeof(instruction);i++)
    if(strcmp(insts[i].name,name)==0)
      return i;
  return -1;
}

operand translate_op(_instruction *pin, BYTE opnum, char *rgd)
{
  operand op;
  char *p;;
  char *offset=0;
  char  sign;
  char *pb;
  p = strdup(rgd);
  pb = p;
  
  memset(&op,0,sizeof(op));

  if(*p=='[')
  {
    op.regop.deref=1;
    p++;
    p[strlen(p)-1]=0;
  }

  switch(*p)
  {
    case 'c':
      op.regop.size = 1;
    break;
    case 's':
      op.regop.size = 2;
    break;
    case 'i':
      op.regop.size = 4;
    break;
    case 'f':
      op.regop.size = 6; 
    break;
    case 'd':
      op.regop.size = 8;
    break;
    default:
      char *s;
      pin->const_=1;
      op.value = strtol (p,&s,0);
      free(pb);
      return op;
  }

  p++;

  // get the register...
  switch(*p)
  {
    case 'r':
      sign=*(p+4);
      *(p+4)=0;
      offset=sign?p+5:0;
      op.regop.reg = atoi(p+1);
    break;
  }

  if(offset)
  {
    op.regop.offset = atoi(offset);
    op.regop.offset *= (sign=='-')?-1:1;
  }

  free(pb);
  return op;
}

void convert(BYTE to_type, BYTE &from_type, value &pv, _register &rg)
{
  switch(to_type)
  {
    case 1:
      switch(from_type)
      {
        case 2:rg.vc=(char)*pv.pvshort;break;
        case 4:rg.vc=(char)*pv.pvint;break;
        case 6:rg.vc=(char)*pv.pfloat;break;
        case 8:rg.vc=(char)*pv.pvdbl;break;
      }
    break;
    case 2:
      switch(from_type)
      {
        case 1:rg.vshort=(short)*pv.pvchar;break;
        case 4:rg.vshort=(short)*pv.pvint;break;
        case 6:rg.vshort=(short)*pv.pfloat;break;
        case 8:rg.vshort=(short)*pv.pvdbl;break;
      }
    break;
    case 4:
      switch(from_type)
      {
        case 1:rg.vint=(int)*pv.pvchar;break;
        case 2:rg.vint=(int)*pv.pvshort;break;
        case 6:rg.vint=(int)*pv.pfloat;break;
        case 8:rg.vint=(int)*pv.pvdbl;break;
      }
    break;
    case 6:
      switch(from_type)
      {
        case 1:rg.vf=(float)*pv.pvchar;break;
        case 2:rg.vf=(float)*pv.pvshort;break;
        case 4:rg.vf=(float)*pv.pvint;break;
        case 8:rg.vf=(float)*pv.pvdbl;break;
      }
    break;
    case 8:
      switch(from_type)
      {
        case 1:rg.vdbl=(double)*pv.pvchar;break;
        case 2:rg.vdbl=(double)*pv.pvshort;break;
        case 4:rg.vdbl=(double)*pv.pvint;break;
        case 6:rg.vdbl=(double)*pv.pfloat;break;
      }
    break;
  }
  pv.pvint=&rg.vint;
  from_type=to_type;
} 

BYTE byte_val(char *p)
{
  return (((p[0]<'A')?p[0]-'0':p[0]-'A'+10)*16)+((p[1]<'A')?p[1]-'0':p[1]-'A'+10);
}

void compile_unit(Cmodule *cu)
{
  char *type;
  char *inst;
  char *op1;
  char *op2;
  char buffer[2000];
  int  op;
  BYTE *cs;
  BYTE *ds;
  lbl_map labels;
  //fn_map functions;
  builtin_map bi_fns;
  std::vector<int> constants;
  std::vector<int> const_sizes;
  _instruction *ip;
  tlist<sym_procedure*> pl;
  sym_procedure *pproc;
  unsigned global_size;
  Cmodule *pUnit;

  lbl_map::iterator lit;
  fn_map::iterator fit;
  
  int  *location;
  unsigned  i;
  unsigned  size;
  int   main_location=-1;

  // initialize the builin array...
  //_builtin_fns
  for(i=0;i<(sizeof(_builtin_fns)/sizeof(_builtin_fn));i++) 
    bi_fns[_builtin_fns[i].name] = i;

  size_t code_size=0;
  size_t data_size=0;
  cs=0;
  ds=0;

  // assemble the file...
  FILE *f = fopen(cu->get_asmname(),"rt");

#ifdef _DEBUG_ASSEMBLY_
  int line = -1;
#endif

  while(fgets(buffer,sizeof(buffer),f))
  { 
    // get rid of the new line...
    buffer[strlen(buffer)-1]=0;
#ifdef _DEBUG_ASSEMBLY_
    cu->_m_asm_lines.push_back(buffer);
    line++;
#endif    
    type = strtok(buffer," ");
    inst = strtok(0,", ");
    op1  = strtok(0,", ");
    op2  = strtok(0,", ");
  
    // see if it's a valid type...
    if(type)
    {
      switch(type[0])
      {
         case 'g':
           // size of the global variable section...
           global_size=atoi(inst);
           data_size=global_size + 4 * (cu->get_procedure_count() + 2);
           ds=(BYTE*)calloc(data_size,1);
         break;
         case '$':
          // function definition...
          // reduce labels for the last function...
          for(lit=labels.begin();lit!=labels.end();lit++)
          {
            //location=(int
            for(i=0;i<(*lit).second.update_locs.size();i++)
            {
              location=((int*)(((unsigned)cs)+(*lit).second.update_locs[i]));
              *location=(*lit).second.offset;
            }
          }
          
          pproc = cu->get_procedure(type);

          if(pproc)
          {
            pproc->proc_offset = code_size;
            location=((int*)(((unsigned)ds)+global_size+(4*pproc->proc_gnumber)));
            *location=code_size;
          }

          labels.clear();
          if(strcmp(type,"$@__main")==0)
            main_location=code_size;
        break;
    case 'i':
      // instruction...
#ifdef _DEBUG_ASSEMBLY_
      cu->_m_asm_line_map.insert(IntPair(code_size,line));
#endif
      op = get_instruction(inst);
      cs=(BYTE*)realloc(cs,code_size+insts[op].size);
      ip=((_instruction*)(&cs[code_size]));
      ip->inst = op;
          switch(op)
          {
            case i_jmp:
              // i should only be able to jump to a const location...
              ip->const_ = 1;
              labels[atoi(&op1[1])].update_locs.push_back(code_size+1);
            break;
            case i_calln:
              ip->const_ = 1;
              ip->dest.value = bi_fns[op1];
            break;
            default:
              ip->const_ = 0;
              if(op1)
              {
                switch(op1[0])
                {
                  case '$':
                    pproc=cu->get_procedure(op1);
                    pUnit=cu->get_manager()->get_unit(pproc->unit_index);
                    ip->dest.regop.deref = 1;
                    ip->dest.regop.offset = pUnit->global.size+(pproc->proc_gnumber*4);
                    ip->dest.regop.reg = EDS;
                    ip->dest.regop.size = 4;
                  break;
                  case '@':
                    ip->dest.regop.deref = 0;
                    ip->dest.regop.offset = constants[atoi(&op1[1])-1];
                    ip->dest.regop.reg = EDS;
                    ip->dest.regop.size = 4;
                  break;
                  case '^':
                    ip->dest.regop.deref = 1;
                    ip->dest.regop.offset = constants[atoi(&op1[1])-1];
                    ip->dest.regop.reg = EDS;
                    ip->dest.regop.size = const_sizes[atoi(&op1[1])-1];
                  break;
                  default:
                    ip->dest = translate_op(ip,0,op1);
                }
              }
     
        if(op2)
              {
                switch(op2[0])
                {
                  case '$':
                    pproc=cu->get_procedure(op2);
                    pUnit=cu->get_manager()->get_unit(pproc->unit_index);
                    ip->src.regop.deref = 1;
                    ip->dest.regop.offset = pUnit->global.size+(pproc->proc_gnumber*4);
                    ip->src.regop.reg = EDS;
                    ip->src.regop.size = 4;
                  break;
                  case '@':
                    ip->src.regop.deref = 0;
                    ip->src.regop.offset = constants[atoi(&op2[1])-1];
                    ip->src.regop.reg = EDS;
                    ip->src.regop.size = 4;
                  break;
                  case '^':
                    ip->src.regop.deref = 1;
                    ip->src.regop.offset = constants[atoi(&op2[1])-1];
                    ip->src.regop.reg = EDS;
                    ip->src.regop.size = const_sizes[atoi(&op2[1])-1];;
                  break;
                  default:
                    ip->src  = translate_op(ip,1,op2);
                }
              }
          }
          code_size+=insts[op].size;
        break;
        case 'l':
          labels[atoi(&inst[1])].offset = code_size;
        break;
        case 'c':
          // constant definition...
          constants.push_back(data_size);
          switch(op1[0])
          {
            case 'h':
              // it's a hex constant...
              size=strlen(&op1[1])/2;
              ds=(BYTE*)realloc(ds,data_size+size);
              const_sizes.push_back(size);
              for(i=0;i<size;i++)
                ds[data_size+i]=byte_val(&op1[1+i*2]);
              data_size+=size;
            break;
            case 'i':
              ds=(BYTE*)realloc(ds,data_size+4);
              *((int*)(&ds[data_size]))=atoi(&op1[1]);
              const_sizes.push_back(4);
              data_size+=4;
            break;
            case 'c':
              ds=(BYTE*)realloc(ds,data_size+1);
              *((char*)(&ds[data_size]))=atoi(&op1[1]);
              data_size+=1;
              const_sizes.push_back(1);
            break;
            case 'd':
              ds=(BYTE*)realloc(ds,data_size+8);
              *((double*)(&ds[data_size]))=atof(&op1[1]);
              data_size+=8;
              const_sizes.push_back(8);
            break;
            case 's':
              ds=(BYTE*)realloc(ds,data_size+2);
              *((short*)(&ds[data_size]))=atoi(&op1[1]);              
              data_size+=2;
              const_sizes.push_back(2);
            break;
            case 'f':
              ds=(BYTE*)realloc(ds,data_size+4);
              *((float*)(&ds[data_size]))=(float)atof(&op1[1]);
              data_size+=4;
              const_sizes.push_back(6);
            break;
          }
        break;
      }
    }
  }

  fclose(f);

#ifndef _DEBUG
	// delete the file if we are in release mode. 
  DeleteFile(cu->get_asmname());
#endif

  // reduce labels for the last function...
  for(lit=labels.begin();lit!=labels.end();lit++)
  {
    for(i=0;i<(*lit).second.update_locs.size();i++)
    {
      location=((int*)(((unsigned)cs)+(*lit).second.update_locs[i]));
      *location=(*lit).second.offset;
    }
  }
  
  cu->data_size = data_size;
  cu->code_size = code_size;
  cu->main_offset = main_location;
  cu->cs=cs;
  cu->ds=ds;
  cu->set_compiled(true);
}

void init_cpu(cpu_state *p)
{
  p->cs = 0;
  p->ds = 0;
  p->stack = (BYTE*)calloc(1000,1);
  p->saved_count = 0;
  p->saved_regs = 0;
}

void clear_cpu(cpu_state *p)
{
  free(p->stack);
  
  if(p->saved_regs)
    free(p->saved_regs);
  
  p->stack = 0;
  p->saved_regs=0;
  p->saved_count=0;
}

void exec_function( unsigned offset, cpu_state *p )
{
  _instruction *ip = (_instruction*)(p->cs + offset);
  _register *regs = p->regs;
  bool b;
  BYTE *cs = p->cs;

  // set ip to 0. this is how we know to return...
  *regs[ESP].pvint=0;
  regs[ESP].pvint++;
  // -- push ebp
  *regs[ESP].pvint=regs[EBP].vint;
  regs[ESP].pvint++;
  // ebp = esp
  regs[EBP].vint=regs[ESP].vint;

  try
  {
    while(1)
    {
      value val1,val2;
      _reg_desc *op1,*op2;
      _register  tmp1;
      BYTE size;

      if(!ip)
        return;

      switch(insts[ip->inst].count)
      {
        case 2:
          op2=&ip->src.regop;
          op1=&ip->dest.regop;

          // convert value 2 if the types are different
          if(ip->const_)
          {
            val2.pvchar=(char*)&(((operand*)op2)->value);
            size=4;
          }
          else if(!op2->deref&&op2->offset)
          {
            tmp1.vint = regs[op2->reg].vint+op2->offset;
            val2.pvchar=&tmp1.vc;
            size=4;
          }
          else
          {
            val2.pvchar=(op2->deref)?regs[op2->reg].pvchar+op2->offset:&regs[op2->reg].vc;
            size=op2->size;
          }
          val1.pvchar=(op1->deref)?regs[op1->reg].pvchar+op1->offset:&regs[op1->reg].vc;
        
          if(size!=op1->size)
            convert(op1->size,size,val2,tmp1);
        break;
        case 1:
          op1=&ip->dest.regop;
          if(ip->const_)
          {
            val1.pvchar=(char*)&((operand*)op1)->value;
            size=4;
          }
          else if(!op1->deref && op1->offset)
          {
            tmp1.vint = regs[op1->reg].vint+op1->offset;
            val1.pvchar=&tmp1.vc;
            size=4;
          }
          else
          {
            size=op1->size;
            val1.pvchar=(op1->deref)?regs[op1->reg].pvchar+op1->offset:&regs[op1->reg].vc;
          }
        break;
      }

  #ifdef _DEBUG_ASSEMBLY_
      int curr_line;
      int curr_offset;
      const char *curr_text;
      std::map<int,int>::iterator it;
      Cmodule *pCurrUnit = p->units[p->curr_unit];
      curr_offset = ((int)ip) - ((int)pCurrUnit->cs);
      it = pCurrUnit->_m_asm_line_map.find(curr_offset);
      if(it != pCurrUnit->_m_asm_line_map.end())
      {
        curr_line = (*it).second;
        curr_text = pCurrUnit->_m_asm_lines[curr_line].c_str();
      }
  #endif

      switch(ip->inst)
      {
          case i_mov:
            switch(size){
              case 1:*val1.pvchar=*val2.pvchar;break;
              case 2:*val1.pvshort=*val2.pvshort;break;
              case 4:*val1.pvint=*val2.pvint;break;
              case 6:*val1.pfloat=*val2.pfloat;break;
              case 8:*val1.pvdbl=*val2.pvdbl;break;            
            }
          break;
          case i_push:
            switch(size){
              case 1:*regs[1].pvchar=*val1.pvchar;regs[1].pvchar+=1;break;
              case 2:*regs[1].pvshort=*val1.pvshort;regs[1].pvchar+=2;break;
              case 4:*regs[1].pvint=*val1.pvint;regs[1].pvchar+=4;break;
              case 6:*regs[1].pfloat=*val1.pfloat;regs[1].pvchar+=4;break;
              case 8:*regs[1].pvdbl=*val1.pvdbl;regs[1].pvchar+=8;break;            
            }
          break;
          case i_pop:
            switch(size){
              case 1:regs[1].pvchar-=1;*val1.pvchar=*regs[1].pvchar;break;
              case 2:regs[1].pvchar-=2;*val1.pvshort=*regs[1].pvshort;break;
              case 4:regs[1].pvchar-=4;*val1.pvint=*regs[1].pvint;break;
              case 6:regs[1].pvchar-=4;*val1.pfloat=*regs[1].pfloat;break;
              case 8:regs[1].pvchar-=8;*val1.pvdbl=*regs[1].pvdbl;break;            
            }
          break;
          case i_push_rgs:
            if(*val1.pvint)
            {
              p->saved_regs=(_register*)realloc(p->saved_regs,sizeof(_register)*(p->saved_count+*val1.pvint));
              memcpy(&p->saved_regs[p->saved_count],&regs[3],sizeof(_register)*(*val1.pvint));
              p->saved_count+=(*val1.pvint);
            }
          break;
          case i_pop_rgs:
            if(*val1.pvint)
            {
              p->saved_count-=(*val1.pvint);
              memcpy(&regs[3],&p->saved_regs[p->saved_count],sizeof(_register)*(*val1.pvint));
            }            
          break;
          case i_mov_ds:
            // there's only one size...
            *val1.pvint=(int)p->units[*val2.pvint]->ds;
          break;
          case i_mv_sg:
            p->curr_unit=*val1.pvint;
            p->cs=(BYTE*)p->units[*val1.pvint]->cs;
            p->ds=(BYTE*)p->units[*val1.pvint]->ds;
            cs = p->cs;
            regs[EDS].vint = (int)p->ds;
          break;
          case i_add:
            switch(size){
              case 1:(*val1.pvchar)+=(*val2.pvchar);break;
              case 2:(*val1.pvshort)+=(*val2.pvshort);break;
              case 4:(*val1.pvint)+=(*val2.pvint);break;
              case 6:(*val1.pfloat)+=(*val2.pfloat);break;
              case 8:(*val1.pvdbl)+=(*val2.pvdbl);break;            
            }
          break;
          case i_sub:
            switch(size){
              case 1:(*val1.pvchar)-=(*val2.pvchar);break;
              case 2:(*val1.pvshort)-=(*val2.pvshort);break;
              case 4:(*val1.pvint)-=(*val2.pvint);break;
              case 6:(*val1.pfloat)-=(*val2.pfloat);break;
              case 8:(*val1.pvdbl)-=(*val2.pvdbl);break;            
            }
          break;
          case i_mult:
            switch(size){
              case 1:(*val1.pvchar)*=(*val2.pvchar);break;
              case 2:(*val1.pvshort)*=(*val2.pvshort);break;
              case 4:(*val1.pvint)*=(*val2.pvint);break;
              case 6:(*val1.pfloat)*=(*val2.pfloat);break;
              case 8:(*val1.pvdbl)*=(*val2.pvdbl);break;            
            }
          break;
          case i_div:
            switch(size){
              case 1:(*val1.pvchar)/=(*val2.pvchar);break;
              case 2:(*val1.pvshort)/=(*val2.pvshort);break;
              case 4:(*val1.pvint)/=(*val2.pvint);break;
              case 6:(*val1.pfloat)/=(*val2.pfloat);break;
              case 8:(*val1.pvdbl)/=(*val2.pvdbl);break;            
            }
          break;
          case i_mod:
            switch(size){
              case 1:(*val1.pvchar)%=(*val2.pvchar);break;
              case 2:(*val1.pvshort)%=(*val2.pvshort);break;
              case 4:(*val1.pvint)%=(*val2.pvint);break;
            }
          break;
          case i_inc:
            switch(size){
              case 1:(*val1.pvchar)+=1;break;
              case 2:(*val1.pvshort)+=1;break;
              case 4:(*val1.pvint)+=1;break;
              case 6:(*val1.pfloat)+=1;break;
              case 8:(*val1.pvdbl)+=1;break;            
            }
          break;
          case i_dec:
            switch(size){
              case 1:(*val1.pvchar)-=1;break;
              case 2:(*val1.pvshort)-=1;break;
              case 4:(*val1.pvint)-=1;break;
              case 6:(*val1.pfloat)-=1;break;
              case 8:(*val1.pvdbl)-=1;break;            
            }
          break;
          case i_calln:
            _builtin_fns[*val1.pvint].fnptr(regs);
          break;
          case i_call:
            // we want to return to the next instruction,
            // otherwise we'll be caught in an infinite loop...
            // -- push ip
            *regs[ESP].pvint=(int)(((BYTE*)ip)+insts[ip->inst].size);
            regs[ESP].pvint++;
            // -- push ebp
            *regs[ESP].pvint=regs[EBP].vint;
            regs[ESP].pvint++;
            // ebp = esp
            regs[EBP].vint=regs[ESP].vint;
            // finally, turn control over to the new function...
            ip=(_instruction*)(cs+*val1.pvint);
            continue;
          break;
          case i_callu:
            // we want to return to the next instruction,
            // otherwise we'll be caught in an infinite loop...
            // -- push ip
            *regs[ESP].pvint=(int)(((BYTE*)ip)+insts[ip->inst].size);
            regs[ESP].pvint++;
            // -- push ebp
            *regs[ESP].pvint=regs[EBP].vint;
            regs[ESP].pvint++;
            // ebp = esp
            regs[EBP].vint=regs[ESP].vint;
            // because we are calling another unit, switch control to that unit
            // we have to adjust the value in val1...
            p->curr_unit=*val2.pvint;
            val1.pvint = (int*)((reinterpret_cast<int>(val1.pvint))-(int)p->ds);
            p->cs=(BYTE*)p->units[*val2.pvint]->cs;
            p->ds=(BYTE*)p->units[*val2.pvint]->ds;
            val1.pvint = ((int*)(((int)val1.pvint)+(int)p->ds));
            cs = p->cs;
            regs[EDS].vint = (int)p->ds;
            // finally, turn control over to the new function...
            ip=(_instruction*)(cs+*val1.pvint);
            continue;
          break;
          case i_jmp:
            ip=(_instruction*)(cs+*val1.pvint);
            continue;
          break;
          case i_ret:
            // esp = ebp
            regs[ESP].vint=regs[EBP].vint;
            // -- pop ebp
            regs[ESP].pvint--;
            regs[EBP].vint=*regs[ESP].pvint;
            // -- pop ip
            regs[ESP].pvint--;
            ip=(_instruction*)(*regs[ESP].pvint);
            // clear the current stack if requested...
            regs[ESP].vint-=(*val1.pvint);
            continue;
          break;
          // boolean functinality
          case i_lt:
            switch(size){
              case 1:b = *val1.pvchar < *val2.pvchar;break;
              case 2:b = *val1.pvshort < *val2.pvshort;break;
              case 4:b = *val1.pvint < *val2.pvint;break;
              case 6:b = *val1.pfloat < *val2.pfloat;break;
              case 8:b = *val1.pvdbl < *val2.pvdbl;break;            
            }
            if(b)ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);
          break;
          case i_le:
            switch(size){
              case 1:b = *val1.pvchar <= *val2.pvchar;break;
              case 2:b = *val1.pvshort <= *val2.pvshort;break;
              case 4:b = *val1.pvint <= *val2.pvint;break;
              case 6:b = *val1.pfloat <= *val2.pfloat;break;
              case 8:b = *val1.pvdbl <= *val2.pvdbl;break;            
            }
            if(b)ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);
          break;
          case i_ge:
            switch(size){
              case 1:b = *val1.pvchar >= *val2.pvchar;break;
              case 2:b = *val1.pvshort >= *val2.pvshort;break;
              case 4:b = *val1.pvint >= *val2.pvint;break;
              case 6:b = *val1.pfloat >= *val2.pfloat;break;
              case 8:b = *val1.pvdbl >= *val2.pvdbl;break;            
            }
            if(b)ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);        
          break;
          case i_gt:
            switch(size){
              case 1:b = *val1.pvchar > *val2.pvchar;break;
              case 2:b = *val1.pvshort > *val2.pvshort;break;
              case 4:b = *val1.pvint > *val2.pvint;break;
              case 6:b = *val1.pfloat > *val2.pfloat;break;
              case 8:b = *val1.pvdbl > *val2.pvdbl;break;            
            }
            if(b)ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);
          break;
          case i_eq:
            switch(size){
              case 1:b = *val1.pvchar == *val2.pvchar;break;
              case 2:b = *val1.pvshort == *val2.pvshort;break;
              case 4:b = *val1.pvint == *val2.pvint;break;
              case 6:b = *val1.pfloat == *val2.pfloat;break;
              case 8:b = *val1.pvdbl == *val2.pvdbl;break;            
            }
            if(b)ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);
          break;
          case i_ne:
            switch(size){
              case 1:b = *val1.pvchar != *val2.pvchar;break;
              case 2:b = *val1.pvshort != *val2.pvshort;break;
              case 4:b = *val1.pvint != *val2.pvint;break;
              case 6:b = *val1.pfloat != *val2.pfloat;break;
              case 8:b = *val1.pvdbl != *val2.pvdbl;break;            
            }
            if(b)ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);
          break;
          // bit operations
          case i_shl:
						throw CRuntimeErr(-1,"Interpreter", "Not Implemented i_shl");
          break;
          case i_shr:
						throw CRuntimeErr(-1,"Interpreter", "Not Implemented i_shr");
          break;
          case i_and:
						throw CRuntimeErr(-1,"Interpreter", "Not Implemented i_and");
          break;
          case i_or:
						throw CRuntimeErr(-1,"Interpreter", "Not Implemented i_or");
          break;
          case i_xor:
						throw CRuntimeErr(-1,"Interpreter", "Not Implemented i_xor");
          break;
          default: 
            throw CRuntimeErr(-1,"Interpreter", "Invalid Instruction");
      }
      ip=(_instruction*)(((BYTE*)ip)+insts[ip->inst].size);
    }
  }
  catch(CRuntimeErr err)
  {
    p->units[p->curr_unit]->handle_error(&err);
  }
}
