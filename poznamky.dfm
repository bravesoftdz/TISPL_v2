object Form_poznamky: TForm_poznamky
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Pozn'#225'mka'
  ClientHeight = 338
  ClientWidth = 934
  Color = 15790320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel_InfoText: TrHTMLLabel
    Left = 10
    Top = 29
    Width = 298
    Height = 19
    Caption = 'Pozn'#225'mka ke vzniku technologick'#233'ho '#269'asu'
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
    Width = 934
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
    ExplicitWidth = 612
    object Konec: TscGPGlyphButton
      AlignWithMargins = True
      Left = 884
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
      ExplicitLeft = 562
    end
    object scLabel_titulek: TscLabel
      AlignWithMargins = True
      Left = 10
      Top = 0
      Width = 874
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
      Caption = 'Pozn'#225'mka'
      ExplicitWidth = 552
    end
  end
  object scGPMemo: TscGPMemo
    Left = 10
    Top = 48
    Width = 911
    Height = 233
    Options.NormalColor = clWindow
    Options.HotColor = clWindow
    Options.FocusedColor = clWindow
    Options.DisabledColor = clWindow
    Options.NormalColorAlpha = 200
    Options.HotColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 150
    Options.FrameNormalColor = 13158600
    Options.FrameHotColor = 13158600
    Options.FrameFocusedColor = 13158600
    Options.FrameDisabledColor = clBtnText
    Options.FrameWidth = 2
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 50
    Options.FontNormalColor = 10114859
    Options.FontHotColor = 10114859
    Options.FontFocusedColor = 10114859
    Options.FontDisabledColor = clGrayText
    Options.ShapeFillGradientAngle = -90
    Options.ShapeFillGradientBeginColorOffset = 25
    Options.ShapeFillGradientEndColorOffset = 25
    Options.ShapeCornerRadius = 0
    Options.ScaleFrameWidth = False
    Options.StyleColors = True
    ScrollBarOptions.BorderWidth = 0
    ScrollBarOptions.FillColor = clBtnFace
    ScrollBarOptions.FillColorAlpha = 100
    ScrollBarOptions.ThumbColor = clWindowText
    ScrollBarOptions.ThumbColorAlpha = 50
    ScrollBarOptions.ThumbColorHotAlpha = 100
    ScrollBarOptions.ThumbColorPressedAlpha = 150
    ScrollBarOptions.ThumbRounded = False
    ScrollBarOptions.Position = scgpsbDefault
    ScrollBarOptions.Size = 11
    Color = clWhite
    ContentMarginLeft = 5
    ContentMarginRight = 5
    ContentMarginTop = 5
    ContentMarginBottom = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 10114859
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 1
    Transparent = True
    OnKeyDown = scGPMemoKeyDown
  end
  object scGPButton_OK: TscGPButton
    Left = 336
    Top = 295
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
    OnClick = scGPButton_OKClick
    Animation = False
    Caption = 'Ulo'#382'it'
    CanFocused = True
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
  object scGPButton_storno: TscGPButton
    Left = 475
    Top = 295
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    TabStop = True
    OnClick = scGPButton_stornoClick
    Animation = False
    Caption = 'Storno'
    CanFocused = True
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = 33023
    Options.HotColor = 1871359
    Options.PressedColor = clRed
    Options.FocusedColor = 33023
    Options.DisabledColor = clPurple
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
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
  object scGPCheckBox_zaokrouhlit: TscGPCheckBox
    Left = 704
    Top = 313
    Width = 52
    Height = 25
    Hint = 'zobrazit na t'#345'i desetinn'#225' m'#237'sta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 4
    TabStop = True
    Visible = False
    OnClick = scGPCheckBox_zaokrouhlitClick
    Animation = False
    Caption = '0,000'
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
    UseFontColorToStyleColor = False
  end
  object scGPCheckBox_STOPKA: TscGPCheckBox
    Left = 592
    Top = 287
    Width = 172
    Height = 25
    Hint = 'zobrazit na t'#345'i desetinn'#225' m'#237'sta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    TabStop = True
    Visible = False
    OnClick = scGPCheckBox_STOPKAClick
    Animation = False
    Caption = 'STOP stanice na konci objektu'
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
    Options.FrameNormalColor = 13158600
    Options.FrameHotColor = clHighlight
    Options.FramePressedColor = clHighlight
    Options.FrameDisabledColor = 13158600
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
    Options.ShapeSize = 18
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
    OptionsChecked.FrameNormalColor = 13158600
    OptionsChecked.FrameHotColor = clHighlight
    OptionsChecked.FramePressedColor = clHighlight
    OptionsChecked.FrameDisabledColor = 13158600
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
    OptionsChecked.ShapeSize = 18
    OptionsChecked.CheckMarkThickness = 2
    OptionsChecked.StyleColors = True
    Checked = False
    ScaleFrameWidth = True
    ScaleCheckMarkThickness = True
    ShowFocusRect = True
    UseFontColorToStyleColor = False
  end
  object scGPButton_zamek_PT: TscGPButton
    Left = 10
    Top = 284
    Width = 15
    Height = 21
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 6
    OnClick = scButton_zamek_PTaDDClick
    Animation = False
    CanFocused = False
    CustomDropDown = False
    Margin = -1
    Spacing = 1
    Layout = blGlyphLeft
    Images = Form1.scGPVirtualImageList1
    ImageIndex = 38
    ImageMargin = 0
    TransparentBackground = True
    Options.NormalColor = 16448250
    Options.HotColor = 16448250
    Options.PressedColor = 16448250
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.FrameNormalColor = 16448250
    Options.FrameHotColor = 16448250
    Options.FramePressedColor = 16448250
    Options.FrameFocusedColor = 16448250
    Options.FrameDisabledColor = 16448250
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clWhite
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
  object scGPButton_zamek_DD: TscGPButton
    Left = 40
    Top = 284
    Width = 15
    Height = 21
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 7
    OnClick = scButton_zamek_PTaDDClick
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
    Options.NormalColor = 16448250
    Options.HotColor = 16448250
    Options.PressedColor = 16448250
    Options.FocusedColor = clWhite
    Options.DisabledColor = clWhite
    Options.NormalColorAlpha = 255
    Options.HotColorAlpha = 255
    Options.PressedColorAlpha = 255
    Options.FocusedColorAlpha = 255
    Options.DisabledColorAlpha = 255
    Options.FrameNormalColor = 16448250
    Options.FrameHotColor = 16448250
    Options.FramePressedColor = 16448250
    Options.FrameFocusedColor = 16448250
    Options.FrameDisabledColor = 16448250
    Options.FrameWidth = 1
    Options.FrameNormalColorAlpha = 255
    Options.FrameHotColorAlpha = 255
    Options.FramePressedColorAlpha = 255
    Options.FrameFocusedColorAlpha = 255
    Options.FrameDisabledColorAlpha = 255
    Options.FontNormalColor = clWhite
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
end
