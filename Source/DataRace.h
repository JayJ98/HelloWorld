/*
  ==============================================================================

    DataRace.h
    Created: 21 Dec 2023 9:40:03pm
    Author:  Jayson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

struct Data
{
    int64 a = 0;
    int64 b = 0;
    int64 c = 0;
    int64 d = 0;
    
    bool operator==(const Data& other);
    bool operator!=(const Data& other);
};

const Data a = { 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa };
const Data b = { 0xbbbbbbbb, 0xbbbbbbbb, 0xbbbbbbbb, 0xbbbbbbbb };
const Data c = { 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc };
const Data d = { 0xdddddddd, 0xdddddddd, 0xdddddddd, 0xdddddddd };

struct A : Thread
{
    A(Data& _x) : Thread("A"), x(_x) { startThread(); }
    ~A() { stopThread(100); }
    void run() override;
    Data& x;
};

struct B : Thread
{
    B(Data& _x) : Thread("B"), x(_x){ startThread(); }
    ~B() { stopThread(100); }
    void run() override;
    Data& x;
};

struct Test
{
    Data data;
    A a { data };
    B b { data };
    
};


//===========================================================================
struct LockingStuct
{
    void threadAFunc();
    void threadBFunc();
    int dataMember = 0;
    CriticalSection criticalSection;
};
