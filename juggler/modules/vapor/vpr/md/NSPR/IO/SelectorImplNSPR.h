
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

#ifndef _VPR_SELECTOR_IMP_NSPR_H_
#define _VPR_SELECTOR_IMP_NSPR_H_

#include <vprConfig.h>
#include <Utils/Debug.h>

#include <vector>
#include <map>
#include <string>

#include <IO/Selector_t.h>
#include <prio.h>

namespace vpr {

// ----------------------------------------------------------------------------
//: NSPR Implementation of ---> Cross-platform selection interface.
//
// A selector is used to wait on a set of Handles untils any of the 
// events occur that the user is interested in.
//
// Implementation site of the Selector_t bridge.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
class SelectorImpNSPR : public SelectorBase
{
public:
   //: Add the given handle to the selector
   //! PRE: handle is a valid handle
   //! POST: handle is added to the handle set, and initialized to a mask of no-events
   bool addHandle(IOSys::Handle handle);

   //: Remove a handle from the selector
   //! PRE: handle is in the selector
   //! POST: handle is removed from the set of valid handles
   bool removeHandle(IOSys::Handle handle);

   //: Set the event flags going in to the select to mask
   bool setIn(IOSys::Handle handle, vpr::Uint16 mask);

   //: Get the current in flag mask
   vpr::Uint16 getIn(IOSys::Handle handle);

   //: Get the current out flag mask
   vpr::Uint16 getOut(IOSys::Handle handle);

   //: Select
   //! ARGS: numWithEvents - Upon completion, this holds the number of items that have events
   //! ARGS: timeout - The number of msecs to select for (0 - don't wait)
   vpr::Status select(vpr::Uint16& numWithEvents, vpr::Uint16 timeout);

   //: Get the number of handles
   vpr::Uint16 getNumHandles()
   {
      return mPollDescs.size();
   }

   IOSys::Handle getHandle(vpr::Uint16 index)
   {
      return mPollDescs[index].fd;
   }

protected:

   // Get the index of the handle given
   //! RETURNS: .end() - Not found, else the index to the handle in mPollDescs
   std::vector<PRPollDesc>::iterator getHandle(PRFileDesc const* handle);

   PRUint16 convertMaskVprToNspr(vpr::Uint16 mask);
   vpr::Uint16 convertMaskNsprToVpr(PRUint16 mask);
   
protected:

   std::vector<PRPollDesc>                  mPollDescs;    // List of Poll Descriptions to pass to PR_Poll()

/*
struct PRPollDesc {
    PRFileDesc* fd;
    PRInt16 in_flags;
    PRInt16 out_flags;
 };
*/
};

}; // End of vpr namespace


#endif  /* _VPR_SELECTOR_BRIDGE_H_ */
