/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_THREAD_H_
#define _VJ_THREAD_H_
//#pragma once

#include <vjConfig.h>
//#include <unistd.h>


// Typedefs to help with cross-platform abilities
typedef void (*THREAD_FUNC)(void *);

#include <Threads/vjBaseThread.h>


#if defined(VJ_IRIX_SPROC)	// ---- SGI IPC Barrier ------ //
    typedef struct cancel_state	cancel_state_t;

#   include <Threads/vjThreadSGI.h>
#   include <Threads/vjThreadKeySGI.h>

    typedef class vjThreadSGI	vjThread;
    typedef class vjThreadKeySGI	vjKeyId;
#elif defined(VJ_OS_Win32)
#   include <windows.h>
#   include <process.h>
#   include <Threads/vjThreadWin32.h>
//#   include <Threads/vjThreadKeyWin32.h>
    typedef vjThreadWin32 vjThread;
//	typedef vjThreadKeyWin32 vjKeyId;
#elif defined(VJ_USE_PTHREADS)
    typedef int		cancel_state_t;

#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE
#endif

#   include <Threads/vjThreadPosix.h>
#   include <Threads/vjThreadKeyPosix.h>

    typedef class vjThreadPosix		vjThread;
    typedef class vjThreadKeyPosix	vjKeyId;
#else
#   include <Threads/vjThreadFunctor.h>
#endif	/* VJ_IRIX_SPROC */

   // Other thread related classes //
#include <Threads/vjThreadManager.h>


#endif	/* _VJ_THREAD_H_ */
