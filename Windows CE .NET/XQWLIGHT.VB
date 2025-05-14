' XQWLIGHT.VB - Source Code for XiangQi Wizard Light, Part V
'
' XiangQi Wizard Light - a Chinese Chess Program for Windows CE
' Designed by Morning Yellow, Version: 1.21, Last Modified: Jan. 2008
' Copyright (C) 2004-2008 www.xqbase.com
'
' This program is free software; you can redistribute it and/or modify
' it under the terms of the GNU General Public License as published by
' the Free Software Foundation; either version 2 of the License, or
' (at your option) any later version.

' This program is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
' GNU General Public License for more details.

' You should have received a copy of the GNU General Public License along
' with this program; if not, write to the Free Software Foundation, Inc.,
' 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Module XQWLight
    Public Const SND_ASYNC As Long = 1
    Public Const SND_NOWAIT As Long = 8192
    Public Const SND_FILENAME As Long = 131072
    Public Declare Function PlaySound Lib "COREDLL.DLL" (ByVal szSound As String, ByVal hMod As IntPtr, ByVal flags As Integer) As Integer
    Public Declare Function PlaySoundA Lib "WINMM.DLL" (ByVal szSound As String, ByVal hmod As Integer, ByVal flags As Integer) As Integer

    Public Const DRAW_SELECTED As Boolean = True
    Public Const RESULT_COMPUTER As Boolean = True

    Public Const LEFT_MARGIN As Integer = 6
    Public Const TOP_MARGIN As Integer = 8

    Public App_frm As MainForm, App_szPath As String
    Public App_bExit As Boolean, App_bFlipped As Boolean, App_bSound As Boolean
    Public App_nHandicap As Integer, App_nLevel As Integer
    Public App_sqCursor As Integer, App_sqSelected As Integer, App_mvLast As Integer

    Public Function IIF_STR(ByVal b As Boolean, ByVal x As String, ByVal y As String) As String
        If b Then
            Return x
        Else
            Return y
        End If
    End Function

    Public Function AddMove(ByVal mv As Integer) As Boolean
        Dim sqSrc As Integer, sqDst As Integer
        If Search_pos.LegalMove(mv) Then
            sqSrc = SRC(mv)
            sqDst = DST(mv)
            If Search_pos.MakeMove(mv) Then
                PlaySound(IIF_STR(Search_pos.InCheck, "CHECK", IIF_STR(Search_pos.Captured, "CAPTURE", "MOVE")))
                If Search_pos.Captured Then
                    Search_pos.SetIrrev()
                End If
                App_sqSelected = 0
                App_mvLast = mv
                App_frm.DrawSquare(sqSrc, DRAW_SELECTED)
                App_frm.DrawSquare(sqDst, DRAW_SELECTED)
                Return True
            Else
                PlaySound("ILLEGAL")
            End If
        End If
        Return False
    End Function

    Public Sub PlaySound(ByVal szWavFile As String)
        If Not App_bSound Then
            Return
        End If
        Try
            PlaySound(App_szPath + "SOUNDS\" + szWavFile + ".WAV", IntPtr.Zero, SND_ASYNC + SND_NOWAIT + SND_FILENAME)
        Catch ex As Exception
            PlaySoundA(App_szPath + "SOUNDS\" + szWavFile + ".WAV", 0, SND_ASYNC + SND_NOWAIT + SND_FILENAME)
        End Try
    End Sub

    Public Function GetResult(Optional ByVal szWavFile As String = "") As Boolean
        Dim vlRep As Integer
        If Search_pos.IsMate Then
            MsgBox(IIF_STR(szWavFile = "", "祝贺你取得胜利！", "请再接再厉！"), MsgBoxStyle.Information, "棋局结束")
            Return True
        End If
        vlRep = Search_pos.RepStatus(3)
        If vlRep > 0 Then
            vlRep = Search_pos.RepValue(vlRep)
            If szWavFile <> "" Then
                vlRep = -vlRep
            End If
            MsgBox(IIF_STR(vlRep > WIN_VALUE, "长打作负，请不要气馁！", _
                    IIF_STR(vlRep < -WIN_VALUE, "电脑长打作负，祝贺你取得胜利！", "双方不变作和，辛苦了！")), MsgBoxStyle.Information, "棋局结束")
            Return True
        End If
        If Search_pos.nMoveNum > 100 Then
            MsgBox("超过自然限着作和，辛苦了！", MsgBoxStyle.Information, "棋局结束")
            Return True
        End If
        If szWavFile <> "" Then
            PlaySound(szWavFile)
        End If
        Return False
    End Function

    Public Function ResponseMove() As Boolean
        Dim sqSrc As Integer, sqDst As Integer, szWavFile As String
        If GetResult() Then
            Return False
        End If
        sqSrc = SRC(App_mvLast)
        sqDst = DST(App_mvLast)
        App_frm.imgThinking.Left = LEFT_MARGIN + IIF_INT(FILE_X(sqDst) < 8 Xor App_bFlipped, 112, 0)
        App_frm.imgThinking.Top = TOP_MARGIN + IIF_INT(RANK_Y(sqDst) < 8 Xor App_bFlipped, 128, 0)
        App_frm.imgThinking.Visible = True
        App_frm.Refresh()
        SearchMain(CInt(4 ^ App_nLevel))
        App_frm.imgThinking.Visible = False
        If App_mvLast > 0 Then
            App_frm.DrawSquare(sqSrc)
            App_frm.DrawSquare(sqDst)
        End If
        Search_pos.MakeMove(Search_mvResult)
        szWavFile = IIF_STR(Search_pos.InCheck, "CHECK2", IIF_STR(Search_pos.Captured, "CAPTURE2", "MOVE2"))
        If Search_pos.Captured Then
            Search_pos.SetIrrev()
        End If
        App_mvLast = Search_mvResult
        App_frm.DrawSquare(SRC(App_mvLast), DRAW_SELECTED)
        App_frm.DrawSquare(DST(App_mvLast), DRAW_SELECTED)
        If GetResult(szWavFile) Then
            Return False
        End If
        Return True
    End Function

    Public Sub Main()
        Dim frmStartUp As StartUp, nSlash As Integer, nBackSlash As Integer

        App_szPath = System.Reflection.Assembly.GetExecutingAssembly.GetName.CodeBase
        If Left(App_szPath, 8) = "file:///" Then
            App_szPath = Mid(App_szPath, 9)
        End If
        nSlash = InStrRev(App_szPath, "/")
        nBackSlash = InStrRev(App_szPath, "\")
        App_szPath = IIF_STR(nSlash = nBackSlash, "", Left(App_szPath, IIF_INT(nSlash > nBackSlash, nSlash, nBackSlash)))
        Randomize()
        InitArrays()
        LoadBook()
        ' Test.Test()
        frmStartUp = New StartUp
        App_frm = New MainForm
        App_bExit = True
        frmStartUp.ShowDialog()
        While Not App_bExit
            App_frm.ShowDialog()
            App_bExit = True
            frmStartUp.ShowDialog()
        End While
    End Sub
End Module