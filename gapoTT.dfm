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
  object rHTMLLabel_InfoText: TrHTMLLabel
    Left = 8
    Top = -20
    Width = 518
    Height = 19
    AutoSize = False
    Caption = 'Upozorn'#283'n'#237
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
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
  object scGPPanel_hlavicka: TscGPPanel
    Left = 0
    Top = 0
    Width = 378
    Height = 34
    Align = alTop
    TabOrder = 0
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
      ModalResult = 2
      ModalSetting = True
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
      ModalSetting = True
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
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 20
      Options.PressedColorAlpha = 30
      Options.FocusedColorAlpha = 255
      Options.DisabledColorAlpha = 0
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
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 200
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
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
    Options.StyleColors = True
    HotImageIndex = -1
    ModalResult = 1
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
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
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
    Options.ShapeCornerRadius = 10
    Options.ShapeStyle = scgpRect
    Options.ArrowSize = 9
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
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
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
  object scImage_zamky: TscGPImage
    Left = 8
    Top = 40
    Width = 271
    Height = 25
    TabOrder = 7
    CustomImageIndex = -1
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
      001908020000007610E63E000000097048597300000EC300000EC301C76FA864
      000005614944415478DAED9C5B4823571C8733B96A1113ACE4B289488AA642AD
      BED8BB502D2E82602C1814A3952E65B77D68FBB617D87D28CBB294F6A194B6CB
      2294B6D60BDA08351677A542DC657BA155C4D607BB424592D4984DC558316A12
      A77F9BB28CCEC599C9994BE9390F613CCC39F9E6C31F736E84D8DFDFD7882A73
      737335353570613018C4F5A09E4EBE38E3864F7FDF7DC549D4D3C9D9E859F8BC
      61BDA138897A3A8186040E8C060786A9C81698783C3E3B3BBBBDBD5D54545457
      5767B55A95727222097360B2D9ECC6FCD7DA1F3F30EDC5F60AECC4E9774B3CF5
      F28BE0898184843B307292A8C7097760909040275353538B8B8BC04092244110
      C0535D5DDDDCDCACD3E9E474C293842130D072E7C3274A767FA756C6CC4F5BDE
      B823E819F214C11F03090947606426518F138EC0202149A7D3E3E3E3B1586C6B
      6BABBCBCBCACAC2C1C0EAFAEAE161717DBEDF6B6B636EAFD923AE14F723C30D0
      72F7A327735FBFA5B726ADCF9BE3DF1767E2F0E746C163056FFDCAE719F21721
      080309095B60E427518F13B6C0A022595858989999C964325EAFB7B2B23257B9
      BCBC1C0C06F57A7D4343436D6DAD3C4EF8931C0FCC1F5FBD5DBE7C132EC22F7C
      6C7FF15CAE3276A7AFECBB37E162B5FAD229EF55194408C24042C21618F949D4
      E3842D30A848FAFBFB1389445555554B4B0BB57E7272726969A9B4B4B4B7B757
      1E27FC498E07E6C1CDA692CDF978D5EB8E97AF535BFEF949BD3DF9D38ECE62B8
      18974184200C24246C81919F443D4ED802838A64606000E6511D1D1D2E978B5A
      1F8944464747614ED5D3D3238F13FE240C731878CDD17B7C301F3C75CBB7AD7F
      D474614D06118230909070CC616426518F138E390C1292C1C1C1F5F5F5F6F676
      181952EB617C38363666B3D9BABBBBE571C29F84EFB272FCE780F35B3F0C130B
      2F44E411C11F030989D06565E948D4E344E8B2B25092A1A121984A7576763A9D
      4E6A7D341A1D1919810995DFEF97C7097F12BE8149FC72DBF18DF72F83ADE07C
      5841118C1848488406463A12F538111A189E24F07E0B8542D96C1630E0FD0083
      C0C2C242EAFDA9540A068D7033C0E874BAC6C64678EF49E1440409483B0CCCC6
      FD7BC6E0AB7A728F8DC098D9329129B800880C61DAF77E5EE2A9472E421C0612
      11B72E3EA3A104464112F538B99CBE4C0D0C2A92959595898909202149F2E0E0
      C068346AB55A6A3F5009FF935049100490B4B6B6BADD6E299C8820F1783C8781
      59FF61D0153AA3E15D228D9FD99EEB462E421C06121177AFBE440D8C8224EA71
      72DD7838937E1818542430F60B040266B3B9A9A989BB87E9E9E96432E9F3F960
      F428851311241515158781C98DF9328471BDB18FBBA52D744E4FEE474F0F599F
      F22117210E03898899771A3494C02848A21E27D7F4D73494C0202481C992C3E1
      E8EAEAE2EE677878786D6D2D37DD92C88950922381491A9C8F9C5FE16EB9F3BE
      DB9C8EB205068908A11848443006461112F538610C0C2A9263EB1C8C85BA3E21
      9D1341244702435F55A097D47BAEE24C9C2330F98B108A81440463601421518F
      13C6C0FCCF9DE0C0E0C0B092E0C0D0497060706058497060E82438303830AC24
      383074121C181C185612490393FF4288222478950CAF92B19248B74A1608042C
      160B9F05EECDCD4DB6A57645488E04668F284C347DCADDB274FA351399E2D887
      C95F84500C24221803A308897A9C3006267F92875BA8271E7249A7D38C9BB90A
      9248B2D32F5A04F25D6DFE2452EFF4FF179D48B4D30FE3BDDC211D3E9DE40EE9
      C048520A2722485C2ED73F67C97EBB5730F10A7190E6D392D41A765BBF2C79BC
      1EB90871184844DCBEF4AC867A964C3912F538B992B9A2A19E25434422AE48E1
      4444E17B5AF9C467402242C14EF0AFC6D00BFE99257AC181F9B7E0C0D00B0E0C
      BD40C3BF01DBD52BB0FD03910E0000000049454E44AE426082}
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
