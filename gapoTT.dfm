object F_gapoTT: TF_gapoTT
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'F_gapoTT'
  ClientHeight = 496
  ClientWidth = 378
  Color = 15790320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel_InfoText_nefunguje: TrHTMLLabel
    Left = 8
    Top = -20
    Width = 518
    Height = 19
    AutoSize = False
    Color = 15790320
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentColor = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
  end
  object rHTMLLabel_legenda: TrHTMLLabel
    Left = -2
    Top = 377
    Width = 154
    Height = 116
    Caption = 
      '- m'#283'n'#283'n'#225' hodnota'#13'<font color=gray>- nem'#283'n'#283'n'#225' hodnota</font>'#13'<fon' +
      't color=silver>- p'#367'vodn'#237' hodnota</font>'#13'<font color=#804000>1. o' +
      'blast</font>'#13'<font color=#008000>2. oblast</font>'#13'<font color=bl' +
      'ack>3. oblast</font>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    FontColor = 33023
  end
  object rHTMLLabel_legenda_titulek: TrHTMLLabel
    Left = 0
    Top = 352
    Width = 65
    Height = 19
    Caption = 'Legenda:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
  end
  object rHTMLLabel_InfoText: TrHTMLLabel
    Left = 0
    Top = 432
    Width = 500
    Height = 19
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    WordWrap = True
  end
  object scGPPanel_hlavicka: TscGPPanel
    Left = 0
    Top = 0
    Width = 378
    Height = 34
    Align = alTop
    TabOrder = 0
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
    Caption = 'scGPPanel_hlavicka'
    TransparentBackground = True
    StorePaintBuffer = True
    Sizeable = False
    WallpaperIndex = -1
    object Konec: TscGPGlyphButton
      AlignWithMargins = True
      Left = 328
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 0
      OnClick = scGPButton_stornoClick
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
      ModalResult = 2
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
    object scLabel_titulek: TscLabel
      AlignWithMargins = True
      Left = 92
      Top = 0
      Width = 199
      Height = 34
      Margins.Left = 10
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alClient
      BiDiMode = bdLeftToRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBiDiMode = False
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
      Caption = 
        'Glob'#225'ln'#237' aktualizace parametr'#367' pohon'#367' a objekt'#367' z d'#367'vodu zm'#283'ny T' +
        'T'
    end
    object scGPGlyphButton_info: TscGPGlyphButton
      Left = 291
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
      ModalSetting = True
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
    object scGPGlyphButton_copy: TscGPGlyphButton
      Left = 45
      Top = 0
      Width = 37
      Height = 34
      Hint = 'Zkop'#237'rovat paremetry objektu'
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNone
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = scGPGlyphButton_copyClick
      Animation = True
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphLeft
      TransparentBackground = True
      ColorValue = clGray
      Options.NormalColor = clBtnText
      Options.HotColor = clBtnText
      Options.PressedColor = clBtnText
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = clGray
      Options.NormalColor2 = clNone
      Options.HotColor2 = clNone
      Options.PressedColor2 = clNone
      Options.FocusedColor2 = clNone
      Options.DisabledColor2 = clNone
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 20
      Options.PressedColorAlpha = 30
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 0
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
      GlyphOptions.NormalColor = 33791
      GlyphOptions.HotColor = 1871359
      GlyphOptions.PressedColor = clBtnText
      GlyphOptions.FocusedColor = 1871359
      GlyphOptions.DisabledColor = clGray
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
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
    object scButton_csv: TscButton
      Left = 0
      Top = 0
      Width = 45
      Height = 34
      Align = alLeft
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      TabStop = True
      OnClick = scButton_csvClick
      Animation = False
      CanFocused = True
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphLeft
      Images = Form1.scGPVirtualImageList1
      ImageIndex = 41
      ImageMargin = 0
      TransparentBackground = True
      ColorOptions.NormalColor = clHighlight
      ColorOptions.HotColor = clHighlight
      ColorOptions.PressedColor = clHighlight
      ColorOptions.FocusedColor = clHighlight
      ColorOptions.DisabledColor = clHighlight
      ColorOptions.FrameNormalColor = clHighlight
      ColorOptions.FrameHotColor = clHighlight
      ColorOptions.FramePressedColor = clHighlight
      ColorOptions.FrameFocusedColor = clHighlight
      ColorOptions.FrameDisabledColor = clHighlight
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clBtnText
      ColorOptions.FontHotColor = clBtnText
      ColorOptions.FontPressedColor = clBtnText
      ColorOptions.FontFocusedColor = clBtnText
      ColorOptions.FontDisabledColor = clHighlight
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clHighlight
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsPushButtonTransparent
      UseGalleryMenuImage = False
      UseGalleryMenuCaption = False
      CustomImageNormalIndex = -1
      CustomImageHotIndex = -1
      CustomImagePressedIndex = -1
      CustomImageDisabledIndex = -1
      CustomImageFocusedIndex = -1
      ScaleMarginAndSpacing = False
      WidthWithCaption = 0
      WidthWithoutCaption = 0
      UseFontColorToStyleColor = False
      RepeatClick = False
      RepeatClickInterval = 100
      GlowEffect.Enabled = False
      GlowEffect.Color = clHighlight
      GlowEffect.AlphaValue = 95
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = False
      GlowEffect.HotColor = clHighlight
      GlowEffect.PressedColor = clHighlight
      GlowEffect.FocusedColor = clHighlight
      GlowEffect.PressedGlowSize = 7
      GlowEffect.PressedAlphaValue = 155
      GlowEffect.States = [scsHot, scsPressed, scsFocused]
      ImageGlow = False
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
  end
  object scGPButton_OK: TscGPButton
    Left = 104
    Top = 391
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = scGPButton_OKClick
    Animation = False
    Caption = 'OK'
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = 33023
    Options.HotColor = 1871359
    Options.PressedColor = clLime
    Options.FocusedColor = 1871359
    Options.DisabledColor = clBlack
    Options.NormalColor2 = clNone
    Options.HotColor2 = clNone
    Options.PressedColor2 = clNone
    Options.FocusedColor2 = clNone
    Options.DisabledColor2 = clNone
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
    Options.NormalColor2Alpha = 255
    Options.HotColor2Alpha = 255
    Options.PressedColor2Alpha = 255
    Options.FocusedColor2Alpha = 255
    Options.DisabledColor2Alpha = 255
    Options.FrameNormalColor = 33023
    Options.FrameHotColor = 1871359
    Options.FramePressedColor = clGray
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
  object scGPButton_storno: TscGPButton
    Left = 223
    Top = 391
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 2
    TabStop = True
    OnClick = scGPButton_stornoClick
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
  object Edit1: TEdit
    Left = 92
    Top = -30
    Width = 121
    Height = 21
    TabOrder = 3
    Text = 'Edit1'
    OnKeyDown = FormKeyDown
  end
  object scScrollBar_horizont: TscScrollBar
    Left = 8
    Top = 356
    Width = 361
    Height = 16
    LargeChange = 20
    PageSize = 0
    SmallChange = 10
    TabOrder = 4
    TabStop = False
    Visible = False
    OnChange = scScrollBar_horizontChange
  end
  object scScrollBar_vertical: TscScrollBar
    Left = 361
    Top = 37
    Width = 16
    Height = 313
    Kind = sbVertical
    LargeChange = 20
    PageSize = 0
    SmallChange = 10
    TabOrder = 5
    TabStop = False
    Visible = False
    OnChange = scScrollBar_verticalChange
  end
  object scGPButton_zamek_locked: TscGPButton
    Left = 223
    Top = 459
    Width = 20
    Height = 29
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 6
    Visible = False
    Animation = False
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 37
    ImageMargin = 0
    TransparentBackground = True
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
    Options.FrameNormalColor = clSilver
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clSilver
    Options.FrameFocusedColor = clSilver
    Options.FrameDisabledColor = clWhite
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 0
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
    Down = False
    GroupIndex = 0
    AllowAllUp = False
  end
  object scImage_zamky: TscGPImage
    Left = 8
    Top = 40
    Width = 271
    Height = 25
    TabOrder = 7
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
    Caption = 'scImage_zamky'
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
      0954506E67496D61676589504E470D0A1A0A0000000D49484452000001100000
      00190806000000F9727169000000097048597300000EC400000EC401952B0E1B
      000005814944415478DAED9C5D48646518C7FF4767460D71C46466DC51C4C012
      32BCD0826A21051741700C14C58FAC2576EBA2BADBADEB2E22EA22A2826597B0
      CC0F3485D4B024595DD8AD28456CBDB084446626C75913C7C4CFD169DF433BE0
      BCF3753CEF3BE6ECF3DCCC306778F8F1BBF873DEF779CF51F6F7F70310543333
      33282F2F17D5EECCF7FEF26291FAF972E7D299E23EABBD2FB92FA99F37EC37CE
      14F759EEAD50805080244B6F0A90C4F70E1B20878787589FFD06293F7F84B43D
      0FF6D26D502EBC8F9CC7CF0B01F57ABD989E9EC6D6D616323333515151018BC5
      A2BBB74CEE9330C71320276526DF7CC51320E45B1C332B2E4018E4F6C74F2267
      F74FEECF1EF333C87EFD165253534F04CA7A8F8F8F637E7E5E850C0402501445
      852E2D2D454D4D8DAEDE32B8F530C70A103DCCE49BAF580142BEC532B33A1620
      070707D8FDE4A920E4A6C1029FE53998BD3F22CBEF557F5B4F7F0CE96FDE85D1
      68D404CA7A0F0F0FC3E3F16073731385858528282880D3E9C4F2F232B2B2B260
      B3D9505F5F7FA2DE32B8F532470B10BDCCE49BEF1B2D40C8B778DFAC8E05C85F
      5FBF85C2C56BEA77E7F39FC2F6C2E5E01F3DB7AEA3E0CE1BEAF7E5D27770CEF1
      AE2609737373989A9A82DFEF87C3E140717171F0DAE2E2224646466030185059
      5989B2B2324DBD6571EB658E16207A99C937CF1C2D40C8B778DFAC8E05C8BD6B
      D5C8D99885B7E435E4BDF81EF7E7BF3F3B0F9BEF176CA766C3F8B6579384AEAE
      2EACADADA1A4A404B5B5B5DCF5B1B1312C2C2C203737171D1D1D9A7ACBE2D6CB
      1C2D40F432936F9E395A80906FF1BE59717B20ECB68655B85B967BB32338F75D
      23B60C8F22EDEA8A2609DDDDDDEA464D535313F2F3F3B9EB2E970B030303EAC6
      4D7B7BBBA6DEB2B8F532C7DA03D1C34CBE79E6587B20E45B2C332B4D635CEFAF
      83B0FFD0AAAEC132AEBA3449E8E9E9C1EAEA2A1A1A1AD4B55668B135D7D0D010
      AC562BDADADA340B96C1AD9759CF18371633F9E699F58C71C9B77666569A0264
      EDB7EF91F7AD03FF18AD48BFE2D424A1B7B757DDAC696E6E86DD6EE7AEBBDD6E
      F4F7F7AB9B36ADADAD42059F945B2FB39E0089C54CBE79663D0142BEB533B352
      3CF33703A691576008ECC50435F9379116D851BF3358BF92867DC717C1397428
      28BB3D9A9C9C5447450C94DD86B1F554464606D77B6767475D8FB15B3406CC46
      47555555C1597468EFF53F6E4306B748E6D00011C94CBE79E6D00021DF727DB3
      529CB73A03F993176342462A575527ACCFB68595B0B4B484D1D1511596CD978F
      8E8E603299909292C2F561D7EEDF0DA9D7D81C9AC1D6D5D5A1A8A8286CEFD59F
      7A20835B24736880886426DF3C736880906FB9BE5929AE3BBD01B67EF22B26AC
      565D8F1BD03A79F97E3AEEC37DA11796A71BC34A606BA8C1C14198CD66545757
      C7DD7B6262023E9F0F8D8D8DC1B51997FEFFADFB44738B640E0D1091CCE49B67
      0E0D10F22DD737AB6080F88C763C7225FEB5FAF68745301FB8630608DB84C9CB
      CB434B4B4BDCBDFBFAFAB0B2B2726C73279260D1DC22992305880866F2CD3347
      0A10F22DC737AB608044DB7D0E573B1FE4ABA7E0E2099068BBB8E12ADCEE7024
      C1A2B94532470A1011CCE49B678E1420E45B8E6F56142012055380508024B36F
      56142012055380508024B36F56142012055380508024B36F5614201205538050
      8024B36F563485014D6192C5374D614E710AB3A76460ADFAF3B81BE64EBCAA9E
      7E8BE71C487676B6E699F3C6C6465C7372D1DC22992305880866F2CD33470A10
      F22DC737AB849D448DF4429270C58ED7FE1F4EEAE9653ECD93A80FA3EFD33C89
      FA30FA66A578EEDE0CA48FBE04E5E840336420C588DDBAAF90F344F86761D89A
      E9C1B97BADF5E0DC3D5BAB85EBBDFEFB6DC8E016C9CC3D0B2390997CF3CCDCB3
      30E45BAA6F56F4567689BDE9ADEC89ED4D6F654F7C6F0A1089BD294012DB9B02
      24F1BDFF053DDF27207033621B0000000049454E44AE426082}
    PngImage.Data = {
      89504E470D0A1A0A0000000D4948445200000110000000190806000000F97271
      69000000097048597300000EC400000EC401952B0E1B0000053D4944415478DA
      ED9C5D48646518C7FFA333A313E288C97CEC28626009195E6841B590828B2038
      068AE247B64BECD64575B75BD75D44D4454405CB2EB1657EA029A4862509BAB0
      D5528AD87A610989E8E4386BE298F8393AEDFB82B3E09933E7CC341E9FD77DDF
      9B19E6300F3FFE173FCEFB3CEF39A6BDBDBD3092B8262727515A5A9ACC9242D7
      FEEA5201FF7CF5D6BC50DCA2D6BEECBBCC3F6F7A6E0AC52D6A6D9314C8C9D696
      0231B6B61488B1B5630AE4E0E0006B53DF22E5EEC748DBF56337DD05D3850F90
      FDE4F98441038100262626B0B9B9898C8C0C949595C1E1702435044ADCF10884
      12B7A879C723104ADCA2E6AD2A1006B9F5C9D3C8DEF94B71CD6F7F0E596FDC46
      6A6AAA6E50566F6464043333331C301C0EC3643271E0E2E26254555545AD176F
      C0D4B8F50A841AB7A879EB1508356E51F38E2A90FDFD7DEC7CFA4C0472C3EC40
      D0F102EC819F91190AF0DFD6D29F40FA5BF760B158344159BD818101F8FD7E6C
      6C6C203F3F1F797979585C5CC4C2C202323333E172B9505B5BABA8174FC014B9
      F5088422B7A879EB1108456E51F38E2A90BFBF791BF973D7F9F7C5173F83EBA5
      2B916BFEDB3790F7D39BFCFB42F1BB38E77D4F13747A7A1AE3E3E3088542F07A
      BD282C2C8C5C9B9B9BC3E0E020CC6633CACBCB5152529270C014B9F5088422B7
      A879EB1108456E51F38E2A90FBD72B91BD3E8540D1EB70BFFCBEE24FFF7C7E1E
      AEE0AFD84ACD82E59D8026687B7B3B56575751545484EAEA6A45BDE1E161CCCE
      CE222727076D6D6D09074C915B8F4028728B9AB71E8150E416356FD51E08BBBD
      612BDAADCBFDA9419CFBBE1E9BE6C791766D5913B4A3A38337691A1A1A909B9B
      ABA8B7B4B484DEDE5EDEB4696D6D4D38608ADC7A7B20D4B845CD5B6F0F841AB7
      A8792734C60DFCD607CF8FCD7C0F66BBB6A409DAD9D989959515D4D5D5F17DD6
      F1C5F65BFDFDFD703A9D686969F95F0153E34EC61857E6AD9F3B19635C99B77E
      EE8404B2FAFB0F707FE7C5BF1627D2AF2E6A82767575F1464D6363233C1E8FA2
      9ECFE7434F4F0F6FD83437379F58C0A7C19D0C81C8BCF573274320326FFDDC11
      81ACFD7907D6C18B3087773541ADA10DA485B7F977061B32A561CFFB259F431F
      81B25BA3B1B1313E266290ECD68BEDA56C369BA2DEF6F636DF8BB1DB3206CBC6
      461515158A3974B410A873AB09843AB7A879AB09843AB7A8794704B2F24B2772
      C72E6942AAADA58A5B703EDF12019D9F9FC7D0D0100765B3E5C3C34358AD56A4
      A4A428FECBAE3DE0E0D7D80C9A81D6D4D4A0A0A0403360EADC6A02A1CE2D6ADE
      6A02A1CE2D6ADE11811CED9F42262B562A6EE806748E5D7960C73DF82E74C1F1
      6C7D0494ED9FFAFAFA60B7DB515959A9BBDEE8E82882C120EAEBEB15FBB26801
      53E7561308756E51F3561308756E51F356082468F1E0B1ABFAF7EB5B1F15C0BE
      EF8B0ACA1A306EB71B4D4D4DBAEB757777637979396A632756C054B9B5044295
      5BD4BCB50442955BD4BC150289D6C18DB5B63FCCE5A7E0D440B53ACFC757ACCE
      70AC80A9726B09842AB7A8796B09842AB7A8794B8148819CA9BCA5408CE59602
      91023953794B8118CB2D05220572A6F2960231965B0A440AE44CE52D05622CB7
      9CC2C829CC99CA5B4E618CE5560864D764C36AE517BA0BE78CBEC64FBFA9CD9B
      B3B2B2E29E37AFAFAFC73D27A7CAAD2510AADCA2E6AD2510AADCA2E66DC849D4
      582F5139BED8D15A2A27F592C17D1A27511FE5BC4FE324EAA39CF7C36761FEB8
      83F4A157603ADC8F1B329C62C14ECDD7C87EEAE1997BB65F3A3A731FEF3A3A73
      CFF6695A0153E7567D168638B7A879AB3E0B439C5BD4BCE55BD94FB8B67C2BBB
      B1B5E55BD98DAD2D0572C2B5A5408CAD2D05626CEDFF00A77B4E207553871D00
      00000049454E44AE426082}
  end
  object Memo1: TMemo
    Left = 152
    Top = 112
    Width = 153
    Height = 89
    Lines.Strings = (
      'Memo'
      '1')
    TabOrder = 8
    Visible = False
  end
  object Button1: TButton
    Left = 152
    Top = 207
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 9
    Visible = False
    OnClick = Button1Click
  end
  object scGPCheckBox_prepocitatPT: TscGPCheckBox
    Left = 0
    Top = 377
    Width = 673
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    TabOrder = 10
    TabStop = True
    Visible = False
    Animation = False
    Caption = 
      'P'#345'i zm'#283'n'#283' technologick'#233'ho '#269'asu CT v'#382'dy m'#283'nit  procesn'#237' '#269'as PT, a' +
      ' to i pokud to nen'#237' nutn'#233'. '
    CanFocused = True
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 95
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
    DisabledFontColor = clNone
    Options.NormalColor = clWindow
    Options.HotColor = clWindow
    Options.PressedColor = clWindow
    Options.DisabledColor = clWindow
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 200
    Options.DisabledColorAlpha = 125
    Options.FrameNormalColor = clBtnShadow
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clHighlight
    Options.FrameDisabledColor = clBtnShadow
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.CheckMarkNormalColor = clWindowText
    Options.CheckMarkHotColor = clWindowText
    Options.CheckMarkPressedColor = clWindowText
    Options.CheckMarkDisabledColor = clWindowText
    Options.CheckMarkNormalColorAlpha = 255
    Options.CheckMarkHotColorAlpha = 255
    Options.CheckMarkPressedColorAlpha = 255
    Options.CheckMarkDisabledColorAlpha = 125
    Options.ShapeSize = 20
    Options.CheckMarkThickness = 2
    Options.StyleColors = True
    OptionsChecked.NormalColor = clWindow
    OptionsChecked.HotColor = clWindow
    OptionsChecked.PressedColor = clWindow
    OptionsChecked.DisabledColor = clWindow
    OptionsChecked.NormalColorAlpha = 255
    OptionsChecked.HotColorAlpha = 255
    OptionsChecked.PressedColorAlpha = 200
    OptionsChecked.DisabledColorAlpha = 125
    OptionsChecked.FrameNormalColor = clBtnShadow
    OptionsChecked.FrameHotColor = clHighlight
    OptionsChecked.FramePressedColor = clHighlight
    OptionsChecked.FrameDisabledColor = clBtnShadow
    OptionsChecked.FrameWidth = 2
    OptionsChecked.FrameNormalColorAlpha = 255
    OptionsChecked.FrameHotColorAlpha = 255
    OptionsChecked.FramePressedColorAlpha = 255
    OptionsChecked.FrameDisabledColorAlpha = 255
    OptionsChecked.CheckMarkNormalColor = clWindowText
    OptionsChecked.CheckMarkHotColor = clWindowText
    OptionsChecked.CheckMarkPressedColor = clWindowText
    OptionsChecked.CheckMarkDisabledColor = clWindowText
    OptionsChecked.CheckMarkNormalColorAlpha = 255
    OptionsChecked.CheckMarkHotColorAlpha = 255
    OptionsChecked.CheckMarkPressedColorAlpha = 255
    OptionsChecked.CheckMarkDisabledColorAlpha = 125
    OptionsChecked.ShapeSize = 20
    OptionsChecked.CheckMarkThickness = 2
    OptionsChecked.StyleColors = True
    Checked = True
    State = cbChecked
    ScaleFrameWidth = True
    ScaleCheckMarkThickness = True
    ShowFocusRect = True
    WordWrap = False
    UseFontColorToStyleColor = False
  end
  object rHTMLHint_vypis: TrHTMLHint
    HintFont.Charset = DEFAULT_CHARSET
    HintFont.Color = clWindowText
    HintFont.Height = -11
    HintFont.Name = 'Tahoma'
    HintFont.Style = []
    Left = 56
    Top = 400
  end
end
