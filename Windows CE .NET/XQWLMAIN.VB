' XQWLMAIN.VB - Source Code for XiangQi Wizard Light, Part IV
'
' XiangQi Wizard Light - a Chinese Chess Program for Windows CE
' Designed by Morning Yellow, Version: 1.13, Last Modified: Dec. 2007
' Copyright (C) 2004-2007 www.elephantbase.net
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

Public Class MainForm
    Inherits System.Windows.Forms.Form

#Region " Windows 窗体设计器生成的代码 "

    Public Sub New()

        MyBase.New()

        '该调用是 Windows 窗体设计器所必需的。
        InitializeComponent()
        '在 InitializeComponent() 调用之后添加任何初始化
        InitializeImages()

    End Sub

    '窗体重写 dispose 以清理组件列表。
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        MyBase.Dispose(disposing)
    End Sub

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改此过程。
    '不要使用代码编辑器修改它。
    Friend WithEvents ilBoards As System.Windows.Forms.ImageList
    Friend WithEvents ilBoardsSelected As System.Windows.Forms.ImageList
    Friend WithEvents imgThinking As System.Windows.Forms.PictureBox
    Friend WithEvents ilPiecesSelected As System.Windows.Forms.ImageList
    Friend WithEvents ilPieces As System.Windows.Forms.ImageList
    Friend WithEvents ilCPieces As System.Windows.Forms.ImageList
    Friend WithEvents ilCPiecesSelected As System.Windows.Forms.ImageList
    Friend WithEvents ilCBoards As System.Windows.Forms.ImageList
    Friend WithEvents ilCBoardsSelected As System.Windows.Forms.ImageList
    Private Sub InitializeComponent()
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(MainForm))
        Me.ilPiecesSelected = New System.Windows.Forms.ImageList
        Me.ilBoards = New System.Windows.Forms.ImageList
        Me.imgThinking = New System.Windows.Forms.PictureBox
        Me.ilBoardsSelected = New System.Windows.Forms.ImageList
        Me.ilPieces = New System.Windows.Forms.ImageList
        Me.ilCPieces = New System.Windows.Forms.ImageList
        Me.ilCPiecesSelected = New System.Windows.Forms.ImageList
        Me.ilCBoards = New System.Windows.Forms.ImageList
        Me.ilCBoardsSelected = New System.Windows.Forms.ImageList
        '
        'ilPiecesSelected
        '
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource1"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource2"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource3"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource4"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource5"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource6"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource7"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource8"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource9"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource10"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource11"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource12"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource13"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource14"), System.Drawing.Image))
        Me.ilPiecesSelected.Images.Add(CType(resources.GetObject("resource15"), System.Drawing.Image))
        Me.ilPiecesSelected.ImageSize = New System.Drawing.Size(16, 16)
        '
        'ilBoards
        '
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource16"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource17"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource18"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource19"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource20"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource21"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource22"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource23"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource24"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource25"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource26"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource27"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource28"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource29"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource30"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource31"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource32"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource33"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource34"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource35"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource36"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource37"), System.Drawing.Image))
        Me.ilBoards.Images.Add(CType(resources.GetObject("resource38"), System.Drawing.Image))
        Me.ilBoards.ImageSize = New System.Drawing.Size(16, 16)
        '
        'imgThinking
        '
        Me.imgThinking.Image = CType(resources.GetObject("imgThinking.Image"), System.Drawing.Image)
        Me.imgThinking.Size = New System.Drawing.Size(32, 32)
        Me.imgThinking.Visible = False
        '
        'ilBoardsSelected
        '
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource39"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource40"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource41"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource42"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource43"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource44"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource45"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource46"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource47"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource48"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource49"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource50"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource51"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource52"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource53"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource54"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource55"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource56"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource57"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource58"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource59"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource60"), System.Drawing.Image))
        Me.ilBoardsSelected.Images.Add(CType(resources.GetObject("resource61"), System.Drawing.Image))
        Me.ilBoardsSelected.ImageSize = New System.Drawing.Size(16, 16)
        '
        'ilPieces
        '
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource62"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource63"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource64"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource65"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource66"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource67"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource68"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource69"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource70"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource71"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource72"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource73"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource74"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource75"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource76"), System.Drawing.Image))
        Me.ilPieces.Images.Add(CType(resources.GetObject("resource77"), System.Drawing.Image))
        Me.ilPieces.ImageSize = New System.Drawing.Size(16, 16)
        '
        'ilCPieces
        '
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource78"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource79"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource80"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource81"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource82"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource83"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource84"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource85"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource86"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource87"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource88"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource89"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource90"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource91"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource92"), System.Drawing.Image))
        Me.ilCPieces.Images.Add(CType(resources.GetObject("resource93"), System.Drawing.Image))
        Me.ilCPieces.ImageSize = New System.Drawing.Size(16, 16)
        '
        'ilCPiecesSelected
        '
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource94"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource95"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource96"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource97"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource98"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource99"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource100"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource101"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource102"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource103"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource104"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource105"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource106"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource107"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource108"), System.Drawing.Image))
        Me.ilCPiecesSelected.Images.Add(CType(resources.GetObject("resource109"), System.Drawing.Image))
        Me.ilCPiecesSelected.ImageSize = New System.Drawing.Size(16, 16)
        '
        'ilCBoards
        '
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource110"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource111"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource112"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource113"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource114"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource115"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource116"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource117"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource118"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource119"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource120"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource121"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource122"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource123"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource124"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource125"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource126"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource127"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource128"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource129"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource130"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource131"), System.Drawing.Image))
        Me.ilCBoards.Images.Add(CType(resources.GetObject("resource132"), System.Drawing.Image))
        Me.ilCBoards.ImageSize = New System.Drawing.Size(16, 16)
        '
        'ilCBoardsSelected
        '
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource133"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource134"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource135"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource136"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource137"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource138"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource139"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource140"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource141"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource142"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource143"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource144"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource145"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource146"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource147"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource148"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource149"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource150"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource151"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource152"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource153"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource154"), System.Drawing.Image))
        Me.ilCBoardsSelected.Images.Add(CType(resources.GetObject("resource155"), System.Drawing.Image))
        Me.ilCBoardsSelected.ImageSize = New System.Drawing.Size(16, 16)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        '
        'MainForm
        '
        Me.BackColor = System.Drawing.Color.White
        Me.ClientSize = New System.Drawing.Size(154, 175)
        Me.Controls.Add(Me.imgThinking)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.Text = "象棋小巫师"

    End Sub
