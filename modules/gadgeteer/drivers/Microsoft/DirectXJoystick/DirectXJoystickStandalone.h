/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _GADGET_DIRECTX_JOYSTICK_STANDALONE_H_
#define _GADGET_DIRECTX_JOYSTICK_STANDALONE_H_

#include <windows.h>
#include <dinput.h>
#include <string>


namespace gadget
{

class DirectXJoystickStandalone
{
public:
   DirectXJoystickStandalone();

   void init();

   void close();

   bool poll();

   const DIJOYSTATE& getData() const;

   DWORD getType() const;

   const std::string& getProductName() const;

   /** @name Direct Input callbacks */
   //@{
   BOOL enumerateJoysticks(const DIDEVICEINSTANCE* dInstance);
   BOOL enumerateAxes(const DIDEVICEOBJECTINSTANCE* doi);
   //@}

   static int getAxisMin()
   {
      return -100;
   }

   static int getAxisMax()
   {
      return 100;
   }

private:
   /** @name Direct Input data */
   //@{
   LPDIRECTINPUT8       mDxObject;    /**< DirectInput object */
   LPDIRECTINPUTDEVICE8 mDxJoystick;  /**< DirectInput device */
   //@}

   /** @name Data format information */
   //@{
   // Use DIJOYSTATE for the data format.  We do not need the extended capabilities
   // of DIJOYSTATE2.
   DIJOYSTATE           mJsData;      /**< Joystick state data structure */

   const DIDATAFORMAT* mDataFormatObj;
   size_t              mDataFormatSize;
   //@}

   DWORD mType;
   std::string mProductName;
};

}

#endif
