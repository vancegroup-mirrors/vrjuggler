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

#include <iostream>
#include <math.h>

#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <gmtl/Vec.h>
#include <gmtl/Convert.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <cubesApp.h>


// ----------------------------------------------------------------------------
// UserData methods.
// ----------------------------------------------------------------------------

// Update the navigation matrix
//
// Uses a quaternion to do rotation in the environment
void UserData::updateNavigation()
{
   gmtl::Vec3f xyzAngles;
   gmtl::Vec3f xyzTrans;

   // Cur*Transform = New Location
   gmtl::Matrix44f transform, transformIdent;
   gmtl::Quatf     source_rot, goal_rot, slerp_rot;

   gmtl::identity(transformIdent);
//   gmtl::normalize(source_rot);
//   source_rot.makeRot(transformIdent);

   gmtl::Matrix44f* wand_matrix;
   wand_matrix = mWand->getData();
   gmtl::getRot(*wand_matrix, xyzAngles[0],  xyzAngles[1], xyzAngles[2],
                gmtl::XYZ);
//   wand_matrix->getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);


   vprDEBUG(vprDBG_ALL,6) << "===================================\n"
                        << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,6) << "Wand:\n" << *wand_matrix << std::endl
                        << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,6) << "Wand XYZ: " << xyzAngles << std::endl
                        << vprDEBUG_FLUSH;

   gmtl::convert(goal_rot, *wand_matrix); // Create the goal rotation quaternion

   if(transformIdent != *wand_matrix)  // If we don't have two identity matrices
   {
      gmtl::slerp(slerp_rot, 0.05f, source_rot, goal_rot); // Transform part way there
      gmtl::convert(slerp_rot, transform);      // Create the transform matrix to use
   }
   else
   {
      gmtl::identity(transform);
   }

   vprDEBUG(vprDBG_ALL,6) << "Transform:\n" << transform << std::endl
                        << vprDEBUG_FLUSH;
   gmtl::getRot(transform, xyzAngles[0],  xyzAngles[1], xyzAngles[2],
                gmtl::XYZ);
   vprDEBUG(vprDBG_ALL,6) << "Transform XYZ: " << xyzAngles << std::endl
                        << vprDEBUG_FLUSH;

   vprDEBUG(vprDBG_ALL,6) << "Nav:\n" << mNavMatrix << std::endl << std::endl
                        << vprDEBUG_FLUSH;

   // ----- Translation ------- //
   const float velocity_inc = 0.001f;

   // Update velocity
   if(mIncVelocityButton->getData())
      mCurVelocity += velocity_inc;
   else if(mDecVelocityButton->getData())
      mCurVelocity -= velocity_inc;
   else if(mStopButton->getData())
      mCurVelocity = 0.0f;


   if(mIncVelocityButton->getData() || mDecVelocityButton->getData())
      vprDEBUG(vprDBG_ALL,6) << "Velocity: " << mCurVelocity << std::endl
                           << vprDEBUG_FLUSH;

   if(mIncVelocityButton->getData() == gadget::Digital::TOGGLE_ON)
      vprDEBUG(vprDBG_ALL,2) << "-- Toggle ON --" << std::endl << vprDEBUG_FLUSH;
   if(mIncVelocityButton->getData() == gadget::Digital::TOGGLE_OFF)
      vprDEBUG(vprDBG_ALL,2) << "-- Toggle OFF --" << std::endl << vprDEBUG_FLUSH;
   if(mIncVelocityButton->getData() == gadget::Digital::ON)
      vprDEBUG(vprDBG_ALL,2) << "-- ON --" << std::endl << vprDEBUG_FLUSH;
//   if(mIncVelocityButton->getData() == Digital::OFF)
//      vprDEBUG(vprDBG_ALL,) << "-- OFF --" << std::endl << vprDEBUG_FLUSH;

   // Find direction vector
   gmtl::Vec3f forward(0.0f, 0.0f, -1.0f);
   forward *= mCurVelocity;

   gmtl::Matrix44f rot_mat, local_xform;
   gmtl::invert(rot_mat, transform);

