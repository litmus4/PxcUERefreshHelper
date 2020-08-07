#pragma once
#include <string>
#include <vector>

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

public:
	Helper();
	Helper(const std::string& strFilePath);
	~Helper();

	bool Init();

private:
	void InitInsertTexts();
	bool InitFile();

	void FillConfig(ProjectText& pt, char* szBuf, const ConfigMatrixRow* pMat, const char* szGuid);

	std::string m_strFilePath;
	std::string m_strUEGameProjDep;
	std::vector<ProjectText> m_vecMyProjects;
};