#ifdef _DEBUG
#pragma comment(lib, "../Debug/FlashDBSimDll.lib")
#else
#pragma comment(lib, "../Release/FlashDBSimDll.lib")
#endif

#include "flashdbsim_i.h"	/* please configure the path here */

#define DLLIMPORT	extern "C" __declspec(dllimport)

DLLIMPORT
RV f_initialize(const VFD_INFO&, const FTL_INFO&);
DLLIMPORT
RV f_release(void);
DLLIMPORT
RV f_alloc_page(int, LBA *);
DLLIMPORT
RV f_release_page(LBA);
DLLIMPORT
RV f_read_page(LBA, BYTE *, int, size_t);
DLLIMPORT
RV f_write_page(LBA, const BYTE *, int, size_t);
DLLIMPORT
const IFTL * f_get_ftl_module(void);
DLLIMPORT
const IVFD * f_get_vfd_module(void);
