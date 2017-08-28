object Form_cesty: TForm_cesty
  Left = 0
  Top = 0
  Caption = 'Zad'#225'n'#237' cesty'
  ClientHeight = 486
  ClientWidth = 1073
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 21
  object rHTMLLabel3: TrHTMLLabel
    Left = 8
    Top = 16
    Width = 318
    Height = 30
    Caption = 'Nastaven'#237' cesty pro zak'#225'zku '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 30
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel2: TrHTMLLabel
    Left = 8
    Top = 70
    Width = 149
    Height = 21
    Caption = 'Zaznamenan'#225' cesta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnHighlight
    Font.Height = 21
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel1: TrHTMLLabel
    Left = 602
    Top = 70
    Width = 49
    Height = 21
    Caption = 'Layout'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnHighlight
    Font.Height = 21
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object Memo1: TMemo
    Left = 602
    Top = 97
    Width = 463
    Height = 384
    Lines.Strings = (
      'Memo1')
    TabOrder = 0
  end
  object rStringGridEd1: TrStringGridEd
    Left = -5
    Top = 97
    Width = 601
    Height = 64
    BorderStyle = bsNone
    Color = clMenu
    ColCount = 6
    DefaultColWidth = 99
    DefaultRowHeight = 30
    DefaultDrawing = False
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = cl3DDkShadow
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs, goAlwaysShowEditor]
    ParentFont = False
    TabOrder = 1
    Columns = <
      item
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'N'#225'zev'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqAntialiased
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        MinValue = 1.000000000000000000
        MaxValue = 10.000000000000000000
        OutOfLimitDataColor = clPurple
        DropDownRows = 10
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'Cycle Time'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
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
        TitleCaption = 'Rychlost dopravn'#237'ku'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
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
        TitleCaption = #268'as v'#253'm'#283'ny barev'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleParentFont = False
        TitleAlignment = taRightJustify
      end
      item
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = #268'as '#269'i'#353't'#283'n'#237
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleParentFont = False
      end
      item
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -17
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'Opakov'#225'n'#237
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -17
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleParentFont = False
        TitleAlignment = taCenter
      end>
    RowHeights = (
      30
      30)
  end
  object Button_OK: TButton
    Left = 195
    Top = 445
    Width = 95
    Height = 33
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ModalResult = 1
    ParentFont = False
    TabOrder = 2
  end
  object Button1: TButton
    Left = 315
    Top = 445
    Width = 95
    Height = 33
    Caption = 'Storno'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ModalResult = 1
    ParentFont = False
    TabOrder = 3
  end
end
