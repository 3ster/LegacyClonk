/* Copyright (C) 1998-2000  Matthes Bender  RedWolf Design */

/* A handy wrapper class to gzio files */

#include <Standard.h>
#include <StdFile.h>
#include <CStdFile.h>

#include <zlib.h>
#include "zlib/gzio.h"
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include <algorithm>

CStdFile::CStdFile()
{
	Status = false;
	hFile = nullptr;
	hgzFile = nullptr;
	ClearBuffer();
	ModeWrite = false;
	Name[0] = 0;
}

CStdFile::~CStdFile()
{
	Close();
}

bool CStdFile::Create(const char *szFilename, bool fCompressed, bool fExecutable)
{
	SCopy(szFilename, Name, _MAX_PATH);
	// Set modes
	ModeWrite = true;
	// Open standard file
	if (fCompressed)
	{
		if (!(hgzFile = c4_gzopen(Name, "wb1"))) return false;
	}
	else
	{
		if (fExecutable)
		{
			// Create an executable file
#ifdef _WIN32
			int mode = _S_IREAD | _S_IWRITE;
			int flags = _O_BINARY | _O_CREAT | _O_WRONLY | _O_TRUNC;
#else
			mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH;
			int flags = O_CREAT | O_WRONLY | O_TRUNC;
#endif
			int fd = open(Name, flags, mode);
			if (fd == -1) return false;
			if (!(hFile = fdopen(fd, "wb"))) return false;
		}
		else
		{
			if (!(hFile = fopen(Name, "wb"))) return false;
		}
	}
	// Reset buffer
	ClearBuffer();
	// Set status
	Status = true;
	return true;
}

bool CStdFile::Open(const char *szFilename, bool fCompressed)
{
	SCopy(szFilename, Name, _MAX_PATH);
	// Set modes
	ModeWrite = false;
	// Open standard file
	if (fCompressed)
	{
		if (!(hgzFile = c4_gzopen(Name, "rb"))) return false;
	}
	else
	{
		if (!(hFile = fopen(Name, "rb"))) return false;
	}
	// Reset buffer
	ClearBuffer();
	// Set status
	Status = true;
	return true;
}

bool CStdFile::Append(const char *szFilename)
{
	SCopy(szFilename, Name, _MAX_PATH);
	// Set modes
	ModeWrite = true;
	// Open standard file
	if (!(hFile = fopen(Name, "ab"))) return false;
	// Reset buffer
	ClearBuffer();
	// Set status
	Status = true;
	return true;
}

bool CStdFile::Close()
{
	bool rval = true;
	Status = false;
	Name[0] = 0;
	// Save buffer if in write mode
	if (ModeWrite && BufferLoad) if (!SaveBuffer()) rval = false;
	// Close file(s)
	if (hgzFile) if (c4_gzclose(hgzFile) != Z_OK) rval = false;
	if (hFile) if (fclose(hFile) != 0) rval = false;
	hgzFile = nullptr; hFile = nullptr;
	return !!rval;
}

bool CStdFile::Default()
{
	Status = false;
	Name[0] = 0;
	hgzFile = nullptr;
	hFile = nullptr;
	BufferLoad = BufferPtr = 0;
	return true;
}

bool CStdFile::Read(void *pBuffer, size_t iSize, size_t *ipFSize)
{
	int transfer;
	if (!pBuffer) return false;
	if (ModeWrite) return false;
	uint8_t *bypBuffer = (uint8_t *)pBuffer;
	if (ipFSize) *ipFSize = 0;
	while (iSize > 0)
	{
		// Valid data in the buffer: Transfer as much as possible
		if (BufferLoad > BufferPtr)
		{
			transfer = std::min<size_t>(BufferLoad - BufferPtr, iSize);
			memmove(bypBuffer, Buffer + BufferPtr, transfer);
			BufferPtr += transfer;
			bypBuffer += transfer;
			iSize -= transfer;
			if (ipFSize) *ipFSize += transfer;
		}
		// Buffer empty: Load
		else if (LoadBuffer() <= 0) return false;
	}
	return true;
}

int CStdFile::LoadBuffer()
{
	if (hFile) BufferLoad = fread(Buffer, 1, CStdFileBufSize, hFile);
	if (hgzFile) BufferLoad = c4_gzread(hgzFile, Buffer, CStdFileBufSize);
	BufferPtr = 0;
	return BufferLoad;
}

