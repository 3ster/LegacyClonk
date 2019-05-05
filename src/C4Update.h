/*
 * LegacyClonk
 *
 * Copyright (c) RedWolf Design
 * Copyright (c) 2017-2019, The LegacyClonk Team and contributors
 *
 * Distributed under the terms of the ISC license; see accompanying file
 * "COPYING" for details.
 *
 * "Clonk" is a registered trademark of Matthes Bender, used with permission.
 * See accompanying file "TRADEMARK" for details.
 *
 * To redistribute this file separately, substitute the full license texts
 * for the above references.
 */

/* Update package support */

#pragma once

#include "C4Group.h"

const int C4UP_MaxUpGrpCnt = 50;

class C4UpdatePackageCore
{
public:
	C4UpdatePackageCore();

public:
	int32_t RequireVersion[4];
	char Name[C4MaxName + 1];
	char DestPath[_MAX_PATH + 1];
	int32_t GrpUpdate;
	int32_t UpGrpCnt; // number of file versions that can be updated by this package
	uint32_t GrpChks1[C4UP_MaxUpGrpCnt], GrpChks2;

public:
	void CompileFunc(StdCompiler *pComp);
	BOOL Load(C4Group &hGroup);
	BOOL Save(C4Group &hGroup);
};

#define C4UPD_CHK_OK              0
#define C4UPD_CHK_NO_SOURCE       1
#define C4UPD_CHK_BAD_SOURCE      2
#define C4UPD_CHK_ALREADY_UPDATED 3
#define C4UPD_CHK_BAD_VERSION     4

class C4UpdatePackage : public C4UpdatePackageCore
{
public:
	BOOL Load(C4Group *pGroup);
	BOOL Execute(C4Group *pGroup);
	static BOOL Optimize(C4Group *pGrpFrom, const char *strTarget);
	int Check(C4Group *pGroup);
	BOOL MakeUpdate(const char *strFile1, const char *strFile2, const char *strUpdateFile, const char *strName = NULL);

protected:
	BOOL DoUpdate(C4Group *pGrpFrom, class C4GroupEx *pGrpTo, const char *strFileName);
	BOOL DoGrpUpdate(C4Group *pUpdateData, class C4GroupEx *pGrpTo);
	static BOOL Optimize(C4Group *pGrpFrom, class C4GroupEx *pGrpTo, const char *strFileName);

	BOOL MkUp(C4Group *pGrp1, C4Group *pGrp2, C4GroupEx *pUpGr, BOOL *fModified);

	CStdFile Log;
	void WriteLog(const char *strMsg, ...) GNUC_FORMAT_ATTRIBUTE_O;
};

bool C4Group_ApplyUpdate(C4Group &hGroup);
