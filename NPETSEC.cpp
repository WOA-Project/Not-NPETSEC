/*
   NPETSEC - Modem Subscription activation bootstrapper
*/

#include "pch.h"
#include "NPETSEC.h"
#include <fstream>

using namespace std;

/*
   Activates the Modem subscription

   We launch the activation tool in order to activate the modem subscription.
   We also enable extra logging if the user requested us to log items into a text file.
*/

__int64 Activate()
{
	// Control logging behind a registry flag
	HKEY hKey;
	LSTATUS nResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\OEM\\NPETSEC", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	DWORD dwData;
	DWORD cbData = sizeof(DWORD);

	bool Logging = FALSE;

	if (nResult == ERROR_SUCCESS)
	{
		nResult = ::RegQueryValueEx(hKey, L"EnableLogging", NULL, NULL, (LPBYTE)&dwData, &cbData);
		RegCloseKey(hKey);

		if (nResult == ERROR_SUCCESS)
			if (dwData == 1)
				Logging = TRUE;
	}

	ofstream logFile;

	// Proceed with initialization

	if (Logging)
	{
		logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
		logFile << "> Activating RIL Subscription" << "\n";
	}

	char psBuffer[128];
	FILE * iopipe;
	if ((iopipe = _popen("\\Windows\\System32\\RilServiceActivation.exe", "r")) == NULL)
	{
		if (Logging)
		{
			logFile << "< Cannot find the RIL Subscription activation utility. Leaving..." << "\n";
			logFile.close();
		}
		return 1;
	}

	while (!feof(iopipe))
	{
		if (fgets(psBuffer, 128, iopipe) != NULL)
			if (Logging)
				logFile << "- " << psBuffer;
	}

	if (Logging)
	{
		logFile << "< Subscription activated" << "\n";
		logFile.close();
	}

	return 0;
}

/*
   We do not call activate for every function because the Modem might not be ready when APPerso requests us to Initialize
   Or read a DPP item.

   Only activate the subscription when we're called from ValidateMultiSim.

   If no Activation is required, we will never be called.
*/

__int64 NPETSEC_Init(__int64 val1) 
{
	return 0;
}

__int64 NPETSEC_DppItemSize(unsigned int val1, int* val2)
{
	return 0;
}

__int64 NPETSEC_UnlockSimlock(unsigned int val1, __int64 val2)
{
	return 0;
}

__int64 NPETSEC_GetSimlockStatus(__int64 val1)
{
	return 0;
}

__int64 NPETSEC_DppItemRead(unsigned int a1, unsigned int a2, __int64 a3, unsigned int a4)
{
	return 0;
}

__int64 NPETSEC_ValidateMultiSim(__int64 a1, __int64 a2)
{
	//
	// When APPerso calls this function, the Modem is ready and is waiting to be activated
	// Activate the Modem subscription
	//
	return Activate();
}

void    NPETSEC_Deinit()
{
	
}