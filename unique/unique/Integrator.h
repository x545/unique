/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

#pragma once

class Integrator
{
public:
	~Integrator(void);
	Integrator(const Integrator&) = delete;
	Integrator(Integrator&&) = delete;
	Integrator& operator=(const Integrator&) = delete;
	Integrator& operator=(Integrator&&) = delete;
	void rename(const CString strAppend = _T("ren%d"));
	void sync(void);
	void moveWhileIntegrating(const bool move) noexcept;


inline Integrator(void) noexcept
{ 
	m_moveWhileIntegrating = TRUE;
	m_files.RemoveAll();
	m_targetFiles.RemoveAll();
	m_filesInPath.RemoveAll();
}
//inline Integrator(CStringList *listFiles) { this->m_listFiles.AddTail(listFiles); }
inline void setTargetDirectory(const CString* path) { this->m_strDestPath = *path; }
inline void addFile(const CString* fileName)
{
	this->m_files.AddTail(*fileName);
}

private:
	CStringList m_files;
	CStringList m_targetFiles;
	CStringList m_filesInPath;
	CString m_strDestPath;
	void getFilesInPath();
	CString getFileName(const CString *fullFileName);
	bool alsoInFilesInPath(const CString *fileName);
	void integrateFiles(void);
	CString changeFileName(const CString *fileName, const CString *append);
	bool m_moveWhileIntegrating;
};
