/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

// uniqueDlg.h : Headerdatei
//

#pragma once

#include <memory>
#include "Integrator.h"

using namespace std;

// CuniqueDlg Dialogfeld
class CuniqueDlg : public CDialogEx
{
// Konstruktion
public:
	CuniqueDlg(CWnd *pParent = NULL);	// Standardkonstruktor
	//~CuniqueDlg();
	
	// call this function when Integrator has done work and GUI should show results
	void afterIntegrator(void);

// Dialogfelddaten
	enum { IDD = IDD_UNIQUE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon() noexcept;
	afx_msg void OnBnClickedSelFiles();
	afx_msg void OnBnClickedRenameFiles();
	afx_msg void OnBnClickedSyncFiles();
	afx_msg void OnBnClickedSelPath();
	DECLARE_MESSAGE_MAP();

	void insertIn_m_listFILES(CString *file);
	void initIntegrator();
	void mayChangeIntegrateButtons();
	
	CListCtrl m_listFILES;
	CString m_strDEST_PATH;
	CButton m_butnRENAME_FILES;
	CEdit m_editDEST_PATH;
	CButton m_butnSYNC_FILES;
	CButton m_checkONLY_SELECTED;
	int m_radCOPY_FILESorMOVE_FILES;
	CString m_editRENAME_APPENDIX;

private:
	int m_listFILEScount;
	unique_ptr<Integrator> m_integrator;
	constexpr static auto BUFFER_SIZE = 102400;

public:
	afx_msg void OnEnKillfocusAppendix();
	afx_msg void OnLvnKeydownFiles(NMHDR *pNMHDR, LRESULT *pResult);
};