//   local_xform.makeTrans(0, 0, mCurVelocity);
//   local_xform.postMult(rot_mat);
   local_xform = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0.0f, 0.0f,
                                                              mCurVelocity));
   gmtl::postMult(local_xform, rot_mat);

//   mNavMatrix.preMult(local_xform);
   gmtl::preMult(mNavMatrix, local_xform);

//   local_xform.getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);
//   local_xform.getTrans(xyzTrans[0], xyzTrans[1], xyzTrans[2]);
   gmtl::getRot(local_xform, xyzAngles[0], xyzAngles[1], xyzAngles[2],
                gmtl::XYZ);
   gmtl::getTrans(local_xform, xyzTrans[0], xyzTrans[1], xyzTrans[2]);
   vprDEBUG(vprDBG_ALL,6) << "Transform   Rot: " << xyzAngles << std::endl
                        << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,6) << "Transform Trans: " << xyzTrans << std::endl
                        << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,6) << "-------------------------------------------"
                        << std::endl << vprDEBUG_FLUSH;
}

// ----------------------------------------------------------------------------
// cubesApp methods.
// ----------------------------------------------------------------------------

// Execute any initialization needed before the API is started.  Put device
// initialization here.
void cubesApp::init()
{
   vprDEBUG(vprDBG_ALL,0) << "---------- cubes:App:init() ---------------"
                        << std::endl << vprDEBUG_FLUSH;
   std::vector<vrj::User*> users = mKernel->getUsers();
   int num_users = users.size();
   vprASSERT(num_users > 0);      // Make sure that we actually have users defined

   UserData* new_user=NULL;
   mUserData = std::vector<UserData*>(num_users);

   switch (num_users)
   {
   case (2):
      new_user = new UserData(users[1],"VJWand1", "VJButton0_1", "VJButton1_1",
                              "VJButton2_1");
      mUserData[1] = new_user;
      vprASSERT(users[1]->getId() == 1);
   case (1):
      new_user = new UserData(users[0],"VJWand", "VJButton0", "VJButton1",
                              "VJButton2");
      mUserData[0] = new_user;
      vprASSERT(users[0]->getId() == 0);
      break;
   default:
      vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrRED, "ERROR:") << " Bad number of users." << vprDEBUG_FLUSH;
      exit();
      break;
   }
}

// Called immediately upon opening a new OpenGL context.  This is called once
// for every display window that is opened.  Put OpenGL resource allocation
// here.
void cubesApp::contextInit()
{
   // Generate some random lists.  NOTE: Needed for testing/debugging only!
   mDlDebugData->maxIndex = rand()%50;
   mDlDebugData->dlIndex = glGenLists(mDlDebugData->maxIndex);

   // Generate the display list to be used for the cube faces.  This is the
   // important one.
   mDlData->dlIndex = glGenLists(1);

   // Verify that we got a valid display list index.
   if ( glIsList(mDlData->dlIndex) == GL_FALSE ) {
       vprASSERT(false && "glGenLists() returned an invalid display list!");
   }
   else {
       glNewList(mDlData->dlIndex, GL_COMPILE);
          drawbox(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, GL_QUADS);
       glEndList();

       vprDEBUG(vprDBG_ALL,0) << "Creating DL:" << mDlData->dlIndex
                            << std::endl << vprDEBUG_FLUSH;
       std::cerr << "created displays lists:" << mDlDebugData->maxIndex + 1
                 << std::endl;

       initGLState();
    }
}

