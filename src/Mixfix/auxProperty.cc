const char*
Token::skipSortName(const char* tokenString, bool& parameterized)
{
  //
  //	A sort name may contain any characters except for
  //	  . : `[ `] `{ `} `,
  //	An exception is made for `{ `} `, iff they occur as part of
  //	a sort name constructed by:
  //	  <ps> ::= <regular name> | <ps> `{ <sl> `}
  //	  <sl> ::= <ps> | <sl> `, <ps>
  //
  //	A sort name can be terminated by \0, `, `] `}
  //	If a legal sort name followed by a legal terminator is seen, the
  //	addess of the first terminator charater is returned otherwise 0 is
  //	returned.
  //	In the first case, parameterized is true iff the sort name contained `{
  //	In the second case, parameterized is undefined.
  //
  parameterized = false;
  int depth = 0;
  bool seenName = false;
  for (const char* p = tokenString;; p++)
    {
      switch (*p)
	{
	case '\0':
	  return (seenName && depth == 0) ? p : 0;
	case '.':
	case ':':
	  return 0;
	case '`':
	  {
	    switch (*(p + 1))
	      {
	      case ']':
		return (seenName && depth == 0) ? p : 0;
	      case '{':
		{
		  if (seenName)
		    {
		      parameterized = true;
		      ++depth;
		      seenName = false;
		      ++p;
		    }
		  else
		    return 0;
		  break;
		}
	      case ',':
		{
		  if (seenName)
		    {
		      if (depth == 0)
			return p;
		      else
			{
			  seenName = false;
			  ++p;
			}
		    }
		  else
		    return 0;
		  break;
		}
	      case '}':
		{
		  if (seenName && depth > 0)
		    {
		      --depth;
		      ++p;
		    }
		  else
		    return 0;
		  break;
		}
	      case '[':
	      case '\0':
		return 0;
	      default:
		{
		  seenName = true;
		  ++p;
		  break;
		}
	      }
	    break;
	  }
	default:
	  {
	    seenName = true;
	    break;
	  }
	}
    }
}

int
Token::computeAuxProperty(const char* tokenString)
{
  {
    //
    //	Check for kind or sort.
    //
    const char* p = tokenString;
    if (*p++ == '`' && *p == '[')
      {
	for(;;)
	  {
	    bool dummy;
	    p = skipSortName(p, dummy);
	    if (p != 0 && *p++ == '`')
	      {
		switch (*p++)
		  {
		  case ']':
		    {
		      if (*p == '\0')
			return AUX_KIND;
		      break;
		    }
		  case ',':
		    continue;
		  }
	      }
	    break;
	  }
      }
    else
      {
	bool parameterized;
	const char* p = skipSortName(tokenString, parameterized);
	if (p != 0 && *p == '\0')
	  return parameterized ? AUX_STRUCTURED_SORT : AUX_SORT;
      }
  }
  {
    //
    //	Check for constant or variable.
    //
    int len = strlen(tokenString);
    for (int i = len - 1; i > 0; i--)
      {
	char c = tokenString[i];
	if (c == '.')
	  {
	    int t = computeAuxProperty(tokenString + i + 1);
	    if (t == AUX_SORT || t == AUX_STRUCTURED_SORT || t == AUX_KIND)
	      return AUX_CONSTANT;
	    break;
	  }
	else if (c == ':')
	  {
	    int t = computeAuxProperty(tokenString + i + 1);
	    if (t == AUX_SORT || t == AUX_STRUCTURED_SORT || t == AUX_KIND)
	      return AUX_VARIABLE;
	    break;
	  }
      }
  }
  return NONE;
}

Rope
Token::sortName(int code)
{
  const char* name = stringTable.name(code);
  if (auxProperty(code) != AUX_STRUCTURED_SORT)
    return Rope(name);

  Rope acc;
  for (;; ++name)
    {
      char c = *(name);
      switch (c)
	{
	case '\0':
	  return acc;
	case '`':
	  {
	    c = *(++name);
	    if (c != '{' && c != ',' && c != '}')
	      acc += '`';
	    break;
	  }
	}
      acc += c;
    }
}

void
Token::splitParameterizedSort(int code, Vector<int>& codes)
{
  Assert(auxProperty(code) == AUX_STRUCTURED_SORT, "called on " << stringTable.name(code));

  codes.clear();
  const char* name = stringTable.name(code);
  char* t = new char[strlen(name) + 1];
  char* p = strcpy(t, name);

  for (char* i = p;; ++i)
    {
      switch (*i)
	{
	case '\0':
	  {
	    Assert(p == i, "surplus characters " << p);
	    delete [] t;
	    return;
	  }
	case '`':
	  {
	    const char* sep;
	    switch (*(i + 1))
	      {
	      case '{':
		{
		  sep = "{";
		  break;
		}
	      case ',':
		{
		  sep = ",";
		  break;
		}
	      case '}':
		{
		  sep = "}";
		  break;
		}
	      default:
		continue;
	      }
	    if (i != p)
	      {
		*i = 0;
		codes.append(encode(p));
	      }
	    codes.append(encode(sep));
	    ++i;
	    p = i + 1;
	  }
	}
    }
}

void
Token::splitParameterList(int code, int& header, Vector<int>& parameters)
{
  Assert(auxProperty(code) == AUX_STRUCTURED_SORT, "called on " << stringTable.name(code));

  parameters.clear();
  const char* n = name(code);
  int len = strlen(n);
  char* t = new char[len + 1];
  char* p = strcpy(t, n);
  //
  // 	Search backwards for the '{' that marks the start of parameter list.
  //
  Assert(t[len - 1] == '}', "missing }");
  Assert(t[len - 2] == '`', "missing `");
  int depth = 1;
  for(p += len - 3;; --p)
    {
      Assert(p > t, "missing {");  // first character is never '{'
      if (*p == '{')
	{
	  --depth;
	  if (depth == 0)
	    break;
	}
      else if (*p == '}')
	++depth;
    }
  //
  //	Now split out header and parameters.
  //
  Assert(*(p - 1) == '`', "missing `");
  *(p - 1) = '\0';
  header = encode(t);
  do
    {
      ++p;
      char* s = p;
      for (p += 2;; ++p)
	{
	  if (*p == '{')
	    ++depth;
	  else if (*p == ',' && depth == 0)
	    break;
	  else if (*p == '}')
	    {
	      if (depth == 0)
		break;
	      else
		--depth;
	    }
	}
      Assert(*(p - 1) == '`', "missing `");
      *(p - 1) = '\0';
      parameters.append(encode(s));
    }
  while (*p != '}');
  delete [] t;
}

int
Token::joinParameterList(int header, const Vector<int>& parameters)
{
  Rope n(name(header));
  const char* sep = "`{";
  FOR_EACH_CONST(i, Vector<int>, parameters)
    {
      n += sep;
      sep = "`,";
      n += name(*i);
    }
  n += "`}";
  return ropeToCode(n);
}
