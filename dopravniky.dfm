object Form_dopravnik: TForm_dopravnik
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Parametry linky'
  ClientHeight = 600
  ClientWidth = 662
  Color = clActiveCaption
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clActiveCaption
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Font.Quality = fqClearType
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel4: TrHTMLLabel
    Left = 8
    Top = 46
    Width = 42
    Height = 21
    Caption = 'D'#233'lka'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnHighlight
    Font.Height = 21
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel5: TrHTMLLabel
    Left = 186
    Top = 75
    Width = 134
    Height = 17
    Caption = '(Nastaven'#237' geometrie)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnHighlight
    Font.Height = 17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object ValueListEditor: TValueListEditor
    Left = 312
    Top = 8
    Width = 232
    Height = 52
    Align = alCustom
    BiDiMode = bdLeftToRight
    BorderStyle = bsNone
    DefaultRowHeight = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = cl3DDkShadow
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    KeyOptions = [keyEdit, keyUnique]
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing, goTabs, goAlwaysShowEditor, goThumbTracking, goFixedColClick, goFixedRowClick, goFixedHotTrack]
    ParentBiDiMode = False
    ParentFont = False
    ScrollBars = ssNone
    Strings.Strings = (
      'hlavn'#237' dopravn'#237'k=5'
      'vedlej'#353#237' dopravn'#237'k=3')
    TabOrder = 0
    TitleCaptions.Strings = (
      ' n'#225'zev'
      ' rychlost [m/min]')
    Visible = False
    ColWidths = (
      161
      69)
  end
  object Button_ADD: TButton
    Left = 130
    Top = 172
    Width = 31
    Height = 25
    Hint = 'P'#345'idat pohon'
    Caption = '+'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 1
    OnClick = Button_ADDClick
  end
  object Button_DEL: TButton
    Left = 173
    Top = 172
    Width = 28
    Height = 25
    Hint = 'Odebrat pohon'
    Caption = '-'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 2
    OnClick = Button_DELClick
  end
  object Button_OK: TButton
    Left = 130
    Top = 369
    Width = 95
    Height = 25
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ModalResult = 1
    ParentFont = False
    TabOrder = 3
    OnClick = Button_OKClick
  end
  object StaticText1: TStaticText
    Left = 8
    Top = 165
    Width = 116
    Height = 32
    Caption = 'Dopravn'#237'ky'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -23
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 4
  end
  object StaticText2: TStaticText
    Left = 8
    Top = 8
    Width = 67
    Height = 32
    Caption = 'Voz'#237'ky'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -23
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 5
  end
  object rEditNum2: TrEditNum
    Left = 81
    Top = 39
    Width = 65
    Height = 28
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 6
  end
  object rStringGridEd1: TrStringGridEd
    Left = 8
    Top = 203
    Width = 657
    Height = 94
    BorderStyle = bsNone
    Color = clMenu
    ColCount = 4
    DefaultColWidth = 160
    DefaultRowHeight = 30
    DefaultDrawing = False
    RowCount = 3
    Font.Charset = DEFAULT_CHARSET
    Font.Color = cl3DDkShadow
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs, goAlwaysShowEditor]
    ParentFont = False
    TabOrder = 7
    Columns = <
      item
        Alignment = taCenter
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'ID'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        PickList.Strings = (
          're'#225'ln'#225
          'servisn'#237)
        TitleCaption = 'N'#225'zev'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'Rychlost [m/min]'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'Rozte'#269' [mm]'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
      end>
    ColWidths = (
      160
      195
      139
      160)
    RowHeights = (
      30
      30
      30)
  end
  object Button1: TButton
    Left = 186
    Top = 37
    Width = 120
    Height = 32
    Caption = 'Pokro'#269'il'#233
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 8
    OnClick = Button_ADDClick
  end
  object Button2: TButton
    Left = 242
    Top = 369
    Width = 95
    Height = 25
    Caption = 'Storno'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ModalResult = 1
    ParentFont = False
    TabOrder = 9
    OnClick = Button_OKClick
  end
end
