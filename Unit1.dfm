object Form1: TForm1
  Left = 1270
  Top = 172
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'LoanCalc'
  ClientHeight = 293
  ClientWidth = 315
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = True
  Position = poDefault
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 96
    Height = 13
    Caption = 'Loan Calculation'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 72
    Top = 42
    Width = 90
    Height = 13
    Caption = '  Amount Borrowed'
  end
  object Label3: TLabel
    Left = 65
    Top = 66
    Width = 97
    Height = 13
    Caption = ' Payment Frequency'
  end
  object Label4: TLabel
    Left = 64
    Top = 90
    Width = 98
    Height = 13
    Caption = 'Number of Payments'
  end
  object Label5: TLabel
    Left = 56
    Top = 114
    Width = 114
    Height = 13
    Caption = 'Annual Interest Rate %  '
  end
  object Label6: TLabel
    Left = 88
    Top = 138
    Width = 74
    Height = 13
    Caption = 'Fees per Period'
  end
  object Label8: TLabel
    Left = 16
    Top = 176
    Width = 135
    Height = 13
    Caption = 'The term of this loan is '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label9: TLabel
    Left = 26
    Top = 202
    Width = 137
    Height = 13
    Caption = ' Payment Amount per Period '
  end
  object Label10: TLabel
    Left = 72
    Top = 226
    Width = 89
    Height = 13
    Caption = ' Total Interest Paid'
  end
  object Label11: TLabel
    Left = 88
    Top = 250
    Width = 74
    Height = 13
    Caption = 'Total Fees Paid'
  end
  object Label12: TLabel
    Left = 72
    Top = 274
    Width = 90
    Height = 13
    Caption = ' Total Amount Paid'
  end
  object Edit1: TEdit
    Left = 176
    Top = 40
    Width = 121
    Height = 21
    AutoSize = False
    TabOrder = 0
    Text = '0'
    OnChange = calculate
    OnExit = fieldexit
    OnKeyPress = keypress
  end
  object ComboBox1: TComboBox
    Left = 176
    Top = 64
    Width = 121
    Height = 21
    AutoDropDown = True
    DropDownCount = 9
    ItemHeight = 13
    ItemIndex = 5
    TabOrder = 1
    Text = 'Monthly       12/year'
    OnChange = Frequencychange
    Items.Strings = (
      'Daily          365/year'
      'Daily          360/year'
      'Weekly       52/year'
      'Biweekly     26/year'
      'Bimonthly    24/year'
      'Monthly       12/year'
      'Quarterly       4/year'
      'Biannually     2/year'
      'Annually        1/year')
  end
  object Edit2: TEdit
    Left = 176
    Top = 88
    Width = 121
    Height = 21
    AutoSize = False
    TabOrder = 2
    Text = '300'
    OnChange = calculate
    OnExit = fieldexit
    OnKeyPress = keypress
  end
  object Edit3: TEdit
    Left = 176
    Top = 112
    Width = 121
    Height = 21
    AutoSize = False
    TabOrder = 3
    Text = '3.5'
    OnChange = calculate
    OnExit = fieldexit
    OnKeyPress = keypress
  end
  object Edit4: TEdit
    Left = 176
    Top = 136
    Width = 121
    Height = 21
    AutoSize = False
    TabOrder = 4
    Text = '0.00'
    OnChange = calculate
    OnExit = fieldexit
    OnKeyPress = keypress
  end
  object Edit6: TEdit
    Left = 176
    Top = 200
    Width = 121
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 5
    Text = '0'
  end
  object Edit7: TEdit
    Left = 176
    Top = 224
    Width = 121
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 6
    Text = '0'
  end
  object Edit8: TEdit
    Left = 176
    Top = 248
    Width = 121
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 7
    Text = '0'
  end
  object Edit9: TEdit
    Left = 176
    Top = 272
    Width = 121
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 8
    Text = '0'
  end
  object MainMenu1: TMainMenu
    object CreateReport1: TMenuItem
      Caption = 'Amortization Schedule'
      OnClick = CreateReport1Click
    end
    object Help1: TMenuItem
      Caption = 'About'
      OnClick = Help1Click
    end
  end
end
