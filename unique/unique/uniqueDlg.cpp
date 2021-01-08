/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

#include "stdafx.h"
#include "unique.h"
#include "uniqueDlg.h"
#include "afxdialogex.h"
#include "Integrator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl 'Info'

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CuniqueDlg Dialogfeld



CuniqueDlg::CuniqueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CuniqueDlg::IDD, pParent)
	, m_strDEST_PATH(_T(''))
	, m_radCOPY_FILESorMOVE_FILES(0)
	, m_editRENAME_APPENDIX(_T("ren%d"))
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CuniqueDlg::~CuniqueDlg()
{
	if (this->m_integrator) {
		delete m_integrator;
	}
	// deaktiviert, sonst Exception mit VS 10 
	// CDialog::~CDialog();
}

void CuniqueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILES, m_listFILES);
	DDX_Text(pDX, IDC_DEST_PATH, m_strDEST_PATH);
	DDX_Control(pDX, IDC_RENAME_FILES, m_butnRENAME_FILES);
	DDX_Control(pDX, IDC_DEST_PATH, m_editDEST_PATH);
	DDX_Control(pDX, IDC_SYNC_FILES, m_butnSYNC_FILES);
	DDX_Control(pDX, IDC_ONLY_SELECTED, m_checkONLY_SELECTED);
	DDX_Radio(pDX, IDC_COPY_FILES, m_radCOPY_FILESorMOVE_FILES);
	DDX_Text(pDX, IDC_APPENDIX, m_editRENAME_APPENDIX);
}

BEGIN_MESSAGE_MAP(CuniqueDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SEL_FILES, OnBnClickedSelFiles)
	ON_BN_CLICKED(IDC_RENAME_FILES, OnBnClickedRenameFiles)
	ON_BN_CLICKED(IDC_SYNC_FILES, OnBnClickedSyncFiles)
	ON_BN_CLICKED(IDC_SEL_PATH, OnBnClickedSelPath)
	ON_EN_KILLFOCUS(IDC_APPENDIX, OnEnKillfocusAppendix)
	ON_NOTIFY(LVN_KEYDOWN, IDC_FILES, OnLvnKeydownFiles)
END_MESSAGE_MAP()


// CuniqueDlg Meldungshandler

BOOL CuniqueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// Initalisations
	
	this->m_strDEST_PATH = _T("");
	this->m_listFILEScount = 0;								// Pointer to the last file of m_listFILES
	this->m_listFILES.SetExtendedStyle(LVS_EX_CHECKBOXES);	// Markers before files in ListFILES
	this->m_checkONLY_SELECTED.SetCheck(BST_CHECKED);		// set already checked
	this->m_integrator = NULL;								// !existing
	CheckRadioButton(IDC_COPY_FILES, IDC_MOVE_FILES, IDC_COPY_FILES);
		
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CuniqueDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CuniqueDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CuniqueDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CuniqueDlg::OnBnClickedSelFiles()
{
#define BUFFER 102400 
	TCHAR buffer[BUFFER];
	buffer[0] = 0;
	CFileDialog fdlg(TRUE, _T("*"), _T("*.*"), OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST
					| OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER, 
					_T("All files (*.*)|*.*|Pictures (*.jpg; *.jpeg; *.bmp; *.gif; *.png)|*.jpg;*.jpeg;*.bmp;*.gif;*.png|Movies (*.avi; *.mov; *.wma; *.wmv; *.asf; *.mpg; *.mpeg)|*.avi;*.mov;*.wma;*.wmv;*.asf;*.mpg;*.mpeg||")); 
	fdlg.m_ofn.lpstrTitle = _T("select files...");
	fdlg.m_ofn.lpstrFile = buffer;
	fdlg.m_ofn.nMaxFile = BUFFER;
	if (fdlg.DoModal() == IDOK)
	{
		CString nextFile;
		POSITION i = fdlg.GetStartPosition();
		while ( i )
		{
			nextFile = fdlg.GetNextPathName(i);
			insertIn_m_listFILES(&nextFile);
#ifdef _DEBUG			
			TRACE(_T("selected file: %s\n"), nextFile);
#endif //_DEBUG
		}
		m_checkONLY_SELECTED.EnableWindow();
		mayChangeIntegrateButtons();
	}
}
void CuniqueDlg::mayChangeIntegrateButtons()
{
	if ( (m_listFILES.GetItemText(0, 0) != _T("")) && (m_strDEST_PATH != _T("")) )
	{
		m_butnRENAME_FILES.EnableWindow();
		m_butnSYNC_FILES.EnableWindow();
	} 
	else
	{
		m_butnRENAME_FILES.EnableWindow(FALSE);
		m_butnSYNC_FILES.EnableWindow(FALSE);
	}
}
void CuniqueDlg::insertIn_m_listFILES(CString *pstrFile)
{
	LVFINDINFO info;
	
	info.flags = LVFI_STRING;
	info.psz = pstrFile->GetBuffer(0);

	int pos_search;
	if (  ( pos_search = m_listFILES.FindItem(&info) ) != -1  ) return; //file already in list
	
	LV_ITEM	lvI;
	lvI.iSubItem = 0;
	lvI.mask = LVIF_TEXT;
	
	lvI.iItem = m_listFILEScount;
	lvI.pszText = pstrFile->GetBuffer(0);
	pstrFile->ReleaseBuffer();
	m_listFILES.InsertItem(&lvI);
	m_listFILES.SetCheck(m_listFILEScount++);
		
	m_listFILES.UpdateData(FALSE);
}

