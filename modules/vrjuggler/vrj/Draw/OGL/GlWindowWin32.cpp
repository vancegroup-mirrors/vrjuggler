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

#include <vrj/vrjConfig.h>

#include <jccl/Config/ConfigElement.h>

#include <vpr/Util/Assert.h>
#include <gadget/InputManager.h>
#include <vrj/Util/Debug.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlWindowWin32.h>

static const char* GL_WINDOW_WIN32_CLASSNAME("vrj::GlWindowWin32");

namespace vrj
{

GlWindowWin32::GlWindowWin32()
   : mWinHandle(NULL)
   , mRenderContext(NULL)
   , mDeviceContext(NULL)
{
   /* Do nothing. */ ;
}

GlWindowWin32::~GlWindowWin32()
{
   this->close();
}

// Open the window
// - Creates a window
// - Creates a rendering context
// - Registers new window with the window list
bool GlWindowWin32::open()
{
   if ( false == GlWindowWin32::registerWindowClass() )
   {
      return 0;
   }

   if ( mWindowIsOpen )
   {
      return 1;
   }

   HMODULE hMod = GetModuleHandle(NULL);
   DWORD style;
   int root_height;

   // OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS.
   style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

   // If we want a border, create an overlapped window.  This will have
   // a titlebar and a border.
   if ( mHasBorder )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
         << "[vrj::GlWindowWin32::open()] Attempting to give window a border"
         << std::endl << vprDEBUG_FLUSH;
      style |= WS_OVERLAPPEDWINDOW;
   }
   // Otherwise, come as close as possible to having no border by using
   // the thin-line border.
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
         << "[vrj::GlWindowWin32::open()] Attempting to make window borderless"
         << std::endl << vprDEBUG_FLUSH;
      style |= WS_OVERLAPPED | WS_POPUP | WS_VISIBLE;
   }

   root_height = GetSystemMetrics(SM_CYSCREEN);

   // Create the main application window
   mWinHandle = CreateWindow(GL_WINDOW_WIN32_CLASSNAME, mWindowName.c_str(),
                             style, mOriginX,
                             root_height - mOriginY - mWindowHeight,
                             mWindowWidth, mWindowHeight, NULL, NULL, hMod,
                             NULL);

   // If window was not created, quit
   if ( NULL == mWinHandle )
   {
      return 0;
   }

   // Attach a pointer to the device for use from the WNDPROC
   SetWindowLong(mWinHandle, GWL_USERDATA, (LPARAM) this);

   // We have a valid window, so... Create the context
   mDeviceContext = GetDC(mWinHandle);            // Store the device context
   if ( false == setPixelFormat(mDeviceContext) ) // Select the pixel format
   {
      return 0;
   }

   // Create the rendering context and make it current
   mRenderContext = wglCreateContext(mDeviceContext);
   vprASSERT(mRenderContext != NULL);
   wglMakeCurrent(mDeviceContext, mRenderContext);

   // Register the window with the window list
   GlWindowWin32::addWindow(mWinHandle,this);

   // Display the window
   ShowWindow(mWinHandle, SW_SHOW);
   UpdateWindow(mWinHandle);             // Tell the window to paint
   mWindowIsOpen = true;

   // ----------- Event window device starting -------------- //
   // Are we going to act like as an event source?
   if (true == mIsEventSource)
   {
      this->becomeEventWindowDevice();
   }

   // If mHideMouse is true we must pass false to ShowCursor
   ShowCursor(! mHideMouse);

   return 1;
}

/** Do the stuff needed to become an event window. */
void GlWindowWin32::becomeEventWindowDevice()
{
   // Set the parameters that we will need to get events
   gadget::EventWindowWin32::m_hWnd = mWinHandle;

   gadget::Input* dev_ptr = dynamic_cast<gadget::Input*>(this);
   vprASSERT( dev_ptr != NULL );

   // @todo Possibly not the best way to add this to input manager
   // - What happens when the event window is removed at run-time???
   // - What happens when this is called again?
   vrj::Kernel::instance()->getInputManager()->addDevice(dev_ptr);
}

void GlWindowWin32::removeEventWindowDevice()
{
   gadget::EventWindowWin32::m_hWnd = 0;

   gadget::Input* dev_ptr = dynamic_cast<gadget::Input*>(this);
   vprASSERT( dev_ptr != NULL );

   // @todo Possibly not the best way to add this to input manager
   // - What happens when the event window is removed at run-time???
   // - What happens when this is called again?

   // @todo reenable this when InputManager::removeDevice(Input*) is public
   //vrj::Kernel::instance()->getInputManager()->removeDevice( dev_ptr );
}

