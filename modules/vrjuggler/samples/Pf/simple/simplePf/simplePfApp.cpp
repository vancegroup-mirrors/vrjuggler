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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <simplePfApp.h>


using namespace vrj;

// ------- SCENE GRAPH ----
// a standard organized interface for derived applications:
//
//                         /-- mLightGroup -- mSun
// mRootNode -- mSceneScale -- mModelRoot
//
void simplePfApp::initScene()
{
   // Load the scene
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "simplePfApp::initScene\n" << vprDEBUG_FLUSH;

   // Allocate all the nodes needed
   mRootNode      = new pfGroup;            // Root of our graph
   mSceneScale    = new pfDCS;

   float scene_scale = 1.0f/gadget::PositionUnitConversion::ConvertToFeet;    // Scene uses feet as units
   mSceneScale->setScale(scene_scale);

   // Create the SUN light source
   mLightGroup = new pfGroup;
   mSun = new pfLightSource;
   mLightGroup->addChild( mSun );
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,1.0f,1.0f,1.0f );
   mSun->setColor( PFLT_AMBIENT,0.3f,0.3f,0.3f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();

   // --- LOAD THE MODEL --- //
   mModelRoot = pfdLoadFile(mModelFileName.c_str());

   // --- CONSTRUCT STATIC Structure of SCENE GRAPH -- //
   mRootNode->addChild(mSceneScale);
   mSceneScale->addChild( mModelRoot );
   mSceneScale->addChild(mLightGroup);
}