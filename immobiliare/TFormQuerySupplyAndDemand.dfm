object FormQuerySupplyAndDemand: TFormQuerySupplyAndDemand
  Left = 0
  Top = 0
  Caption = 'FormQuerySupplyAndDemand'
  ClientHeight = 486
  ClientWidth = 802
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCanResize = FormCanResize
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PanelLeft: TPanel
    Left = 0
    Top = 0
    Width = 280
    Height = 444
    Align = alLeft
    TabOrder = 1
    object Label2: TLabel
      Left = 14
      Top = 254
      Width = 80
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Typology :'
    end
    object Label4: TLabel
      Left = 14
      Top = 281
      Width = 80
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Region :'
    end
    object Label1: TLabel
      Left = 24
      Top = 36
      Width = 124
      Height = 16
      Caption = 'Customers Filtering'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 24
      Top = 227
      Width = 100
      Height = 16
      Caption = 'Assets Filtering'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 14
      Top = 93
      Width = 80
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Budget :'
    end
    object Label8: TLabel
      Left = 14
      Top = 66
      Width = 80
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Ownership :'
    end
    object Label9: TLabel
      Left = 36
      Top = 308
      Width = 58
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Order by :'
    end
    object Label10: TLabel
      Left = 34
      Top = 147
      Width = 58
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Order by :'
    end
    object Label11: TLabel
      Left = 14
      Top = 120
      Width = 80
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Region :'
    end
    object ButtonUpdate: TButton
      Left = 104
      Top = 177
      Width = 129
      Height = 27
      Caption = '&Update'
      TabOrder = 0
      OnClick = ButtonUpdateClick
    end
    object ComboBoxTypology: TComboBox
      Left = 104
      Top = 251
      Width = 128
      Height = 21
      Style = csDropDownList
      TabOrder = 1
    end
    object ComboBoxAssetsRegion: TComboBox
      Left = 104
      Top = 278
      Width = 128
      Height = 21
      Style = csDropDownList
      TabOrder = 2
    end
    object ComboBoxBudget: TComboBox
      Left = 104
      Top = 90
      Width = 128
      Height = 21
      TabOrder = 3
    end
    object ComboBoxOwnership: TComboBox
      Left = 104
      Top = 63
      Width = 128
      Height = 21
      Style = csDropDownList
      TabOrder = 4
    end
    object ComboBoxAssetsSorting: TComboBox
      Left = 104
      Top = 305
      Width = 128
      Height = 21
      Style = csDropDownList
      TabOrder = 5
    end
    object ComboBoxCustomersSorting: TComboBox
      Left = 104
      Top = 144
      Width = 128
      Height = 21
      Style = csDropDownList
      TabOrder = 6
    end
    object ComboBoxCustomersRegion: TComboBox
      Left = 104
      Top = 117
      Width = 128
      Height = 21
      Style = csDropDownList
      TabOrder = 7
    end
  end
  object PanelBottom: TPanel
    Left = 0
    Top = 444
    Width = 802
    Height = 42
    Align = alBottom
    TabOrder = 0
    object ButtonClose: TButton
      Left = 284
      Top = 10
      Width = 75
      Height = 25
      Caption = '&Close'
      TabOrder = 0
      OnClick = ButtonCloseClick
    end
  end
  object PanelClient: TPanel
    Left = 280
    Top = 0
    Width = 522
    Height = 444
    Align = alClient
    TabOrder = 2
    object PanelAssets: TPanel
      Left = 40
      Top = 230
      Width = 439
      Height = 197
      TabOrder = 0
      object Panel2: TPanel
        Left = 1
        Top = 1
        Width = 437
        Height = 41
        Align = alTop
        TabOrder = 0
        object LabelAssets: TLabel
          Left = 11
          Top = 19
          Width = 32
          Height = 13
          Caption = 'Assets'
        end
        object Label7: TLabel
          Left = 11
          Top = 5
          Width = 38
          Height = 13
          Caption = 'Assets'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
      end
      object Panel4: TPanel
        Left = 1
        Top = 155
        Width = 437
        Height = 41
        Align = alBottom
        TabOrder = 1
        object LabelAssetsResults: TLabel
          Left = 11
          Top = 11
          Width = 256
          Height = 13
          AutoSize = False
        end
      end
      object GridAssets: TStringGrid
        Left = 1
        Top = 42
        Width = 437
        Height = 113
        Align = alClient
        TabOrder = 2
        OnDblClick = GridAssetsDblClick
      end
    end
    object PanelCustomers: TPanel
      Left = 40
      Top = 21
      Width = 437
      Height = 203
      TabOrder = 1
      object Panel1: TPanel
        Left = 1
        Top = 1
        Width = 435
        Height = 41
        Align = alTop
        TabOrder = 0
        object LabelCustomers: TLabel
          Left = 11
          Top = 19
          Width = 51
          Height = 13
          Caption = 'Customers'
        end
        object Label6: TLabel
          Left = 11
          Top = 5
          Width = 61
          Height = 13
          Caption = 'Customers'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
      end
      object Panel3: TPanel
        Left = 1
        Top = 161
        Width = 435
        Height = 41
        Align = alBottom
        TabOrder = 1
        object LabelCustomersResults: TLabel
          Left = 11
          Top = 11
          Width = 256
          Height = 13
          AutoSize = False
        end
      end
      object GridCustomers: TStringGrid
        Left = 1
        Top = 42
        Width = 435
        Height = 119
        Align = alClient
        TabOrder = 2
        OnClick = GridCustomersClick
        OnDblClick = GridCustomersDblClick
      end
    end
  end
end