/**
 * Closes the OpenGL window.
 * @note Must be called by the same thread that called open.
 */
bool GlWindowWin32::close()
{
   // if not open, then don't bother.
   if ( !mWindowIsOpen )
   {
      return false;
   }

   if (mIsEventSource)
   {
      this->removeEventWindowDevice();
   }

   // Remove window from window list
   GlWindowWin32::removeWindow(mWinHandle);

   mWindowIsOpen = false;

   // destroy the win32 window
   return(1 == DestroyWindow(mWinHandle));
}

/**
 * Sets the current OpenGL context to this window.
 * @post this.context is active context.
 */
bool GlWindowWin32::makeCurrent()
{
   vprASSERT((mDeviceContext != NULL) && (mRenderContext != NULL));
   wglMakeCurrent(mDeviceContext, mRenderContext);  // Make our context current
   return true;
}

// Swap the front and back buffers
// Process events here
void GlWindowWin32::swapBuffers()
{
   vprASSERT(mDeviceContext != NULL);
   SwapBuffers(mDeviceContext);
}

void GlWindowWin32::checkEvents()
{
   if (true == mIsEventSource)
   {
      /** Sample from the window */
      gadget::EventWindowWin32::sample();
      // if event source, use its event processor
      // it will pass all events up to this->processEvent()
      // when done, so that we can see the messages.
   }
   else
   {
      // not an event source, so pump our own events
      MSG win_message;
      while ( PeekMessage(&win_message, NULL, 0, 0, PM_REMOVE) )
      {
         // Test if quit
         if (win_message.message == WM_QUIT)
         {
            break;
         }

         TranslateMessage(&win_message);     // Translate the accelerator keys
         DispatchMessage(&win_message);      // Send to the WinProc
      }
   }
}

void GlWindowWin32::processEvent(UINT message, UINT wParam, LONG lParam)
{

}

void GlWindowWin32::configWindow(vrj::Display* disp)
{
   const char neg_one_STRING[] = "-1";
   vprASSERT( disp != NULL );
   vrj::GlWindow::configWindow(disp);

    // Get the vector of display chunks
   jccl::ConfigElementPtr disp_sys_elt =
      DisplayManager::instance()->getDisplaySystemElement();
   jccl::ConfigElementPtr display_elt = disp->getConfigElement();

   mWindowName = disp->getName();
   mPipe = disp->getPipe();
   vprASSERT(mPipe >= 0);

   mXDisplayName = disp_sys_elt->getProperty<std::string>("x11_pipes", mPipe);
   if (mXDisplayName == neg_one_STRING)    // Use display env
   {
       const std::string DISPLAY_str("DISPLAY");
       const char* d = getenv(DISPLAY_str.c_str());
       if (NULL != d)
       {
          mXDisplayName = std::string( d );
       }
   }
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
      << "glxWindow::config: display name is: "
      << mXDisplayName << std::endl << vprDEBUG_FLUSH;

   bool was_i_a_keyboard = mIsEventSource;
   mIsEventSource = display_elt->getProperty<bool>("act_as_event_source");

   // If i'm being configured to NOT be an event source, and I was one already.
   if (false == mIsEventSource && true == was_i_a_keyboard)
   {
      this->removeEventWindowDevice();
   }

   // If i'm being configured to be an event source, and I wasn't one already.
   else if (true == mIsEventSource && false == was_i_a_keyboard)
   {
      // Configure event window device portion.
      jccl::ConfigElementPtr event_win_chunk =
         display_elt->getProperty<jccl::ConfigElementPtr>("event_window_device");

      // Set the name of the chunk to the same as the parent chunk (so we can
      // point at it).
      //event_win_chunk->setProperty("name", display_elt->getName();

      gadget::EventWindowWin32::config(event_win_chunk);

      // Custom configuration
      gadget::EventWindowWin32::mWidth = GlWindowWin32::mWindowWidth;
      gadget::EventWindowWin32::mHeight = GlWindowWin32::mWindowHeight;

      mWeOwnTheWindow = false;      // Event window device does not own window

      // if the window is already open, then make it an event window device
      // otherwise, this will be called once the window opens.
      if (mWindowIsOpen)
      {
         this->becomeEventWindowDevice();
      }
   }
}

