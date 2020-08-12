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

bool Helper::InsertUEGameProjectDep()
{
	//
	return false;
}

std::list<std::string>::iterator Helper::LocateInsertAfter(const std::vector<LocatingParam>& vecParams)
{
	std::list<std::string>::iterator itLine = m_lisLines.begin();
	std::list<std::string>::reverse_iterator ritLine = m_lisLines.rbegin();

	int iLevel = 0;
	std::vector<LocatingParam>::const_iterator iter = vecParams.begin();
	for (; iter != vecParams.end(); iter++)
	{
		int iItemIndex = (iter->iItemIndex < 0 ? abs(iter->iItemIndex) - 1 : iter->iItemIndex);
		int iItemCount = 0;
		bool bFind = false;
		if (iter->iItemIndex < 0)
		{
			for (; ritLine != m_lisLines.rend(); ritLine++)//FLAGJK ���Level>0��ֻ�������ڶ����ϼ�Level��ǰ
			{
				size_t ipos = ritLine->find_first_not_of('\t');
				if (ipos != iLevel) continue;

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
			for (; itLine != m_lisLines.end(); itLine++)
			{
				size_t ipos = itLine->find_first_not_of('\t');
				if (ipos != iLevel) continue;

				bool bCheck = false;
				if (!iter->strItemName.empty())
				{
					std::string strCurItemName;
					size_t ipos2 = itLine->find_first_of('(', ipos);
					if (ipos2 != std::string::npos)
						strCurItemName = itLine->substr(ipos, ipos2 - ipos);
					else
						strCurItemName = itLine->substr(ipos);
					if (strCurItemName == iter->strItemName)
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

		if (bFind)
		{
			if (iter->iItemIndex < 0)
			{
				itLine = ritLine.base();
				//FLAGJK ����Ҫ�ҵ�itLine��Ӧ�Ŀ�ʼ��ǵ����У�����¼�������������ʱ��ֵ��itLine
			}
			else
			{
				//FLAGJK �����ȱ���һ��itLine��Ȼ���ҵ�itLine��Ӧ�Ľ�����ǵ���һ�У���ֱ�Ӹ�ֵ��itLine������¼�������������ʱ�򽫱��ݸ�ֵ��itLine
				ritLine = std::list<std::string>::reverse_iterator(itLine);
			}
		}
		else
			return m_lisLines.end();
		iLevel++;
	}
}