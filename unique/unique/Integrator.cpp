/* 
 * Copyright 2002 - 2010 by x545, x545@gmx.net
 */

// $Id$

#include "StdAfx.h"
#include "integrator.h"


Integrator::~Integrator(void) {
	if (!m_files.IsEmpty())
	{
		m_files.RemoveAll();
	}
	if (!m_targetFiles.IsEmpty())
	{
		m_targetFiles.RemoveAll();
	}
}

void Integrator::rename(const CString append /* = _T("ren%d") */)
{
	getFilesInPath();
	for (POSITION pos = m_files.GetHeadPosition(); NULL != pos; )
	{
		CString strNextFile = m_files.GetNext(pos);
		strNextFile = getFileName(&strNextFile);
		
		// if file already exists: rename
		if ( alsoInFilesInPath(&strNextFile)  )
		{
			m_targetFiles.AddTail( m_strDestPath + changeFileName(&strNextFile, &append) );
		}
		else
		{
			m_targetFiles.AddTail( m_strDestPath + strNextFile );
		}
	}
	integrateFiles();
}

void Integrator::sync(void)
{
	getFilesInPath();
	POSITION posOld;
	for (POSITION pos = m_files.GetHeadPosition(); NULL != (posOld = pos); )
	{
		CString strNextFile = m_files.GetNext(pos);
		strNextFile = getFileName(&strNextFile);
		
		// if file already exists: skip and delete file in m_strListFiles to keep lists in sync
		if ( alsoInFilesInPath(&strNextFile)  )
		{
			m_files.RemoveAt(posOld);
		}
		else
		{
			m_targetFiles.AddTail( m_strDestPath + strNextFile );
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

CString Integrator::getFileName(const CString *fullFileName)
{
	const int pos = fullFileName->ReverseFind(_T('\\'));
	ASSERT(pos != -1);	// Ups: given FullFileName doesn't contain '\' -- is he wrong?
	return fullFileName->Right(fullFileName->GetLength() - pos - 1);
}

bool Integrator::alsoInFilesInPath(const CString *fileName)
{
	for ( POSITION pos = m_filesInPath.GetHeadPosition(); pos != NULL; ) 
	{
		if ( m_filesInPath.GetNext(pos) == *fileName )
		{
			return true;
		}
	}
	return false;
}

void Integrator::integrateFiles(void)
{
	POSITION headPos = m_targetFiles.GetHeadPosition();
	if (headPos)
	{
		for ( POSITION pos = m_files.GetHeadPosition(); pos != NULL; ) 
		{
			CString strInFileName = m_files.GetNext(pos);
			CString strOutFileName = m_targetFiles.GetNext(headPos);
#ifdef _DEBUG			
			TRACE(_T("integrate file: %s into: %s\n"), strInFileName, strOutFileName);
#endif //_DEBUG
			constexpr auto BUFFER_SIZE = 4096;
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
			catch(CFileException* ex)
			{
				in.Abort();
				out.Abort();
				ex->ReportError();
			}
		}
	}
}

CString Integrator::changeFileName(const CString *fileName, const CString *append)
{
	const int pos = fileName->ReverseFind(_T('.'));
	CString strExtension = _T("");
	CString strFileNameWithoutExtension = *fileName;
	if (-1 != pos)
	{
		strFileNameWithoutExtension = fileName->Left(pos);
		strExtension = fileName->Right(fileName->GetLength() - pos);
	}
	unsigned int i = 0;
	CString strAndNumberToAppend = *append;
	strAndNumberToAppend.Format(*append, i);
	auto newFileName = strFileNameWithoutExtension + strAndNumberToAppend + strExtension;
	while (alsoInFilesInPath(&newFileName))
	{
		strAndNumberToAppend.Format(*append, ++i);
	}
	return strFileNameWithoutExtension + strAndNumberToAppend + strExtension;
}

void Integrator::moveWhileIntegrating(const bool doMove) noexcept
{
	doMove ? this->m_moveWhileIntegrating = TRUE : this->m_moveWhileIntegrating = FALSE;
}