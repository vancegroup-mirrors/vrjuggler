/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Generate.h>

#include <VNCDesktop.h>
#include <wandApp.h>

namespace vrjvnc
{

bool wandApp::configCanHandle(jccl::ConfigChunkPtr element)
{
   static const std::string my_type("vrjVncApp");

   return (my_type == element->getDescToken());
}

bool wandApp::configAdd(jccl::ConfigChunkPtr element)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "wandApp::configAdd()\n"
                                             << vprDEBUG_FLUSH;

   if ( element->getProperty<bool>("embeddedGUI") )
   {
      std::string vnc_host = element->getProperty<std::string>("vncHost");
      vpr::Uint16 vnc_port = element->getProperty<vpr::Uint16>("vncPort");
      std::string vnc_pass = element->getProperty<std::string>("vncPassword");
      float       vnc_size = element->getProperty<float>("vncDesktopSideLength");

      try
      {
         mDesktop = new VNCDesktop(vnc_host, vnc_port, vnc_pass, vnc_size);
         mDesktop->init("VJWand", "VJButton0", "VJButton1", "VJButton2");
         mEmbeddedGUI = true;
      }
      catch (...)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR:")
            << " Failed to initialize VNC desktop\n" << vprDEBUG_FLUSH;
         mEmbeddedGUI = false;
      }
   }

   return true;
}

void wandApp::preFrame()
{
   bool we_have_focus(true);

   mFrameCount++;

   vpr::Interval frame_time = vpr::Interval::now() - mPrevFrameStartTime;
   mPrevFrameStartTime.setNow();
   mFrameRate.addSample(frame_time.secf());

   if((mFrameCount % 50) == 1)
   {
      mDesktop->printStats();

      // fps = 1.0f / time_per_frame
      float fps = 1.0f / mFrameRate.getSTA();
      std::cout << "fps: " << fps << std::endl;
   }

   // Update embedded VNC desktop
   if ( mEmbeddedGUI )
   {
      gmtl::Matrix44f nav_matrix;
      VNCDesktop::Focus focus = mDesktop->update(nav_matrix);
      we_have_focus = (focus == VNCDesktop::NOT_IN_FOCUS);
   }


   // Put your pre frame computations here.

   /*
       std::cout  << "Wand Buttons:"
              << " 0:" << mButton0->getData()
              << " 1:" << mButton1->getData()
              << " 2:" << mButton2->getData()
              << " 3:" << mButton3->getData()
              << " 4:" << mButton4->getData()
              << " 5:" << mButton5->getData() << std::endl;
             */
   if(we_have_focus)
   {
      // Update navigation
      // - Find forward direction of wand
      // - Translate along that direction
      float velocity(0.0f);
      if(mButton0->getData())
         velocity = 0.05f;

      if(velocity > 0.0f)
      {
         gmtl::Matrix44f wandMatrix;
         wandMatrix = (*mWand->getData());      // Get the wand matrix
         gmtl::Vec3f Zdir = gmtl::Vec3f(0.0f, 0.0f, velocity);
         gmtl::Vec3f direction(wandMatrix * Zdir);
         gmtl::preMult(mNavMat, gmtl::makeTrans<gmtl::Matrix44f>(direction));
      }

      // Check logger play button
      if(mLoggerPlayButton->getData() == gadget::Digital::TOGGLE_ON)
      {
         std::cout << "\n\n------ Log Play Button hit ----\n" << std::flush;
         gadget::InputManager* input_mgr = gadget::InputManager::instance();
         gadget::InputLoggerPtr logger = input_mgr->getInputLogger();

         vprASSERT(logger.get() != NULL);
         logger->load("test_logging.xml");
         logger->play();
      }
   }

}

void wandApp::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
}


//----------------------------------------------
//  Draw the scene.  A box on the end of the wand
//----------------------------------------------

void wandApp::myDraw()
{
   glClear(GL_DEPTH_BUFFER_BIT);
   //std::cout << "\n--- myDraw() ---\n";

   //std::cout << "HeadPos:" << vrj::Coord(*mHead->getData()).pos << "\t"
   //          << "WandPos:" << vrj::Coord(*mWand->getData()).pos << std::endl;

   glMatrixMode(GL_MODELVIEW);

   // -- Draw box on wand --- //
   gmtl::Matrix44f* wandMatrix;
   wandMatrix = mWand->getData();      // Get the wand matrix

   glPushMatrix();
   {
      // Move world for navigation
      glMultMatrixf(mNavMat.mData);

      // cout << "wand:\n" << *wandMatrix << endl;
      glPushMatrix();
         glMultMatrixf(wandMatrix->mData);  // Push the wand matrix on the stack
         //glColor3f(1.0f, 0.0f, 1.0f);
         float wand_color[3];
         wand_color[0] = wand_color[1] = wand_color[2] = 0.0f;
         if(mButton0->getData() == gadget::Digital::ON)
            wand_color[0] += 0.5f;
         if(mButton1->getData() == gadget::Digital::ON)
            wand_color[1] += 0.5f;
         if(mButton2->getData() == gadget::Digital::ON)
            wand_color[2] += 0.5f;
         if(mButton3->getData() == gadget::Digital::ON)
            wand_color[0] += 0.5f;
         if(mButton4->getData() == gadget::Digital::ON)
            wand_color[1] += 0.5f;
         if(mButton5->getData() == gadget::Digital::ON)
            wand_color[2] += 0.5f;
         glColor3fv(wand_color);
         glScalef(0.25f, 0.25f, 0.25f);
         drawCube();
      glPopMatrix();

      // Draw a floor
      drawFloor();

      // VNC GUI
      if ( mEmbeddedGUI )
      {  mDesktop->draw(); }
   }
   glPopMatrix();
}

void wandApp::initGLState()
{
   GLfloat light0_ambient[] = { 0.2f,  0.2f,  0.2f,  1.0f};
   GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.5f, 0.75f, 0.75f, 0.0f};

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

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

//: Utility function for drawing a cube
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
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

void wandApp::drawFloor()
{
   glPolygonMode(GL_FRONT, GL_LINE);
   glLineWidth(2);
   glColor3f(0.30f, 0.87f, 0.22f);     // Nice matrix green

   glBegin(GL_LINES);
      for ( int z = -25; z < 25; ++z )
      {
         for ( int x = -25; x < 25; ++x )
         {
            // Draw the line along the Z axis.
            glVertex3f((float) x, 0.01f, (float) z);
            glVertex3f((float) x, 0.01f, (float) z + 1.0f);

            // Draw the line along the X axis.
            glVertex3f((float) x, 0.01f, (float) z);
            glVertex3f((float) x + 1.0f, 0.01f, (float) z);
         }
      }
   glEnd();

   glPolygonMode(GL_FRONT, GL_FILL);
}

} // namespace vrjvnc

