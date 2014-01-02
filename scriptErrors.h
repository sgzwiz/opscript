/* 
 * scriptErrors.h - Declaration of error reporting facilities. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __scriptErrors_h__
#define __scriptErrors_h__

class CScriptErr
{
public:
  CScriptErr();
  virtual ~CScriptErr();
  int _m_line;
  std::string _m_message;
  std::string _m_source;
};

class CRuntimeErr : public CScriptErr
{
public:
  CRuntimeErr(int line, const char *src, const char *msg);
};

class CSyntaxErr : public CScriptErr
{
public:
  CSyntaxErr(int line, const char *msg,...);
};

class CCompilerErr : public CScriptErr
{
public:
  CCompilerErr(int line, const char *msg,...);
};

class CParserErr : public CScriptErr
{
public:
  CParserErr(int line, const char *msg,...);
};

class scriptErrorHandler
{
public:
  virtual void handle_error(CScriptErr *perr) = 0;
};

#endif//__script_errors_h_