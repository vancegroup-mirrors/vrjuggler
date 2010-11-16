/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _WAND_APP_
#define _WAND_APP_

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vrj/Draw/Direct3D/D3dApp.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>

namespace vrjTest
{

/**
 * Demonstration OpenGL application class
 *
 * This application simply renders a box on the wand with the axes for the
 * wand's coordinate space.
 */
class CrateApp : public vrj::D3dApp
{
public:
   /** Constructor. */
   CrateApp(vrj::Kernel* kernel);

   /** Destructor. */
   ~CrateApp();

   /**
    * Execute any initialization needed before the API is started<BR><BR>
    * Initialize VR Juggler device interfaces here.
    *
    * This is called once before OGL is initialized
    */
   void init();

   /**
    * Function that is called immediately after a new OGL context is created.
    * This is called for every display window that is opened.  Initialize GL
    * state here. Also used to create context specific information<BR>
    *
    * This is called once for each display (twice for each stereo display)
    */
   void contextInit(LPDIRECT3DDEVICE9 renderDevice);

   /**
    * Function called after tracker update but before start of drawing<BR><BR>
    * Called once before every frame.  Do calculations and state modifications
    * here.
    */
   void preFrame()
   {;}

   void bufferPreDraw()
   {;}

   /**
    * Function to draw the scene.  Put OpenGL draw functions here.
    *
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn<BR><BR>
    *
    * called [once * number of displays] per frame
    */
   void draw(LPDIRECT3DDEVICE9 renderDevice);

private:
   void init_light(LPDIRECT3DDEVICE9 renderDevice);
   void init_graphics(LPDIRECT3DDEVICE9 renderDevice);

private:
   gadget::PositionInterface    mWand;
   gadget::PositionInterface    mHead;
   gadget::DigitalInterface     mButton0;
   gadget::DigitalInterface     mButton1;
   gadget::DigitalInterface     mButton2;
   gadget::DigitalInterface     mButton3;
   gadget::DigitalInterface     mButton4;
   gadget::DigitalInterface     mButton5;
   LPDIRECT3DTEXTURE9           texture_1;
   LPDIRECT3DVERTEXBUFFER9      mTBuffer;
};

}

#endif
