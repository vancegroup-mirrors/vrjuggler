/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_MUTEX_H_
#define _VJ_MUTEX_H_
//----------------------------------------------
// vjMutex
//
// Purpose:
//    Defines a mutex wrapper.  No base class because
//   we don't want to have to use virtual functions.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------

#include <vjConfig.h>

#ifdef VJ_SGI_IPC
#   include <ulocks.h>
#   include <Sync/vjMutexSGI.h>    
    
    typedef  vjMutexSGI vjMutex;
#else
#ifdef WIN32
#	include <Sync/vjMutexWin32.h>
	typedef vjMutexWin32 vjMutex;
#else
#ifdef VJ_USE_PTHREADS
#   include <Sync/vjMutexPosix.h>

    typedef  vjMutexPosix vjMutex;
#endif	/* VJ_USE_PTHREADS */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */

#include <Sync/vjNullMutex.h>
#endif
