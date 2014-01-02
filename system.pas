unit system;

interface

type
  HRESULT = type Longint;  { from WTYPES.H }
  
  SYSUINT = LongWord;
  WordBool = boolean;
  PChar = ^char;
  PWidechar = ^Widechar;
  PWChar = PWidechar;
  Real = double;
  TDateTime = double;
  float = single;
  short = smallint;
  Variant = OleVariant;
  
  tagSAFEARRAYBOUND = record
    cElements: Longint;
    lLbound: Longint;
  end;

  TSafeArrayBound = tagSAFEARRAYBOUND;

  PSafeArrayBound = ^TSafeArrayBound;

  SAFEARRAYBOUND = TSafeArrayBound;
  
  tagSAFEARRAY = record
    cDims: Word;
    fFeatures: Word;
    cbElements: Longint;
    cLocks: Longint;
    pvData: Pointer;
    rgsabound: array[0..0] of TSafeArrayBound;
  end;

  TSafeArray = tagSAFEARRAY;
  
  PSafeArray = ^TSafeArray;
  
  SAFEARRAY = TSafeArray;

  TOleEnum = integer;
  
  TGUID = packed record
    D1: LongWord;
    D2: Word;
    D3: Word;
    D4: array[0..7] of Byte;
  end;

const
  // TypeLibrary Major and minor versions
  stdoleMajorVersion = 2;
  stdoleMinorVersion = 0;

  LIBID_stdole : TGUID = '{00020430-0000-0000-C000-000000000046}';

  IID_IUnknown : TGUID = '{00000000-0000-0000-C000-000000000046}';
  IID_IDispatch : TGUID = '{00020400-0000-0000-C000-000000000046}';
  IID_IEnumVARIANT : TGUID = '{00020404-0000-0000-C000-000000000046}';
  IID_IFont : TGUID = '{BEF6E002-A874-101A-8BBA-00AA00300CAB}';
  DIID_Font : TGUID = '{BEF6E003-A874-101A-8BBA-00AA00300CAB}';
  CLASS_StdFont : TGUID = '{0BE35203-8F91-11CE-9DE3-00AA004BB851}';
  IID_IPicture : TGUID = '{7BF80980-BF32-101A-8BBB-00AA00300CAB}';
  DIID_Picture : TGUID = '{7BF80981-BF32-101A-8BBB-00AA00300CAB}';
  CLASS_StdPicture : TGUID = '{0BE35204-8F91-11CE-9DE3-00AA004BB851}';
  DIID_FontEvents : TGUID = '{4EF6100A-AF88-11D0-9846-00C04FC29993}';

type

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary                    
// *********************************************************************//
  IUnknown = interface;
  IDispatch = interface;
  IEnumVARIANT = interface;
  IFont = interface;
  FontDisp = dispinterface;
  IPicture = interface;
  PictureDisp = dispinterface;
  FontEventsDisp = dispinterface;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library                       
// (NOTE: Here we map each CoClass to its Default Interface)              
// *********************************************************************//
  StdFont = FontDisp;
  StdPicture = PictureDisp;

// Constants for enum OLE_TRISTATE
type
  OLE_TRISTATE = TOleEnum;
const
  Unchecked = $00000000;
  Checked = $00000001;
  Gray = $00000002;

// Constants for enum LoadPictureConstants
type
  LoadPictureConstants = TOleEnum;
const
  Default_ = $00000000;
  Monochrome = $00000001;
  VgaColor = $00000002;
  Color = $00000004;

// *********************************************************************//
// Declaration of structures, unions and aliases.                         
// *********************************************************************//

type
  DISPPARAMS = packed record
    rgvarg : ^OleVariant;
    rgdispidNamedArgs : ^Integer;
    cArgs : SYSUINT;
    cNamedArgs : SYSUINT;
  end;

  EXCEPINFO = packed record
    wCode : Word;
    wReserved : Word;
    bstrSource : WideString;
    bstrDescription : WideString;
    bstrHelpFile : WideString;
    dwHelpContext : LongWord;
    pvReserved : ^pointer;
    pfnDeferredFillIn : ^pointer;
    scode : HResult;
  end;

  OLE_COLOR = LongWord;
  OLE_XPOS_PIXELS = Integer;
  OLE_YPOS_PIXELS = Integer;
  OLE_XSIZE_PIXELS = Integer;
  OLE_YSIZE_PIXELS = Integer;
  OLE_XPOS_HIMETRIC = Integer;
  OLE_YPOS_HIMETRIC = Integer;
  OLE_XSIZE_HIMETRIC = Integer;
  OLE_YSIZE_HIMETRIC = Integer;
  OLE_XPOS_CONTAINER = real;
  OLE_YPOS_CONTAINER = real;
  OLE_XSIZE_CONTAINER = real;
  OLE_YSIZE_CONTAINER = real;
  OLE_HANDLE = Integer;
  OLE_OPTEXCLUSIVE = WordBool;
  OLE_CANCELBOOL = WordBool;
  OLE_ENABLEDEFAULTBOOL = WordBool;
  FONTNAME = WideString;
  FONTSIZE = Currency;
  FONTBOLD = WordBool;
  FONTITALIC = WordBool;
  FONTUNDERSCORE = WordBool;
  FONTSTRIKETHROUGH = WordBool;
  IFontDisp = Font;
  IPictureDisp = Picture;
  IFontEventsDisp = FontEvents;

