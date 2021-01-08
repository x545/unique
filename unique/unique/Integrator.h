/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

#pragma once

class Integrator
{
public:
	~Integrator(void);
	void rename(CString strAppend = _T("ren%d"));
	void sync(void);
	void moveWhileIntegrating(bool move);


inline Integrator::Integrator(void)
{ 
	m_moveWhileIntegrating = TRUE;
	m_strListFiles.RemoveAll();
	m_strListTargetFiles.RemoveAll();
	m_filesInPath.RemoveAll();
}
//inline Integrator(CStringList *pstrListFiles) { this->m_strListFiles.AddTail(pstrListFiles); }
inline void setTargetDirectory(const CString* pstrPath) { this->m_strDestPath = *pstrPath; }
inline void addFile(const CString* pstrFile)
{
	POSITION pos = this->m_strListFiles.AddTail(*pstrFile);
}
private:
	CStringList m_strListFiles;
	CStringList m_strListTargetFiles;
	CStringList m_filesInPath;
	CString m_strDestPath;
	void getFilesInPath();
	CString getFileName(const CString *pstrFullFileName);
	bool alsoInFilesInPath(const CString *pstrFileName);
	void integrateFiles(void);
	CString changeFileName(const CString *strFileName, const CString *strAppend);
	bool m_moveWhileIntegrating;
};
