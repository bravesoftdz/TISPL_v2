object Form_help: TForm_help
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Form1'
  ClientHeight = 485
  ClientWidth = 880
  Color = clBtnFace
  Constraints.MinHeight = 300
  Constraints.MinWidth = 700
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  StyleElements = [seFont, seClient]
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BorderPanel: TscGPPanel
    Left = 0
    Top = 0
    Width = 880
    Height = 485
    Align = alClient
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
    FrameWidth = 1
    FillColor = clBtnFace
    FillColorAlpha = 150
    FillColor2 = clNone
    FrameColor = 10114859
    FrameColorAlpha = 255
    FrameRadius = 0
    ScaleFrameWidth = False
    ShowCaption = False
    CaptionGlowEffect.Enabled = False
    CaptionGlowEffect.Color = clBtnShadow
    CaptionGlowEffect.AlphaValue = 255
    CaptionGlowEffect.GlowSize = 7
    CaptionGlowEffect.Offset = 0
    CaptionGlowEffect.Intensive = True
    CaptionGlowEffect.StyleColors = True
    Color = clBtnFace
    Caption = 'BorderPanel'
    TransparentBackground = False
    StorePaintBuffer = False
    Sizeable = True
    WallpaperIndex = -1
    object ClientPageView: TscPageViewer
      Left = 1
      Top = 1
      Width = 878
      Height = 483
      Align = alClient
      TabOrder = 0
      PageIndex = 0
      Pages = <
        item
          Page = scPageViewerPage1
        end>
      ActivePage = scPageViewerPage1
      object scPageViewerPage1: TscPageViewerPage
        Left = 0
        Top = 0
        Width = 878
        Height = 483
        HorzScrollBar.Tracking = True
        VertScrollBar.Tracking = True
        Align = alTop
        AutoScroll = False
        BorderStyle = bsNone
        Color = 15132390
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
        WallpaperIndex = -1
        CustomBackgroundImageIndex = -1
        FullUpdate = False
        StorePaintBuffer = False
        BackgroundStyle = scsbsPanel
        object scGPPanel2: TscGPPanel
          Left = 0
          Top = 0
          Width = 878
          Height = 34
          Align = alTop
          TabOrder = 0
          BlurBackground = False
          BlurBackgroundAmount = 5
          CustomImageIndex = -1
          DragForm = True
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
          TransparentBackground = False
          StorePaintBuffer = True
          Sizeable = False
          WallpaperIndex = -1
          object CloseButton: TscGPGlyphButton
            AlignWithMargins = True
            Left = 833
            Top = 0
            Width = 45
            Height = 34
            Margins.Left = 0
            Margins.Top = 0
            Margins.Right = 0
            Margins.Bottom = 0
            Align = alRight
            TabOrder = 0
            OnClick = CloseButtonClick
            Animation = True
            Caption = 'CloseButton'
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
            GlyphOptions.Size = 11
            GlyphOptions.StyleColors = True
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
            Width = 653
            Height = 34
            Margins.Left = 0
            Margins.Top = 0
            Margins.Right = 0
            Margins.Bottom = 4
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
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
            ShowEllipsis = True
            UseFontColorToStyleColor = True
            Caption = 'Help'
          end
          object scGPPanel4: TscGPPanel
            Left = 653
            Top = 0
            Width = 180
            Height = 34
            Align = alRight
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
            Caption = 'scGPPanel4'
            TransparentBackground = False
            StorePaintBuffer = True
            Sizeable = False
            WallpaperIndex = -1
          end
        end
        object scGPPanel3: TscGPPanel
          Left = 0
          Top = 457
          Width = 878
          Height = 26
          Align = alBottom
          TabOrder = 1
          BlurBackground = False
          BlurBackgroundAmount = 5
          CustomImageIndex = -1
          DragForm = False
          DragTopForm = True
          FrameSides = [gppfsTop]
          FillGradientAngle = 90
          FillGradientBeginAlpha = 255
          FillGradientEndAlpha = 255
          FillGradientBeginColorOffset = 25
          FillGradientEndColorOffset = 25
          FrameWidth = 1
          FillColor = 15987699
          FillColorAlpha = 255
          FillColor2 = clNone
          FrameColor = 15987699
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
          Color = 15987699
          Caption = 'scGPPanel3'
          TransparentBackground = False
          StorePaintBuffer = True
          Sizeable = False
          WallpaperIndex = -1
          object scGPSizeBox1: TscGPSizeBox
            Left = 852
            Top = 1
            Width = 26
            Height = 25
            Cursor = crSizeNWSE
            Align = alRight
            TabOrder = 0
            GlyphColor = clBlack
            GlyphColorAlpha = 200
            GlyphThickness = 1
          end
        end
        object scGPToolPager: TscGPToolPager
          Left = 0
          Top = 34
          Width = 878
          Height = 423
          Align = alTop
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Roboto'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = scGPToolPagerClick
          Color = clWhite
          TabsBGFillColor = 15987699
          TabsBGFillColorAlpha = 255
          TransparentBackground = False
          FrameWidth = 0
          FrameScaleWidth = False
          FrameColor = 15987699
          FrameColorAlpha = 255
          BorderStyle = scgpbsLineTopBottom
          MouseWheelSupport = True
          ShowFocusRect = True
          ShowInActiveTab = False
          ShowCloseButtons = False
          TabsLeftOffset = 100
          TabsRightOffset = 80
          TabsTopOffset = 0
          TabGlowEffect.Enabled = False
          TabGlowEffect.Color = clHighlight
          TabGlowEffect.AlphaValue = 255
          TabGlowEffect.GlowSize = 7
          TabGlowEffect.Offset = 0
          TabGlowEffect.Intensive = True
          TabGlowEffect.StyleColors = True
          TabGlowEffect.HotColor = clNone
          TabGlowEffect.PressedColor = clNone
          TabGlowEffect.FocusedColor = clNone
          TabGlowEffect.PressedGlowSize = 7
          TabGlowEffect.PressedAlphaValue = 255
          TabGlowEffect.States = [scsFocused]
          TabOptions.NormalColor = clBtnShadow
          TabOptions.ActiveColor = 15987699
          TabOptions.MouseInColor = 15987699
          TabOptions.FocusedColor = 15987699
          TabOptions.NormalColorAlpha = 70
          TabOptions.ActiveColorAlpha = 255
          TabOptions.MouseInColorAlpha = 40
          TabOptions.FocusedColorAlpha = 255
          TabOptions.FrameNormalColor = 10114859
          TabOptions.FrameActiveColor = 10114859
          TabOptions.FrameMouseInColor = 10114859
          TabOptions.FrameFocusedColor = 10114859
          TabOptions.FrameWidth = 3
          TabOptions.FrameNormalColorAlpha = 0
          TabOptions.FrameActiveColorAlpha = 255
          TabOptions.FrameMouseInColorAlpha = 40
          TabOptions.FrameFocusedColorAlpha = 200
          TabOptions.FontNormalColor = 4934475
          TabOptions.FontActiveColor = clBtnText
          TabOptions.FontMouseInColor = clBtnText
          TabOptions.FontFocusedColor = 10114859
          TabOptions.TabStyle = gptsBottomLine
          TabOptions.ShapeFillGradientAngle = 90
          TabOptions.GradientColorOffset = 30
          TabOptions.ShapeCornerRadius = 0
          TabOptions.StyleColors = False
          TabSpacing = 10
          TabMargin = 10
          ScrollButtonWidth = 20
          TabHeight = 35
          Tabs = <
            item
              Page = scGPToolPagerPage_PL
              Visible = True
              Enabled = True
              ImageIndex = -1
              Caption = 'Parametry linky'
              CustomOptions.NormalColor = clBtnShadow
              CustomOptions.ActiveColor = clBtnFace
              CustomOptions.MouseInColor = clBtnShadow
              CustomOptions.FocusedColor = clBtnFace
              CustomOptions.NormalColorAlpha = 70
              CustomOptions.ActiveColorAlpha = 230
              CustomOptions.MouseInColorAlpha = 40
              CustomOptions.FocusedColorAlpha = 255
              CustomOptions.FrameNormalColor = clBtnText
              CustomOptions.FrameActiveColor = clBtnText
              CustomOptions.FrameMouseInColor = clBtnText
              CustomOptions.FrameFocusedColor = clBtnText
              CustomOptions.FrameWidth = 2
              CustomOptions.FrameNormalColorAlpha = 50
              CustomOptions.FrameActiveColorAlpha = 80
              CustomOptions.FrameMouseInColorAlpha = 60
              CustomOptions.FrameFocusedColorAlpha = 80
              CustomOptions.FontNormalColor = clBtnText
              CustomOptions.FontActiveColor = clBtnText
              CustomOptions.FontMouseInColor = clBtnText
              CustomOptions.FontFocusedColor = clBtnText
              CustomOptions.TabStyle = gptsShape
              CustomOptions.ShapeFillGradientAngle = 90
              CustomOptions.GradientColorOffset = 30
              CustomOptions.ShapeCornerRadius = 5
              CustomOptions.StyleColors = True
              CustomGlowEffect.Enabled = False
              CustomGlowEffect.Color = clHighlight
              CustomGlowEffect.AlphaValue = 255
              CustomGlowEffect.GlowSize = 7
              CustomGlowEffect.Offset = 0
              CustomGlowEffect.Intensive = True
              CustomGlowEffect.StyleColors = True
              CustomGlowEffect.HotColor = clNone
              CustomGlowEffect.PressedColor = clNone
              CustomGlowEffect.FocusedColor = clNone
              CustomGlowEffect.PressedGlowSize = 7
              CustomGlowEffect.PressedAlphaValue = 255
              CustomGlowEffect.States = [scsFocused]
              CustomFrameColor = clNone
              CustomFrameColorAlpha = 255
              ShowCloseButton = True
              UseCustomOptions = False
            end
            item
              Page = scGPToolPagerPage2
              Visible = True
              Enabled = True
              ImageIndex = -1
              Caption = 'Layout'
              CustomOptions.NormalColor = clBtnShadow
              CustomOptions.ActiveColor = 13303780
              CustomOptions.MouseInColor = 13303780
              CustomOptions.FocusedColor = 13303780
              CustomOptions.NormalColorAlpha = 0
              CustomOptions.ActiveColorAlpha = 255
              CustomOptions.MouseInColorAlpha = 40
              CustomOptions.FocusedColorAlpha = 255
              CustomOptions.FrameNormalColor = 10114859
              CustomOptions.FrameActiveColor = 10114859
              CustomOptions.FrameMouseInColor = 10114859
              CustomOptions.FrameFocusedColor = 10114859
              CustomOptions.FrameWidth = 1
              CustomOptions.FrameNormalColorAlpha = 40
              CustomOptions.FrameActiveColorAlpha = 40
              CustomOptions.FrameMouseInColorAlpha = 40
              CustomOptions.FrameFocusedColorAlpha = 40
              CustomOptions.FontNormalColor = clWhite
              CustomOptions.FontActiveColor = clBlack
              CustomOptions.FontMouseInColor = clWhite
              CustomOptions.FontFocusedColor = clBlack
              CustomOptions.TabStyle = gptsShape
              CustomOptions.ShapeFillGradientAngle = 90
              CustomOptions.GradientColorOffset = 30
              CustomOptions.ShapeCornerRadius = 5
              CustomOptions.StyleColors = True
              CustomGlowEffect.Enabled = False
              CustomGlowEffect.Color = clHighlight
              CustomGlowEffect.AlphaValue = 255
              CustomGlowEffect.GlowSize = 7
              CustomGlowEffect.Offset = 0
              CustomGlowEffect.Intensive = True
              CustomGlowEffect.StyleColors = True
              CustomGlowEffect.HotColor = clNone
              CustomGlowEffect.PressedColor = clNone
              CustomGlowEffect.FocusedColor = clNone
              CustomGlowEffect.PressedGlowSize = 7
              CustomGlowEffect.PressedAlphaValue = 255
              CustomGlowEffect.States = [scsFocused]
              CustomFrameColor = clNone
              CustomFrameColorAlpha = 255
              ShowCloseButton = True
              UseCustomOptions = False
            end
            item
              Page = scGPToolPagerPage3
              Visible = True
              Enabled = True
              ImageIndex = -1
              Caption = 'Geometrie'
              CustomOptions.NormalColor = clBtnShadow
              CustomOptions.ActiveColor = clBtnFace
              CustomOptions.MouseInColor = clBtnShadow
              CustomOptions.FocusedColor = clBtnFace
              CustomOptions.NormalColorAlpha = 70
              CustomOptions.ActiveColorAlpha = 230
              CustomOptions.MouseInColorAlpha = 40
              CustomOptions.FocusedColorAlpha = 255
              CustomOptions.FrameNormalColor = clBtnText
              CustomOptions.FrameActiveColor = clBtnText
              CustomOptions.FrameMouseInColor = clBtnText
              CustomOptions.FrameFocusedColor = clBtnText
              CustomOptions.FrameWidth = 2
              CustomOptions.FrameNormalColorAlpha = 50
              CustomOptions.FrameActiveColorAlpha = 80
              CustomOptions.FrameMouseInColorAlpha = 60
              CustomOptions.FrameFocusedColorAlpha = 80
              CustomOptions.FontNormalColor = clBtnText
              CustomOptions.FontActiveColor = clBtnText
              CustomOptions.FontMouseInColor = clBtnText
              CustomOptions.FontFocusedColor = clBtnText
              CustomOptions.TabStyle = gptsShape
              CustomOptions.ShapeFillGradientAngle = 90
              CustomOptions.GradientColorOffset = 30
              CustomOptions.ShapeCornerRadius = 5
              CustomOptions.StyleColors = True
              CustomGlowEffect.Enabled = False
              CustomGlowEffect.Color = clHighlight
              CustomGlowEffect.AlphaValue = 255
              CustomGlowEffect.GlowSize = 7
              CustomGlowEffect.Offset = 0
              CustomGlowEffect.Intensive = True
              CustomGlowEffect.StyleColors = True
              CustomGlowEffect.HotColor = clNone
              CustomGlowEffect.PressedColor = clNone
              CustomGlowEffect.FocusedColor = clNone
              CustomGlowEffect.PressedGlowSize = 7
              CustomGlowEffect.PressedAlphaValue = 255
              CustomGlowEffect.States = [scsFocused]
              CustomFrameColor = clNone
              CustomFrameColorAlpha = 255
              ShowCloseButton = True
              UseCustomOptions = False
            end
            item
              Page = scGPToolPagerPage4
              Visible = True
              Enabled = True
              ImageIndex = -1
              Caption = 'Q&A'
              CustomOptions.NormalColor = clBtnShadow
              CustomOptions.ActiveColor = clBtnFace
              CustomOptions.MouseInColor = clBtnShadow
              CustomOptions.FocusedColor = clBtnFace
              CustomOptions.NormalColorAlpha = 70
              CustomOptions.ActiveColorAlpha = 230
              CustomOptions.MouseInColorAlpha = 40
              CustomOptions.FocusedColorAlpha = 255
              CustomOptions.FrameNormalColor = clBtnText
              CustomOptions.FrameActiveColor = clBtnText
              CustomOptions.FrameMouseInColor = clBtnText
              CustomOptions.FrameFocusedColor = clBtnText
              CustomOptions.FrameWidth = 2
              CustomOptions.FrameNormalColorAlpha = 50
              CustomOptions.FrameActiveColorAlpha = 80
              CustomOptions.FrameMouseInColorAlpha = 60
              CustomOptions.FrameFocusedColorAlpha = 80
              CustomOptions.FontNormalColor = clBtnText
              CustomOptions.FontActiveColor = clBtnText
              CustomOptions.FontMouseInColor = clBtnText
              CustomOptions.FontFocusedColor = clBtnText
              CustomOptions.TabStyle = gptsShape
              CustomOptions.ShapeFillGradientAngle = 90
              CustomOptions.GradientColorOffset = 30
              CustomOptions.ShapeCornerRadius = 5
              CustomOptions.StyleColors = True
              CustomGlowEffect.Enabled = False
              CustomGlowEffect.Color = clHighlight
              CustomGlowEffect.AlphaValue = 255
              CustomGlowEffect.GlowSize = 7
              CustomGlowEffect.Offset = 0
              CustomGlowEffect.Intensive = True
              CustomGlowEffect.StyleColors = True
              CustomGlowEffect.HotColor = clNone
              CustomGlowEffect.PressedColor = clNone
              CustomGlowEffect.FocusedColor = clNone
              CustomGlowEffect.PressedGlowSize = 7
              CustomGlowEffect.PressedAlphaValue = 255
              CustomGlowEffect.States = [scsFocused]
              CustomFrameColor = clNone
              CustomFrameColorAlpha = 255
              ShowCloseButton = True
              UseCustomOptions = False
            end
            item
              Page = scGPToolPagerPage_about
              Visible = True
              Enabled = True
              ImageIndex = -1
              Caption = 'About'
              CustomOptions.NormalColor = clBtnShadow
              CustomOptions.ActiveColor = clBtnFace
              CustomOptions.MouseInColor = clBtnShadow
              CustomOptions.FocusedColor = clBtnFace
              CustomOptions.NormalColorAlpha = 70
              CustomOptions.ActiveColorAlpha = 230
              CustomOptions.MouseInColorAlpha = 40
              CustomOptions.FocusedColorAlpha = 255
              CustomOptions.FrameNormalColor = clBtnText
              CustomOptions.FrameActiveColor = clBtnText
              CustomOptions.FrameMouseInColor = clBtnText
              CustomOptions.FrameFocusedColor = clBtnText
              CustomOptions.FrameWidth = 2
              CustomOptions.FrameNormalColorAlpha = 50
              CustomOptions.FrameActiveColorAlpha = 80
              CustomOptions.FrameMouseInColorAlpha = 60
              CustomOptions.FrameFocusedColorAlpha = 80
              CustomOptions.FontNormalColor = clBtnText
              CustomOptions.FontActiveColor = clBtnText
              CustomOptions.FontMouseInColor = clBtnText
              CustomOptions.FontFocusedColor = clBtnText
              CustomOptions.TabStyle = gptsShape
              CustomOptions.ShapeFillGradientAngle = 90
              CustomOptions.GradientColorOffset = 30
              CustomOptions.ShapeCornerRadius = 5
              CustomOptions.StyleColors = True
              CustomGlowEffect.Enabled = False
              CustomGlowEffect.Color = clHighlight
              CustomGlowEffect.AlphaValue = 255
              CustomGlowEffect.GlowSize = 7
              CustomGlowEffect.Offset = 0
              CustomGlowEffect.Intensive = True
              CustomGlowEffect.StyleColors = True
              CustomGlowEffect.HotColor = clNone
              CustomGlowEffect.PressedColor = clNone
              CustomGlowEffect.FocusedColor = clNone
              CustomGlowEffect.PressedGlowSize = 7
              CustomGlowEffect.PressedAlphaValue = 255
              CustomGlowEffect.States = [scsFocused]
              CustomFrameColor = clNone
              CustomFrameColorAlpha = 255
              ShowCloseButton = True
              UseCustomOptions = False
            end>
          TabIndex = 4
          ActivePage = scGPToolPagerPage_about
          FreeOnClose = False
          StorePaintBuffer = False
          object scGPToolPagerPage3: TscGPToolPagerPage
            Left = 0
            Top = 35
            Width = 878
            Height = 388
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            CanScroll = True
            FillGradientAngle = 90
            FillGradientBeginAlpha = 255
            FillGradientEndAlpha = 255
            FillGradientBeginColorOffset = 25
            FillGradientEndColorOffset = 25
            FillColor = clNone
            FillColorAlpha = 255
            FillColor2 = clNone
            Color = clWhite
            BGStyleKind = scgspbPanel
            ScrollButtonWidth = 17
            CustomImageIndex = -1
            WallpaperIndex = -1
            HotScroll = False
            AutoSize = False
            ScrollType = scstHorizontal
            ScrollOffset = 0
            ScrollTimerInterval = 50
            ScrollButtonArrowWidth = 1
            TouchScroll = False
            StorePaintBuffer = False
            object scLabel28: TscLabel
              Left = 14
              Top = 14
              Width = 86
              Height = 27
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              StyleElements = [seClient, seBorder]
              DragForm = False
              DragTopForm = True
              GlowEffect.Enabled = True
              GlowEffect.Color = clWhite
              GlowEffect.AlphaValue = 200
              GlowEffect.GlowSize = 7
              GlowEffect.Offset = 0
              GlowEffect.Intensive = True
              GlowEffect.StyleColors = True
              AutoSize = True
              UseFontColorToStyleColor = False
              Caption = 'Geometrie text'
            end
          end
          object scGPToolPagerPage2: TscGPToolPagerPage
            Left = 0
            Top = 35
            Width = 878
            Height = 388
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            CanScroll = True
            FillGradientAngle = 90
            FillGradientBeginAlpha = 255
            FillGradientEndAlpha = 255
            FillGradientBeginColorOffset = 25
            FillGradientEndColorOffset = 25
            FillColor = clNone
            FillColorAlpha = 255
            FillColor2 = clNone
            Color = clWhite
            BGStyleKind = scgspbPanel
            ScrollButtonWidth = 17
            CustomImageIndex = -1
            WallpaperIndex = -1
            HotScroll = False
            AutoSize = False
            ScrollType = scstHorizontal
            ScrollOffset = 0
            ScrollTimerInterval = 50
            ScrollButtonArrowWidth = 1
            TouchScroll = False
            StorePaintBuffer = False
            object scLabel2: TscLabel
              Left = 22
              Top = 22
              Width = 84
              Height = 13
              TabOrder = 0
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
              Caption = 'Layout - info text'
            end
          end
          object scGPToolPagerPage4: TscGPToolPagerPage
            Left = 0
            Top = 35
            Width = 878
            Height = 388
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Visible = False
            CanScroll = True
            FillGradientAngle = 90
            FillGradientBeginAlpha = 255
            FillGradientEndAlpha = 255
            FillGradientBeginColorOffset = 25
            FillGradientEndColorOffset = 25
            FillColor = clNone
            FillColorAlpha = 255
            FillColor2 = clNone
            Color = clBtnFace
            BGStyleKind = scgspbTransparent
            ScrollButtonWidth = 17
            CustomImageIndex = -1
            WallpaperIndex = -1
            HotScroll = False
            AutoSize = False
            ScrollType = scstHorizontal
            ScrollOffset = 0
            ScrollTimerInterval = 50
            ScrollButtonArrowWidth = 2
            TouchScroll = False
            StorePaintBuffer = False
            object scLabel4: TscLabel
              Left = 22
              Top = 18
              Width = 104
              Height = 27
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              StyleElements = [seClient, seBorder]
              DragForm = False
              DragTopForm = True
              GlowEffect.Enabled = True
              GlowEffect.Color = clWhite
              GlowEffect.AlphaValue = 200
              GlowEffect.GlowSize = 7
              GlowEffect.Offset = 0
              GlowEffect.Intensive = True
              GlowEffect.StyleColors = True
              AutoSize = True
              UseFontColorToStyleColor = False
              Caption = 'Ot'#225'zky a odpov'#283'di'
            end
          end
          object scGPToolPagerPage_PL: TscGPToolPagerPage
            Left = 0
            Top = 35
            Width = 878
            Height = 388
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            CanScroll = True
            FillGradientAngle = 90
            FillGradientBeginAlpha = 255
            FillGradientEndAlpha = 255
            FillGradientBeginColorOffset = 25
            FillGradientEndColorOffset = 25
            FillColor = clNone
            FillColorAlpha = 255
            FillColor2 = clNone
            Color = clWhite
            BGStyleKind = scgspbPanel
            ScrollButtonWidth = 17
            CustomImageIndex = -1
            WallpaperIndex = -1
            HotScroll = False
            AutoSize = False
            ScrollType = scstHorizontal
            ScrollOffset = 0
            ScrollTimerInterval = 50
            ScrollButtonArrowWidth = 1
            TouchScroll = False
            StorePaintBuffer = True
            object scLabel_PL: TscLabel
              Left = 14
              Top = 22
              Width = 104
              Height = 13
              Align = alCustom
              TabOrder = 0
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
              Caption = 'Parametry linky - text'
            end
          end
          object scGPToolPagerPage_about: TscGPToolPagerPage
            Left = 0
            Top = 35
            Width = 878
            Height = 388
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            CanScroll = True
            FillGradientAngle = 90
            FillGradientBeginAlpha = 255
            FillGradientEndAlpha = 255
            FillGradientBeginColorOffset = 25
            FillGradientEndColorOffset = 25
            FillColor = clNone
            FillColorAlpha = 255
            FillColor2 = clNone
            Color = clWhite
            BGStyleKind = scgspbPanel
            ScrollButtonWidth = 17
            CustomImageIndex = -1
            WallpaperIndex = -1
            HotScroll = False
            AutoSize = False
            ScrollType = scstHorizontal
            ScrollOffset = 0
            ScrollTimerInterval = 50
            ScrollButtonArrowWidth = 2
            TouchScroll = False
            StorePaintBuffer = False
            object scHTMLLabel_about: TscHTMLLabel
              Left = 14
              Top = 16
              Width = 131
              Height = 18
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -15
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              AutoSizeHeight = True
              AutoSizeWidth = True
              AutoExecuteURL = True
              Caption = 'scHTMLLabel_about'
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
        end
      end
    end
  end
  object scStyledForm1: TscStyledForm
    DWMClientShadow = True
    DWMClientShadowHitTest = True
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
    Left = 640
    Top = 432
  end
  object scStyleManager1: TscStyleManager
    ArrowsType = scsatModern
    MenuHookEnabled = True
    MenuAlphaBlendValue = 255
    MenuWallpaperIndex = -1
    MenuBackgroundIndex = -1
    MenuBackgroundOverContentIndex = -1
    MenuHeadersSupport = True
    MenuSelectionStyle = scmssStyled
    ScaleStyles = True
    ScaleThemes = False
    ScaleResources = True
    ScaleFormBorder = True
    Left = 712
    Top = 424
  end
end
