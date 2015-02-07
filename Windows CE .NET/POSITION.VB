' POSITION.VB - Source Code for XiangQi Wizard Light, Part I
'
' XiangQi Wizard Light - a Chinese Chess Program for Windows CE
' Designed by Morning Yellow, Version: 1.21, Last Modified: Jan. 2008
' Copyright (C) 2004-2008 www.elephantbase.net
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

Imports System.IO

Module Position
    Public Const PIECE_DEL As Boolean = True
    Public Const CAP_ONLY As Boolean = True

    Public Const MATE_VALUE As Integer = 10000
    Public Const BAN_VALUE As Integer = MATE_VALUE - 100
    Public Const WIN_VALUE As Integer = MATE_VALUE - 200
    Public Const NULL_SAFE_MARGIN As Integer = 400
    Public Const NULL_OKAY_MARGIN As Integer = 200
    Public Const DRAW_VALUE As Integer = 20
    Public Const ADVANCED_VALUE As Integer = 3

    Public Const MAX_MOVE_NUM As Integer = 256
    Public Const MAX_GEN_MOVES As Integer = 128

    Public Const PIECE_KING As Integer = 1
    Public Const PIECE_ADVISOR As Integer = 2
    Public Const PIECE_BISHOP As Integer = 3
    Public Const PIECE_KNIGHT As Integer = 4
    Public Const PIECE_ROOK As Integer = 5
    Public Const PIECE_CANNON As Integer = 6
    Public Const PIECE_PAWN As Integer = 7

    Public Const RANK_TOP As Integer = 3
    Public Const RANK_BOTTOM As Integer = 12
    Public Const FILE_LEFT As Integer = 3
    Public Const FILE_RIGHT As Integer = 11

    Public BOARD_HANDICAP(3) As String
    Public IN_BOARD(255) As Boolean

    Private IN_FORT(255) As Boolean
    Private PIECE_VALUE(6, 255) As Integer

    Private KING_DELTA(3) As Integer
    Private ADVISOR_DELTA(3) As Integer
    Private KNIGHT_DELTA(3, 1) As Integer
    Private KNIGHT_CHECK_DELTA(3, 1) As Integer
    Private MVV_VALUE(7) As Integer

    Private LEGAL_SPAN(511) As Integer
    Private KNIGHT_PIN_TAB(511) As Integer

    Public Function IIF_INT(ByVal b As Boolean, ByVal x As Integer, ByVal y As Integer) As Integer
        If b Then
            Return x
        Else
            Return y
        End If
    End Function

    Public Sub SetBoolArray(ByVal bArray() As Boolean, ByVal ParamArray nData() As Integer)
        Dim i As Integer, n As Integer
        n = nData.Length - 1
        For i = 0 To n
            bArray(i) = nData(i) <> 0
        Next
    End Sub

    Public Sub SetStrArray(ByVal szArray() As String, ByVal ParamArray szData() As String)
        Dim i As Integer, n As Integer
        n = szData.Length - 1
        For i = 0 To n
            szArray(i) = szData(i)
        Next
    End Sub

    Public Sub SetIntArray(ByVal nArray() As Integer, ByVal ParamArray nData() As Integer)
        Dim i As Integer, n As Integer
        n = nData.Length - 1
        For i = 0 To n
            nArray(i) = nData(i)
        Next
    End Sub

    Public Sub SetIntArray2(ByVal nArray(,) As Integer, ByVal nIndex As Integer, ByVal ParamArray nData() As Integer)
        Dim i As Integer, n As Integer
        n = nData.Length - 1
        For i = 0 To n
            nArray(nIndex, i) = nData(i)
        Next
    End Sub

    Public Sub InitBoardFort()
        SetBoolArray(IN_BOARD, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetBoolArray(IN_FORT, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetIntArray(KING_DELTA, -16, -1, 1, 16)
        SetIntArray(ADVISOR_DELTA, -17, -15, 15, 17)
        SetIntArray2(KNIGHT_DELTA, 0, -33, -31)
        SetIntArray2(KNIGHT_DELTA, 1, -18, 14)
        SetIntArray2(KNIGHT_DELTA, 2, -14, 18)
        SetIntArray2(KNIGHT_DELTA, 3, 31, 33)
        SetIntArray2(KNIGHT_CHECK_DELTA, 0, -33, -18)
        SetIntArray2(KNIGHT_CHECK_DELTA, 1, -31, -14)
        SetIntArray2(KNIGHT_CHECK_DELTA, 2, 14, 31)
        SetIntArray2(KNIGHT_CHECK_DELTA, 3, 18, 33)
        SetIntArray(MVV_VALUE, 0, 50, 10, 10, 30, 40, 30, 20)
    End Sub

    Public Sub InitLegalSpan()
        SetIntArray(LEGAL_SPAN, _
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0)
        SetIntArray(KNIGHT_PIN_TAB, _
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, -16, 0, -16, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 16, 0, 16, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0)
    End Sub

    Public Sub InitPieceValue1()
        SetIntArray2(PIECE_VALUE, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 9, 9, 9, 11, 13, 11, 9, 9, 9, 0, 0, 0, 0, _
                0, 0, 0, 19, 24, 34, 42, 44, 42, 34, 24, 19, 0, 0, 0, 0, _
                0, 0, 0, 19, 24, 32, 37, 37, 37, 32, 24, 19, 0, 0, 0, 0, _
                0, 0, 0, 19, 23, 27, 29, 30, 29, 27, 23, 19, 0, 0, 0, 0, _
                0, 0, 0, 14, 18, 20, 27, 29, 27, 20, 18, 14, 0, 0, 0, 0, _
                0, 0, 0, 7, 0, 13, 0, 16, 0, 13, 0, 7, 0, 0, 0, 0, _
                0, 0, 0, 7, 0, 7, 0, 15, 0, 7, 0, 7, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 11, 15, 11, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetIntArray2(PIECE_VALUE, 1, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 18, 0, 0, 20, 23, 20, 0, 0, 18, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 20, 20, 0, 20, 20, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetIntArray2(PIECE_VALUE, 2, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 18, 0, 0, 20, 23, 20, 0, 0, 18, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 20, 20, 0, 20, 20, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    End Sub

    Public Sub InitPieceValue2()
        SetIntArray2(PIECE_VALUE, 3, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 90, 90, 90, 96, 90, 96, 90, 90, 90, 0, 0, 0, 0, _
                0, 0, 0, 90, 96, 103, 97, 94, 97, 103, 96, 90, 0, 0, 0, 0, _
                0, 0, 0, 92, 98, 99, 103, 99, 103, 99, 98, 92, 0, 0, 0, 0, _
                0, 0, 0, 93, 108, 100, 107, 100, 107, 100, 108, 93, 0, 0, 0, 0, _
                0, 0, 0, 90, 100, 99, 103, 104, 103, 99, 100, 90, 0, 0, 0, 0, _
                0, 0, 0, 90, 98, 101, 102, 103, 102, 101, 98, 90, 0, 0, 0, 0, _
                0, 0, 0, 92, 94, 98, 95, 98, 95, 98, 94, 92, 0, 0, 0, 0, _
                0, 0, 0, 93, 92, 94, 95, 92, 95, 94, 92, 93, 0, 0, 0, 0, _
                0, 0, 0, 85, 90, 92, 93, 78, 93, 92, 90, 85, 0, 0, 0, 0, _
                0, 0, 0, 88, 85, 90, 88, 90, 88, 90, 85, 88, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetIntArray2(PIECE_VALUE, 4, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 206, 208, 207, 213, 214, 213, 207, 208, 206, 0, 0, 0, 0, _
                0, 0, 0, 206, 212, 209, 216, 233, 216, 209, 212, 206, 0, 0, 0, 0, _
                0, 0, 0, 206, 208, 207, 214, 216, 214, 207, 208, 206, 0, 0, 0, 0, _
                0, 0, 0, 206, 213, 213, 216, 216, 216, 213, 213, 206, 0, 0, 0, 0, _
                0, 0, 0, 208, 211, 211, 214, 215, 214, 211, 211, 208, 0, 0, 0, 0, _
                0, 0, 0, 208, 212, 212, 214, 215, 214, 212, 212, 208, 0, 0, 0, 0, _
                0, 0, 0, 204, 209, 204, 212, 214, 212, 204, 209, 204, 0, 0, 0, 0, _
                0, 0, 0, 198, 208, 204, 212, 212, 212, 204, 208, 198, 0, 0, 0, 0, _
                0, 0, 0, 200, 208, 206, 212, 200, 212, 206, 208, 200, 0, 0, 0, 0, _
                0, 0, 0, 194, 206, 204, 212, 200, 212, 204, 206, 194, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetIntArray2(PIECE_VALUE, 5, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 100, 100, 96, 91, 90, 91, 96, 100, 100, 0, 0, 0, 0, _
                0, 0, 0, 98, 98, 96, 92, 89, 92, 96, 98, 98, 0, 0, 0, 0, _
                0, 0, 0, 97, 97, 96, 91, 92, 91, 96, 97, 97, 0, 0, 0, 0, _
                0, 0, 0, 96, 99, 99, 98, 100, 98, 99, 99, 96, 0, 0, 0, 0, _
                0, 0, 0, 96, 96, 96, 96, 100, 96, 96, 96, 96, 0, 0, 0, 0, _
                0, 0, 0, 95, 96, 99, 96, 100, 96, 99, 96, 95, 0, 0, 0, 0, _
                0, 0, 0, 96, 96, 96, 96, 96, 96, 96, 96, 96, 0, 0, 0, 0, _
                0, 0, 0, 97, 96, 100, 99, 101, 99, 100, 96, 97, 0, 0, 0, 0, _
                0, 0, 0, 96, 97, 98, 98, 98, 98, 98, 97, 96, 0, 0, 0, 0, _
                0, 0, 0, 96, 96, 97, 99, 99, 99, 97, 96, 96, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        SetIntArray2(PIECE_VALUE, 6, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 9, 9, 9, 11, 13, 11, 9, 9, 9, 0, 0, 0, 0, _
                0, 0, 0, 19, 24, 34, 42, 44, 42, 34, 24, 19, 0, 0, 0, 0, _
                0, 0, 0, 19, 24, 32, 37, 37, 37, 32, 24, 19, 0, 0, 0, 0, _
                0, 0, 0, 19, 23, 27, 29, 30, 29, 27, 23, 19, 0, 0, 0, 0, _
                0, 0, 0, 14, 18, 20, 27, 29, 27, 20, 18, 14, 0, 0, 0, 0, _
                0, 0, 0, 7, 0, 13, 0, 16, 0, 13, 0, 7, 0, 0, 0, 0, _
                0, 0, 0, 7, 0, 7, 0, 15, 0, 7, 0, 7, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 11, 15, 11, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    End Sub

    Public Sub InitBoardHandicap()
        SetStrArray(BOARD_HANDICAP, _
                "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1", _
                "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/R1BAKABNR w - - 0 1", _
                "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/R1BAKAB1R w - - 0 1", _
                "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/9/1C5C1/9/RN2K2NR w - - 0 1")
    End Sub

    Public Class RC4Struct
        Private s(255) As Integer
        Private x As Integer, y As Integer

        Private Sub Swap(ByVal i As Integer, ByVal j As Integer)
            Dim t As Integer
            t = s(i)
            s(i) = s(j)
            s(j) = t
        End Sub

        Public Sub Init(ByVal nKey() As Integer)
            Dim i As Integer, j As Integer, nKeyLen As Integer
            x = 0
            y = 0
            For i = 0 To 255
                s(i) = CByte(i)
            Next
            j = 0
            nKeyLen = nKey.Length
            For i = 0 To 255
                j = (j + s(i) + nKey(i Mod nKeyLen)) And 255
                Swap(i, j)
            Next
        End Sub

        Public Sub InitZero()
            Dim nKey(0) As Integer
            nKey(0) = 0
            Init(nKey)
        End Sub

        Public Function NextByte() As Integer
            x = (x + 1) And 255
            y = (y + s(x)) And 255
            Swap(x, y)
            Return s((s(x) + s(y)) And 255)
        End Function

        Public Function NextLong() As Integer
            Dim n0 As Integer, n1 As Integer, n2 As Integer, n3 As Integer
            n0 = NextByte()
            n1 = NextByte()
            n2 = NextByte()
            n3 = NextByte()
            Return n0 + (n1 << 8) + (n2 << 16) + (n3 << 24)
        End Function
    End Class

    Private PreGen_dwZobristKeyPlayer As Integer
    Private PreGen_dwZobristLockPlayer As Integer
    Private PreGen_dwZobristKeyTable(13, 255) As Integer
    Private PreGen_dwZobristLockTable(13, 255) As Integer

    Public Sub InitArrays()
        Dim i As Integer, j As Integer, dwRandSeed As Integer
        Dim rc4 As RC4Struct
        ' 注：由于常量过于庞大可能会导致程序崩溃，所以一些数组只能在函数中初始化
        InitBoardFort()
        InitLegalSpan()
        InitPieceValue1()
        InitPieceValue2()
        InitBoardHandicap()
        ' 初始化Zobrist
        rc4 = New RC4Struct
        rc4.InitZero()
        PreGen_dwZobristKeyPlayer = rc4.NextLong
        rc4.NextLong()
        PreGen_dwZobristLockPlayer = rc4.NextLong
        For i = 0 To 13
            For j = 0 To 255
                PreGen_dwZobristKeyTable(i, j) = rc4.NextLong
                rc4.NextLong()
                PreGen_dwZobristLockTable(i, j) = rc4.NextLong
            Next
        Next
    End Sub

    Public Structure BookStruct
        Public dwZobristLock As Integer
        Public mv As Short
        Public vl As Short
    End Structure

    Public Class BookComparer
        Implements IComparer

        Public Shared Instance As New BookComparer

        Public Overridable Function Compare(ByVal x As Object, ByVal y As Object) As Integer Implements IComparer.Compare
            Return CType(x, BookStruct).dwZobristLock - CType(y, BookStruct).dwZobristLock
        End Function
    End Class

    Public Book_nMoveNum As Integer
    Public Book_Table() As BookStruct

    Public Sub LoadBook()
        Dim szFile As String, nSlash As Integer, nBackSlash As Integer, fp As FileStream
        Dim i As Integer, Buffer(7) As Byte, BookRecord As BookStruct

        szFile = App_szPath + "BOOK.DAT"
        If File.Exists(szFile) Then
            fp = File.Open(szFile, FileMode.Open, FileAccess.Read)
            Book_nMoveNum = CInt(fp.Length \ 8)
            ReDim Book_Table(Book_nMoveNum - 1)
            For i = 0 To Book_nMoveNum - 1
                fp.Read(Buffer, 0, 8)
                BookRecord.dwZobristLock = ((CInt(Buffer(0)) + (CInt(Buffer(1)) << 8) + (CInt(Buffer(2)) << 16) + (CInt(Buffer(3)) << 24)) >> 1) And &H7FFFFFFF
                BookRecord.mv = CShort(Buffer(4)) + (CShort(Buffer(5)) << 8)
                BookRecord.vl = CShort(Buffer(6)) + (CShort(Buffer(7)) << 8)
                Book_Table(i) = BookRecord
            Next
            fp.Close()
        Else
            Book_nMoveNum = 0
        End If
    End Sub

    Public Structure MoveStruct
        Public vl As Integer, mv As Integer
    End Structure

    Public Class MoveComparer
        Implements IComparer

        Public Shared Instance As New MoveComparer

        Public Overridable Function Compare(ByVal x As Object, ByVal y As Object) As Integer Implements IComparer.Compare
            Return CType(y, MoveStruct).vl - CType(x, MoveStruct).vl
        End Function
    End Class

    Public Structure MoveStructEx
        Public vl As Integer, mv As Integer, pcCaptured As Integer
        Public bCheck As Boolean
    End Structure

    Public Function RANK_Y(ByVal sq As Integer) As Integer
        Return sq >> 4
    End Function

    Public Function FILE_X(ByVal sq As Integer) As Integer
        Return sq And 15
    End Function

    Public Function COORD_XY(ByVal x As Integer, ByVal y As Integer) As Integer
        Return x + (y << 4)
    End Function

    Public Function SQUARE_FLIP(ByVal sq As Integer) As Integer
        Return 254 - sq
    End Function

    Public Function FILE_FLIP(ByVal x As Integer) As Integer
        Return 14 - x
    End Function

    Public Function RANK_FLIP(ByVal y As Integer) As Integer
        Return 15 - y
    End Function

    Public Function MIRROR_SQUARE(ByVal sq As Integer) As Integer
        Return COORD_XY(FILE_FLIP(FILE_X(sq)), RANK_Y(sq))
    End Function

    Public Function SQUARE_FORWARD(ByVal sq As Integer, ByVal sd As Integer) As Integer
        Return sq - 16 + (sd << 5)
    End Function

    Public Function SQUARE_BACKWARD(ByVal sq As Integer, ByVal sd As Integer) As Integer
        Return sq + 16 - (sd << 5)
    End Function

    Public Function KING_SPAN(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return LEGAL_SPAN(sqDst - sqSrc + 256) = 1
    End Function

    Public Function ADVISOR_SPAN(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return LEGAL_SPAN(sqDst - sqSrc + 256) = 2
    End Function

    Public Function BISHOP_SPAN(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return LEGAL_SPAN(sqDst - sqSrc + 256) = 3
    End Function

    Public Function BISHOP_PIN(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Integer
        Return (sqSrc + sqDst) >> 1
    End Function

    Public Function KNIGHT_PIN(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Integer
        Return sqSrc + KNIGHT_PIN_TAB(sqDst - sqSrc + 256)
    End Function

    Public Function HOME_HALF(ByVal sq As Integer, ByVal sd As Integer) As Boolean
        Return (sq And &H80) <> (sd << 7)
    End Function

    Public Function AWAY_HALF(ByVal sq As Integer, ByVal sd As Integer) As Boolean
        Return (sq And &H80) = (sd << 7)
    End Function

    Public Function SAME_HALF(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return ((sqSrc Xor sqDst) And &H80) = 0
    End Function

    Public Function DIFF_HALF(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return ((sqSrc Xor sqDst) And &H80) <> 0
    End Function

    Public Function SAME_RANK(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return ((sqSrc Xor sqDst) And &HF0) = 0
    End Function

    Public Function SAME_FILE(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Boolean
        Return ((sqSrc Xor sqDst) And &HF) = 0
    End Function

    Public Function SIDE_TAG(ByVal sd As Integer) As Integer
        Return 8 + (sd << 3)
    End Function

    Public Function OPP_SIDE_TAG(ByVal sd As Integer) As Integer
        Return 16 - (sd << 3)
    End Function

    Public Function SRC(ByVal mv As Integer) As Integer
        Return mv And 255
    End Function

    Public Function DST(ByVal mv As Integer) As Integer
        Return mv >> 8
    End Function

    Public Function MOVE(ByVal sqSrc As Integer, ByVal sqDst As Integer) As Integer
        Return sqSrc + (sqDst << 8)
    End Function

    Public Function MIRROR_MOVE(ByVal mv As Integer) As Integer
        Return MOVE(MIRROR_SQUARE(SRC(mv)), MIRROR_SQUARE(DST(mv)))
    End Function

    Public Function MVV_LVA(ByVal pc As Integer, ByVal lva As Integer) As Integer
        Return MVV_VALUE(pc And 7) - lva
    End Function

    Public Structure PositionStruct
        Public sdPlayer As Integer
        Public pcSquares() As Integer
        Public dwZobristKey As Integer, dwZobristLock As Integer
        Public vlWhite As Integer, vlBlack As Integer
        Public nMoveNum As Integer, nDistance As Integer
        Public mvsList() As MoveStructEx

        Public Sub ClearBoard()
            Dim sq As Integer
            ReDim pcSquares(255)
            ReDim mvsList(MAX_MOVE_NUM - 1)
            sdPlayer = 0
            dwZobristKey = 0
            dwZobristLock = 0
            For sq = 0 To 255
                pcSquares(sq) = 0
            Next
            vlWhite = 0
            vlBlack = 0
        End Sub

        Public Sub SetIrrev()
            mvsList(0).mv = 0
            mvsList(0).pcCaptured = 0
            mvsList(0).bCheck = Checked(sdPlayer)
            nMoveNum = 1
            nDistance = 0
        End Sub

        Public Sub AddPiece(ByVal sq As Integer, ByVal pc As Integer, Optional ByVal bDel As Boolean = False)
            Dim pcAdjust As Integer
            pcSquares(sq) = IIF_INT(bDel, 0, pc)
            If pc < 16 Then
                pcAdjust = pc - 9
                vlWhite += IIF_INT(bDel, -PIECE_VALUE(pcAdjust, sq), PIECE_VALUE(pcAdjust, sq))
            Else
                pcAdjust = pc - 17
                vlBlack += IIF_INT(bDel, -PIECE_VALUE(pcAdjust, SQUARE_FLIP(sq)), PIECE_VALUE(pcAdjust, SQUARE_FLIP(sq)))
                pcAdjust += 7
            End If
            dwZobristKey = dwZobristKey Xor PreGen_dwZobristKeyTable(pcAdjust, sq)
            dwZobristLock = dwZobristLock Xor PreGen_dwZobristLockTable(pcAdjust, sq)
        End Sub

        Public Function MovePiece(ByVal mv As Integer) As Integer
            Dim sqSrc As Integer, sqDst As Integer, pc As Integer, pcCaptured As Integer
            sqSrc = SRC(mv)
            sqDst = DST(mv)
            pcCaptured = pcSquares(sqDst)
            If pcCaptured > 0 Then
                AddPiece(sqDst, pcCaptured, PIECE_DEL)
            End If
            pc = pcSquares(sqSrc)
            AddPiece(sqSrc, pc, PIECE_DEL)
            AddPiece(sqDst, pc)
            Return pcCaptured
        End Function

        Public Sub UndoMovePiece(ByVal mv As Integer, ByVal pcCaptured As Integer)
            Dim sqSrc As Integer, sqDst As Integer, pc As Integer
            sqSrc = SRC(mv)
            sqDst = DST(mv)
            pc = pcSquares(sqDst)
            AddPiece(sqDst, pc, PIECE_DEL)
            AddPiece(sqSrc, pc)
            If pcCaptured > 0 Then
                AddPiece(sqDst, pcCaptured)
            End If
        End Sub

        Public Function MakeMove(ByVal mv As Integer) As Boolean
            Dim pcCaptured As Integer
            mvsList(nMoveNum).vl = dwZobristKey
            pcCaptured = MovePiece(mv)
            If Checked(sdPlayer) Then
                UndoMovePiece(mv, pcCaptured)
                Return False
            End If
            ChangeSide()
            mvsList(nMoveNum).mv = mv
            mvsList(nMoveNum).pcCaptured = pcCaptured
            mvsList(nMoveNum).bCheck = Checked(sdPlayer)
            nMoveNum += 1
            nDistance += 1
            Return True
        End Function

        Public Sub UndoMakeMove()
            nMoveNum -= 1
            nDistance -= 1
            ChangeSide()
            UndoMovePiece(mvsList(nMoveNum).mv, mvsList(nMoveNum).pcCaptured)
        End Sub

        Public Sub NullMove()
            mvsList(nMoveNum).vl = dwZobristKey
            ChangeSide()
            mvsList(nMoveNum).mv = 0
            mvsList(nMoveNum).pcCaptured = 0
            mvsList(nMoveNum).bCheck = False
            nMoveNum += 1
            nDistance += 1
        End Sub

        Public Sub UndoNullMove()
            nMoveNum -= 1
            nDistance -= 1
            ChangeSide()
        End Sub

        Public Sub ChangeSide()
            sdPlayer = 1 - sdPlayer
            dwZobristKey = dwZobristKey Xor PreGen_dwZobristKeyPlayer
            dwZobristLock = dwZobristLock Xor PreGen_dwZobristLockPlayer
        End Sub

        Public Function FenPiece(ByVal c As Char) As Integer
            Select Case c
                Case CChar("K")
                    Return 1
                Case CChar("A")
                    Return 2
                Case CChar("B"), CChar("E")
                    Return 3
                Case CChar("H"), CChar("N")
                    Return 4
                Case CChar("R")
                    Return 5
                Case CChar("C")
                    Return 6
                Case CChar("P")
                    Return 7
                Case Else
                    Return 0
            End Select
        End Function

        Public Sub FromFen(ByVal szFen As String)
            Dim y As Integer, x As Integer, nIndex As Integer, k As Integer
            Dim c As Char
            ClearBoard()
            y = RANK_TOP
            x = FILE_LEFT
            ' 假定"szFen"不是空的
            nIndex = 0
            c = szFen.Chars(nIndex)
            ' 假定"szFen"中必定有空格
            While c <> CChar(" ")
                If c = CChar("/") Then
                    x = FILE_LEFT
                    y += 1
                    If y > RANK_BOTTOM Then
                        Exit While
                    End If
                ElseIf c >= CChar("1") And c <= CChar("9") Then
                    For k = 1 To Asc(c.ToString) - Asc("0")
                        If x >= FILE_RIGHT Then
                            Exit For
                        End If
                        x += 1
                    Next
                ElseIf c >= CChar("A") And c <= CChar("Z") Then
                    If x <= FILE_RIGHT Then
                        AddPiece(COORD_XY(x, y), FenPiece(c) + 8)
                        x += 1
                    End If
                ElseIf c >= CChar("a") And c <= CChar("z") Then
                    If x <= FILE_RIGHT Then
                        AddPiece(COORD_XY(x, y), FenPiece(Char.ToUpper(c)) + 16)
                        x += 1
                    End If
                End If
                nIndex += 1
                c = szFen.Chars(nIndex)
            End While
            ' 假定空格之后一定还有字符
            nIndex += 1
            If sdPlayer = IIF_INT(szFen.Chars(nIndex) = CChar("b"), 0, 1) Then
                ChangeSide()
            End If
            SetIrrev()
        End Sub

        Public Function GenerateMoves(ByVal mvs() As MoveStruct, Optional ByVal bCapOnly As Boolean = False) As Integer
            Dim sqSrc As Integer, sqDst As Integer, pcSrc As Integer, pcDst As Integer
            Dim i As Integer, nDelta As Integer, nMoves As Integer
            Dim pcSelfSide As Integer, pcOppSide As Integer
            nMoves = 0
            pcSelfSide = SIDE_TAG(sdPlayer)
            pcOppSide = OPP_SIDE_TAG(sdPlayer)
            For sqSrc = 0 To 255
                pcSrc = pcSquares(sqSrc)
                If (pcSrc And pcSelfSide) <> 0 Then
                    Select Case pcSrc - pcSelfSide
                        Case PIECE_KING
                            For i = 0 To 3
                                sqDst = sqSrc + KING_DELTA(i)
                                If IN_FORT(sqDst) Then
                                    pcDst = pcSquares(sqDst)
                                    If bCapOnly Then
                                        If (pcDst And pcOppSide) <> 0 Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            mvs(nMoves).vl = MVV_LVA(pcDst, 5)
                                            nMoves += 1
                                        End If
                                    ElseIf (pcDst And pcSelfSide) = 0 Then
                                        mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                        nMoves += 1
                                    End If
                                End If
                            Next
                        Case PIECE_ADVISOR
                            For i = 0 To 3
                                sqDst = sqSrc + ADVISOR_DELTA(i)
                                If IN_FORT(sqDst) Then
                                    pcDst = pcSquares(sqDst)
                                    If bCapOnly Then
                                        If (pcDst And pcOppSide) <> 0 Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            mvs(nMoves).vl = MVV_LVA(pcDst, 1)
                                            nMoves += 1
                                        End If
                                    ElseIf (pcDst And pcSelfSide) = 0 Then
                                        mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                        nMoves += 1
                                    End If
                                End If
                            Next
                        Case PIECE_BISHOP
                            For i = 0 To 3
                                sqDst = sqSrc + ADVISOR_DELTA(i)
                                If IN_BOARD(sqDst) AndAlso HOME_HALF(sqDst, sdPlayer) AndAlso pcSquares(sqDst) = 0 Then
                                    sqDst += ADVISOR_DELTA(i)
                                    pcDst = pcSquares(sqDst)
                                    If bCapOnly Then
                                        If (pcDst And pcOppSide) <> 0 Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            mvs(nMoves).vl = MVV_LVA(pcDst, 1)
                                            nMoves += 1
                                        End If
                                    ElseIf (pcDst And pcSelfSide) = 0 Then
                                        mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                        nMoves += 1
                                    End If
                                End If
                            Next
                        Case PIECE_KNIGHT
                            For i = 0 To 3
                                sqDst = sqSrc + KING_DELTA(i)
                                If pcSquares(sqDst) = 0 Then
                                    For nDelta = 0 To 1
                                        sqDst = sqSrc + KNIGHT_DELTA(i, nDelta)
                                        If IN_BOARD(sqDst) Then
                                            pcDst = pcSquares(sqDst)
                                            If bCapOnly Then
                                                If (pcDst And pcOppSide) <> 0 Then
                                                    mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                                    mvs(nMoves).vl = MVV_LVA(pcDst, 3)
                                                    nMoves += 1
                                                End If
                                            ElseIf (pcDst And pcSelfSide) = 0 Then
                                                mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                                nMoves += 1
                                            End If
                                        End If
                                    Next
                                End If
                            Next
                        Case PIECE_ROOK
                            For i = 0 To 3
                                nDelta = KING_DELTA(i)
                                sqDst = sqSrc + nDelta
                                While IN_BOARD(sqDst)
                                    pcDst = pcSquares(sqDst)
                                    If pcDst = 0 Then
                                        If Not bCapOnly Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            nMoves += 1
                                        End If
                                    Else
                                        If (pcDst And pcOppSide) <> 0 Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            mvs(nMoves).vl = MVV_LVA(pcDst, 4)
                                            nMoves += 1
                                        End If
                                        Exit While
                                    End If
                                    sqDst += nDelta
                                End While
                            Next
                        Case PIECE_CANNON
                            For i = 0 To 3
                                nDelta = KING_DELTA(i)
                                sqDst = sqSrc + nDelta
                                While IN_BOARD(sqDst)
                                    pcDst = pcSquares(sqDst)
                                    If pcDst = 0 Then
                                        If Not bCapOnly Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            nMoves += 1
                                        End If
                                    Else
                                        Exit While
                                    End If
                                    sqDst += nDelta
                                End While
                                sqDst += nDelta
                                While IN_BOARD(sqDst)
                                    pcDst = pcSquares(sqDst)
                                    If pcDst > 0 Then
                                        If (pcDst And pcOppSide) <> 0 Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            mvs(nMoves).vl = MVV_LVA(pcDst, 3)
                                            nMoves += 1
                                        End If
                                        Exit While
                                    End If
                                    sqDst += nDelta
                                End While
                            Next
                        Case PIECE_PAWN
                            sqDst = sqSrc + (sdPlayer << 5) - 16
                            If IN_BOARD(sqDst) Then
                                pcDst = pcSquares(sqDst)
                                If bCapOnly Then
                                    If (pcDst And pcOppSide) <> 0 Then
                                        mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                        mvs(nMoves).vl = MVV_LVA(pcDst, 2)
                                        nMoves += 1
                                    End If
                                ElseIf (pcDst And pcSelfSide) = 0 Then
                                    mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                    nMoves += 1
                                End If
                            End If
                            If AWAY_HALF(sqSrc, sdPlayer) Then
                                For nDelta = -1 To 1 Step 2
                                    sqDst = sqSrc + nDelta
                                    If IN_BOARD(sqDst) Then
                                        pcDst = pcSquares(sqDst)
                                        If bCapOnly Then
                                            If (pcDst And pcOppSide) <> 0 Then
                                                mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                                mvs(nMoves).vl = MVV_LVA(pcDst, 2)
                                                nMoves += 1
                                            End If
                                        ElseIf (pcDst And pcSelfSide) = 0 Then
                                            mvs(nMoves).mv = MOVE(sqSrc, sqDst)
                                            nMoves += 1
                                        End If
                                    End If
                                Next
                            End If
                    End Select
                End If
            Next
            Return nMoves
        End Function

        Public Function LegalMove(ByVal mv As Integer) As Boolean
            Dim sqSrc As Integer, sqDst As Integer, pcSrc As Integer, pcDst As Integer, sqPin As Integer
            Dim pcSelfSide As Integer, i As Integer, nDelta As Integer

            sqSrc = SRC(mv)
            pcSrc = pcSquares(sqSrc)
            pcSelfSide = SIDE_TAG(sdPlayer)
            If (pcSrc And pcSelfSide) = 0 Then
                Return False
            End If

            sqDst = DST(mv)
            pcDst = pcSquares(sqDst)
            If (pcDst And pcSelfSide) <> 0 Then
                Return False
            End If

            Select Case pcSrc - pcSelfSide
                Case PIECE_KING
                    Return IN_FORT(sqDst) AndAlso KING_SPAN(sqSrc, sqDst)
                Case PIECE_ADVISOR
                    Return IN_FORT(sqDst) AndAlso ADVISOR_SPAN(sqSrc, sqDst)
                Case PIECE_BISHOP
                    Return SAME_HALF(sqSrc, sqDst) AndAlso BISHOP_SPAN(sqSrc, sqDst) AndAlso pcSquares(BISHOP_PIN(sqSrc, sqDst)) = 0
                Case PIECE_KNIGHT
                    sqPin = KNIGHT_PIN(sqSrc, sqDst)
                    Return sqPin <> sqSrc AndAlso pcSquares(sqPin) = 0
                Case PIECE_ROOK, PIECE_CANNON
                    If ((sqSrc Xor sqDst) And &HF) = 0 Then
                        nDelta = IIF_INT(sqDst < sqSrc, -16, 16)
                    ElseIf ((sqSrc Xor sqDst) And &HF0) = 0 Then
                        nDelta = IIF_INT(sqDst < sqSrc, -1, 1)
                    Else
                        Return False
                    End If
                    sqPin = sqSrc + nDelta
                    While sqPin <> sqDst AndAlso pcSquares(sqPin) = 0
                        sqPin += nDelta
                    End While
                    If sqPin = sqDst Then
                        Return pcDst = 0 OrElse pcSrc - pcSelfSide = PIECE_ROOK
                    ElseIf pcDst > 0 AndAlso pcSrc - pcSelfSide = PIECE_CANNON Then
                        sqPin += nDelta
                        While sqPin <> sqDst AndAlso pcSquares(sqPin) = 0
                            sqPin += nDelta
                        End While
                        Return sqPin = sqDst
                    Else
                        Return False
                    End If
                Case PIECE_PAWN
                    If AWAY_HALF(sqDst, sdPlayer) AndAlso (sqDst = sqSrc + 1 OrElse sqDst = sqSrc - 1) Then
                        Return True
                    Else
                        Return sqDst = SQUARE_FORWARD(sqSrc, sdPlayer)
                    End If
                Case Else
                    Return False
            End Select
        End Function

        Public Function Checked(ByVal sd As Integer) As Boolean
            Dim sqSrc As Integer, sqDst As Integer, pcSrc As Integer, pcDst As Integer
            Dim pcSelfSide As Integer, pcOppSide, i As Integer, nDelta As Integer
            pcSelfSide = SIDE_TAG(sd)
            pcOppSide = OPP_SIDE_TAG(sd)
            For sqSrc = 0 To 255
                If pcSquares(sqSrc) = pcSelfSide + PIECE_KING Then
                    If pcSquares(SQUARE_FORWARD(sqSrc, sd)) = pcOppSide + PIECE_PAWN Then
                        Return True
                    End If
                    For nDelta = -1 To 1 Step 2
                        If pcSquares(sqSrc + nDelta) = pcOppSide + PIECE_PAWN Then
                            Return True
                        End If
                    Next
                    For i = 0 To 3
                        If pcSquares(sqSrc + ADVISOR_DELTA(i)) = 0 Then
                            For nDelta = 0 To 1
                                pcDst = pcSquares(sqSrc + KNIGHT_CHECK_DELTA(i, nDelta))
                                If pcDst = pcOppSide + PIECE_KNIGHT Then
                                    Return True
                                End If
                            Next
                        End If
                    Next
                    For i = 0 To 3
                        nDelta = KING_DELTA(i)
                        sqDst = sqSrc + nDelta
                        While IN_BOARD(sqDst)
                            pcDst = pcSquares(sqDst)
                            If pcDst > 0 Then
                                If pcDst = pcOppSide + PIECE_ROOK OrElse pcDst = pcOppSide + PIECE_KING Then
                                    Return True
                                End If
                                Exit While
                            End If
                            sqDst += nDelta
                        End While
                        sqDst += nDelta
                        While IN_BOARD(sqDst)
                            pcDst = pcSquares(sqDst)
                            If pcDst > 0 Then
                                If pcDst = pcOppSide + PIECE_CANNON Then
                                    Return True
                                End If
                                Exit While
                            End If
                            sqDst += nDelta
                        End While
                    Next
                End If
            Next
            Return False
        End Function

        Public Function IsMate() As Boolean
            Dim mvs(MAX_GEN_MOVES - 1) As MoveStruct
            Dim i As Integer, nMoves As Integer
            nMoves = GenerateMoves(mvs)
            For i = 0 To nMoves - 1
                If MakeMove(mvs(i).mv) Then
                    UndoMakeMove()
                    Return False
                End If
            Next
            Return True
        End Function

        Public Function MateValue() As Integer
            Return nDistance - MATE_VALUE
        End Function

        Public Function BanValue() As Integer
            Return nDistance - BAN_VALUE
        End Function

        Public Function Evaluate() As Integer
            Dim vl As Integer
            vl = IIF_INT(sdPlayer = 0, vlWhite - vlBlack, vlBlack - vlWhite) + ADVANCED_VALUE
            Return IIF_INT(vl = DrawValue(), vl - 1, vl)
        End Function

        Public Function DrawValue() As Integer
            Return IIF_INT((nDistance And 1) = 0, -DRAW_VALUE, DRAW_VALUE)
        End Function

        Public Function NullOkay() As Boolean
            Return IIF_INT(sdPlayer = 0, vlWhite, vlBlack) > NULL_OKAY_MARGIN
        End Function

        Public Function NullSafe() As Boolean
            Return IIF_INT(sdPlayer = 0, vlWhite, vlBlack) > NULL_SAFE_MARGIN
        End Function

        Public Function InCheck() As Boolean
            Return mvsList(nMoveNum - 1).bCheck
        End Function

        Public Function Captured() As Boolean
            Return mvsList(nMoveNum - 1).pcCaptured > 0
        End Function

        Public Function RepValue(ByVal vlRep As Integer) As Integer
            Dim vlReturn As Integer
            vlReturn = IIF_INT((vlRep And 2) = 0, 0, BanValue) + IIF_INT((vlRep And 4) = 0, 0, -BanValue())
            Return IIF_INT(vlReturn = 0, DrawValue, vlReturn)
        End Function

        Public Function RepStatus(Optional ByVal nRecur As Integer = 1) As Integer
            Dim nIndex As Integer, vl As Integer
            Dim bSelfSide As Boolean, bPerpCheck As Boolean, bOppPerpCheck As Boolean
            ' 1. 初始化
            bSelfSide = False
            bPerpCheck = True
            bOppPerpCheck = True
            nIndex = nMoveNum - 1
            ' 2. 有空着或者吃子就中断
            While mvsList(nIndex).mv > 0 AndAlso mvsList(nIndex).pcCaptured = 0
                ' 3. 设置长将标志
                If bSelfSide Then
                    bPerpCheck = bPerpCheck AndAlso mvsList(nIndex).bCheck
                    ' 4. 检查循环
                    If mvsList(nIndex).vl = dwZobristKey Then
                        nRecur -= 1
                        If nRecur = 0 Then
                            Return 1 + IIF_INT(bPerpCheck, 2, 0) + IIF_INT(bOppPerpCheck, 4, 0)
                        End If
                    End If
                Else
                    bOppPerpCheck = bOppPerpCheck AndAlso mvsList(nIndex).bCheck
                End If
                bSelfSide = Not bSelfSide
                nIndex -= 1
            End While
            Return 0
        End Function

        Public Function Mirror() As PositionStruct
            Dim pos As PositionStruct
            Dim sq As Integer, pc As Integer
            pos.ClearBoard()
            For sq = 0 To 255
                pc = pcSquares(sq)
                If pc > 0 Then
                    pos.AddPiece(MIRROR_SQUARE(sq), pc)
                End If
            Next
            If sdPlayer = 1 Then
                pos.ChangeSide()
            End If
            Return pos
        End Function

        Public Function BookMove() As Integer
            Dim pos As PositionStruct, bMirror As Boolean
            Dim bk As BookStruct, nIndex As Integer, i As Integer
            Dim mv As Integer, mvs(MAX_GEN_MOVES - 1) As MoveStruct
            Dim nValue As Integer, nMoves As Integer

            If Book_nMoveNum = 0 Then
                Return 0
            End If
            ' 1. 搜索开局库表
            bMirror = False
            bk.dwZobristLock = (dwZobristLock >> 1) And &H7FFFFFFF
            nIndex = Array.BinarySearch(Book_Table, 0, Book_nMoveNum, bk, BookComparer.Instance)
            If nIndex < 0 Then
                bMirror = True
                pos = Mirror()
                bk.dwZobristLock = (pos.dwZobristLock >> 1) And &H7FFFFFFF
                nIndex = Array.BinarySearch(Book_Table, 0, Book_nMoveNum, bk, BookComparer.Instance)
            End If
            If nIndex < 0 Then
                Return 0
            End If
            nIndex -= 1
            While nIndex >= 0 AndAlso Book_Table(nIndex).dwZobristLock = bk.dwZobristLock
                nIndex -= 1
            End While
            ' 2. 获得着法列表
            nValue = 0
            nMoves = 0
            nIndex += 1
            While nIndex < Book_nMoveNum AndAlso Book_Table(nIndex).dwZobristLock = bk.dwZobristLock
                mv = Book_Table(nIndex).mv And &HFFFF
                mv = IIF_INT(bMirror, MIRROR_MOVE(mv), mv)
                If LegalMove(mv) Then
                    mvs(nMoves).mv = mv
                    mvs(nMoves).vl = Book_Table(nIndex).vl
                    nValue += Book_Table(nIndex).vl
                    nMoves += 1
                    If nMoves = MAX_GEN_MOVES Then
                        Exit While
                    End If
                End If
                nIndex += 1
            End While
            ' 3. 从着法列表中选择着法
            If nValue = 0 Then
                Return 0
            End If
            nValue = CInt(Int(Rnd() * nValue))
            For nIndex = 0 To nMoves - 1
                nValue -= mvs(nIndex).vl
                If nValue < 0 Then
                    Exit For
                End If
            Next
            Return mvs(nIndex).mv
        End Function

        Public Function HistoryIndex(ByVal mv As Integer) As Integer
            Return ((pcSquares(SRC(mv)) - 8) << 8) + DST(mv)
        End Function
    End Structure
End Module