//----------------------------
// Copyright (c) Lonely Cat Games  All rights reserved.
// Assert macro, providing similar functionality that standard assert.
// Designed to work with IExcept library (which is capable to extract
// information when int3 (BREAKPOINT exception) occurs.
//----------------------------
#undef  assert

#ifdef  NDEBUG

#define assert(exp) ((void)0)

#else

#include <assert.h>

#endif