#End Region
    Private Class ImageSquare
        Inherits PictureBox
        Public sq As Integer

        Public Sub New(ByVal sq As Integer)
            Me.sq = sq
        End Sub
    End Class

    Private imgBoards(255) As Image, imgBoardsSelected(255) As Image, imgCBoards(255) As Image, imgCBoardsSelected(255) As Image

    Private Sub SetBoardImage(ByVal sq As Integer, ByVal nImage As Integer)
        imgBoards(sq) = ilBoards.Images(nImage)
        imgBoardsSelected(sq) = ilBoardsSelected.Images(nImage)
        imgCBoards(sq) = ilCBoards.Images(nImage)
        imgCBoardsSelected(sq) = ilCBoardsSelected.Images(nImage)
    End Sub

    Private Sub InitializeImages()
        Dim sq As Integer
        ' Cross
        For sq = 0 To 255
            If IN_BOARD(sq) Then
                SetBoardImage(sq, 0)
            End If
        Next
        ' TopBorder
        For sq = &H34 To &H3A
            SetBoardImage(sq, 1)
        Next
        ' BottomBorder
        For sq = &HC4 To &HCA
            SetBoardImage(sq, 2)
        Next
        ' LeftBorder
        For sq = &H43 To &HB3 Step 16
            SetBoardImage(sq, 3)
        Next
        ' RightBorder
        For sq = &H4B To &HBB Step 16
            SetBoardImage(sq, 4)
        Next
        ' TopLeftCorner
        SetBoardImage(&H33, 5)
        ' TopRightCorner
        SetBoardImage(&H3B, 6)
        ' BottomLeftCorner
        SetBoardImage(&HC3, 7)
        ' BottomRightCorner
        SetBoardImage(&HCB, 8)
        ' TopBank
        For sq = &H74 To &H7A
            SetBoardImage(sq, 9)
        Next
        ' BottomBank
        For sq = &H84 To &H8A
            SetBoardImage(sq, 10)
        Next
        ' Cross
        SetBoardImage(&H54, 11)
        SetBoardImage(&H5A, 11)
        SetBoardImage(&H65, 11)
        SetBoardImage(&H67, 11)
        SetBoardImage(&H69, 11)
        SetBoardImage(&H95, 11)
        SetBoardImage(&H97, 11)
        SetBoardImage(&H99, 11)
        SetBoardImage(&HA4, 11)
        SetBoardImage(&HAA, 11)
        ' LeftBorderCross
        SetBoardImage(&H63, 12)
        SetBoardImage(&H93, 12)
        ' RightBorderCross
        SetBoardImage(&H6B, 13)
        SetBoardImage(&H9B, 13)
        ' TopLeftFortBorder
        SetBoardImage(&H36, 14)
        ' TopRightFortBorder
        SetBoardImage(&H38, 15)
        ' BottomLeftFortBorder
        SetBoardImage(&HC6, 16)
        ' BottomRightFortBorder
        SetBoardImage(&HC8, 17)
        ' TopLeftFort
        SetBoardImage(&HA6, 18)
        ' TopRightFort
        SetBoardImage(&HA8, 19)
        ' BottomLeftFort
        SetBoardImage(&H56, 20)
        ' BottomRightFort
        SetBoardImage(&H58, 21)
        ' CenterFort
        SetBoardImage(&H47, 22)
        SetBoardImage(&HB7, 22)
    End Sub

    Private bClicking As Boolean
    Private imgSquares(255) As ImageSquare

    Private Sub imgSquares_Click(ByVal sender As Object, ByVal e As EventArgs)
        Dim sqLast As Integer
        sqLast = App_sqCursor
        App_sqCursor = CType(sender, ImageSquare).sq
        DrawCursor(sqLast)
        If App_bFlipped Then
            ClickSquare(SQUARE_FLIP(App_sqCursor))
        Else
            ClickSquare(App_sqCursor)
        End If
    End Sub

    Private Sub frmMain_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim x As Integer, y As Integer, sq As Integer
        Dim img As ImageSquare

        Controls.Clear()
        Controls.Add(imgThinking)
        For x = 0 To 15
            For y = 0 To 15
                sq = x + (y << 4)
                If IN_BOARD(sq) Then
                    img = New ImageSquare(sq)
                    img.sq = sq
                    img.Left = (x - FILE_LEFT) * 16 + LEFT_MARGIN
                    img.Top = (y - RANK_TOP) * 16 + TOP_MARGIN
                    img.Width = 16
                    img.Height = 16
                    Controls.Add(img)
                    AddHandler img.Click, AddressOf imgSquares_Click
                    imgSquares(sq) = img
                End If
            Next
        Next
        Search_pos.FromFen(Position.BOARD_HANDICAP(App_nHandicap))
        App_sqCursor = &HAA
        For sq = 0 To 255
            If IN_BOARD(sq) Then
                DrawSquare(sq)
            End If
        Next
        App_sqSelected = 0
        App_mvLast = 0
        bClicking = False
        Show()
        If App_bFlipped Then
            ResponseMove()
        End If
    End Sub

    Private Sub frmMain_Closing(ByVal sender As System.Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        e.Cancel = bClicking
    End Sub

    Private Sub frmMain_KeyDown(ByVal sender As System.Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles MyBase.KeyDown
        Dim sqLast As Integer
        sqLast = App_sqCursor
        Select Case e.KeyCode
            Case Keys.NumPad1
                MoveUp()
                MoveLeft()
                DrawCursor(sqLast)
            Case Keys.NumPad2, Keys.Up
                MoveUp()
                DrawCursor(sqLast)
            Case Keys.NumPad3
                MoveUp()
                MoveRight()
                DrawCursor(sqLast)
            Case Keys.NumPad4, Keys.Left
                MoveLeft()
                DrawCursor(sqLast)
            Case Keys.NumPad5, Keys.Enter
                If App_bFlipped Then
                    ClickSquare(SQUARE_FLIP(App_sqCursor))
                Else
                    ClickSquare(App_sqCursor)
                End If
            Case Keys.NumPad6, Keys.Right
                MoveRight()
                DrawCursor(sqLast)
            Case Keys.NumPad7
                MoveDown()
                MoveLeft()
                DrawCursor(sqLast)
            Case Keys.NumPad8, Keys.Down
                MoveDown()
                DrawCursor(sqLast)
            Case Keys.NumPad9
                MoveDown()
                MoveRight()
                DrawCursor(sqLast)
        End Select
    End Sub

    Private Sub ClickSquare(ByVal sq As Integer)
        Dim sqLast As Integer, pc As Integer

        If bClicking Then
            Exit Sub
        End If
        bClicking = True
        pc = Search_pos.pcSquares(sq)
        If (pc And SIDE_TAG(Search_pos.sdPlayer)) <> 0 Then
            If App_sqSelected > 0 Then
                DrawSquare(App_sqSelected)
            End If
            App_sqSelected = sq
            DrawSquare(App_sqSelected, DRAW_SELECTED)
            If App_mvLast > 0 Then
                DrawSquare(SRC(App_mvLast))
                DrawSquare(DST(App_mvLast))
            End If
            PlaySound("CLICK")
        Else
            If App_sqSelected > 0 AndAlso AddMove(MOVE(App_sqSelected, sq)) AndAlso Not ResponseMove() Then
                bClicking = False
                ' 如果"bClicking = False"在"Close()"之后，则窗口会重新打开！
                Close()
                Exit Sub
            End If
        End If
        bClicking = False
    End Sub

    Private Sub MoveUp()
        App_sqCursor -= 16
        If Not IN_BOARD(App_sqCursor) Then
            App_sqCursor += 160
        End If
    End Sub

    Private Sub MoveDown()
        App_sqCursor += 16
        If Not IN_BOARD(App_sqCursor) Then
            App_sqCursor -= 160
        End If
    End Sub

    Private Sub MoveLeft()
        App_sqCursor -= 1
        If Not IN_BOARD(App_sqCursor) Then
            App_sqCursor += 9
        End If
    End Sub

    Private Sub MoveRight()
        App_sqCursor += 1
        If Not IN_BOARD(App_sqCursor) Then
            App_sqCursor -= 9
        End If
    End Sub

    Private Sub DrawCursor(ByVal sqLast As Integer)
        Dim sq As Integer
        sq = sqLast
        If App_bFlipped Then
            sq = SQUARE_FLIP(sq)
        End If
        DrawSquare(sq, App_sqSelected = sq OrElse SRC(App_mvLast) = sq OrElse DST(App_mvLast) = sq)
        sq = App_sqCursor
        If App_bFlipped Then
            sq = SQUARE_FLIP(sq)
        End If
        DrawSquare(sq, App_sqSelected = sq OrElse SRC(App_mvLast) = sq OrElse DST(App_mvLast) = sq)
    End Sub

    Public Sub DrawSquare(ByVal sq As Integer, Optional ByVal bSelected As Boolean = False)
        Dim sqFlipped As Integer, pc As Integer
        sqFlipped = sq
        If App_bFlipped Then
            sqFlipped = SQUARE_FLIP(sqFlipped)
        End If
        pc = Search_pos.pcSquares(sq)
        If pc = 0 Then
            If bSelected Then
                If sqFlipped = App_sqCursor Then
                    imgSquares(sqFlipped).Image = imgCBoardsSelected(sqFlipped)
                Else
                    imgSquares(sqFlipped).Image = imgBoardsSelected(sqFlipped)
                End If
            Else
                If sqFlipped = App_sqCursor Then
                    imgSquares(sqFlipped).Image = imgCBoards(sqFlipped)
                Else
                    imgSquares(sqFlipped).Image = imgBoards(sqFlipped)
                End If
            End If
        Else
            If bSelected Then
                If sqFlipped = App_sqCursor Then
                    imgSquares(sqFlipped).Image = ilCPiecesSelected.Images(pc - 8)
                Else
                    imgSquares(sqFlipped).Image = ilPiecesSelected.Images(pc - 8)
                End If
            Else
                If sqFlipped = App_sqCursor Then
                    imgSquares(sqFlipped).Image = ilCPieces.Images(pc - 8)
                Else
                    imgSquares(sqFlipped).Image = ilPieces.Images(pc - 8)
                End If
            End If
        End If
    End Sub
End Class