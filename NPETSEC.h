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