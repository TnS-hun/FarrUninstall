// Copyright (C) 2007 Miklós "TnS" Végvári
#pragma once


class CUninstallItem
{
public:
	CUninstallItem();
	CUninstallItem(HKEY rootKey, const char* pszUninstallEntryKeyName, bool needHotFixes);

	bool IsValid() const { return !m_UninstallCommand.IsEmpty(); }

	const CString& GetDisplayName() const { return m_DisplayName; }
	const CString& GetDisplayIcon() const { return m_DisplayIcon; }
	const CString& GetUninstallCommand() const { return m_UninstallCommand; }


private:
	void Invalidate() { m_UninstallCommand.Empty(); }
	void InvalidateIfHotFix(CRegKey& uninstallEntryKey);
	void FindDisplayIcon(HKEY rootKey, CRegKey& uninstallEntryKey, bool windowsInstaller);
	void FixDisplayIconName(bool removeRunDllAndMsiExec = false);
	CString ReadMsiIcon(HKEY rootKey);
	CString ReadArpCacheIcon(HKEY rootKey);

	CString m_UninstallEntryKeyName;
	CString m_DisplayName;
	CString m_DisplayIcon;
	CString m_UninstallCommand; 
};
