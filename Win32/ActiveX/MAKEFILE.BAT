DEL upload\xqwlight.cab
SIGNCODE /SPC ..\..\..\XQWIZARD\MAKEFILE\XQBASE.SPC /V ..\..\..\XQWIZARD\MAKEFILE\XQBASE.PVK Release\xqwlight.ocx
MAKECAB /F MAKEFILE\FILELIST.TXT
SIGNCODE /SPC ..\..\..\XQWIZARD\MAKEFILE\XQBASE.SPC /V ..\..\..\XQWIZARD\MAKEFILE\XQBASE.PVK disk1\1.cab
MOVE disk1\1.cab upload\xqwlight.cab
RD disk1
DEL setup.inf
DEL setup.rpt