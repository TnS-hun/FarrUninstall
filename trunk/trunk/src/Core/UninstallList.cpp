// Copyright (C) 2007 Miklós "TnS" Végvári
#include "stdafx.h"
#include "UninstallList.h"


void CUninstallList::MakeList()
{
	m_Items.RemoveAll();

	CRegKey key;
	key.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	MakeList(HKEY_LOCAL_MACHINE, key);
}


void CUninstallList::MakeList(HKEY rootKey, CRegKey& key)
{
	char name[1024];
	for ( DWORD index = 0; true; ++index)
	{
		DWORD nameLength = sizeof(name) - 1;
		FILETIME lastWriteTime;
		LONG errorCode = key.EnumKey(index, name, &nameLength, &lastWriteTime);
		if ( errorCode == ERROR_NO_MORE_ITEMS )
			break;

		CUninstallItem item(rootKey, name, false);
		if ( item.IsValid() )
			m_Items.Add(item);
	}
}