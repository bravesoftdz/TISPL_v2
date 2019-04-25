object Form_parametry_linky: TForm_parametry_linky
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Parametry linky'
  ClientHeight = 650
  ClientWidth = 1108
  Color = 15790320
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clActiveCaption
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Font.Quality = fqClearType
  OldCreateOrder = False
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnKeyDown = FormKeyDown
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel_eDesigner: TrHTMLLabel
    Left = 467
    Top = 601
    Width = 308
    Height = 19
    Caption = 'Zadejte parametry linky nebo zvolte storno.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    Visible = False
  end
  object rHTMLLabel_takt: TrHTMLLabel
    Left = 8
    Top = 82
    Width = 98
    Height = 21
    Caption = 'TaktTime <font color=#2b579a>[s]</font>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    Visible = False
    OnClick = rHTMLLabel_taktClick
  end
  object rHTMLLabel_sirka_jig: TrHTMLLabel
    Left = 218
    Top = 160
    Width = 71
    Height = 21
    Caption = #353#237#345'ka <font color=#2b579a>[m]</font>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    Visible = False
    OnClick = rHTMLLabel_sirka_jigClick
  end
  object rHTMLLabel_doporuc_pohony: TrHTMLLabel
    Left = 491
    Top = 275
    Width = 445
    Height = 19
    Caption = 'Navr'#382'en'#233' pohony pro objekty bez p'#345'i'#345'azen'#253'ch pohon'#367':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    Font.Quality = fqClearType
    ParentFont = False
    Visible = False
  end
  object rHTMLLabel_delka_jig: TrHTMLLabel
    Left = 111
    Top = 160
    Width = 75
    Height = 21
    Caption = 'd'#233'lka <font color=#2b579a>[m]</font>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    OnClick = rHTMLLabel_delka_jigClick
  end
  object rHTMLLabel3: TrHTMLLabel
    Left = 218
    Top = 246
    Width = 73
    Height = 19
    Caption = 'podlahov'#253
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    OnClick = rHTMLLabel_delkavozikuClick
  end
  object rHTMLLabel4: TrHTMLLabel
    Left = 345
    Top = 246
    Width = 67
    Height = 19
    Caption = 'podv'#283'sn'#253
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    OnClick = rHTMLLabel_delkavozikuClick
  end
  object rHTMLLabel_JIG: TrHTMLLabel
    Left = 71
    Top = 184
    Width = 20
    Height = 19
    Caption = 'Jig'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    OnClick = rHTMLLabel_delkavozikuClick
  end
  object rHTMLLabel_vyska_jig: TrHTMLLabel
    Left = 324
    Top = 160
    Width = 77
    Height = 21
    Caption = 'v'#253#353'ka <font color=#2b579a>[m]</font>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    Visible = False
    OnClick = rHTMLLabel_sirka_jigClick
  end
  object rHTMLLabel_podvozek_zaves: TrHTMLLabel
    Left = 33
    Top = 249
    Width = 67
    Height = 19
    Caption = 'Podvozek'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    OnClick = rHTMLLabel_delkavozikuClick
  end
  object rHTMLLabel_delka_podvozek: TrHTMLLabel
    Left = 111
    Top = 218
    Width = 75
    Height = 21
    Caption = 'd'#233'lka <font color=#2b579a>[m]</font>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    OnClick = rEditNum_delkavozikuClick
  end
  object rHTMLLabel_InfoText: TrHTMLLabel
    Left = 10
    Top = 614
    Width = 153
    Height = 19
    Caption = 'rHTMLLabel_InfoText'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    OnClick = rHTMLLabel_InfoTextClick
  end
  object rHTMLLabel_info_zmenaR: TrHTMLLabel
    Left = 8
    Top = 589
    Width = 166
    Height = 19
    Caption = 'rHTML_label_RinfoText'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHotLight
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    OnClick = rHTMLLabel_InfoTextClick
  end
  object Button_save: TscGPButton
    Left = 214
    Top = 573
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 0
    TabStop = True
    OnClick = Button_saveClick
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
    Options.PressedColor = clGreen
    Options.FocusedColor = clRed
    Options.DisabledColor = clPurple
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = 1871359
    Options.FrameHotColor = 1871359
    Options.FramePressedColor = 1871359
    Options.FrameFocusedColor = 1871359
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
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    ModalResult = 1
    ModalSetting = True
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object Button_storno: TscGPButton
    Left = 352
    Top = 573
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 1
    TabStop = True
    OnClick = Button_stornoClick
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
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = 1871359
    Options.FrameHotColor = 1871359
    Options.FramePressedColor = 1871359
    Options.FrameFocusedColor = 1871359
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
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    ModalResult = 2
    ModalSetting = True
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPPanel2: TscGPPanel
    Left = 0
    Top = 0
    Width = 1108
    Height = 34
    Align = alTop
    TabOrder = 2
    BlurBackground = False
    BlurBackgroundAmount = 5
    CustomImageIndex = -1
    DragForm = False
    DragTopForm = True
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
    Sizeable = False
    WallpaperIndex = -1
    object Konec: TscGPGlyphButton
      AlignWithMargins = True
      Left = 1058
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
      Options.NormalColor2 = clNone
      Options.HotColor2 = clNone
      Options.PressedColor2 = clNone
      Options.FocusedColor2 = clNone
      Options.DisabledColor2 = clNone
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 220
      Options.PressedColorAlpha = 180
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 5
      Options.NormalColor2Alpha = 255
      Options.HotColor2Alpha = 255
      Options.PressedColor2Alpha = 255
      Options.FocusedColor2Alpha = 255
      Options.DisabledColor2Alpha = 255
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
      Options.ShapeFillGradientColorOffset = 25
      Options.ShapeCornerRadius = 10
      Options.ShapeStyle = scgpRect
      Options.ArrowSize = 9
      Options.ArrowAreaSize = 0
      Options.ArrowType = scgpatDefault
      Options.ArrowThickness = 2
      Options.ArrowThicknessScaled = False
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
      GlyphOptions.ThicknessScaled = False
      GlyphOptions.Size = 0
      GlyphOptions.StyleColors = True
      ModalResult = 8
      ModalSetting = True
      TextMargin = -1
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      SplitButton = False
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
      Width = 984
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
      Font.Quality = fqClearType
      ParentFont = False
      TabOrder = 1
      DragForm = True
      DragTopForm = True
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
      Caption = 'Parametry linky'
    end
    object scGPGlyphButton_info: TscGPGlyphButton
      Left = 1021
      Top = 0
      Width = 37
      Height = 34
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNone
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      TabOrder = 2
      Visible = False
      OnClick = scGPGlyphButton_infoClick
      Animation = True
      Caption = 'Info'
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
      Options.NormalColor2 = clNone
      Options.HotColor2 = clNone
      Options.PressedColor2 = clNone
      Options.FocusedColor2 = clNone
      Options.DisabledColor2 = clNone
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 20
      Options.PressedColorAlpha = 30
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 5
      Options.NormalColor2Alpha = 255
      Options.HotColor2Alpha = 255
      Options.PressedColor2Alpha = 255
      Options.FocusedColor2Alpha = 255
      Options.DisabledColor2Alpha = 255
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
      Options.ShapeFillGradientColorOffset = 25
      Options.ShapeCornerRadius = 5
      Options.ShapeStyle = scgpRoundedRect
      Options.ArrowSize = 9
      Options.ArrowAreaSize = 0
      Options.ArrowType = scgpatDefault
      Options.ArrowThickness = 2
      Options.ArrowThicknessScaled = False
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
      GlyphOptions.ThicknessScaled = False
      GlyphOptions.Size = 0
      GlyphOptions.StyleColors = True
      TextMargin = -1
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      SplitButton = False
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
    object scGPGlyphButton_refresh: TscGPGlyphButton
      Left = 984
      Top = 0
      Width = 37
      Height = 34
      Hint = 'Na'#269#237'st p'#367'vodn'#237' ulo'#382'en'#225' data'
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNone
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      Visible = False
      Animation = True
      Caption = 'Info'
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
      Options.NormalColor2 = clNone
      Options.HotColor2 = clNone
      Options.PressedColor2 = clNone
      Options.FocusedColor2 = clNone
      Options.DisabledColor2 = clNone
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 20
      Options.PressedColorAlpha = 30
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 5
      Options.NormalColor2Alpha = 255
      Options.HotColor2Alpha = 255
      Options.PressedColor2Alpha = 255
      Options.FocusedColor2Alpha = 255
      Options.DisabledColor2Alpha = 255
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
      Options.ShapeFillGradientColorOffset = 25
      Options.ShapeCornerRadius = 5
      Options.ShapeStyle = scgpRoundedRect
      Options.ArrowSize = 9
      Options.ArrowAreaSize = 0
      Options.ArrowType = scgpatDefault
      Options.ArrowThickness = 2
      Options.ArrowThicknessScaled = False
      Options.StyleColors = True
      GlyphOptions.NormalColor = clWhite
      GlyphOptions.HotColor = 16744448
      GlyphOptions.PressedColor = clBtnText
      GlyphOptions.FocusedColor = 16744448
      GlyphOptions.DisabledColor = 16744448
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 150
      GlyphOptions.Kind = scgpbgkRefresh
      GlyphOptions.Thickness = 2
      GlyphOptions.ThicknessScaled = False
      GlyphOptions.Size = 0
      GlyphOptions.StyleColors = True
      TextMargin = -1
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      SplitButton = False
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
  object scGPButton_vozik: TscGPButton
    Left = 0
    Top = 123
    Width = 100
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 3
    Animation = False
    Caption = '  Voz'#237'k'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 21
    ImageMargin = 0
    TransparentBackground = False
    Options.NormalColor = clSilver
    Options.HotColor = clPurple
    Options.PressedColor = 15790320
    Options.FocusedColor = clRed
    Options.DisabledColor = clPurple
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clSilver
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clBlack
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
    Down = True
    GroupIndex = 0
    AllowAllUp = False
  end
  object scGPButton_pohon: TscGPButton
    Left = 0
    Top = 294
    Width = 109
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 4
    Animation = False
    Caption = '  Pohony'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 25
    ImageMargin = 0
    TransparentBackground = False
    Options.NormalColor = clSilver
    Options.HotColor = clPurple
    Options.PressedColor = 15790320
    Options.FocusedColor = clRed
    Options.DisabledColor = clPurple
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clSilver
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clBlack
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
    Down = True
    GroupIndex = 0
    AllowAllUp = False
  end
  object Vypis_pohony: TButton
    Left = 295
    Top = 625
    Width = 75
    Height = 25
    Caption = 'Vypis_pohony'
    TabOrder = 5
    Visible = False
    OnClick = Vypis_pohonyClick
  end
  object scGPButton_obecne: TscGPButton
    Left = -6
    Top = 40
    Width = 100
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 6
    Animation = False
    Caption = '  Takt'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 34
    ImageMargin = 0
    TransparentBackground = False
    Options.NormalColor = clSilver
    Options.HotColor = clPurple
    Options.PressedColor = 15790320
    Options.FocusedColor = clRed
    Options.DisabledColor = clPurple
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clSilver
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clBlack
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
    Down = True
    GroupIndex = 0
    AllowAllUp = False
  end
  object scExPanel_doporuc_pohony: TscExPanel
    Left = 480
    Top = 239
    Width = 456
    Height = 66
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGrayText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 7
    Visible = False
    BorderWidth = 1
    BackgroundStyle = scexbgsPanel
    FrameColor = clBtnShadow
    HeaderColor = clBtnFace
    ButtonGlyphColor = clBtnText
    HeaderStyle = scexphsColor
    CaptionImageIndex = -1
    ChangeRollStateWithCaptionClick = False
    Color = clWhite
    Spacing = 5
    ShowFrame = False
    RealWidth = 0
    RealHeight = 0
    ShowRollButton = False
    ShowCloseButton = True
    CaptionHeight = 24
    RollKind = scrkRollVertical
    RollUpState = False
    Moveable = True
    Sizeable = True
    HideControlsInRollUpState = True
    StorePaintBuffer = False
    Caption = 'Navr'#382'en'#233' pohony pro objekty bez p'#345'i'#345'azen'#253'ch pohon'#367
    OnClose = scExPanel_doporuc_pohonyClose
    object scHTMLLabel_doporuc_pohony: TscHTMLLabel
      Left = 3
      Top = 32
      Width = 221
      Height = 19
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 10114859
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      TabOrder = 0
      OnClick = scHTMLLabel_doporuc_pohonyClick
      AutoSizeHeight = True
      AutoSizeWidth = True
      AutoExecuteURL = True
      Caption = 'scHTMLLabel_doporuc_pohony'
      LinkFontNormal.Charset = DEFAULT_CHARSET
      LinkFontNormal.Color = clHighlight
      LinkFontNormal.Height = -11
      LinkFontNormal.Name = 'Tahoma'
      LinkFontNormal.Style = []
      LinkFontHover.Charset = DEFAULT_CHARSET
      LinkFontHover.Color = clHighlight
      LinkFontHover.Height = -11
      LinkFontHover.Name = 'Tahoma'
      LinkFontHover.Style = [fsUnderline]
      MaxWidth = 0
      TextAlignment = taLeftJustify
    end
    object scGPGlyphButton_add_mezi_pohony: TscGPGlyphButton
      Left = 419
      Top = 24
      Width = 37
      Height = 42
      Hint = 'P'#345'idat do seznamu pou'#382'iteln'#253'ch pohon'#367
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNone
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = scGPGlyphButton_add_mezi_pohonyClick
      Animation = True
      Caption = 'scGPGlyphButton_add_mezi_pohony'
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
      Options.NormalColor2 = clNone
      Options.HotColor2 = clNone
      Options.PressedColor2 = clNone
      Options.FocusedColor2 = clNone
      Options.DisabledColor2 = clNone
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 20
      Options.PressedColorAlpha = 30
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 5
      Options.NormalColor2Alpha = 255
      Options.HotColor2Alpha = 255
      Options.PressedColor2Alpha = 255
      Options.FocusedColor2Alpha = 255
      Options.DisabledColor2Alpha = 255
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
      Options.ShapeFillGradientColorOffset = 25
      Options.ShapeCornerRadius = 5
      Options.ShapeStyle = scgpRoundedRect
      Options.ArrowSize = 9
      Options.ArrowAreaSize = 0
      Options.ArrowType = scgpatDefault
      Options.ArrowThickness = 2
      Options.ArrowThicknessScaled = False
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
      GlyphOptions.Kind = scgpbgkView
      GlyphOptions.Thickness = 2
      GlyphOptions.ThicknessScaled = False
      GlyphOptions.Size = 0
      GlyphOptions.StyleColors = True
      TextMargin = -1
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      SplitButton = False
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
  object scGPButton_doporucene: TscGPButton
    Left = 848
    Top = 270
    Width = 45
    Height = 35
    Align = alCustom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 8
    OnClick = scGPButton_doporuceneClick
    Animation = False
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 35
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = clSilver
    Options.HotColor = clSilver
    Options.PressedColor = clSilver
    Options.FocusedColor = clRed
    Options.DisabledColor = clPurple
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 200
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clSilver
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 0
    Options.FramePressedColorAlpha = 200
    Options.FrameFocusedColorAlpha = 200
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clBlack
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 8
    Options.ShapeStyle = scgpRoundedRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scLabel_TT_change_hint: TscLabel
    Left = 831
    Top = 508
    Width = 118
    Height = 13
    TabOrder = 9
    Visible = False
    DragForm = False
    DragTopForm = True
    GlowEffect.Enabled = False
    GlowEffect.Color = clBtnShadow
    GlowEffect.AlphaValue = 255
    GlowEffect.GlowSize = 7
    GlowEffect.Offset = 0
    GlowEffect.Intensive = True
    GlowEffect.StyleColors = True
    AutoSize = True
    UseFontColorToStyleColor = False
    Caption = 'scLabel_TT_change_hint'
  end
  object Memo1: TMemo
    Left = 768
    Top = 508
    Width = 57
    Height = 30
    Lines.Strings = (
      'Memo1')
    TabOrder = 10
    Visible = False
  end
  object scGPButton_jig: TscGPButton
    Left = 789
    Top = 40
    Width = 105
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 11
    Visible = False
    Animation = False
    Caption = '  Jig'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 19
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = clSilver
    Options.HotColor = clPurple
    Options.PressedColor = clSilver
    Options.FocusedColor = clRed
    Options.DisabledColor = clPurple
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clGray
    Options.FrameFocusedColor = clSilver
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clBlack
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clAqua
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    HotImageIndex = -1
    FocusedImageIndex = -1
    PressedImageIndex = -1
    UseGalleryMenuImage = False
    UseGalleryMenuCaption = False
    ScaleMarginAndSpacing = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
    Down = True
    GroupIndex = 0
    AllowAllUp = False
  end
  object scGPSwitch: TscGPSwitch
    Left = 294
    Top = 242
    Width = 47
    Height = 22
    Margins.Left = 1
    Margins.Top = 1
    Margins.Right = 1
    Margins.Bottom = 1
    Align = alCustom
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
    TabOrder = 12
    Animation = True
    Color = clRed
    FrameColor = 33023
    FrameOnColor = 33023
    FramePressedColor = clRed
    State = scswOff
    StyleKind = scswsStyled
    ThumbColor = clWhite
    ThumbOnColor = clWhite
    ThumbPressedColor = clWhite
    FrameColorAlpha = 255
    FrameOnColorAlpha = 255
    FramePressedColorAlpha = 255
    ThumbColorAlpha = 255
    ThumbOnColorAlpha = 255
    ThumbPressedColorAlpha = 255
    ThumbShadow = False
    FrameSolid = True
    FrameOnSolid = True
    FrameInside = False
    OnChangeState = scGPSwitchChangeState
  end
  object scGPNumericEdit_delka_jig: TscGPNumericEdit
    Left = 112
    Top = 182
    Width = 100
    Height = 29
    Options.NormalColor = clWhite
    Options.HotColor = clWhite
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 150
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = 1871359
    Options.FrameFocusedColor = 10114859
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 50
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clBlack
    Options.FontFocusedColor = clBlack
    Options.FontDisabledColor = clGrayText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpessRect
    Options.ScaleFrameWidth = False
    Options.StyleColors = True
    ContentMarginLeft = 5
    ContentMarginRight = 5
    ContentMarginTop = 5
    ContentMarginBottom = 5
    HidePromptTextIfFocused = False
    PromptTextColor = clNone
    Transparent = True
    Increment = 1.000000000000000000
    SupportUpDownKeys = False
    SupportMouseWheel = False
    ValueType = scvtFloat
    MaxValue = 1000000.000000000000000000
    Value = 1.000000000000000000
    DisplayType = scedtNumeric
    LeftButton.Kind = scgpebCustom
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.ShowHint = False
    LeftButton.GlyphColor = clWindowText
    LeftButton.GlyphColorAlpha = 180
    LeftButton.GlyphColorHotAlpha = 240
    LeftButton.GlyphColorPressedAlpha = 150
    LeftButton.GlyphThickness = 2
    LeftButton.GlyphSize = 0
    LeftButton.Width = 0
    RightButton.Kind = scgpebCustom
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.ShowHint = False
    RightButton.GlyphColor = clWindowText
    RightButton.GlyphColorAlpha = 180
    RightButton.GlyphColorHotAlpha = 240
    RightButton.GlyphColorPressedAlpha = 150
    RightButton.GlyphThickness = 2
    RightButton.GlyphSize = 0
    RightButton.Width = 0
    BiDiMode = bdLeftToRight
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 13
    OnKeyDown = FormKeyDown
  end
  object scGPNumericEdit_sirka_jig: TscGPNumericEdit
    Left = 218
    Top = 182
    Width = 100
    Height = 29
    Options.NormalColor = clWhite
    Options.HotColor = clWhite
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 150
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = 1871359
    Options.FrameFocusedColor = 10114859
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 50
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clBlack
    Options.FontFocusedColor = clBlack
    Options.FontDisabledColor = clGrayText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpessRect
    Options.ScaleFrameWidth = False
    Options.StyleColors = True
    ContentMarginLeft = 5
    ContentMarginRight = 5
    ContentMarginTop = 5
    ContentMarginBottom = 5
    HidePromptTextIfFocused = False
    PromptTextColor = clNone
    Transparent = True
    Increment = 1.000000000000000000
    SupportUpDownKeys = False
    SupportMouseWheel = False
    ValueType = scvtFloat
    MaxValue = 1000000.000000000000000000
    Value = 1.000000000000000000
    DisplayType = scedtNumeric
    LeftButton.Kind = scgpebCustom
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.ShowHint = False
    LeftButton.GlyphColor = clWindowText
    LeftButton.GlyphColorAlpha = 180
    LeftButton.GlyphColorHotAlpha = 240
    LeftButton.GlyphColorPressedAlpha = 150
    LeftButton.GlyphThickness = 2
    LeftButton.GlyphSize = 0
    LeftButton.Width = 0
    RightButton.Kind = scgpebCustom
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.ShowHint = False
    RightButton.GlyphColor = clWindowText
    RightButton.GlyphColorAlpha = 180
    RightButton.GlyphColorHotAlpha = 240
    RightButton.GlyphColorPressedAlpha = 150
    RightButton.GlyphThickness = 2
    RightButton.GlyphSize = 0
    RightButton.Width = 0
    BiDiMode = bdLeftToRight
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 14
    OnKeyDown = FormKeyDown
  end
  object PopUPmenu: TscGPPanel
    Left = 850
    Top = 30
    Width = 211
    Height = 170
    TabOrder = 15
    BlurBackground = False
    BlurBackgroundAmount = 5
    CustomImageIndex = -1
    DragForm = False
    DragTopForm = True
    FrameSides = []
    FillGradientAngle = 90
    FillGradientBeginAlpha = 255
    FillGradientEndAlpha = 255
    FillGradientBeginColorOffset = 25
    FillGradientEndColorOffset = 25
    FrameWidth = 1
    FillColor = cl3DLight
    FillColorAlpha = 255
    FillColor2 = clNone
    FrameColor = clWhite
    FrameColorAlpha = 255
    FrameRadius = 0
    ShowCaption = False
    CaptionGlowEffect.Enabled = False
    CaptionGlowEffect.Color = cl3DLight
    CaptionGlowEffect.AlphaValue = 255
    CaptionGlowEffect.GlowSize = 7
    CaptionGlowEffect.Offset = 0
    CaptionGlowEffect.Intensive = True
    CaptionGlowEffect.StyleColors = True
    Color = cl3DLight
    TransparentBackground = True
    StorePaintBuffer = True
    Sizeable = False
    WallpaperIndex = -1
    object Item_zobrazit_parametry: TscGPPanel
      Left = 0
      Top = 34
      Width = 211
      Height = 34
      Align = alTop
      TabOrder = 0
      Visible = False
      BlurBackground = False
      BlurBackgroundAmount = 5
      CustomImageIndex = -1
      DragForm = False
      DragTopForm = True
      FrameSides = [gppfsBottom]
      FillGradientAngle = 90
      FillGradientBeginAlpha = 255
      FillGradientEndAlpha = 255
      FillGradientBeginColorOffset = 25
      FillGradientEndColorOffset = 25
      FrameWidth = 0
      FillColor = 15790320
      FillColorAlpha = 255
      FillColor2 = clNone
      FrameColor = clBtnShadow
      FrameColorAlpha = 255
      FrameRadius = 0
      ShowCaption = False
      CaptionGlowEffect.Enabled = False
      CaptionGlowEffect.Color = cl3DLight
      CaptionGlowEffect.AlphaValue = 255
      CaptionGlowEffect.GlowSize = 7
      CaptionGlowEffect.Offset = 0
      CaptionGlowEffect.Intensive = True
      CaptionGlowEffect.StyleColors = True
      Color = clBtnFace
      TransparentBackground = True
      StorePaintBuffer = True
      Sizeable = False
      WallpaperIndex = -1
      object scLabel_zobrazit_parametry: TscLabel
        Left = 37
        Top = 0
        Width = 174
        Height = 34
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 0
        DragForm = True
        DragTopForm = True
        GlowEffect.Enabled = False
        GlowEffect.Color = cl3DLight
        GlowEffect.AlphaValue = 255
        GlowEffect.GlowSize = 0
        GlowEffect.Offset = 2
        GlowEffect.Intensive = False
        GlowEffect.StyleColors = False
        AutoSize = True
        Layout = tlCenter
        VertAlignment = scvtaCenter
        UseFontColorToStyleColor = True
        Caption = '  Zobrazit parametry'
      end
      object GlyphButton_zobrazit_parametry: TscGPGlyphButton
        Left = 0
        Top = 0
        Width = 37
        Height = 34
        Align = alLeft
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNone
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 1
        Animation = False
        Caption = 'GlyphButton_zobrazit_parametry'
        CanFocused = False
        CustomDropDown = False
        Layout = blGlyphLeft
        TransparentBackground = False
        ColorValue = clRed
        Options.NormalColor = 10114859
        Options.HotColor = clBtnText
        Options.PressedColor = clBtnText
        Options.FocusedColor = clRed
        Options.DisabledColor = clBtnText
        Options.NormalColor2 = clNone
        Options.HotColor2 = clNone
        Options.PressedColor2 = clNone
        Options.FocusedColor2 = clNone
        Options.DisabledColor2 = clNone
        Options.NormalColorAlpha = 255
        Options.HotColorAlpha = 0
        Options.PressedColorAlpha = 30
        Options.FocusedColorAlpha = 255
        Options.DisabledColorAlpha = 5
        Options.NormalColor2Alpha = 255
        Options.HotColor2Alpha = 255
        Options.PressedColor2Alpha = 255
        Options.FocusedColor2Alpha = 255
        Options.DisabledColor2Alpha = 255
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
        Options.FontDisabledColor = clRed
        Options.ShapeFillGradientAngle = 90
        Options.ShapeFillGradientPressedAngle = -90
        Options.ShapeFillGradientColorOffset = 25
        Options.ShapeCornerRadius = 1
        Options.ShapeStyle = scgpRect
        Options.ArrowSize = 9
        Options.ArrowAreaSize = 0
        Options.ArrowType = scgpatDefault
        Options.ArrowThickness = 2
        Options.ArrowThicknessScaled = False
        Options.StyleColors = True
        GlyphOptions.NormalColor = clWhite
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
        GlyphOptions.ThicknessScaled = False
        GlyphOptions.Size = 0
        GlyphOptions.StyleColors = True
        TextMargin = -1
        WidthWithCaption = 0
        WidthWithoutCaption = 0
        SplitButton = False
        RepeatClick = False
        RepeatClickInterval = 100
        ShowGalleryMenuFromTop = False
        ShowGalleryMenuFromRight = False
        ShowMenuArrow = False
        ShowFocusRect = False
        Down = False
        GroupIndex = 0
        AllowAllUp = False
      end
    end
    object Item_smazat: TscGPPanel
      Left = 0
      Top = 68
      Width = 211
      Height = 34
      Align = alTop
      TabOrder = 3
      BlurBackground = False
      BlurBackgroundAmount = 5
      CustomImageIndex = -1
      DragForm = False
      DragTopForm = True
      FrameSides = [gppfsBottom]
      FillGradientAngle = 0
      FillGradientBeginAlpha = 255
      FillGradientEndAlpha = 255
      FillGradientBeginColorOffset = 25
      FillGradientEndColorOffset = 0
      FrameWidth = 0
      FillColor = 15790320
      FillColorAlpha = 255
      FillColor2 = cl3DLight
      FrameColor = clBtnShadow
      FrameColorAlpha = 255
      FrameRadius = 0
      ShowCaption = False
      CaptionGlowEffect.Enabled = False
      CaptionGlowEffect.Color = cl3DLight
      CaptionGlowEffect.AlphaValue = 255
      CaptionGlowEffect.GlowSize = 7
      CaptionGlowEffect.Offset = 0
      CaptionGlowEffect.Intensive = True
      CaptionGlowEffect.StyleColors = True
      Color = clBtnFace
      TransparentBackground = True
      StorePaintBuffer = True
      Sizeable = False
      WallpaperIndex = -1
      object GlyphButton_smazat: TscGPGlyphButton
        Left = 0
        Top = 0
        Width = 37
        Height = 34
        Align = alLeft
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNone
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 1
        OnClick = scLabel_smazatClick
        OnMouseEnter = GlyphButton_smazatMouseEnter
        OnMouseLeave = GlyphButton_smazatMouseLeave
        Animation = False
        Caption = 'GlyphButton_smazat'
        CanFocused = False
        CustomDropDown = False
        Layout = blGlyphLeft
        TransparentBackground = False
        ColorValue = clRed
        Options.NormalColor = 10114859
        Options.HotColor = clBtnText
        Options.PressedColor = clBtnText
        Options.FocusedColor = clBtnFace
        Options.DisabledColor = clBtnText
        Options.NormalColor2 = clNone
        Options.HotColor2 = clNone
        Options.PressedColor2 = clNone
        Options.FocusedColor2 = clNone
        Options.DisabledColor2 = clNone
        Options.NormalColorAlpha = 255
        Options.HotColorAlpha = 0
        Options.PressedColorAlpha = 30
        Options.FocusedColorAlpha = 255
        Options.DisabledColorAlpha = 5
        Options.NormalColor2Alpha = 255
        Options.HotColor2Alpha = 255
        Options.PressedColor2Alpha = 255
        Options.FocusedColor2Alpha = 255
        Options.DisabledColor2Alpha = 255
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
        Options.ShapeFillGradientColorOffset = 25
        Options.ShapeCornerRadius = 1
        Options.ShapeStyle = scgpRect
        Options.ArrowSize = 9
        Options.ArrowAreaSize = 0
        Options.ArrowType = scgpatDefault
        Options.ArrowThickness = 2
        Options.ArrowThicknessScaled = False
        Options.StyleColors = True
        GlyphOptions.NormalColor = clWhite
        GlyphOptions.HotColor = 16744448
        GlyphOptions.PressedColor = clBtnText
        GlyphOptions.FocusedColor = 16744448
        GlyphOptions.DisabledColor = 16744448
        GlyphOptions.NormalColorAlpha = 200
        GlyphOptions.HotColorAlpha = 255
        GlyphOptions.PressedColorAlpha = 255
        GlyphOptions.FocusedColorAlpha = 255
        GlyphOptions.DisabledColorAlpha = 150
        GlyphOptions.Kind = scgpbgkCancel
        GlyphOptions.Thickness = 2
        GlyphOptions.ThicknessScaled = False
        GlyphOptions.Size = 0
        GlyphOptions.StyleColors = True
        TextMargin = -1
        WidthWithCaption = 0
        WidthWithoutCaption = 0
        SplitButton = False
        RepeatClick = False
        RepeatClickInterval = 100
        ShowGalleryMenuFromTop = False
        ShowGalleryMenuFromRight = False
        ShowMenuArrow = False
        ShowFocusRect = False
        Down = False
        GroupIndex = 0
        AllowAllUp = False
      end
      object scLabel_smazat: TscLabel
        Left = 37
        Top = 0
        Width = 174
        Height = 34
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 0
        OnClick = scLabel_smazatClick
        OnMouseEnter = scLabel_smazatMouseEnter
        OnMouseLeave = scLabel_smazatMouseLeave
        DragForm = True
        DragTopForm = True
        GlowEffect.Enabled = False
        GlowEffect.Color = clBtnShadow
        GlowEffect.AlphaValue = 255
        GlowEffect.GlowSize = 7
        GlowEffect.Offset = 2
        GlowEffect.Intensive = True
        GlowEffect.StyleColors = True
        AutoSize = False
        Layout = tlCenter
        VertAlignment = scvtaCenter
        UseFontColorToStyleColor = True
        Caption = '  Smazat pohon'
      end
    end
    object Item_smazat_nepouzite: TscGPPanel
      Left = 0
      Top = 136
      Width = 211
      Height = 34
      Align = alBottom
      TabOrder = 2
      BlurBackground = False
      BlurBackgroundAmount = 5
      CustomImageIndex = -1
      DragForm = False
      DragTopForm = True
      FrameSides = []
      FillGradientAngle = 90
      FillGradientBeginAlpha = 255
      FillGradientEndAlpha = 255
      FillGradientBeginColorOffset = 25
      FillGradientEndColorOffset = 25
      FrameWidth = 1
      FillColor = 15790320
      FillColorAlpha = 255
      FillColor2 = clNone
      FrameColor = cl3DLight
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
      Caption = 'Item1'
      TransparentBackground = True
      StorePaintBuffer = True
      Sizeable = False
      WallpaperIndex = -1
      object scLabel_smazat_nepouzite: TscLabel
        Left = 37
        Top = 0
        Width = 174
        Height = 34
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 1
        OnClick = scLabel_smazat_nepouziteClick
        OnMouseEnter = scLabel_smazat_nepouziteMouseEnter
        OnMouseLeave = scLabel_smazat_nepouziteMouseLeave
        DragForm = True
        DragTopForm = True
        GlowEffect.Enabled = False
        GlowEffect.Color = clBtnShadow
        GlowEffect.AlphaValue = 255
        GlowEffect.GlowSize = 7
        GlowEffect.Offset = 2
        GlowEffect.Intensive = True
        GlowEffect.StyleColors = True
        AutoSize = False
        Layout = tlCenter
        VertAlignment = scvtaCenter
        UseFontColorToStyleColor = True
        Caption = '  Smazat nepou'#382'it'#233
      end
      object GlyphButton_smazat_nepouzite: TscGPGlyphButton
        Left = 0
        Top = 0
        Width = 37
        Height = 34
        Align = alLeft
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNone
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 0
        OnClick = scLabel_smazat_nepouziteClick
        OnMouseEnter = GlyphButton_smazat_nepouziteMouseEnter
        OnMouseLeave = GlyphButton_smazat_nepouziteMouseLeave
        Animation = False
        Caption = 'scGPGlyphButton1'
        CanFocused = False
        CustomDropDown = False
        Layout = blGlyphLeft
        TransparentBackground = False
        ColorValue = clRed
        Options.NormalColor = 10114859
        Options.HotColor = clBtnText
        Options.PressedColor = clBtnText
        Options.FocusedColor = clBtnFace
        Options.DisabledColor = clBtnText
        Options.NormalColor2 = clNone
        Options.HotColor2 = clNone
        Options.PressedColor2 = clNone
        Options.FocusedColor2 = clNone
        Options.DisabledColor2 = clNone
        Options.NormalColorAlpha = 255
        Options.HotColorAlpha = 0
        Options.PressedColorAlpha = 30
        Options.FocusedColorAlpha = 255
        Options.DisabledColorAlpha = 5
        Options.NormalColor2Alpha = 255
        Options.HotColor2Alpha = 255
        Options.PressedColor2Alpha = 255
        Options.FocusedColor2Alpha = 255
        Options.DisabledColor2Alpha = 255
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
        Options.ShapeFillGradientColorOffset = 25
        Options.ShapeCornerRadius = 1
        Options.ShapeStyle = scgpRect
        Options.ArrowSize = 9
        Options.ArrowAreaSize = 0
        Options.ArrowType = scgpatDefault
        Options.ArrowThickness = 2
        Options.ArrowThicknessScaled = False
        Options.StyleColors = True
        GlyphOptions.NormalColor = clWhite
        GlyphOptions.HotColor = 16744448
        GlyphOptions.PressedColor = clBtnText
        GlyphOptions.FocusedColor = 16744448
        GlyphOptions.DisabledColor = 16744448
        GlyphOptions.NormalColorAlpha = 200
        GlyphOptions.HotColorAlpha = 255
        GlyphOptions.PressedColorAlpha = 255
        GlyphOptions.FocusedColorAlpha = 255
        GlyphOptions.DisabledColorAlpha = 150
        GlyphOptions.Kind = scgpbgkTrash
        GlyphOptions.Thickness = 2
        GlyphOptions.ThicknessScaled = False
        GlyphOptions.Size = 0
        GlyphOptions.StyleColors = True
        TextMargin = -1
        WidthWithCaption = 0
        WidthWithoutCaption = 0
        SplitButton = False
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
    object Item_kopirovat: TscGPPanel
      Left = 0
      Top = 102
      Width = 211
      Height = 34
      Align = alTop
      TabOrder = 4
      BlurBackground = False
      BlurBackgroundAmount = 5
      CustomImageIndex = -1
      DragForm = False
      DragTopForm = True
      FrameSides = [gppfsBottom]
      FillGradientAngle = 90
      FillGradientBeginAlpha = 255
      FillGradientEndAlpha = 255
      FillGradientBeginColorOffset = 25
      FillGradientEndColorOffset = 25
      FrameWidth = 0
      FillColor = 15790320
      FillColorAlpha = 255
      FillColor2 = clNone
      FrameColor = clBtnShadow
      FrameColorAlpha = 255
      FrameRadius = 0
      ShowCaption = False
      CaptionGlowEffect.Enabled = False
      CaptionGlowEffect.Color = cl3DLight
      CaptionGlowEffect.AlphaValue = 255
      CaptionGlowEffect.GlowSize = 7
      CaptionGlowEffect.Offset = 0
      CaptionGlowEffect.Intensive = True
      CaptionGlowEffect.StyleColors = True
      Color = clBtnShadow
      TransparentBackground = True
      StorePaintBuffer = True
      Sizeable = False
      WallpaperIndex = -1
      object scLabel_kopirovat: TscLabel
        Left = 37
        Top = 0
        Width = 174
        Height = 34
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 0
        OnClick = scLabel_kopirovatClick
        OnMouseEnter = scLabel_kopirovatMouseEnter
        OnMouseLeave = scLabel_kopirovatMouseLeave
        DragForm = True
        DragTopForm = True
        GlowEffect.Enabled = False
        GlowEffect.Color = clBtnShadow
        GlowEffect.AlphaValue = 255
        GlowEffect.GlowSize = 7
        GlowEffect.Offset = 2
        GlowEffect.Intensive = True
        GlowEffect.StyleColors = True
        AutoSize = False
        Layout = tlCenter
        VertAlignment = scvtaCenter
        UseFontColorToStyleColor = True
        Caption = '  Kop'#237'rovat pohon'
      end
      object GlyphButton_kopirovat: TscGPGlyphButton
        Left = 0
        Top = 0
        Width = 37
        Height = 34
        Align = alLeft
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNone
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 1
        OnClick = scLabel_kopirovatClick
        OnMouseEnter = GlyphButton_kopirovatMouseEnter
        OnMouseLeave = GlyphButton_kopirovatMouseLeave
        Animation = False
        Caption = 'GlyphButton_smazat'
        CanFocused = False
        CustomDropDown = False
        Layout = blGlyphLeft
        TransparentBackground = False
        ColorValue = clRed
        Options.NormalColor = 10114859
        Options.HotColor = clBtnText
        Options.PressedColor = clBtnText
        Options.FocusedColor = clBtnFace
        Options.DisabledColor = clBtnText
        Options.NormalColor2 = clNone
        Options.HotColor2 = clNone
        Options.PressedColor2 = clNone
        Options.FocusedColor2 = clNone
        Options.DisabledColor2 = clNone
        Options.NormalColorAlpha = 255
        Options.HotColorAlpha = 0
        Options.PressedColorAlpha = 30
        Options.FocusedColorAlpha = 255
        Options.DisabledColorAlpha = 5
        Options.NormalColor2Alpha = 255
        Options.HotColor2Alpha = 255
        Options.PressedColor2Alpha = 255
        Options.FocusedColor2Alpha = 255
        Options.DisabledColor2Alpha = 255
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
        Options.ShapeFillGradientColorOffset = 25
        Options.ShapeCornerRadius = 1
        Options.ShapeStyle = scgpRect
        Options.ArrowSize = 9
        Options.ArrowAreaSize = 0
        Options.ArrowType = scgpatDefault
        Options.ArrowThickness = 2
        Options.ArrowThicknessScaled = False
        Options.StyleColors = True
        GlyphOptions.NormalColor = clWhite
        GlyphOptions.HotColor = 16744448
        GlyphOptions.PressedColor = clBtnText
        GlyphOptions.FocusedColor = 16744448
        GlyphOptions.DisabledColor = 16744448
        GlyphOptions.NormalColorAlpha = 200
        GlyphOptions.HotColorAlpha = 255
        GlyphOptions.PressedColorAlpha = 255
        GlyphOptions.FocusedColorAlpha = 255
        GlyphOptions.DisabledColorAlpha = 150
        GlyphOptions.Kind = scgpbgkCopy
        GlyphOptions.Thickness = 2
        GlyphOptions.ThicknessScaled = False
        GlyphOptions.Size = 0
        GlyphOptions.StyleColors = True
        TextMargin = -1
        WidthWithCaption = 0
        WidthWithoutCaption = 0
        SplitButton = False
        RepeatClick = False
        RepeatClickInterval = 100
        ShowGalleryMenuFromTop = False
        ShowGalleryMenuFromRight = False
        ShowMenuArrow = False
        ShowFocusRect = False
        Down = False
        GroupIndex = 0
        AllowAllUp = False
      end
    end
    object Item_nastavit_parametry: TscGPPanel
      Left = 0
      Top = 0
      Width = 211
      Height = 34
      Align = alTop
      TabOrder = 1
      Visible = False
      BlurBackground = False
      BlurBackgroundAmount = 5
      CustomImageIndex = -1
      DragForm = False
      DragTopForm = True
      FrameSides = [gppfsBottom]
      FillGradientAngle = 90
      FillGradientBeginAlpha = 255
      FillGradientEndAlpha = 255
      FillGradientBeginColorOffset = 25
      FillGradientEndColorOffset = 25
      FrameWidth = 0
      FillColor = 15790320
      FillColorAlpha = 255
      FillColor2 = clNone
      FrameColor = clWhite
      FrameColorAlpha = 255
      FrameRadius = 0
      ShowCaption = False
      CaptionGlowEffect.Enabled = False
      CaptionGlowEffect.Color = cl3DLight
      CaptionGlowEffect.AlphaValue = 255
      CaptionGlowEffect.GlowSize = 7
      CaptionGlowEffect.Offset = 0
      CaptionGlowEffect.Intensive = True
      CaptionGlowEffect.StyleColors = True
      Color = clBtnFace
      Caption = 'Item1'
      TransparentBackground = True
      StorePaintBuffer = True
      Sizeable = False
      WallpaperIndex = -1
      object scLabel_nastavit_parametry: TscLabel
        Left = 37
        Top = 0
        Width = 174
        Height = 34
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alClient
        BiDiMode = bdLeftToRight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGray
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentBiDiMode = False
        ParentFont = False
        TabOrder = 0
        DragForm = True
        DragTopForm = True
        GlowEffect.Enabled = False
        GlowEffect.Color = cl3DLight
        GlowEffect.AlphaValue = 255
        GlowEffect.GlowSize = 0
        GlowEffect.Offset = 0
        GlowEffect.Intensive = False
        GlowEffect.StyleColors = False
        AutoSize = False
        Layout = tlCenter
        VertAlignment = scvtaCenter
        UseFontColorToStyleColor = True
        Caption = '  Nastavit parametry'
      end
      object GlyphButton_nastavit_parametry: TscGPGlyphButton
        Left = 0
        Top = 0
        Width = 37
        Height = 34
        Align = alLeft
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNone
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Font.Quality = fqClearType
        ParentFont = False
        TabOrder = 1
        Animation = False
        Caption = 'GlyphButton_zobrazit_parametry'
        CanFocused = False
        CustomDropDown = False
        Layout = blGlyphLeft
        TransparentBackground = False
        ColorValue = clRed
        Options.NormalColor = 10114859
        Options.HotColor = clBtnText
        Options.PressedColor = clBtnText
        Options.FocusedColor = clRed
        Options.DisabledColor = clBtnText
        Options.NormalColor2 = clNone
        Options.HotColor2 = clNone
        Options.PressedColor2 = clNone
        Options.FocusedColor2 = clNone
        Options.DisabledColor2 = clNone
        Options.NormalColorAlpha = 255
        Options.HotColorAlpha = 0
        Options.PressedColorAlpha = 30
        Options.FocusedColorAlpha = 255
        Options.DisabledColorAlpha = 5
        Options.NormalColor2Alpha = 255
        Options.HotColor2Alpha = 255
        Options.PressedColor2Alpha = 255
        Options.FocusedColor2Alpha = 255
        Options.DisabledColor2Alpha = 255
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
        Options.ShapeFillGradientColorOffset = 25
        Options.ShapeCornerRadius = 1
        Options.ShapeStyle = scgpRect
        Options.ArrowSize = 9
        Options.ArrowAreaSize = 0
        Options.ArrowType = scgpatDefault
        Options.ArrowThickness = 2
        Options.ArrowThicknessScaled = False
        Options.StyleColors = True
        GlyphOptions.NormalColor = clWhite
        GlyphOptions.HotColor = 16744448
        GlyphOptions.PressedColor = clBtnText
        GlyphOptions.FocusedColor = 16744448
        GlyphOptions.DisabledColor = 16744448
        GlyphOptions.NormalColorAlpha = 200
        GlyphOptions.HotColorAlpha = 255
        GlyphOptions.PressedColorAlpha = 255
        GlyphOptions.FocusedColorAlpha = 255
        GlyphOptions.DisabledColorAlpha = 150
        GlyphOptions.Kind = scgpbgkGear
        GlyphOptions.Thickness = 2
        GlyphOptions.ThicknessScaled = False
        GlyphOptions.Size = 0
        GlyphOptions.StyleColors = True
        TextMargin = -1
        WidthWithCaption = 0
        WidthWithoutCaption = 0
        SplitButton = False
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
    object GlyphButton_close: TscGPGlyphButton
      AlignWithMargins = True
      Left = 191
      Top = 0
      Width = 20
      Height = 24
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alCustom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNone
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      TabOrder = 5
      StyleElements = [seFont, seClient]
      OnClick = GlyphButton_closeClick
      ArrowPosition = scapBottom
      Animation = False
      Caption = 'GlyphButton_close'
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphRight
      TransparentBackground = True
      ColorValue = clRed
      Cancel = True
      Options.NormalColor = cl3DLight
      Options.HotColor = cl3DLight
      Options.PressedColor = clRed
      Options.FocusedColor = clRed
      Options.DisabledColor = clBtnText
      Options.NormalColor2 = clNone
      Options.HotColor2 = clNone
      Options.PressedColor2 = clNone
      Options.FocusedColor2 = clNone
      Options.DisabledColor2 = clNone
      Options.NormalColorAlpha = 255
      Options.HotColorAlpha = 255
      Options.PressedColorAlpha = 180
      Options.FocusedColorAlpha = 220
      Options.DisabledColorAlpha = 0
      Options.NormalColor2Alpha = 255
      Options.HotColor2Alpha = 255
      Options.PressedColor2Alpha = 255
      Options.FocusedColor2Alpha = 255
      Options.DisabledColor2Alpha = 255
      Options.FrameNormalColor = cl3DLight
      Options.FrameHotColor = clRed
      Options.FramePressedColor = clRed
      Options.FrameFocusedColor = cl3DLight
      Options.FrameDisabledColor = clBtnText
      Options.FrameWidth = 2
      Options.FrameNormalColorAlpha = 255
      Options.FrameHotColorAlpha = 220
      Options.FramePressedColorAlpha = 220
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 0
      Options.FontNormalColor = clBtnText
      Options.FontHotColor = clBtnText
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clBtnText
      Options.FontDisabledColor = cl3DLight
      Options.ShapeFillGradientAngle = 90
      Options.ShapeFillGradientPressedAngle = -90
      Options.ShapeFillGradientColorOffset = 25
      Options.ShapeCornerRadius = 1
      Options.ShapeStyle = scgpRect
      Options.ArrowSize = 9
      Options.ArrowAreaSize = 0
      Options.ArrowType = scgpatDefault
      Options.ArrowThickness = 2
      Options.ArrowThicknessScaled = False
      Options.StyleColors = True
      GlyphOptions.NormalColor = clWhite
      GlyphOptions.HotColor = clWhite
      GlyphOptions.PressedColor = clWhite
      GlyphOptions.FocusedColor = clWhite
      GlyphOptions.DisabledColor = clWhite
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 150
      GlyphOptions.Kind = scgpbgkCancel
      GlyphOptions.Thickness = 2
      GlyphOptions.ThicknessScaled = False
      GlyphOptions.Size = 0
      GlyphOptions.StyleColors = True
      TextMargin = -1
      ScaleFrameWidth = False
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      SplitButton = False
      RepeatClick = False
      RepeatClickInterval = 100
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = False
      Down = False
      GroupIndex = 0
      AllowAllUp = True
    end
  end
  object scGPGlyphButton_ADD_old: TscGPGlyphButton
    Left = 364
    Top = 275
    Width = 37
    Height = 34
    Hint = 'P'#345'idat nov'#253' pohon'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 16
    Visible = False
    OnClick = Button_ADD_Click
    Animation = True
    Caption = 'Info'
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
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 0
    Options.HotColorAlpha = 20
    Options.PressedColorAlpha = 30
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 5
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
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
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 5
    Options.ShapeStyle = scgpRoundedRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
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
    GlyphOptions.Kind = scgpbgkPlus
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPGlyphButton_DEL_nepouzite: TscGPGlyphButton
    Left = 1041
    Top = 608
    Width = 39
    Height = 34
    Hint = 'Smazat nepou'#382#237'van'#233' pohony'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 17
    Visible = False
    OnClick = scLabel_smazat_nepouziteClick
    Animation = True
    Caption = 'Info'
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
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 0
    Options.HotColorAlpha = 20
    Options.PressedColorAlpha = 30
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 5
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
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
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 5
    Options.ShapeStyle = scgpRoundedRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
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
    GlyphOptions.Kind = scgpbgkTrash
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPGlyphButton_takttime_kalkulator: TscGPGlyphButton
    Left = 279
    Top = 40
    Width = 39
    Height = 34
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 18
    Visible = False
    Animation = False
    Caption = 'GlyphButton_zobrazit_parametry'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = False
    ColorValue = clRed
    Options.NormalColor = 10114859
    Options.HotColor = clBtnText
    Options.PressedColor = clBtnText
    Options.FocusedColor = clRed
    Options.DisabledColor = clBtnText
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 0
    Options.PressedColorAlpha = 30
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 5
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
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
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 1
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    GlyphOptions.NormalColor = clWhite
    GlyphOptions.HotColor = 16744448
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = 16744448
    GlyphOptions.DisabledColor = 16744448
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkCalendar
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPNumericEdit_delka_podvozek: TscGPNumericEdit
    Left = 111
    Top = 239
    Width = 100
    Height = 29
    Options.NormalColor = clWhite
    Options.HotColor = clWhite
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 150
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = 1871359
    Options.FrameFocusedColor = 10114859
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 50
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clBlack
    Options.FontFocusedColor = clBlack
    Options.FontDisabledColor = clGrayText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpessRect
    Options.ScaleFrameWidth = False
    Options.StyleColors = True
    ContentMarginLeft = 5
    ContentMarginRight = 5
    ContentMarginTop = 5
    ContentMarginBottom = 5
    HidePromptTextIfFocused = False
    PromptTextColor = clNone
    Transparent = True
    Increment = 1.000000000000000000
    SupportUpDownKeys = False
    SupportMouseWheel = False
    ValueType = scvtFloat
    MaxValue = 1000000.000000000000000000
    Value = 1.000000000000000000
    DisplayType = scedtNumeric
    LeftButton.Kind = scgpebCustom
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.ShowHint = False
    LeftButton.GlyphColor = clWindowText
    LeftButton.GlyphColorAlpha = 180
    LeftButton.GlyphColorHotAlpha = 240
    LeftButton.GlyphColorPressedAlpha = 150
    LeftButton.GlyphThickness = 2
    LeftButton.GlyphSize = 0
    LeftButton.Width = 0
    RightButton.Kind = scgpebCustom
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.ShowHint = False
    RightButton.GlyphColor = clWindowText
    RightButton.GlyphColorAlpha = 180
    RightButton.GlyphColorHotAlpha = 240
    RightButton.GlyphColorPressedAlpha = 150
    RightButton.GlyphThickness = 2
    RightButton.GlyphSize = 0
    RightButton.Width = 0
    BiDiMode = bdLeftToRight
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 19
    OnKeyDown = FormKeyDown
  end
  object scGPNumericEdit_vyska_jig: TscGPNumericEdit
    Left = 324
    Top = 182
    Width = 100
    Height = 29
    Options.NormalColor = clWhite
    Options.HotColor = clWhite
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 150
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = 1871359
    Options.FrameFocusedColor = 10114859
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 50
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clBlack
    Options.FontFocusedColor = clBlack
    Options.FontDisabledColor = clGrayText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpessRect
    Options.ScaleFrameWidth = False
    Options.StyleColors = True
    ContentMarginLeft = 5
    ContentMarginRight = 5
    ContentMarginTop = 5
    ContentMarginBottom = 5
    HidePromptTextIfFocused = False
    PromptTextColor = clNone
    Transparent = True
    Increment = 1.000000000000000000
    SupportUpDownKeys = False
    SupportMouseWheel = False
    ValueType = scvtFloat
    MaxValue = 1000000.000000000000000000
    Value = 1.000000000000000000
    DisplayType = scedtNumeric
    LeftButton.Kind = scgpebCustom
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.ShowHint = False
    LeftButton.GlyphColor = clWindowText
    LeftButton.GlyphColorAlpha = 180
    LeftButton.GlyphColorHotAlpha = 240
    LeftButton.GlyphColorPressedAlpha = 150
    LeftButton.GlyphThickness = 2
    LeftButton.GlyphSize = 0
    LeftButton.Width = 0
    RightButton.Kind = scgpebCustom
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.ShowHint = False
    RightButton.GlyphColor = clWindowText
    RightButton.GlyphColorAlpha = 180
    RightButton.GlyphColorHotAlpha = 240
    RightButton.GlyphColorPressedAlpha = 150
    RightButton.GlyphThickness = 2
    RightButton.GlyphSize = 0
    RightButton.Width = 0
    BiDiMode = bdLeftToRight
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 20
    Visible = False
    OnKeyDown = FormKeyDown
  end
  object scGPGlyphButton_TT: TscGPGlyphButton
    Left = 451
    Top = 52
    Width = 31
    Height = 29
    Hint = 'nastavit TaktTime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 21
    Visible = False
    OnClick = scGPGlyphButton_TTClick
    Animation = False
    Caption = 'GlyphButton_zobrazit_parametry'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = False
    ColorValue = clRed
    Options.NormalColor = 10114859
    Options.HotColor = 10114859
    Options.PressedColor = 10114859
    Options.FocusedColor = clRed
    Options.DisabledColor = cl3DDkShadow
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 200
    Options.PressedColorAlpha = 200
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clBtnText
    Options.FrameHotColor = clBtnText
    Options.FramePressedColor = 10114859
    Options.FrameFocusedColor = clBtnText
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 0
    Options.FrameHotColorAlpha = 0
    Options.FramePressedColorAlpha = 200
    Options.FrameFocusedColorAlpha = 0
    Options.FrameDisabledColorAlpha = 0
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = clBtnShadow
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 1
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    GlyphOptions.NormalColor = clWhite
    GlyphOptions.HotColor = clWhite
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = 16744448
    GlyphOptions.DisabledColor = 16744448
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkEdit
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPGlyphButton_vozik_edit: TscGPGlyphButton
    Left = 491
    Top = 52
    Width = 31
    Height = 29
    Hint = 'nastavit jig/podvozek'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 22
    Visible = False
    OnClick = sc
    Animation = False
    Caption = 'GlyphButton_zobrazit_parametry'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = False
    ColorValue = clRed
    Options.NormalColor = 10114859
    Options.HotColor = 10114859
    Options.PressedColor = 10114859
    Options.FocusedColor = clRed
    Options.DisabledColor = cl3DDkShadow
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 200
    Options.PressedColorAlpha = 200
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clBtnText
    Options.FrameHotColor = clBtnText
    Options.FramePressedColor = 10114859
    Options.FrameFocusedColor = clBtnText
    Options.FrameDisabledColor = clBlack
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 0
    Options.FrameHotColorAlpha = 0
    Options.FramePressedColorAlpha = 200
    Options.FrameFocusedColorAlpha = 0
    Options.FrameDisabledColorAlpha = 0
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = clBtnText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 1
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    GlyphOptions.NormalColor = clWhite
    GlyphOptions.HotColor = clWhite
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = 16744448
    GlyphOptions.DisabledColor = 16744448
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkEdit
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPGlyphButton_ADD: TscGPGlyphButton
    Left = 2
    Top = 474
    Width = 30
    Height = 29
    Hint = 'p'#345'idat nov'#253' pohon'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 23
    OnClick = Button_ADD_Click
    Animation = False
    Caption = 'GlyphButton_zobrazit_parametry'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = False
    ColorValue = clRed
    Options.NormalColor = 10114859
    Options.HotColor = 10114859
    Options.PressedColor = 10114859
    Options.FocusedColor = clRed
    Options.DisabledColor = cl3DDkShadow
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 200
    Options.PressedColorAlpha = 200
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clBtnText
    Options.FrameHotColor = clBtnText
    Options.FramePressedColor = 10114859
    Options.FrameFocusedColor = clBtnText
    Options.FrameDisabledColor = clBlack
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 0
    Options.FrameHotColorAlpha = 0
    Options.FramePressedColorAlpha = 200
    Options.FrameFocusedColorAlpha = 0
    Options.FrameDisabledColorAlpha = 0
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = clBtnText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 1
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    GlyphOptions.NormalColor = clWhite
    GlyphOptions.HotColor = clWhite
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = 16744448
    GlyphOptions.DisabledColor = 16744448
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkPlus
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
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
  object scGPGlyphButton1: TscGPGlyphButton
    Left = 340
    Top = 46
    Width = 30
    Height = 30
    Align = alCustom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 24
    Visible = False
    OnClick = scLabel_smazatClick
    OnMouseEnter = GlyphButton_smazatMouseEnter
    OnMouseLeave = GlyphButton_smazatMouseLeave
    Animation = False
    Caption = 'GlyphButton_smazat'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = False
    ColorValue = clRed
    Options.NormalColor = 10114859
    Options.HotColor = clBtnText
    Options.PressedColor = clBtnText
    Options.FocusedColor = clBtnFace
    Options.DisabledColor = clBtnText
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 0
    Options.PressedColorAlpha = 30
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 5
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
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
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 2
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    GlyphOptions.NormalColor = clWhite
    GlyphOptions.HotColor = 16744448
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = 16744448
    GlyphOptions.DisabledColor = 16744448
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkCancel
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
    RepeatClick = False
    RepeatClickInterval = 100
    ShowGalleryMenuFromTop = False
    ShowGalleryMenuFromRight = False
    ShowMenuArrow = False
    ShowFocusRect = False
    Down = False
    GroupIndex = 0
    AllowAllUp = False
  end
  object scGPGlyphButton_smazat_pohon: TscGPGlyphButton
    AlignWithMargins = True
    Left = 392
    Top = 52
    Width = 20
    Height = 24
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Align = alCustom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 25
    Visible = False
    StyleElements = [seFont, seClient]
    OnClick = GlyphButton_closeClick
    ArrowPosition = scapBottom
    Animation = False
    Caption = 'GlyphButton_close'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphRight
    TransparentBackground = True
    ColorValue = clRed
    Cancel = True
    Options.NormalColor = cl3DLight
    Options.HotColor = cl3DLight
    Options.PressedColor = clRed
    Options.FocusedColor = clRed
    Options.DisabledColor = clBtnText
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 180
    Options.FocusedColorAlpha = 220
    Options.DisabledColorAlpha = 0
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = cl3DLight
    Options.FrameHotColor = clRed
    Options.FramePressedColor = clRed
    Options.FrameFocusedColor = cl3DLight
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 220
    Options.FramePressedColorAlpha = 220
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 0
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = cl3DLight
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeFillGradientColorOffset = 25
    Options.ShapeCornerRadius = 1
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.ArrowAreaSize = 0
    Options.ArrowType = scgpatDefault
    Options.ArrowThickness = 2
    Options.ArrowThicknessScaled = False
    Options.StyleColors = True
    GlyphOptions.NormalColor = clWhite
    GlyphOptions.HotColor = clWhite
    GlyphOptions.PressedColor = clWhite
    GlyphOptions.FocusedColor = clWhite
    GlyphOptions.DisabledColor = clWhite
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkCancel
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    ScaleFrameWidth = False
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    SplitButton = False
    RepeatClick = False
    RepeatClickInterval = 100
    ShowGalleryMenuFromTop = False
    ShowGalleryMenuFromRight = False
    ShowMenuArrow = False
    ShowFocusRect = False
    Down = False
    GroupIndex = 0
    AllowAllUp = True
  end
  object rEditNum_takt: TscGPNumericEdit
    Left = 112
    Top = 75
    Width = 100
    Height = 29
    Options.NormalColor = clWhite
    Options.HotColor = clWhite
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 150
    Options.FrameNormalColor = clGray
    Options.FrameHotColor = 1871359
    Options.FrameFocusedColor = 10114859
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 50
    Options.FontNormalColor = clBlack
    Options.FontHotColor = clBlack
    Options.FontFocusedColor = clBlack
    Options.FontDisabledColor = clGrayText
    Options.ShapeFillGradientAngle = 90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpessRect
    Options.ScaleFrameWidth = False
    Options.StyleColors = True
    ContentMarginLeft = 5
    ContentMarginRight = 5
    ContentMarginTop = 5
    ContentMarginBottom = 5
    HidePromptTextIfFocused = False
    PromptTextColor = clNone
    Transparent = True
    Increment = 1.000000000000000000
    SupportUpDownKeys = False
    SupportMouseWheel = False
    ValueType = scvtFloat
    MaxValue = 1000000.000000000000000000
    Value = 1.000000000000000000
    DisplayType = scedtNumeric
    LeftButton.Kind = scgpebCustom
    LeftButton.Enabled = True
    LeftButton.Visible = False
    LeftButton.ImageIndex = -1
    LeftButton.ImageHotIndex = -1
    LeftButton.ImagePressedIndex = -1
    LeftButton.RepeatClick = False
    LeftButton.RepeatClickInterval = 200
    LeftButton.ShowHint = False
    LeftButton.GlyphColor = clWindowText
    LeftButton.GlyphColorAlpha = 180
    LeftButton.GlyphColorHotAlpha = 240
    LeftButton.GlyphColorPressedAlpha = 150
    LeftButton.GlyphThickness = 2
    LeftButton.GlyphSize = 0
    LeftButton.Width = 0
    RightButton.Kind = scgpebCustom
    RightButton.Enabled = True
    RightButton.Visible = False
    RightButton.ImageIndex = -1
    RightButton.ImageHotIndex = -1
    RightButton.ImagePressedIndex = -1
    RightButton.RepeatClick = False
    RightButton.RepeatClickInterval = 200
    RightButton.ShowHint = False
    RightButton.GlyphColor = clWindowText
    RightButton.GlyphColorAlpha = 180
    RightButton.GlyphColorHotAlpha = 240
    RightButton.GlyphColorPressedAlpha = 150
    RightButton.GlyphThickness = 2
    RightButton.GlyphSize = 0
    RightButton.Width = 0
    BiDiMode = bdLeftToRight
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 26
    OnKeyDown = FormKeyDown
  end
  object scGPComboBox1: TscGPComboBox
    Left = 528
    Top = 132
    Width = 247
    Height = 30
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 27
    Visible = False
    ContentMargin = 0
    AutoComplete = False
    Animation = False
    CheckedListMode = False
    CheckedListWrap = True
    Images = Form1.scGPVirtualImageList1
    Items = <
      item
        Header = False
        Enabled = True
        Caption = 'Ahoj'
        ImageIndex = 36
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'Nazdar'
        ImageIndex = 37
        Checked = False
      end
      item
        Header = True
        Enabled = True
        Caption = 'Bazar'
        ImageIndex = 38
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = #268'est'
        ImageIndex = 39
        Checked = False
      end
      item
        Header = True
        Enabled = True
        Caption = 'Hello'
        ImageIndex = 40
        Checked = True
      end
      item
        Header = False
        Enabled = True
        Caption = 'Kuku'
        ImageIndex = 41
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'Huku'
        ImageIndex = 42
        Checked = False
      end
      item
        Header = True
        Enabled = True
        Caption = 'ELTEP'
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'Rosta'
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'Martin'
        Checked = False
      end>
    ItemIndex = -1
    ItemWordWrap = True
    ItemShowEllipsis = False
    DropDownCount = 7
    Options.NormalColor = clWhite
    Options.HotColor = clWhite
    Options.PressedColor = clWhite
    Options.FocusedColor = clWhite
    Options.DisabledColor = clBtnFace
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.FrameNormalColor = clBlack
    Options.FrameHotColor = clBlack
    Options.FramePressedColor = clBlack
    Options.FrameFocusedColor = clBtnText
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = clBtnShadow
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpcssRect
    Options.ScaleFrameWidth = False
    Options.ArrowSize = 9
    Options.ArrowAreaWidth = 0
    Options.StyleColors = True
    ShowItemImage = True
    ShowItemText = True
    ShowItemDetail = False
    ListBoxHeaderFont.Charset = DEFAULT_CHARSET
    ListBoxHeaderFont.Color = clWindowText
    ListBoxHeaderFont.Height = -17
    ListBoxHeaderFont.Name = 'Tahoma'
    ListBoxHeaderFont.Style = []
    ListBoxHeaderOptions.Color = clSkyBlue
    ListBoxHeaderOptions.ColorAlpha = 220
    ListBoxHeaderOptions.FillStyle = scgpsfColor
    ListBoxHeaderOptions.GradientAngle = 90
    ListBoxHeaderOptions.Margin = 5
    DetailFont.Charset = DEFAULT_CHARSET
    DetailFont.Color = clRed
    DetailFont.Height = -16
    DetailFont.Name = 'Tahoma'
    DetailFont.Style = []
    ListBoxLineColor = clWindowText
    ListBoxWidth = 0
    ListBoxHeight = 0
    ListBoxShowItemDetails = False
    ListBoxShowLines = False
    ListBoxItemHeight = 25
    ListBoxHeaderHeight = 28
    ListBoxItemSpacing = 5
    ListBoxItemMargin = 5
    ListBoxIndentMargin = 5
    ListBoxScrollBarOptions.BorderWidth = 0
    ListBoxScrollBarOptions.FillColor = clSilver
    ListBoxScrollBarOptions.FillColorAlpha = 100
    ListBoxScrollBarOptions.ThumbColor = clWindowText
    ListBoxScrollBarOptions.ThumbColorAlpha = 50
    ListBoxScrollBarOptions.ThumbColorHotAlpha = 100
    ListBoxScrollBarOptions.ThumbColorPressedAlpha = 150
    ListBoxScrollBarOptions.ThumbRounded = False
    ListBoxScrollBarOptions.Position = scgpsbDefault
    ListBoxScrollBarOptions.Size = 11
    ListBoxOptions.FillColor = clWhite
    ListBoxOptions.FillColor2 = clOlive
    ListBoxOptions.FillColorAlpha = 255
    ListBoxOptions.FillColor2Alpha = 255
    ListBoxOptions.FillGradientAngle = 90
    ListBoxOptions.FrameColor = clWhite
    ListBoxOptions.FrameColorAlpha = 255
    ListBoxOptions.FrameWidth = 1
    ListBoxOptions.FrameScaleWidth = False
    MouseWheelSupport = True
    Color = clYellow
  end
  object scGPComboBox2: TscGPComboBox
    Left = 520
    Top = 176
    Width = 217
    Height = 41
    TabOrder = 28
    Visible = False
    ContentMargin = 0
    AutoComplete = False
    Animation = False
    CheckedListMode = False
    CheckedListWrap = True
    Items = <
      item
        Header = False
        Enabled = True
        Caption = 'kuk'
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'kuku'
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'kukukukukuk'
        Checked = False
      end
      item
        Header = False
        Enabled = True
        Caption = 'kukukukukukukukukukuk'
        Checked = False
      end>
    ItemIndex = -1
    ItemWordWrap = True
    ItemShowEllipsis = False
    DropDownCount = 7
    Options.NormalColor = clBtnFace
    Options.HotColor = clBtnFace
    Options.PressedColor = clWhite
    Options.FocusedColor = clBtnFace
    Options.DisabledColor = clBtnFace
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.FrameNormalColor = clBtnShadow
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clHighlight
    Options.FrameFocusedColor = clHighlight
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clBtnText
    Options.FontHotColor = clBtnText
    Options.FontPressedColor = clBtnText
    Options.FontFocusedColor = clBtnText
    Options.FontDisabledColor = clBtnShadow
    Options.ShapeFillGradientAngle = 90
    Options.ShapeFillGradientPressedAngle = -90
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpcssRect
    Options.ScaleFrameWidth = False
    Options.ArrowSize = 9
    Options.ArrowAreaWidth = 0
    Options.StyleColors = True
    ShowItemImage = True
    ShowItemText = True
    ShowItemDetail = False
    ListBoxHeaderFont.Charset = DEFAULT_CHARSET
    ListBoxHeaderFont.Color = clWindowText
    ListBoxHeaderFont.Height = -11
    ListBoxHeaderFont.Name = 'Tahoma'
    ListBoxHeaderFont.Style = [fsBold]
    ListBoxHeaderOptions.Color = clBtnFace
    ListBoxHeaderOptions.ColorAlpha = 220
    ListBoxHeaderOptions.FillStyle = scgpsfColor
    ListBoxHeaderOptions.GradientAngle = 90
    ListBoxHeaderOptions.Margin = 5
    DetailFont.Charset = DEFAULT_CHARSET
    DetailFont.Color = clWindowText
    DetailFont.Height = -11
    DetailFont.Name = 'Tahoma'
    DetailFont.Style = []
    ListBoxLineColor = clWindowText
    ListBoxWidth = 0
    ListBoxHeight = 0
    ListBoxShowItemDetails = False
    ListBoxShowLines = False
    ListBoxItemHeight = 30
    ListBoxHeaderHeight = 20
    ListBoxItemSpacing = 5
    ListBoxItemMargin = 5
    ListBoxIndentMargin = 10
    ListBoxScrollBarOptions.BorderWidth = 0
    ListBoxScrollBarOptions.FillColor = clBtnFace
    ListBoxScrollBarOptions.FillColorAlpha = 100
    ListBoxScrollBarOptions.ThumbColor = clWindowText
    ListBoxScrollBarOptions.ThumbColorAlpha = 50
    ListBoxScrollBarOptions.ThumbColorHotAlpha = 100
    ListBoxScrollBarOptions.ThumbColorPressedAlpha = 150
    ListBoxScrollBarOptions.ThumbRounded = False
    ListBoxScrollBarOptions.Position = scgpsbDefault
    ListBoxScrollBarOptions.Size = 11
    ListBoxOptions.FillColor = clWindow
    ListBoxOptions.FillColor2 = clWindow
    ListBoxOptions.FillColorAlpha = 255
    ListBoxOptions.FillColor2Alpha = 255
    ListBoxOptions.FillGradientAngle = 90
    ListBoxOptions.FrameColor = clBtnShadow
    ListBoxOptions.FrameColorAlpha = 255
    ListBoxOptions.FrameWidth = 1
    ListBoxOptions.FrameScaleWidth = False
    MouseWheelSupport = True
  end
  object Button1: TButton
    Left = 137
    Top = 294
    Width = 88
    Height = 25
    Caption = 'Add pohon - test'
    TabOrder = 29
    Visible = False
    OnClick = Button1Click
  end
  object scGPImage1: TscGPImage
    Left = 430
    Top = 40
    Width = 307
    Height = 250
    TabOrder = 30
    CustomImageIndex = -1
    DragForm = False
    DragTopForm = True
    StyleKind = scpsPanel
    ShowCaption = False
    BorderStyle = scpbsNone
    WallpaperIndex = -1
    LightBorderColor = clBtnHighlight
    ShadowBorderColor = clBtnShadow
    CaptionGlowEffect.Enabled = False
    CaptionGlowEffect.Color = clBtnShadow
    CaptionGlowEffect.AlphaValue = 255
    CaptionGlowEffect.GlowSize = 7
    CaptionGlowEffect.Offset = 0
    CaptionGlowEffect.Intensive = True
    CaptionGlowEffect.StyleColors = True
    Color = clBtnFace
    Caption = 'scGPImage1'
    StorePaintBuffer = False
    ImageIndex = -1
    InterpolationMode = scgppimDefault
    ClipFrame = scgpcfNone
    ClipFrameRadius = 10
    ClipFrameFillColor = clWindow
    ClipFrameFillColorAlpha = 255
    ClipFrameColor = clBtnFace
    ClipFrameWidth = 2
    RotationAngle = 0
    AnimationAcceleration = False
    RotateAnimation = False
    Picture.Data = {
      0954574943496D61676549492A00A6330000803C204FF004160D0784426150B8
      64361D0F8844625138A4562D178C466351B8E4763D1F904864523924964D2794
      4A6552B964B65D2F984C665338BC09E1049A4E6753B9E4F67D3FA05068543A25
      168D47A4526954B8E4DA714CA8546A553AA556AD57AC566B55BAE5767B4EAF58
      6C563B2596CD67B45A6D56BB6476C16DB85C6E573BA5D6ED77BC5E6A76FBD5F6
      FD7FC06070583C2616997CC36262EA9C60010B8F0002F258ACA6572D97AABF33
      40011674018C54E62ED88D168945A7001BB55A5D66B75DAF9F9CB64003F6D761
      69D26DF04B7DE000B3BFDD70785C3E247103C7001BF95C5B16E799744CF44007
      7EA44097D7003DFB5CFEE777BD6B09F8400ABF24538E81E4F2FBF59808803C20
      4FF004160D0784426150B864361D0F8844625138A4562D178C4416B1B0016E3D
      10454840075924664D27944A6552B964B65D2F984C665339A4D66D379C4D9413
      B001867D1440D04006FA24E68D47A4526110278412954FA8546909CAA000E957
      88262B40033D76A55FB0586C563B2596CD67B4582AE7400236DD18A0A068745B
      4DD6ED32A653AEF7BBE4CD677F00177051049E14006CC45F7158BC66371D8FC8
      6463080CA002E328CBD10DF92CE62EF39DD063949A3C3E261E90D45CF37A1D66
      B75DAFD86C7407CDA0010BB78831F74001CEF61999BA6CB8549CFF0F8D4954F2
      6B95E87D51380031F478FD3EA757ADD7EC41CEFDB0048515105BF840048F2000
      03E7DFD0B35D9F64A78BEDF84595BF3E874A1E87FC000F1FBF8FF7FCFFC0100A
      66DB90A003683E220D19488EA3E83BCE00BD2B93D701428A5A070AC049D94000
      0E10EA2050C40000BD11C3112C4D13C50EC8DD15800C2926F03C4F20908641F0
      8B5514C02F7C70EAA365AC182DA2041484000FB22C7723C9124C94BB2DC46AAC
      AC21E534A4000B32AA211AA16E035725BB31D4B8D692D308003D4C88814533B0
      2C1CBF35CD936CDC958D7388013092C881773B800224F48A4B0854B537BA92F5
      00C6169428002E510881194580039D1D41D2148D2537BF043CC732A1E57D3400
      0A14EA313EA133FD26D8A020803C204FF004160D0784426150B864361D0F8844
      625138A4562D114D46400768E441571F001524517924964D27944A6552B964B6
      5D2F984C665339A4D66D379B1CE7400474F620B3A00009943940068D0C40D240
      06FA64E29D4FA85461B0278412A557AC56618A9AE000CF5F88046C4003DD9400
      E7B456AD56BB65B6DD6FB85C6E573B780AEC007B5E40095BE441737F0011B052
      CA3006914AA61BEE98BC64DAA956C6E4725061E65400D4CC64F359BCE6773D9F
      D0687251F55C86472FC2E1D034BA6E8B5D9DC7EBF654FC111800CADC6CF75BBD
      E6F77DBFE04C171C30011F8D34D4C2E93ABC4F079D58D8F3FA515DAEDF73D3EC
      767B5DBEE7765CA7F0000B1E39C72615CBD662BBDEB94747D9DCEAEE19508CA8
      F000C3FC7BFF5FBFE7F7FC9A8D900AF6BEA16E1970E2B8EA83CC84BD0E6BFF07
      A14F7420E03E2EBA0C1AC30EB3E709C390EC3D0FBBC304440014312A1006C500
      01E515AB305A1106B5B103FD09464D942B0DC2F0CBE51AC791EC7D1FB23110C1
      1244C83815238007AC9516A8EE531118C80EF46928B3B1BA110C06B0D4A92DCB
      92ECBC96C85221428448E0549325AB11720F183D52FBB129CDCC64AC83CB12D4
      E33BCF13CCBF30C4B31C8D244947AC98C349CE64A13D37C808803C204FF00416
      0D0784426150B864361D0F8844625138A4562D0D2346400CA8E4206B1F8DC762
      F23924964D27944A6552B964B65D2F984C665339A4D66D30304E400A19E42015
      3F003D68537858068D0C40D24006FA65129D4FA8546110278412A557AC566111
      923485951E90472BF5AB2596CD67B45A6D56BB65B6AF39304EE7B079F82A8343
      ABD180348A5530DF6EC06066554AB60B0D87AE57AC035C561F1D8FC86472593C
      A6564F70B92867D40A13D6B17ABE206974DCB6972384D36A69D89B162F1BAAD8
      6C765B3DA6D763989E66AE99CBC54B410BA4E8AFDB6E256751C5E445F5922834
      7F19ADE4F47A5D3EA757AD09DC5CE0D75BBE7AF347E05F749D7F24A38FE5EBF2
      EC7CDB0F33D1EFF87C7E5F39BF6775DBDE77B7DE08570746BFBE900A18F3C04E
      23D4D73A102C1505C1906BE0FB336BB33ACFBF884BFCE1C1D014090CB5503A0E
      E735F0E44511C4912B0F083770937AA8B7EFEBC50044CF443718B250F3D8E7BD
      D1A4751DC791EA5B143F1153F51642A8442EF1C7CE946724B031B20B10413264
      A529CA92948082BB909BBEBDBC2E14912AB6A808803C204FF004160D07844261
      50B864361D0F8844625138A4562D0D2346400CA8E4206B1F8DC762F23924964D
      27944A6552B964B65D2F984C665339A4D66D30304E400A19E420153F003D6853
      7858068D0C40D24006FA65129D4FA8546110278412A557AC566111923485951E
      90472BF5AB2596CD67B45A6D56BB65B6AF39304EE7B079F82A8343ABD180348A
      5530DF6EC06066554AB60B0D87AE57AC035C561F1D8FC86472593CA6564F70B9
      2867D40A13D6B17ABE206974DCB6972384D36A69D89B162F1BAAD86C765B3DA6
      D763989E66AE99CBC54B410BA4E8AFDB6E256751C5E445F59228347F19ADE4F4
      7A5D3EA757AD09DC5CE0D75BBE7AF347E05F749D7F24A38FE5EBF2EC7CDB0F33
      D1EFF87C7E5F39BF6775DBDE77B7DE08570746BFBE900A18F3C04E23D4D73A10
      2C1505C1906BE0FB336BB33ACFBF884BFCE1C1D014090CB5503A0EE735F0E445
      11C4912B0F083770937AA8B7EFEBC50044CF443718B250F3D8E7BDD1A4751DC7
      91EA5B143F1153F51642A8442EF1C7CE946724B031B20B10413264A529CA9294
      8082BB909BBEBDBC2E14912AB6A808803C204FF004160D0784426150B864361D
      0F8844625138A4562D0D2346400CA8E4206B1F8DC762F23924964D27944A6552
      B964B65D2F984C665339A4D66D30304E400A19E420153F003D68537858068D0C
      40D24006FA65129D4FA8546110278412A557AC566111923485951E90472BF5AB
      2596CD67B45A6D56BB65B6AF39304EE7B079F82A8343ABD180348A5530DF6EC0
      6066554AB60B0D87AE57AC035C561F1D8FC86472593CA6564F70B92867D40A13
      D6B17ABE206974DCB6972384D36A69D89B162F1BAAD86C765B3DA6D763989E66
      AE99CBC54B410BA4E8AFDB6E256751C5E445F59228347F19ADE4F47A5D3EA757
      AD09DC5CE0D75BBE7AF347E05F749D7F24A38FE5EBF2EC7CDB0F33D1EFF87C7E
      5F39BF6775DBDE77B7DE08570746BFBE900A18F3C04E23D4D73A102C1505C190
      6BE0FB336BB33ACFBF884BFCE1C1D014090CB5503A0EE735F0E44511C4912B0F
      083770937AA8B7EFEBC50044CF443718B250F3D8E7BDD1A4751DC791EA5B143F
      1153F51642A8442EF1C7CE946724B031B20B10413264A529CA92948082BB909B
      BEBDBC2E14912AB6A808803C204FF004160D0784426150B864361D0F88446251
      38A4562D0D2346400CA8E4206B1F8DC762F23924964D27944A6552B964B65D2F
      984C665339A4D66D30304E400A19E420153F003D68537858068D0C40D24006FA
      65129D4FA8546110278412A557AC566111923485951E90472BF5AB2596CD67B4
      5A6D56BB65B6AF39304EE7B079F82A8343ABD180348A5530DF6EC06066554AB6
      0B0D87AE57AC035C561F1D8FC86472593CA6564F70B92867D40A13D6B17ABE20
      6974DCB6972384D36A69D89B162F1BAAD86C765B3DA6D763989E66AE99CBC54B
      410BA4E8AFDB6E256751C5E445F59228347F19ADE4F47A5D3EA757AD09DC5CE0
      D75BBE7AF347E05F749D7F24A38FE5EBF2EC7CDB0F33D1EFF87C7E5F39BF6775
      DBDE77B7DE08570746BFBE900A18F3C04E23D4D73A102C1505C1906BE0FB336B
      B33ACFBF884BFCE1C1D014090CB5503A0EE735F0E44511C4912B0F083770937A
      A8B7EFEBC50044CF443718B250F3D8E7BDD1A4751DC791EA5B143F1153F51642
      A8442EF1C7CE946724B031B20B10413264A529CA92948082BB909BBEBDBC2E14
      912AB6A808803C204FF004160D0784426150B864361D0F8844625138A4562D0D
      2346400CA8E4206B1F8DC762F23924964D27944A6552B964B65D2F984C665339
      A4D66D30304E400A19E420153F003D68537858068D0C40D24006FA65129D4FA8
      546110278412A50B58180200030001400078280A108ACD6D1028640019078140
      01B0881C00070D83C582C5066C002E171BA3C2BD63ABCDE324690B2A3D208E61
      B038BC66371D8FC86472593CA6561B39AECF1433EA0509EB81A3006914AA61BF
      2DA7D44D6A956C65640010AED7AC200C0426DF72B9800F15FAE6C541B3DAC16F
      37BB71E2FD77D4C9F0785C38D799C9E8747A5D3EA757ADD7A7662773D83CFC15
      41A1D5F43A340D2E9BD8F47A3578BE18E2F9C6BFC46CB5C585B4F0C8B4DB217E
      DF7F1F82F4A308D312E6B9F0040D03C1104C1505C188B3B4CD338EFB3CD028E8
      5A92F2B4B06C34C7BD70DA1AFE38AFF43C82B9701A0E8FB9D1344715C5916C5D
      17C6107275083BACEBC2A93C70B348F3C631EA590EA1AF98C0BA2D4B5800B6A1
      4D7360828D6281600012B223F0BAC90842DEB8870582C729C8F250C127A18142
      EB2E2DB102FAF8B844437135374B4AD72AC40442F2852C6DFABEFFA8912A4483
      45102C7D3FD0140D0541B2907BB8833BCF033EF142A8542EF334D42524894812
      0ABAD82F2FBBF32AADCBD3DC004B5370513553136D3683C4150AD527371278A0
      FC3754E21F333E05054735A0AFBAD4FD3854F3FB3420F212BCBB4F09A8956380
      0635950245549D9D67DA168DA48750CCDC6B0946EA8C734747748DA76952B0FD
      7D5055F5DC90DBBDD57560B655372CDF34D5ADA5D61449EAD2BABA4DD5E22133
      0A0BAADF35050DFD8092CAEB94B522CA885141858006FE1C0000988A46066280
      01318B80074E35664F76FE3D8FE4190C376AC2345428D14750C4799150970A1D
      82CB32DC8D4F60D9961334CB183CB99A6638448EB22B925CC0833ED79D5171CE
      483E8B53C3F353DD354E686CC59F564829C1AB800796B400005AEA46096C0000
      A7B18006BECD8E315966D5B5ED9B6B27925AF93519945B9955BDB747D9765FA7
      4B3793783057EE42137AC96AF2D351F0362A1F2F53379D68BF0F06C4B1365355
      8AF1BE60181705CBE739B67EA2091D0C8AB4C4EC463BBC751D4F55D5A51B8511
      1B5171C51A84D1F0CF5917A020803C204FF004160D0784421606008001100028
      41561053C3C1411084C2A18003044E0D115045A31199241DB10F1C0E22650643
      200078143621D29003622A0050444010B86C7641222849C0129878A243389D44
      A3810304EC5078004B6A02892D56AD570011AB400655761035B0572BD58B2596
      CD67B45A6D56BB65B6DD6FB85C6E573BA5D6ED77BC5E6F57BBE470C14D50E061
      00AC2001EB87BED54038B92A071C0037E471393CA6572D65813C2096A8580299
      1E2845E2D49AB6773F05D08034709D3476B1A1785224D331C4D66F39D9A22690
      FA854A0BBAA1A226509AA1E2610E585537D54CA5688D626557EC35DE8E5FADD7
      EC767B5DBEE777BDDFF0786FD80C141F080AC3623B18B00E371F9137F8BE5F3E
      C66737D7A14A6572D97F33E8FFAB0E73A0E906B01C0103C1104C1505C1906C1C
      FA2FEF2142C1B0AC39EAECBD8F7102C832507C3D0FA10FB3B093B6A95222E5C4
      103C04EA4090345317C6118C6519C691AAE308800C0C26F342AF53AF0CA48C74
      36F846D22BC11132E99374DA960962A2E3C8CEF456B1A0CB040B164A32CCB52D
      CB92ECBCBC4711D428F442D0C31920BDF0ECBF35AF1244D937A1129BAB2ABA72
      A4E13BCF13CCF53DBE530BCA833CEF4C2EF5CCE8CC850E3E33E514CC20745CBF
      3945B2C51D49D294AD2D4BA133F4774047B41C7F42A1343C894C52D375491AD2
      083CAD1754F56D5D57D6117D3531D05333DB34487355633D54D5DC4154CE92BC
      ED5F58962D8D63B2759C793247CEB481434D344D913657B69C156020B55D256B
      5B96EDBD6FA33655396653D67540845455D5C128A020803C204FF004160D0784
      426150B864361D0F8844625138A4562D0D2346400CA8E4206B1F8DC762F23924
      964D27944A6552B964B65D2F984C665339A4D66D30304E400A19E420153F003D
      68537858068D0C40D24006FA65129D4FA854611027841224B03004000606C1E0
      00C86457850006C00110381C59850A0003C14050A95C6E5108C91A42CA8F4823
      978B9DF6FD7FC06070583C26170D859C982773D83CFC1541A1DC68C01A452A98
      6FC3E673535AA55A2358AD572BD60B1592CD68B55B2DD70CDEB65375BBDE46BB
      1D76D76DB7DC6E775BBDE4E2753C50CFA81427ADCB279540D2E9BBDE66EF3BCD
      E8543617BD96D3A3D7EC767B5DBEE7760D89C5F071BC3C8D4B8F0BA4F272FDEF
      65CF9F126C59C0038585C34800B1C17415B006AADE84BE2F98704434C87AC63C
      2C0000F0FCBDA96BA69120C8FB66EA41B0AC2D0BC310CC34883C0E0384C7B88E
      328EF432CE5C3713A54F7A22F8AD2FA3ECB0BF0833F6C53FCD6000582B608314
      AE990AFC1682C02B4470284131FC509441EBE422BD42123C9D27CA128CA4A943
      AC620CC7320E2B2511A14F4B94CC4A730A1D15221163E6FAABF1841919BFAB6B
      FEB2409210012247B23480D3AD2AE9E0FEC6D3122524BAB0A4FD41D0942D0D31
      4AAF14AEF24B4F34B884CBCF5D0F41CC887CCD174D2D2BF4ACBF91AC651CC770
      447CFCC82FA4E722C1949A174020F093AD5556158D6559B7544C3F2CC44CA448
      F544D5A49D4AA1D4BCD0FBCD74E4693746D1C2B2C5470868A0504F737D7C84D5
      925C2726DA76CDB56DDB89956CF1C40F2AA2F3CBB12CC16EC336021B6145F4D4
      6F634DAD5D3600020442C7625D088DAA82D5D415F37FE0180E0482DBF45DC346
      DC747A1148D7B81BBA8080803C204FF004160D0784001B0881C00070B0280019
      0C83C000510658180200030001400078282230785C3470886C4265450503C23D
      23954C665339A4D66D379C4E664469E001953F840D6853EA04EA8D47A4526954
      BA65369D4FA8546A553AA556AD57AC566B55BA4182BC0050D86100AB2001EB67
      AE4C8036B99206DC0037DC6D373BA5D6ED33813C2093892C3A21128A45AF90C8
      72222F2D8FCC21119000423B1E908031577CA53A7846A232A83439FE6B2B9FD0
      68745A3D26974DA7D46A6955E8ED854363B2D9DEB9FB5806DB6FB89BF55BBDE5
      CEF37B9BC66371D8FC872735BEC3E23138AC5E13C9C364313BDD565F339B1AF5
      FA9DBEE777BDDFF0787C5A2D6582C507B202ACD68CAED76E81B85CBC7F3FA417
      7F351C04237298B9E226000F0E728C94A1886A509AA22C4324FAB42EB33AEC3B
      50642509C290AC2D0BC30A3BCAD7360F5364DA2D898ADCF83730CC4CD23EF13C
      54A641CA2A0CA13B307C5719C691AC6D1BC70AEABF0E3D0D8BD8CA3DD11370F9
      47322AA514C8D2480116B3D17B391749528CA529CA92AB290DBCE833D2F5B66F
      6C4295446F8B752B4C89A49132C5726421194D136CDD37CE132CB0D7C7B0F47E
      BBC833048731CE3344CF3EC2B3520F184234050D43D1144BC539C3B2E440DB48
      512489454A33FD28F1D05274632852F4ED3D4FD40AC5193AD1D2F5213D5253E5
      431BD2D55BB74CA0B424D9575695AD6D5BA0B51CB51F4BB204BE84CC312D7115
      A020803C204FF004160D0784426150B864361D0F8844625138A4562D0D234640
      0CA8E4206B1F8DC762F23924964D27944A6552B964B65D2F984C665339A4D66D
      30304E400A19E420153F003D68537858068D0C40D24006FA65129D4FA8546110
      278412A557AC566111923485951E90472BF5AB2596CD67B45A6D56BB65B6AF39
      304EE7B079F82A8343ABD180348A5530DF6EC06066554AB60B0D87AE57AC035C
      561F1D8FC86472593CA6564F70B92867D40A13D6B17ABE206974DCB6972384D3
      6A69D89B162F1BAAD86C765B3DA6D763989E66AE99CBC54B410BA4E8AFDB6E25
      6751C5E445F59228347F19ADE4F47A5D3EA757AD09DC5CE0D75BBE7AF347E05F
      749D7F24A38FE5EBF2EC7CDB0F33D1EFF87C7E5F39BF6775DBDE77B7DE085707
      46BFBE900A18F3C04E23D4D73A102C1505C1906BE0FB336BB33ACFBF884BFCE1
      C1D014090CB5503A0EE735F0E44511C4912B0F083770937AA8B7EFEBC50044CF
      443718B250F3D8E7BDD1A4751DC791EA5B143F1153F51642A8442EF1C7CE9467
      24B031B20B10413264A529CA92948082BB909BBEBDBC2E14912AB6A808803C20
      4FF004160D0784426150B864361D0F8844625138A4562D0D2346400CA8E4206B
      1F8DC762F23924964D27944A6552B964B65D2F984C665339A4D66D30304E400A
      19E420153F003D68537858068D0C40D24006FA65129D4FA8546110278412A557
      AC566111923485951E90472BF5AB2596CD67B45A6D56BB65B6AF39304EE7B079
      F82A8343ABD180348A5530DF6EC06066554AB60B0D87AE57AC035C561F1D8FC8
      6472593CA6564F70B92867D40A13D6B17ABE206974DCB6972384D36A69D89B16
      2F1BAAD86C765B3DA6D763989E66AE99CBC54B410BA4E8AFDB6E256751C5E445
      F59228347F19ADE4F47A5D3EA757AD09DC5CE0D75BBE7AF347E05F749D7F24A3
      8FE5EBF2EC7CDB0F33D1EFF87C7E5F39BF6775DBDE77B7DE08570746BFBE900A
      18F3C04E23D4D73A102C1505C1906BE0FB336BB33ACFBF884BFCE1C1D014090C
      B5503A0EE735F0E44511C4912B0F083770937AA8B7EFEBC50044CF443718B250
      F3D8E7BDD1A4751DC791EA5B143F1153F51642A8442EF1C7CE946724B031B20B
      10413264A529CA92948082BB909BBEBDBC2E14912AB6A808803C204FF004160D
      0784426150B864361D0F8844625138A4562D0D2346400CA8E4206B1F8DC762F2
      3924964D27944A6552B964B65D2F984C665339A4D66D30304E400A19E420153F
      003D68537858068D0C40D24006FA65129D4FA8546110278412A557AC56611192
      3485951E90472BF5AB2596CD67B45A6D56BB65B6AF39304EE7B079F82A8343AB
      D180348A5530DF6EC06066554AB60B0D87AE57AC035C561F1D8FC86472593CA6
      564F70B92867D40A13D6B17ABE206974DCB6972384D36A69D89B162F1BAAD86C
      765B3DA6D763989E66AE99CBC54B410BA4E8AFDB6E256751C5E445F59228347F
      19ADE4F47A5D3EA757AD09DC5CE0D75BBE7AF347E05F749D7F24A38FE5EBF2EC
      7CDB0F33D1EFF87C7E5F39BF6775DBDE77B7DE08570746BFBE900A18F3C04E23
      D4D73A102C1505C1906BE0FB336BB33ACFBF884BFCE1C1D014090CB5503A0EE7
      35F0E44511C4912B0F083770937AA8B7EFEBC50044CF443718B250F3D8E7BDD1
      A4751DC791EA5B143F1153F51642A8442EF1C7CE946724B031B20B10413264A5
      29CA92948082BB909BBEBDBC2E14912AB6A808803C204FF004160D0784426150
      B864361D0F8844625138A4562D0D2346400CA8E4206B1F8DC762F23924964D27
      944A6552B964B65D2F984C665339A4D66D30304E400A19E420153F003D685378
      58068D0C40D24006FA65129D4FA8546110278412A557AC566111923485951E90
      472BF5AB2596CD67B45A6D56BB65B6AF39304EE7B079F82A8343ABD180348A55
      30DF6EC06066554AB60B0D87AE57AC035C561F1D8FC86472593CA6564F70B928
      67D40A13D6B17ABE206974DCB6972384D36A69D89B162F1BAAD86C765B3DA6D7
      63989E66AE99CBC54B410BA4E8AFDB6E256751C5E445F59228347F19ADE4F47A
      5D3EA757AD09DC5CE0D75BBE7AF347E05F749D7F24A38FE5EBF2EC7CDB0F33D1
      EFF87C7E5F39BF6775DBDE77B7DE08570746BFBE900A18F3C04E23D4D73A102C
      1505C1906BE0FB336BB33ACFBF884BFCE1C1D014090CB5503A0EE735F0E44511
      C4912B0F083770937AA8B7EFEBC50044CF443718B250F3D8E7BDD1A4751DC791
      EA5B143F1153F51642A8442EF1C7CE946724B031B20B10413264A529CA929480
      82BB909BBEBDBC2E14912AB6A808803C204FF004160D0784426150B864361D0F
      8844625138A4562D0D2346400CA8E4206B1F8DC762F23924964D27944A6552B9
      64B65D2F984C665339A4D66D30304E400A19E420153F003D68537858068D0C40
      D24006FA65129D4FA8546110278412A557AC566111923485951E90472BF5AB25
      96CD67B45A6D56BB65B6AF39304EE7B079F82A8343ABD180348A5530DF6EC060
      66554AB60B0D87AE57AC035C561F1D8FC86472593CA6564F70B92867D40A13D6
      B17ABE206974DCB6972384D36A69D89B162F1BAAD86C765B3DA6D763989E66AE
      99CBC54B410BA4E8AFDB6E256751C5E445F59228347F19ADE4F47A5D3EA757AD
      09DC5CE0D75BBE7AF347E05F749D7F24A38FE5EBF2EC7CDB0F33D1EFF87C7E5F
      39BF6775DBDE77B7DE08570746BFBE900A18F3C04E23D4D73A102C1505C1906B
      E0FB336BB33ACFBF884BFCE1C1D014090CB5503A0EE735F0E44511C4912B0F08
      3770937AA8B7EFEBC50044CF443718B250F3D8E7BDD1A4751DC791EA5B143F11
      53F51642A8442EF1C7CE946724B031B20B10413264A529CA92948082BB909BBE
      BDBC2E14912AB6A808803C204FF004160D0784426150B864361D0F8844625138
      A4562D0D2346400CA8E4206B1F8DC762F23924964D27944A6552B964B65D2F98
      4C665339A4D66D30304E400A19E420153F003D68537858068D0C40D24006FA65
      129D4FA8546110278412A557AC566111923485951E90472BF5AB2596CD67B45A
      6D56BB65B6AF39304EE7B079F82A8343ABD180348A5530DF6EC06066554AB60B
      0D87AE57AC035C561F1D8FC86472593CA6564F70B92867D40A13D6B17ABE2069
      74DCB6972384D36A69D89B162F1BAAD86C765B3DA6D763989E66AE99CBC54B41
      0BA4E8AFDB6E256751C5E445F59228347F19ADE4F47A5D3EA757AD09DC5CE0D7
      5BBE7AF347E05F749D7F24A38FE5EBF2EC7CDB0F33D1EFF87C7E5F39BF6775DB
      DE77B7DE08570746BFBE900A18F3C04E23D4D73A102C1505C1906BE0FB336BB3
      3ACFBF884BFCE1C1D014090CB5503A0EE735F0E44511C4912B0F083770937AA8
      B7EFEBC50044CF443718B250F3D8E7BDD1A4751DC791EA5B143F1153F51642A8
      442EF1C7CE946724B031B20B10413264A529CA92948082BB909BBEBDBC2E1491
      2AB6A808803C204FF004160D0784426150B864361D0F8844625138A4562D0D23
      46400CA8E4206B1F8DC762F23924964D27944A6552B964B65D2F984C665339A4
      D66D30304E400A19E420153F003D68537858068D0C40D24006FA65129D4FA854
      6110278412A557AC566111923485951E90472BF5AB2596CD67B45A6D56BB65B6
      AF39304EE7B079F82A8343ABD180348A5530DF6EC06066554AB60B0D87AE57AC
      035C561F1D8FC86472593CA6564F70B92867D40A13D6B17ABE206974DCB69723
      84D36A69D89B162F1BAAD86C765B3DA6D763989E66AE99CBC53875BF0031F85A
      1D1DFF6DC7AC6A391CB8BEB245068FE335BCCEA757ADD7EC76613B8B9C1AEB77
      CF4D86DE30032FCD103A7A4007CF676BDD2AE57BFB5CEB1F42C3CFF97E7F5FBF
      E7F66EEE374EF378F0A622040C0018304A2017C1904414FF420883E308B8EFA3
      5CE9C290CC350DC38FD400CDAECCEA5E1CC480019113A20714540001D16C3B17
      A0B09C60D4C2C83BA2D7C671CC751DC78B743EDDC42DEA5020C880018123A207
      8C94001FF26C7B0E46527B231ABECE93F1294B12CCB52DA511FC0520C089206F
      31C7084340744D00000F35CB90CCA336B012A20B1BC31384ED3BCF12D4BC82BB
      F1122E1E5000018B41A2076D0C0000744CF308A020803C204FF004160D078442
      6150B864361D0F8844625138A4562D0D2346400CA8E4206B1F8DC762F2392496
      4D27944A6552B964B65D2F984C665339A4D66D30304E400A19E420153F003D68
      5141851400D1A4441E94B003F29D3782B6000881C0E000D83C3C000A02849160
      6008000C15B003C2B95093D4AA956AC56ACF16B5556AF59ADD76D109813C2097
      7BE5F6FD148C91A42CA8F4823984BFE27158BC66371D8FC86472593924E6C73C
      50CFA81427AC40679F0033B451072694000CD45FAE36CBA5BE2F5FB0D8D4165D
      76520FABB9DBAED15DC5B6EB8ABCDEF6DC3E243F0383C28D791C5E67379DCFE8
      747A5D3C665A773D83CFC1541A1C2EAB5664F8620EFF200003E7C5EF75BBB8B6
      C2C564B37B389EADD45FE9BFC4F07A9FBC6F1F0EE4B96FE40501C0902C0D03C1
      098BACCC334EDB390019908A1805C28001D90B8007CC34C7BEEDAA2AF7364DA3
      E4E1C3B11A2312B8081C1315A5EFF2448323EE53FF164691AC6D1BC711CAFF05
      BB08301D1F80078C840007D2280061C9086009258000949C0002F28BB8CEA5C7
      B4AC000CB2C80039CB8873D4D9AC4B196084050000F064191334CA84440F843C
      A8AA6B9110A9214AE940B3000F945084ABE00020B1AB73C2BB2FA0A9D4C683CC
      B33CD23C4D7382D6DCBF0AB910002AB49CCD349914620CDC4E685CED41214FD4
      7551A23173111830D17D4955D5956D5D57C131E3328381B5A800E395B5CA2071
      D780056A06BE738AAD39D13344D482C40AC001340F00051B36B66F8D37612A61
      45A30F4405853ED9D076A37D378013ECFE82CEF6E2AF6A5894BD17359B0AA528
      444CB72CF373D1F6F85149D2A82CCF65D35472E56FC4768444845455855D5321
      0EF80063E198361D87E2188E24BBCB2328004E630939E78D8007EE3CE74F7362
      C771D0373590B03DF6BAED715017944D642C5924EEBAE4283E59725419AE6D98
      E5ADD37A2859B76D276B3EAB4DDC1C5B565D336720D82E271C61083845A98004
      86AC001E5ACE9FADEB9AEEBD89B500600046EC923C9289805B480074ED80000B
      B7BA59D64F3F67B935C394443696EFBA671BB4F99E6719A5BCF5A159BE4B79EE
      5982C19EF0547D3A8358B75CBD7C07149CE88685166D99A620BA76BF04EA3CFF
      45D1F49D2F4C76F5000007D5BF9C4F0D9720D816F5D7D4085CC7C5F02AE75D91
      EEBC4707A2A1176D29CAD9D99F75DFCCC6C2ADE25D563AA3CA687E344776F97A
      4F3746F3DD3405D0FB5EEFBDEFFC115CAC7B0007D7CD02F77BE70EDDF64D776F
      71DB595644D8D95EBF63BC4DDF670131DB7DF71C361C8BCE7A8BBD78B397905B
      D6CBFD0A0F097CB44522B85FDB99694B35469010803C204FF004160D07844261
      50B864361D0F8844625138A4562D0D20C6400CF8E45E3D1F904864523924964D
      27944A6552B964B65D2F984C6651F72CD4000B9C4CE64D800220703800360F0F
      000280A10A581800010A5515E0A00051E0B49080029550A7D4A10B0AB530015C
      850A0F00064322C62886570C15EB041A8EA0645A27D3CA1D428F3C9F534F147A
      4A228308B41E2CA003C5A20D789FD068745AD5FE7D40BF58ECB639ED0070889E
      42703859EAC2D1930061801027841275A7D46A63C68D600197AF0001B65A8026
      D400E2DC00025BB0002B7C007F70755C3E27178D070172400F6E6001BFCFE204
      3A4000EF5400FCEC71FB5DBEE77648FDF00001FE3002EFCDDEF47A7D5E9D269B
      D7EFF87C61687FA000D5F7F1793E5FBFE7A1CCFF80015404E20E3028003FC10F
      EC1505C1906C1D07C20953DB08C290AA4C3D430000C70D80011C3D0B44110A28
      6A4480007913A1869C540003F168007DC608837609362D9C607DA1033C740011
      31EC451FC8120C8521C8886C2722C91104303D4350E43C11C9328C1F121A9134
      50859D12C8000BCB88F36A02000F01FB1CC771E913294D134CD535CD894C8F36
      CE0F4C97268C70EC3F38CF0ED4A92B078861C93F8000DD052F36D314C833C791
      F4F345D1946D1D0B4DF47D2498CE70DCEB27D274CA5B3DC4F3EA173F9C940D06
      8BCBF30BC283C75444CD4D55956D5D57A5948D6159A3D4AC9D3BD695CA274E4A
      E8554151037424C14355132D155D59164D955656565D9C83D6D4BD7167D9F5E5
      3D5F501415835250B53A0D54D1333DA971DC972C8566DCD640ED7580032DDC00
      04B78DD3655AD3F5B351A2D52D896FD8D715E77FE0180BD68080803C204FF004
      160D0784426150B864361D0F8844625138A4562D178C46614948E000A31F0006
      E451A924964D27944A6552B88B525C001E4C618E49A4864717024E400FD9E420
      CF3F00226852CA25168D47A4526954BA651D606008000C0D83C00190C8AA8A00
      0D8002207038AE8A1400078280A14DB45A6D506813C2096BB85C6E573BA5D6ED
      77BC4665CD4984CA1734724D837189C812773D83CFCCF41A1DE71D8FC8647257
      2A7D46A755ABD66B75DAFD86C765B3E4F4574B6DBF47A7A2D3C0010304154165
      0068623AAD66BB61B283E56A400CFD9A0D5CAF582A8F0DE6E351C7E4726277BB
      E8F267359160E713A9E3F67D40A122795DBEE777BDDFF078629A5F179617B4D6
      EF36F10E067787C586EEB5BBDD97B78478E26FB4FF2AA55AB0002B4FB33CB23F
      4F340CA2B9898B9CBFBA09BA2CC2B0EEB312EC31B03C2D0BC30D39B0AF0001C1
      60B3B330020CF93790238CAD9110E87114AB887AB43C2AE000F0AD432BC3C91A
      BC2F436CB1C4E84B690FC4C88448FA37ECE3EEFCC7AC83F8CC3FF00C8D01B411
      C4A48BC12BF214C0304C23A8C420CC5318ED4A730CC531A8B0DAC10F44126C46
      A83772220A582A4DAAA8642AD19A0B0DC55380A118CED3234881CFCE5474F547
      8A348720CEF27BDF02D0346CC92C05948A187B528000134BA3003534001F74EA
      10385400010151D1D52D4D53B7F0E4D0FF33537CD8F9D113C2BF3D4F91A512E0
      AB6FC3E1542D11BD78B4D6515B3683C5F3B11114D08D8A110111116A12B3B5F4
      2C7D57C4B28D6EF757502D83644613AD6D6659C845A0F5A333343B204435B50F
      6B20F6DD868745F5AA2B050000B5ED5FA52015F4001DF7E80066E00865E803E0
      8001FF83A20046140016D86A180D6200005B89D394F5F093ADA0013D8D800106
      3D8BE40A5DCD55DD335B2D6AC0B382A0D6CE76F4EF14D66D8DE590A915F66932
      C9E442C5440019536B3D592B3D999DDD997CCF744D55764F373375C516146616
      14653A2B16FD156CC4F75D188A647A455B9EDA9A667D965BBAAE5F3CE653A4FA
      8B821B6E6FB7EE08797DB9800196EDB8EF0936BB34EBFAD365B1E7A870A0D7D7
      7BCA899B70C92D835A6D55B372D6B6BA0F17B4C65C737338C5214667BF48BA70
      A0AAC37CCF09ADA31BDD591122BD364BA54DB58EA3C672B76EAF24233B6AA3C4
      F719A183DD80017F7DDCF8088F55A4EC1A5D10DA675D3F2DE0E6B407988CD076
      8D94857A2DBDDD70A25C1F69E2F5B765C1626A7C6A1DEBC5D266BE89F87BEEC3
      9E70196ECD146D13DFC48C76C865EC0B0000BFF4001F9FEF9EFFCC7801804844
      000D980C431DD8C177AEFE0040D450D1DBE3A87B8AC1A2C0F43AB34883DA70B0
      38899010803C204FF004160D0784426150B864361D0F8844625138636110001C
      0E160002832190003C0A214B030000212500281E0A08E45A311A8E47A41228A4
      1E481000496592B28419B0004446400D83C3C240D81C4622F20994867D178CC5
      C512A94CF61116A4C6A7B1E3C0026914AC461615B64576BF379CCA4013C8346C
      C1270046E1A50AA282AD3584842F50C4C5F40067C05E30583C26170D87C46271
      58BC65E1FD8F0000F250C60E54002FCC637359BCE6773D9F89586B400AE57B43
      41A548A552CBBC224926946AE39A0DA6D71102784136DBBDE5E2DD70A247ECB3
      49754687B3D94F67F41A4A22A76BBB60ED13AE85DF9742A251BA371B7CE6C729
      95CB6A038A95D75BBF92AC0515DD2D7E27A2EFFB6DBDDEA5B3B926E7692CBA6D
      05EA7085917008003A408DEC0D03C1104C14CE1D5068000BC20CA32CCC05F05C
      2D0BC31043A6B53ECD3AB2F8BF6F729EACBF2E4C3313B38DC37514458CE38AF2
      38E83A7A50190912829FA8AD62AEA02844427E87466F0AEF0DA76EDB8EE6286A
      2AAA83BAEE6A40FD2BB1E2951FA10910F0A7110F54A0FE2C0D4346F920B223AA
      ABCBD1F220BA3B4D6A6AFF2190091701C0B16CE539CE93AA1F069D507C2285B2
      A60B2ECCCED40D0514070BDA7F2B29B113431E39B2A21920C7541D248845549D
      2D4BD30C43E0B22CD0F352F052284B5EB8540D9CD6BDC01014083A53356D5D57
      AF13C4F40BC253F429585715CD755DD254AD795FD81164C50EA234DCB7452873
      2B55212F13655337D5760DA569C5B594215A4F909D016A5B96EDBD6FA1B5F5C1
      71DC8F7BC68BD0EA623EA72F126A81472174854CC1D9C854DD3855972DF57DB0
      D6B4F6854FB3FC2B7E60982E0D16DC583E1585D337AA137BDA386625835FD6C6
      016D60789E358DE38C1E138EE4190B7B8721047E4C000E194E4595D82DC00008
      E615AE05966699AE258FE6D9CE7566D50855A236680001EBA1E77A2CE6026900
      01D1A58001FE9D9956FA36A5A9D799C6A9ABE8B926B1ADD7980EA3AE6C1B0CE4
      8080803C204FF004160D0784426150B864361D0F8844625138A4562D178C4663
      51B8E4763D09084863F23924964D27944A6552B8BB065C0017CC659339A4D66D
      379C4E6753B93409E1049E5068543A25168D1D908428F4BA65369D4F5CD44003
      7AA53EAD57AC566B55B89CFA815CB0586C56395B7ACC0003DA6C96BB64E8036F
      00022E40024DD400BEBC0027D457DDF40025C0002FAFBB6E170D87C4592BD89C
      66371D8FC86471C3DCA0018797C9667359BCE627179DD068745A3D268C77A700
      31755A5D66B75DAF8E67F61B3DA6D76DB791E9C77A9D5EE37DBFE063765C1E27
      178DC7C8EEB78C5E47379DCF9DF0FA1D3EA757AD34E56AB99D7EE777BD0DE977
      FC5E3F2435606000043D000503C140002844FCFE9F5FB7DFF184F9E95E8F7FBB
      F0001B0001101C071000F0783D8FFBCA863E6F520AFB3E0E3C0301C0A6C40F04
      C170CA28F0C350EC3CE841AFABDD08A1F09C090341050414FC8C0FDBD8BD4548
      344D0AC2F18C3E8444307C4715B8119C510C46F20A0E8080803C204FF004160D
      0784426150B864361D0F8844625138A4562D178C466351B8E4763D1F90486452
      38A2C0C0000849C00A078280005087C9A50B0984B66131300400127974DA0CD8
      00220703800360F0F095CDE494BA6536153294C16592EA553AAD57A050A8946A
      4282AB57B0586C563B253A04F082596D56BB65B6DD6FB85C6E56DA84AAA72FA6
      49A753C004FA0B59A1D168F49B9E169B75A94B6F186C65168381AE6131B93CA6
      56D767B4E5B359BCE6773D9FCDB611000A1E8E810714000F078D4A23472BC557
      F01444469E1330BBD7E0B7A9DCAEFB5E9FE3EB783E05FF47A5829E19000646B3
      835AA0EDA11B8D8C6301763C51471275842353CAE5F3A15B3E8C2FA95485E8B4
      838D377B55CED76265D3BBDD1B99CD006A69F27A8EC3D0F236AF33FCC5B8CF5B
      DA86850E5398F131CE8402853CF022FEE1304AEA94F5390D5396E6BF4D043D0F
      B18CC44111C4912C4D13C41000509EB8A003BA9CBB8BC37306B0244456DFB64A
      13489A3EEECC3ADE2F8BF468E1ABA1438EF63930DC18F232316A0D2037D21227
      001B0EF990F0BF4D1288F635319C55162BF204792F4291B4C127BE918C22282E
      AA03C0D543B21C5C2849482C1D1BCA5172729DCC6EAA1D36C352C4BF1C2112D4
      76984AF1F4E526C8D0349305CE31452549A49115294BD314CD354DA290C070EE
      CE92C3F6942ECD8D3D504AF3821717820D185154BC5284CF09BA06C0A0ECB455
      7258C92374353F443F148A1F26388DD37734B12DF3F952BD155D90D825746A87
      5454484B1101A60F2269254B323DA8D51B165BE5092115654B5E5CB6428D6E40
      B69A5F5858563A73575E12656EA2BE315C2D4E5F97EA254B5FD80E0581E08C65
      AF19D46FEB734F40E8A0A15DB8B595095A46AE94AD6AA198631C874131EBF34E
      C2926D8D1759F8837B854FCA7E4AAEE4F665C927DC56C27F1D57F8F31F43E3C1
      460F94DAD95DD0DDCF6303ED44E3F99D1CE922587C2D2A35140D5582EA14E601
      A8EA9AAEADABA3353DDF8C20F9DAA9AD543A7A2F89485622903C1B12DB5F37C9
      723D757DD0B9A479A25E286ECD9FDDB4FEB7706638465518592505A5BD6C3066
      BBBEE79B0190F98C122579AF65F347016845B406D978F15AE4A790B07B46D524
      F35AC744CE6A7D1F4DD3F5182DCCC15D97C68F193AB41CF289EC916EEF88E833
      EEBFB73F3882ABD59618EEE99041D2AE9CF157D1B665E061F89E60FABB3E1F57
      75D21BCBDB35BF69D3469BBBAD5679927009A567E7A77A1D83F02A3EA4388776
      527225DBFB9DCFB1D4FE8B9F4BFAFF1FCFF511E35A4370320D68384DCF7DA610
      E7E69E8BDA5176CE71B83143686A99B33441E7B8D59AD782CE5E21900F0E3118
      B4D72EB39F090B784F9CF1BBC7FC4AE0018F806B356B2CB7BAF35D9AE984242A
      11A8A660A91EE90F86102DE2C3A664FC083A00820F0DFDC452AEFDE234498951
      2E26293881136284518A44288080803C204FF004160D0784426150B864361D0F
      8844625138A4562D178C466351B8E4763D1F904864523924964D2789B6000881
      C0E000D83C3C000A0284A26D379C4E6753B9E4F67D3F9C409E104A05168D47A4
      526954BA65369D4F9BCAA592E984CA6950AC566B55BAE576B142A257AC563B25
      96CD67B45A6D56BB65B6DD6FB84160201100FE00040001000000000000000001
      0400010000002C0100000101040001000000F300000002010300040000007834
      0000030103000100000005000000060103000100000002000000110104001900
      0000803400001201030001000000010000001501030001000000040000001601
      0400010000000A0000001701040019000000E43400001A010500010000004835
      00001B01050001000000503500001C0103000100000001000000280103000100
      0000020000003D01030001000000020000005201030001000000020000000000
      00000800080008000800080000001301000002030000CF0400006A0600000508
      0000A00900003B0B0000830E0000CC1000004113000058150000F31600008E18
      0000291A0000C41B00005F1D0000FA1E0000CB2000008A240000F0260000BA2A
      0000982D00000E2F0000243300000B010000EF010000CD0100009B0100009B01
      00009B0100009B010000480300004902000075020000170200009B0100009B01
      00009B0100009B0100009B0100009B010000D1010000BF03000066020000CA03
      0000DE0200007601000016040000820000000C770100E80300000C770100E803
      0000}
  end
  object rHTMLHint1: TrHTMLHint
    HintFont.Charset = DEFAULT_CHARSET
    HintFont.Color = clActiveCaption
    HintFont.Height = -16
    HintFont.Name = 'Tahoma'
    HintFont.Style = []
    HintFont.Quality = fqClearType
    Left = 224
    Top = 624
  end
end
