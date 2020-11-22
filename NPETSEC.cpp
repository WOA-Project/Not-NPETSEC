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
#include <string>
#include <iostream>

using namespace std;

//#define LOGGING_ENABLED

const wchar_t* Files[30] = {
	L"C:\\DPP\\Microsoft\\Microsoft.PVK",
	L"C:\\DPP\\QCOM\\BT.Provision",
	L"C:\\DPP\\QCOM\\WLAN.Provision",
	L"C:\\DPP\\MMO\\CoverColor.txt",
	L"C:\\DPP\\MMO\\customer_nvi_log.txt",
	L"C:\\DPP\\MMO\\simlock\\sign",
	L"C:\\DPP\\MMO\\simlock\\keys",
	L"C:\\DPP\\MMO\\simlock\\lock",
	L"C:\\DPP\\MMO\\product.dat",
	L"C:\\DPP\\MMO\\certs\\hwc",
	L"C:\\DPP\\MMO\\certs\\ccc",
	L"C:\\DPP\\MMO\\certs\\npc",
	L"C:\\DPP\\MMO\\certs\\rdc",
	L"C:\\DPP\\MMO\\certs\\devcert.bin",
	L"C:\\DPP\\MMO\\certs\\mirlink.bin",
	L"C:\\DPP\\MMO\\RegScreen\\imagelabel_dark.png",
	L"C:\\DPP\\MMO\\RegScreen\\imagelabel_light.png",
	L"C:\\DPP\\MMO\\simlock\\unlock.bin",
	L"C:\\DPP\\MMO\\ssdhash.bin",
	L"C:\\DPP\\MMO\\certs\\label_data.bin",
	L"C:\\DPP\\MMO\\RegScreen\\coo.txt",
	L"C:\\DPP\\MMO\\imageimeibarcode.png",
	L"C:\\DPP\\MMO\\fsghash.bin",
	L"C:\\DPP\\MMO\\Label\\panel.ver",
	L"C:\\DPP\\MMO\\Label\\label.ver",
	L"C:\\DPP\\MMO\\testfile.txt",
	L"C:\\DPP\\MMO\\testfilerestricted.txt",
	L"C:\\DPP\\MMO\\Label\\panelmdcl.ver",
	L"C:\\DPP\\MMO\\Label\\labelmdcl.ver",
	L"C:\\DPP\\MMO\\simlock\\sign2"
};

const char* ProductionDataIds[8] = {
	"HWID:",
	"LPSN:",
	"MC:",
	"BTR:",
	"CTR:",
	"ODR:",
	"TYPE:",
	"RECD:"
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
int Activate()
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
	FILE* iopipe;
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
int NPETSEC_DppItemSize(unsigned int fileIndex, int* outputSize)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_DppItemSize: file: " << fileIndex << "\n";
#endif
	HANDLE hFile = CreateFile(Files[fileIndex], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
#ifdef LOGGING_ENABLED
		logFile << "NPETSEC_DppItemSize: Fail 1\n";
		logFile.close();
#endif
		return 1;
	}

	LARGE_INTEGER lFileSize;
	BOOL bGetSize = GetFileSizeEx(hFile, &lFileSize);
	if (!bGetSize)
	{
#ifdef LOGGING_ENABLED
		logFile << "NPETSEC_DppItemSize: Fail 2\n";
		logFile.close();
#endif
		CloseHandle(hFile);
		return 1;
	}

	*outputSize = lFileSize.QuadPart;
	CloseHandle(hFile);

#ifdef LOGGING_ENABLED
	logFile << "NPETSEC_DppItemSize: Success: outputSize: " << *outputSize << "\n";
	logFile.close();
#endif
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Reads a file from DPP
//
///////////////////////////////////////////////////////////////////////////////////////
int NPETSEC_DppItemRead(unsigned int fileIndex, unsigned int startPosition, char* outputBuffer, unsigned int bufferSize)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_DppItemRead: file: " << fileIndex << " startPosition: " << startPosition << " buffersize: " << bufferSize << "\n";
#endif
	HANDLE hFile = CreateFile(Files[fileIndex], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
#ifdef LOGGING_ENABLED
		logFile << "NPETSEC_DppItemRead: Fail 1\n";
		logFile.close();
#endif
		return 1;
	}

	DWORD bytesRead = 0;
	OVERLAPPED ol = { 0 };

	DWORD dSeekFile = SetFilePointer(hFile, startPosition, NULL, FILE_BEGIN);
	if (dSeekFile == INVALID_SET_FILE_POINTER)
	{
#ifdef LOGGING_ENABLED
		logFile << "NPETSEC_DppItemRead: Fail seek\n";
		logFile.close();
#endif
		CloseHandle(hFile);
		return 1;
	}

	BOOL bReadFile = ReadFile(hFile, outputBuffer, bufferSize, &bytesRead, &ol);
	if (!bReadFile)
	{
#ifdef LOGGING_ENABLED
		logFile << "NPETSEC_DppItemRead: Fail 2\n";
		logFile.close();
#endif
		CloseHandle(hFile);
		return 1;
	}

	CloseHandle(hFile);

#ifdef LOGGING_ENABLED
	logFile << "NPETSEC_DppItemRead: Success\n";
	logFile.close();
#endif
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Validates Modem subscription externally
//
///////////////////////////////////////////////////////////////////////////////////////
int NPETSEC_ValidateMultiSim(__int64 a1, __int64 a2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ValidateMultiSim\n";
	logFile.close();
#endif
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

int NPETSEC_Init(__int64 val1)
{
	return 0;
}

int NPETSEC_UnlockSimlock(unsigned int val1, __int64 val2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_UnlockSimlock\n";
	logFile.close();
#endif
	return 0;
}

// OK - DONE
int NPETSEC_GetSimlockStatus(char* buffer)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_GetSimlockStatus\n";
	logFile.close();
#endif

	// size 18

	return 0;
}

void    NPETSEC_Deinit()
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_Deinit\n";
	logFile.close();
#endif
}

