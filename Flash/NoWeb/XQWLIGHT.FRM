VERSION 5.00
Object = "{D27CDB6B-AE6D-11CF-96B8-444553540000}#1.0#0"; "Flash10e.ocx"
Begin VB.Form frmXQWLight 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "象棋小巫师"
   ClientHeight    =   8880
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9240
   BeginProperty Font 
      Name            =   "宋体"
      Size            =   9
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "XQWLIGHT.frx":0000
   MaxButton       =   0   'False
   ScaleHeight     =   592
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   616
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lstLevel 
      Height          =   600
      ItemData        =   "XQWLIGHT.frx":030A
      Left            =   8040
      List            =   "XQWLIGHT.frx":0317
      TabIndex        =   8
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CheckBox chkSound 
      Caption         =   "音效"
      Height          =   255
      Left            =   8040
      TabIndex        =   9
      Top             =   4080
      Value           =   1  'Checked
      Width           =   735
   End
   Begin VB.CommandButton btnRetract 
      Caption         =   "悔棋"
      Height          =   375
      Left            =   8040
      TabIndex        =   6
      Top             =   2400
      Width           =   1095
   End
   Begin VB.ListBox lstHandicap 
      Height          =   780
      ItemData        =   "XQWLIGHT.frx":032D
      Left            =   8040
      List            =   "XQWLIGHT.frx":033D
      TabIndex        =   4
      Top             =   1080
      Width           =   1095
   End
   Begin VB.ListBox lstFlipped 
      Height          =   420
      ItemData        =   "XQWLIGHT.frx":0361
      Left            =   8040
      List            =   "XQWLIGHT.frx":036B
      TabIndex        =   2
      Top             =   360
      Width           =   1095
   End
   Begin VB.CommandButton btnRestart 
      Caption         =   "重新开始"
      Height          =   375
      Left            =   8040
      TabIndex        =   5
      Top             =   1920
      Width           =   1095
   End
   Begin ShockwaveFlashObjectsCtl.ShockwaveFlash flash 
      Height          =   8640
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7800
      _cx             =   13758
      _cy             =   15240
      FlashVars       =   ""
      Movie           =   ""
      Src             =   ""
      WMode           =   "Transparent"
      Play            =   "0"
      Loop            =   "-1"
      Quality         =   "High"
      SAlign          =   ""
      Menu            =   "0"
      Base            =   ""
      AllowScriptAccess=   "always"
      Scale           =   "ShowAll"
      DeviceFont      =   "0"
      EmbedMovie      =   "0"
      BGColor         =   ""
      SWRemote        =   ""
      MovieData       =   ""
      SeamlessTabbing =   "1"
      Profile         =   "0"
      ProfileAddress  =   ""
      ProfilePort     =   "0"
      AllowNetworking =   "all"
      AllowFullScreen =   "false"
   End
   Begin VB.Image Image1 
      Height          =   8655
      Left            =   120
      Picture         =   "XQWLIGHT.frx":0381
      Top             =   120
      Width           =   7815
   End
   Begin VB.Label lblLevel 
      Caption         =   "电脑水平："
      Height          =   255
      Left            =   8040
      TabIndex        =   7
      Top             =   3000
      Width           =   1095
   End
   Begin VB.Label lblHandicap 
      Caption         =   "先走让子："
      Height          =   255
      Left            =   8040
      TabIndex        =   3
      Top             =   840
      Width           =   1095
   End
   Begin VB.Label lblFlipped 
      Caption         =   "谁先走："
      Height          =   255
      Left            =   8040
      TabIndex        =   1
      Top             =   120
      Width           =   1095
   End
End
Attribute VB_Name = "frmXQWLight"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function ChildWindowFromPoint Lib "USER32.DLL" (ByVal hWndParent As Long, ByVal y As Long, ByVal x As Long) As Long
Private Declare Function GetWindowLongA Lib "USER32.DLL" (ByVal hWnd As Long, ByVal nIndex As Long) As Long
Private Declare Sub SetWindowLongA Lib "USER32.DLL" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long)

Private Const GWL_WNDPROC As Long = -4

Private bLoaded As Boolean

Private Function CallFlash(ByVal szFunc As String, ParamArray szParam() As Variant) As String

Dim sz As String, i As Integer
sz = "<invoke name=""" + szFunc + """ returntype=""xml""><arguments>"
For i = LBound(szParam) To UBound(szParam)
    If VarType(szParam(i)) = vbBoolean Then
        sz = sz + IIf(CBool(szParam(i)), "<true/>", "<false/>")
    Else
        sz = sz + "<string>" + CStr(szParam(i)) + "</string>"
    End If
Next
sz = sz + "</arguments></invoke>"
CallFlash = flash.CallFunction(sz)

End Function

Private Sub Form_Load()

Dim x As Long, y As Long, hWndFlash As Long

flash.Movie = App.Path + IIf(Right(App.Path, 1) = "\", "", "\") + "XQWLight.swf"
bLoaded = False
lstFlipped.ListIndex = 0
lstHandicap.ListIndex = 0
lstLevel.ListIndex = 0
bLoaded = True

hWndFlash = ChildWindowFromPoint(hWnd, flash.Left, flash.Top)
PrevWndProc = GetWindowLongA(hWndFlash, GWL_WNDPROC)
SetWindowLongA hWndFlash, GWL_WNDPROC, AddressOf WndProc

End Sub

Private Sub btnRestart_Click()

CallFlash "restart", lstFlipped.ListIndex = 1, lstHandicap.ListIndex

End Sub

Private Sub btnRetract_Click()

CallFlash "retract"

End Sub

Private Sub lstLevel_Click()

If bLoaded Then
    CallFlash "setLevel", lstLevel.ListIndex
End If

End Sub

Private Sub chkSound_Click()

CallFlash "setSound", chkSound.Value <> 0

End Sub
