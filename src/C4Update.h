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
	bool Load(C4Group &hGroup);
	bool Save(C4Group &hGroup);
};

#define C4UPD_CHK_OK              0
#define C4UPD_CHK_NO_SOURCE       1
#define C4UPD_CHK_BAD_SOURCE      2
#define C4UPD_CHK_ALREADY_UPDATED 3
#define C4UPD_CHK_BAD_VERSION     4

class C4UpdatePackage : public C4UpdatePackageCore
{
public:
	bool Load(C4Group *pGroup);
	bool Execute(C4Group *pGroup);
	static bool Optimize(C4Group *pGrpFrom, const char *strTarget);
	int Check(C4Group *pGroup);
	bool MakeUpdate(const char *strFile1, const char *strFile2, const char *strUpdateFile, const char *strName = nullptr);

protected:
	bool DoUpdate(C4Group *pGrpFrom, class C4GroupEx *pGrpTo, const char *strFileName);
	bool DoGrpUpdate(C4Group *pUpdateData, class C4GroupEx *pGrpTo);
	static bool Optimize(C4Group *pGrpFrom, class C4GroupEx *pGrpTo, const char *strFileName);

	bool MkUp(C4Group *pGrp1, C4Group *pGrp2, C4GroupEx *pUpGr, bool *fModified);

	CStdFile Log;
	void WriteLog(const char *strMsg, ...) GNUC_FORMAT_ATTRIBUTE_O;
};

bool C4Group_ApplyUpdate(C4Group &hGroup);