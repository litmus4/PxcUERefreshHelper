// PxcUERefreshHelper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Helper.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Pxc UE Sln Refresh Helper" << std::endl;
    std::cout << "Parsing UE4Game.sln..." << std::endl;

    if (argc < 2)
    {
        std::cout << "Wrong Args !" << std::endl;
        return 0;
    }

    Helper helper(argv[1]);
    bool bSuccess = false;
    do
    {
        if (!helper.Init())
            break;

        if (!helper.Run())
            break;

        if (!helper.Save())
            break;

        bSuccess = true;
    } while (false);

    if (bSuccess)
        std::cout << "Success !!!" << std::endl;
    else
        std::cout << "Failed !" << std::endl;
    return 0;
}
