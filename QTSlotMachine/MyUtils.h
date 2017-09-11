#pragma once

#include <stdlib.h> // NULL

#include <iostream> // cout
#include <vector>
using namespace std;

class MyUtils
{
public:
    MyUtils(void);
    ~MyUtils(void);
    // Wait indefinitely until user strikes any key
    //static void waitForKeystroke(const stringstream &ss, const char* i_title=NULL);
    static void waitForKeystroke(const char* i_msg=NULL, const char* i_title=NULL);
};
