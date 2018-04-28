#include "AsyncoTaskManager.h"
#include <stdio.h>

int main()
{
    volatile auto instance = AsyncoTaskManager::GetInstance();
    return 0;
}

