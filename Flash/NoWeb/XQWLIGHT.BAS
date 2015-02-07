Attribute VB_Name = "mdlXQWLight"
Option Explicit

Private Const WM_LBUTTONDOWN As Long = &H201
Private Const WM_RBUTTONDOWN As Long = &H204

Private Declare Function CallWindowProcA Lib "USER32.DLL" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

Public PrevWndProc As Long

Public Function WndProc(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

If uMsg = WM_RBUTTONDOWN Then
    WndProc = CallWindowProcA(PrevWndProc, hWnd, WM_LBUTTONDOWN, wParam, lParam)
Else
    WndProc = CallWindowProcA(PrevWndProc, hWnd, uMsg, wParam, lParam)
End If

End Function
