#ifndef _VJ_CONFIG_H_
#define _VJ_CONFIG_H_
#pragma once

/*
 * ----------------------------------------------------------------------------
 * This file (vjConfig.h) includes header files common to most, if not all,
 * files in the VR Juggler source tree.  It must be included at the top of
 * every .h and .cpp file before any other headers because it includes system
 * headers.
 *
 * $Id$
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#ifdef HAVE_CONFIG_H
#include <vjDefines.h>
#endif	/* HAVE_CONFIG_H */

/* Common includes */
/* Put here for pre-compiled headers */
#ifdef __cplusplus
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#endif

#endif	/* _VJ_CONFIG_H_ */
