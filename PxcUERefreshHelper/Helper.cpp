#include "Helper.h"

#define Project_Dep_Begin "\tProjectSection(ProjectDependencies) = postProject"
#define Project_Dep_End "\tEndProjectSection"
#define Tab2_Equal "\t\t{%s} = {%s}"

#define Project_Info_Begin "Project(\"{%s}\") = \"%s\", \"%s\", \"{%s}\""
#define Project_Info_End "EndProject"

#define Config_Line "{%s}.%s|%s.%s = %s|%s"

//FLAGJK

Helper::Helper()
{
	m_strFileName = "UE4Game";
}

Helper::Helper(const std::string& strFileName)
{
	m_strFileName = strFileName;
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
	//
}

bool Helper::InitFile()
{
	return false;
}