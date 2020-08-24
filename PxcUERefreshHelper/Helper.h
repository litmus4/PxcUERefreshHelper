#pragma once
#include <string>
#include <vector>
#include <list>
#include <set>

class Helper
{
public:
	struct ProjectText
	{
		std::string strInfo;
		std::string strConfig;
		std::string strNested;
	};

	struct ConfigMatrixRow//此处参数与元素数应与c_szGamePlatforms元素数相同
	{
		ConfigMatrixRow(int i0, int i1, int i2)
		{ iItems[0] = i0; iItems[1] = i1; iItems[2] = i2; }
		int iItems[3];
	};

	struct LocatingParam
	{
		LocatingParam(const char* szItemName, int iIndex)
			: strItemName(szItemName), iItemIndex(iIndex)
		{}
		LocatingParam(const char* szItemName, const char* szItemKey)
			: strItemName(szItemName), strItemKey(szItemKey), iItemIndex(0)
		{}

		std::string strItemName;
		int iItemIndex;
		std::string strItemKey;
	};

public:
	Helper();
	Helper(const std::string& strFilePath);
	~Helper();

	bool Init();
	bool Run();
	bool Save();

private:
	void InitInsertTexts();
	bool InitFile();

	void FillConfig(ProjectText& pt, char* szBuf, const ConfigMatrixRow* pMat, const char* szGuid);

	bool ReadyUEGameProjectDep();
	bool ReadyProjectInfo();
	bool ReadyProjectConfig();
	bool ReadyProjectNested();

	void Insert();

	std::list<std::string>::iterator LocateInsertAfter(const std::vector<LocatingParam>& vecParams, bool bInternal = false);
	bool FindItemPositively(const LocatingParam& param,
		std::list<std::string>::iterator& itLine, int iLevel, int* pItemIndex = nullptr);
	bool CheckKey(const LocatingParam& param, const std::string& strLine);
	bool GetStringKeyAfterEqual(const std::string& strLine, int iKeyIndex, std::string& strOutKey);
	std::string GetItemSubKey(const std::string& strLine);
	std::string CastOffSkin(const std::string& strIn, int iLayerNum);

	std::string m_strFilePath;
	std::string m_strUEGameProjDep;
	std::vector<ProjectText> m_vecMyProjects;
	std::list<std::string> m_lisLines;

	std::list<std::string>::iterator m_itUEGameProjDepReady;
	std::list<std::string>::iterator m_itProjInfoReady;
	std::list<std::string>::iterator m_itProjConfigReady;
	std::list<std::string>::iterator m_itProjNestedReady;
	std::set<std::string*> m_setInsertedPtrs;
};