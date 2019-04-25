#include "pch.h"
#include "NPETSEC.h"

#include <iostream>
#include <fstream>
#include <sstream>

HANDLE hDLL;

using namespace std;

ofstream logFile;

LPNPETSEC_INIT Init;
LPNPETSEC_DPPITEMSIZE DppItemSize;
LPNPETSEC_UNLOCKSIMLOCK UnlockSimlock;
LPNPETSEC_GETSIMLOCKSTATUS GetSimlockStatus;
LPNPETSEC_DPPITEMREAD DppItemRead;
LPNPETSEC_VALIDATEMULTISIM ValidateMultiSim;
LPNPETSEC_DEINIT Deinit;

std::string ToString(__int64 obj)
{
	std::stringstream stream;
	stream << obj;
	std::string strValue(stream.str());
	return strValue;
}

__int64 NPETSEC_Init(__int64 val1) 
{
	// Initialize stubbed library entry points
	hDLL = LoadLibrary(L"NPETSEC2.DLL");

	LPNPETSEC_INIT Init = (LPNPETSEC_INIT)GetProcAddress((HMODULE)hDLL, "NPETSEC_Init");

	// Proceed with initialization
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_Init";
	logFile << "\n";
	logFile << ToString(val1);
	logFile << "\n";

	__int64 ret = Init(val1);

	logFile << "< NPETSEC_Init";
	logFile << "\n";
	logFile << ToString(ret);

	logFile << "\n";
	logFile << "Bypassing error by returning 0";

	logFile << "\n";
	logFile.close();

	return 0;//ret;
}

__int64 NPETSEC_DppItemSize(unsigned int val1, int* val2)
{
	LPNPETSEC_DPPITEMSIZE DppItemSize = (LPNPETSEC_DPPITEMSIZE)GetProcAddress((HMODULE)hDLL, "NPETSEC_DppItemSize");

	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_DppItemSize";
	logFile << "\n";
	logFile << val1;
	logFile << "\n";
	logFile << *val2;
	logFile << "\n";

	__int64 ret = DppItemSize(val1, val2);

	logFile << "< NPETSEC_DppItemSize";
	logFile << "\n";
	logFile << ToString(ret);

	logFile << "\n";
	logFile << "Bypassing error by returning 0";

	logFile << "\n";
	logFile.close();

	return 0;//ret;
}

__int64 NPETSEC_UnlockSimlock(unsigned int val1, __int64 val2)
{
	LPNPETSEC_UNLOCKSIMLOCK UnlockSimlock = (LPNPETSEC_UNLOCKSIMLOCK)GetProcAddress((HMODULE)hDLL, "NPETSEC_UnlockSimlock");

	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_UnlockSimlock";
	logFile << "\n";
	logFile << val1;
	logFile << "\n";
	logFile << ToString(val2);
	logFile << "\n";

	__int64 ret = UnlockSimlock(val1, val2);

	logFile << "< NPETSEC_UnlockSimlock";
	logFile << "\n";
	logFile << ToString(ret);

	logFile << "\n";
	logFile << "Bypassing error by returning 0";

	logFile << "\n";
	logFile.close();

	return 0;//ret;
}

__int64 NPETSEC_GetSimlockStatus(__int64 val1)
{
	LPNPETSEC_GETSIMLOCKSTATUS GetSimlockStatus = (LPNPETSEC_GETSIMLOCKSTATUS)GetProcAddress((HMODULE)hDLL, "NPETSEC_GetSimlockStatus");

	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_GetSimlockStatus";
	logFile << "\n";
	logFile << ToString(val1);
	logFile << "\n";

	__int64 ret =  GetSimlockStatus(val1);

	logFile << "< NPETSEC_GetSimlockStatus";
	logFile << "\n";
	logFile << ToString(ret);

	logFile << "\n";
	logFile << "Bypassing error by returning 0";

	logFile << "\n";
	logFile.close();

	return 0;//ret;
}

__int64 NPETSEC_DppItemRead(unsigned int a1, unsigned int a2, __int64 a3, unsigned int a4)
{
	LPNPETSEC_DPPITEMREAD DppItemRead = (LPNPETSEC_DPPITEMREAD)GetProcAddress((HMODULE)hDLL, "NPETSEC_DppItemRead");

	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_DppItemRead";
	logFile << "\n";
	logFile << a1;
	logFile << "\n";
	logFile << a2;
	logFile << "\n";
	logFile << ToString(a3);
	logFile << "\n";
	logFile << a4;
	logFile << "\n";

	__int64 ret =  DppItemRead(a1, a2, a3, a4);

	logFile << "< NPETSEC_DppItemRead";
	logFile << "\n";
	logFile << ToString(ret);
	logFile << "\n";

	logFile << "Bypassing error by returning 0";
	logFile << "\n";

	logFile.close();

	return 0;//ret;
}

__int64 NPETSEC_ValidateMultiSim(__int64 a1, __int64 a2)
{
	LPNPETSEC_VALIDATEMULTISIM ValidateMultiSim = (LPNPETSEC_VALIDATEMULTISIM)GetProcAddress((HMODULE)hDLL, "NPETSEC_ValidateMultiSim");

	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_ValidateMultiSim";
	logFile << "\n";
	logFile << ToString(a1);
	logFile << "\n";
	logFile << ToString(a2);
	logFile << "\n";

	__int64 ret = ValidateMultiSim(a1, a2);

	logFile << "< NPETSEC_ValidateMultiSim";
	logFile << "\n";
	logFile << ToString(ret);

	logFile << "\n";
	logFile << "Bypassing error by returning 0";

	logFile << "\n";
	logFile.close();

	return 0;//ret;
}

void    NPETSEC_Deinit()
{
	LPNPETSEC_DEINIT Deinit = (LPNPETSEC_DEINIT)GetProcAddress((HMODULE)hDLL, "NPETSEC_Deinit");

	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_Deinit";
	logFile << "\n";

	Deinit();

	logFile << "< NPETSEC_Deinit";
	logFile << "\n";
	logFile.close();
}