type
// *********************************************************************//
// Interface:   IUnknown
// Flags:       
// GUID:       {00000000-0000-0000-C000-000000000046}
// *********************************************************************//
  IUnknown = interface
    ['{00000000-0000-0000-C000-000000000046}']
    function QueryInterface( var riid : TGUID; out ppvObj : pointer) : HResult; stdcall;
    function AddRef : LongWord; stdcall;
    function Release : LongWord; stdcall;
  end;

// *********************************************************************//
// Interface:   IDispatch
// Flags:       
// GUID:       {00020400-0000-0000-C000-000000000046}
// *********************************************************************//
  IDispatch = interface(IUnknown)
    ['{00020400-0000-0000-C000-000000000046}']
    function GetTypeInfoCount( out pctinfo : SYSUINT) : HResult; stdcall;
    function GetTypeInfo( itinfo : SYSUINT; lcid : LongWord; out pptinfo : pointer) : HResult; stdcall;
    function GetIDsOfNames( var riid : TGUID; var rgszNames : smallint; cNames : SYSUINT; lcid : LongWord; out rgdispid : Integer) : HResult; stdcall;
    function Invoke( dispidMember : Integer; var riid : TGUID; lcid : LongWord; wFlags : Word; var pdispparams : DISPPARAMS; out pvarResult : OleVariant; out pexcepinfo : EXCEPINFO; out puArgErr : SYSUINT) : HResult; stdcall;
  end;

// *********************************************************************//
// Interface:   IEnumVARIANT
// Flags:       
// GUID:       {00020404-0000-0000-C000-000000000046}
// *********************************************************************//
  IEnumVARIANT = interface(IUnknown)
    ['{00020404-0000-0000-C000-000000000046}']
    function Next( celt : LongWord; var rgvar : OleVariant; out pceltFetched : LongWord) : HResult; stdcall;
    function Skip( celt : LongWord) : HResult; stdcall;
    function Reset : HResult; stdcall;
    function Clone( out ppenum : IEnumVARIANT) : HResult; stdcall;
  end;

// *********************************************************************//
// Interface:   IFont
// Flags:       
// GUID:       {BEF6E002-A874-101A-8BBA-00AA00300CAB}
// Helpstring: Font Object
// *********************************************************************//
  IFont = interface(IUnknown)
    ['{BEF6E002-A874-101A-8BBA-00AA00300CAB}']
    function Get_Name( out pname : WideString) : HResult; stdcall;
    procedure Set_Name( pname : WideString); stdcall;
    function Get_Size( out psize : Currency) : HResult; stdcall;
    procedure Set_Size( psize : Currency); stdcall;
    function Get_Bold( out pbold : WordBool) : HResult; stdcall;
    procedure Set_Bold( pbold : WordBool); stdcall;
    function Get_Italic( out pitalic : WordBool) : HResult; stdcall;
    procedure Set_Italic( pitalic : WordBool); stdcall;
    function Get_Underline( out punderline : WordBool) : HResult; stdcall;
    procedure Set_Underline( punderline : WordBool); stdcall;
    function Get_Strikethrough( out pstrikethrough : WordBool) : HResult; stdcall;
    procedure Set_Strikethrough( pstrikethrough : WordBool); stdcall;
    function Get_Weight( out pweight : shortint) : HResult; stdcall;
    procedure Set_Weight( pweight : shortint); stdcall;
    function Get_Charset( out pcharset : shortint) : HResult; stdcall;
    procedure Set_Charset( pcharset : shortint); stdcall;
    function Get_hFont( out phfont : OLE_HANDLE) : HResult; stdcall;
    function Clone( out ppfont : IFont) : HResult; stdcall;
    function IsEqual( pfontOther : IFont) : HResult; stdcall;
    function SetRatio( cyLogical : Integer; cyHimetric : Integer) : HResult; stdcall;
    function AddRefHfont( hFont : OLE_HANDLE) : HResult; stdcall;
    function ReleaseHfont( hFont : OLE_HANDLE) : HResult; stdcall;
  end;

