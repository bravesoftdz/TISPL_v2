object Form_parametry_linky: TForm_parametry_linky
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Parametry linky'
  ClientHeight = 623
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
  object rHTMLLabel_doporuc_pohony: TrHTMLLabel
    Left = 539
    Top = 544
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
  object rHTMLLabel_InfoText: TrHTMLLabel
    Left = 8
    Top = 576
    Width = 153
    Height = 19
    Caption = 'rHTMLLabel_InfoText'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentColor = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    Transparent = False
    OnClick = rHTMLLabel_InfoTextClick
  end
  object rHTMLLabel_info_zmenaR: TrHTMLLabel
    Left = 808
    Top = 601
    Width = 166
    Height = 19
    Caption = 'rHTML_label_RinfoText'
    Color = clNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHotLight
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentColor = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    Transparent = True
    OnClick = rHTMLLabel_InfoTextClick
    ShadowColor = clBlue
    ShadowSize = 50
  end
  object rHTMLLabel_podlahovy: TrHTMLLabel
    Left = 685
    Top = 55
    Width = 73
    Height = 18
    Caption = 'podlahov'#253
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -15
    Font.Name = 'Roboto'
    Font.Style = []
    Font.Quality = fqClearType
    ParentColor = False
    ParentFont = False
  end
  object rHTMLLabel_podvesny: TrHTMLLabel
    Left = 811
    Top = 55
    Width = 68
    Height = 18
    Caption = 'podv'#283'sn'#253
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -15
    Font.Name = 'Roboto'
    Font.Style = []
    Font.Quality = fqClearType
    ParentColor = False
    ParentFont = False
  end
  object Button_save: TscGPButton
    Left = 214
    Top = 580
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Roboto'
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
    Options.PressedColor = clLime
    Options.FocusedColor = 33023
    Options.DisabledColor = clRed
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 100
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = 33023
    Options.FrameHotColor = 1871359
    Options.FramePressedColor = 33023
    Options.FrameFocusedColor = 33023
    Options.FrameDisabledColor = 33023
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 100
    Options.FontNormalColor = clWhite
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clWhite
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
    Left = 356
    Top = 580
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Roboto'
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
    Options.PressedColor = clRed
    Options.FocusedColor = 33023
    Options.DisabledColor = 33023
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 100
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = 33023
    Options.FrameHotColor = 1871359
    Options.FramePressedColor = 33023
    Options.FrameFocusedColor = 33023
    Options.FrameDisabledColor = 33023
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 100
    Options.FontNormalColor = clWhite
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clWhite
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
      Font.Height = -19
      Font.Name = 'Roboto'
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
    Left = 450
    Top = 48
    Width = 120
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Roboto'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 3
    Animation = False
    Caption = ' Voz'#237'k'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = -1
    ImageMargin = 25
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
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clWhite
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
    Left = 8
    Top = 283
    Width = 120
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Roboto'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 4
    Animation = False
    Caption = ' Pohony'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = -1
    ImageMargin = 25
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
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clWhite
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
    Left = 830
    Top = 353
    Width = 75
    Height = 25
    Caption = 'Vypis_pohony'
    TabOrder = 5
    Visible = False
    OnClick = Vypis_pohonyClick
  end
  object scExPanel_doporuc_pohony: TscExPanel
    Left = 652
    Top = 529
    Width = 456
    Height = 66
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGrayText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 6
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
    Left = 1006
    Top = 503
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
    TabOrder = 7
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
    TabOrder = 8
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
    TabOrder = 9
    Visible = False
  end
  object scGPButton_jig: TscGPButton
    Left = 711
    Top = 382
    Width = 105
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 10
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
  object PopUPmenu: TscGPPanel
    Left = 900
    Top = 30
    Width = 211
    Height = 170
    TabOrder = 11
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
    Left = 467
    Top = 515
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
    TabOrder = 12
    Visible = False
    OnClick = Button_ADD_Click
    Animation = True
    Caption = 'Info'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = True
    ColorValue = clRed
    Options.NormalColor = clSilver
    Options.HotColor = clSilver
    Options.PressedColor = clBtnText
    Options.FocusedColor = clBtnFace
    Options.DisabledColor = clBtnText
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 200
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
    Left = 990
    Top = 432
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
    TabOrder = 13
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
    Left = 615
    Top = 474
    Width = 39
    Height = 34
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 14
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
  object scGPGlyphButton_TT: TscGPGlyphButton
    Left = 597
    Top = 394
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
    TabOrder = 15
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
    Left = 659
    Top = 389
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
    TabOrder = 16
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
    Width = 25
    Height = 25
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
    TabOrder = 17
    OnClick = Button_ADD_Click
    Animation = False
    Caption = 'GlyphButton_zobrazit_parametry'
    CanFocused = False
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = False
    ColorValue = clRed
    Options.NormalColor = clLime
    Options.HotColor = clLime
    Options.PressedColor = clLime
    Options.FocusedColor = clRed
    Options.DisabledColor = clSilver
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 220
    Options.HotColorAlpha = 150
    Options.PressedColorAlpha = 200
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 150
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clBtnText
    Options.FrameHotColor = clBtnText
    Options.FramePressedColor = clLime
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
    GlyphOptions.FocusedColor = clLime
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
    Left = 676
    Top = 480
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
    TabOrder = 18
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
    Left = 728
    Top = 486
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
    TabOrder = 19
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
  object Button1: TButton
    Left = 970
    Top = 328
    Width = 88
    Height = 25
    Caption = 'Add pohon - test'
    TabOrder = 20
    Visible = False
    OnClick = Button1Click
  end
  object scPanel_takt: TscPanel
    Left = 14
    Top = 79
    Width = 430
    Height = 194
    TabOrder = 21
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
    Color = clWhite
    Caption = 'scPanel_takt'
    StorePaintBuffer = False
    object rHTMLLabel_takt: TrHTMLLabel
      Left = 342
      Top = 50
      Width = 47
      Height = 44
      Caption = '<font color=#2b579a>[s]</font>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -35
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqAntialiased
      ParentFont = False
      Visible = False
      OnClick = rHTMLLabel_taktClick
    end
    object rEditNum_takt: TscGPNumericEdit
      Left = 169
      Top = 35
      Width = 173
      Height = 68
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 200
      Options.HotColorAlpha = 255
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 150
      Options.FrameNormalColor = clWhite
      Options.FrameHotColor = clWhite
      Options.FrameFocusedColor = clWhite
      Options.FrameDisabledColor = clWhite
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
      Font.Height = -48
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqAntialiased
      ParentBiDiMode = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnKeyDown = FormKeyDown
    end
    object scImage1: TscImage
      Left = 40
      Top = 21
      Width = 100
      Height = 100
      TabOrder = 1
      CustomImageIndex = -1
      DragForm = False
      DragTopForm = True
      StyleKind = scpsTransparent
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
      Color = clWhite
      Caption = 'scImage1'
      StorePaintBuffer = False
      Picture.Data = {
        0954506E67496D61676589504E470D0A1A0A0000000D49484452000000640000
        00640804000000DAEB5DDF000000097048597300000DD700000DD70142289B78
        00000C034944415478DAD55C097814451A7D1D4238120C128344301210391617
        12CEA00882C8A1DC87CBA172A380C2728882AE1CAEB0CBB5105D1644585801F9
        02C88653CC864B5061951B596E21402210AE40C8BDAF33994C574FF74CCF744F
        065F3EBFB1AB8AEA7E5D7FFD5755B504AB511AA1A88047500961FC2D871004B2
        34071948C775A4E10AAEE106FFEE597B5BC9B29E42F038EAF0AF3AA2F0042210
        A4D32E17A9F805677106C7710C1770FBC121F2089E4613FED542558E8771E4E0
        3C4E601FBEC361FCEA6F224DD001CFA336C9788F34FC8C246CC20F3AF55D39C2
        9FE1AEAF8894462BF4430B3C66F255D871193BB11C3B705F55DE0C1B108C6ED8
        EC0B22C1688D111C89921691B0239B23F309B62BDE7E2812F01C7F3FC2075613
        2989173088C31D6031093B72B1168B4928B7E0EA2F78A7E07733FAE2A69544A2
        311C3DF99E7C8B34C473648EA23D7F830B4B5AE190554482284E6F5333B9C70D
        9CC3255CE5EF2DDA8B6CC8E358963645B62F95A99EC30CF471082BD10B0D8AAE
        FB639935446A612A052AD0458B7C9ABA03F81E476825AE93420629E409F70A42
        2994279D28D4452C1F32DCE5FDF385DAA514E87CB3442452988EA774EB737111
        FFA51C27711C720CBE9840DAFEE7F1121A2312250CB43FC996B7CC112985B198
        5828ABCE48A749DB846D9468EF50076D0BE89473D32E8BED769821F23055DF70
        9DBADB48A424EFA448994318ED511FB4C1432E5B4DC39FBC275219F3D05DB326
        9F1AFE33ECC21D9324EC082199A1E8E4A2C5768E49B677441EE3A376D0AC39C9
        715A6F19093BCAD1824FA2DBA96DA32EA023BD322F88446009DA6994DFC72A4C
        A107EB1B44639D8E8ACFA2887FEE399187A9F03A6B949FC764FA442E14A149BC
        4DFD5856A76E01460A0ADD0091B288C3408DF2244CA0A2358A2854410A4E7940
        A31905365CB7763F452FD913220194D4291A75CB4823D5F0430DE2FB8BA07D5F
        844FF5DEA30AE1584DDBA28FCBB467FB3C21D28762A58EF0F23946EF7910A0BE
        8C2F0B6D4F165EE7FFBB87440532D1658BDDE84D936B98480C156B6555591EFE
        4A2DAEABFC34B09237B523015D0CCCAAA6D8A3D25779BCA31CEDDFA3639F4911
        9DAD1B7C691009C5571AC33B07EF7A4403D8487B6DC72EC62FEE5D97582AFB10
        3EF2CD82BFDBF478D3F87B87AEE7F5827485876EFCC7142035166134DF8B6748
        A0D6B76307ADB67B22126A5018D349259D6360D467D321D2126B9C9CEC0D78CD
        F5DBB0888829884442F12FC5ED6D388A5770DC8B9EFD4AE4755A4ED1A5BE837E
        7C246FE0472215E98E3754D54F77A3101F4822A3315755BB87D6C0F3D9E16722
        95B0972E851237E8C06FF7BA67BF11194F9327220EA34C3887BE24521B03F03B
        8673EB78977C91481883D518A1F1595AE323266EE63B226DB0B050767230833E
        618E92487FFC1D6584E63369CB8DB97AC54B241C9B152A29835A759D834819DA
        0F31A0FD85B1C8210FBA2F3E227D68229439FF35A4926927D2885180988CFE94
        F323F78124F206E60B39E78B9C023FD9898CA2E2552AE2347AAEDB4CDED05744
        5A3324502E62E4D16CC4D9880453B0BA0A8D93D0411EAE07924869CE11D13B5F
        C251CA9689D4A1E32E6611C7D26D370BDF69AD318C4B94588C3791231351C783
        3739674E3FC044AA32DC75C4F5791828A7B7253A8933304E68B895219119C5EB
        6B22C030FCAD48732DC348C62F24521E2B5449B8776843CCC3974402A8A986A1
        26AE229EC6B120B52D71766C413545A33CB445E2034E4446109554A6231922A1
        05BE11F4F2290A962799287F115141E25411D390090C6C6F79D99B5F894CC3FB
        42C93CAA37F353DD0F44FEC9005789F1986549CFC541A4099E61BF3B65AF50A2
        B26DABA8CAA71FBCFC3741A40C1DF81105E9EE5B0CC8674B342E8D14D577D017
        1B7E03440268FB6614F98739E82FE138E32D0752D00BBB2D27B293443CCB53BA
        4315F61FADB8DE2DE19CB0AC72013DB0DF7222FFC10B96D200EA31F0503EF745
        89FE7C1545C159743319506911398E57BC5EF5D5C6D37474AB2BAE93255C1242
        AA732472D07222C035BA12CB2C31B436D425912715D7572406B5918A82F32472
        C0925BFDDB6985F6342387780BFC6A1B91F5C288A4487C4B4A66C99C233F78DA
        AB2676A1B946E951C677AB2D205389449A28AEBF937098F2E6C03506B956B88C
        C0242AC8F29A3587B18A31E9250FFB131140E77D4E519E3A0703247C4BEBE8C0
        7DF4C35A4B8894420C86737CB57639E6E118E3BA2F9066AAFFF7F0C7828D1F37
        6910E74A9C345D8406C3B0C81222B6F7D692376B8910CDDA830C8F1270C344FF
        F5D00CB2101F935D94380E92121F62AA65446404527BF5472B1D3289D4655B70
        DDFC6D244AB2180F2EA14098CDA0A811821749A6BDE66EAF1C3A30CBCD6F0791
        D01D6B84925D74528CAFA51B472889F4171C54073278D7A57C0E132941090DE9
        4028B717A5D22F3293BC76850A68478F355673513C9D23B380D1AA519FAC0427
        7A8E9C76B01389E4B0460B4DBADBD2C23E4230F5E250D4D7DC01944DCF3B0E7B
        91E5B69758CEEC5846EC5F93FC691B91B25484BD8546330BB7A8FA0E8F924C1F
        D532861DF7B0825666AF4B316B8E95451EE25EF675CE96327D977A588983546A
        9EAEAA7B8E2874E523446BD6FD4AB5BC8C164E1B41D886168AEB399860CB34B6
        A3DBA0DCC99B8DD616C524EEC9FC8164EA68D6A5D2CACCD5D49FB17C3AE5DAF3
        B7F40FAFCA44226914C5619EC5C8BDB8500DAF61201ED7A8C9A4C87FA5513E9D
        32A4C41ECEEA54994849FC43B537EB203597D90D979E200A6F51E957762A9F8F
        314E73A53C05AB9150F2392D5F964D11BECA0B65922E036F589482308E680C46
        671599381251C7FA311C2565E0918541F4DA0A357A0D4EAE5A42F3F5945D3727
        367C8026E88B9E74D16DB84BD1724E84544592B08C7E022FE38C9D4800B98B7B
        7B6FB1932DC54E4436738D39673A316A3DC599BA54C33C4A746A072BAE67D158
        E43B56753B50338B67105672C8EEC31F28899A08C365DDC0B80AC5DEBE66B591
        C6F58A8D9F0DA5398862A6E306A79F352196F5A8C031798EA325BB9B856180C3
        EBE92B4F190109142F8B0FD9F90E0E2265E93C3615EAF218642DF6F7037A4E04
        D4175FAA5CB9648ADBFFFCFD884508C693B82DFB55EE8894C32AC5864A1BE2A9
        86DDFBA2C581BE7893562617DBF191D676763132E880D5AA903417139D760DF9
        036FF129EC698C3DE88114D7444AE1134147CB48655CB7D5CF345EA4767A5471
        3D19535C13914F526D101276324EF00D1CF3238D5A14F0BA424912E7AE6A2799
        73D03998AE5A19555961F0E21794A2ED1EA92A4BE418B92512485F789053E956
        965DF60B9148CAC8EF55651F70C2ABA0950688A0296CE854BA995645E7C8834F
        51039BF89F12FBD1D139CFA37D5AE119AC29F2411DD841E7BEF8ADCA439CE8CA
        0339290CA3F63A37D33B08D393118AF381BA4318859DC54EA53DED9BDDA1BD8D
        01DA391EFDA349E3F0678DAF045CC6877E705BBAF30536862C54F3A9C134A14F
        241053314123FB9449221FFB74E29767281B8193F849E153441444F5C9FAF775
        75EA2D900F3C563391760093F08D4FB60004A20DDE67A41840273D1EE36DB186
        11B83E8718440B3A4E33F57C97E3B2103F5B4CE3098CC610C561DAE5F4AF0C06
        12EE4E869660F83F59E738DD694691F116AED63EC5FE1A0825999CE80637B51B
        3974FC2A6D6B459DBA93D4216BF0A3254456A08F53D910A3AAC5D831F036982D
        AC348A48E65B5B8B5DA6D69EE4EC48A2B04E6B434FD5A2874922A023398DAEA3
        FEB1FC5C0ADA26C698A768B08C2CD984229C133A45F17997DA74832255AD0EA2
        8BD1A3B4C6BF3010C498608CDB8F879CE5CD4FF2EF7B5D45501DCD51931E6D2D
        F6F8231614CD8132F85AB5A09D8CE1C637F878F6F18AA6B42C9D0DFD9BE398A7
        B9A8DA89AA4399814F45EF222A1DF145D192937CC87CBA272BFE9E7E85E32174
        E5C83430D03203BDB05155560FDB9CD4C60E6A267BFEAC071DF6C6C8C33E2ADE
        759E7D55C89B0FBC5461FC3C42337F2E6203474F8C1A66AA7618CB48E7083976
        42544414895CF07C15D3DB4FEE54C55074A30A70F5D989237856F556D56BFA36
        22F5E5DCAD5998F90852353ED64B88D1D9A8211F116EADCADD2E6020A04612FB
        B020356BF66B4E6174F05A337EA9AD41C7790FC5B354B1E2D73C2E71B25BB23A
        66CD87C222188CC630AAAC5FB4A73B1B7198A8B1703684C1817D83FE1906480B
        B1C79227B0F08B6772822F8253B50E03D30C9AC6449DC45E63FA09E13847057D
        9EE361596EF9FF1F75326B5C5E90B80000000049454E44AE426082}
      Proportional = True
    end
  end
  object scGPButton_obecne: TscGPButton
    Left = 10
    Top = 48
    Width = 120
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -19
    Font.Name = 'Roboto'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 22
    Animation = False
    Caption = ' Takt Time'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = -1
    ImageMargin = 25
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
    Options.FontNormalColor = clWhite
    Options.FontHotColor = clWhite
    Options.FontPressedColor = clWhite
    Options.FontFocusedColor = clWhite
    Options.FontDisabledColor = clWhite
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
  object scPanel_vozik: TscPanel
    Left = 450
    Top = 79
    Width = 430
    Height = 194
    TabOrder = 23
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
    Color = clWhite
    Caption = 'scPanel_vozik'
    StorePaintBuffer = False
    object rHTMLLabel_delka_jig: TrHTMLLabel
      Left = 255
      Top = 0
      Width = 28
      Height = 21
      Caption = '<font color=#2b579a>[m]</font>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = 19
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      OnClick = rHTMLLabel_delka_jigClick
    end
    object rHTMLLabel_delka_podvozek: TrHTMLLabel
      Left = 245
      Top = 173
      Width = 28
      Height = 21
      Caption = '<font color=#2b579a>[m]</font>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = 19
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      OnClick = rEditNum_delkavozikuClick
    end
    object rHTMLLabel_JIG: TrHTMLLabel
      Left = 407
      Top = 143
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
      Visible = False
      OnClick = rHTMLLabel_delkavozikuClick
    end
    object rHTMLLabel_podvozek_zaves: TrHTMLLabel
      Left = 294
      Top = 143
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
      Visible = False
      OnClick = rHTMLLabel_delkavozikuClick
    end
    object rHTMLLabel_sirka_jig: TrHTMLLabel
      Left = 355
      Top = 100
      Width = 28
      Height = 21
      Margins.Bottom = 0
      Caption = '<font color=#2b579a>[m]</font>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = 19
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      Visible = False
      OnClick = rHTMLLabel_sirka_jigClick
    end
    object rHTMLLabel_vyska_jig: TrHTMLLabel
      Left = 106
      Top = 80
      Width = 28
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Caption = '<font color=#2b579a>[m]</font>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = 19
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      OnClick = rHTMLLabel_sirka_jigClick
    end
    object rImageEx_jig_podlahovy: TrImageEx
      Left = 145
      Top = 19
      Width = 141
      Height = 158
      Align = alCustom
      Picture.Data = {
        0954574943496D61676549492A00080000001400FE0004000100000000000000
        00010300010000008B00000001010300010000009C0000000201030001000000
        0800000003010300010000000100000006010300010000000100000011010400
        0100000010500000120103000100000001000000150103000100000001000000
        16010300010000009C0000001701040001000000B45400001A01050001000000
        FE0000001B010500010000000601000028010300010000000200000031010200
        1C0000000E01000032010200140000002A010000BC020100BF3600003E010000
        4986010012180000FE3700006987040001000000C4A400005C930700081B0000
        F0A400000000000080FC0A001027000080FC0A001027000041646F6265205068
        6F746F73686F70204353352057696E646F777300323031393A30343A32392030
        303A31333A3436003C3F787061636B657420626567696E3D22EFBBBF22206964
        3D2257354D304D7043656869487A7265537A4E54637A6B633964223F3E0A3C78
        3A786D706D65746120786D6C6E733A783D2261646F62653A6E733A6D6574612F
        2220783A786D70746B3D2241646F626520584D5020436F726520352E302D6330
        36302036312E3133343737372C20323031302F30322F31322D31373A33323A30
        302020202020202020223E0A2020203C7264663A52444620786D6C6E733A7264
        663D22687474703A2F2F7777772E77332E6F72672F313939392F30322F32322D
        7264662D73796E7461782D6E7323223E0A2020202020203C7264663A44657363
        72697074696F6E207264663A61626F75743D22220A2020202020202020202020
        20786D6C6E733A786D703D22687474703A2F2F6E732E61646F62652E636F6D2F
        7861702F312E302F223E0A2020202020202020203C786D703A43726561746F72
        546F6F6C3E41646F62652050686F746F73686F70204353352057696E646F7773
        3C2F786D703A43726561746F72546F6F6C3E0A2020202020202020203C786D70
        3A437265617465446174653E323031392D30342D32395430303A31333A34352B
        30323A30303C2F786D703A437265617465446174653E0A202020202020202020
        3C786D703A4D65746164617461446174653E323031392D30342D32395430303A
        31333A34362B30323A30303C2F786D703A4D65746164617461446174653E0A20
        20202020202020203C786D703A4D6F64696679446174653E323031392D30342D
        32395430303A31333A34362B30323A30303C2F786D703A4D6F64696679446174
        653E0A2020202020203C2F7264663A4465736372697074696F6E3E0A20202020
        20203C7264663A4465736372697074696F6E207264663A61626F75743D22220A
        202020202020202020202020786D6C6E733A64633D22687474703A2F2F707572
        6C2E6F72672F64632F656C656D656E74732F312E312F223E0A20202020202020
        20203C64633A666F726D61743E696D6167652F746966663C2F64633A666F726D
        61743E0A2020202020203C2F7264663A4465736372697074696F6E3E0A202020
        2020203C7264663A4465736372697074696F6E207264663A61626F75743D2222
        0A202020202020202020202020786D6C6E733A786D704D4D3D22687474703A2F
        2F6E732E61646F62652E636F6D2F7861702F312E302F6D6D2F220A2020202020
        20202020202020786D6C6E733A73744576743D22687474703A2F2F6E732E6164
        6F62652E636F6D2F7861702F312E302F73547970652F5265736F757263654576
        656E7423223E0A2020202020202020203C786D704D4D3A496E7374616E636549
        443E786D702E6969643A33344439313745343032364145393131384234444643
        394441453041394343463C2F786D704D4D3A496E7374616E636549443E0A2020
        202020202020203C786D704D4D3A446F63756D656E7449443E786D702E646964
        3A33344439313745343032364145393131384234444643394441453041394343
        463C2F786D704D4D3A446F63756D656E7449443E0A2020202020202020203C78
        6D704D4D3A4F726967696E616C446F63756D656E7449443E786D702E6469643A
        3334443931374534303236414539313138423444464339444145304139434346
        3C2F786D704D4D3A4F726967696E616C446F63756D656E7449443E0A20202020
        20202020203C786D704D4D3A486973746F72793E0A2020202020202020202020
        203C7264663A5365713E0A2020202020202020202020202020203C7264663A6C
        69207264663A7061727365547970653D225265736F75726365223E0A20202020
        20202020202020202020202020203C73744576743A616374696F6E3E63726561
        7465643C2F73744576743A616374696F6E3E0A20202020202020202020202020
        20202020203C73744576743A696E7374616E636549443E786D702E6969643A33
        344439313745343032364145393131384234444643394441453041394343463C
        2F73744576743A696E7374616E636549443E0A20202020202020202020202020
        20202020203C73744576743A7768656E3E323031392D30342D32395430303A31
        333A34352B30323A30303C2F73744576743A7768656E3E0A2020202020202020
        202020202020202020203C73744576743A736F6674776172654167656E743E41
        646F62652050686F746F73686F70204353352057696E646F77733C2F73744576
        743A736F6674776172654167656E743E0A202020202020202020202020202020
        3C2F7264663A6C693E0A2020202020202020202020203C2F7264663A5365713E
        0A2020202020202020203C2F786D704D4D3A486973746F72793E0A2020202020
        203C2F7264663A4465736372697074696F6E3E0A2020202020203C7264663A44
        65736372697074696F6E207264663A61626F75743D22220A2020202020202020
        20202020786D6C6E733A70686F746F73686F703D22687474703A2F2F6E732E61
        646F62652E636F6D2F70686F746F73686F702F312E302F223E0A202020202020
        2020203C70686F746F73686F703A436F6C6F724D6F64653E313C2F70686F746F
        73686F703A436F6C6F724D6F64653E0A2020202020203C2F7264663A44657363
        72697074696F6E3E0A2020203C2F7264663A5244463E0A3C2F783A786D706D65
        74613E0A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020200A20202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        0A20202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020200A2020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020200A202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020200A20202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020200A2020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020200A202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020200A20
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020200A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020200A20202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        0A20202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020200A2020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020200A202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020200A20202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020200A2020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020200A202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020200A20
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020200A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020200A20202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        0A20202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020200A2020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020200A202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020200A20202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020200A2020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020200A202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020200A20
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020200A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        202020202020202020202020202020202020200A3C3F787061636B657420656E
        643D2277223F3E003842494D0425000000000010000000000000000000000000
        000000003842494D043A000000000093000000100000000100000000000B7072
        696E744F75747075740000000500000000436C7253656E756D00000000436C72
        53000000005247424300000000496E7465656E756D00000000496E7465000000
        00436C726D000000004D70426C626F6F6C010000000F7072696E745369787465
        656E426974626F6F6C000000000B7072696E7465724E616D6554455854000000
        010000003842494D043B0000000001B200000010000000010000000000127072
        696E744F75747075744F7074696F6E7300000012000000004370746E626F6F6C
        0000000000436C6272626F6F6C00000000005267734D626F6F6C000000000043
        726E43626F6F6C0000000000436E7443626F6F6C00000000004C626C73626F6F
        6C00000000004E677476626F6F6C0000000000456D6C44626F6F6C0000000000
        496E7472626F6F6C000000000042636B674F626A630000000100000000000052
        474243000000030000000052642020646F7562406FE000000000000000000047
        726E20646F7562406FE0000000000000000000426C2020646F7562406FE00000
        0000000000000042726454556E744623526C7400000000000000000000000042
        6C6420556E744623526C7400000000000000000000000052736C74556E744623
        50786C40520000000000000000000A766563746F7244617461626F6F6C010000
        000050675073656E756D00000000506750730000000050675043000000004C65
        6674556E744623526C74000000000000000000000000546F7020556E74462352
        6C7400000000000000000000000053636C20556E744623507263405900000000
        00003842494D03ED000000000010004800000001000200480000000100023842
        494D042600000000000E000000000000000000003F8000003842494D040D0000
        00000004000000783842494D04190000000000040000001E3842494D03F30000
        00000009000000000000000001003842494D271000000000000A000100000000
        000000023842494D03F4000000000012003500000001002D0000000600000000
        00013842494D03F700000000001C0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF03E800003842494D040000000000000200003842494D04020000
        0000000200003842494D043000000000000101003842494D042D000000000006
        0001000000023842494D04080000000000100000000100000240000002400000
        00003842494D041E000000000004000000003842494D041A00000000034B0000
        000600000000000000000000009C0000008B0000000B00420065007A0020006E
        00E1007A00760075002D00310000000100000000000000000000000000000000
        0000000100000000000000000000008B0000009C000000000000000000000000
        0000000001000000000000000000000000000000000000001000000001000000
        0000006E756C6C0000000200000006626F756E64734F626A6300000001000000
        000000526374310000000400000000546F70206C6F6E6700000000000000004C
        6566746C6F6E67000000000000000042746F6D6C6F6E670000009C0000000052
        6768746C6F6E670000008B00000006736C69636573566C4C73000000014F626A
        6300000001000000000005736C6963650000001200000007736C69636549446C
        6F6E67000000000000000767726F757049446C6F6E6700000000000000066F72
        6967696E656E756D0000000C45536C6963654F726967696E0000000D6175746F
        47656E6572617465640000000054797065656E756D0000000A45536C69636554
        79706500000000496D672000000006626F756E64734F626A6300000001000000
        000000526374310000000400000000546F70206C6F6E6700000000000000004C
        6566746C6F6E67000000000000000042746F6D6C6F6E670000009C0000000052
        6768746C6F6E670000008B0000000375726C5445585400000001000000000000
        6E756C6C54455854000000010000000000004D73676554455854000000010000
        00000006616C74546167544558540000000100000000000E63656C6C54657874
        497348544D4C626F6F6C010000000863656C6C54657874544558540000000100
        0000000009686F727A416C69676E656E756D0000000F45536C696365486F727A
        416C69676E0000000764656661756C740000000976657274416C69676E656E75
        6D0000000F45536C69636556657274416C69676E0000000764656661756C7400
        00000B6267436F6C6F7254797065656E756D0000001145536C6963654247436F
        6C6F7254797065000000004E6F6E6500000009746F704F75747365746C6F6E67
        000000000000000A6C6566744F75747365746C6F6E67000000000000000C626F
        74746F6D4F75747365746C6F6E67000000000000000B72696768744F75747365
        746C6F6E6700000000003842494D042800000000000C000000023FF000000000
        00003842494D041100000000000101003842494D041400000000000400000003
        3842494D040C000000000F40000000010000008B0000009C000001A40000FFF0
        00000F2400180001FFD8FFED000C41646F62655F434D0003FFEE000E41646F62
        6500648000000001FFDB0084000C08080809080C09090C110B0A0B11150F0C0C
        0F1518131315131318110C0C0C0C0C0C110C0C0C0C0C0C0C0C0C0C0C0C0C0C0C
        0C0C0C0C0C0C0C0C0C0C0C0C0C010D0B0B0D0E0D100E0E10140E0E0E14140E0E
        0E0E14110C0C0C0C0C11110C0C0C0C0C0C110C0C0C0C0C0C0C0C0C0C0C0C0C0C
        0C0C0C0C0C0C0C0C0C0C0C0C0C0CFFC0001108009C008B030122000211010311
        01FFDD00040009FFC4013F000001050101010101010000000000000003000102
        0405060708090A0B010001050101010101010000000000000001000203040506
        0708090A0B1000010401030204020507060805030C3301000211030421123105
        4151611322718132061491A1B14223241552C16233347282D14307259253F0E1
        F163733516A2B283264493546445C2A3743617D255E265F2B384C3D375E3F346
        2794A485B495C4D4E4F4A5B5C5D5E5F55666768696A6B6C6D6E6F63747576777
        8797A7B7C7D7E7F7110002020102040403040506070706053501000211032131
        12044151617122130532819114A1B14223C152D1F0332462E172829243531563
        7334F1250616A2B283072635C2D2449354A317644555367465E2F2B384C3D375
        E3F34694A485B495C4D4E4F4A5B5C5D5E5F55666768696A6B6C6D6E6F6273747
        5767778797A7B7C7FFDA000C03010002110311003F00F52AAAAA9A994D2C6D75
        56D0CAEB600D6B5AD1B58C631BED6B5AD53492494A55F32DB6A6566AD5CEB1AD
        8F104FD1956156CDE28FF8E67E5494D2C7CFEB4FCBB6BB708318D6CB06E89D79
        F5A1CC7AB7F68EA1FF00711BFF006E8FFC82B6924A6A7DA3A87FDC46FF00DBA3
        FF002097DA3A87FDC46FFDBA3FF20ADA4929A9F68EA1FF00711BFF006E8FFC82
        5F68EA1FF711BFF6E8FF00C82B6924A6A7DA3A87FDC46FFDBA3FF2097DA3A87F
        DC46FF00DBA3FF0020ADA4929A9F68EA1FF711BFF6E8FF00C825F68EA1FF0071
        1BFF006E8FFC82B6924A6A7DA3A87FDC46FF00DBA3FF0020AA579FD64E75949C
        20686C7BF746DD07F843EDB7FB2C5AC98F052521C1B1F6E1516D8773DEC6B9C7
        892423AABD33FE4EC6FF008A6FE4569253FFD0F554955CBCCB31DEC68A4D82C2
        1AD76E6B65C67D9EF3FC94DF6ACDFF00B84EFF00B719FF0092494D8B6DAE9ADD
        6DAE0C6304B9C74002C12ECBFAC37FE8DCEC6E9B51D1E347D8E1DDAB9BEB7D7F
        EB3753EBF95D1F03A6B726AE9E58FB28F503490E0D76EB5FB9ADFCEFCC57ABEB
        DFE30EAADB5D7F56AA631821AD1734003FEDC494EE7FCD8AFF00EE6E4FF9E97F
        CD8AFF00EE6E4FF9EB17FE717F8C6FFE772AFF00B79BFF00A512FF009C5FE31B
        FF009DCABFEDE6FF00E94494ED7FCD8AFF00EE6E4FF9E97FCD8AFF00EE6E4FF9
        EB17FE717F8C6FFE772AFF00B79BFF00A512FF009C5FE31BFF009DCABFEDE6FF
        00E94494ED7FCD8AFF00EE6E4FF9E97FCD8AFF00EE6E4FF9EB17FE717F8C6FFE
        772AFF00B79BFF00A512FF009C5FE31BFF009DCABFEDE6FF00E94494ED7FCD8A
        FF00EE6E4FF9E97FCD8AFF00EE6E4FF9EB17FE717F8C6FFE772AFF00B79BFF00
        A512FF009C5FE31BFF009DCABFEDE6FF00E94494ED7FCD8AFF00EE6E4FF9E97F
        CD8AFF00EE6E4FF9EB17FE717F8C6FFE772AFF00B79BFF00A512FF009C5FE31B
        FF009DCABFEDE6FF00E94494ED7FCD8AFF00EE6E4FF9EB27A9E2331AEFB2E164
        E46464B4175A0BFDAC681B9DBE3F3967753FAE7F5DB0594B33FA3D582DCCB5B8
        F5DFEA6E21CF3F9AD639FF009ABADA7A553D3BA5E4807D4BECA9E6EB8F2E3B5D
        FF00452533FAB849E8D8E499D1DCFF0059CB49667D5BFF009171BE0EFF00AA72
        D3494FFFD1F40EB784FC9B70DCDB8D5B2E01A009F7105C2CE7F3762D55533FE9
        62FF00C7B7FEA5EADA4A78AFABDFFE51FEB1FF00C553F92B5DAAE2BEAF7FF947
        FAC7FF00154FE4AD76A9294924924A5249249294924924A5249249294924924A
        78AFF19DFD1FA37FE9C6AFC8575D9FFD0723FE29FF00F525723FE33BFA3F46FF
        00D38D5F90AEBB3FFA0E47FC53FF00EA4A4A69FD5BFF009171BE0EFF00AA72D3
        599F56FF00E45C6F83BFEA9CB4D253FFD2F4ACFF00A58BFF001EDFFA97AB6AA6
        7FD2C5FF008F6FFD4BD5B494F15F57BFFCA3FD63FF008AA7F256BB55C57D5EFF
        00F28FF58FFE2A9FC95AED525292492494A4924925292492494A492492529249
        2494F15FE33BFA3F46FF00D38D5F90AEBB3FFA0E47FC53FF00EA4AE47FC677F4
        7E8DFF00A71ABF215D767FF41C8FF8A7FF00D49494D3FAB7FF0022E37C1DFF00
        54E5A6B33EADFF00C8B8DF077FD53969A4A7FFD3F4ACFF00A58BFF001EDFFA97
        AB6AA67FD2C5FF008F6FFD4BD5B494F15F57BFFCA3FD63FF008AA7F256BB55C5
        7D5EFF00F28FF58FFE2A9FC95AED525292492494A4924925292492494A492492
        5292492494F15FE33BFA3F46FF00D38D5F90AEBB3FFA0E47FC53FF00EA4AE47F
        C677F47E8DFF00A71ABF215D175CEA3562E33B1C0366464B4D7554DE4EE1B372
        4A57D5BFF9171BE0EFFAA72D359BD11AFC5C3AB0321BE9DEC69700750E04EEF6
        3BF91BBDEB49253FFFD4F4ACEFA589FF001EDFFA97AB6AA67FD2C5FF008F6FFD
        4BD5B494F9B33EB2F4AFABFF00E303AF5FD4EC7319732A633634BCC86D6E3A35
        6D7FE3A9F53FFD3DBFF6CBD754FC5C57B8BDF4B1CE3CB8B4127F051FB161FF00
        A0AFFCC6FF00724A797FFC753EA7FF00A7B7FED97A5FF8EA7D4FFF004F6FFDB2
        F5D47D8B0FFD057FE637FB92FB161FFA0AFF00CC6FF724A797FF00C753EA7FFA
        7B7FED97A5FF008EA7D4FF00F4F6FF00DB2F5D47D8B0FF00D057FE637FB92FB1
        61FF00A0AFFCC6FF00724A797FFC753EA7C4FAF6C7FC4BFF00B92FFC753EA7FF
        00A7B7FED97AADD731F1DBFE31FA0562A6063A8B8B9A1A20E96F2D5D8FD8B0FF
        00D057FE637FB9253CBFFE3A9F53FF00D3DBFF006CBD2FFC753EA7FF00A7B7FE
        D97AEA3EC587FE82BFF31BFDC97D8B0FFD057FE637FB9253CBFF00E3A9F53FFD
        3DBFF6CBD2FF00C753EA7FFA7B7FED97AEA3EC587FE82BFF0031BFDC97D8B0FF
        00D057FE637FB9253E67F5D3EBA744EBD5F4DA3A558FB2FA336BB4B5EC73440F
        6B7E97D2F7B977BD2BA5595DAEEA19E7D4CEB75D75158FDC62BCFE9F82F6963B
        1EA2D70823637FB956B2FC8E9EC754F06E61118B69E4B8FD0A2F71FF00A16FE7
        FF005D2531EB1D430F18329B83DD90F20D0DAC7B83BE8B5CC79F635137757E36
        D73B76CF6DFF004BD6FEAEDF67A7FE93FE0D57E97D2F24345BD576DF7B5C1D4B
        892E2C8D76CFD1FA5FBAB5D253FFD5F4ACFF00A58BFF001EDFFA97AB6B27ADE6
        D98D6E186D26D0FB8169063DC0168AF8FCEDEB55252E924924A5249249294924
        924A78BEBBFF00E52BEAFF00FE17BBF25ABB45C5F5DFFF00295F57FF00F0BDDF
        92D5DA24A5249249294924924A528D95B2C61AEC687B1C21CD22410A4924A5B8
        D13A4924A7FFD6F4DC9C4390F638DAE60AC873000D30E13EFF007B5DFBC97D9F
        27FEE53FFCDAFF00F49AB0924A7CE3F67F58EB9F5CBACF4F675BCCC2AB0BD37B
        3D27183BDACF6FA6D754C67F6569FF00CC0EB1FF00CF4750FBDDFF00A594FA8F
        D40E95D5BAEE7665D93954DB68A9EE14BDAD125A59FE8DDFE8943FF1ABE8BFF7
        3BA87FDBCDFF00D249295FF303AC7FF3D1D43EF77FE964BFE60758FF00E7A3A8
        7DEEFF00D2CB9EC0FA9B8195F5C7A8F427E6E5FD970E865B53DB68F50B9C2ADC
        1EEDBB3FC23BF31743FF008D5F45FF00B9DD43FEDE6FFE92494AFF00981D63FF
        009E8EA1F7BBFF004B25FF00303AC7FF003D1D43EF77FE964BFF001ABE8BFF00
        73BA87FDBCDFFD2497FE357D17FEE7750FFB79BFFA492521B3FC59E6D9935E5D
        9F58B31F934822ABDC25EC07E90AECF57733946FF981D63FF9E8EA1F7BBFF4B2
        5FF8D5F45FFB9DD43FEDE6FF00E924BFF1ABE8BFF73BA87FDBCDFF00D249295F
        F303AC7FF3D1D43EF77FE964BFE60758FF00E7A3A87DEEFF00D2C97FE357D17F
        EE7750FF00B79BFF00A492FF00C6AFA2FF00DCEEA1FF006F37FF004924A57FCC
        0EB1FF00CF4750FBDDFF00A592FF00981D63FF009E8EA1F7BBFF004B2E77EB7F
        D4EC0E87FB3062E665BCE6E5B31EDF56D0618EFA4E66D6B3DCBA2FFC6AFA2FFD
        CEEA1FF6F37FF4924A71BEB1F47EB5F571DD3B2075FCDCB19399552EADEF7344
        13B8F163B77D15E9AB86BBFC5A746C6BB12D19799616E45702CB1AE1A1DFFE8F
        F92BB9494A49249253FFD7F5549249253528FF009472BFA957FE8D5C4FD73EBB
        959BD5327A35196EE9FD2BA6522EEAF9757F38EDFB7D3C5A78F7D9EA32B633F7
        FF009CFD1D6BB6A3FE51CAFEA55FFA3579B7D64E9FE9FD61EB9D2329E281D7EB
        AB23A7E45876D66EA4878C7B2C3ED67A8EF52AFF00B692538EEC2E9D4E3D59F6
        749EA3D3302C23D1EB02F2FB1A5DFCDDF6D1E9D6DD8EFF0083755FF0562EFF00
        EA3F5EEA1916E5F42EAF60BF3FA706BEBCA6F17D0F00D37FF2BDAE67BFF72C58
        5D4FABFD69EABD00FD5DBBA2BF0AC2C655999F79D98CCAEB2D2EB858E02B6FF3
        7FE97FE2F7AB5F50A96E67D62CFEAD8B2EE9B8B8D4F4DC6BC823D53536A63ED1
        3FF13BFF00EBAC494FA0249249294924924A52E77EBAFD60C9E8DD3AAAFA7B77
        F53EA168C6C269130E77366DFCED9FF56BA25C6FF8C6AEFC6FD91D7ABACDB574
        7CB16E431BCFA6E2CF7FF9D5ECFEDA4A78C7F4DAB2F2B22A6E166FD67CEC631D
        4335B79AEA6D9F9F562C32C759B1DEDF7FF39FE0E9D8B4FEAAFD60B3A3E5E17A
        1917647D5FEA369C57519266DC3C9D3F44E7FEE7BDAEF6FB2CABFC1FAB52B1D0
        7A9F56FAB4DCBABA774DB3AE74ACFB9D93819789EED5FF00E0B23636C756F66D
        DB632CF7B1EB22AE9F97939B8BD16D0D3D57A8F521D4FA8535105B8D5B27F476
        164B5B6EDB2DB1ECFF0007FA1AFE9A4A7D5BA8738DFF008619FF007E56D54EA1
        CE37FE1867FDF95B494A49249253FFD0F554924925397919C30B2F2AD355977B
        2AD2B6EE8FE77E97EEAA59D4F4BFACBD2D95F53E9F6DB5BA5CC2C6925864B775
        57376BB72DE6FA5EA3B6EDF5206F8898D766E526ED8F6C47924A78267F8BDE8B
        B836FB3AAE462B4CB715EEF608EDED0DFF00A2BACC37E26063578987836D18F5
        08656CAE001F7AD24925353F687FDD6C8FFB6FFDA97ED0FF00BAD91FF6DFFB55
        B4925353F687FDD6C8FF00B6FF00DA97ED0FFBAD91FF006DFF00B55B4925353F
        687FDD6C8FFB6FFDAA36E5D5756EAADC4BACADE0B5EC7552D20E8E6B9A55D492
        53C1DFF503A19B9F6611EA7D3EBB0CBE8C730C3E403F73BFE92D9E89D23A37D5
        CC7B7F6760DE2C7826DBED69758F8F77BEDFCD67F5174698C419E3BCA4A716AE
        AC3A8B319DE8594917B3DCE1EC3A3BE859F9CB6D41DE8C337ED891E9CC7D2FCD
        D9FCA53494A49249253FFFD93842494D04210000000000550000000101000000
        0F00410064006F00620065002000500068006F0074006F00730068006F007000
        00001300410064006F00620065002000500068006F0074006F00730068006F00
        70002000430053003500000001003842494D0FA00000000000F86D616E694952
        4652000000EC3842494D416E4473000000CC0000001000000001000000000000
        6E756C6C0000000300000000414653746C6F6E6700000000000000004672496E
        566C4C73000000014F626A63000000010000000000006E756C6C000000010000
        0000467249446C6F6E674BD75D020000000046537473566C4C73000000014F62
        6A63000000010000000000006E756C6C0000000400000000467349446C6F6E67
        00000000000000004146726D6C6F6E67000000000000000046734672566C4C73
        000000016C6F6E674BD75D02000000004C436E746C6F6E670000000000003842
        494D526F6C6C0000000800000000000000003842494D0FA100000000001C6D66
        7269000000020000001000000001000000000000000100000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFCFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFF9FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFD6C7FFFFFFFFE7E4FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFF8D9F6FFFFFFF7A6EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C2FFF9D8AE89C1FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF49397BEE8FFF89DECFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFD4B8C497898A8C9FB3B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1
        B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1
        B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B2AE91
        8C888CA5CC9FEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFD4BCD8A98F8889ABCFCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        CCCCCCCCCCCCCCCCCDC693898997B9DA9FEDFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C3FFFFE9C092C2
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF497A5D2F6FFF89FEDFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFD2C3FFFFFFFFF4EDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA
        E8FDFFFFFFF79EEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79EEDFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C3FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79EEDFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFD2C3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFF79EEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79EEDFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C3FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79EEDFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFD2C3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFF79EEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2C3FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF79EEDFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD4C5FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9A0ED
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFCBA7CBC8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8
        C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8
        C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8C8
        C8C8C8C8C8C8C8C9C495C5FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDB948A88888888888888888888888888
        8888888888888888888888888888888888888888888888888888888888888888
        8888888888888888888888888888888888888888888888888888888888888888
        88888888888888888888888888888888888989898B97F8FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD88F8B8B8D8D
        8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D
        8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D
        8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D918F8F8C
        9BF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFD58E8B8ABCE3E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0
        E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0
        E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0
        E0E0E0E0E0E1E5AF8C8F8C9BF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFD08D8C8AC1FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF8F8D8E8C9BF8FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCD8C8B8AC6FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCB8B8B9094
        8C9BF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFC98B8C8ACAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFCB8B8B8DD3AC889BF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFC68A8B8CCEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD08B8B8CD1FFA9879BF8FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1898C8DD1FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD18C8C8BD0FFFF
        A9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE
        BD898B8ED5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFD08C8B8ACBFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC08F939DE4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFD78F8B8BCBFFFFFFFFA9879BF8FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE7734E53526C8D888888888888
        8888888888888888888888888888888888888888888888888888888888888888
        8888888888888888888888888888888888888888888888888888888888888888
        888888888888888888888888888888888888888888888A81516F8ECAFFFFFFFF
        FFA9879BF8FFFFFFFFFFFFFFFFFFFFF8D2CFD1D1D0D0D0D0D0D0D0D0D0D7A815
        181A191612121212121212121212121212121212121212121212121212121212
        1212121212121212121212121212121212121212121212121212121212121212
        1212121212121212121212121212121212121212121212121212121212121212
        1212141256D1FFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFF4B0ADA7AAAE
        ADADADADADADADADB291211F1E1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B
        1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B
        1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B1B
        1B1B1B1B1B1B1B1B1B1B1B1B1C201587FFFFFFFFFFFFFFA9879BF8FFFFFFFFFF
        FFFFFFFFFFFFFFFFC5DEFFFFFFFFFFFFFFFFFFFFCE1E1370D8CDCDCDCDCDCDCD
        CDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCD
        CDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCD
        CDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDCDD59D1D1396FFFFFFFFFF
        FFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFF997B6FFFFFFFFFFFFFFFFFFFFCE
        1E108AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFC41C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFD78897
        F3FFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFF
        FFFFFFFFFFFFFFFFAD8A89CFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFF
        FFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFE78B8D87A2FEFFFFFFFFFFFFFFFF
        CE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFD6A997
        A0AFF0FFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFF
        FFFFFFFFFFFFFFFFFFFFAFD2FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFF
        FFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFF
        FFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFF
        AED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFF
        FFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFF
        FFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFF
        FFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFF
        FFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FF
        FFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FF
        FFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFF
        FFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFF
        FFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8
        FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394
        FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFF
        FFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFF
        FFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879B
        F8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C13
        94FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFF
        FFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFF
        FFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA987
        9BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C
        1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFF
        FFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFF
        FFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9
        879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1
        1C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFF
        FFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFF
        FFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFF
        A9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E10
        87FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        C11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFF
        FFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFF
        FFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFF
        FFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E
        1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FF
        FFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFF
        FFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFF
        FFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE
        1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1
        FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFF
        FFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFF
        FFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFF
        CE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAE
        D1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFF
        FFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFF
        FFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFF
        FFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFF
        AED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFF
        FFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFF
        FFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFF
        FFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FFFFFFFFFFFFFFFFFFFFFFFF
        FFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFA9879BF8FF
        FFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FF
        FFFFFFFFFFFFA8889BF8FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFF
        FFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFC11C1394FFFFFFFFFFFFF79F899CF9FFFFFFFFFFFFFFFFFFFFFF
        FFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFF9AE8B8B91E0
        FFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394
        FFFFFFFFF9AD888A9DD7A5DFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFF
        FFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFC11C1394FFFFFFFAAD888A9AEBFFEAA3DBFFFFFFFFFFFFFFFF
        FFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFF9AD888A9AEAFFFF
        FFEBA3DBFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C13
        94FFFCB0888A9AEAFFFFFFFFFFECA3DBFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFF
        FFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFC11C1396FFB3888A9AEBFFFFFFFFFFFFFFECA3D9FFFFFFFF
        FFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1491BF898A97E8FFFFFFFF
        FFFFFFFFFFEEA4D7FFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1087FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C
        1960928A96E6FFFFFFFFFFFFFFFFFFFFFFF3A2D6FFFFFFFFFFFFAED1FFFFFFFF
        FFFFFFFFFFFFCE1E108BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFC41D195A9096E6FFFFFFFFFFFFFFFFFFFFFFF3C7C1A8D6
        FFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E165EADA5A5A5A5A5A5A5A5A5A5
        A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5
        A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5
        A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5AB801E1A569CE6FFFFFFFFFFFF
        FFFFFFFFEBBD948AEAF3AEF3FFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFCE1E1F1B
        1314141414141414141414141414141414141414141414141414141414141414
        1414141414141414141414141414141414141414141414141414141414141414
        1414141414141414141414141414141414141414141414141414141414141317
        201865ECFFFFFFFFFFFFFFFFFFF7B2908987AFFFFFFAFEFFFFFFFFAED1FFFFFF
        FFFFFFFFFFFFFFCB111113131313131313131313131313131313131313131313
        131313131313131313131313131313131313131313131313121B211F15131313
        1313131313131313131313131313131313131313131313131313131313131313
        131313131313131313131313088DFFFFFFFFFFFFFFFFFFFFFFC68A8D8BDAFFFF
        FFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFEEB2B3B3B3B3B3B3B3B3B3B3B3B3
        B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3
        B3B3B3B95716269CB8B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3
        B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B69589EDFFFFFFFFFFFF
        FFF3F9FFCB938C9EFAFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF76102BDFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC695E8FFFFFFFFFFFFB6B4C997EACBC8FFFFFFFFFFFFFFFFFFAED1FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC795E8FFFFFFFFF196888EE2FFFEFCFFFF
        FFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC794E5FFFF
        FFCA888E88B5FDFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFCB93E4FFFBA0868BA1CBFAFFFFFFFFFFFFFFFFFFFFFFAED1FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCC93EADC8BAEDCFCFFFFFFFFFF
        FFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCD
        94BCEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFD194E4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAED1
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DD
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD196EFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFE7F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAE
        D1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129
        DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        AED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7311
        29DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF73
        1129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFAED1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFAED2FFFEFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        D3A3969DA9EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFE88B8C87A3FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAE8989D0FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF731129DDFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFD98897F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FF74112ADEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFA98B7FFFFFFFFFFFFFFFFFFFFFAE9EAEAEAEA
        EAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEA
        EAEAEAEAEAEAEAEAEAEAEBF26B1228CBF0EAEAEAEAEAEAEAEAEAEAEAEAEAEAEA
        EAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAE9
        F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7DFFFFFFFFFFF
        FFFFFFFFFFC9272F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F
        2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F30251F212D2F2F2F2F2F
        2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F
        2F2F2F2F2F2F2F2F2F2F249BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFF4B4B1ADAFB1B1B1B1B1B1B1B1B1B88A161C17111212121212121212121212
        1212121212121212121212121212121212121212121212121212121212121212
        1212141616131212121212121212121212121212121212121212121212121212
        12121212121212121212121212121212121212151C1092FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFCECEBECECEBEBEBEBEBEBEBEBEBF5B6181C4165
        6161616161616161616161616161616161616161616161616161616161616161
        6161616161616161616161616161616161616161616161616161616161616161
        6161616161616161616161616161616161616161616161616161616161644F1F
        1494FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFC5171396FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFC31C1394FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5171393FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC11C1394FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5171393
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1
        1C1394FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFC5171396FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC31C1394FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5181C3F615D5D5D5D5D5D5D5D5D
        5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D
        5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D
        5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D5D604C1F1494FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4131E
        1611121212121212121212121212161615121212121212121212121212121212
        1212121212121212121212121212121212121212121212121212121212121212
        1212121212121212121212121212121214161613121212121212121212121212
        151D1192FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFCA272B3131313131313131313131313130221F2431313131
        3131313131313131313131313131313131313131313131313131313131313131
        3131313131313131313131313131313131313131313131313131322B1F1F2B32
        3131313131313131313131312C229DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCA9CEEEEEDEDEDEDEDEDEDED
        EDEDF0E2331352F1EEEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDED
        EDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDED
        EDEDEDEDEDF9A01613A2F9EDEDEDEDEDEDEDEDEDEDEDF2ADB4FDFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDB
        AAFFFFFFFFFFFFFFFFFFFFFFFFFFF5351257FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAC1512AFFFFFFFFFFFFFFFFFFFFFFF
        FFFFBBC3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFFFFFFFFFFFFFFF3351257FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAC1512AE
        FFFFFFFFFFFFFFFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFFFFFF
        FFFFFFFFF6361258FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFAC1612B0FFFFFFFFFFFFFFFFFFFFFFFFFFB9C2FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        DAA7FFFFFFFFFFFFFFFFFFFFFFFFF8CF2F144CECFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF58F171496FFFFFFFFFFFFFFFFFFFF
        FFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFFFFFFFFE4843D241F20202E5B
        B6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC165311F1F1F
        233B78D8FFFFFFFFFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFFFF
        FFA12E1214111415121213165DE4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFE9691A12131115141214112595FEFFFFFFFFFFFFFFFFB9C2FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFDAA7FFFFFFFFFFFFFFFF7E1417153679A3AB95591F161237D7FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFE64511171C508EA8A7813E16171273FCFFFFFF
        FFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFF9112192294FAF7D4C6DBF9
        D64D151442EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF650131541C9FFE9CA
        CBE7FAA42718128CFFFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFD6
        241724BFFF94311A151D41BBFC5E151278FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFF8C11164AF0E6652416172464ECCE29181EC7FFFFFFFFFFFFB9C2FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFDAA7FFFFFFFFFFFF6F1317A1FF6511161314131616A1F33D1621D1FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFE22C1633E6D42D13151313151237E8B2181459FF
        FFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFE62D1449FF8210182E89A477
        21191BCEAD171187FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9D12159BF83E151753
        9E9C5017134FF7571423D7FFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFB9
        16149AEE2A1731D0FFFFFFA51C1363F4371252FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFF63122AEC9C111767FAFFFFF85C1714C1B51714A7FFFFFFFFFFB9C2FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFDAA7FFFFFFFFFF99121CCAC0151492FFFFFFFFFF531238F05D1137EEFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFD461147FF63122ADEFFFFFFFFC91F1287E02212
        7BFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFF89131FD7B21217ADFFFFFF
        FFFF6B1030EB6C1030E6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF53F1152FF551237F5
        FFFFFFFFE72A1171EA261272FFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFF
        93121CCAD31C1476FFFFFFFFF443133FF75C1139F1FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFD471242FF781120C6FFFFFFFFB81B1192DD211284FFFFFFFFFFB9C2FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFDAA7FFFFFFFFFFBB181397FF401621B1FFFFFA7B181178F6351251FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFF691227E9C0161746E2FFFFDB3E181AD5B116
        13A7FFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFE92F1447FFB2151A1E5D
        7E4318162CE9AD171188FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA1121490FF6010
        182F76702C181070FF581423D9FFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFF
        FFFF6A131599FF8716141310141128C4EF3A1623D4FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFE42D162DE1EF4C10151111141158FAA918135DFFFFFFFFFFFFB9C2FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFDAA7FFFFFFFFFFFFD5241822B4FFBE5428233872E3F45016117EFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9212164EEDF88E3D2328479DFFBB2318
        21CCFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFF9C14191F8AF4FF
        EEE4FAFFC641151447F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA5611163C
        BEFFFDE5ECFFF89421191390FFFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFF
        FFFFFFFFFF87141715316E979C844B1B161144E2FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFE54A11171948839E976F3415171484FFFFFFFFFFFFFFFFB9C2
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFDAA7FFFFFFFFFFFFFFFFFFA83111131112121113121B6DEBFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0721B12131113121113112DA1
        FFFFFFFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFFFFFFFFFFFFFFFFFFE58A46
        27191820396DC4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        C3693720181A274A8BE2FFFFFFFFFFFFFFFFFFFFB9C2FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDAA7FFFF
        FFFFFBE9FDFFFFFFFFFFFBDDC5C3D3F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFF1D2C3C8DEFCFFFFFFFFFFFFEDF1FFFFFFFFB9
        C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFDAA7FFFFF1CB9EA5FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFC692BBE5FEFFB9C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDBA5DAB093898998C1C3C2C2C2C2
        C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2
        C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2
        C2C2C2C2C2C2C2C2C2C2C2C2C2C2C5A88A888DA3CFB0C4FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDBA4CF
        A18B898B98BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
        BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
        BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDA58B8A8996C2
        ACC4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFD9A5FFFEE5BB93A1FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFC589ABD6F7FFB7C1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDEB1FFFFFFFFF2DCFDFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE5E3FEFFFFFFC1CAFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFA
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFBFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFF030001A0030001000000FFFF000002A00400
        010000008B00000003A00400010000009C00000000000000000041646F626520
        50686F746F73686F7020446F63756D656E74204461746120426C6F636B004D49
        42387279614C961A0000010000000000000000009C0000008B0000000200FFFF
        1E0600000000081300004D4942386D726F6EFF00080040010000000000002800
        00000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000
        FFFF0000FFFF0000FFFF0856727374766120310000004D494238696E756C1400
        000008000000560072007300740076006100200031004D49423872736E6C0400
        00007279616C4D4942386469796C04000000020000004D4942386C626C630400
        0000010000004D49423878666E6904000000000000004D4942386F6B6E6B0400
        0000000000004D4942386670736C04000000000000004D494238726C636C0800
        000000000000000000004D494238646D687348000000010000004D4942387473
        7563000000003400000010000000010000000000080000006D65746164617461
        01000000090000006C6179657254696D6562756F643915334D8931D741004D49
        423870727866100000000000000000000000000000000000000001000A000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000A0001DEF383EEF8EE01F3DE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE82FFF7FF00EE00EE
        82FFF7FF00EE00EE82FFF7FF00EE01DEF383EEF8EE01F3DE010004000D001900
        1A001C001C001A0019000E000E000E000E000E000E000E000E00110011001300
        1700150016001700180019001B001A001D00220021001F001E001F001F002100
        21001D001D001D001D001D001D001D001D001D001D001D001D001D001D001D00
        1D001D001D001D001D001D001D001D001D001D001D001D001D001D001D001D00
        1D001D001D001D001D001D001D001D001E001F00210023002400240023002200
        2100210024002400240028001F0020001E001D001A0016001500140013001000
        10001000100010001000100010001000140014001400120012001E001B002200
        1C00120012001200120014002000200025001C001C001C001F0029002F003300
        360038003A003C003C003B003B003C003C003C003A003800360032002E002800
        2B001B001D001B001B001A000E000400040081FFF6FFE5FF01FCFBA0FF00F900
        FEF7FFE5FF01D6C7FDFF01E7E4ADFF02F8D9F6FEFF01F7A600EEF7FFE5FF07D2
        C2FFF9D8AE89C1ADFF07F49397BEE8FFF89D00ECF7FFE5FF08D4B8C497898A8C
        9FB3AFB108B2AE918C888CA5CC9F00EDF7FFE5FF08D4BCD8A98F8889ABCFAFCC
        08CDC693898997B9DA9F00EDF7FFE5FF07D2C3FFFFE9C092C2ADFF07F497A5D2
        F6FFF89F00EDF7FFE5FF01D2C3FDFF01F4EDADFF02FAE8FDFEFF01F79E00EDF7
        FFE5FF01D2C3A1FF01F79E00EDF7FFE5FF01D2C3A1FF01F79E00EDF7FFE5FF01
        D2C3A1FF01F79E00EDF7FFE5FF01D2C3A1FF01F79E00EDF7FFE5FF01D2C3A1FF
        01F79E00EDF7FFE5FF01D2C3A1FF01F79E00EDF7FFE5FF01D2C3A1FF01F79E00
        EDF7FFE5FF01D4C5A1FF01F9A000EDF7FFE5FF02CBA7CBA3C802C9C49501C5FE
        F8FFE6FF02DB948AA388FE89008B0197F8F8FFE7FF03D88F8B8BA38D03918F8F
        8C019BF8F8FFE8FF05D58E8B8ABCE3A6E005E1E5AF8C8F8C019BF8F8FFE9FF04
        D08D8C8AC1A3FF04CF8F8D8E8C019BF8F8FFEAFF04CD8C8B8AC6A3FF05CB8B8B
        90948C019BF8F8FFEBFF04C98B8C8ACAA3FF06CB8B8B8DD3AC88019BF8F8FFEC
        FF04C68A8B8CCEA3FF07D08B8B8CD1FFA987019BF8F8FFEDFF04C1898C8DD1A3
        FF08D18C8C8BD0FFFFA987019BF8F8FFEFFF05FEBD898B8ED5A3FF04D08C8B8A
        CBFEFF01A987019BF8F8FFEFFF04C08F939DE4A3FF04D78F8B8BCBFDFF01A987
        019BF8F8FFF1FF06E7734E53526C8DA588058A81516F8ECAFCFF01A987019BF8
        F8FF05FFF8D2CFD1D1F8D006D7A815181A1916A31203141256D1FBFF01A98701
        9BF8F8FF06FFF4B0ADA7AAAEF9AD04B291211F1EA21B031C201587FAFF01A987
        019BF8F8FFFDFF01C5DEF7FF04CE1E1370D8A4CD04D59D1D1396FAFF01A98701
        9BF8F8FFFEFF02F997B6F7FF03CE1E108AA2FF03C41C1394FAFF01A987019BF8
        F8FFFEFF03D78897F3F8FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8
        FFFEFF03AD8A89CFF8FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FF
        FFFF05E78B8D87A2FEF9FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8
        FFFFFF05D6A997A0AFF0F9FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8
        F8FFFDFF01AFD2F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFD
        FF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AE
        D1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF
        03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E
        1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2
        FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C1
        1C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394
        FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01
        A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A98701
        9BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8
        FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF
        01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1
        F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03
        CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E10
        87A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF
        03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C
        1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FA
        FF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A9
        87019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019B
        F8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FF
        FDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01
        AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7
        FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE
        1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087
        A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03
        C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C13
        94FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF
        01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987
        019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8
        F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFD
        FF01AED1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AE
        D1F7FF03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF
        03CE1E1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E
        1087A2FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2
        FF03C11C1394FAFF01A987019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C1
        1C1394FAFF01A888019BF8F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394
        FBFF02F79F89019CF9F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FCFF
        03F9AE8B8B0191E0F8FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FDFF04
        F9AD888A9D02D7A5DFF9FFFDFF01AED1F7FF03CE1E1087A2FF03C11C1394FEFF
        05FAAD888A9AEB03FFEAA3DBFAFFFDFF01AED1F7FF03CE1E1087A2FF0CC11C13
        94FFFFF9AD888A9AEAFFFFFF02EBA3DBFBFFFDFF01AED1F7FF03CE1E1087A2FF
        0CC11C1394FFFCB0888A9AEAFFFFFEFF02ECA3DBFCFFFDFF01AED1F7FF03CE1E
        1087A2FF09C11C1396FFB3888A9AEBFEFFFDFF02ECA3D9FDFFFDFF01AED1F7FF
        03CE1E1087A2FF08C11C1491BF898A97E8FDFFFCFF02EEA4D7FEFFFDFF01AED1
        F7FF03CE1E1087A2FF07C11C1960928A96E6FCFFFBFF04F3A2D6FFFFFDFF01AE
        D1F7FF03CE1E108BA2FF06C41D195A9096E6FBFFFCFF05F3C7C1A8D6FFFDFF01
        AED1F7FF04CE1E165EADA4A506AB801E1A569CE6FAFFFEFF07EBBD948AEAF3AE
        F2FDFF01AED1F7FF04CE1E1F1B13A414051317201865ECF9FF0AFFF7B2908987
        AFFFFFFAFEFDFF01AED1F7FF02CB1111D31304121B211F15D21301088DF8FFFF
        FF04C68A8D8BDAFDFFFDFF01AED1F7FF01EEB2D2B305B95716269CB8D4B303B6
        9589EDFAFF00F306F9FFCB938C9EFAFDFFFDFF01AED1C5FF0376102BDFD1FF02
        C695E8FBFF00B605B4C997EACBC8FCFFFDFF01AED1C5FF03731129DDD0FF02C7
        95E8FDFF01F19605888EE2FFFEFCFCFFFDFF01AED1C5FF03731129DDCFFF02C7
        94E5FEFF01CA88038E88B5FDFAFFFDFF01AED1C5FF03731129DDCEFF06CB93E4
        FFFBA086038BA1CBFAFAFFFDFF01AED1C5FF03731129DDCDFF05CC93EADC8BAE
        01DCFCF8FFFDFF01AED1C5FF03731129DDCCFF04CD94BCEBFFF6FFFDFF01AED1
        C5FF03731129DDCBFF03D194E4FFF6FFFDFF01AED1C5FF03731129DDCAFF02D1
        96EFF6FFFDFF01AED1C5FF03731129DDC9FF01E7F9F6FFFDFF01AED1C5FF0373
        1129DDC7FFF6FFFDFF01AED1C5FF03731129DDC7FFF6FFFDFF01AED1C5FF0373
        1129DDC7FFF6FFFDFF01AED1C5FF03731129DDC7FFF6FFFDFF01AED1C5FF0373
        1129DDC7FFF6FFFDFF01AED1C5FF03731129DDC7FFF6FFFDFF01AED1C5FF0373
        1129DDC7FFF6FFFDFF01AED1C5FF03731129DDC7FFF6FFFDFF01AED1C5FF0373
        1129DDC7FFF6FFFFFF05FEFFAED2FFFEC7FF03731129DDC7FFF6FFFFFF05D3A3
        969DA9EEC7FF03731129DDC7FFF6FFFFFF05E88B8C87A3FEC7FF03731129DDC7
        FFF6FFFEFF03AE8989D0C6FF03731129DDC7FFF6FFFEFF03D98897F3C6FF0374
        112ADEC7FFF6FFFEFF02FA98B7F7FF01FAE9D3EA06EBF26B1228CBF0D3EA01E9
        F5F8FFF6FFFDFF01C7DFF7FF01C927D22F0430251F212DD22F01249BF8FFF6FF
        05FFF4B4B1ADAFF8B105B88A161C1711D4120314161613D41203151C1092F8FF
        F6FF05FFFCECEBECECF8EB05F5B6181C4165A46104644F1F1494F8FFF6FFF1FF
        03C5171396A2FF03C31C1394F8FFF6FFF1FF03C5171393A2FF03C11C1394F8FF
        F6FFF1FF03C5171393A2FF03C11C1394F8FFF6FFF1FF03C5171396A2FF03C31C
        1394F8FFF6FFF1FF04C5181C3F61A45D04604C1F1494F8FFF6FFF1FF04C4131E
        1611F512FF160015C2120314161613F51203151D1192F8FFF6FFF1FF02CA272B
        F4310330221F24C33105322B1F1F2B32F531022C229DF8FFF6FFF0FF03CA9CEE
        EEF7ED06F0E2331352F1EEC5ED05F9A01613A2F9F6ED03F2ADB4FDF8FFF6FFF0
        FF01DBAAF4FF03F5351257C2FF03AC1512AFF4FF01BBC3F7FFF6FFF0FF01DAA7
        F4FF03F3351257C2FF03AC1512AEF4FF01B9C2F7FFF6FFF0FF01DAA7F4FF03F6
        361258C2FF03AC1612B0F4FF01B9C2F7FFF6FFF0FF01DAA7F5FF05F8CF2F144C
        ECC4FF04F58F171496F4FF01B9C2F7FFF6FFF0FF01DAA7F7FF0AE4843D241F20
        202E5BB6FBC9FF02C16531FE1F03233B78D8F7FF01B9C2F7FFF6FFF0FF01DAA7
        F8FF0CA12E1214111415121213165DE4CCFF0DE9691A12131115141214112595
        FEF9FF01B9C2F7FFF6FFF0FF01DAA7F9FF0E7E1417153679A3AB95591F161237
        D7CEFF0FE64511171C508EA8A7813E16171273FCFAFF01B9C2F7FFF6FFF0FF01
        DAA7FAFF109112192294FAF7D4C6DBF9D64D151442EFD0FF10F650131541C9FF
        E9CACBE7FAA42718128CFAFF01B9C2F7FFF6FFF0FF01DAA7FBFF11D6241724BF
        FF94311A151D41BBFC5E151278D0FF118C11164AF0E6652416172464ECCE2918
        1EC7FBFF01B9C2F7FFF6FFF0FF01DAA7FBFF126F1317A1FF6511161314131616
        A1F33D1621D1D2FF12E22C1633E6D42D13151313151237E8B2181459FBFF01B9
        C2F7FFF6FFF0FF01DAA7FCFF13E62D1449FF8210182E89A47721191BCEAD1711
        87D2FF139D12159BF83E1517539E9C5017134FF7571423D7FCFF01B9C2F7FFF6
        FFF0FF01DAA7FCFF08B916149AEE2A1731D0FEFF07A51C1363F4371252D2FF13
        63122AEC9C111767FAFFFFF85C1714C1B51714A7FCFF01B9C2F7FFF6FFF0FF01
        DAA7FCFF0799121CCAC0151492FCFF07531238F05D1137EED4FF08FD461147FF
        63122ADEFDFF07C91F1287E022127BFCFF01B9C2F7FFF6FFF0FF01DAA7FCFF07
        89131FD7B21217ADFCFF076B1030EB6C1030E6D4FF08F53F1152FF551237F5FD
        FF07E72A1171EA261272FCFF01B9C2F7FFF6FFF0FF01DAA7FCFF0793121CCAD3
        1C1476FDFF08F443133FF75C1139F1D4FF08FD471242FF781120C6FDFF07B81B
        1192DD211284FCFF01B9C2F7FFF6FFF0FF01DAA7FCFF13BB181397FF401621B1
        FFFFFA7B181178F6351251D2FF13691227E9C0161746E2FFFFDB3E181AD5B116
        13A7FCFF01B9C2F7FFF6FFF0FF01DAA7FCFF13E92F1447FFB2151A1E5D7E4318
        162CE9AD171188D2FF13A1121490FF6010182F76702C181070FF581423D9FCFF
        01B9C2F7FFF6FFF0FF01DAA7FBFF126A131599FF8716141310141128C4EF3A16
        23D4D2FF12E42D162DE1EF4C10151111141158FAA918135DFBFF01B9C2F7FFF6
        FFF0FF01DAA7FBFF11D5241822B4FFBE5428233872E3F45016117ED0FF119212
        164EEDF88E3D2328479DFFBB231821CCFBFF01B9C2F7FFF6FFF0FF01DAA7FAFF
        109C14191F8AF4FFEEE4FAFFC641151447F2D0FF10FA5611163CBEFFFDE5ECFF
        F89421191390FAFF01B9C2F7FFF6FFF0FF01DAA7F9FF0E87141715316E979C84
        4B1B161144E2CEFF0EE54A11171948839E976F3415171484F9FF01B9C2F7FFF6
        FFF0FF01DAA7F8FF0CA83111131112121113121B6DEBCCFF0CF0721B12131113
        121113112DA1F8FF01B9C2F7FFF6FFF0FF01DAA7F7FF09E58A4627191820396D
        C4C8FF09C3693720181A274A8BE2F7FF01B9C2F7FFF6FFF0FF01DAA7FDFF02FB
        E9FDFCFF05FBDDC5C3D3F2C4FF05F1D2C3C8DEFCFBFF01EDF1FDFF01B9C2F7FF
        F6FFF0FF08DAA7FFFFF1CB9EA5FEADFF07C692BBE5FEFFB9C2F7FFF6FFF0FF09
        DBA5DAB093898998C1C3AFC208C5A88A888DA3CFB0C4F7FFF6FFF0FF07DBA4CF
        A18B898B98ADBB08BDA58B8A8996C2ACC4F7FFF6FFF0FF08D9A5FFFEE5BB93A1
        FEADFF07C589ABD6F7FFB7C1F7FFF6FFF0FF01DEB1FDFF02F2DCFDADFF02E5E3
        FEFEFF01C1CAF7FFF6FFF0FF01FDFAA0FF01FBFCF7FFF6FF81FFF6FF81FFF6FF
        00004D4942386B734D4C0E0000000000FFFF0000000000003200800000004D49
        423874746150000000004D4942386B734D460C0000000000FFFF000000000000
        320000000000}
      BoundLabel.Left = 214
      BoundLabel.Top = 91
      BoundLabel.Width = 3
      BoundLabel.Height = 13
      BoundLabel.Transparent = True
    end
    object rImageEx_jig_podvesny: TrImageEx
      Left = 140
      Top = 17
      Width = 150
      Height = 158
      Align = alCustom
      Picture.Data = {
        0954574943496D61676549492A00080000001400FE0004000100000000000000
        00010300010000009500000001010300010000009C0000000201030001000000
        0800000003010300010000000100000006010300010000000100000011010400
        01000000CE530000120103000100000001000000150103000100000001000000
        16010300010000009C0000001701040001000000CC5A00001A01050001000000
        FE0000001B010500010000000601000028010300010000000200000031010200
        1C0000000E01000032010200140000002A010000BC020100BF3600003E010000
        49860100D01B0000FE37000069870400010000009CAE00005C930700A41C0000
        C8AE00000000000080FC0A001027000080FC0A001027000041646F6265205068
        6F746F73686F70204353352057696E646F777300323031393A30353A30332031
        303A30343A3230003C3F787061636B657420626567696E3D22EFBBBF22206964
        3D2257354D304D7043656869487A7265537A4E54637A6B633964223F3E0A3C78
        3A786D706D65746120786D6C6E733A783D2261646F62653A6E733A6D6574612F
        2220783A786D70746B3D2241646F626520584D5020436F726520352E302D6330
        36302036312E3133343737372C20323031302F30322F31322D31373A33323A30
        302020202020202020223E0A2020203C7264663A52444620786D6C6E733A7264
        663D22687474703A2F2F7777772E77332E6F72672F313939392F30322F32322D
        7264662D73796E7461782D6E7323223E0A2020202020203C7264663A44657363
        72697074696F6E207264663A61626F75743D22220A2020202020202020202020
        20786D6C6E733A786D703D22687474703A2F2F6E732E61646F62652E636F6D2F
        7861702F312E302F223E0A2020202020202020203C786D703A43726561746F72
        546F6F6C3E41646F62652050686F746F73686F70204353352057696E646F7773
        3C2F786D703A43726561746F72546F6F6C3E0A2020202020202020203C786D70
        3A437265617465446174653E323031392D30352D30335431303A30343A32302B
        30323A30303C2F786D703A437265617465446174653E0A202020202020202020
        3C786D703A4D65746164617461446174653E323031392D30352D30335431303A
        30343A32302B30323A30303C2F786D703A4D65746164617461446174653E0A20
        20202020202020203C786D703A4D6F64696679446174653E323031392D30352D
        30335431303A30343A32302B30323A30303C2F786D703A4D6F64696679446174
        653E0A2020202020203C2F7264663A4465736372697074696F6E3E0A20202020
        20203C7264663A4465736372697074696F6E207264663A61626F75743D22220A
        202020202020202020202020786D6C6E733A64633D22687474703A2F2F707572
        6C2E6F72672F64632F656C656D656E74732F312E312F223E0A20202020202020
        20203C64633A666F726D61743E696D6167652F746966663C2F64633A666F726D
        61743E0A2020202020203C2F7264663A4465736372697074696F6E3E0A202020
        2020203C7264663A4465736372697074696F6E207264663A61626F75743D2222
        0A202020202020202020202020786D6C6E733A786D704D4D3D22687474703A2F
        2F6E732E61646F62652E636F6D2F7861702F312E302F6D6D2F220A2020202020
        20202020202020786D6C6E733A73744576743D22687474703A2F2F6E732E6164
        6F62652E636F6D2F7861702F312E302F73547970652F5265736F757263654576
        656E7423223E0A2020202020202020203C786D704D4D3A496E7374616E636549
        443E786D702E6969643A39413446384130453741364445393131413137314243
        334431394537303636443C2F786D704D4D3A496E7374616E636549443E0A2020
        202020202020203C786D704D4D3A446F63756D656E7449443E786D702E646964
        3A39413446384130453741364445393131413137314243334431394537303636
        443C2F786D704D4D3A446F63756D656E7449443E0A2020202020202020203C78
        6D704D4D3A4F726967696E616C446F63756D656E7449443E786D702E6469643A
        3941344638413045374136444539313141313731424333443139453730363644
        3C2F786D704D4D3A4F726967696E616C446F63756D656E7449443E0A20202020
        20202020203C786D704D4D3A486973746F72793E0A2020202020202020202020
        203C7264663A5365713E0A2020202020202020202020202020203C7264663A6C
        69207264663A7061727365547970653D225265736F75726365223E0A20202020
        20202020202020202020202020203C73744576743A616374696F6E3E63726561
        7465643C2F73744576743A616374696F6E3E0A20202020202020202020202020
        20202020203C73744576743A696E7374616E636549443E786D702E6969643A39
        413446384130453741364445393131413137314243334431394537303636443C
        2F73744576743A696E7374616E636549443E0A20202020202020202020202020
        20202020203C73744576743A7768656E3E323031392D30352D30335431303A30
        343A32302B30323A30303C2F73744576743A7768656E3E0A2020202020202020
        202020202020202020203C73744576743A736F6674776172654167656E743E41
        646F62652050686F746F73686F70204353352057696E646F77733C2F73744576
        743A736F6674776172654167656E743E0A202020202020202020202020202020
        3C2F7264663A6C693E0A2020202020202020202020203C2F7264663A5365713E
        0A2020202020202020203C2F786D704D4D3A486973746F72793E0A2020202020
        203C2F7264663A4465736372697074696F6E3E0A2020202020203C7264663A44
        65736372697074696F6E207264663A61626F75743D22220A2020202020202020
        20202020786D6C6E733A70686F746F73686F703D22687474703A2F2F6E732E61
        646F62652E636F6D2F70686F746F73686F702F312E302F223E0A202020202020
        2020203C70686F746F73686F703A436F6C6F724D6F64653E313C2F70686F746F
        73686F703A436F6C6F724D6F64653E0A2020202020203C2F7264663A44657363
        72697074696F6E3E0A2020203C2F7264663A5244463E0A3C2F783A786D706D65
        74613E0A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020200A20202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        0A20202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020200A2020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020200A202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020200A20202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020200A2020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020200A202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020200A20
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020200A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020200A20202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        0A20202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020200A2020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020200A202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020200A20202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020200A2020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020200A202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020200A20
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020200A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020200A20202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        0A20202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020200A2020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020200A202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020200A20202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020200A2020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020200A202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020200A20
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020200A20202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020200A2020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020200A202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020200A20202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020200A2020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020200A202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        200A202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020200A20202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020200A2020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020200A202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020200A20202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020200A2020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020202020202020202020200A
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020200A202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020200A20202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020200A2020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020200A202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020200A20202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020202020202020202020202020200A2020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20200A2020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020200A202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020200A20202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        20202020202020202020202020202020200A2020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        2020202020202020202020202020202020202020202020202020202020202020
        202020202020202020202020202020202020202020200A202020202020202020
        202020202020202020202020202020202020200A3C3F787061636B657420656E
        643D2277223F3E003842494D0425000000000010000000000000000000000000
        000000003842494D043A000000000093000000100000000100000000000B7072
        696E744F75747075740000000500000000436C7253656E756D00000000436C72
        53000000005247424300000000496E7465656E756D00000000496E7465000000
        00436C726D000000004D70426C626F6F6C010000000F7072696E745369787465
        656E426974626F6F6C000000000B7072696E7465724E616D6554455854000000
        010000003842494D043B0000000001B200000010000000010000000000127072
        696E744F75747075744F7074696F6E7300000012000000004370746E626F6F6C
        0000000000436C6272626F6F6C00000000005267734D626F6F6C000000000043
        726E43626F6F6C0000000000436E7443626F6F6C00000000004C626C73626F6F
        6C00000000004E677476626F6F6C0000000000456D6C44626F6F6C0000000000
        496E7472626F6F6C000000000042636B674F626A630000000100000000000052
        474243000000030000000052642020646F7562406FE000000000000000000047
        726E20646F7562406FE0000000000000000000426C2020646F7562406FE00000
        0000000000000042726454556E744623526C7400000000000000000000000042
        6C6420556E744623526C7400000000000000000000000052736C74556E744623
        50786C40520000000000000000000A766563746F7244617461626F6F6C010000
        000050675073656E756D00000000506750730000000050675043000000004C65
        6674556E744623526C74000000000000000000000000546F7020556E74462352
        6C7400000000000000000000000053636C20556E744623507263405900000000
        00003842494D03ED000000000010004800000001000200480000000100023842
        494D042600000000000E000000000000000000003F8000003842494D040D0000
        00000004000000783842494D04190000000000040000001E3842494D03F30000
        00000009000000000000000001003842494D271000000000000A000100000000
        000000023842494D03F4000000000012003500000001002D0000000600000000
        00013842494D03F700000000001C0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF03E800003842494D040000000000000200003842494D04020000
        0000000200003842494D043000000000000101003842494D042D000000000006
        0001000000023842494D04080000000000100000000100000240000002400000
        00003842494D041E000000000004000000003842494D041A00000000034B0000
        000600000000000000000000009C000000950000000B00420065007A0020006E
        00E1007A00760075002D00310000000100000000000000000000000000000000
        000000010000000000000000000000950000009C000000000000000000000000
        0000000001000000000000000000000000000000000000001000000001000000
        0000006E756C6C0000000200000006626F756E64734F626A6300000001000000
        000000526374310000000400000000546F70206C6F6E6700000000000000004C
        6566746C6F6E67000000000000000042746F6D6C6F6E670000009C0000000052
        6768746C6F6E670000009500000006736C69636573566C4C73000000014F626A
        6300000001000000000005736C6963650000001200000007736C69636549446C
        6F6E67000000000000000767726F757049446C6F6E6700000000000000066F72
        6967696E656E756D0000000C45536C6963654F726967696E0000000D6175746F
        47656E6572617465640000000054797065656E756D0000000A45536C69636554
        79706500000000496D672000000006626F756E64734F626A6300000001000000
        000000526374310000000400000000546F70206C6F6E6700000000000000004C
        6566746C6F6E67000000000000000042746F6D6C6F6E670000009C0000000052
        6768746C6F6E67000000950000000375726C5445585400000001000000000000
        6E756C6C54455854000000010000000000004D73676554455854000000010000
        00000006616C74546167544558540000000100000000000E63656C6C54657874
        497348544D4C626F6F6C010000000863656C6C54657874544558540000000100
        0000000009686F727A416C69676E656E756D0000000F45536C696365486F727A
        416C69676E0000000764656661756C740000000976657274416C69676E656E75
        6D0000000F45536C69636556657274416C69676E0000000764656661756C7400
        00000B6267436F6C6F7254797065656E756D0000001145536C6963654247436F
        6C6F7254797065000000004E6F6E6500000009746F704F75747365746C6F6E67
        000000000000000A6C6566744F75747365746C6F6E67000000000000000C626F
        74746F6D4F75747365746C6F6E67000000000000000B72696768744F75747365
        746C6F6E6700000000003842494D042800000000000C000000023FF000000000
        00003842494D041100000000000101003842494D041400000000000400000003
        3842494D040C0000000012FD00000001000000950000009C000001C000011100
        000012E100180001FFD8FFED000C41646F62655F434D0003FFEE000E41646F62
        6500648000000001FFDB0084000C08080809080C09090C110B0A0B11150F0C0C
        0F1518131315131318110C0C0C0C0C0C110C0C0C0C0C0C0C0C0C0C0C0C0C0C0C
        0C0C0C0C0C0C0C0C0C0C0C0C0C010D0B0B0D0E0D100E0E10140E0E0E14140E0E
        0E0E14110C0C0C0C0C11110C0C0C0C0C0C110C0C0C0C0C0C0C0C0C0C0C0C0C0C
        0C0C0C0C0C0C0C0C0C0C0C0C0C0CFFC0001108009C0095030122000211010311
        01FFDD0004000AFFC4013F000001050101010101010000000000000003000102
        0405060708090A0B010001050101010101010000000000000001000203040506
        0708090A0B1000010401030204020507060805030C3301000211030421123105
        4151611322718132061491A1B14223241552C16233347282D14307259253F0E1
        F163733516A2B283264493546445C2A3743617D255E265F2B384C3D375E3F346
        2794A485B495C4D4E4F4A5B5C5D5E5F55666768696A6B6C6D6E6F63747576777
        8797A7B7C7D7E7F7110002020102040403040506070706053501000211032131
        12044151617122130532819114A1B14223C152D1F0332462E172829243531563
        7334F1250616A2B283072635C2D2449354A317644555367465E2F2B384C3D375
        E3F34694A485B495C4D4E4F4A5B5C5D5E5F55666768696A6B6C6D6E6F6273747
        5767778797A7B7C7FFDA000C03010002110311003F00F54003406B4401A00380
        13A4924A58F057396757FAC0CC3C47D1882E65ACACD9933B892E8DDFA266DF4B
        FACBA377D13F054FA2FF00C9187FF12CFC89298FDAFAAFFDC01FF6FB7FF2097D
        AFAAFF00DC01FF006FB7FF0020B88EBBF5E7ACF52CECBC1FAB56D181D3FA71DB
        9DD6B288F4DAE9D9B2ADEDB1BEE7FB6AD95DD75DFE0FD8B27A7FD62FADCC2FC8
        E8DF5831FEB13A806CBFA73EB732D731BAD8EC7AEFAA8B6D6B1BFF0071ECFF00
        ADA4A7D37ED7D57FEE00FF00B7DBFF00904BED7D57FEE00FFB7DBFF9055BEAB7
        D65C2FACBD259D47146C74ECBE8265D5D83E930FEF73B98FFDC5B0929A1F6BEA
        BFF7007FDBEDFF00C825F6BEABFF007007FDBEDFFC82BE924A687DAFAAFF00DC
        01FF006FB7FF002097DAFAAFFDC01FF6FB7FF20AFA4929A1F6BEABFF007007FD
        BEDFFC825F6BEABFF7007FDBEDFF00C82BCE735AD2E7101A0492740005E65D57
        EBD75DEB5664D9D0B268E8DD0B11E6AB3ABE541363FF00728616DAE739EDF7D7
        4D34FADFE12CF4D253DFFDAFAAFF00DC01FF006FB7FF0020A865F54FAC35E7D5
        551D3C3D8E64BD9BA4731BBD71B595FF0055CB82C0FAD1F5B70EAB3A8F4EEB38
        FF0059F0B146FCCC42C7577B6B1F4EE6D76D55647A4CFF004B5FAAC67E7D6BD2
        FA0F5BC3EBDD2A8EA7844FA578D587E931C34B2A7FF298E49493A75F7DDF68F5
        C6D732EDA1920ED1B2A76CDCD8DDEE72B8A9E07F3B9BFF00860FFE7BA55C494A
        49249253FFD0F5549249252CEFA27E0B2A8F5BFE6B0FB3CFAFF633E9473BFD33
        B36FF696ABBE89F82A7D17FE48C3FF008967E4494F89E0FECE7746FAB4CCE247
        48767E47ED622437D6DD57A7F682DFFBA7B36FFC17AEBA3FAECCFAB74F52E86F
        FAA7F67FDB4326B15B304B0B4D7C37D7F43D9F4F637DDFE0FD5577EB07D44EB1
        D3F332F2FEAED14F50E99D40EFCDE8D91A377825DEA51EEAB6ED71FD13EAB69B
        A9FE6FF9B5CF7D5CC3EAFD63D61F54FA2E3747D4E3E4F537DAFB6CA891FA5AF1
        ECC87D8FA5DB1FFF0069E9F57FE15253D57F8B98FF009CDF5AC6247ECEFB58F4
        F6FD1DFBEF9F4FF92BD0163FD55FAB587F56BA4B3A7631F51F3EA645E441B2C3
        F49FFC96E9B58CFDC5B09294924924A524924929C8FADFEBFF00CD6EADF67FE7
        7EC9744731B1DBE3FB0BC9BA68E92FBFEA8D7D60B4741345C5FB8C5472FD5C8F
        57ED1FDAFB1FA9BFFC0FA7FE097B7B9AD734B5C039AE1041D4107B15E5BD73EA
        3F58E84328F4BC6A7AC7D5DB5C726CE99904B5D4B80F7598F607D5631CDAFD9E
        AD16FA9E97E8EEAAC494BF5D6FD5FAFEBDF4077D57347DA9F611D41989B7D1F4
        3DA1FEAFA3FA1FE8DF69F57FE0FF009C5A7FE28F6FD8BAC0C7FE8233DFF65FDD
        DB03E8FF00D6FD25CC7D5EE91D6FAFE23DBF56FA5E3741E9B960D395D44D8FB6
        E7301FD3515DB73DF7FA6EFCFAE9AEAF53FC25ABD57A0743C3E83D2A8E99840F
        A548D5E7E93DE75B2D7FF29EE4949703F9DCDFFC307FF3DD2AE2A781FCEE6FFE
        183FF9EE957125292492494FFFD1F554967E5E7E651975E3B29A9DEB92292EB4
        B4BB6B77BFDBE93D4BD7EB1FF7129FFB7CFF00E90494BF5ACE7F4EE8F9B9EC60
        B1F894597358E301C58D73F6B88FEAAE03A37D75FAF57F4BC6B303EAEB727176
        06D5735E61C19FA3DDCFEF3575DF5819D7333A1750C4AB0EA7597E35B5B1ADB8
        9712E639A0343A963777F6D71DF57FA8FF008C7E85D1F1BA553F5705D5E30706
        D8F780E3B9CEB75DB67F2D253A1FF3BFFC647FF3AC3FCF3FF92587F556DFF181
        F5671B271E8FABAEC819579C8739EE820B8359B06D77F216851FE30FEBCE4752
        BFA553D02A7E7E2B43EFA03CEE6B4EDDAE3EFDBFE118AEFF00CEAFF199FF00CE
        C33FEDCFFD489296FF009DFF00E323FF009D61FE79FF00C925FF003BFF00C647
        FF003AC3FCF3FF00924FFF003ABFC667FF003B0CFF00B73FF5225FF3ABFC667F
        F3B0CFFB73FF005224A5BFE77FF8C8FF00E7587F9E7FF2497FCEFF00F191FF00
        CEB0FF003CFF00E493FF00CEAFF199FF00CEC33FEDCFFD4897FCEAFF00199FFC
        EC33FEDCFF00D489296FF9DFFE323FF9D61FE79FFC925FF3BFFC647FF3AC3FCF
        3FF924FF00F3ABFC667FF3B0CFFB73FF005225FF003ABFC667FF003B0CFF00B7
        3FF5224A5BFE77FF008C8FFE7587F9E7FF00248595F59FFC6364E2DD8CEFAB1B
        5B731D597079901C0B27E979A37FCEAFF199FF00CEC33FEDCFFD4897FCEAFF00
        199FFCEC33FEDCFF00D48929C8FAAF9BFE307EAE748674BA3EAE3AF656F7BFD4
        7BA09DE7770D72D6FF009DDFE323FF009D61FE79FF00C92AFD47EBDFD7FE9987
        66767FD5DAE8C6AA3D4B5D61206E218DFA2F77E7B9171FEB8FF8C6C9A2BC8A3E
        AD56FA6E636CADE2CD0B5C37B1DAD9F9CD2929D4FA83F5973BAEBFABB73B15B8
        77E26435B656D2490F2D3558C76EFDC763AEB9711FE2DBA5F5BC3BFADE5F58C3
        76159D4721B7B184823DC6E7D819B5CFFA1EA2EDD25292492494FF00FFD2EE7A
        BF4D7E4F59E9F78C87565A5DB1A1A080583D571FFAE7D05B8A8E67FCA381FD6B
        7FF3D9579252924952EA7D5F0BA5D22DCA71F718656DD5CEFEAB5253C6FD5DFF
        00F2AFF58FFF000B57F931177EBCB3A1FD62C1A3FC63F5CEA0F65A69C8A18D60
        0D1BA40C6FA4DDDFC85D97FCF6E93FE8EFFF00307FE4D253D0A4B9EFF9EDD27F
        D1DFFE60FF00C9A5FF003DBA4FFA3BFF00CC1FF93494F4292E7BFE7B749FF477
        FF00983FF2697FCF6E93FE8EFF00F307FE4D253D0A4B9EFF009EDD27FD1DFF00
        E60FFC9A5FF3DBA4FF00A3BFFCC1FF0093494F4292E7BFE7B749FF00477FF983
        FF002697FCF6E93FE8EFFF00307FE4D25353FC697FE21FA87C69FF00CFD52D9F
        AB3FF89BE95FF84F1FFF003D3171DFE30FEB474FEA1F5473716965A2CB0D505E
        D00696D6FF00DE3FBAB57A07D70E974742E9D43EBB8BAAC5A58E21A225B5B1A6
        3DE929EBD25CF7FCF6E93FE8EFFF00307FE4D6D60E6559D895E5D208AED12D0E
        10798D794949D2492494FF00FFD3F47CCFF94703FAD6FF00E7B2AF2A399FF28E
        07F5ADFF00CF655E494E7F58EB38FD2E8DEF1EA5EFD29A5BF49C7FF20B3BA4F4
        5C9C9C9FDAFD6BDF92ED69C73F46B1F9B2DFDEFE47FE8C5C4E6FD5FB3EB37F8C
        BEB182FCFBF0D98B557756EA89304331DBB5A0B9BB3F9DDEB5FF00F1A9BBFF00
        9E2CFF00F38FFE94494EEF4DFAA96617D6FEA5F589D7B5F5F50A9B5B71C320B0
        B452376FDDEEFE67F75745B5BE03EE5C07FE35377FF3C59FFE71FF00D2897FE3
        5377FF003C59FF00E71FFD28929EFF006B7C07DC96D6F80FB9701FF8D4DDFF00
        CF167FF9C7FF004A25FF008D4DDFFCF167FF009C7FF4A24A7BFDADF01F725B5B
        E03EE5C07FE35377FF003C59FF00E71FFD2897FE35377FF3C59FFE71FF00D289
        29EFF6B7C07DC96D6F80FB9701FF008D4DDFFCF167FF009C7FF4A25FF8D4DDFF
        00CF167FF9C7FF004A24A7BFDADF01F725B5BE03EE5C07FE35377FF3C59FFE71
        FF00D2897FE35377FF003C59FF00E71FFD28929EABEB474067D60E8991D24DBF
        66FB46CFD306EF8D8F65BF43733E96CFDE46C7AF17A2F46A29C8B01AB068AEA3
        6911BBD36B6BDDB3DDEE7EDFA0B8FF00FC6A6EFF00E78B3FFCE3FF00A5160F56
        FAAD6742FAD5F57B02DEA595D471FA8640F5597B8810C7D6D88DEEFA7EA24A7A
        9CD6E6F5DC6C9EA97038FD3F158F762D5DDEE03E9BBFEFCEFF00ADD6BA1FAB3F
        F2161FF50FFD5394FADB18CE8796C600D6B68706B468000380A1F567FE42C3FE
        A1FF00AA724A7512492494FF00FFD4F47CCFF94703FAD6FF00E7B2AF2A399FF2
        8E07F5ADFF00CF655E494F01D03FFCADFD60FF00C2ACFC986BBF5C0740FF00F2
        B7F583FF000AB3F261AEFD25292492494A4924925292492494A4924925292492
        494A5E7FF5EFFF00171F547FE3DDFF009F285E80BCFF00EBDFFE2E3EA8FF00C7
        BBFF003E50929EC7AF7FC8D9BFF12FFC885F567FE42C3FEA1FFAA722F5EFF91B
        37FE25FF00910BEACFFC8587FD43FF0054E494EA24924929FFD5F47CCFF94703
        FAD6FF00E7B2AF2A399FF28E07F5ADFF00CF655E494F01D03FFCADFD60FF00C2
        ACFC986BBF5C0740FF00F2B7F583FF000AB3F261AEFD25292492494A49249252
        92492494A4924925292492494A5E7FF5EFFF00171F547FE3DDFF009F285E80BC
        FF00EBDFFE2E3EA8FF00C7BBFF003E50929EC7AF7FC8D9BFF12FFC885F567FE4
        2C3FEA1FFAA722F5EFF91B37FE25FF00910BEACFFC8587FD43FF0054E494EA24
        924929FFD6F47CCFF94703FAD6FF00E7B2AF2A399FF28E07F5ADFF00CF655E49
        4F01D03FFCADFD60FF00C2ACFC986BBF5C0740FF00F2B7F583FF000AB3F261AE
        FD25292492494A4924925292492494A4924925292492494A5E7FF5EFFF00171F
        547FE3DDFF009F285E80BCFF00EBDFFE2E3EA8FF00C7BBFF003E50929EC7AF7F
        C8D9BFF12FFC885F567FE42C3FEA1FFAA7217D68EA18F8DD36DC679DD7E534D7
        554DD5C777B7747EEA27D5D2EA7A753837B4D595433DF53B430E25CD7B7F7989
        29D549249253FFD7EDBACE3E459D6B01F5653AAAA48B9A1FB76ED1BC9FE4FAD5
        FE8D6D7DA71FFD2B3FCE093F171AC7173EA639C79739A09FC428FD8B0FFD057F
        E637FB9253E79D3BAB74CE9DFE353AF646765D38D4BF198C6596BDAD6976DC43
        B5AE71FA5ED5D8FF00CF2FAA7FF97187FF006F33FF0024B3F37EA4FD56EA3D7E
        FB733A7B2C7D9457638873DA0BB73EADDB6B7B1BFCDD6C52FF00C6D7EA47FE55
        B3FEDCB7FF004AA4A6F7FCF2FAA7FF0097187FF6F33FF2497FCF2FAA7FF97187
        FF006F33FF0024A8FF00E36BF523FF002AD9FF006E5BFF00A552FF00C6D7EA47
        FE55B3FEDCB7FF004AA4A6F7FCF2FAA7FF0097187FF6F33FF2497FCF2FAA7FF9
        7187FF006F33FF0024A8FF00E36BF523FF002AD9FF006E5BFF00A552FF00C6D7
        EA47FE55B3FEDCB7FF004AA4A6F7FCF2FAA7FF0097187FF6F33FF2497FCF2FAA
        7FF97187FF006F33FF0024A8FF00E36BF523FF002AD9FF006E5BFF00A552FF00
        C6D7EA47FE55B3FEDCB7FF004AA4A6F7FCF2FAA7FF0097187FF6F33FF2497FCF
        2FAA7FF97187FF006F33FF0024A8FF00E36BF523FF002AD9FF006E5BFF00A552
        FF00C6D7EA47FE55B3FEDCB7FF004AA4A6F7FCF2FAA7FF0097187FF6F33FF249
        7FCF2FAA7FF97187FF006F33FF0024A8FF00E36BF523FF002AD9FF006E5BFF00
        A552FF00C6D7EA47FE55B3FEDCB7FF004AA4A6F7FCF2FAA7FF0097187FF6F33F
        F24B86FAF3F583A6657D6BFAB595D2F269CF38B6B8B9B4BC3E1C5F4EC6BBD3DD
        F4F6AEAFFF001B5FA91FF956CFFB72DFFD2A8167D41FAB3879F85774DC36E265
        56F75955C1CF7C3EB1EA565CCB2C735CDDC929D0E8BD16F379EAFD5BF499D66B
        5B0F150FCDD3F7FF00F3DABBD5F23A7E3D6DB72720635D5FBA9B06AF07C1B5FD
        2B58FF00CFAD31EB0CA71721F96DF472B1185D6D3320F663E977E7D56BBE82CC
        E9F83675EF4F3FAB52EA6DA0B3D201BB1AF68F7B9CEDDFA47FA8EFF33FC1A4A7
        57F6864FD8CBFECCEFB50806B83B7569B3D5DFFE8B6B7FE337FE87F9C4968249
        29FFD0F5549249253919B9F474FEA57645E1EE60C6AC456D2F3F4EEFDDFA3FDB
        4DD3FEB2E266D2EBBD2B9803CB5A056FB2408D4BA963D9FD85A43ECDF6B7447D
        A7D36EEF1F4E5DB3FB3BFD44B1BECBB5FF0065D9B771DFE9C46FD376ED9F9E92
        9AFF00B6313F72FF00FB62DFFD2697ED8C4FDCBFFED8B7FF0049ABC924A68FED
        8C4FDCBFFED8B7FF0049A5FB6313F72FFF00B62DFF00D26AF24929A3FB6313F7
        2FFF00B62DFF00D2697ED8C4FDCBFF00ED8B7FF49ABC924A68FED8C4FDCBFF00
        ED8B7FF49A5FB6313F72FF00FB62DFFD26AF24929A3FB6313F72FF00FB62DFFD
        2697ED8C4FDCBFFED8B7FF0049ABC924A72B37EB0E2E2E33EF155CFD9076BAAB
        180C90DFE72CAC31AA189D5B1FAA6461DD436C6006C91634B7F33F35FF0041FF
        00D872D3C9FB3FA2EFB56CF474DDEA46DE46DDDBBDBF4933BECDEA533B77EBE8
        47C3DDB63F9092986660D595E9BC92CBA976EAAD6C6E69EE3DDF498FFCE62B29
        249294924924A7FFD9003842494D042100000000005500000001010000000F00
        410064006F00620065002000500068006F0074006F00730068006F0070000000
        1300410064006F00620065002000500068006F0074006F00730068006F007000
        2000430053003500000001003842494D0FA00000000000F86D616E6949524652
        000000EC3842494D416E4473000000CC00000010000000010000000000006E75
        6C6C0000000300000000414653746C6F6E6700000000000000004672496E566C
        4C73000000014F626A63000000010000000000006E756C6C0000000100000000
        467249446C6F6E674002D7110000000046537473566C4C73000000014F626A63
        000000010000000000006E756C6C0000000400000000467349446C6F6E670000
        0000000000004146726D6C6F6E67000000000000000046734672566C4C730000
        00016C6F6E674002D711000000004C436E746C6F6E670000000000003842494D
        526F6C6C0000000800000000000000003842494D0FA100000000001C6D667269
        000000020000001000000001000000000000000100000000FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFDE2F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFF8EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF47FE1FFFFFFF9D4
        D9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCDAFAFF
        FFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFF480E7FFE3BA8B6ABBFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA16D8DBDE5FFE293FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFF484BEA0736F74758A9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C
        9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C
        9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C9C
        9C9C9C9C9C9C8475746F75A2BA96FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF483CAB38C72717397
        B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5
        B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5
        B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B58C7371728E
        B4C596FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFF481E6FFFAD4A86FBCFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA174ABD6FCFFE194FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFF481E2FFFFFFFFE1E4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFDAE7FFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        DD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFFFFFFFFFFFFFBD4A2858AB3
        E6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFE5AF8D88A1D6FAFFFFFFFFFFFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        F481E2FFFFFFFFFFFFFFFFFFFFF07D2A0500000000123BABFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA43A110000000005297EF6
        FFFFFFFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFFFF
        FFAF22000000000000000000004FE6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFDF3F000000000000000000002EBEFFFFFFFFFFFFFFFFFFDD
        94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFFFF8A04000005316A8D865320
        00000026DAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC21B0000
        002657838A642C00000008A1FFFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4
        81E2FFFFFFFFFFFFFF9C00000035CBFFFFEFF6FFFD881000002DEAFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD71000001699FFFFF3F1FFFFB72B0000
        02BDFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFE20F00
        005AFFEB7F280B114CC5FFC914000060FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFF4500002AE1FF9E3A0D0C319EFFF63D000026EEFFFFFFFFFFFFDD94
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFF69000043FFBC1500000000000078
        FFC5020004BEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB400000EDEEF3E
        0000000000003EE9EF2B000086FFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481
        E2FFFFFFFFFFE8150006DFCE0500000232170000007CFF76000061FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFE3D000094F53B000000292A00000031FEBB00
        001DECFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFF97000050FF
        2E000039C3FFE46B000009E4E807002EF9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFEE12001BFF80000015A0F8F99913000094FF3F0000B4FFFFFFFFFFDD94FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFF481E2FFFFFFFFFF6A0000B5DB000029EDFFFFFFFF4F0000
        80FF300003D0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3010046FD240005B3
        FFFFFFFFA5000039FF8A000095FFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2
        FFFFFFFFFF5C0000D3B6000070FFFFFFFFFFCD05003BFF520000B4FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFA300006BED080031FFFFFFFFFFF31B000BEDA700
        007FFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFF5A0000D4A900
        0077FFFFFFFFFFE90C002DFF550000B3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFA5000068E7050038FFFFFFFFFFFF310000E1AB00007DFFFFFFFFFFDD94FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFF481E2FFFFFFFFFF6D0000BACB000043FFFFFFFFFF8700004B
        FF380000C8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3000048F91C000ED9FF
        FFFFFFD4090023FC9400008FFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FF
        FFFFFFFF9700005AF314000078F8FFFEA80B0000BBEB070027F6FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFED15001DFF6A000039DFFFFFD72F000068FF490000
        AEFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFE6130004E4B200
        00002741320000004AFF82000060FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FF3E000098E8110000153E3B0E000013E9C9030017EAFFFFFFFFFFDD94FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFF481E2FFFFFFFFFFFF5D000052FF83000000000000003CF4D50B
        0002BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA6000015EFC80E000000
        00000014CBFC3B00007CFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFF
        FFFFFFFFD50B00006FFFAA310F000A2589FFD21E00005AFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFF3F000039E8DD4D1802041B5AE5FC4F000020E6FF
        FFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFF9700000043D7
        FDF3D9EDFFF79F13000022E5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFD50F00001FB0F9FCDEE3FFFFC733000003B3FFFFFFFFFFFFFFDD94FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFF481E2FFFFFFFFFFFFFFFF78000000023E8CAE9D68190000001AD2
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB90E0000002677A7A5
        7E2F0000000393FFFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFF
        FFFFFFFFFFFF9512000000000000000000003FDBFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFC92B0000000000000000000021ABFFFFFFFFFF
        FFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFFFFFFFFDD6B2000
        000000000441A0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFF48C390000000000002C7BEEFFFFFFFFFFFFFFFFFFFFDD94FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFF481E2FFFFFFFFFFFFFFFFFFFFFFFFF0B52400007BD3FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC94B000058
        C0F8FFFFFFFFFFFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFF
        FFFFFFFFFFFFFFFFFFFF470000CEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7C0000A1FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFF481E2FFFFFFFFFFFFFFFFFFFFFFFFFFFF43
        0000C3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF76000098FFFFFFFFFFFFFFFFFFFFFFFFFFFFDD94FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFF481E2FFFFFFFFFFFFFFFFFFFFFFFFFFFF430000C3FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF76000098FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFDD94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF89EFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFF470000CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7D0000A1FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFEA9DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFF282407281808080808080808080808081812200
        0062828080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        80808080823B00004D82808080808080808080808080816F48A2FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFE3010000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000000000000000000038FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50F0000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000043FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFE50F00198A8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B
        8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B
        8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B
        8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8B8C75030043FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        E50F0031FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFE4050043FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50F002EFDFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7
        050043FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFE50F002EFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7050043FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8
        0F0031FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFE3050043FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE
        F3F2F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F4DA0E00157172727272727272
        7272727272727272727272727272727272727272727272727272727272727272
        7272727272727272727272727272727272727272727272727272727272727272
        7272727272727272727272727272727272727272727272727272727272736002
        0043FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF18881858683838383838383
        8383838383838383867507000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000043FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFBE3E1C6B2E2E1E1E1E1E1E1E1E1E1E1E1E1E1E1E2C902
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000000000003AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFC09BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBADA7A7A7A7A7A7A7A7A88801
        0027A8A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7
        A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7
        A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A7A89609001AA3A7A7A7A7A7A7A7A7A6
        BEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB8F75E5FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDA01003FFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFF10E0029FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFE3756EB2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027
        F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAA
        717489F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF0100
        3CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEE76727669CCFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFE3A49086A2C9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBF
        A2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF01003C
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF01003CFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFCF01003CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF01003CFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFE50D0027F9FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD001003CFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFE60D0027FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFBFCCC01003BFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFC
        FCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFC
        FCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCE20D0027F6FCFCFC
        FCFCFCFCFCFCFCFCFCFCFCFCFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF19A8A91720000218F8E8D
        8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D
        8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D8D
        8D8D8D8D8D8D8D8D8D8D8D8D907E070016898E8D8D8D8D8D8D8D8D8D8D8D8D8D
        87B1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFED8C7073765C00001A7473727272727272727272727272
        7272727272727272727272727272727272727272727272727272727272727272
        7272727272727272727272727272727272727272727272727272727272727272
        7275660600126F73727272727272727272727474746DA0FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE98971
        7578846700001E81807F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F
        7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F
        7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F82720600147C807F7F7F
        7F7F7F7F7F80837A787871A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE687717382D9F3C4010039F2F2F1F1
        F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
        F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1F1
        F1F1F1F1F1F1F1F1F1F1F1F2D90C0025ECF2F1F1F1F1F1F1F1F2F5C276777871
        A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFE381737381E3FFFFD201003DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFE80D0027FCFFFFFFFFFFFFFFFFFFD27876767871A3FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDF7E747284E4FF
        FFFFCF00003CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE4060021F9FFFFFFFFFF
        FFFFFFCF767574837971A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFDC7C747188E9FFFFFFFFCD000034FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFE92F2345FAFFFFFFFFFFFFFFD7787570A0D07671A3
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFD57D74708DEDFFFFFFFFFFF1B3B2C4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FEF7F7F8FFFFFFFFFFFFFFD57A75709AFADC7471A3FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD178747091EFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7
        78757097F7FFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFD075757196F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDA7C747093F3FFFFD77471A3FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFFFFFCC7D7C
        7BABFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFDB7E747093F4FFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFF0844443454D8D969494949494949494
        9494949494949494949494949494949494949494949494949494949494949494
        9494949494949494949494949494949494949494949494949494949494949494
        9494949494949494949494949494949494949494949494949494958C494F7293
        F4FFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFF
        FFFFFFFFFFD70000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000001498F4FFFFFFFFFFD77471A3FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD90600000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000002FF3FFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD90600136F717171717171717171717171
        7171717171717171717171717171717171717171717171717171717171717171
        7171717171717171717171717171717171717171717171717171717171717171
        7171717171717171717171717171717171717171717171717355000044FFFFFF
        FFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFF
        FFFFFFFFD906002DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFCB000042FFFFFFFFFFFFFFD77471A3FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFF
        FFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFF
        FFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        BCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFF
        FFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFF
        FFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBC
        A0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFF
        FFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFF
        FFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        C0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0
        FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFF
        D77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFF
        D906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0
        000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FF
        FFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD7
        7471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD9
        06002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC000
        0042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFF
        FFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD774
        71A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906
        002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC00000
        42FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFF
        FFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471
        A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD90600
        2BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042
        FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFF
        FFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002B
        FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FF
        FFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFF
        FFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFB
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFF
        FFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFF
        FFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFF
        FFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFF
        FFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFF
        FFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFF
        FFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        BCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFF
        FFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFF
        FFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBC
        A0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFF
        FFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFF
        FFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        C0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0
        FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFF
        D77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFF
        D906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0
        000042FFFFFFFFFFFFFFD77471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FF
        FFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFD7
        7471A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD9
        06002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFFFDA7471A4FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC000
        0042FFFFFFFFFFFFFFB073719BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFF
        FFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFFFFFFFB67276
        7483C2FFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906
        002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFC0000042FFFFFFFFFFBA717672B8EC95BDFFFFFFFFFFFFFF
        FFFFFFFFFFFFBCA0FFFFFFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC00000
        42FFFFFFFFB6707670B6FFFFF494B5FFFFFFFFFFFFFFFFFFFFFEFFBDA1FFFEFF
        FFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000042FFFFFFB771766FB5FFFF
        FFFFF795BAFFFFFFFFFFFFFFFFFFDE92878190C0FFFFFFFFFFFFFFFFFFD90600
        2BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFC0000042FFFFBE727672B8FFFFFFFFFFFFF798B7FFFFFFFFFF
        FFFFFFF17B73766CD2FFFFFFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000043
        FFBF717671B3FFFFFFFFFFFFFFFFF695B3FFFFFFFFFFFFFFFFB271748DFAFFFF
        FFFFFFFFFFFFFFD906002BFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0000046C9717670AFFFFFFFFFFFFF
        FFFFFFFFF89AB6FFFFFFFFFFFFFFE6766EB6FFFFFFFFFFFFFFFFFFFFD906002B
        FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFC00000257C766FADFEFFFFFFFFFFFFFFFFFFFFFFFF9BB4FFFFFF
        FFFFFFFC9479EBFFFFFFFFFFFFFFFFFFFFD906002DFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC700001E7A
        71ADFEFFFFFFFFFFFFFFFFFFFFFFFFDEBC97B1FFFFFFFFFFFFC7A3FFFFFFFFFF
        FFFFFFFFFFFFDF060023CDD1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1
        D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1
        D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1
        D1D1D1D1D1D1D1D1D1D1D1D1D1D1D1D29D00001E75AEFFFFFFFFFFFFFFFFFFFF
        FFFCD3A06CBAFD9BC1FFFFFACFCCB7A8CDCCCCCCCCCCCCCCCCCCCEAE05000002
        0202020202020202020202020202020202020202020202020202020202020202
        0202020202020202020202020202020202020202020202020202020202020202
        0202020202020202020202020202020202020202020202020202020202020202
        02020202030200001AAFFFFFFFFFFFFFFFFFFFFFF4C49372707BE9FFF7E4FFFF
        F8B9B3B6B8B4B4B4B4B4B4B4B4B4B4B897000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000000000000000000000000000000000000000000000000036FFFF
        FFFFFFFFFFFFFFFFFFF1906F7871A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFE7381B2A3433333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333333333333333333333429124EFFFFFFFFFFFFFFFFFFFFFFFFDE
        6F7273D4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFCA9984B6FFFFFFFFFFFFFFFEC0FDEA82AD968CFEFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8CD4FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC6BDE47EB5FF
        FFFFFFFFFFE8759F8DADFFEFD7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFE8BD2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5B6FFE479B0FFFFFFFFFFB4706E88FEFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFC5B6FFFFE37AB0FFFFFFFA8B74766DA2FCFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5B6FFFFFFE682
        ABFEFFCA6D707EA5DCFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFE8BD2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFC5B6FFFFFFFFE77DA9FC977DB2EEFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC5B6FFFFFFFFFFE88298C5F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5B6FFFFFFFFFFFF
        EC7FADFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFE8BD2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFC5B6FFFFFFFFFFFFFFEB82ABFEFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFFFFFFFF
        F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEF9FFFFFF
        FFFFC5B6FFFFFFFFFFFFFFFFE8D1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFE8BD2FFFFFFDBAFA3FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9FB3E0FFFFFFC5B6FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFE8BD3F0BD95746B91EDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDED
        EDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDED
        EDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDED
        EDEDEDEDEDEC8A6B7698C2F4C6B6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE90AD88676F77787A
        8281818181818181818181818181818181818181818181818181818181818181
        8181818181818181818181818181818181818181818181818181818181818181
        8181818181818181818181818181818181818181818181818181817A78776D67
        93ACB9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFE8CD1E4BC8A706C91E9E9E9E9E9E9E9E9E9E9E9
        E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
        E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
        E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E88B6D718FC0EAC5B6FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFE84D0FFFFF6CF9B9CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFF969FD2FAFFFFC2B2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBAE4FFFFFFFFFFF2FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEF2FFFFFFFFFF
        DCD3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFF0000030001A0030001000000FFFF000002A004000100000095000000
        03A00400010000009C00000000000000000041646F62652050686F746F73686F
        7020446F63756D656E74204461746120426C6F636B004D4942387279614C341C
        0000010000000000000000009C000000950000000200FFFF1A0600000000AA14
        00004D4942386D726F6EFF0008004001000000000000280000000000FFFF0000
        FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000
        FFFF0856727374766120310000004D494238696E756C14000000080000005600
        72007300740076006100200031004D49423872736E6C040000007279616C4D49
        42386469796C04000000020000004D4942386C626C6304000000010000004D49
        423878666E6904000000000000004D4942386F6B6E6B04000000000000004D49
        42386670736C04000000000000004D494238726C636C08000000000000000000
        00004D494238646D687348000000010000004D49423874737563000000003400
        000010000000010000000000080000006D657461646174610100000009000000
        6C6179657254696D6562756F6471212A78FD32D741004D494238707278661000
        0000000000000000000000000000000000000100080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        0800080008000800080008000800080008000800080008000800080008000800
        08000800080008000800080000DC82EDEDED00DC00F282FFEDFF00F200ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FF
        EDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00ED82FFEDFF00ED00F282FF
        EDFF00F200DC82EDEDED00DC010004000D0019001A001A001A001A0017000D00
        21002700210033003700390035003E003F003D003B003A003C003F003D003400
        3900370033002100250021001B001B001B001A0025000D000D00130011001200
        12001100180014001500240019001A001B001B001B0018001800180018001800
        1800180018001E00250028002C002D002A002B002B002C001F00200020002400
        1800190020001E001F001F001F001F001F001F001F001F001F001F001F001F00
        1F001F001F001F001F001F001F001F001F001F001F001F001F001F001F001F00
        1F001F001F001F001F001F001F001F001F001F001F001F001F001F001F001F00
        1F001F001F001F00210023002500280027002600250023002300240028001C00
        1B001C001A001A001D001C0019001700150016001D001B001C001C001C001B00
        17000400040081FFECFFEBFF02FDE2F89AFF01F8EAEEFFEBFF02F47FE1FEFF02
        F9D4D9A6FF02CCDAFAFEFF01DD94EEFFEBFF08F480E7FFE3BA8B6ABBA6FF05A1
        6D8DBDE5FF01E293EEFFEBFF08F484BEA0736F74758AA69C058475746F75A201
        BA96EEFFEBFF08F483CAB38C72717397A6B5058C7371728EB401C596EEFFEBFF
        08F481E6FFFAD4A86FBCA6FF05A174ABD6FCFF01E194EEFFEBFF02F481E2FDFF
        01E1E4A6FF01DAE7FDFF01DD94EEFFEBFF02F481E29AFF01DD94EEFFEBFF02F4
        81E2F5FF06FBD4A2858AB3E6C0FF06E5AF8D88A1D6FAF5FF01DD94EEFFEBFF02
        F481E2F7FF03F07D2A05FD0002123BABC4FF02A43A11FD000305297EF6F7FF01
        DD94EEFFEBFF02F481E2F8FF01AF22F700014FE6C8FF01DF3FF700012EBEF8FF
        01DD94EEFFEBFF02F481E2F9FF0A8A04000005316A8D865320FE000126DACAFF
        01C21BFE00052657838A642CFE000108A1F9FF01DD94EEFFEBFF02F481E2FAFF
        009CFE000D35CBFFFFEFF6FFFD881000002DEACCFF11D71000001699FFFFF3F1
        FFFFB72B000002BDFAFF01DD94EEFFEBFF02F481E2FBFF12E20F00005AFFEB7F
        280B114CC5FFC914000060CCFF124500002AE1FF9E3A0D0C319EFFF63D000026
        EEFBFF01DD94EEFFEBFF02F481E2FBFF0669000043FFBC15FB000678FFC50200
        04BECEFF06B400000EDEEF3EFB00063EE9EF2B000086FBFF01DD94EEFFEBFF02
        F481E2FCFF0BE8150006DFCE050000023217FE00057CFF76000061CFFF06FE3D
        000094F53BFE0001292AFE000631FEBB00001DECFCFF01DD94EEFFEBFF02F481
        E2FCFF1597000050FF2E000039C3FFE46B000009E4E807002EF9D0FF15EE1200
        1BFF80000015A0F8F99913000094FF3F0000B4FCFF01DD94EEFFEBFF02F481E2
        FCFF086A0000B5DB000029EDFDFF084F000080FF300003D0D0FF08C3010046FD
        240005B3FDFF08A5000039FF8A000095FCFF01DD94EEFFEBFF02F481E2FCFF07
        5C0000D3B6000070FCFF08CD05003BFF520000B4D0FF07A300006BED080031FC
        FF08F31B000BEDA700007FFCFF01DD94EEFFEBFF02F481E2FCFF075A0000D4A9
        000077FCFF08E90C002DFF550000B3D0FF07A5000068E7050038FBFF07310000
        E1AB00007DFCFF01DD94EEFFEBFF02F481E2FCFF076D0000BACB000043FCFF08
        8700004BFF380000C8D0FF08C3000048F91C000ED9FDFF08D4090023FC940000
        8FFCFF01DD94EEFFEBFF02F481E2FCFF159700005AF314000078F8FFFEA80B00
        00BBEB070027F6D0FF15ED15001DFF6A000039DFFFFFD72F000068FF490000AE
        FCFF01DD94EEFFEBFF02F481E2FCFF05E6130004E4B2FE0002274132FE00054A
        FF82000060CEFF143E000098E8110000153E3B0E000013E9C9030017EAFCFF01
        DD94EEFFEBFF02F481E2FBFF055D000052FF83FA00063CF4D50B0002BFCEFF06
        A6000015EFC80EFB000614CBFC3B00007CFBFF01DD94EEFFEBFF02F481E2FBFF
        12D50B00006FFFAA310F000A2589FFD21E00005ACCFF123F000039E8DD4D1802
        041B5AE5FC4F000020E6FBFF01DD94EEFFEBFF02F481E2FAFF0097FE000D43D7
        FDF3D9EDFFF79F13000022E5CCFF11D50F00001FB0F9FCDEE3FFFFC733000003
        B3FAFF01DD94EEFFEBFF02F481E2F9FF0078FE0006023E8CAE9D6819FE00011A
        D2CAFF01B90EFE00052677A7A57E2FFE00010393F9FF01DD94EEFFEBFF02F481
        E2F8FF019512F700013FDBC8FF01C92BF7000121ABF8FF01DD94EEFFEBFF02F4
        81E2F7FF02DD6B20FC00020441A0C5FF02F48C39FB00022C7BEEF7FF01DD94EE
        FFEBFF02F481E2F5FF06F0B52400007BD3C0FF06C94B000058C0F8F5FF01DD94
        EEFFEBFF02F481E2F3FF03470000CEBEFF037C0000A1F3FF01DD94EEFFEBFF02
        F481E2F3FF03430000C3BEFF0376000098F3FF01DD94EEFFEBFF02F481E2F3FF
        03430000C3BEFF0376000098F3FF01DD94EEFFEAFF0189EFF3FF03470000CFBE
        FF037D0000A1F3FF01EA9DEEFFECFF04F282407281F680FF81042200006282C0
        8005823B00004D82F5800081026F48A2EFFFECFF01E3019800FF000038EFFFEC
        FF01E50F9800FF000043EFFFECFF04E50F00198A9D8B018C7502030043EFFFEC
        FF03E50F00319BFF00E402050043EFFFECFF04E50F002EFD9CFF00D702050043
        EFFFECFF04E50F002EFD9CFF00D702050043EFFFECFF03E80F00319BFF00E302
        050043EFFF02FEF3F2F0F305F4DA0E0015719D7201736002020043EFFF04F088
        818586F283028675079800FF000043EFFF05FBE3E1C6B2E2F3E102E2C9029800
        FF00003AEFFFFEFF01C09BF1FF01FBADF9A705A888010027A8B3A705A8960900
        1AA3FAA702A7A6BEEFFFFFFF03FB8F75E5E7FF03DA01003FB1FF03F10E0029F9
        FFECFFFFFF03E3756EB2E7FF03CF01003CB1FF04E50D0027F9FAFFECFFFFFF04
        AA717489F7E8FF03CF01003CB1FF04E50D0027F9FAFFECFF06FFEE76727669CC
        E8FF03CF01003CB1FF04E50D0027F9FAFFECFF06FFE3A49086A2C9E8FF03CF01
        003CB1FF04E50D0027F9FAFFECFFFEFF01BFA2E6FF03CF01003CB1FF04E50D00
        27F9FAFFECFFFEFF01BCA0E6FF03CF01003CB1FF04E50D0027F9FAFFECFFFEFF
        01BCA0E6FF03CF01003CB1FF04E50D0027F9FAFFECFFFEFF01BCA0E6FF03CF01
        003CB1FF04E50D0027F9FAFFECFFFEFF01BCA0E6FF03CF01003CB1FF04E50D00
        27F9FAFFECFFFEFF01BCA0E6FF03CF01003CB1FF04E50D0027F9FAFFECFFFEFF
        01BCA0E6FF03CF01003CB1FF04E50D0027F9FAFFECFFFEFF01BCA0E6FF03D001
        003CB1FF04E60D0027FBFAFFECFFFEFF01BCA0E8FF05FBFCCC01003BB1FC04E2
        0D0027F6FAFCF9FC00FDF5FFFEFF01BCA0EAFF09F19A8A91720000218F8EB48D
        06907E070016898EFB8DFA8D0187B1F5FFFEFF01BCA0EBFF0AED8C7073765C00
        001A7473B4720675660600126F73FB72FD72FE74016DA0F5FFFEFF01BCA0ECFF
        0BE989717578846700001E8180B47F0682720600147C80FB7FFF7F0680837A78
        7871A3F5FFFEFF01BCA0EDFF0CE687717382D9F3C4010039F2F2B4F106F2D90C
        0025ECF2FBF108F1F2F5C276777871A3F5FFFEFF01BCA0EEFF0BE381737381E3
        FFFFD201003DB1FF04E80D0027FCFAFFFFFF06D27876767871A3F5FFFEFF01BC
        A0EFFF05DF7E747284E4FEFF03CF00003CB1FF04E4060021F9FAFF08FFCF7675
        74837971A3F5FFFEFF01BCA0F0FF05DC7C747188E9FDFF03CD000034B1FF04E9
        2F2345FAFAFF08D7787570A0D07671A3F5FFFEFF01BCA0F1FF05D57D74708DED
        FCFF03F1B3B2C4B1FF03FEF7F7F8FAFF00D5087A75709AFADC7471A3F5FFFEFF
        01BCA0F2FF05D178747091EF9DFF01D77808757097F7FFD77471A3F5FFFEFF01
        BCA0F3FF05D075757196F49DFF02DA7C74087093F3FFFFD77471A3F5FFFEFF01
        BCA0F4FF04CC7D7C7BAB9CFF03DB7E74700193F4FEFF03D77471A3F5FFFEFF01
        BCA0F6FF07F0844443454D8D969F9405958C494F729300F4FDFF03D77471A3F5
        FFFEFF01BCA0F6FF00D79500021498F4FCFF03D77471A3F5FFFEFF01BCA0F6FF
        01D9069600022FF3FFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D90600136F9D
        71067355000044FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF03D906002D9BFF
        05CB000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05
        C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0
        000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C000
        0042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C00000
        42FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042
        FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FF
        FFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFF
        FCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFC
        FF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF
        03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03
        D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D7
        7471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D774
        71A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471
        A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3
        F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5
        FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FF
        FEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFE
        FF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF
        01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01
        BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BC
        A0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0
        F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6
        FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF
        04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04
        D906002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D9
        06002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906
        002BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D90600
        2BFB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002B
        FB9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB
        9CFF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9C
        FF05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF
        05C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05
        C0000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0
        000042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C000
        0042FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C00000
        42FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042
        FFFFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FF
        FFFCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFF
        FCFF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFC
        FF03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF
        03D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03
        D77471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D7
        7471A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D774
        71A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471
        A3F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03D77471A3
        F5FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03DA7471A4F5
        FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFCFF03B073719BF5FF
        FEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFDFF05B672767483C2F6
        FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFEFF07BA717672B8EC
        95BDF7FFFEFF01BCA0F6FF04D906002BFB9CFF05C0000042FFFFFFFF09B67076
        70B6FFFFF494B5F8FF06FFFEFFBDA1FFFEF8FF04D906002BFB9CFF05C0000042
        FFFF05FFB771766FB5FDFF02F795BAF9FF06FFDE92878190C0F8FF04D906002B
        FB9CFF05C0000042FFFF04BE727672B8FBFF02F798B7FAFF06FFF17B73766CD2
        F8FF04D906002BFB9CFF05C0000043FFBF03717671B3F9FF02F695B3FBFFFFFF
        04B271748DFAF8FF04D906002BFB9CFF05C0000046C971027670AFF7FF02F89A
        B6FCFFFFFF03E6766EB6F7FF04D906002BFB9CFF05C00000257C76026FADFEF5
        FF019BB4FDFFFFFF03FC9479EBF7FF03D906002D9BFF05C700001E7A7101ADFE
        F5FF03DEBC97B1FEFFFEFF01C7A3F6FF04DF060023CD9DD106D29D00001E75AE
        F6FF09FCD3A06CBAFD9BC1FFFF05FACFCCB7A8CDF8CC04CEAE0500009C020603
        0200001AAFFFF8FF0BF4C49372707BE9FFF7E4FFFF04F8B9B3B6B8F7B401B897
        95000236FFFFF8FF05F1906F7871A3FBFFF1FF04E7381B2A349C33053429124E
        FFFFF7FF04DE6F7273D4FBFFF0FF02FE8BD29AFF04CA9984B6FFFBFF08FEC0FD
        EA82AD968CFEFBFFEFFF018CD49AFF04C6BDE47EB5FBFF07E8759F8DADFFEFD7
        FAFFF0FF02FE8BD29AFF04C5B6FFE47900B0FCFF04B4706E88FEF7FFF0FF02FE
        8BD29AFF04C5B6FFFFE3017AB0FEFF06FA8B74766DA2FCF8FFF0FF02FE8BD29A
        FF01C5B6FEFF0BE682ABFEFFCA6D707EA5DCFDF8FFF0FF02FE8BD29AFF01C5B6
        FEFF08FFE77DA9FC977DB2EEF5FFF0FF02FE8BD29AFF01C5B6FEFFFFFF04E882
        98C5F5F3FFF0FF02FE8BD29AFF01C5B6FEFFFEFF02EC7FADF2FFF0FF02FE8BD2
        9AFF01C5B6FEFFFDFF03EB82ABFEF4FFF0FF02FE8BD2FCFF00F9A7FF01FEF9FC
        FF01C5B6FEFFFCFF01E8D1F3FFF0FF02FE8BD2FEFF02DBAFA3A6FF029FB3E0FE
        FF01C5B6FEFFECFFF0FF08FE8BD3F0BD95746B91A7ED08EC8A6B7698C2F4C6B6
        FEFFECFFF0FF09FE90AD88676F77787A82A781077A78776D6793ACB9FEFFECFF
        F0FF08FE8CD1E4BC8A706C91A7E908E88B6D718FC0EAC5B6FEFFECFFF0FF08FE
        84D0FFFFF6CF9B9CA6FF07969FD2FAFFFFC2B2FEFFECFFEFFF01BAE4FCFF00F2
        A7FF01FEF2FCFF01DCD3FEFFECFF81FFECFF81FFECFF4D4942386B734D4C0E00
        00000000FFFF0000000000003200800000004D49423874746150000000004D49
        42386B734D460C0000000000FFFF000000000000320000000000}
      BoundLabel.Left = 213
      BoundLabel.Top = 89
      BoundLabel.Width = 3
      BoundLabel.Height = 13
      BoundLabel.Transparent = True
    end
    object scGPNumericEdit_delka_jig: TscGPNumericEdit
      Left = 194
      Top = 0
      Width = 61
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 200
      Options.HotColorAlpha = 255
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 150
      Options.FrameNormalColor = clWhite
      Options.FrameHotColor = clWhite
      Options.FrameFocusedColor = clWhite
      Options.FrameDisabledColor = clWhite
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
      ContentMarginLeft = 1
      ContentMarginRight = 1
      ContentMarginTop = 1
      ContentMarginBottom = 1
      HidePromptTextIfFocused = False
      PromptTextColor = clNone
      Transparent = True
      Increment = 1.000000000000000000
      SupportUpDownKeys = False
      SupportMouseWheel = False
      ValueType = scvtFloat
      MaxValue = 1000000.000000000000000000
      Value = 1000.000000000000000000
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
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentBiDiMode = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnKeyDown = FormKeyDown
    end
    object scGPNumericEdit_delka_podvozek: TscGPNumericEdit
      Left = 184
      Top = 172
      Width = 61
      Height = 19
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 200
      Options.HotColorAlpha = 255
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 150
      Options.FrameNormalColor = clWhite
      Options.FrameHotColor = clWhite
      Options.FrameFocusedColor = clWhite
      Options.FrameDisabledColor = clWhite
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
      ContentMarginLeft = 0
      ContentMarginRight = 0
      ContentMarginTop = 0
      ContentMarginBottom = 0
      HidePromptTextIfFocused = False
      PromptTextColor = clNone
      Transparent = True
      Increment = 1.000000000000000000
      SupportUpDownKeys = False
      SupportMouseWheel = False
      ValueType = scvtFloat
      MaxValue = 1000000.000000000000000000
      Value = 1000.000000000000000000
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
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentBiDiMode = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = scGPNumericEdit_delka_podvozekChange
      OnKeyDown = FormKeyDown
    end
    object scGPNumericEdit_sirka_jig: TscGPNumericEdit
      Left = 292
      Top = 100
      Width = 61
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 200
      Options.HotColorAlpha = 255
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 150
      Options.FrameNormalColor = clWhite
      Options.FrameHotColor = clWhite
      Options.FrameFocusedColor = clWhite
      Options.FrameDisabledColor = clWhite
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
      ContentMarginLeft = 1
      ContentMarginRight = 1
      ContentMarginTop = 1
      ContentMarginBottom = 1
      HidePromptTextIfFocused = False
      PromptTextColor = clNone
      Transparent = True
      Increment = 1.000000000000000000
      SupportUpDownKeys = False
      SupportMouseWheel = False
      ValueType = scvtFloat
      MaxValue = 1000000.000000000000000000
      Value = 1000.000000000000000000
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
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentBiDiMode = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnKeyDown = FormKeyDown
    end
    object scGPNumericEdit_vyska_jig: TscGPNumericEdit
      Left = 45
      Top = 80
      Width = 61
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 200
      Options.HotColorAlpha = 255
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 150
      Options.FrameNormalColor = clWhite
      Options.FrameHotColor = clWhite
      Options.FrameFocusedColor = clWhite
      Options.FrameDisabledColor = clWhite
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
      ContentMarginLeft = 1
      ContentMarginRight = 1
      ContentMarginTop = 1
      ContentMarginBottom = 1
      HidePromptTextIfFocused = False
      PromptTextColor = clNone
      Transparent = True
      Increment = 1.000000000000000000
      SupportUpDownKeys = False
      SupportMouseWheel = False
      ValueType = scvtFloat
      MaxValue = 1000000.000000000000000000
      Value = 1000.000000000000000000
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
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearType
      ParentBiDiMode = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnKeyDown = FormKeyDown
    end
    object scHTMLLabel_jig_info: TscHTMLLabel
      Left = 2
      Top = 178
      Width = 145
      Height = 13
      Margins.Left = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      TabOrder = 4
      AutoSizeHeight = True
      AutoSizeWidth = True
      AutoExecuteURL = True
      Caption = '* pohled na Jig p'#345'i nav'#283#353'ov'#225'n'#237
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
  end
  object scGPGlyphButton_OPTIONS: TscGPGearActivityIndicator
    AlignWithMargins = True
    Left = 7007
    Top = 11049
    Width = 127
    Height = 127
    Hint = 'Nastaven'#237' aplikace'
    Margins.Top = 5
    Margins.Bottom = 5
    Align = alCustom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 24
    Visible = False
    Active = False
    AnimationAcceleration = False
    GlyphColor = clWhite
    GlyphColorAlpha = 255
    GlyphThickness = 1
    Kind = scgpgkOneGear
    TransparentBackground = True
  end
  object scGPSwitch: TscGPSwitch
    Left = 762
    Top = 55
    Width = 45
    Height = 20
    Margins.Left = 1
    Margins.Top = 1
    Margins.Right = 1
    Margins.Bottom = 1
    Align = alCustom
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Orientation = 90
    Font.Style = []
    Font.Quality = fqClearType
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 25
    Animation = True
    Color = clRed
    FrameColor = clLime
    FrameOnColor = clLime
    FramePressedColor = clLime
    State = scswOff
    StyleKind = scswsStyled
    ThumbColor = clWhite
    ThumbOnColor = clWhite
    ThumbPressedColor = clBlack
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
  object scGPGlyphButton_katalog: TscGPGlyphButton
    Left = 134
    Top = 283
    Width = 222
    Height = 29
    Hint = 'Katalog dopravn'#237'k'#367
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 26
    TabStop = True
    OnClick = scGPGlyphButton_katalogClick
    Animation = False
    Caption = 'scGPGlyphButton_katalog'
    CanFocused = True
    CustomDropDown = False
    Layout = blGlyphLeft
    TransparentBackground = True
    ColorValue = clRed
    Options.NormalColor = clBtnText
    Options.HotColor = clWhite
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
    Options.HotColor2Alpha = 200
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = clNone
    Options.FrameHotColor = clNone
    Options.FramePressedColor = clNone
    Options.FrameFocusedColor = clNone
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 150
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 30
    Options.FontNormalColor = clWhite
    Options.FontHotColor = clWhite
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
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = clBtnText
    GlyphOptions.DisabledColor = clBtnText
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 200
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 100
    GlyphOptions.Kind = scgpbgkFolder
    GlyphOptions.Thickness = 2
    GlyphOptions.ThicknessScaled = False
    GlyphOptions.Size = 0
    GlyphOptions.StyleColors = True
    TextMargin = -1
    WidthWithCaption = 0
    WidthWithoutCaption = 0
    ShowCaption = True
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
  object scGPTrackBar_uchyceni: TscGPTrackBar
    Left = 608
    Top = 210
    Width = 101
    Height = 20
    Hint = 'pozice palce '
    Margins.Left = 1
    Margins.Right = 1
    Margins.Bottom = 2
    Align = alCustom
    ParentShowHint = False
    ShowHint = True
    TabOrder = 27
    TabStop = True
    OnMouseMove = scGPTrackBar_uchyceniMouseMove
    TransparentBackground = False
    ShowFocusRect = True
    JumpWhenClick = True
    CanFocused = True
    MouseSupport = True
    MinValue = 0
    MaxValue = 15
    Value = 10
    Vertical = False
    ReadOnly = False
    Color = clBlack
    TrackColor = clWhite
    TrackProgressColor = clWhite
    TrackSize = 1
    TrackAlpha = 100
    TrackProgressAlpha = 255
    ThumbColor = clWhite
    ThumbHotColor = clWhite
    ThumbPressedColor = clWhite
    ThumbDisabledColor = clBtnShadow
    ThumbShapeStyle = scgptssRoundRect
    ThumbCursor = crDefault
    ThumbUseCursor = False
  end
  object rHTMLHint1: TrHTMLHint
    HintFont.Charset = DEFAULT_CHARSET
    HintFont.Color = clActiveCaption
    HintFont.Height = -16
    HintFont.Name = 'Tahoma'
    HintFont.Style = []
    HintFont.Quality = fqClearType
    Left = 920
    Top = 336
  end
  object scStyledForm2: TscStyledForm
    DWMClientShadow = False
    DWMClientShadowHitTest = False
    DropDownForm = False
    DropDownAnimation = False
    DropDownBorderColor = clBtnShadow
    StylesMenuSorted = False
    ShowStylesMenu = False
    StylesMenuCaption = 'Styles'
    ClientWidth = 0
    ClientHeight = 0
    ShowHints = True
    Buttons = <>
    ButtonFont.Charset = DEFAULT_CHARSET
    ButtonFont.Color = clWindowText
    ButtonFont.Height = -11
    ButtonFont.Name = 'Tahoma'
    ButtonFont.Style = []
    CaptionFont.Charset = DEFAULT_CHARSET
    CaptionFont.Color = clWindowText
    CaptionFont.Height = -11
    CaptionFont.Name = 'Tahoma'
    CaptionFont.Style = [fsBold]
    CaptionAlignment = taLeftJustify
    InActiveClientColor = clWindow
    InActiveClientColorAlpha = 100
    InActiveClientBlurAmount = 5
    Tabs = <>
    TabFont.Charset = DEFAULT_CHARSET
    TabFont.Color = clWindowText
    TabFont.Height = -11
    TabFont.Name = 'Tahoma'
    TabFont.Style = []
    ShowButtons = True
    ShowTabs = True
    TabIndex = 0
    TabsPosition = sctpLeft
    ShowInactiveTab = True
    CaptionWallpaperIndex = -1
    CaptionWallpaperInActiveIndex = -1
    CaptionWallpaperLeftMargin = 1
    CaptionWallpaperTopMargin = 1
    CaptionWallpaperRightMargin = 1
    CaptionWallpaperBottomMargin = 1
    Left = 440
    Top = 288
  end
end
