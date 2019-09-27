// NPETSEC.h - Contains declarations of npetsec functions
#pragma once

#ifdef NPETSEC_EXPORTS
#define NPETSEC_API __declspec(dllexport)
#else
#define NPETSEC_API __declspec(dllimport)
#endif

extern "C" NPETSEC_API __int64 NPETSEC_Init(__int64 val1);
extern "C" NPETSEC_API __int64 NPETSEC_DppItemSize(unsigned int val1, int *val2);
extern "C" NPETSEC_API __int64 NPETSEC_UnlockSimlock(unsigned int val1, __int64 val2);
extern "C" NPETSEC_API __int64 NPETSEC_GetSimlockStatus(__int64 val1);
extern "C" NPETSEC_API __int64 NPETSEC_DppItemRead(unsigned int a1, unsigned int a2, char* a3, unsigned int a4);
extern "C" NPETSEC_API __int64 NPETSEC_ValidateMultiSim(__int64 a1, __int64 a2);
extern "C" NPETSEC_API void    NPETSEC_Deinit();
// Extras required by NCSd
extern "C" NPETSEC_API __int64 NPETSEC_DeleteSecurityLogFile(unsigned int a1);
extern "C" NPETSEC_API __int64 NPETSEC_GetConfigAccessMode(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_GetProductionData(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_GetSecurityLogFileSize(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_GetSecurityMode(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_GetUefiCertStates(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_IsProductionDone(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_LastErrorStr();
extern "C" NPETSEC_API __int64 NPETSEC_QcEfsDataFileWrite(__int64 a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API __int64 NPETSEC_QcEfsFileDelete(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_QcEfsFileRead(__int64 a1, __int64 a2, __int64 a3);
extern "C" NPETSEC_API __int64 NPETSEC_QcEfsFileWrite(__int64 a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API __int64 NPETSEC_QcEfsItemFileWrite(__int64 a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API __int64 NPETSEC_QcNvItemIndexedRead(unsigned __int16 a1, unsigned __int16 a2, unsigned __int8 a3, __int64 a4, __int64 a5);
extern "C" NPETSEC_API __int64 NPETSEC_QcNvItemWrite(unsigned __int16 a1, unsigned __int16 a2, __int64 a3, unsigned int a4);
extern "C" NPETSEC_API __int64 NPETSEC_QcNvSpcUnlock(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_QcSubscriptionNvItemIndexedRead(unsigned __int16 a1, __int64 a2, unsigned __int8 a3, __int64 a4, __int64 a5);
extern "C" NPETSEC_API __int64 NPETSEC_QcSubscriptionNvItemRead(unsigned __int16 a1, __int64 a2, __int64 a3, __int64 a4);
extern "C" NPETSEC_API __int64 NPETSEC_QcSubscriptionNvItemWrite(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_QfsEfsFileWrite(unsigned __int16 a1, __int64 a2, __int64 a3, unsigned int a4);
extern "C" NPETSEC_API __int64 NPETSEC_RdcPhoneReq(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadBtId(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadImeiRaw(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadImeiStr(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadImeiStrEx(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_ReadManufacturingData(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadMeidRaw(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadMeidStr(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadPublicId(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadSecFea(__int64 a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_ReadSecFeaSize(__int64 a1);
extern "C" NPETSEC_API __int64 NPETSEC_ReadSecurityLogFile(unsigned int a1, __int64 a2, __int64 a3, unsigned int a4, __int64 a5);
extern "C" NPETSEC_API __int64 NPETSEC_ReadServiceTag(__int64 a1, unsigned int a2, __int64 a3, __int64 a5);
extern "C" NPETSEC_API __int64 NPETSEC_ReadServiceTag3(__int64 a1, unsigned int a2, __int64 a3, __int64 a5);
extern "C" NPETSEC_API __int64 NPETSEC_ReadServiceTag3Size(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_ReadServiceTagSize(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_ReadWlanId(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_RebootDevice(unsigned int a1, unsigned int a2);
extern "C" NPETSEC_API __int64 NPETSEC_RegItemValueWrite(unsigned int a1, __int64 a2, unsigned int a3);
extern "C" NPETSEC_API __int64 NPETSEC_SetConfigAccessMode(unsigned int a1);
extern "C" NPETSEC_API __int64 NPETSEC_SetConfigAccessModeExternal(unsigned int a1);
extern "C" NPETSEC_API __int64 NPETSEC_SetDriverLogging(unsigned int a1);
extern "C" NPETSEC_API __int64 NPETSEC_SetProductionDataUnchecked(unsigned int a1, __int64 a2);
extern "C" NPETSEC_API __int64 NPETSEC_SetSecurityMode(unsigned int a1);