// *********************************************************************//
// Interface:   FontDisp
// Flags:       
// GUID:       {BEF6E003-A874-101A-8BBA-00AA00300CAB}
// *********************************************************************//
  FontDisp = dispinterface
    ['{BEF6E003-A874-101A-8BBA-00AA00300CAB}']
  end;

// *********************************************************************//
// Interface:   IPicture
// Flags:       
// GUID:       {7BF80980-BF32-101A-8BBB-00AA00300CAB}
// Helpstring: Picture Object
// *********************************************************************//
  IPicture = interface(IUnknown)
    ['{7BF80980-BF32-101A-8BBB-00AA00300CAB}']
    function Get_Handle( out phandle : OLE_HANDLE) : HResult; stdcall;
    function Get_hPal( out phpal : OLE_HANDLE) : HResult; stdcall;
    function Get_Type_( out ptype : shortint) : HResult; stdcall;
    function Get_Width( out pwidth : OLE_XSIZE_HIMETRIC) : HResult; stdcall;
    function Get_Height( out pheight : OLE_YSIZE_HIMETRIC) : HResult; stdcall;
    function Render( hdc : Integer; x : Integer; y : Integer; cx : Integer; cy : Integer; xSrc : OLE_XPOS_HIMETRIC; ySrc : OLE_YPOS_HIMETRIC; cxSrc : OLE_XSIZE_HIMETRIC; cySrc : OLE_YSIZE_HIMETRIC; prcWBounds : pointer) : HResult; stdcall;
    procedure Set_hPal( phpal : OLE_HANDLE); stdcall;
    function Get_CurDC( out phdcOut : Integer) : HResult; stdcall;
    function SelectPicture( hdcIn : Integer; out phdcOut : Integer; out phbmpOut : OLE_HANDLE) : HResult; stdcall;
    function Get_KeepOriginalFormat( out pfkeep : WordBool) : HResult; stdcall;
    procedure Set_KeepOriginalFormat( pfkeep : WordBool); stdcall;
    function PictureChanged : HResult; stdcall;
    function SaveAsFile( pstm : pointer; fSaveMemCopy : WordBool; out pcbSize : Integer) : HResult; stdcall;
    function Get_Attributes( out pdwAttr : Integer) : HResult; stdcall;
    function SetHdc( hdc : OLE_HANDLE) : HResult; stdcall;
  end;

// *********************************************************************//
// Interface:   PictureDisp
// Flags:       
// GUID:       {7BF80981-BF32-101A-8BBB-00AA00300CAB}
// *********************************************************************//
  PictureDisp = dispinterface
    ['{7BF80981-BF32-101A-8BBB-00AA00300CAB}']
    procedure Render( hdc : Integer; x : Integer; y : Integer; cx : Integer; cy : Integer; xSrc : OLE_XPOS_HIMETRIC; ySrc : OLE_YPOS_HIMETRIC; cxSrc : OLE_XSIZE_HIMETRIC; cySrc : OLE_YSIZE_HIMETRIC; prcWBounds : pointer); dispid 6;
  end;

// *********************************************************************//
// Interface:   FontEventsDisp
// Flags:       
// GUID:       {4EF6100A-AF88-11D0-9846-00C04FC29993}
// *********************************************************************//
  FontEventsDisp = dispinterface
    ['{4EF6100A-AF88-11D0-9846-00C04FC29993}']
    procedure FontChanged( PropertyName : WideString); dispid 9;
  end;


// *********************************************************************//
// Implemented Interfaces:
// [default] Font
// [default] FontEvents
//           IFont
// *********************************************************************//
  CoStdFont = class
    class function Create : FontDisp;
    class function CreateRemote( const MachineName : string ) : FontDisp;
  end;

