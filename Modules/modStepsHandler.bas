Attribute VB_Name = "modStepsHandler"
''' modStepsHandler
''' Helps deal with the development steps.
'''
''' Author: Nathan Campos <nathan@innoveworkshop.com>

Option Explicit

' Color constants.
Private Const clrAgitate = &HFF

' Controlled controls.
Private grdSteps As GridCtrl

' Initializes the steps handler.
Public Sub InitializeStepsHandler(ByRef grdCtrl As GridCtrl)
    Set grdSteps = grdCtrl
End Sub

' Add a new step to the list.
Public Sub AddStepToQueue(intSeconds As Integer, strChemical As String, _
                          bAgitate As Boolean)
    Dim strItem As String
    
    ' Build up the row item string.
    strItem = SecondsToTimeStr(intSeconds) & vbTab
    strItem = strItem & strChemical & vbTab
    If bAgitate Then
        strItem = strItem & "Agitate"
    Else
        strItem = strItem & "Stand"
    End If
    
    ' Add the item to the list.
    grdSteps.AddItem strItem
    
    ' Colorize the agitation cell if needed.
    grdSteps.Row = grdSteps.Rows - 1
    grdSteps.Col = 2
    If bAgitate Then
        grdSteps.CellForeColor = clrAgitate
    End If
End Sub

' Converts a duration in seconds to a time string
Public Function SecondsToTimeStr(intDuration As Integer)
    Dim intMinutes As Integer
    Dim intSeconds As Integer
    Dim strTime As String
    
    ' Split the minutes from the seconds
    intSeconds = intDuration Mod 60
    intMinutes = (intDuration - intSeconds) / 60
    
    ' Ensure we have leading zeros for the minutes in the string.
    strTime = CStr(intMinutes) & ":"
    If intMinutes < 10 Then
        strTime = "0" & strTime
    End If
    If intSeconds < 10 Then
        strTime = strTime & "0"
    End If
    strTime = strTime & CStr(intSeconds)
    
    SecondsToTimeStr = strTime
End Function
