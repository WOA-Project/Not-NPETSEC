///////////////////////////////////////////////////////////////////////////////////////
//
// NPETSEC
//
// Bootstraps the Modem subcription activation service
//
///////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "NPETSEC.h"
#include <fstream>

using namespace std;

const wchar_t* Files [30] = { 
	L"\\Microsoft\\Microsoft.PVK",
	L"\\QCOM\\BT.Provision",
	L"\\QCOM\\WLAN.Provision",
	L"\\MMO\\CoverColor.txt",
	L"\\MMO\\customer_nvi_log.txt",
	L"\\MMO\\simlock\\sign",
	L"\\MMO\\simlock\\keys",
	L"\\MMO\\simlock\\lock",
	L"\\MMO\\product.dat",
	L"\\MMO\\certs\\hwc",
	L"\\MMO\\certs\\ccc",
	L"\\MMO\\certs\\npc",
	L"\\MMO\\certs\\rdc",
	L"\\MMO\\certs\\devcert.bin",
	L"\\MMO\\certs\\mirlink.bin",
	L"\\MMO\\RegScreen\\imagelabel_dark.png",
	L"\\MMO\\RegScreen\\imagelabel_light.png",
	L"\\MMO\\simlock\\unlock.bin",
	L"\\MMO\\ssdhash.bin",
	L"\\MMO\\certs\\label_data.bin",
	L"\\MMO\\RegScreen\\coo.txt",
	L"\\MMO\\imageimeibarcode.png",
	L"\\MMO\\fsghash.bin",
	L"\\MMO\\Label\\panel.ver",
	L"\\MMO\\Label\\label.ver",
	L"\\MMO\\testfile.txt",
	L"\\MMO\\testfilerestricted.txt",
	L"\\MMO\\Label\\panelmdcl.ver",
	L"\\MMO\\Label\\labelmdcl.ver",
	L"\\MMO\\simlock\\sign2"
};

///////////////////////////////////////////////////////////////////////////////////////
//
// Activates the Modem subscription
//
// We launch the activation tool in order to activate the modem subscription.
// We also enable extra logging if the user requested us to log items into a text file.
//
// We do not call activate for every function because the Modem might not be ready when
// APPerso requests us to Initialize or read a DPP item.
//
// Only activate the subscription when we're called from ValidateMultiSim.
//
// If no Activation is required, we will never be called.
//
///////////////////////////////////////////////////////////////////////////////////////
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

	// Control experimental dual sim mitigation behind a registry flag
	nResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\OEM\\NPETSEC", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	cbData = sizeof(DWORD);

	bool DualSimMitigation = FALSE;

	if (nResult == ERROR_SUCCESS)
	{
		nResult = ::RegQueryValueEx(hKey, L"EnableDualSimMitigation", NULL, NULL, (LPBYTE)&dwData, &cbData);
		RegCloseKey(hKey);

		if (nResult == ERROR_SUCCESS)
			if (dwData == 1)
				DualSimMitigation = TRUE;
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

	if (DualSimMitigation)
	{
		_popen("\\Windows\\System32\\netsh.exe mbn set dataenablement interface=* profileset=all mode=yes", "r");
		_popen("\\Windows\\System32\\netsh.exe mbn set dataenablement interface=*2 profileset=all mode=yes", "r");
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Returns the file size of an item in DPP
//
///////////////////////////////////////////////////////////////////////////////////////
__int64 NPETSEC_DppItemSize(unsigned int val1, int* val2)
{
	const wchar_t *file = Files[val1];

	__int64 ret = 0;

	try
	{
		ifstream ifs("C:\\DPP" + *file, ios::binary | ios::ate);
		ifstream::pos_type pos = ifs.tellg();
		int bytecount = pos;
		val2 = &bytecount;
		ifs.close();
	}
	catch (int e)
	{
		ret = e;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Reads a file from DPP
//
///////////////////////////////////////////////////////////////////////////////////////
__int64 NPETSEC_DppItemRead(unsigned int a1, unsigned int a2, char* a3, unsigned int a4)
{
	const wchar_t* file = Files[a1];

	__int64 ret = 0;

	try
	{
		ifstream ifs("C:\\DPP" + *file, ios::binary | ios::in);
		ifstream::pos_type pos = ifs.tellg();
		ifs.seekg(0, ios::beg);
		ifs.read(a3, a4);
		ifs.close();
	}
	catch (int e)
	{
		ret = e;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Validates Modem subscription externally
//
///////////////////////////////////////////////////////////////////////////////////////
__int64 NPETSEC_ValidateMultiSim(__int64 a1, __int64 a2)
{
	//
	// When APPerso calls this function, the Modem is ready and is waiting to be activated
	// Activate the Modem subscription
	//
	return Activate();
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Not implemented
//
///////////////////////////////////////////////////////////////////////////////////////

__int64 NPETSEC_Init(__int64 val1)
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

void    NPETSEC_Deinit()
{
	
}