//
//
//

int NPETSEC_DeleteSecurityLogFile(unsigned int a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_DeleteSecurityLogFile\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_GetConfigAccessMode(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_GetConfigAccessMode\n";
	logFile.close();
#endif
	return 0;
}

// OK - DONE
int NPETSEC_GetProductionData(unsigned int productionDataId, char* buffer)
{
	char fakeStr[100] = { 0 };
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_GetProductionData productionDataId: " << productionDataId << "\n";
	logFile.close();
#endif

	if (productionDataId < 8)
	{
		const char* id = ProductionDataIds[productionDataId];
		std::fstream ProductFile;
		ProductFile.open("C:\\DPP\\MMO\\product.dat", std::ios::in);
		if (!ProductFile.is_open())
		{
			return 0;
		}

		std::string line;
		while (getline(ProductFile, line))
		{
			if (strncmp(line.c_str(), id, strlen(id)) == 0)
			{
				strcpy_s(fakeStr, line.c_str() + (int)strlen(id));
				break;
			}
		}
	}

	memcpy_s(buffer, 100, fakeStr, 100);

	return 0;
}

int NPETSEC_GetSecurityLogFileSize(unsigned int a1, __int64 a2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_GetSecurityLogFileSize\n";
	logFile.close();
#endif
	return 0;
}

// OK - DONE
int NPETSEC_GetSecurityMode(int* securityMode)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_GetSecurityMode\n";
	logFile.close();
#endif

	*securityMode = 1;

	return 0;
}

// OK - DONE
int NPETSEC_GetUefiCertStates(int states[5])
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_GetUefiCertStates\n";
	logFile.close();
#endif

	states[0] = 4;
	states[1] = 4;
	states[2] = 4;
	states[3] = 4;
	states[4] = 4;

	return 0;
}

// OK - DONE
int NPETSEC_IsProductionDone(int* productionState)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_IsProductionDone\n";
	logFile.close();
#endif

	*productionState = 0;

	return 0;
}

