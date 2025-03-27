#pragma once

#ifndef _EXPORT_H
#define _EXPORT_H

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

extern std::string outputFileName;

std::string GetCurrentTimestamp();
std::string GetUnixTimestamp();


#endif // !1