void CuniqueDlg::OnBnClickedRenameFiles(void)
{
	initIntegrator();
	this->UpdateData(FALSE);
	m_integrator->rename(this->m_editRENAME_APPENDIX);
	
	afterIntegrator();
}

void CuniqueDlg::OnBnClickedSyncFiles(void)
{ 
	initIntegrator();
	m_integrator->sync();
	
	afterIntegrator();
}

// call this function when Integrator has done work and the GUI has to show the results
void CuniqueDlg::afterIntegrator(void)
{
	m_listFILES.DeleteAllItems();
	m_listFILES.UpdateData(FALSE);
	m_checkONLY_SELECTED.SetCheck(BST_CHECKED);		// set already checked
	this->m_listFILEScount = 0;

	mayChangeIntegrateButtons();
	
	delete m_integrator;
	this->m_integrator = NULL;

}

void CuniqueDlg::initIntegrator(void)
{
	if (!this->m_integrator)
	{
		m_integrator = new Integrator();
	}

	m_integrator->setTargetDirectory(&m_strDEST_PATH);
	for (int pos = 0; pos < m_listFILEScount; pos++)
	{
		if ( m_checkONLY_SELECTED.GetState() & 0x0003 )
		{
			if ( m_listFILES.GetCheck(pos) ) 
			{
				m_integrator->addFile( &m_listFILES.GetItemText(pos, 0) );
			}
		}
		else
		{
			m_integrator->addFile( &m_listFILES.GetItemText(pos, 0) );
		}
	}
	UpdateData();
	m_radCOPY_FILESorMOVE_FILES ? this->m_integrator->moveWhileIntegrating(TRUE)
								: this->m_integrator->moveWhileIntegrating(FALSE);
}

void CuniqueDlg::OnBnClickedSelPath(void)
{
	HWND owner = NULL;
	TCHAR buf[MAX_PATH];
	BROWSEINFO bi;
	LPMALLOC ma;
	
	ZeroMemory(&bi, sizeof(bi));

	UpdateData();
	
	bi.hwndOwner = owner;
	bi.pszDisplayName = buf;
	bi.lpszTitle = _T("choose target directory");
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		SHGetPathFromIDList(pidl, buf);
		m_strDEST_PATH = buf;
		
		if(m_strDEST_PATH.GetLength() > 0 && m_strDEST_PATH.Right(1) != _T('\\'))
		{
			m_strDEST_PATH += _T('\\');
		}
		if(SHGetMalloc(&ma) != E_FAIL)
		{
			ma->Free(pidl);
			ma->Release();
		}
#ifdef _DEBUG			
		TRACE(_T("selected path: %s\n"), m_strDEST_PATH);
#endif //_DEBUG		
		
		UpdateData(FALSE);
		mayChangeIntegrateButtons();
	}
}

void CuniqueDlg::OnEnKillfocusAppendix()
{
	CString old_m_editRENAME_APPENDIX = m_editRENAME_APPENDIX;
	UpdateData();			// get new appendix
	int pos = m_editRENAME_APPENDIX.Find(_T("%d"));
	if (  (pos == -1)
		  || (m_editRENAME_APPENDIX.Find(_T('%'), pos + 2) != -1) 
		  || m_editRENAME_APPENDIX.Find(_T('%')) < pos  )
	{
		CString str_errMsg;
		str_errMsg.LoadString(IDS_INCORRECT_RENAME_APPENDIX);
		::MessageBox(NULL, str_errMsg, _T("unique") , MB_ICONSTOP | MB_OK);

		m_editRENAME_APPENDIX = old_m_editRENAME_APPENDIX;
		UpdateData(FALSE);	// update GUI with old (correct) appendix
	}
}

void CuniqueDlg::OnLvnKeydownFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if ( pLVKeyDown->wVKey == VK_DELETE )
	{
		POSITION pos = m_listFILES.GetFirstSelectedItemPosition();
		unsigned int del_positions[BUFFER], i = 0;
		
		while (pos)
		{
			del_positions[i++] = m_listFILES.GetNextSelectedItem(pos);

		}
		while (i + 1)
		{
			m_listFILES.DeleteItem(del_positions[i--]);
			this->m_listFILEScount--;
		}
		
		m_listFILES.UpdateData(FALSE);
		m_checkONLY_SELECTED.SetCheck(BST_CHECKED);		// set already checked

		mayChangeIntegrateButtons();
	}

	*pResult = 0;
}
