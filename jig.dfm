object Form_jig: TForm_jig
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  BorderWidth = 1
  Caption = 'Nastaven'#237' Jigu'
  ClientHeight = 331
  ClientWidth = 410
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel_jig_sirka: TrHTMLLabel
    Left = 44
    Top = 177
    Width = 66
    Height = 19
    Caption = #352#237#345'ka [m]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_jig_vyska: TrHTMLLabel
    Left = 219
    Top = 97
    Width = 72
    Height = 19
    Caption = 'V'#253#353'ka [m]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_jig_pocetks: TrHTMLLabel
    Left = 44
    Top = 97
    Width = 136
    Height = 19
    Caption = 'Po'#269'et [v'#253'robk'#367'/jig]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object rHTMLLabel_jig_delka: TrHTMLLabel
    Left = 219
    Top = 177
    Width = 70
    Height = 19
    Caption = 'D'#233'lka [m]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object scGPPanel2: TscGPPanel
    Left = 0
    Top = 0
    Width = 410
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
    Color = clGrayText
    Caption = 'Nastaven'#237' Jigu'
    TransparentBackground = True
    StorePaintBuffer = True
    ExplicitWidth = 413
    object Konec: TscGPGlyphButton
      AlignWithMargins = True
      Left = 360
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
      ModalResult = 8
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
      ExplicitLeft = 363
    end
    object MinButton: TscGPGlyphButton
      AlignWithMargins = True
      Left = 260
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 1
      Animation = True
      Caption = 'scGPGlyphButton1'
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphLeft
      TransparentBackground = True
      ColorValue = clRed
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBlack
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 50
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
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkMinimize
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
      ExplicitLeft = 263
    end
    object MaxButton: TscGPGlyphButton
      AlignWithMargins = True
      Left = 310
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 2
      Animation = True
      Caption = 'scGPGlyphButton1'
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphLeft
      TransparentBackground = True
      ColorValue = clRed
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBlack
      Options.FocusedColor = clWhite
      Options.DisabledColor = clWhite
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 50
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
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkMaximize
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
      ExplicitLeft = 313
    end
    object scLabel1: TscLabel
      Left = 0
      Top = 0
      Width = 260
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
      TabOrder = 3
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
      Caption = 'Jig'
      ExplicitWidth = 263
    end
  end
  object Button_jig_storno: TscGPButton
    Left = 225
    Top = 283
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    TabStop = True
    OnClick = Button_jig_stornoClick
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
  object Button_jig_save: TscGPButton
    Left = 85
    Top = 283
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    TabStop = True
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
    ModalResult = 1
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
  object Edit_jig_pocet_ks: TrEditNum
    Left = 44
    Top = 122
    Width = 149
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 3
    Value = 235.000000000000000000
  end
  object scGPButton2: TscGPButton
    Left = -16
    Top = 50
    Width = 209
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Animation = False
    Caption = '   Nastaven'#237' jigu'
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
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
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
    Down = True
    GroupIndex = 0
    AllowAllUp = False
  end
  object Edit_jig_vyska: TrEditNum
    Left = 219
    Top = 122
    Width = 149
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 5
    Value = 1.000000000000000000
  end
  object Edit_jig_sirka: TrEditNum
    Left = 44
    Top = 202
    Width = 149
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 6
    Value = 1.000000000000000000
  end
  object Edit_jig_delka: TrEditNum
    Left = 219
    Top = 202
    Width = 149
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 7
    Value = 1.000000000000000000
  end
end
