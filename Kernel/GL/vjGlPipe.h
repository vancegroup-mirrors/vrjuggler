#ifndef _VJ_GL_PIPE_
#define _VJ_GL_PIPE_

#include <config.h>

#ifdef VJ_OS_HPUX
#   include <vector>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#endif

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/GL/vjGlDrawManager.h>
class vjGlDrawManager;

#include <Sync/vjCond.h>

   
//--------------------------------------------------------------------------------------
//: Handles the rendering on multiple GLWindows in a single process.
//
//    This class can be used to group glWindows to render efficiently
//    on one pipe (SGI terminology).
//
// @author Allen Bierbaum
//  Date: 1-12-98
//---------------------------------------------------------------------------------------
class vjGlPipe
{
public:
      //: Constructor
      //!ARGS: _num - The id number of the pipe.
      //!NOTE: All contained windows SHOULD have the same pipe number
   vjGlPipe(int _num, vjGlDrawManager* glMgr) : controlExit(0), glManager(glMgr)
   {
      ;
   }

      //: Start the pipe running
      //!POST: The pipe has it's own thread of control and is ready to operate
      //+       The Thread of control is running controlLoop
   int start();
  
      //: Stop the pipe
      //!POST: Flag is set to tell pipe to stop rendering
   void stop()
   {
      controlExit = 1;     // Set the control loop exit flag
   }

      //: Trigger rendering of the pipe to start
      //!POST: The pipe has be told to start rendering
   void triggerRender();

      //: Blocks until rendering of the windows is completed
      //!POST: The pipe has completed its rendering
   void completeRender();
    
      //: Add a GLWindow to the new window list
      // Control loop must now open the window on the next frame
   void addWindow(vjGlWindow* win);

      //: Remove a GLWindow from the window list
   void removeWindow(vjGlWindow* win)
   {
      // XXX: Need to implement
         // Erase the window from the list
      //windows.erase(remove(windows.begin(), windows.end(), win), windows.end());
   }

      //: Returns true if pipe has any windows
   int hasWindows()
   {
       return ( (newWins.size() > 0) || (openWins.size() > 0));
   }

      //: The main loop routine
      // -Checks for new windows <br>
      // -renders all windows when triggered <br>
      // -
   void controlLoop(void* nullParam);

private:
      //:  Checks for any new windows to add to the pipe
      //! POST: Any new windows will be opened and added to the pipe's rendering list
   void checkForNewWindows();

   void checkForWindowsToClose()
   {
      ;
   }
   
      //:   Renders the window using OpenGL
      //!POST: win is rendered (In stereo if it is a stereo window)
   void renderWindow(vjGlWindow* win);   

public:
   enum RenderState {RENDERING, WAITING};    // Type to delineate the current rendering state

private:
   int   pipeNum;                //: The id of the pipe

   vector<vjGlWindow*> newWins;  //: List of windows still to be opened on current pipe
   vjMutex newWinLock;           //: Lock for accessing the newWin list

   vector<vjGlWindow*> openWins; //: List of current open windows to render
   vjMutex openWinLock;          //: Lock for accessing the openWinList

   vjGlDrawManager*    glManager;    //: The openlGL manager that we are rendering for
                                     //: Needed to get app, etc.

   vjCond      renderCond;          //: Condition variable for Redner state
   RenderState renderState;         //: The current render state.
   int         controlExit;         //: Flag for when to exit the control loop 
};

#endif
