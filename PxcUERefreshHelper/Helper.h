#pragma once
#include <string>
#include <vector>
#include <list>

class Helper
{
public:
	struct ProjectText
	{
		std::string strInfo;
		std::string strConfig;
		std::string strNested;
	};

	struct ConfigMatrixRow//�˴�������Ԫ����Ӧ��c_szGamePlatformsԪ������ͬ
	{
		ConfigMatrixRow(int i0, int i1, int i2)
		{ iItems[0] = i0; iItems[1] = i1; iItems[2] = i2; }
		int iItems[3];
	};

	struct LocatingParam
	{
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

private:
	void InitInsertTexts();
	bool InitFile();

	void FillConfig(ProjectText& pt, char* szBuf, const ConfigMatrixRow* pMat, const char* szGuid);

	bool InsertUEGameProjectDep();

	std::list<std::string>::iterator LocateInsertAfter(const std::vector<LocatingParam>& vecParams, bool bInternal = false);
	bool FindItemPositively(const LocatingParam& param, std::list<std::string>::iterator& itLine, int iLevel, int* pItemIndex);
	bool CheckKey(const LocatingParam& param, const std::string& strLine);

	std::string m_strFilePath;
	std::string m_strUEGameProjDep;
	std::vector<ProjectText> m_vecMyProjects;
	std::list<std::string> m_lisLines;
};