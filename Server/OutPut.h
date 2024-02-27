#pragma once

void onConnected(unsigned int conv)
{
    std::cout << conv << " connected" << std::endl;
}

void onClosed(unsigned int conv)
{
    std::cout << conv << " onClosed" << std::endl;
}