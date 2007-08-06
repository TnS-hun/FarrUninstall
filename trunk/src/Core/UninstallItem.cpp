// Copyright (C) 2007 Miklós "TnS" Végvári
#include "stdafx.h"
#include "UninstallItem.h"
#include "Helper.h"


CUninstallItem::CUninstallItem()
{
}

CUninstallItem::CUninstallItem(HKEY rootKey, const char* pszUninstallEntryKeyName, bool needHotFixes)
: m_UninstallEntryKeyName(pszUninstallEntryKeyName)
{
	CString path = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
	path += m_UninstallEntryKeyName;
	CRegKey uninstallEntryKey;
	uninstallEntryKey.Open(rootKey, path);

	m_DisplayName = CHelper::GetRegistryString(uninstallEntryKey, "DisplayName");
	if ( m_DisplayName.IsEmpty() )
		return;
	m_UninstallCommand = CHelper::GetRegistryString(uninstallEntryKey, "UninstallString");
	if ( m_UninstallCommand.IsEmpty() )
		return;

	if ( !needHotFixes )
		InvalidateIfHotFix(uninstallEntryKey);
	if ( !IsValid() )
		return;

	bool windowsInstaller = CHelper::GetRegistryDword(uninstallEntryKey, "WindowsInstaller") == 1;
	FindDisplayIcon(rootKey, uninstallEntryKey, windowsInstaller);
}


void CUninstallItem::InvalidateIfHotFix(CRegKey& uninstallEntryKey)
{
	if ( m_UninstallEntryKeyName.GetLength() > 2 && m_UninstallEntryKeyName[0] == 'K' && m_UninstallEntryKeyName[1] == 'B' )
	{
		CString publisher = CHelper::GetRegistryString(uninstallEntryKey, "Publisher");
		if ( publisher == "Microsoft Corporation" )
			Invalidate();
	}
}

void CUninstallItem::FindDisplayIcon(HKEY rootKey, CRegKey& uninstallEntryKey, bool windowsInstaller)
{
	m_DisplayIcon = CHelper::GetRegistryString(uninstallEntryKey, "DisplayIcon");
	if ( !m_DisplayIcon.IsEmpty() )
	{
		FixDisplayIconName();
		return;
	}

	if ( windowsInstaller )
	{
		m_DisplayIcon = ReadMsiIcon(rootKey);
		if ( !m_DisplayIcon.IsEmpty() )
		{
			FixDisplayIconName();
			return;
		}
	}

	m_DisplayIcon = ReadArpCacheIcon(rootKey);
	if ( !m_DisplayIcon.IsEmpty() )
	{
		FixDisplayIconName();
		return;
	}

	m_DisplayIcon = m_UninstallCommand;
	FixDisplayIconName(true);
	m_DisplayIcon = m_DisplayIcon;
}

void CUninstallItem::FixDisplayIconName(bool removeRunDllAndMsiExec)
{
	m_DisplayIcon.MakeLower();

	// Remove the first quotation mark and everything after the second one.
	if ( !m_DisplayIcon.IsEmpty() && m_DisplayIcon[0] == '"' )
	{
		int index = m_DisplayIcon.Find('"', 1);
		if ( index != -1 )
			m_DisplayIcon = m_DisplayIcon.Mid(1, index - 1);
	}

	// Remove text after ".dll," including the ",".
	int index = m_DisplayIcon.Find(".dll,");
	if ( index != -1 )
		m_DisplayIcon = m_DisplayIcon.Left(index + 4); // +4 = ".dll"

	// Remove text after ".exe," including the ",".
	index = m_DisplayIcon.Find(".exe,");
	if ( index != -1 )
		m_DisplayIcon = m_DisplayIcon.Left(index + 4); // +4 = ".exe"

	// Remove text after ".ico," including the ",".
	index = m_DisplayIcon.Find(".ico,");
	if ( index != -1 )
		m_DisplayIcon = m_DisplayIcon.Left(index + 4); // +4 = ".ico"

	// Remove text after ".exe " including the space.
	index = m_DisplayIcon.Find(".exe ");
	if ( index != -1 )
		m_DisplayIcon = m_DisplayIcon.Left(index + 4); // +4 = ".exe"

	if ( removeRunDllAndMsiExec )
	{
		if ( m_DisplayIcon.Find("rundll32") == 0 || m_DisplayIcon.Find("msiexec") == 0 )
			m_DisplayIcon.Empty();
	}
}

CString CUninstallItem::ReadMsiIcon(HKEY rootKey)
{
	struct local
	{
		static void Swap(CString& text, int index1, int index2)
		{
			char c = text[index1];
			text.SetAt( index1, text[index2] );
			text.SetAt( index2, c );
		}
	};


	CString msiName = m_UninstallEntryKeyName;
	msiName = msiName.Mid(1, msiName.GetLength() - 2);

	{
		// INFO: Converting a Regular GUID to a Compressed GUID
		// http://support.installshield.com/kb/view.asp?articleid=Q105971

		// The first group of eight hexadecimal digits are placed in reverse order:
		for ( int i = 0; i < 4; ++i )
			local::Swap(msiName, i, 7 - i);

		// The same is done with the second group of four hexadecimal digits:
		for ( int i = 0; i < 2; ++i )
			local::Swap(msiName, i + 9, 12- i);

		// The same is done with the third group of four hexadecimal digits:
		for ( int i = 0; i < 2; ++i )
			local::Swap(msiName, i + 14, 17- i);

		// In the fourth group of four hexadecimal digits, every two digits switch places:
		local::Swap(msiName, 19, 20);
		local::Swap(msiName, 21, 22);

		// In the last group of 12 hexadecimal digits, again every two digits switch places:
		for ( int i = 0; i < 12; i += 2 )
			local::Swap(msiName, i + 24, i + 25);

		// Finally, the hyphens and curly braces are dropped
		msiName.Remove('-');
	}

	CString path = "SOFTWARE\\Classes\\Installer\\Products\\";
	path += msiName;

	CRegKey key;
	if ( key.Open(rootKey, path) != ERROR_SUCCESS )
		return CString();

	return CHelper::GetRegistryString(key, "ProductIcon");
}

// UnClean 2 Controls Uninstalls
// http://www.pcmag.com/article2/0,1759,1173443,00.asp
// http://www.baselinemag.com/print_article/0,3668,a=31172,00.asp
CString CUninstallItem::ReadArpCacheIcon(HKEY rootKey)
{
	#pragma pack(push, 1)
	struct SSlowInfoCache
	{
		DWORD size;
		DWORD hasName;
		INT64 installSize;
		FILETIME lastUsed;
		DWORD frequency;
		WCHAR name[262];
	};
	#pragma pack(pop)


	CRegKey cacheKey;
	CString path = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Management\\ARPCache\\";
	path += m_UninstallEntryKeyName;
	if ( cacheKey.Open(rootKey, path) != ERROR_SUCCESS )
		return CString();

	SSlowInfoCache cache;
	DWORD size = sizeof(SSlowInfoCache);
	DWORD errorCode = cacheKey.QueryBinaryValue("SlowInfoCache", &cache, &size);
	if ( errorCode != ERROR_SUCCESS || size != sizeof(SSlowInfoCache) )
		return CString();

	char buffer[MAX_PATH];
	if ( WideCharToMultiByte(CP_ACP, 0, cache.name, -1, buffer, MAX_PATH, NULL, NULL) == 0 )
		return CString();

	return buffer;
}