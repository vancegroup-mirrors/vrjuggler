dnl ===========================================================================
dnl Define macros useful when working in a Win32/DOS-based environment.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_WIN32_SETUP - Define path conversion (DOS to UNIX and UNIX to DOS)
dnl                       subroutines.
dnl 
dnl Subroutines defined:
dnl     unix2dos - Convert a UNIX-style path into a DOS-style path.
dnl     dos2unix - Convert a DOS-style path into a UNIX-style path.
dnl ===========================================================================

dnl win32.m4,v 1.3 2000/12/19 16:56:23 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Define path conversion (DOS to UNIX and UNIX to DOS) subroutines.
dnl
dnl Usage:
dnl     DPP_WIN32_SETUP
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_WIN32_SETUP,
[
    dnl UNIX path translator.  This converts a UNIX-style path to a DOS/Win32
    dnl path.  With this routine, we can safely use UNIX paths for all Win32
    dnl variables while the configure script runs.  Then, immediately before
    dnl Makefile substitution is done, we can translate all the paths to
    dnl DOS-style for use in the Makefiles.
    unix2dos ( ) {
        dnl The first expression changes a "//C" drive name to "C:".  It has a
        dnl pair of surrounding []'s because those symbols have special meaning
        dnl to m4(1).  Without them, the first pair of []'s in the regular
        dnl expression will be lost.  Similarly, the second expression chagnes
        dnl a "/cygdrive/C" drive name to "C:".
        dnl The third expression ensures that a path beginning with / is
        dnl returned as being a directory on the home drive.  The UNIX utilities
        dnl treat $HOMEDRIVE as the root directory and will give, for example,
        dnl /tmp instead of //D/tmp if D: is the home drive.  The expression
        dnl simply prepends "$HOMEDRIVE" (after evaluation) to the path.
        dpp_drive_exp=['s/^\/\/\([[:alpha:]]\)/\1:/']
        dpp_drive_exp2=['s/^\/cygdrive\/\([[:alpha:]]\)/\1:/']
        dpp_add_home_exp=["s/^\//$HOMEDRIVE\//"]
        dpp_retval=`echo ${1} | sed -e "$dpp_drive_exp" -e "$dpp_drive_exp2" -e "$dpp_add_home_exp"`

        echo "$dpp_retval"
    }

    dnl DOS path translator.  This converts a DOS-style path to a UNIX path.
    dos2unix ( ) {
        dnl All expressions hers are enclosed in []'s because those symbols
        dnl have special meaning to m4(1).  This is the same situation as
        dnl above.

        dnl This changes \'s to /'s and escapes any whitespace in the path.
        dpp_slash_exp=['s/\\/\//g']
        dpp_ws_esc_exp=['s/ /\\ /g']
        dpp_unix_root=`echo "${1}" | sed -e "$dpp_slash_exp" -e "$dpp_ws_esc_exp"`

        dnl This changes the leading "C:" to "//C" (for the appropriate drive
        dnl letter).
        dpp_colon_rem_exp=['s/^\([[:alpha:]]\):/\/\/\1/']
        dpp_retval=`echo "$dpp_unix_root" | sed -e "$dpp_colon_rem_exp"`

        echo "$dpp_retval"
    }
])
