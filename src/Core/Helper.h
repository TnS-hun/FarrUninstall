// Copyright (C) 2007 Miklós "TnS" Végvári
#pragma once


class CHelper
{
public:
	static CString GetRegistryString(CRegKey& key, const char* pszName);
	static DWORD GetRegistryDword(CRegKey& key, const char* pszName, DWORD defaultValue = 0);
};