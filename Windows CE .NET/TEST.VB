Imports System.IO

Module Test
    Public Sub Test()
        Dim pos As PositionStruct
        Dim mvList(128) As MoveStruct
        Dim fpRead As StreamReader
        Dim sz As String
        Dim nLegal As Integer, nGened As Integer, nMoved As Integer, nCheck As Integer
        Dim sqSrc As Integer, sqDst As Integer, nMoveNum As Integer, i As Integer

        nLegal = 0
        nGened = 0
        nMoved = 0
        nCheck = 0
        fpRead = New StreamReader(App_szPath + "\TEST\FUNNY.EPD")
        sz = fpRead.ReadLine
        While sz <> vbNullString
            pos.FromFen(sz)
            For sqSrc = 0 To 255
                If IN_BOARD(sqSrc) Then
                    For sqDst = 0 To 255
                        If IN_BOARD(sqDst) Then
                            nLegal += IIF_INT(pos.LegalMove(MOVE(sqSrc, sqDst)), 1, 0)
                        End If
                    Next
                End If
            Next
            nMoveNum = pos.GenerateMoves(mvList)
            For i = 0 To nMoveNum - 1
                If pos.MakeMove(mvList(i).mv) Then
                    nMoved += 1
                    nCheck += IIF_INT(pos.InCheck(), 1, 0)
                    pos.UndoMakeMove()
                End If
            Next
            nGened += nMoveNum
            sz = fpRead.ReadLine
        End While
        fpRead.Close()
        ' Legal: 7809
        ' Gened: 7809
        ' Moved: 7207
        ' Check: 718
        MsgBox("Legal: " + Str(nLegal) + vbCrLf + _
                "Gened: " + Str(nGened) + vbCrLf + _
                "Moved: " + Str(nMoved) + vbCrLf + _
                "Check: " + Str(nCheck))
    End Sub
End Module