﻿// PxcUERefreshHelper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Helper.h"
#include <iostream>

int main()
{
    std::cout << "Pxc UE Sln Refresh Helper" << std::endl;
    std::cout << "Parsing UE4Game.sln..." << std::endl;

    Helper helper;
    helper.Init();
}