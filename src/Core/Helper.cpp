// Copyright (C) 2007 Miklós "TnS" Végvári
#include "stdafx.h"
#include "Helper.h"


CString CHelper::GetRegistryString(CRegKey& key, const char* pszName)
{
	char text[1024];
	DWORD length = sizeof(text) - 1;
	DWORD errorCode = key.QueryStringValue(pszName, text, &length);
	return errorCode == ERROR_SUCCESS ? text : CString();
}

DWORD CHelper::GetRegistryDword(CRegKey& key, const char* pszName, DWORD defaultValue)
{
	DWORD value = defaultValue;
	DWORD errorCode = key.QueryDWORDValue(pszName, value);
	return errorCode == ERROR_SUCCESS ? value : defaultValue;
}