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

	return 0;//ret;
}

__int64 NPETSEC_DppItemSize(unsigned int itemId, int* bytecount)
{
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_DppItemSize";
	logFile << "\n";
	logFile << "Item ID: ";
	logFile << itemId;
	logFile << "\n";

	string filePath;

	switch (itemId)
	{
		case 0:
		{
			filePath = "\\Microsoft\\Microsoft.PVK";
			break;
		}
		case 1:
		{
			filePath = "\\QCOM\\BT.Provision";
			break;
		}
		case 2:
		{
			filePath = "\\QCOM\\WLAN.Provision";
			break;
		}
		case 3:
		{
			filePath = "\\MMO\\CoverColor.txt";
			break;
		}
		case 4:
		{
			filePath = "\\MMO\\customer_nvi_log.txt";
			break;
		}
		case 5:
		{
			filePath = "\\MMO\\simlock\\sign";
			break;
		}
		case 6:
		{
			filePath = "\\MMO\\simlock\\keys";
			break;
		}
		case 7:
		{
			filePath = "\\MMO\\simlock\\lock";
			break;
		}
		case 8:
		{
			filePath = "\\MMO\\product.dat";
			break;
		}
		case 9:
		{
			filePath = "\\MMO\\certs\\hwc";
			break;
		}
		case 10:
		{
			filePath = "\\MMO\\certs\\ccc";
			break;
		}
		case 11:
		{
			filePath = "\\MMO\\certs\\npc";
			break;
		}
		case 12:
		{
			filePath = "\\MMO\\certs\\rdc";
			break;
		}
		case 13:
		{
			filePath = "\\MMO\\certs\\devcert.bin";
			break;
		}
		case 14:
		{
			filePath = "\\MMO\\certs\\mirlink.bin";
			break;
		}
		case 15:
		{
			filePath = "\\MMO\\RegScreen\\imagelabel_dark.png";
			break;
		}
		case 16:
		{
			filePath = "\\MMO\\RegScreen\\imagelabel_light.png";
			break;
		}
		case 17:
		{
			filePath = "\\MMO\\simlock\\unlock.bin";
			break;
		}
		case 18:
		{
			filePath = "\\MMO\\ssdhash.bin";
			break;
		}
		case 19:
		{
			filePath = "\\MMO\\certs\\label_data.bin";
			break;
		}
		case 20:
		{
			filePath = "\\MMO\\RegScreen\\coo.txt";
			break;
		}
		case 21:
		{
			filePath = "\\MMO\\imageimeibarcode.png";
			break;
		}
		case 22:
		{
			filePath = "\\MMO\\fsghash.bin";
			break;
		}
		case 23:
		{
			filePath = "\\MMO\\Label\\panel.ver";
			break;
		}
		case 24:
		{
			filePath = "\\MMO\\Label\\label.ver";
			break;
		}
		case 25:
		{
			filePath = "\\MMO\\testfile.txt";
			break;
		}
		case 26:
		{
			filePath = "\\MMO\\testfilerestricted.txt";
			break;
		}
		case 27:
		{
			filePath = "\\MMO\\Label\\panelmdcl.ver";
			break;
		}
		case 28:
		{
			filePath = "\\MMO\\Label\\labelmdcl.ver";
			break;
		}
		case 29:
		{
			filePath = "\\MMO\\simlock\\sign2";
			break;
		}
	}

	logFile << "File name from ID: ";
	logFile << filePath;
	logFile << " -> ";
	logFile << "C:\\DPP" + filePath;
	logFile << "\n";

	__int64 ret = 0;

	try
	{
		ifstream ifs("C:\\DPP" + filePath, ios::binary | ios::in);
		ifstream::pos_type pos = ifs.tellg();

		ifs.seekg(0, ios::beg);
		bytecount = ifs.gcount();

		ifs.close();
	}
	catch (int e)
	{
		logFile << "Exception: " << e << '\n';
		ret = e;
	}

	logFile << "< NPETSEC_DppItemSize";
	logFile << "\n";
	logFile << ToString(ret);
	logFile << "\n";

	logFile.close();

	return ret;
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

__int64 NPETSEC_DppItemRead(unsigned int itemId, unsigned int loggingflag, char* outputBuffer, unsigned int bytecount)
{
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);

	logFile << "> NPETSEC_DppItemRead";
	logFile << "\n";
	logFile << "Item ID: ";
	logFile << itemId;
	logFile << "\n";
	logFile << "Logging flag: ";
	logFile << loggingflag;
	logFile << "\n";
	logFile << "Byte count: ";
	logFile << bytecount;
	logFile << "\n";

	string filePath;

	switch (itemId)
	{
		case 0:
		{
			filePath = "\\Microsoft\\Microsoft.PVK";
			break;
		}
		case 1:
		{
			filePath = "\\QCOM\\BT.Provision";
			break;
		}
		case 2:
		{
			filePath = "\\QCOM\\WLAN.Provision";
			break;
		}
		case 3:
		{
			filePath = "\\MMO\\CoverColor.txt";
			break;
		}
		case 4:
		{
			filePath = "\\MMO\\customer_nvi_log.txt";
			break;
		}
		case 5:
		{
			filePath = "\\MMO\\simlock\\sign";
			break;
		}
		case 6:
		{
			filePath = "\\MMO\\simlock\\keys";
			break;
		}
		case 7:
		{
			filePath = "\\MMO\\simlock\\lock";
			break;
		}
		case 8:
		{
			filePath = "\\MMO\\product.dat";
			break;
		}
		case 9:
		{
			filePath = "\\MMO\\certs\\hwc";
			break;
		}
		case 10:
		{
			filePath = "\\MMO\\certs\\ccc";
			break;
		}
		case 11:
		{
			filePath = "\\MMO\\certs\\npc";
			break;
		}
		case 12:
		{
			filePath = "\\MMO\\certs\\rdc";
			break;
		}
		case 13:
		{
			filePath = "\\MMO\\certs\\devcert.bin";
			break;
		}
		case 14:
		{
			filePath = "\\MMO\\certs\\mirlink.bin";
			break;
		}
		case 15:
		{
			filePath = "\\MMO\\RegScreen\\imagelabel_dark.png";
			break;
		}
		case 16:
		{
			filePath = "\\MMO\\RegScreen\\imagelabel_light.png";
			break;
		}
		case 17:
		{
			filePath = "\\MMO\\simlock\\unlock.bin";
			break;
		}
		case 18:
		{
			filePath = "\\MMO\\ssdhash.bin";
			break;
		}
		case 19:
		{
			filePath = "\\MMO\\certs\\label_data.bin";
			break;
		}
		case 20:
		{
			filePath = "\\MMO\\RegScreen\\coo.txt";
			break;
		}
		case 21:
		{
			filePath = "\\MMO\\imageimeibarcode.png";
			break;
		}
		case 22:
		{
			filePath = "\\MMO\\fsghash.bin";
			break;
		}
		case 23:
		{
			filePath = "\\MMO\\Label\\panel.ver";
			break;
		}
		case 24:
		{
			filePath = "\\MMO\\Label\\label.ver";
			break;
		}
		case 25:
		{
			filePath = "\\MMO\\testfile.txt";
			break;
		}
		case 26:
		{
			filePath = "\\MMO\\testfilerestricted.txt";
			break;
		}
		case 27:
		{
			filePath = "\\MMO\\Label\\panelmdcl.ver";
			break;
		}
		case 28:
		{
			filePath = "\\MMO\\Label\\labelmdcl.ver";
			break;
		}
		case 29:
		{
			filePath = "\\MMO\\simlock\\sign2";
			break;
		}
	}

	logFile << "File name from ID: ";
	logFile << filePath;
	logFile << " -> ";
	logFile << "C:\\DPP" + filePath;
	logFile << "\n";

	logFile << "Reading start";
	logFile << "\n";

	__int64 ret = 0;

	try
	{
		ifstream ifs("C:\\DPP" + filePath, ios::binary | ios::in);
		ifstream::pos_type pos = ifs.tellg();

		ifs.seekg(0, ios::beg);
		ifs.read(outputBuffer, bytecount);
		ifs.close();
	}
	catch (int e)
	{
		logFile << "Exception: " << e << '\n';
		ret = e;
	}

	logFile << "Reading end";
	logFile << "\n";

	logFile << "< NPETSEC_DppItemRead";
	logFile << "\n";
	logFile << "Returning 0";
	logFile << "\n";

	logFile.close();

	return ret;
}