//: Called immediately upon closing an OpenGL context
// (called for every window that is closed)
// put your opengl deallocation here...
void cubesApp::contextClose()
{
   // Deallocate the random display lists used for debugging.
   if ( glIsList(mDlDebugData->dlIndex) == GL_TRUE ) {
      vprDEBUG(vprDBG_ALL, 0) << "Deallocating " << mDlDebugData->maxIndex
                            << " display lists\n" << vprDEBUG_FLUSH;
      glDeleteLists(mDlDebugData->dlIndex, mDlDebugData->maxIndex);
   }

   // Deallocate the cube face geometry data from the video hardware.
   if ( glIsList(mDlData->dlIndex) == GL_TRUE ) {
      glDeleteLists(mDlData->dlIndex, 1);
   }
}

//----------------------------------------------
// Draw the scene.  A bunch of boxes of
// differing color and stuff.
//----------------------------------------------
void cubesApp::myDraw(vrj::User* user)
{
   vprDEBUG(vprDBG_ALL,5) << "\n--- myDraw() ---\n" << vprDEBUG_FLUSH;

   static const float SCALE = 100;
   //static const float SCALE = 10;
   static const float INCR = 0.1;

   glPushMatrix();
         // --- Push on Navigation matrix for the user --- //

      gmtl::Matrix44f nav_matrix = mUserData[user->getId()]->mNavMatrix;
      glMultMatrixf(nav_matrix.mData);


      // dereferencing a GlContextData is expensive, so we'll do it once
      // and store the reference.
      ContextData& cd = *mDlData;
      
      //---- Main box loop -----///
      for (float x=0;x<1;x += INCR)
         for (float y=0;y<1; y += INCR)
            for (float z=0;z<1;z += INCR)
            {
               glColor3f(x, y, z);     // Set the Color
               glPushMatrix();
               glTranslatef( (x-0.5)*SCALE, (y-0.5)*SCALE, (z-0.5)*SCALE);
               glCallList (cd.dlIndex);
               glPopMatrix();
            }


     /*
         // --- Draw corner boxes --- //
      for(float x=0;x<=1.0;x++)
         for(float y=0.0;y<=1.0;y++)
            for(float z=0.0;z<=1.0;z++)
            {
               //glColor3f(x,y,z);    // Set the color
               glColor3f(1.0, 0.0,  0.0f);
                  glPushMatrix();
               {
                  glTranslatef( (x-0.5)*SCALE, (y-0.0)*SCALE, (z-0.5)*SCALE);
                  glScalef(2.0f, 2.0f, 2.0f);
                  drawCube();
               }
               glPopMatrix();
            }
     */

   glPopMatrix();
}

void cubesApp::initGLState()
{
   GLfloat light0_ambient[] = { .2,  .2,  .2,  1.0};
   GLfloat light0_diffuse[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_position[] = {2000.0, 1000.0, 100.0, 1.0};
   GLfloat light0_direction[] = {-100, -100.0, -100.0};

   GLfloat mat_ambient[] = { 0.7, 0.7,  0.7,  1.0};
   GLfloat mat_diffuse[] = { 1.0,  0.5,  0.8,  1.0};
   GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat mat_shininess[] = { 50.0};
   GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
}

void cubesApp::drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                       GLdouble z0, GLdouble z1, GLenum type)
{
   static GLdouble n[6][3] = {
      {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
   };
   static GLint faces[6][4] = {
      { 0, 1, 2, 3}, { 3, 2, 6, 7}, { 7, 6, 5, 4},
      { 4, 5, 1, 0}, { 5, 6, 2, 1}, { 7, 4, 0, 3}
   };
   GLdouble v[8][3], tmp;
   GLint i;

   if (x0 > x1)
   {
      tmp = x0; x0 = x1; x1 = tmp;
   }
   if (y0 > y1)
   {
      tmp = y0; y0 = y1; y1 = tmp;
   }
   if (z0 > z1)
   {
      tmp = z0; z0 = z1; z1 = tmp;
   }
   v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0;
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0;
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1;

   for (i = 0; i < 6; i++)
   {
      glBegin(type);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][0]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][1]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][2]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][3]][0]);
      glEnd();
   }
}