// WindowProcedure to deal with the events generated.
// Called only for the window that we are controlling
LRESULT GlWindowWin32::handleEvent(HWND hWnd, UINT message, WPARAM wParam,
                                   LPARAM lParam)
{
   switch ( message )
   {
      // ---- Window creation, setup for OpenGL ---- //
      case WM_CREATE:
         vprASSERT(false);                               // Should never get called because
                                                         //we are not registered when this gets called

         mDeviceContext = GetDC(hWnd);                   // Store the device context
         if ( false == setPixelFormat(mDeviceContext) )  // Select the pixel format
         {
            return 0;
         }

         // Create the rendering context and make it current
         mRenderContext = wglCreateContext(mDeviceContext);
         wglMakeCurrent(mDeviceContext, mRenderContext);
         break;

         // ---- Window is being destroyed, cleanup ---- //
      case WM_DESTROY:

         // Deselect the current rendering context and delete it
         wglMakeCurrent(mDeviceContext, NULL);
         wglDeleteContext(mRenderContext);

         // Tell the application to terminate after the window
         // is gone.
         PostQuitMessage(0);
         break;

         // --- Window is resized. --- //
      case WM_SIZE:
         // Call our function which modifies the clipping
         // volume and viewport
         sizeChanged(LOWORD(lParam), HIWORD(lParam));
         break;


         // The painting function.  This message sent by Windows
         // whenever the screen needs updating.
      case WM_PAINT:
         {
            PAINTSTRUCT ps;         // Paint structure
            BeginPaint(hWnd, &ps);  // Validate the drawing of the window
            EndPaint(hWnd, &ps);
         }
         break;

      default:   // Passes it on if unproccessed
         return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return(0L);
}

// Set the pixel format for the given window
bool GlWindowWin32::setPixelFormat(HDC hDC)
{
   PIXELFORMATDESCRIPTOR pfd;

   memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   pfd.nSize = (sizeof(PIXELFORMATDESCRIPTOR));
   pfd.nVersion = 1;

   int red_size(8), green_size(8), blue_size(8), alpha_size(8), db_size(32);
   int visual_id(-1);
   jccl::ConfigElementPtr gl_fb_chunk = mVrjDisplay->getGlFrameBufferConfig();

   if ( gl_fb_chunk.get() != NULL )
   {
      visual_id  = gl_fb_chunk->getProperty<int>("visual_id");
      red_size   = gl_fb_chunk->getProperty<int>("red_size");
      green_size = gl_fb_chunk->getProperty<int>("green_size");
      blue_size  = gl_fb_chunk->getProperty<int>("blue_size");
      alpha_size = gl_fb_chunk->getProperty<int>("alpha_size");
      db_size    = gl_fb_chunk->getProperty<int>("depth_buffer_size");

      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Red channel size was negative, set to: " << red_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Green channel size was negative, set to: "
            << green_size << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Blue channel size was negative, set to: " << blue_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Alpha channel size was negative, set to: "
            << alpha_size << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Depth buffer size was negative, set to: " << db_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }
   }

   int pixel_format;

   if ( visual_id != -1 )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "Requesting visual 0x" << std::hex << visual_id << std::dec
         << " from WGL." << std::endl << vprDEBUG_FLUSH;

      int result = DescribePixelFormat(hDC, visual_id,
                                       sizeof(PIXELFORMATDESCRIPTOR), &pfd);

      if ( result == 0 )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "ERROR: Failed to get requested visual (ID 0x" << std::hex
            << visual_id << std::dec << ")\n" << vprDEBUG_FLUSH;
         // XXX: It would be helpful to print out the Windows error message.
//         DWORD error = GetLastError();
         return false;
      }

      mInStereo = (mVrjDisplay->isStereoRequested() &&
                   (pfd.dwFlags & PFD_STEREO));
      pixel_format = visual_id;
   }
   else
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_CONFIG_LVL)
         << "Frame buffer visual settings for " << mVrjDisplay->getName()
         << ": R:" << red_size << " G:" << green_size << " B:" << blue_size
         << " A:" << alpha_size << " DB:" << db_size << std::endl
         << vprDEBUG_FLUSH;

      /* Defaults. */
      pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

      if ( mVrjDisplay->isStereoRequested() )
      {
         mInStereo = true;
         pfd.dwFlags |= PFD_STEREO;
      }
      else
      {
         mInStereo = false;
      }

      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.cRedBits = red_size;
      pfd.cGreenBits = green_size;
      pfd.cBlueBits = blue_size;
      pfd.cAlphaBits = alpha_size;
      pfd.cDepthBits = db_size;
      pfd.cStencilBits = 0;
      pfd.cAccumBits = 0;
      pfd.cAuxBuffers = 0;

      // Let Win32 choose one for us
      pixel_format = ChoosePixelFormat(hDC, &pfd);

      if ( pixel_format > 0 )
      {
         PIXELFORMATDESCRIPTOR match;
         DescribePixelFormat(hDC, pixel_format, sizeof(PIXELFORMATDESCRIPTOR),
                             &match);

         // ChoosePixelFormat is dumb in that it will return a pixel format
         // that doesn't have stereo even if it was requested so we need to
         // make sure that if stereo was selected, we got it.
         if ( mVrjDisplay->isStereoRequested() )
         {
            if ( !(match.dwFlags & PFD_STEREO) )
            {
               return false;
            }
         }
      }
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Visual ID: 0x" << std::hex << pixel_format << std::dec
      << std::endl << vprDEBUG_FLUSH;

   // Set the pixel format for the device context
   SetPixelFormat(hDC, pixel_format, &pfd);
   return true;
}

