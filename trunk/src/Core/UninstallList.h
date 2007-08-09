// Copyright (C) 2007 Miklós "TnS" Végvári
#pragma once
#include "UninstallItem.h"


class CUninstallList
{
public:
	void MakeList();

	int GetItemCount() const { return (int)m_Items.GetSize(); }
	const CUninstallItem& GetItem(int index) { return m_Items[index]; }


private:
	void MakeList(HKEY rootKey);

	CArray<CUninstallItem> m_Items;
};