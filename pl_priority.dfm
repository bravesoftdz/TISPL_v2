object Form_PL_priority: TForm_PL_priority
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Form_PL_priority'
  ClientHeight = 338
  ClientWidth = 776
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object rHTMLLabel_text: TrHTMLLabel
    Left = 8
    Top = 57
    Width = 607
    Height = 59
    Caption = 
      'Ve formul'#225#345'i do'#353'lo ke zm'#283'n'#225'm parametr'#367' <font color=#2b579a>[voz'#237 +
      'ku][TT]</font> ovliv'#328'uj'#237'c'#237' parametry objekt'#367'.'#13#13'Vyberte parametr,' +
      ' jeho'#382' hodnota z'#367'stane na objektech <font color=#2b579a>zachov'#225'n' +
      'a</font>.  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = 19
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
  end
  object scGPRadioButton1: TscGPRadioButton
    Left = 12
    Top = 151
    Width = 30
    Height = 35
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 0
    TabStop = True
    OnClick = scGPRadioButton1Click
    Animation = False
    CanFocused = True
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 255
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
    ImageGlow = True
    Checked = True
    ScaleFrameWidth = True
    ShowFocusRect = True
  end
  object rStringGridEd_tab: TrStringGridEd
    Left = 48
    Top = 122
    Width = 705
    Height = 159
    ColCount = 2
    DefaultColWidth = 350
    DefaultRowHeight = 30
    DefaultDrawing = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing]
    ParentFont = False
    TabOrder = 1
    Columns = <
      item
        Alignment = taCenter
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'Z'#367'stane zachov'#225'no'
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleParentFont = False
      end
      item
        Alignment = taCenter
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TitleCaption = 'Dojde ke zm'#283'n'#283
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -15
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        TitleParentFont = False
      end>
    OnGetCellParams = rStringGridEd_tabGetCellParams
    RowHeights = (
      30
      30
      30
      30
      30)
  end
  object scGPRadioButton2: TscGPRadioButton
    Left = 12
    Top = 183
    Width = 30
    Height = 35
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 2
    TabStop = True
    OnClick = scGPRadioButton2Click
    Animation = False
    CanFocused = True
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 255
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
    ImageGlow = True
    Checked = False
    ScaleFrameWidth = True
    ShowFocusRect = True
  end
  object scGPRadioButton3: TscGPRadioButton
    Left = 12
    Top = 214
    Width = 30
    Height = 35
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 3
    TabStop = True
    OnClick = scGPRadioButton3Click
    Animation = False
    CanFocused = True
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 255
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
    ImageGlow = True
    Checked = False
    ScaleFrameWidth = True
    ShowFocusRect = True
  end
  object scGPPanel_header: TscGPPanel
    Left = 0
    Top = 0
    Width = 776
    Height = 34
    Align = alTop
    TabOrder = 4
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
    Caption = 'scGPPanel_header'
    TransparentBackground = True
    StorePaintBuffer = True
    ExplicitWidth = 755
    object Konec: TscGPGlyphButton
      AlignWithMargins = True
      Left = 726
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
      ExplicitLeft = 705
    end
    object scLabel_header: TscLabel
      Left = 0
      Top = 0
      Width = 726
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
      Caption = 'V'#253'b'#283'r priority '#250'prav'
      ExplicitWidth = 705
    end
  end
  object Button_save: TscGPButton
    Left = 222
    Top = 295
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 5
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
    Left = 360
    Top = 295
    Width = 100
    Height = 35
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuBar
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    Font.Quality = fqClearType
    ParentFont = False
    TabOrder = 6
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
  object scGPRadioButton4: TscGPRadioButton
    Left = 12
    Top = 246
    Width = 30
    Height = 35
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 7
    TabStop = True
    OnClick = scGPRadioButton3Click
    Animation = False
    CanFocused = True
    Spacing = 1
    Layout = blGlyphLeft
    ImageIndex = -1
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 255
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
    ImageGlow = True
    Checked = False
    ScaleFrameWidth = True
    ShowFocusRect = True
  end
end
