/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_DATAGRAM_H_
#define _VPR_SOCKET_DATAGRAM_H_

#include <vpr/vprConfig.h>

#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>
#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/Stats/IOStatsStrategyAdapter.h>

// include bridge class
#include <vpr/IO/Socket/SocketDatagram_t.h>

#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#include <vpr/md/NSPR/IO/Socket/SocketDatagramImplNSPR.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#include <vpr/md/POSIX/IO/Socket/SocketDatagramImplBSD.h>
#endif

namespace vpr
{
   typedef SocketDatagram_t<SocketConfiguration>  SocketDatagram;
}


#endif	/* _VJ_SOCKET_DATAGRAM_H_ */
