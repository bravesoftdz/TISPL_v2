object Form_parametry_vozik: TForm_parametry_vozik
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Form_parametry_vozik'
  ClientHeight = 338
  ClientWidth = 651
  Color = 15790320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel_delka_jig: TrHTMLLabel
    Left = 110
    Top = 88
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
  end
  object rHTMLLabel_JIG: TrHTMLLabel
    Left = 70
    Top = 112
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
  end
  object rHTMLLabel_sirka_jig: TrHTMLLabel
    Left = 217
    Top = 88
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
  end
  object rHTMLLabel_vyska_jig: TrHTMLLabel
    Left = 323
    Top = 88
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
  end
  object rHTMLLabel_delka_podvozek: TrHTMLLabel
    Left = 110
    Top = 146
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
  end
  object rHTMLLabel_podvozek_zaves: TrHTMLLabel
    Left = 32
    Top = 177
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
  end
  object Button_save: TscGPButton
    Left = 206
    Top = 279
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
  object Button_storno: TscGPButton
    Left = 328
    Top = 279
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
  object scGPButton_vozik: TscGPButton
    Left = 0
    Top = 51
    Width = 130
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 2
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
  object scGPNumericEdit_delka_jig: TscGPNumericEdit
    Left = 111
    Top = 110
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
    TabOrder = 3
  end
  object scGPNumericEdit_sirka_jig: TscGPNumericEdit
    Left = 217
    Top = 110
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
    TabOrder = 4
  end
  object scGPNumericEdit_vyska_jig: TscGPNumericEdit
    Left = 323
    Top = 110
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
    TabOrder = 5
    Visible = False
  end
  object scGPNumericEdit_delka_podvozek: TscGPNumericEdit
    Left = 110
    Top = 167
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
    TabOrder = 6
  end
  object scGPPanel2: TscGPPanel
    Left = 0
    Top = 0
    Width = 651
    Height = 34
    Align = alTop
    TabOrder = 7
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
      Left = 601
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 0
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
    end
    object scLabel1: TscLabel
      Left = 0
      Top = 0
      Width = 564
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
      Caption = 'Parametry voz'#237'k'
    end
    object scGPGlyphButton_info: TscGPGlyphButton
      Left = 564
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
end
