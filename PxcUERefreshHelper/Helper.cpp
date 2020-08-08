#include "Helper.h"

#define Project_Dep_Begin "\tProjectSection(ProjectDependencies) = postProject"
#define Project_Dep_End "\tEndProjectSection"
#define Tab2_Equal "\t\t{%s} = {%s}"

#define Project_Info_Begin "Project(\"{%s}\") = \"%s\", \"%s\", \"{%s}\""
#define Project_Info_End "EndProject"

#define Config_Line "\t\t{%s}.%s|%s.%s = %s|%s"

#define PI_PublicDef_GUID "D86EB3AC-237E-4B1B-B985-F31A225A042F"
#define PI_PublicDef_Name "PublicDefinitions"
#define PI_PublicDef_Path "PxcCore\\PublicDefinitions\\PublicDefinitions.vcxproj"
#define PI_DataTables_GUID "C362F347-D4A4-4D3C-892A-25876EA1BB24"
#define PI_DataTables_Name "DataTables"
#define PI_DataTables_Path "PxcCore\\DataTables\\DataTables.vcxproj"

#define mibf(SUFF) (m_str##SUFF = szBuf) += "\n"
#define nibf(SUFF) (pt.str##SUFF = szBuf) += "\n"
#define mabf(SUFF) (m_str##SUFF += szBuf) += "\n"
#define nabf(SUFF) (pt.str##SUFF += szBuf) += "\n"

#define mtxi(PC, PP) (PC << 1) | PP

const char* c_szGameConfigs[] = {
	"DebugGame Editor", "DebugGame", "Development Editor", "Development", "Shipping"
};
#define GCNum 5
const char* c_szGamePlatforms[] = {
	"HoloLens", "Win32", "Win64"
};
#define GPNum 3
const char* c_szGameSubs[] = {
	"ActiveCfg", "Build.0"
};
#define GSNum 2

const char* c_szProjectConfigs[] = {
	"Release", "Debug"
};
#define PCRelease 0
#define PCDebug 1
const char* c_szProjectPlatforms[] = {
	"Win32", "x64"
};
#define PPWin32 0
#define PPx64 1

Helper::Helper()
{
	m_strFilePath = "UE4Game.sln";
}

Helper::Helper(const std::string& strFilePath)
{
	m_strFilePath = strFilePath;
}

Helper::~Helper()
{
	//
}

bool Helper::Init()
{
	InitInsertTexts();
	return InitFile();
}

void Helper::InitInsertTexts()
{
	char szBuf[512];

	(m_strUEGameProjDep = Project_Dep_Begin) += "\n";
	sprintf_s(szBuf, Tab2_Equal, PI_DataTables_GUID, PI_DataTables_GUID); mabf(UEGameProjDep);
	sprintf_s(szBuf, Tab2_Equal, PI_PublicDef_GUID, PI_PublicDef_GUID); mabf(UEGameProjDep);
	(m_strUEGameProjDep += Project_Dep_End) += "\n";

	ProjectText pt;

	sprintf_s(szBuf, Project_Info_Begin, "%s", PI_PublicDef_Name, PI_PublicDef_Path, PI_PublicDef_GUID); nibf(Info);
	(pt.strInfo += Project_Info_End) += "\n";
	const ConfigMatrixRow p1Mat[] = {		//HoloLens					Win32						Win64
	/*DebugGame Editor*/	ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCDebug, PPWin32),		mtxi(PCDebug, PPx64)),
	/*DebugGame*/			ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCDebug, PPWin32),		mtxi(PCDebug, PPx64)),
	/*Development Editor*/	ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPx64)),
	/*Development*/			ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPx64)),
	/*Shipping*/			ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPx64))
	};
	FillConfig(pt, szBuf, p1Mat, PI_PublicDef_GUID);
	sprintf_s(szBuf, Tab2_Equal, PI_PublicDef_GUID, "%s"); nibf(Nested);
	m_vecMyProjects.push_back(pt);

	sprintf_s(szBuf, Project_Info_Begin, "%s", PI_DataTables_Name, PI_DataTables_Path, PI_DataTables_GUID); nibf(Info);
	(pt.strInfo += Project_Dep_Begin) += "\n";
	sprintf_s(szBuf, Tab2_Equal, PI_PublicDef_GUID, PI_PublicDef_GUID); nabf(Info);
	(pt.strInfo += Project_Dep_End) += "\n";
	(pt.strInfo += Project_Info_End) += "\n";
	const ConfigMatrixRow p2Mat[] = {		//HoloLens					Win32						Win64
	/*DebugGame Editor*/	ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCDebug, PPWin32),		mtxi(PCDebug, PPx64)),
	/*DebugGame*/			ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCDebug, PPWin32),		mtxi(PCDebug, PPx64)),
	/*Development Editor*/	ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPx64)),
	/*Development*/			ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPx64)),
	/*Shipping*/			ConfigMatrixRow(mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPWin32),	mtxi(PCRelease, PPx64))
	};
	FillConfig(pt, szBuf, p2Mat, PI_DataTables_GUID);
	sprintf_s(szBuf, Tab2_Equal, PI_DataTables_GUID, "%s"); nibf(Nested);
	m_vecMyProjects.push_back(pt);
}

bool Helper::InitFile()
{
	return false;
}

void Helper::FillConfig(ProjectText& pt, char* szBuf, const ConfigMatrixRow* pMat, const char* szGuid)
{
	bool bInit = false;
	for (int i = 0; i < GCNum; ++i)
	{
		const char* szConfig = c_szGameConfigs[i];
		for (int j = 0; j < GPNum; ++j)
		{
			const char* szPlatform = c_szGamePlatforms[j];
			for (const char* szSub : c_szGameSubs)
			{
				int iItem = pMat[i].iItems[j];
				sprintf(szBuf, Config_Line, szGuid, szConfig, szPlatform, szSub,
					c_szProjectConfigs[iItem >> 1], c_szProjectPlatforms[iItem & 1]);
				if (bInit) nabf(Config);
				else { nibf(Config); bInit = true; }
			}
		}
	}
}