// *********************************************************************//
// Implemented Interfaces:
// [default] Picture
//           IPicture
// *********************************************************************//
  CoStdPicture = class
    class function Create : PictureDisp;
    class function CreateRemote( const MachineName : string ) : PictureDisp;
  end;
  
  
  TObject = class
  public
    constructor Create;
    procedure Free;
  (*
    class function InitInstance(Instance: Pointer): TObject;
    procedure CleanupInstance;
    function ClassType: TClass;
    class function ClassName: ShortString;
    class function ClassNameIs(const Name: string): Boolean;
    class function ClassParent: TClass;
    class function ClassInfo: Pointer;
    class function InstanceSize: Longint;
    class function InheritsFrom(AClass: TClass): Boolean;
    class function MethodAddress(const Name: ShortString): Pointer;
    class function MethodName(Address: Pointer): ShortString;
    function FieldAddress(const Name: ShortString): Pointer;
    function GetInterface(const IID: TGUID; out Obj): Boolean;
    class function GetInterfaceEntry(const IID: TGUID): PInterfaceEntry;
    class function GetInterfaceTable: PInterfaceTable;
    function SafeCallException(ExceptObject: TObject;
      ExceptAddr: Pointer): HResult; virtual;
    procedure AfterConstruction; virtual;
    procedure BeforeDestruction; virtual;
    procedure Dispatch(var Message); virtual;
    procedure DefaultHandler(var Message); virtual;
    class function NewInstance: TObject; virtual;
    procedure FreeInstance; virtual;
    destructor Destroy; virtual;
  *)
  end;

//****************************************************************************
// Input / Output Functions
//****************************************************************************
procedure writeln( val : boolean);overload;
procedure writeln( val : char ); overload;
procedure writeln( val : byte ); overload;
procedure writeln( val : short ); overload;
procedure writeln( val : integer );overload;
procedure writeln( val : float ); overload;
procedure writeln( val : double ); overload;
procedure writeln( val : string ); overload;
procedure writeln( val : widestring); overload;
procedure writeln( val : OleVariant ); overload;
procedure write( val : boolean ); overload;
procedure write( val : byte ); overload;
procedure write( val : char ); overload;
procedure write( val : short ); overload;
procedure write( val : integer );overload;
procedure write( val : float );overload;
procedure write( val : double ); overload;
procedure write( val : string ); overload;
procedure write( val : widestring); overload;
procedure write( val : OleVariant); overload;
procedure ShowMessage( msg : string );

//****************************************************************************
//
//****************************************************************************
procedure inc( var i : integer ); overload;
procedure dec( var i : integer ); overload;

//****************************************************************************
// Variant Support
//****************************************************************************
function VarCmp( var v1, v2 : OleVariant ) : integer;
procedure VarClear( var v1 : OleVariant );
procedure VarCopy( var v1, v2 : OleVariant );
function VarToBool( var v1 : OleVariant ) : boolean;
function VarToChar( var v1 : OleVariant ) : char; 
function VarToByte( var v1 : OleVariant ) : byte;
function VarToShort( var v1 : OleVariant ) : smallint;
function VarToInt( var v1 : OleVariant ) : integer; 
function VarToFloat( var v1 : OleVariant ) : float;
function VarToDouble( var v1 : OleVariant ) : double; 
function VarToString( var v1 : OleVariant ) : string;
function VarToWideStr( var v1 : OleVariant ) : widestring;
function VarToDispatch( var v1 : OleVariant ) : IDispatch;
procedure BoolToVar( val : boolean; var ovar : olevariant );
procedure CharToVar( val : char; var ovar: OleVariant );
procedure ByteToVar( val : byte; var ovar : Olevariant);
procedure ShortToVar( val : smallint; var ovar: OleVariant );
procedure IntToVar( val : integer; var ovar: OleVariant );
procedure FloatToVar( val : float; var ovar: OleVariant );
procedure DoubleToVar( val : double; var ovar: OleVariant );
procedure StringToVar( val : string; var ovar: OleVariant );
procedure WideStrToVar( val : WideString; var ovar: OleVariant );
procedure InterfaceToVar( val : IUnknown; var ovar : OleVariant );
procedure GetMem( size : integer; var myvar : pointer);

//****************************************************************************
// COM Functions
//****************************************************************************
function CreateCOMObject(progid : string) : IUnknown; overload;
function CreateCOMObject(iid : TGUID) : IUnknown; overload;
function CreateRemoteCOMObject(machine : string; progid : string) : IUnknown; overload;
function CreateRemoteCOMObject(machine : string; iid : TGUID) : IUnknown; overload;

implementation

class function CoStdFont.Create: FontDisp;
begin
  Result := CreateComObject(CLASS_StdFont) as FontDisp;
end;

class function CoStdFont.CreateRemote(const MachineName: string): FontDisp;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_StdFont) as FontDisp;
end;

class function CoStdPicture.Create: PictureDisp;
begin
  Result := CreateComObject(CLASS_StdPicture) as PictureDisp;
end;

class function CoStdPicture.CreateRemote(const MachineName: string): PictureDisp;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_StdPicture) as PictureDisp;
end;


end.