bool CStdFile::SaveBuffer()
{
	int saved = 0;
	if (hFile) saved = fwrite(Buffer, 1, BufferLoad, hFile);
	if (hgzFile) saved = c4_gzwrite(hgzFile, Buffer, BufferLoad);
	if (saved != BufferLoad) return false;
	BufferLoad = 0;
	return true;
}

void CStdFile::ClearBuffer()
{
	BufferLoad = BufferPtr = 0;
}

bool CStdFile::Write(const void *pBuffer, int iSize)
{
	int transfer;
	if (!pBuffer) return false;
	if (!ModeWrite) return false;
	uint8_t *bypBuffer = (uint8_t *)pBuffer;
	while (iSize > 0)
	{
		// Space in buffer: Transfer as much as possible
		if (BufferLoad < CStdFileBufSize)
		{
			transfer = (std::min)(CStdFileBufSize - BufferLoad, iSize);
			memcpy(Buffer + BufferLoad, bypBuffer, transfer);
			BufferLoad += transfer;
			bypBuffer += transfer;
			iSize -= transfer;
		}
		// Buffer full: Save
		else if (!SaveBuffer()) return false;
	}
	return true;
}

bool CStdFile::WriteString(const char *szStr)
{
	uint8_t nl[2] = { 0x0D, 0x0A };
	if (!szStr) return false;
	int size = SLen(szStr);
	if (!Write((void *)szStr, size)) return false;
	if (!Write(nl, 2)) return false;
	return true;
}

bool CStdFile::Rewind()
{
	if (ModeWrite) return false;
	ClearBuffer();
	if (hFile) rewind(hFile);
	if (hgzFile)
	{
		if (c4_gzclose(hgzFile) != Z_OK) { hgzFile = nullptr; return false; }
		if (!(hgzFile = c4_gzopen(Name, "rb"))) return false;
	}
	return true;
}

bool CStdFile::Advance(int iOffset)
{
	if (ModeWrite) return false;
	while (iOffset > 0)
	{
		// Valid data in the buffer: Transfer as much as possible
		if (BufferLoad > BufferPtr)
		{
			int transfer = (std::min)(BufferLoad - BufferPtr, iOffset);
			BufferPtr += transfer;
			iOffset -= transfer;
		}
		// Buffer empty: Load or skip
		else
		{
			if (hFile) return !fseek(hFile, iOffset, SEEK_CUR); // uncompressed: Just skip
			if (LoadBuffer() <= 0) return false; // compressed: Read...
		}
	}
	return true;
}

bool CStdFile::Save(const char *szFilename, const uint8_t *bpBuf,
	int iSize, bool fCompressed)
{
	if (!bpBuf || (iSize < 1)) return false;
	if (!Create(szFilename, fCompressed)) return false;
	if (!Write(bpBuf, iSize)) return false;
	if (!Close()) return false;
	return true;
}

bool CStdFile::Load(const char *szFilename, uint8_t **lpbpBuf,
	int *ipSize, int iAppendZeros,
	bool fCompressed)
{
	int iSize = fCompressed ? UncompressedFileSize(szFilename) : FileSize(szFilename);
	if (!lpbpBuf || (iSize < 1)) return false;
	if (!Open(szFilename, fCompressed)) return false;
	*lpbpBuf = new uint8_t[iSize + iAppendZeros];
	if (!Read(*lpbpBuf, iSize)) { delete[] *lpbpBuf; return false; }
	if (iAppendZeros) std::fill_n(*lpbpBuf + iSize, iAppendZeros, 0);
	if (ipSize) *ipSize = iSize;
	Close();
	return true;
}

int UncompressedFileSize(const char *szFilename)
{
	int rval = 0;
	uint8_t buf;
	gzFile hFile;
	if (!(hFile = c4_gzopen(szFilename, "rb"))) return 0;
	while (c4_gzread(hFile, &buf, 1) > 0) rval++;
	c4_gzclose(hFile);
	return rval;
}

int CStdFile::AccessedEntrySize()
{
	if (hFile)
	{
		long pos = ftell(hFile);
		fseek(hFile, 0, SEEK_END);
		int r = ftell(hFile);
		fseek(hFile, pos, SEEK_SET);
		return r;
	}
	assert(!hgzFile);
	return 0;
}