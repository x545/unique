/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

// unique.h : Hauptheaderdatei f�r die unique-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole


// CuniqueApp:
// Siehe unique.cpp f�r die Implementierung dieser Klasse
//

class CuniqueApp : public CWinApp
{
public:
	CuniqueApp();

// �berschreibungen
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CuniqueApp theApp;
