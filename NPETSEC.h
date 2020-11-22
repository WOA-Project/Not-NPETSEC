// NPETSEC.h - Contains declarations of npetsec functions
#pragma once

#ifdef NPETSEC_EXPORTS
#define NPETSEC_API __declspec(dllexport)
#else
#define NPETSEC_API __declspec(dllimport)
#endif

extern "C" NPETSEC_API int NPETSEC_Init(__int64 val1);
extern "C" NPETSEC_API int NPETSEC_DppItemSize(unsigned int val1, int *val2);
extern "C" NPETSEC_API int NPETSEC_UnlockSimlock(unsigned int val1, __int64 val2);
extern "C" NPETSEC_API int NPETSEC_GetSimlockStatus(char* buffer);
extern "C" NPETSEC_API int NPETSEC_DppItemRead(unsigned int a1, unsigned int a2, char* a3, unsigned int a4);
extern "C" NPETSEC_API int NPETSEC_ValidateMultiSim(__int64 a1, __int64 a2);
extern "C" NPETSEC_API void    NPETSEC_Deinit();

// Extras required by NCSd
extern "C" NPETSEC_API int NPETSEC_DeleteSecurityLogFile(unsigned int a1);
extern "C" NPETSEC_API int NPETSEC_GetConfigAccessMode(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_GetSecurityLogFileSize(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API int NPETSEC_LastErrorStr();
extern "C" NPETSEC_API int NPETSEC_QcEfsDataFileWrite(__int64 a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API int NPETSEC_QcEfsFileDelete(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_QcEfsFileRead(__int64 a1, __int64 a2, __int64 a3);
extern "C" NPETSEC_API int NPETSEC_QcEfsFileWrite(__int64 a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API int NPETSEC_QcEfsItemFileWrite(__int64 a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API int NPETSEC_QcNvItemIndexedRead(unsigned __int16 a1, unsigned __int16 a2, unsigned __int8 a3, __int64 a4, __int64 a5);
extern "C" NPETSEC_API int NPETSEC_QcNvItemWrite(unsigned __int16 a1, unsigned __int16 a2, __int64 a3, unsigned int a4);
extern "C" NPETSEC_API int NPETSEC_QcNvSpcUnlock(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_QcSubscriptionNvItemIndexedRead(unsigned __int16 a1, __int64 a2, unsigned __int8 a3, __int64 a4, __int64 a5);
extern "C" NPETSEC_API int NPETSEC_QcSubscriptionNvItemRead(unsigned __int16 a1, __int64 a2, __int64 a3, __int64 a4);
extern "C" NPETSEC_API int NPETSEC_QcSubscriptionNvItemWrite(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API int NPETSEC_QfsEfsFileWrite(unsigned __int16 a1, __int64 a2, __int64 a3, unsigned int a4);
extern "C" NPETSEC_API int NPETSEC_RdcPhoneReq(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_ReadManufacturingData(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_ReadMeidRaw(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_ReadMeidStr(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_ReadSecFea(__int64 a1, __int64 a2);
extern "C" NPETSEC_API int NPETSEC_ReadSecFeaSize(__int64 a1);
extern "C" NPETSEC_API int NPETSEC_ReadSecurityLogFile(unsigned int a1, __int64 a2, __int64 a3, unsigned int a4, __int64 a5);
extern "C" NPETSEC_API int NPETSEC_ReadServiceTag(__int64 a1, unsigned int a2, __int64 a3, __int64 a5);
extern "C" NPETSEC_API int NPETSEC_ReadServiceTag3(__int64 a1, unsigned int a2, __int64 a3, __int64 a5);
extern "C" NPETSEC_API int NPETSEC_ReadServiceTag3Size(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API int NPETSEC_ReadServiceTagSize(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API int NPETSEC_RebootDevice(unsigned int a1, unsigned int a2);
extern "C" NPETSEC_API int NPETSEC_RegItemValueWrite(unsigned int a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API int NPETSEC_SetConfigAccessMode(unsigned int a1);
extern "C" NPETSEC_API int NPETSEC_SetConfigAccessModeExternal(unsigned int a1);
extern "C" NPETSEC_API int NPETSEC_SetDriverLogging(unsigned int a1);
extern "C" NPETSEC_API int NPETSEC_SetProductionDataUnchecked(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API int NPETSEC_SetSecurityMode(unsigned int a1);

// OK
extern "C" NPETSEC_API int NPETSEC_GetProductionData(unsigned int productionDataId, char* buffer);
extern "C" NPETSEC_API int NPETSEC_GetSecurityMode(int* securityMode);
extern "C" NPETSEC_API int NPETSEC_GetUefiCertStates(int states[5]);
extern "C" NPETSEC_API int NPETSEC_IsProductionDone(int* productionState);
extern "C" NPETSEC_API int NPETSEC_ReadBtId(char* buffer);
extern "C" NPETSEC_API int NPETSEC_ReadImeiRaw(char* buffer);
extern "C" NPETSEC_API int NPETSEC_ReadImeiStr(char* buffer);
extern "C" NPETSEC_API int NPETSEC_ReadImeiStrEx(unsigned int IMEIIndex, char* buffer);
extern "C" NPETSEC_API int NPETSEC_ReadPublicId(char* buffer);
extern "C" NPETSEC_API int NPETSEC_ReadWlanId(unsigned int WlanIdIndex, char* buffer);

// Added
extern "C" NPETSEC_API int NPETSEC_ReadImeiRawEx(unsigned int IMEIIndex, char* buffer);