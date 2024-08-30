object Form2: TForm2
  Left = 241
  Top = 178
  Width = 1200
  Height = 631
  Caption = 'Loan Amortization Schedule'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = ClearFormData
  OnShow = createreport
  DesignSize = (
    1184
    573)
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 1
    Top = 1
    Width = 1184
    Height = 568
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 8
    object SaveAs1: TMenuItem
      Caption = 'File'
      object SaveAs2: TMenuItem
        Caption = 'Save As'
        OnClick = btnSaveToFileClick
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 40
    Top = 8
  end
end
