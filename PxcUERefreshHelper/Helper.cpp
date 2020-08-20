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

bool Helper::Run()
{
	return false;
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
	FILE* pFile = fopen(m_strFilePath.c_str(), "r");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		long lSize = ftell(pFile);
		rewind(pFile);
		char* pFileBuf = new char[lSize];
		fread(pFileBuf, 1, lSize, pFile);

		char* pBegin = pFileBuf;
		char* pEnd = strchr(pFileBuf, '\n');
		while (pEnd)
		{
			std::string strLine;
			strLine.insert(0, pBegin, pEnd - pBegin);
			if (!strLine.empty())
				m_lisLines.push_back(strLine);
			pBegin = pEnd + 1;
			pEnd = strchr(pEnd + 1, '\n');
		}

		delete[] pFileBuf;
		return !m_lisLines.empty();
	}
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

bool Helper::ReadyUEGameProjectDep()
{
	std::vector<LocatingParam> vecParams;
	vecParams.push_back(LocatingParam("Project", "UE4Game"));
	m_itUEGameProjDepReady = LocateInsertAfter(vecParams, true);
	return (m_itUEGameProjDepReady != m_lisLines.end());
}

bool Helper::ReadyProjectInfo()
{
	std::vector<LocatingParam> vecParams;
	vecParams.push_back(LocatingParam("Project", -1));
	m_itProjInfoReady = LocateInsertAfter(vecParams);
	return (m_itProjInfoReady != m_lisLines.end());
}

bool Helper::ReadyProjectConfig()
{
	std::vector<LocatingParam> vecParams;
	vecParams.push_back(LocatingParam("Global", 0));
	vecParams.push_back(LocatingParam("GlobalSection", "ProjectConfigurationPlatforms"));
	m_itProjConfigReady = LocateInsertAfter(vecParams, true);
	return (m_itProjConfigReady != m_lisLines.end());
}

bool Helper::ReadyProjectNested()
{
	std::vector<LocatingParam> vecParams;
	vecParams.push_back(LocatingParam("Global", 0));
	vecParams.push_back(LocatingParam("GlobalSection", "NestedProjects"));
	m_itProjNestedReady = LocateInsertAfter(vecParams, true);
	return (m_itProjNestedReady != m_lisLines.end());
}

std::list<std::string>::iterator Helper::LocateInsertAfter(const std::vector<LocatingParam>& vecParams, bool bInternal)
{
	std::list<std::string>::iterator itLine = m_lisLines.begin();
	std::list<std::string>::reverse_iterator ritLine = m_lisLines.rbegin();
	std::list<std::string>::iterator itBack = m_lisLines.end();

	int iLevel = 0;
	std::vector<LocatingParam>::const_iterator iter = vecParams.begin();
	for (; iter != vecParams.end(); iter++)
	{
		int iItemIndex = (iter->iItemIndex < 0 ? abs(iter->iItemIndex) - 1 : iter->iItemIndex);
		bool bFind = false;
		if (iter->iItemIndex < 0 && iter->strItemKey.empty())
		{
			int iItemCount = 0;
			for (; ritLine != m_lisLines.rend(); ritLine++)
			{
				size_t ipos = ritLine->find_first_not_of('\t');
				switch (iLevel)
				{
				case 1: case 2: case 3: case 4:
					if (ipos < iLevel) break;
				case 0:
					if (ipos != iLevel) continue;
				}

				bool bCheck = false;
				if (!iter->strItemName.empty())
				{
					std::string strCurItemName;
					size_t ipos2 = ritLine->find_first_of('(', ipos);
					if (ipos2 != std::string::npos)
						strCurItemName = ritLine->substr(ipos, ipos2 - ipos);
					else
						strCurItemName = ritLine->substr(ipos);
					if (strCurItemName == "End" + iter->strItemName)
						bCheck = true;
				}
				else
					bCheck = true;

				if (bCheck)
				{
					if (iItemCount == iItemIndex)
					{
						bFind = true;
						break;
					}
					iItemCount++;
				}
			}
		}
		else
		{
			if (iLevel > 0) itLine = itBack;

			bFind = FindItemPositively(*iter, itLine, iLevel, &iItemIndex);
		}

		if (bFind)
		{
			if (iter->iItemIndex < 0)
			{
				itLine = ritLine.base();
				//这里要找到itLine对应的开始标记的那行，如果下级搜索是正，则到时候赋值给itLine
				(itBack = itLine)--;//FLAGJK ?
				itBack--;
				while (itBack->find_first_not_of('\t') > iLevel)
					itBack--;
			}
			else
			{
				//这里先备份一个itLine，然后找到itLine对应的结束标记的下一行，并直接赋值给itLine，如果下级搜索是正，则到时候将备份赋值给itLine
				itBack = itLine++;
				while (itLine->find_first_not_of('\t') > iLevel)
					itLine++;
				itLine++;
				ritLine = std::list<std::string>::reverse_iterator(itLine);
			}
		}
		else
			return m_lisLines.end();
		iLevel++;
	}

	if (bInternal) itLine--;
	return itLine;
}