__int64 NPETSEC_ValidateMultiSim(char* outputBuffer, char* Buffer2)
{
	logFile.open("C:\\RIL_NPETSEC.txt", std::ofstream::out | std::ofstream::app);
	logFile << "> NPETSEC_ValidateMultiSim";
	logFile << "\n";

	logFile << sizeof(*outputBuffer) << "\n";
	logFile << sizeof(*Buffer2) << "\n";

	char  outputBuffer2[23];

	char data[] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x02, 0x01 };

	/*outputBuffer2[0] = 0x00;
	outputBuffer2[1] = 0x00;
	outputBuffer2[2] = 0x00;
	outputBuffer2[3] = 0x00;
	outputBuffer2[4] = 0x01;
	outputBuffer2[5] = 0x00;
	outputBuffer2[6] = 0x00;
	outputBuffer2[7] = 0x00;
	outputBuffer2[8] = 0x01;
	outputBuffer2[9] = 0x00;
	outputBuffer2[10] = 0x00;
	outputBuffer2[11] = 0x00;
	outputBuffer2[12] = 0x01;
	outputBuffer2[13] = 0x00;
	outputBuffer2[14] = 0x00;
	outputBuffer2[15] = 0x00;
	outputBuffer2[16] = 0x00;
	outputBuffer2[17] = 0x01;
	outputBuffer2[18] = 0x01;
	outputBuffer2[19] = 0x00;
	outputBuffer2[20] = 0x01;
	outputBuffer2[21] = 0x00;
	outputBuffer2[22] = 0x02;
	outputBuffer2[23] = 0x01;*/

	Buffer2 = data;

	logFile << sizeof(Buffer2) << "\n";
	logFile << sizeof(*Buffer2) << "\n";

	logFile << "< NPETSEC_ValidateMultiSim";
	logFile << "\n";

	logFile.close();

	return 0;
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