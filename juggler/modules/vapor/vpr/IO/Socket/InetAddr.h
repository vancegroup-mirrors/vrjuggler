/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_INET_ADDR_H_
#define _VPR_INET_ADDR_H_

#include <vpr/vprConfig.h>

#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#  include <vpr/md/NSPR/IO/Socket/InetAddrNSPR.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#  include <vpr/md/POSIX/IO/Socket/InetAddrBSD.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
#  include <vpr/md/SIM/IO/Socket/InetAddrSIM.h>
#endif

#ifdef VPR_HASH_MAP_INCLUDE
#  include VPR_HASH_MAP_INCLUDE
#endif

namespace vpr
{

/// Nice little helper class for hashing a <code>vpr::InetAddr</code>
struct InetAddrHash
{
   size_t operator() (vpr::InetAddr addr) const
   {
      return ((addr.getAddressValue() << 16) | addr.getPort());
   }
};

} // End of std namespace

inline bool operator<(const vpr::InetAddr& addr1, const vpr::InetAddr& addr2)
{
   if ( addr1.getAddressValue() < addr2.getAddressValue() )
   {
      return true;
   }
   else if ( addr1.getAddressValue() > addr2.getAddressValue() )
   {
      return false;
   }
   else if ( addr1.getAddressValue() == addr2.getAddressValue() )
   {
      return (addr1.getPort() < addr2.getPort());
   }

   return false;
}

namespace std
{

inline ostream& operator<<(std::ostream& out, const vpr::InetAddr& addr)
{
   out << addr.getAddressString() << ":" << addr.getPort();
   return out;
}

} // End of std namespace

#endif   /* _VPR_INET_ADDR_H_ */