// the user has changed the size of the window
void GlWindowWin32::sizeChanged(long width, long height)
{
   // Make sure we don't have window of 1 size (divide by zero would follow).
   if ( width == 0 )
   {
      width = 1;
   }

   if ( height == 0 )
   {
      height = 1;
   }

   updateOriginSize(mOriginX, mOriginY, width, height);
   setDirtyViewport(true);
}

/**
 * Global Window event handler.
 */
LRESULT CALLBACK GlWindowWin32::WndProc(HWND hWnd, UINT message,
                                        WPARAM wParam, LPARAM lParam)
{
   GlWindowWin32* glWin = getGlWin(hWnd);

   if ( glWin != NULL )       // Message for one of ours
   {
      glWin->processEvent(message, wParam, lParam);
      return glWin->handleEvent(hWnd, message, wParam, lParam);
   }
   else
   {
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
}

/**
 * Window registration.
 */
bool GlWindowWin32::mWinRegisteredClass = false;
WNDCLASS GlWindowWin32::mWinClass;           // The window class to register
std::map<HWND, GlWindowWin32*> GlWindowWin32::mGlWinMap;

bool GlWindowWin32::registerWindowClass()
{
   if ( mWinRegisteredClass )
   {
      return true;
   }

   char lpszAppName[1024];
   GetModuleFileName(NULL,lpszAppName,sizeof(lpszAppName));

   mWinRegisteredClass = true;     // We have registered now

   HINSTANCE hInstance = GetModuleHandle(NULL);

   // Register Window style
   mWinClass.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   mWinClass.lpfnWndProc = (WNDPROC)GlWindowWin32::WndProc;
   mWinClass.cbClsExtra  = 0;
   mWinClass.cbWndExtra  = 0;
   mWinClass.hInstance   = hInstance;            // Get handle to the module that created current process
   mWinClass.hIcon       = LoadIcon(NULL, IDI_WINLOGO);;
   mWinClass.hCursor     = LoadCursor(NULL, IDC_ARROW);

   // No need for background brush for OpenGL window
   mWinClass.hbrBackground  = NULL;

   mWinClass.lpszMenuName   = NULL;
   mWinClass.lpszClassName  = GL_WINDOW_WIN32_CLASSNAME;

   // Register the window class
   if ( RegisterClass(&mWinClass) == 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

void GlWindowWin32::addWindow(HWND handle, GlWindowWin32* glWin)
{
   vprASSERT(glWin != NULL);

   if ( mGlWinMap.find(handle) == mGlWinMap.end() )     // Not already there
   {
      mGlWinMap[handle] = glWin;
   }
}

void GlWindowWin32::removeWindow(HWND handle)
{
   mGlWinMap.erase(handle);     // Erase the entry in the list
}

GlWindowWin32* GlWindowWin32::getGlWin(HWND handle)
{
   std::map<HWND, GlWindowWin32*>::iterator glWinIter;

   glWinIter = mGlWinMap.find(handle);
   if ( glWinIter == mGlWinMap.end() )     // Not found
   {
      return NULL;
   }
   else
   {
      return(*glWinIter).second;                 // Return the found window
   }
}

} // End of vrj namespace