bool Helper::FindItemPositively(const LocatingParam& param, std::list<std::string>::iterator& itLine, int iLevel, int* pItemIndex)
{
	int iItemIndex = (pItemIndex ? *pItemIndex : param.iItemIndex);
	int iItemCount = 0;
	for (; itLine != m_lisLines.end(); itLine++)
	{
		size_t ipos = itLine->find_first_not_of('\t');
		switch (iLevel)
		{
		case 1: case 2: case 3: case 4:
			if (ipos < iLevel) break;
		case 0:
			if (ipos != iLevel) continue;
		}

		bool bCheck = false;
		if (!param.strItemName.empty())
		{
			std::string strCurItemName;
			size_t ipos2 = itLine->find_first_of('(', ipos);
			if (ipos2 != std::string::npos)
				strCurItemName = itLine->substr(ipos, ipos2 - ipos);
			else
				strCurItemName = itLine->substr(ipos);
			if (strCurItemName == param.strItemName)
				bCheck = true;
		}
		else
			bCheck = true;

		if (bCheck)
		{
			if (param.strItemKey.empty() ? iItemCount == iItemIndex : CheckKey(param, *itLine))
				return true;
			iItemCount++;
		}
	}
	return false;
}

bool Helper::CheckKey(const LocatingParam& param, const std::string& strLine)
{
	if (param.strItemName == "Project")
	{
		std::string strKey;
		if (GetStringKeyAfterEqual(strLine, 0, strKey))
			return (strKey == param.strItemKey);
	}
	else if (param.strItemName == "GlobalSection")
	{
		std::string&& strKey = GetItemSubKey(strLine);
		if (!strKey.empty())
			return (strKey == param.strItemKey);
	}
	else
	{
		size_t ipos = strLine.find_first_of('=');
		if (ipos == std::string::npos)
			return false;

		ipos = strLine.find_first_not_of(' ', ipos + 1);
		if (ipos != std::string::npos)
		{
			std::string strKey = strLine.substr(ipos);
			return (strKey == param.strItemKey);
		}
	}
	return false;
}

bool Helper::GetStringKeyAfterEqual(const std::string& strLine, int iKeyIndex, std::string& strOutKey)
{
	size_t ipos = strLine.find_first_of('=');
	if (ipos == std::string::npos)
		return false;

	std::string strKeyLine = strLine.substr(ipos + 1);
	ipos = strKeyLine.find_first_of('\"');
	int i = 0;
	while (ipos != std::string::npos)
	{
		size_t ipos2 = strKeyLine.find_first_of('\"', ipos + 1);

		if (i == iKeyIndex)
		{
			if (ipos2 == std::string::npos)
				strOutKey = strKeyLine.substr(ipos + 1);
			else if (ipos2 - ipos > 1)
				strOutKey = strKeyLine.substr(ipos + 1, ipos2 - ipos - 1);
			return true;
		}

		if (ipos2 == std::string::npos)
			break;
		ipos = strKeyLine.find_first_of('\"', ipos2 + 1);
		i++;
	}
	return false;
}

std::string Helper::GetItemSubKey(const std::string& strLine)
{
	size_t ipos = strLine.find_first_of('(');
	if (ipos == std::string::npos)
		return "";

	size_t ipos2 = strLine.find_first_of(')', ipos + 1);
	if (ipos2 == std::string::npos)
		return "";

	if (ipos2 - ipos > 1)
		return strLine.substr(ipos + 1, ipos2 - ipos - 1);
	return "";
}

std::string Helper::CastOffSkin(const std::string& strIn, int iLayerNum)
{
	if (iLayerNum <= 0) return strIn;
	if (strIn.size() <= iLayerNum * 2)
		return "";
	return strIn.substr(iLayerNum, strIn.size() - iLayerNum * 2);
}