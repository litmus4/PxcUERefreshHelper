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

public:
	Helper();
	Helper(const std::string& strFileName);
	~Helper();

	bool Init();

private:
	void InitInsertTexts();
	bool InitFile();

	std::string m_strFileName;
	std::string m_strUEGameProjDep;
	std::vector<ProjectText> m_vecMyProjects;
};