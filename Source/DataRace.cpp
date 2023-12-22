/*
  ==============================================================================

    DataRace.cpp
    Created: 21 Dec 2023 9:40:03pm
    Author:  Jayson

  ==============================================================================
*/

#include "DataRace.h"
bool Data::operator==(const Data& other)
{
    return a == other.a && b == other.b;
}

bool Data::operator!=(const Data& other)
{
    return !(*this == other);
}

void A::run()
{
    while( true )
    {
        x = a;
        if (threadShouldExit() )
            break;
        
        x = b;
        
        //compare
        if( x != a && x != b && x != c && x != d)
        {
            auto _local = x;
            std::cout << "A: " << "x = { .a =" << String::toHexString(_local.a)
            << ", .b=" << String::toHexString(_local.b) << " "
            << ", .c=" << String::toHexString(_local.c) << " "
            << ", .d=" << String::toHexString(_local.d) << " }" <<std::endl;
        }
        
        wait(3);
    }
}

void B::run()
{
    while( true )
    {
        x = c;
        if (threadShouldExit() )
            break;
        
        x = d;
        
        //compare
        if( x != a && x != b && x != c && x != d)
        {
            auto _local = x;
            std::cout << "B: " << "x = { .a =" << String::toHexString(_local.a)
            << ", .b=" << String::toHexString(_local.b) << " "
            << ", .c=" << String::toHexString(_local.c) << " "
            << ", .d=" << String::toHexString(_local.d) << " }" <<std::endl;
        }
        
        wait(3);
    }
}

void LockingStuct::threadAFunc()
{
    {
        const ScopedLock sl(criticalSection);
        dataMember = 2;
    }
    std::cout << "thread A Func " << dataMember << std::endl;
}

void LockingStuct::threadBFunc()
{
    {
        const ScopedLock sl(criticalSection);
        dataMember = 0;
    }
    std::cout << "thread B Func " << dataMember << std::endl;
}
