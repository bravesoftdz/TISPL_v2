object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 337
  ClientWidth = 618
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object rStringGridEd_tab_dopravniky: TrStringGridEd
    Left = 0
    Top = 0
    Width = 618
    Height = 337
    Align = alClient
    BorderStyle = bsNone
    Color = clWhite
    Ctl3D = False
    DefaultColWidth = 120
    DefaultRowHeight = 30
    DefaultDrawing = False
    DrawingStyle = gdsGradient
    FixedColor = clWhite
    FixedCols = 0
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = cl3DDkShadow
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    GradientEndColor = clWhite
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs, goAlwaysShowEditor]
    ParentCtl3D = False
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    Columns = <
      item
        Alignment = taCenter
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'OBJEKT'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        Alignment = taCenter
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        PickList.Strings = (
          're'#225'ln'#225
          'servisn'#237)
        TitleCaption = 're'#382'im'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        DataType = sgdNumber
        Alignment = taCenter
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'po'#382'. kapacita'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        DataType = sgdNumber
        Alignment = taCenter
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'CT'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleFont.Quality = fqClearType
        TitleParentFont = False
        TitleAlignment = taCenter
      end
      item
        DataType = sgdNumber
        Alignment = taCenter
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = cl3DDkShadow
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TitleCaption = 'dopravn'#237'k'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = cl3DDkShadow
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleParentFont = False
        TitleAlignment = taCenter
      end>
    AutoAddNewRow = True
    ExplicitLeft = 5
    ExplicitTop = 3
    ExplicitWidth = 605
    ExplicitHeight = 126
    RowHeights = (
      30
      30)
  end
end
