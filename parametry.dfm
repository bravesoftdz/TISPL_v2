object Form_parametry: TForm_parametry
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Parametry objektu'
  ClientHeight = 433
  ClientWidth = 348
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label_CT: TLabel
    Left = 32
    Top = 448
    Width = 140
    Height = 13
    Caption = 'CYCLE TIME [min/voz'#237'k] :'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_kapacita: TLabel
    Left = 32
    Top = 488
    Width = 152
    Height = 13
    Caption = 'Kapacita objektu [voz'#237'k'#367']: '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_kapacita_hodnota: TLabel
    Left = 227
    Top = 486
    Width = 7
    Height = 13
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_CT_hodnota: TLabel
    Left = 227
    Top = 448
    Width = 7
    Height = 13
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_vypis: TLabel
    Left = 71
    Top = 507
    Width = 194
    Height = 13
    Caption = 'TT objektu nen'#237' toto'#382'n'#253' s TT linky!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_TT: TLabel
    Left = 32
    Top = 467
    Width = 183
    Height = 13
    Caption = 'TAKT TIME objektu [min/voz'#237'k] :'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_TT_hodnota: TLabel
    Left = 227
    Top = 467
    Width = 7
    Height = 13
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_delka_prepravniku: TLabel
    Left = 32
    Top = 526
    Width = 124
    Height = 13
    Caption = 'D'#233'lka koleje [metry]: '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_delka_prepravniku_hodnota: TLabel
    Left = 227
    Top = 526
    Width = 7
    Height = 13
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label1: TLabel
    Left = 2
    Top = 5
    Width = 37
    Height = 13
    Caption = 'N'#225'zev:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 193
    Top = 5
    Width = 48
    Height = 13
    Caption = 'Zkratka:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object rHTMLLabel5_rezim: TrHTMLLabel
    Left = 8
    Top = 93
    Width = 43
    Height = 19
    Caption = 'Re'#382'im'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_kapacita: TrHTMLLabel
    Left = 8
    Top = 281
    Width = 208
    Height = 19
    Caption = 'Po'#382'adovan'#225' kapacita [voz'#237'k'#367']'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object Image_vozik: TImage
    Left = 99
    Top = 486
    Width = 91
    Height = 34
    Hint = 'Kliknut'#237'm lze zm'#283'nit orientaci voz'#237'ku'
    ParentShowHint = False
    ShowHint = True
    Transparent = True
    Visible = False
    OnClick = Image_vozikClick
  end
  object rHTMLLabel_pohon: TrHTMLLabel
    Left = 8
    Top = 133
    Width = 45
    Height = 19
    Caption = 'Pohon'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_odchylkaCT: TrHTMLLabel
    Left = 8
    Top = 247
    Width = 201
    Height = 19
    Caption = 'Povolen'#225' odchylka z CT [%]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_delka_dopravniku: TrHTMLLabel
    Left = 8
    Top = 174
    Width = 154
    Height = 19
    Caption = 'D'#233'lka dopravn'#237'ku [m]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_name: TrHTMLLabel
    Left = 8
    Top = 53
    Width = 42
    Height = 19
    Caption = 'N'#225'zev'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_shortname: TrHTMLLabel
    Left = 232
    Top = 53
    Width = 52
    Height = 19
    Caption = 'Zkratka'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_cekani: TrHTMLLabel
    Left = 8
    Top = 211
    Width = 110
    Height = 19
    Caption = #268'ek'#225'n'#237' na palec'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_doporuc_cekani_value: TrHTMLLabel
    Left = 8
    Top = 355
    Width = 81
    Height = 19
    Caption = 'Upozorn'#283'n'#237
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object Button_OK: TButton
    Left = 221
    Top = 486
    Width = 95
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 4
    Visible = False
  end
  object ValueListEditor: TValueListEditor
    Left = 32
    Top = 576
    Width = 271
    Height = 55
    BiDiMode = bdLeftToRight
    BorderStyle = bsNone
    DefaultColWidth = 192
    KeyOptions = [keyEdit, keyUnique]
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing, goTabs, goAlwaysShowEditor, goThumbTracking, goFixedColClick, goFixedRowClick, goFixedHotTrack]
    ParentBiDiMode = False
    ScrollBars = ssNone
    Strings.Strings = (
      'PT - '#269'ist'#253' technologick'#253' v'#253'robn'#237' '#269'as=0'
      'MT - '#269'as p'#345'esouv'#225'n'#237' produktu=0'
      'WT - doba '#269'ek'#225'n'#237' voz'#237'ku=0'
      'IT - doba kontroly voz'#237'ku=0'
      'QT - doba '#269'ek'#225'n'#237' ve front'#283'=0'
      '...=')
    TabOrder = 2
    TitleCaptions.Strings = (
      ' n'#225'zev'
      ' '#269'as [min/voz'#237'k]')
    Visible = False
    OnClick = ValueListEditorClick
    OnStringsChange = ValueListEditorStringsChange
    ColWidths = (
      192
      77)
  end
  object ComboBox_druh_objektu: TComboBox
    Left = 31
    Top = 668
    Width = 267
    Height = 21
    Hint = 'Volba re'#382'imu objektu'
    Style = csDropDownList
    ItemIndex = 0
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    Text = 'STOP & GO'
    OnChange = ComboBox_druh_objektuChange
    Items.Strings = (
      'STOP & GO'
      'KONTINU'#193'LN'#205' (line tracking)'
      'POSTPROCESN'#205)
  end
  object ComboBox_dopravnik: TComboBox
    Left = 43
    Top = 545
    Width = 229
    Height = 21
    Hint = 'Volba dopravn'#237'ku'
    Style = csDropDownList
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    OnChange = ComboBox_dopravnikChange
  end
  object Button_min_sec: TButton
    Left = 196
    Top = 524
    Width = 50
    Height = 16
    Hint = 'P'#345'epo'#269#237't'#225' hodnoty z minut na sekundy a obr'#225'cen'#283
    Caption = 'min<->sec'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 8
    Visible = False
    OnClick = Button_min_secClick
  end
  object Button_DEL: TButton
    Left = 264
    Top = 528
    Width = 15
    Height = 15
    Hint = 'Sma'#382'e '#345#225'dek'
    Caption = 'X'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 12
    Visible = False
    OnClick = Button_DELClick
  end
  object Edit_name: TEdit
    Left = 43
    Top = 1
    Width = 142
    Height = 21
    CharCase = ecUpperCase
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 13
    OnChange = Edit_nameChange
  end
  object Edit_shortname: TEdit
    Left = 245
    Top = 1
    Width = 29
    Height = 21
    Alignment = taCenter
    AutoSize = False
    CharCase = ecUpperCase
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    MaxLength = 4
    ParentFont = False
    TabOrder = 14
  end
  object scGPPanel2: TscGPPanel
    Left = 0
    Top = 0
    Width = 348
    Height = 34
    Align = alTop
    TabOrder = 15
    FillGradientAngle = 90
    FillGradientBeginAlpha = 255
    FillGradientEndAlpha = 255
    FillGradientBeginColorOffset = 25
    FillGradientEndColorOffset = 25
    FrameWidth = 0
    FillColor = 10114859
    FillColorAlpha = 255
    FillColor2 = clNone
    FrameColor = clBtnShadow
    FrameColorAlpha = 255
    FrameRadius = 0
    ShowCaption = False
    CaptionGlowEffect.Enabled = False
    CaptionGlowEffect.Color = clBtnShadow
    CaptionGlowEffect.AlphaValue = 255
    CaptionGlowEffect.GlowSize = 7
    CaptionGlowEffect.Offset = 0
    CaptionGlowEffect.Intensive = True
    CaptionGlowEffect.StyleColors = True
    Color = clBtnFace
    Caption = 'scGPPanel2'
    TransparentBackground = True
    StorePaintBuffer = True
    object Konec: TscGPGlyphButton
      AlignWithMargins = True
      Left = 298
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 0
      OnClick = KonecClick
      Animation = True
      Caption = 'Konec'
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphLeft
      TransparentBackground = True
      ColorValue = clRed
      Options.NormalColor = clNone
      Options.HotColor = clRed
      Options.PressedColor = clRed
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = clBtnText
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 220
      Options.PressedColorAlpha = 180
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 5
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
      Options.FrameWidth = 2
      Options.FrameNormalColorAlpha = 70
      Options.FrameHotColorAlpha = 100
      Options.FramePressedColorAlpha = 150
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 30
      Options.FontNormalColor = clBtnText
      Options.FontHotColor = clBtnText
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clBtnText
      Options.FontDisabledColor = clBtnShadow
      Options.ShapeFillGradientAngle = 90
      Options.ShapeFillGradientPressedAngle = -90
      Options.ShapeCornerRadius = 10
      Options.ShapeStyle = scgpRect
      Options.ArrowSize = 9
      Options.StyleColors = True
      GlyphOptions.NormalColor = clWhite
      GlyphOptions.HotColor = clWhite
      GlyphOptions.PressedColor = clWhite
      GlyphOptions.FocusedColor = clWhite
      GlyphOptions.DisabledColor = clWhite
      GlyphOptions.NormalColorAlpha = 255
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkClose
      GlyphOptions.Thickness = 1
      GlyphOptions.StyleColors = True
      TextMargin = -1
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      RepeatClick = False
      RepeatClickInterval = 100
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
    object scLabel1: TscLabel
      Left = 0
      Top = 0
      Width = 261
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      DragForm = True
      GlowEffect.Enabled = False
      GlowEffect.Color = clBtnShadow
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 2
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      AutoSize = False
      Alignment = taCenter
      VertAlignment = scvtaCenter
      UseFontColorToStyleColor = True
      Caption = 'Parametry objektu'
    end
    object scGPGlyphButton15: TscGPGlyphButton
      Left = 261
      Top = 0
      Width = 37
      Height = 34
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNone
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Animation = True
      Caption = 'scGPGlyphButton1'
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphLeft
      TransparentBackground = True
      ColorValue = clRed
      Options.NormalColor = clBtnText
      Options.HotColor = clBtnText
      Options.PressedColor = clBtnText
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = clBtnText
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 20
      Options.PressedColorAlpha = 30
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 5
      Options.FrameNormalColor = clBtnText
      Options.FrameHotColor = clBtnText
      Options.FramePressedColor = clBtnText
      Options.FrameFocusedColor = clHighlight
      Options.FrameDisabledColor = clBtnText
      Options.FrameWidth = 2
      Options.FrameNormalColorAlpha = 0
      Options.FrameHotColorAlpha = 0
      Options.FramePressedColorAlpha = 0
      Options.FrameFocusedColorAlpha = 0
      Options.FrameDisabledColorAlpha = 0
      Options.FontNormalColor = clBtnText
      Options.FontHotColor = clBtnText
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clBtnText
      Options.FontDisabledColor = clBtnShadow
      Options.ShapeFillGradientAngle = 90
      Options.ShapeFillGradientPressedAngle = -90
      Options.ShapeCornerRadius = 5
      Options.ShapeStyle = scgpRoundedRect
      Options.ArrowSize = 9
      Options.StyleColors = True
      GlyphOptions.NormalColor = 16744448
      GlyphOptions.HotColor = 16744448
      GlyphOptions.PressedColor = clBtnText
      GlyphOptions.FocusedColor = 16744448
      GlyphOptions.DisabledColor = 16744448
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 150
      GlyphOptions.Kind = scgpbgkInfo
      GlyphOptions.Thickness = 2
      GlyphOptions.StyleColors = True
      TextMargin = -1
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      RepeatClick = False
      RepeatClickInterval = 100
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
  end
  object rEditNum_kapacita: TrEditNum
    Left = 232
    Top = 277
    Width = 108
    Height = 28
    BiDiMode = bdRightToLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 16
  end
  object scGPButton1_OK: TscGPButton
    Left = 62
    Top = 383
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    TabStop = True
    OnClick = scGPButton1_OKClick
    Animation = False
    Caption = 'Ulo'#382'it'
    CanFocused = True
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = -1
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = 33023
    Options.HotColor = 1871359
    Options.PressedColor = clBlack
    Options.FocusedColor = clRed
    Options.DisabledColor = clBlack
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clGray
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clGray
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clWhite
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    RepeatClick = False
    RepeatClickInterval = 100
    GlowEffect.Enabled = False
    GlowEffect.Color = clMaroon
    GlowEffect.AlphaValue = 255
    GlowEffect.GlowSize = 1
    GlowEffect.Offset = 0
    GlowEffect.Intensive = True
    GlowEffect.StyleColors = True
    GlowEffect.HotColor = clNone
    GlowEffect.PressedColor = clRed
    GlowEffect.FocusedColor = clRed
    GlowEffect.PressedGlowSize = 1
    GlowEffect.PressedAlphaValue = 255
    GlowEffect.States = [scsHot, scsPressed, scsFocused]
    ImageGlow = True
    ShowGalleryMenuFromTop = False
    ShowGalleryMenuFromRight = False
    ShowMenuArrow = True
    ShowFocusRect = True
    Down = False
    GroupIndex = 0
    AllowAllUp = False
  end
  object scGPButton3: TscGPButton
    Left = 184
    Top = 383
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    TabStop = True
    OnClick = scGPButton3Click
    Animation = False
    Caption = 'Storno'
    CanFocused = True
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = -1
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = 33023
    Options.HotColor = 1871359
    Options.PressedColor = clBlack
    Options.FocusedColor = 33023
    Options.DisabledColor = clPurple
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clGray
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clGray
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clWhite
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    RepeatClick = False
    RepeatClickInterval = 100
    GlowEffect.Enabled = False
    GlowEffect.Color = clMaroon
    GlowEffect.AlphaValue = 255
    GlowEffect.GlowSize = 1
    GlowEffect.Offset = 0
    GlowEffect.Intensive = True
    GlowEffect.StyleColors = True
    GlowEffect.HotColor = clNone
    GlowEffect.PressedColor = clRed
    GlowEffect.FocusedColor = clRed
    GlowEffect.PressedGlowSize = 1
    GlowEffect.PressedAlphaValue = 255
    GlowEffect.States = [scsHot, scsPressed, scsFocused]
    ImageGlow = True
    ShowGalleryMenuFromTop = False
    ShowGalleryMenuFromRight = False
    ShowMenuArrow = True
    ShowFocusRect = True
    Down = False
    GroupIndex = 0
    AllowAllUp = False
  end
  object RadioButton_na_delku: TRadioButton
    Left = 216
    Top = 444
    Width = 113
    Height = 17
    Caption = 'RadioButton_na_delku'
    TabOrder = 17
    Visible = False
  end
  object Edit_vzdalenost_voziku: TEdit
    Left = 182
    Top = 635
    Width = 121
    Height = 21
    TabOrder = 18
    Text = 'Edit_vzdalenost_voziku'
    Visible = False
  end
  object RadioButton_na_sirku: TRadioButton
    Left = 37
    Top = 639
    Width = 113
    Height = 17
    Caption = 'RadioButton_na_sirku'
    TabOrder = 19
    Visible = False
  end
  object scCheckBox_stopky: TscCheckBox
    Left = 2
    Top = 319
    Width = 338
    Height = 17
    BiDiMode = bdRightToLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGrayText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 11
    TabStop = True
    CustomCheckedImageIndex = -1
    CustomCheckedImageHotIndex = -1
    CustomCheckedImagePressedIndex = -1
    CustomCheckedImageDisabledIndex = -1
    CustomUnCheckedImageIndex = -1
    CustomUnCheckedImageHotIndex = -1
    CustomUnCheckedImagePressedIndex = -1
    CustomUnCheckedImageDisabledIndex = -1
    CustomGrayedImageIndex = -1
    CustomGrayedImageHotIndex = -1
    CustomGrayedImagePressedIndex = -1
    CustomGrayedImageDisabledIndex = -1
    UseFontColorToStyleColor = False
    Animation = False
    Caption = 'STOP stanice na konci objektu '
    CanFocused = True
    Spacing = 0
    Layout = blGlyphLeft
    ImageIndex = -1
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 175
    GlowEffect.GlowSize = 7
    GlowEffect.Offset = 0
    GlowEffect.Intensive = True
    GlowEffect.StyleColors = True
    GlowEffect.HotColor = clNone
    GlowEffect.PressedColor = clNone
    GlowEffect.FocusedColor = clNone
    GlowEffect.PressedGlowSize = 7
    GlowEffect.PressedAlphaValue = 255
    GlowEffect.States = [scsHot, scsPressed, scsFocused]
    ImageGlow = True
    Checked = False
    ShowFocusRect = True
  end
  object scEdit_name: TscEdit
    Left = 65
    Top = 50
    Width = 158
    Height = 27
    UseFontColorToStyleColor = False
    ContentMarginLeft = 0
    ContentMarginRight = 0
    ContentMarginTop = 0
    ContentMarginBottom = 0
    CustomBackgroundImageNormalIndex = -1
    CustomBackgroundImageHotIndex = -1
    CustomBackgroundImageDisabledIndex = -1
    WallpaperIndex = -1
    LeftButton.ComboButton = False
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ShowHint = False
    LeftButton.ShowEllipses = False
    LeftButton.StyleKind = scbsPushButton
    LeftButton.Width = 18
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.CustomImageNormalIndex = -1
    LeftButton.CustomImageHotIndex = -1
    LeftButton.CustomImagePressedIndex = -1
    LeftButton.CustomImageDisabledIndex = -1
    RightButton.ComboButton = False
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ShowHint = False
    RightButton.ShowEllipses = False
    RightButton.StyleKind = scbsPushButton
    RightButton.Width = 18
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.CustomImageNormalIndex = -1
    RightButton.CustomImageHotIndex = -1
    RightButton.CustomImagePressedIndex = -1
    RightButton.CustomImageDisabledIndex = -1
    Transparent = False
    BorderKind = scebFrame
    FrameColor = clBtnShadow
    FrameActiveColor = clHighlight
    Text = ''
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object scEdit_shortname: TscEdit
    Left = 290
    Top = 50
    Width = 50
    Height = 27
    UseFontColorToStyleColor = False
    ContentMarginLeft = 0
    ContentMarginRight = 0
    ContentMarginTop = 0
    ContentMarginBottom = 0
    CustomBackgroundImageNormalIndex = -1
    CustomBackgroundImageHotIndex = -1
    CustomBackgroundImageDisabledIndex = -1
    WallpaperIndex = -1
    LeftButton.ComboButton = False
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ShowHint = False
    LeftButton.ShowEllipses = False
    LeftButton.StyleKind = scbsPushButton
    LeftButton.Width = 18
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.CustomImageNormalIndex = -1
    LeftButton.CustomImageHotIndex = -1
    LeftButton.CustomImagePressedIndex = -1
    LeftButton.CustomImageDisabledIndex = -1
    RightButton.ComboButton = False
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ShowHint = False
    RightButton.ShowEllipses = False
    RightButton.StyleKind = scbsPushButton
    RightButton.Width = 18
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.CustomImageNormalIndex = -1
    RightButton.CustomImageHotIndex = -1
    RightButton.CustomImagePressedIndex = -1
    RightButton.CustomImageDisabledIndex = -1
    Transparent = False
    BorderKind = scebFrame
    FrameColor = clBtnShadow
    FrameActiveColor = clHighlight
    Text = ''
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 20
  end
  object scComboBox1: TscComboBox
    Left = 232
    Top = 208
    Width = 108
    Height = 27
    ItemIndex = 2
    Items.Strings = (
      'ne'
      'ano'
      'automaticky')
    WordBreak = False
    SelectionStyle = scstStyled
    SelectionColor = clNone
    SelectionTextColor = clHighlightText
    ImageIndex = -1
    ItemHeight = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    Text = 'automaticky'
    StyleKind = scscbDefault
    ShowFocusRect = True
  end
  object rEditNum_delka_dopravniku: TrEditNum
    Left = 232
    Top = 174
    Width = 108
    Height = 28
    BiDiMode = bdRightToLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    NumbersOnly = True
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 21
  end
  object rEditNum_odchylka: TrEditNum
    Left = 232
    Top = 243
    Width = 108
    Height = 28
    Hint = #218'prava CT v objektu za '#250#269'elem srovn'#225'n'#237' TT linky'
    BiDiMode = bdRightToLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 7
  end
  object scComboBox_rezim: TscComboBox
    Left = 65
    Top = 90
    Width = 275
    Height = 27
    ItemIndex = 0
    Items.Strings = (
      'STOP&GO'
      'Kontinu'#225'ln'#237' (line tracking)'
      'Postprocesn'#237
      'Stopka')
    WordBreak = False
    SelectionStyle = scstStyled
    SelectionColor = clNone
    SelectionTextColor = clHighlightText
    ImageIndex = -1
    ItemHeight = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'STOP&GO'
    StyleKind = scscbDefault
    ShowFocusRect = True
  end
  object scComboBox_pohon: TscComboBox
    Left = 65
    Top = 130
    Width = 275
    Height = 27
    ItemIndex = -1
    WordBreak = False
    SelectionStyle = scstStyled
    SelectionColor = clNone
    SelectionTextColor = clHighlightText
    ImageIndex = -1
    ItemHeight = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 22
    StyleKind = scscbDefault
    ShowFocusRect = True
  end
  object scGPGlyphButton1: TscGPGlyphButton
    Left = 95
    Top = 345
    Width = 34
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 23
    Animation = True
    Caption = 'scGPGlyphButton1'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = True
    ColorValue = clRed
    Options.NormalColor = clBtnText
    Options.HotColor = clBtnText
    Options.PressedColor = clBtnText
    Options.FocusedColor = clBtnFace
    Options.DisabledColor = clBtnText
    Options.NormalColorAlpha = 0
    Options.HotColorAlpha = 20
    Options.PressedColorAlpha = 30
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 5
    Options.FrameNormalColor = clBtnText
    Options.FrameHotColor = clBtnText
    Options.FramePressedColor = clBtnText
    Options.FrameFocusedColor = clHighlight
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 0
    Options.FrameHotColorAlpha = 0
    Options.FramePressedColorAlpha = 0
    Options.FrameFocusedColorAlpha = 0
    Options.FrameDisabledColorAlpha = 0
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = clBtnShadow
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeCornerRadius = 5
    Options.ShapeStyle = scgpRoundedRect
    Options.ArrowSize = 9
    Options.StyleColors = True
    GlyphOptions.NormalColor = 16744448
    GlyphOptions.HotColor = 16744448
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = 16744448
    GlyphOptions.DisabledColor = 16744448
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkInfo
    GlyphOptions.Thickness = 2
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    RepeatClick = False
    RepeatClickInterval = 100
    ShowGalleryMenuFromTop = False
    ShowGalleryMenuFromRight = False
    ShowMenuArrow = False
    ShowFocusRect = True
    Down = False
    GroupIndex = 0
    AllowAllUp = False
  end
end