int NPETSEC_LastErrorStr()
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_LastErrorStr\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcEfsDataFileWrite(__int64 a1, __int64 a2, unsigned int a3)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcEfsDataFileWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcEfsFileDelete(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcEfsFileDelete\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcEfsFileRead(__int64 a1, __int64 a2, __int64 a3)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcEfsFileRead\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcEfsFileWrite(__int64 a1, __int64 a2, unsigned int a3)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcEfsFileWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcEfsItemFileWrite(__int64 a1, __int64 a2, unsigned int a3)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcEfsItemFileWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcNvItemIndexedRead(unsigned __int16 a1, unsigned __int16 a2, unsigned __int8 a3, __int64 a4, __int64 a5)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcNvItemIndexedRead\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcNvItemWrite(unsigned __int16 a1, unsigned __int16 a2, __int64 a3, unsigned int a4)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcNvItemWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcNvSpcUnlock(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcNvSpcUnlock\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcSubscriptionNvItemIndexedRead(unsigned __int16 a1, __int64 a2, unsigned __int8 a3, __int64 a4, __int64 a5)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcSubscriptionNvItemIndexedRead\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcSubscriptionNvItemRead(unsigned __int16 a1, __int64 a2, __int64 a3, __int64 a4)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcSubscriptionNvItemRead\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QcSubscriptionNvItemWrite(unsigned int a1, __int64 a2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QcSubscriptionNvItemWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_QfsEfsFileWrite(unsigned __int16 a1, __int64 a2, __int64 a3, unsigned int a4)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_QfsEfsFileWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_RdcPhoneReq(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_RdcPhoneReq\n";
	logFile.close();
#endif
	return 0;
}

// OK - DONE
int NPETSEC_ReadBtId(char* buffer)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadBtId\n";
	logFile.close();
#endif

	for (int i = 0; i < 6; ++i)
	{
		*(buffer - i + 5) = 'A';
	}

	return 0;
}

int NPETSEC_ReadImeiRaw(char* buffer)
{
	char IMEIBuf[8] = { 00, 44, 00, 00, 00, 00, 00, 00 };

#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadImeiRaw\n";
	logFile.close();
#endif

	memcpy_s(buffer, 8, IMEIBuf, 8);

	return 0;
}

int NPETSEC_ReadImeiRawEx(unsigned int IMEIIndex, char* buffer)
{
	char IMEIBuf[8] = { 00, 44, 00, 00, 00, 00, 00, 00 };

#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadImeiRawEx IMEIIndex: " << IMEIIndex << "\n";
	logFile.close();
#endif

	memcpy_s(buffer, 8, IMEIBuf, 8);

	return 0;
}

int NPETSEC_ReadImeiStr(char* buffer)
{
	char IMEIBuf[8] = { 0 };
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadImeiStr\n";
	logFile.close();
#endif

	if (NPETSEC_ReadImeiRaw(IMEIBuf))
	{
		int position = 0;
		for (int i = 0; i < 8; ++i)
		{
			if (i > 0)
				*(buffer + position++) = (*(IMEIBuf + i) & 0xF) + 48;
			*(buffer + position++) = (*(IMEIBuf + i) >> 4) + 48;
		}

		*(buffer + position) = 0;
	}

	return 0;
}

// OK - DONE
int NPETSEC_ReadImeiStrEx(unsigned int IMEIIndex, char* buffer)
{
	char IMEIBuf[8] = { 0 };
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadImeiStrEx IMEIIndex: " << IMEIIndex << "\n";
	logFile.close();
#endif

	if (NPETSEC_ReadImeiRawEx(IMEIIndex, IMEIBuf))
	{
		int position = 0;
		for (int i = 0; i < 8; ++i)
		{
			if (i > 0)
				*(buffer + position++) = (*(IMEIBuf + i) & 0xF) + 48;
			*(buffer + position++) = (*(IMEIBuf + i) >> 4) + 48;
		}

		*(buffer + position) = 0;
	}

	return 0;
}

int NPETSEC_ReadManufacturingData(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadManufacturingData\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadMeidRaw(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadMeidRaw\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadMeidStr(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadMeidStr\n";
	logFile.close();
#endif
	return 0;
}

// OK - DONE
int NPETSEC_ReadPublicId(char* buffer)
{
	char publicIdBuf[20] = { 0 };

#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadPublicId\n";
	logFile.close();
#endif

	memcpy_s(buffer, 20, publicIdBuf, 20);

	return 0;
}

int NPETSEC_ReadSecFea(__int64 a1, __int64 a2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadSecFea\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadSecFeaSize(__int64 a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadSecFeaSize\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadSecurityLogFile(unsigned int a1, __int64 a2, __int64 a3, unsigned int a4, __int64 a5)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadSecurityLogFile\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadServiceTag(__int64 a1, unsigned int a2, __int64 a3, __int64 a5)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadServiceTag\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadServiceTag3(__int64 a1, unsigned int a2, __int64 a3, __int64 a5)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadServiceTag3\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadServiceTag3Size(unsigned int a1, __int64 a2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadServiceTag3Size\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_ReadServiceTagSize(unsigned int a1, __int64 a2)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadServiceTagSize\n";
	logFile.close();
#endif
	return 0;
}

// OK - DONE
int NPETSEC_ReadWlanId(unsigned int WlanIdIndex, char* buffer)
{
	char data[6] = { 0 };
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_ReadWlanId WlanIdIndex: " << WlanIdIndex << "\n";
	logFile.close();
#endif

	for (int i = 0; i < 6; ++i)
	{
		*(buffer - i + 5) = *(data + i);
	}

	return 0;
}

int NPETSEC_RebootDevice(unsigned int bootMode, unsigned int options)
{
	// bootMode 4 => ignore
	// options  4 => modem restart
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_RebootDevice bootMode: " << bootMode << " options: " << options << "\n";
#endif

	char bootModeChar = 0;
	switch (bootMode)
	{
	case 0:
		bootModeChar = 'N';
		break;
	case 1:
		bootModeChar = 'S';
		break;
	case 2:
		bootModeChar = 'Y';
		break;
	case 3:
		bootModeChar = 'Z';
		break;
	case 4:
		// Do modem restart
#ifdef LOGGING_ENABLED
		logFile << "Modem restart not yet implemented\n";
		logFile.close();
#endif
		return 0;
	case 5:
		bootModeChar = 'M';
		break;
	case 6:
		bootModeChar = 'A';
		break;
	default:
#ifdef LOGGING_ENABLED
		logFile << "Unknown boot mode\n";
		logFile.close();
#endif
		return 0;
	}

	HANDLE startflagFile = CreateFile(L"C:\\efiesp\\mmo_uefi_start_flag.txt", 0x40000000, 0, 0, 2, 128, 0);

	if (startflagFile == INVALID_HANDLE_VALUE)
	{
#ifdef LOGGING_ENABLED
		logFile << "Creating boot mode hint file failed\n";
		logFile.close();
#endif
		return 0;
	}

	DWORD written;
	OVERLAPPED ol = { 0 };
	if (WriteFile(startflagFile, &bootModeChar, 1, &written, &ol) == FALSE)
	{
		CloseHandle(startflagFile);
#ifdef LOGGING_ENABLED
		logFile << "Writing boot mode failed\n";
		logFile.close();
#endif
		return 0;
	}

#ifdef LOGGING_ENABLED
	logFile << "Writing boot mode succeeded\n";
	CloseHandle(startflagFile);
#endif

	HANDLE proc_handle = GetCurrentProcess();
	TOKEN_PRIVILEGES* tp = (TOKEN_PRIVILEGES*)malloc(sizeof(TOKEN_PRIVILEGES) + sizeof(LUID_AND_ATTRIBUTES));
	HANDLE token_handle;

	if (!OpenProcessToken(proc_handle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token_handle))
	{
#ifdef LOGGING_ENABLED
		logFile << "Reboot failed 1\n";
		logFile.close();
#endif
		return 0;
	}

	tp->PrivilegeCount = 1;

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp->Privileges[0].Luid))
	{
#ifdef LOGGING_ENABLED
		logFile << "Reboot failed 2\n";
		logFile.close();
#endif
		CloseHandle(token_handle);
		return 0;
	}

	tp->Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(token_handle, FALSE, tp, 0, NULL, NULL))
	{
#ifdef LOGGING_ENABLED
		logFile << "Reboot failed 3\n";
		logFile.close();
#endif
		CloseHandle(token_handle);
		return 0;
	}

	if (InitiateSystemShutdownEx(NULL , NULL, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED) == FALSE)
	{
#ifdef LOGGING_ENABLED
		logFile << "Reboot failed 4\n";
#endif
	}

	CloseHandle(token_handle);
#ifdef LOGGING_ENABLED
	logFile.close();
#endif
	return 0;
}

int NPETSEC_RegItemValueWrite(unsigned int a1, __int64 a2, unsigned int a3)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_RegItemValueWrite\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_SetConfigAccessMode(unsigned int a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_SetConfigAccessMode\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_SetConfigAccessModeExternal(unsigned int a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_SetConfigAccessModeExternal\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_SetDriverLogging(unsigned int a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_SetDriverLogging\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_SetProductionDataUnchecked(unsigned int a1, __int64 buffer)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_SetProductionDataUnchecked\n";
	logFile.close();
#endif
	return 0;
}

int NPETSEC_SetSecurityMode(unsigned int a1)
{
#ifdef LOGGING_ENABLED
	ofstream logFile;
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "NPETSEC_SetSecurityMode\n";
	logFile.close();
#endif
	return 0;
}
