@ECHO OFF
rem ************* <auto-copyright.pl BEGIN do not edit this line> *************
rem
rem VR Juggler is (C) Copyright 1998-2003 by Iowa State University
rem
rem Original Authors:
rem   Allen Bierbaum, Christopher Just,
rem   Patrick Hartling, Kevin Meinert,
rem   Carolina Cruz-Neira, Albert Baker
rem
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of the GNU Library General Public
rem License as published by the Free Software Foundation; either
rem version 2 of the License, or (at your option) any later version.
rem
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Library General Public License for more details.
rem
rem You should have received a copy of the GNU Library General Public
rem License along with this library; if not, write to the
rem Free Software Foundation, Inc., 59 Temple Place - Suite 330,
rem Boston, MA 02111-1307, USA.
rem
rem -----------------------------------------------------------------
rem File:          $RCSfile$
rem Date modified: $Date$
rem Version:       $Revision$
rem -----------------------------------------------------------------
rem
rem ************** <auto-copyright.pl END do not edit this line> **************
IF NOT "%VJ_BASE_DIR%" == "" GOTO TEST_VJ_EXIST
ECHO [ERR] VJ_BASE_DIR unset; please set the environment variable VJ_BASE_DIR to
ECHO [ERR] point to your VR Juggler installation directory.  For more information,
ECHO [ERR] please see INSTALL.html
GOTO ERREXIT
:TEST_VJ_EXIST
IF EXIST "%VJ_BASE_DIR%" GOTO TEST_TWEEK_BASE
ECHO [ERR] The VJ_BASE_DIR %VJ_BASE_DIR% does not appear to exist.  Please
ECHO [ERR] check the path and try again.
GOTO ERREXIT
:TEST_TWEEK_BASE
IF NOT "%TWEEK_BASE_DIR%" == "" GOTO TEST_TWEEK_EXIST
ECHO  NOTE: Setting TWEEK_BASE_DIR to "%VJ_BASE_DIR%"
set TWEEK_BASE_DIR=%VJ_BASE_DIR%
:TEST_TWEEK_EXIST
IF EXIST "%TWEEK_BASE_DIR%" GOTO TEST_JCCL_DEF
ECHO [ERR] The TWEEK_BASE_DIR %TWEEK_BASE_DIR% does not appear to exist.  Please
ECHO [ERR] check the path and try again.
GOTO ERREXIT
IF NOT "%JCCL_BASE_DIR%" == "" GOTO TEST_JCCL_EXIST
ECHO  NOTE: Setting JCCL_BASE_DIR to "%VJ_BASE_DIR%"
set JCCL_BASE_DIR=%VJ_BASE_DIR%
:TEST_JCCL_EXIST
IF EXIST "%JCCL_BASE_DIR%" GOTO TEST_JCCL_DEF
ECHO [ERR] The JCCL_BASE_DIR %JCCL_BASE_DIR% does not appear to exist.  Please
ECHO [ERR] check the path and try again.
GOTO ERREXIT
:TEST_JCCL_DEF
IF NOT "%JCCL_DEFINITION_PATH%" == "" GOTO RUNJAVA
ECHO  NOTE: Setting JCCL_DEFINITION_PATH to "%VJ_BASE_DIR%\share\vrjuggler\data\definitions"
set JCCL_DEFINITION_PATH=%VJ_BASE_DIR%\share\vrjuggler\data\definitions
:RUNJAVA
java -DTWEEK_BASE_DIR="%TWEEK_BASE_DIR%" -DJCCL_BASE_DIR="%JCCL_BASE_DIR%" -DVJ_BASE_DIR="%VJ_BASE_DIR%" -DJCCL_DEFINITION_PATH="%JCCL_DEFINITION_PATH%" -Djava.security.policy="%TWEEK_BASE_DIR%/bin/java.security.policy.txt" -cp "%TWEEK_BASE_DIR%/bin/jaxen-core.jar;%TWEEK_BASE_DIR%/bin/jaxen-jdom.jar;%TWEEK_BASE_DIR%/bin/jdom.jar;%TWEEK_BASE_DIR%/bin/saxpath.jar;%TWEEK_BASE_DIR%/bin/xalan.jar;%TWEEK_BASE_DIR%/bin/xerces.jar;%TWEEK_BASE_DIR%/bin/xml-apis.jar;%TWEEK_BASE_DIR%/bin/Tweek.jar;%TWEEK_BASE_DIR%/bin/TweekBeans.jar;%TWEEK_BASE_DIR%/bin/TweekEvents.jar;%TWEEK_BASE_DIR%/bin/TweekNet.jar;%TWEEK_BASE_DIR%/bin/TweekBeanDelivery.jar;%TWEEK_BASE_DIR%/bin/TweekServices.jar;%TWEEK_BASE_DIR%/bin/kunststoff-mod.jar;%TWEEK_BASE_DIR%/bin/liquidlnf.jar;%TWEEK_BASE_DIR%/bin/metouia.jar;%TWEEK_BASE_DIR%/bin/looks.jar" org.vrjuggler.tweek.Tweek --defaultbean="Configuration Editor" %*
GOTO DONE
:ERREXIT
ECHO [ERR] VRJConfig exiting due to previous errors.
:DONE