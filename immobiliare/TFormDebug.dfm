object FormDebug: TFormDebug
  Left = 0
  Top = 0
  Caption = ' Debug'
  ClientHeight = 435
  ClientWidth = 284
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object RichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 284
    Height = 435
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object MainMenu: TMainMenu
    Left = 45
    Top = 26
    object MenuFile: TMenuItem
      Caption = '&File'
      object FileSaveAs: TMenuItem
        Caption = '&Save As ...'
        OnClick = FileSaveAsClick
      end
    end
    object MenuActions: TMenuItem
      Caption = '&Actions'
      object ActionsClear: TMenuItem
        Caption = '&Clear'
        OnClick = ActionsClearClick
      end
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '*.txt'
    Filter = 'Ascii text file|*.txt'
    Left = 115
    Top = 28
  end
end
