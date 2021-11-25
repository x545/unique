/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

#include "StdAfx.h"
#include "integrator.h"

Integrator::~Integrator(void) {
	if (!m_strListFiles.IsEmpty())
	{
		m_strListFiles.RemoveAll();
	}
	if (!m_strListTargetFiles.IsEmpty())
	{
		m_strListTargetFiles.RemoveAll();
	}
}

void Integrator::rename(CString strAppend /* = _T("ren%d") */)
{
	getFilesInPath();
	for (POSITION pos = m_strListFiles.GetHeadPosition(); pos != NULL; )
	{
		CString strNextFile = m_strListFiles.GetNext(pos);
		strNextFile = getFileName(&strNextFile);
		
		// if file already exists: rename
		if ( alsoInFilesInPath(&strNextFile)  )
		{
			m_strListTargetFiles.AddTail( m_strDestPath + changeFileName(&strNextFile, &strAppend) );
		}
		else
		{
			m_strListTargetFiles.AddTail( m_strDestPath + strNextFile );
		}
	}
	integrateFiles();
}

void Integrator::sync(void)
{
	getFilesInPath();
	POSITION posOld;
	for (POSITION pos = m_strListFiles.GetHeadPosition(); (posOld = pos) != NULL; )
	{
		CString strNextFile = m_strListFiles.GetNext(pos);
		strNextFile = getFileName(&strNextFile);
		
		// if file already exists: skip and delete file in m_strListFiles to keep lists in sync
		if ( alsoInFilesInPath(&strNextFile)  )
		{
			m_strListFiles.RemoveAt(posOld);
		}
		else
		{
			m_strListTargetFiles.AddTail( m_strDestPath + strNextFile );
		}
	}
	integrateFiles();
}

void Integrator::getFilesInPath(void)
{
	//store all files in m_strDestPath into a m_filesInPath
	CFileFind finder;
	BOOL bWorking = finder.FindFile( m_strDestPath + _T("*.*") );
	
	while (bWorking)   
	{
		bWorking = finder.FindNextFile();
		if( !finder.IsDirectory() )
		{		
			m_filesInPath.AddTail( finder.GetFileName() );
		}
	}
}

CString Integrator::getFileName(const CString *pstrFullFileName)
{
	int pos = pstrFullFileName->ReverseFind(_T('\\'));
	ASSERT(pos != -1);	// Upps: given FullFileName doesn't contain '\' -- is he wrong?
	return pstrFullFileName->Right(pstrFullFileName->GetLength() - pos-1);
}

bool Integrator::alsoInFilesInPath(const CString* pstrFileName)
{
	for ( POSITION pos = m_filesInPath.GetHeadPosition(); pos != NULL; ) 
	{
		if ( m_filesInPath.GetNext(pos) == *pstrFileName )
		{
			return true;
		}
	}
	return false;
}

void Integrator::integrateFiles(void)
{
	POSITION pos2 = m_strListTargetFiles.GetHeadPosition();
	if (pos2)
	{
		for ( POSITION pos = m_strListFiles.GetHeadPosition(); pos != NULL; ) 
		{
			CString strInFileName = m_strListFiles.GetNext(pos);
			CString strOutFileName = m_strListTargetFiles.GetNext(pos2);
#ifdef _DEBUG			
			TRACE(_T("integrate file: %s into: %s\n"), strInFileName, strOutFileName);
#endif //_DEBUG
			#define BUFFER_SIZE 4096
			CFile in, out;
			CFileException e;
			CString s;
			BYTE buffer[BUFFER_SIZE];
			DWORD dwRead;
			try
			{
				if( in.Open(strInFileName, CFile::modeRead, &e) 
				    && out.Open(strOutFileName, CFile::modeWrite | CFile::modeCreate, &e) )
				{
					do
					{
						dwRead = in.Read(buffer, BUFFER_SIZE);
						out.Write(buffer, dwRead);
					}
					while (dwRead > 0);
				}
				else
				{
					e.ReportError();
					return;
				}
				in.Close();
				out.Close();
				
				if (this->m_moveWhileIntegrating)
				{
					in.Remove(strInFileName);
				}
			}
			catch( CFileException* e )
			{
				in.Abort();
				out.Abort();
				e->ReportError();
			}
		}
	}
}

CString Integrator::changeFileName(const CString *strFileName, const CString *strAppend)
{
	int pos = strFileName->ReverseFind(_T('.'));
	CString strExtension = _T("");
	CString strFileNameWithoutExtension = *strFileName;
	if ( pos != -1 )
	{
		strFileNameWithoutExtension = strFileName->Left(pos);
		strExtension = strFileName->Right(strFileName->GetLength() - pos);
	}
	unsigned int i = 0;
	CString strAndNumberToAppend = *strAppend;
	strAndNumberToAppend.Format(*strAppend, i);
	while (  alsoInFilesInPath( &(strFileNameWithoutExtension + strAndNumberToAppend + strExtension) )  )
	{
		strAndNumberToAppend.Format(*strAppend, ++i);
	}
	return strFileNameWithoutExtension + strAndNumberToAppend + strExtension;
}

void Integrator::moveWhileIntegrating(bool move)
{
	move ? this->m_moveWhileIntegrating = TRUE : this->m_moveWhileIntegrating = FALSE;
}