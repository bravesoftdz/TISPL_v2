object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'ELTEP - TISPL - Timing Inspection of Single Purpose Lines'
  ClientHeight = 726
  ClientWidth = 1026
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  Position = poScreenCenter
  StyleElements = [seFont, seClient]
  OnActivate = FormActivate
  OnCloseQuery = FormCloseQuery
  OnKeyDown = FormKeyDown
  OnKeyUp = FormKeyUp
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  OnPaint = FormPaint
  OnResize = FormResize
  OnShortCut = FormShortCut
  OnShow = FormShow
  DesignSize = (
    1026
    726)
  PixelsPerInch = 96
  TextHeight = 13
  object Label_wip: TLabel
    Left = 213
    Top = 159
    Width = 78
    Height = 19
    Caption = 'Label_wip'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label_zamerovac: TLabel
    Left = 189
    Top = 123
    Width = 32
    Height = 28
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Caption = 'voz'#237'k:'#13'min:'
    Color = clWhite
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clGray
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    Visible = False
  end
  object RzToolbar1: TRzToolbar
    Left = 539
    Top = 597
    Width = 437
    Height = 29
    Align = alNone
    Images = ImageList1
    BorderInner = fsNone
    BorderOuter = fsGroove
    BorderSides = [sdTop]
    BorderWidth = 0
    TabOrder = 0
    Visible = False
    VisualStyle = vsGradient
    ToolbarControls = (
      RzToolButton1
      RzToolButton2
      RzToolButton3
      RzSpacer1
      RzToolButton4
      RzToolButton5
      RzSpacer2
      RzToolButton6
      RzToolButton7
      RzToolButton8
      RzToolButton9
      RzToolButton10
      RzToolButton11
      RzToolButton12
      LabelRoletka)
    object RzToolButton1: TRzToolButton
      Left = 4
      Top = 2
      Hint = 'Zalo'#382#237' nov'#253' soubor'
      ImageIndex = 0
      Images = ImageList1
      ParentShowHint = False
      ShowHint = True
      OnClick = NovySouborClick
    end
    object RzToolButton2: TRzToolButton
      Left = 29
      Top = 2
      Hint = 'Otev'#345'e existuj'#237'c'#237' soubor'
      ImageIndex = 1
      Images = ImageList1
      ParentShowHint = False
      ShowHint = True
      OnClick = OtevritClick
    end
    object RzToolButton3: TRzToolButton
      Left = 54
      Top = 2
      Hint = 'Ulo'#382#237' aktu'#225'ln'#237' soubor'
      ImageIndex = 2
      Images = ImageList1
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      OnClick = UlozitClick
    end
    object RzSpacer1: TRzSpacer
      Left = 79
      Top = 2
    end
    object RzToolButton4: TRzToolButton
      Left = 87
      Top = 2
      Hint = 'Zp'#283't'
      ImageIndex = 3
      Enabled = False
      ParentShowHint = False
      ShowHint = True
    end
    object RzToolButton5: TRzToolButton
      Left = 112
      Top = 2
      Hint = 'Vp'#345'ed'
      ImageIndex = 4
      Enabled = False
    end
    object RzToolButton6: TRzToolButton
      Left = 145
      Top = 2
      Hint = 'Posouvat obraz'
      ImageIndex = 6
      ParentShowHint = False
      ShowHint = True
      OnClick = Posouvat2Click
    end
    object RzToolButton7: TRzToolButton
      Left = 170
      Top = 2
      Hint = 'Posunout obraz'
      ImageIndex = 5
      ParentShowHint = False
      ShowHint = True
      OnClick = Posunout3Click
    end
    object RzToolButton8: TRzToolButton
      Left = 195
      Top = 2
      Hint = 'P'#345'ibl'#237#382'it obraz'
      ImageIndex = 7
      ParentShowHint = False
      ShowHint = True
      OnClick = RzToolButton8Click
    end
    object RzToolButton9: TRzToolButton
      Left = 220
      Top = 2
      Hint = 'Odd'#225'lit obraz'
      ImageIndex = 8
      ParentShowHint = False
      ShowHint = True
      OnClick = RzToolButton9Click
    end
    object RzSpacer2: TRzSpacer
      Left = 137
      Top = 2
    end
    object RzToolButton10: TRzToolButton
      Left = 245
      Top = 2
      Hint = 'P'#345'ibl'#237#382'it obraz dle v'#253'b'#283'ru oknem'
      ImageIndex = 9
      ParentShowHint = False
      ShowHint = True
      OnClick = Vybratoknem2Click
    end
    object RzToolButton11: TRzToolButton
      Left = 270
      Top = 2
      Hint = 'Zobrazit v'#353'echny objekty'
      ImageIndex = 10
      ParentShowHint = False
      ShowHint = True
      OnClick = RzToolButton11Click
    end
    object RzToolButton12: TRzToolButton
      Left = 295
      Top = 2
      Hint = 'P'#345'edchoz'#237' pohled'
      ImageIndex = 11
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      OnClick = Predchozipohled1Click
    end
    object LabelRoletka: TLabel
      Left = 320
      Top = 8
      Width = 61
      Height = 13
      Caption = 'LabelRoletka'
      Visible = False
    end
  end
  object Button1: TButton
    Left = 185
    Top = 537
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 1
    Visible = False
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 0
    Top = 674
    Width = 1026
    Height = 24
    Align = alBottom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    Lines.Strings = (
      '')
    ParentFont = False
    TabOrder = 2
    Visible = False
  end
  object Button3: TButton
    Left = 183
    Top = 184
    Width = 85
    Height = 25
    Caption = 'Nav'#283'sit voz'#237'k (0)'
    TabOrder = 3
    Visible = False
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 183
    Top = 215
    Width = 85
    Height = 25
    Caption = 'Pauza simulace'
    TabOrder = 4
    Visible = False
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 183
    Top = 277
    Width = 85
    Height = 25
    Caption = 'stopka'
    TabOrder = 5
    Visible = False
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 183
    Top = 308
    Width = 85
    Height = 25
    Caption = #269'ekat na palec'
    TabOrder = 6
    Visible = False
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 183
    Top = 246
    Width = 85
    Height = 25
    Caption = 'Krok simulace'
    TabOrder = 7
    Visible = False
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 183
    Top = 339
    Width = 85
    Height = 25
    Caption = 'D'#233'lka dopravn'#237'ku'
    TabOrder = 8
    Visible = False
    OnClick = Button8Click
  end
  object Button9: TButton
    Left = 174
    Top = 469
    Width = 129
    Height = 25
    Caption = 'vypsat spoj'#225'k VOZIKY'
    TabOrder = 9
    Visible = False
    OnClick = Button9Click
  end
  object Button10: TButton
    Left = 175
    Top = 425
    Width = 110
    Height = 25
    Caption = 'zkr'#225'tit linky dle palc'#367
    TabOrder = 10
    Visible = False
    OnClick = Button10Click
  end
  object Chart1: TChart
    Left = 569
    Top = 265
    Width = 76
    Height = 58
    Title.Text.Strings = (
      'TChart')
    Panning.MouseWheel = pmwNone
    View3D = False
    TabOrder = 11
    OnClick = Chart1Click
    DefaultCanvas = 'TGDIPlusCanvas'
    PrintMargins = (
      15
      12
      15
      12)
    ColorPaletteIndex = 19
    object Series1: TGanttSeries
      Marks.Children = <
        item
          Shape.ShapeStyle = fosRectangle
          Shape.Style = smsValue
        end>
      Marks.ChildLayout = slLeftRight
      ValueFormat = '##0.## %'
      ClickableLine = False
      Pointer.HorizSize = 18
      Pointer.InflateMargins = True
      Pointer.Pen.Visible = False
      Pointer.Style = psRectangle
      Pointer.VertSize = 18
      XValues.DateTime = False
      XValues.Name = 'Start'
      XValues.Order = loNone
      YValues.Name = 'Y'
      YValues.Order = loNone
      Callout.Style = psRightTriangle
      Callout.Arrow.Visible = False
      StartValues.DateTime = False
      StartValues.Name = 'Start'
      StartValues.Order = loNone
      EndValues.Name = 'End'
      EndValues.Order = loNone
      NextTask.Name = 'NextTask'
      NextTask.Order = loNone
    end
  end
  object Chart2: TChart
    Left = 651
    Top = 265
    Width = 76
    Height = 58
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Panning.MouseWheel = pmwNone
    View3D = False
    TabOrder = 12
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series2: TBarSeries
      BarPen.Visible = False
      Marks.Frame.Visible = False
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Arrow.Visible = False
      Marks.Callout.Arrow.Visible = False
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = 5
      Marks.Callout.Length = 0
      MultiBar = mbStacked
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
    object Series3: TBarSeries
      BarPen.Visible = False
      Marks.Children = <
        item
          Shape.ShapeStyle = fosRectangle
          Shape.Visible = False
          Shape.Style = smsValue
        end>
      Marks.Frame.Visible = False
      Marks.Shadow.Visible = False
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = 5
      Marks.Callout.Length = 0
      MultiBar = mbStacked
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
      Data = {0000000000}
    end
  end
  object Chart3: TChart
    Left = 743
    Top = 265
    Width = 76
    Height = 58
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Panning.MouseWheel = pmwNone
    View3D = False
    TabOrder = 13
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series5: TBarSeries
      BarPen.Visible = False
      Marks.Frame.Visible = False
      Marks.Transparency = 30
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Arrow.Visible = False
      Marks.Callout.Pen.Visible = False
      Marks.Callout.Arrow.Visible = False
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = 5
      Marks.Callout.Length = 0
      Marks.Symbol.Frame.Visible = False
      Marks.Symbol.Pen.Visible = False
      MultiBar = mbStacked
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
    object Series6: TBarSeries
      BarBrush.Gradient.EndColor = 3513587
      BarPen.Visible = False
      Marks.Frame.Visible = False
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = 5
      Marks.Callout.Length = 0
      Gradient.EndColor = 3513587
      MultiBar = mbStacked
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
  object Chart4: TChart
    Left = 825
    Top = 267
    Width = 73
    Height = 53
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Panning.MouseWheel = pmwNone
    View3D = False
    View3DOptions.Elevation = 315
    View3DOptions.Orthogonal = False
    View3DOptions.Perspective = 0
    View3DOptions.Rotation = 360
    TabOrder = 14
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series7: TPieSeries
      Marks.Font.Color = clWhite
      Marks.Frame.Visible = False
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Arrow.Visible = False
      Marks.Callout.Arrow.Visible = False
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = -35
      Marks.Callout.Length = 0
      XValues.Order = loAscending
      YValues.Name = 'Pie'
      YValues.Order = loNone
      Frame.InnerBrush.BackColor = clRed
      Frame.InnerBrush.Gradient.EndColor = clGray
      Frame.InnerBrush.Gradient.MidColor = clWhite
      Frame.InnerBrush.Gradient.StartColor = 4210752
      Frame.InnerBrush.Gradient.Visible = True
      Frame.MiddleBrush.BackColor = clYellow
      Frame.MiddleBrush.Gradient.EndColor = 8553090
      Frame.MiddleBrush.Gradient.MidColor = clWhite
      Frame.MiddleBrush.Gradient.StartColor = clGray
      Frame.MiddleBrush.Gradient.Visible = True
      Frame.OuterBrush.BackColor = clGreen
      Frame.OuterBrush.Gradient.EndColor = 4210752
      Frame.OuterBrush.Gradient.MidColor = clWhite
      Frame.OuterBrush.Gradient.StartColor = clSilver
      Frame.OuterBrush.Gradient.Visible = True
      Frame.Width = 4
      OtherSlice.Legend.Visible = False
      PiePen.Visible = False
    end
  end
  object Button2: TButton
    Left = 852
    Top = 1
    Width = 75
    Height = 25
    Caption = 'EXPO'
    TabOrder = 15
    Visible = False
    OnClick = Button2Click
  end
  object ButtonPLAY: TButton
    Left = 795
    Top = 1
    Width = 43
    Height = 21
    Caption = 'PLAY'
    TabOrder = 16
    Visible = False
    OnClick = ButtonPLAYClick
  end
  object Chart6: TChart
    Left = 904
    Top = 265
    Width = 84
    Height = 58
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Panning.MouseWheel = pmwNone
    View3D = False
    TabOrder = 17
    DefaultCanvas = 'TGDIPlusCanvas'
    PrintMargins = (
      15
      13
      15
      13)
    ColorPaletteIndex = 13
    object Series9: TBarSeries
      BarPen.Visible = False
      Marks.Frame.Visible = False
      Marks.Shadow.Visible = False
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Arrow.Visible = False
      Marks.Callout.Arrow.Visible = False
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = 5
      Marks.Callout.Length = 0
      MultiBar = mbStacked
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
    object Series10: TBarSeries
      BarPen.Visible = False
      Marks.Frame.Visible = False
      Marks.Transparent = True
      Marks.Style = smsValue
      Marks.Callout.ArrowHeadSize = 0
      Marks.Callout.Distance = 5
      Marks.Callout.Length = 0
      MultiBar = mbStacked
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
  object CheckBoxVytizenost: TCheckBox
    Left = 285
    Top = 228
    Width = 152
    Height = 20
    Caption = 'zobrazit vyt'#237#382'enost objekt'#367
    Color = clBtnFace
    ParentColor = False
    TabOrder = 18
    Visible = False
    OnClick = CheckBoxVytizenostClick
  end
  object CheckBoxAnimovatSG: TCheckBox
    Left = 285
    Top = 249
    Width = 145
    Height = 20
    Caption = 'animovat i jednokapacitn'#237
    Checked = True
    Color = clBtnFace
    ParentColor = False
    State = cbChecked
    TabOrder = 19
    Visible = False
    OnClick = CheckBoxVytizenostClick
  end
  object CheckBoxPALCE: TCheckBox
    Left = 285
    Top = 186
    Width = 163
    Height = 20
    Caption = 'zohlednit rozte'#269' palc'#367' '#345'et'#283'zu'
    Color = clBtnFace
    ParentColor = False
    TabOrder = 20
    Visible = False
    OnClick = CheckBoxPALCEClick
  end
  object CheckBoxVymena_barev: TCheckBox
    Left = 285
    Top = 207
    Width = 211
    Height = 20
    Caption = 'zohlednit '#269'i'#353't'#283'n'#237' pistole a v'#253'm'#283'nu barev'
    Color = clBtnFace
    Ctl3D = False
    ParentColor = False
    ParentCtl3D = False
    TabOrder = 21
    Visible = False
    OnClick = CheckBoxVymena_barevClick
  end
  object ComboBoxDOmin: TComboBox
    Left = 299
    Top = 157
    Width = 40
    Height = 21
    TabOrder = 22
    Text = 'Do'
    Visible = False
    OnChange = ComboBoxDOminChange
    Items.Strings = (
      ''
      '')
  end
  object ComboBoxODmin: TComboBox
    Left = 253
    Top = 157
    Width = 40
    Height = 21
    TabOrder = 23
    Text = 'Od'
    Visible = False
    OnChange = ComboBoxODminChange
    Items.Strings = (
      '')
  end
  object Button11: TButton
    Left = 618
    Top = 508
    Width = 75
    Height = 25
    Caption = 'Button11'
    TabOrder = 25
    Visible = False
    OnClick = Button11Click
  end
  object CheckBox_pouzit_zadane_kapacity: TCheckBox
    Left = 285
    Top = 268
    Width = 171
    Height = 20
    Caption = 'pou'#382#237't zadan'#233' kapacity objekt'#367
    Color = clBtnFace
    ParentColor = False
    TabOrder = 24
    Visible = False
    OnClick = CheckBox_pouzit_zadane_kapacityClick
  end
  object rComboBoxKrok: TrComboBoxEx
    Left = 345
    Top = 159
    Width = 75
    Height = 21
    TabOrder = 26
    Text = 'Krok [min]'
    OnChange = rComboBoxKrokChange
    Items.Strings = (
      '0.1'
      '0.5'
      '1'
      '2'
      '5'
      '10'
      '20'
      '50'
      '100')
  end
  object ComboBoxCekani: TrComboBoxEx
    Left = 285
    Top = 294
    Width = 145
    Height = 21
    TabOrder = 27
    Text = #268'as '#269'ek'#225'n'#237' na palec'
    Visible = False
    Items.Strings = (
      'nulov'#253
      'st'#345'edn'#237
      'n'#225'hodn'#253
      'maxim'#225'ln'#237)
  end
  object scGPPanel2: TscGPPanel
    Left = 0
    Top = 0
    Width = 1026
    Height = 34
    Align = alTop
    TabOrder = 28
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
      Left = 976
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
    object MinButton: TscGPGlyphButton
      AlignWithMargins = True
      Left = 876
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 1
      OnClick = MinButtonClick
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
    end
    object editacelinky1: TscGPButton
      Left = 404
      Top = 0
      Width = 78
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = editacelinky1Click
      Animation = True
      Caption = 'Sch'#233'ma'
      CanFocused = False
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphLeft
      ImageIndex = -1
      ImageMargin = 0
      TransparentBackground = True
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBtnFace
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = 16250613
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 255
      Options.FocusedColorAlpha = 30
      Options.DisabledColorAlpha = 0
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
      Options.FrameWidth = 1
      Options.FrameNormalColorAlpha = 255
      Options.FrameHotColorAlpha = 255
      Options.FramePressedColorAlpha = 255
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 255
      Options.FontNormalColor = clWhite
      Options.FontHotColor = clWhite
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clWhite
      Options.FontDisabledColor = clGray
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
      GlowEffect.Color = clHighlight
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      GlowEffect.HotColor = clNone
      GlowEffect.PressedColor = clBtnFace
      GlowEffect.FocusedColor = clNone
      GlowEffect.PressedGlowSize = 7
      GlowEffect.PressedAlphaValue = 255
      GlowEffect.States = [scsHot, scsPressed, scsFocused]
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = True
      GroupIndex = 1
      AllowAllUp = False
    end
    object scLabel1: TscLabel
      Left = 242
      Top = 0
      Width = 162
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnDblClick = scLabel1DblClick
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
      Caption = 'TISPL'
    end
    object PopupMenuButton: TscGPButton
      AlignWithMargins = True
      Left = 0
      Top = 0
      Width = 81
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = PopupMenuButtonClick
      Animation = True
      Caption = 'Menu'
      CanFocused = False
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphTop
      ImageIndex = -1
      ImageMargin = 0
      TransparentBackground = True
      Options.NormalColor = 33023
      Options.HotColor = 1871359
      Options.PressedColor = 33023
      Options.FocusedColor = 33023
      Options.DisabledColor = 16250613
      Options.NormalColorAlpha = 220
      Options.HotColorAlpha = 255
      Options.PressedColorAlpha = 200
      Options.FocusedColorAlpha = 30
      Options.DisabledColorAlpha = 0
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
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
      Options.FontDisabledColor = 33023
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
      ImageGlow = True
      DropDownMenu = PopupMenu2
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
    object casovosa1: TscGPButton
      Left = 482
      Top = 0
      Width = 78
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = casovosa1Click
      Animation = True
      Caption = #268'asov'#225' osa'
      CanFocused = False
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphLeft
      ImageIndex = -1
      ImageMargin = 0
      TransparentBackground = True
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBtnFace
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = 16250613
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 255
      Options.FocusedColorAlpha = 30
      Options.DisabledColorAlpha = 0
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
      Options.FrameWidth = 1
      Options.FrameNormalColorAlpha = 255
      Options.FrameHotColorAlpha = 255
      Options.FramePressedColorAlpha = 255
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 255
      Options.FontNormalColor = clWhite
      Options.FontHotColor = clWhite
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clWhite
      Options.FontDisabledColor = clGray
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
      GlowEffect.Color = clHighlight
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      GlowEffect.HotColor = clNone
      GlowEffect.PressedColor = clBtnFace
      GlowEffect.FocusedColor = clNone
      GlowEffect.PressedGlowSize = 7
      GlowEffect.PressedAlphaValue = 255
      GlowEffect.States = [scsHot, scsPressed, scsFocused]
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
    end
    object technologickprocesy1: TscGPButton
      Left = 560
      Top = 0
      Width = 78
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      OnClick = technologickprocesy1Click
      Animation = True
      Caption = 'Procesy'
      CanFocused = False
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphLeft
      ImageIndex = -1
      ImageMargin = 0
      TransparentBackground = True
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBtnFace
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = 16250613
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 255
      Options.FocusedColorAlpha = 30
      Options.DisabledColorAlpha = 0
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
      Options.FrameWidth = 1
      Options.FrameNormalColorAlpha = 255
      Options.FrameHotColorAlpha = 255
      Options.FramePressedColorAlpha = 255
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 255
      Options.FontNormalColor = clWhite
      Options.FontHotColor = clWhite
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clWhite
      Options.FontDisabledColor = clGray
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
      GlowEffect.Color = clHighlight
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      GlowEffect.HotColor = clNone
      GlowEffect.PressedColor = clBtnFace
      GlowEffect.FocusedColor = clNone
      GlowEffect.PressedGlowSize = 7
      GlowEffect.PressedAlphaValue = 255
      GlowEffect.States = [scsHot, scsPressed, scsFocused]
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
    end
    object scGPGlyphButton1: TscGPGlyphButton
      AlignWithMargins = True
      Left = 836
      Top = 0
      Width = 35
      Height = 33
      Margins.Left = 5
      Margins.Top = 0
      Margins.Right = 5
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 7
      OnClick = scGPGlyphButton1Click
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
      GlyphOptions.Kind = scgpbgkGear
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
      GroupIndex = 2
      AllowAllUp = True
    end
    object Ulozit: TscGPGlyphButton
      AlignWithMargins = True
      Left = 207
      Top = 0
      Width = 35
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alLeft
      TabOrder = 8
      OnClick = UlozitClick
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
      GlyphOptions.Kind = scgpbgkFileSave
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
    object NovySoubor: TscGPGlyphButton
      AlignWithMargins = True
      Left = 137
      Top = 0
      Width = 35
      Height = 33
      Margins.Left = 5
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alLeft
      TabOrder = 9
      OnClick = NovySouborClick
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
      GlyphOptions.Kind = scgpbgkFileNew
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
    object Otevrit: TscGPGlyphButton
      AlignWithMargins = True
      Left = 172
      Top = 0
      Width = 35
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alLeft
      TabOrder = 10
      OnClick = OtevritClick
      Animation = True
      Caption = 'Otev'#345#237't'
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
      GlyphOptions.Kind = scgpbgkFileOpen
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
    object MaxButton: TscGPGlyphButton
      AlignWithMargins = True
      Left = 926
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 11
      OnClick = MaxButtonClick
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
    end
    object DetailsButton: TscGPGlyphButton
      AlignWithMargins = True
      Left = 81
      Top = 0
      Width = 51
      Height = 33
      Hint = 'Menu'
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alLeft
      TabOrder = 12
      Visible = False
      OnClick = DetailsButtonClick
      Animation = True
      Caption = 'scGPGlyphButton1'
      CanFocused = False
      CustomDropDown = False
      Layout = blGlyphLeft
      TransparentBackground = True
      ColorValue = clRed
      Options.NormalColor = 33023
      Options.HotColor = 1871359
      Options.PressedColor = clNone
      Options.FocusedColor = 33023
      Options.DisabledColor = clNone
      Options.NormalColorAlpha = 255
      Options.HotColorAlpha = 255
      Options.PressedColorAlpha = 200
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
      Options.FontDisabledColor = 33023
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
      GlyphOptions.Kind = scgpbgkDetails
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
    object scGPButton1: TscGPButton
      Left = 638
      Top = 0
      Width = 78
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 13
      OnClick = casovosa1Click
      Animation = True
      Caption = 'Layout'
      CanFocused = False
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphLeft
      ImageIndex = -1
      ImageMargin = 0
      TransparentBackground = True
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBtnFace
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = 16250613
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 255
      Options.FocusedColorAlpha = 30
      Options.DisabledColorAlpha = 0
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
      Options.FrameWidth = 1
      Options.FrameNormalColorAlpha = 255
      Options.FrameHotColorAlpha = 255
      Options.FramePressedColorAlpha = 255
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 255
      Options.FontNormalColor = clWhite
      Options.FontHotColor = clWhite
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clWhite
      Options.FontDisabledColor = clGray
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
      GlowEffect.Color = clHighlight
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      GlowEffect.HotColor = clNone
      GlowEffect.PressedColor = clBtnFace
      GlowEffect.FocusedColor = clNone
      GlowEffect.PressedGlowSize = 7
      GlowEffect.PressedAlphaValue = 255
      GlowEffect.States = [scsHot, scsPressed, scsFocused]
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
    end
    object scGPButton2: TscGPButton
      Left = 716
      Top = 0
      Width = 78
      Height = 34
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 14
      OnClick = casovosa1Click
      Animation = True
      Caption = 'Simulace'
      CanFocused = False
      CustomDropDown = False
      Margin = -1
      Spacing = 1
      Layout = blGlyphLeft
      ImageIndex = -1
      ImageMargin = 0
      TransparentBackground = True
      Options.NormalColor = clWhite
      Options.HotColor = clWhite
      Options.PressedColor = clBtnFace
      Options.FocusedColor = clBtnFace
      Options.DisabledColor = 16250613
      Options.NormalColorAlpha = 0
      Options.HotColorAlpha = 30
      Options.PressedColorAlpha = 255
      Options.FocusedColorAlpha = 30
      Options.DisabledColorAlpha = 0
      Options.FrameNormalColor = clNone
      Options.FrameHotColor = clNone
      Options.FramePressedColor = clNone
      Options.FrameFocusedColor = clNone
      Options.FrameDisabledColor = clNone
      Options.FrameWidth = 1
      Options.FrameNormalColorAlpha = 255
      Options.FrameHotColorAlpha = 255
      Options.FramePressedColorAlpha = 255
      Options.FrameFocusedColorAlpha = 255
      Options.FrameDisabledColorAlpha = 255
      Options.FontNormalColor = clWhite
      Options.FontHotColor = clWhite
      Options.FontPressedColor = clBtnText
      Options.FontFocusedColor = clWhite
      Options.FontDisabledColor = clGray
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
      GlowEffect.Color = clHighlight
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      GlowEffect.HotColor = clNone
      GlowEffect.PressedColor = clBtnFace
      GlowEffect.FocusedColor = clNone
      GlowEffect.PressedGlowSize = 7
      GlowEffect.PressedAlphaValue = 255
      GlowEffect.States = [scsHot, scsPressed, scsFocused]
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
    end
    object scGPGlyphButton15: TscGPGlyphButton
      Left = 794
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
      TabOrder = 15
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
  object scGPPanel3: TscGPPanel
    Left = 0
    Top = 698
    Width = 1026
    Height = 28
    Align = alBottom
    TabOrder = 29
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
    Caption = 'scGPPanel3'
    TransparentBackground = False
    StorePaintBuffer = True
    object RzStatusPane1: TRzStatusPane
      Left = 158
      Top = 0
      Width = 127
      Height = 28
      Hint = 'Kliknut'#237'm lze p'#345'epnout m'#243'd zobrazen'#237
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      BorderWidth = 0
      FlatColor = 10114859
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      OnClick = RzStatusPane1Click
      BlinkColor = 10114859
      Caption = ''
      DefaultWidth = 50
      ExplicitLeft = 161
      ExplicitTop = 6
      ExplicitHeight = 24
    end
    object RzStatusPane2: TRzStatusPane
      Left = 285
      Top = 0
      Width = 77
      Height = 28
      Hint = 'Aktu'#225'ln'#237' '#250'rove'#328' p'#345'ibl'#237#382'en'#237' obrazu'
      BorderWidth = 0
      FlatColor = 10114859
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      BlinkColor = 10114859
      Caption = ''
      ExplicitLeft = 356
      ExplicitTop = -3
      ExplicitHeight = 24
    end
    object RzStatusPane3: TRzStatusPane
      Left = 362
      Top = 0
      Height = 28
      Hint = 'Aktu'#225'ln'#237' polohu kurzoru v sou'#345'adnic'#237'ch aplikace'
      BorderWidth = 0
      FlatColor = 10114859
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      BlinkColor = 10114859
      Caption = ''
      ExplicitLeft = 333
      ExplicitTop = -3
      ExplicitHeight = 24
    end
    object RzStatusPane4: TRzStatusPane
      Left = 538
      Top = 0
      Width = 462
      Height = 28
      Hint = 'Rychl'#225' n'#225'pov'#283'da aplikace'
      BorderWidth = 0
      FlatColor = 10114859
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      BlinkColor = 10114859
      Caption = ''
      DefaultWidth = 200
      ExplicitLeft = 462
      ExplicitWidth = 283
      ExplicitHeight = 24
    end
    object RzStatusPane5: TRzStatusPane
      Left = 462
      Top = 0
      Width = 76
      Height = 28
      Hint = 'Kliknut'#237'm zm'#283'n'#237'te re'#382'im p'#345'ichyt'#225'v'#225'n'#237' objektu ke m'#345#237#382'ce'
      BorderWidth = 0
      FlatColor = 10114859
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      OnClick = RzStatusPane5Click
      BlinkColor = 10114859
      Caption = ''
      ExplicitLeft = 874
      ExplicitTop = -3
      ExplicitHeight = 24
    end
    object scGPTrackBar1: TscGPTrackBar
      AlignWithMargins = True
      Left = 29
      Top = 3
      Width = 100
      Height = 23
      Margins.Left = 1
      Margins.Right = 1
      Margins.Bottom = 2
      Align = alLeft
      TabOrder = 0
      TransparentBackground = True
      ShowFocusRect = True
      JumpWhenClick = True
      CanFocused = False
      MouseSupport = True
      MinValue = 0
      MaxValue = 100
      Value = 50
      Vertical = False
      ReadOnly = False
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
    object scGPGlyphButton5: TscGPGlyphButton
      AlignWithMargins = True
      Left = 1
      Top = 1
      Width = 27
      Height = 27
      Margins.Left = 1
      Margins.Top = 1
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 1
      OnClick = scGPGlyphButton5Click
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
      GlyphOptions.NormalColorAlpha = 255
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkMinus
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
    object scGPGlyphButton6: TscGPGlyphButton
      AlignWithMargins = True
      Left = 130
      Top = 1
      Width = 27
      Height = 27
      Margins.Left = 0
      Margins.Top = 1
      Margins.Right = 1
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 2
      OnClick = scGPGlyphButton6Click
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
      GlyphOptions.NormalColorAlpha = 255
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkPlus
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
    object scGPSizeBox1: TscGPSizeBox
      Left = 1000
      Top = 0
      Width = 26
      Height = 28
      Align = alRight
      TabOrder = 3
      Visible = False
      GlyphColor = clWhite
      GlyphColorAlpha = 200
      GlyphThickness = 1
    end
  end
  object scCheckBox2: TscCheckBox
    Left = 299
    Top = 321
    Width = 131
    Height = 27
    TabOrder = 30
    TabStop = True
    Visible = False
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
    Caption = 'Show TscGPSizeBox'
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
    ImageGlow = True
    Checked = True
    State = cbChecked
    ShowFocusRect = True
  end
  object scSplitView1: TscSplitView
    Left = 1026
    Top = 34
    Width = 0
    Height = 621
    Anchors = [akTop, akRight, akBottom]
    Enabled = False
    TabOrder = 31
    CustomImageIndex = -1
    StyleKind = scpsPanel
    ShowCaption = False
    BorderStyle = scpbsLeftShadowLine
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
    StorePaintBuffer = False
    AnimationStep = 25
    CompactWidth = 0
    CompactHeight = 20
    GripSize = 0
    Opened = False
    OpenedWidth = 250
    OpenedHeight = 50
    OpenedMinWidth = 0
    OpenedMaxWidth = 0
    OpenedMinHeight = 0
    OpenedMaxHeight = 0
    ParentBackground = False
    ParentColor = False
    Placement = scsvpRight
    HideControls = False
    Animation = True
    DisplayMode = scsvmOverlay
    object scLabel4: TscLabel
      AlignWithMargins = True
      Left = 11
      Top = 10
      Width = 229
      Height = 30
      Margins.Left = 10
      Margins.Top = 10
      Margins.Right = 10
      Margins.Bottom = 10
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      DragForm = False
      GlowEffect.Enabled = False
      GlowEffect.Color = clBtnShadow
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      AutoSize = True
      UseFontColorToStyleColor = False
      Caption = 'Options'
    end
    object scScrollBox1: TscScrollBox
      Left = 1
      Top = 50
      Width = 249
      Height = 571
      HorzScrollBar.Tracking = True
      VertScrollBar.Tracking = True
      Align = alLeft
      BorderStyle = bsNone
      TabOrder = 1
      WallpaperIndex = -1
      CustomBackgroundImageIndex = -1
      FullUpdate = False
      BackgroundStyle = scsbsPanel
      object scListGroupPanel1: TscListGroupPanel
        Left = 0
        Top = 0
        Width = 249
        Height = 150
        Align = alTop
        TabOrder = 0
        AutoSize = True
        RowCount = 3
        RowLineMargin = 20
        RowHeight = 40
        GroupColor = clWindow
        GroupColorAlpha = 255
        Color = clBtnFace
        Caption = 'scListGroupPanel1'
        HeaderFont.Charset = DEFAULT_CHARSET
        HeaderFont.Color = clBtnText
        HeaderFont.Height = -15
        HeaderFont.Name = 'Tahoma'
        HeaderFont.Style = []
        HeaderHeight = 30
        HeaderMargin = 10
        HeaderAutoColor = True
        TransparentBackground = False
        object scLabel5: TscLabel
          Left = 20
          Top = 47
          Width = 53
          Height = 13
          TabOrder = 0
          DragForm = False
          GlowEffect.Enabled = False
          GlowEffect.Color = clBtnShadow
          GlowEffect.AlphaValue = 255
          GlowEffect.GlowSize = 7
          GlowEffect.Offset = 0
          GlowEffect.Intensive = True
          GlowEffect.StyleColors = True
          AutoSize = True
          UseFontColorToStyleColor = True
          Caption = 'Option #1:'
        end
        object scLabel6: TscLabel
          Left = 20
          Top = 85
          Width = 53
          Height = 13
          TabOrder = 1
          DragForm = False
          GlowEffect.Enabled = False
          GlowEffect.Color = clBtnShadow
          GlowEffect.AlphaValue = 255
          GlowEffect.GlowSize = 7
          GlowEffect.Offset = 0
          GlowEffect.Intensive = True
          GlowEffect.StyleColors = True
          AutoSize = True
          UseFontColorToStyleColor = True
          Caption = 'Option #2:'
        end
        object scLabel9: TscLabel
          Left = 21
          Top = 123
          Width = 53
          Height = 13
          TabOrder = 2
          DragForm = False
          GlowEffect.Enabled = False
          GlowEffect.Color = clBtnShadow
          GlowEffect.AlphaValue = 255
          GlowEffect.GlowSize = 7
          GlowEffect.Offset = 0
          GlowEffect.Intensive = True
          GlowEffect.StyleColors = True
          AutoSize = True
          UseFontColorToStyleColor = True
          Caption = 'Option #3:'
        end
        object scGPSwitch1: TscGPSwitch
          Left = 170
          Top = 43
          Width = 44
          Height = 20
          TabOrder = 3
          Animation = True
          FrameColor = clWindowText
          FrameOnColor = clHighlight
          FramePressedColor = clBtnShadow
          ParentColor = True
          State = scswOn
          StyleKind = scswsStyled
          ThumbColor = clWindowText
          ThumbOnColor = clHighlightText
          ThumbPressedColor = clBtnText
          FrameColorAlpha = 255
          FrameOnColorAlpha = 255
          FramePressedColorAlpha = 255
          ThumbColorAlpha = 255
          ThumbOnColorAlpha = 255
          ThumbPressedColorAlpha = 255
          ThumbShadow = False
          FrameSolid = False
          FrameOnSolid = True
          FrameInside = False
        end
        object scGPSwitch2: TscGPSwitch
          Left = 170
          Top = 82
          Width = 44
          Height = 20
          TabOrder = 4
          Animation = True
          FrameColor = clWindowText
          FrameOnColor = clHighlight
          FramePressedColor = clBtnShadow
          ParentColor = True
          State = scswOff
          StyleKind = scswsStyled
          ThumbColor = clWindowText
          ThumbOnColor = clHighlightText
          ThumbPressedColor = clBtnText
          FrameColorAlpha = 255
          FrameOnColorAlpha = 255
          FramePressedColorAlpha = 255
          ThumbColorAlpha = 255
          ThumbOnColorAlpha = 255
          ThumbPressedColorAlpha = 255
          ThumbShadow = False
          FrameSolid = False
          FrameOnSolid = True
          FrameInside = False
        end
        object scGPSwitch3: TscGPSwitch
          Left = 171
          Top = 120
          Width = 44
          Height = 20
          TabOrder = 5
          Animation = True
          FrameColor = clWindowText
          FrameOnColor = clHighlight
          FramePressedColor = clBtnShadow
          ParentColor = True
          State = scswOn
          StyleKind = scswsStyled
          ThumbColor = clWindowText
          ThumbOnColor = clHighlightText
          ThumbPressedColor = clBtnText
          FrameColorAlpha = 255
          FrameOnColorAlpha = 255
          FramePressedColorAlpha = 255
          ThumbColorAlpha = 255
          ThumbOnColorAlpha = 255
          ThumbPressedColorAlpha = 255
          ThumbShadow = False
          FrameSolid = False
          FrameOnSolid = True
          FrameInside = False
        end
      end
      object scListGroupPanel3: TscListGroupPanel
        Left = 0
        Top = 150
        Width = 249
        Height = 110
        Align = alTop
        TabOrder = 1
        AutoSize = True
        RowCount = 2
        RowLineMargin = 20
        RowHeight = 40
        GroupColor = clWindow
        GroupColorAlpha = 255
        Color = clBtnFace
        Caption = 'Login information'
        HeaderFont.Charset = DEFAULT_CHARSET
        HeaderFont.Color = clBtnText
        HeaderFont.Height = -15
        HeaderFont.Name = 'Tahoma'
        HeaderFont.Style = []
        HeaderHeight = 30
        HeaderMargin = 10
        HeaderAutoColor = True
        TransparentBackground = False
        object scLabel12: TscLabel
          Left = 19
          Top = 46
          Width = 55
          Height = 13
          TabOrder = 0
          DragForm = False
          GlowEffect.Enabled = False
          GlowEffect.Color = clBtnShadow
          GlowEffect.AlphaValue = 255
          GlowEffect.GlowSize = 7
          GlowEffect.Offset = 0
          GlowEffect.Intensive = True
          GlowEffect.StyleColors = True
          AutoSize = True
          UseFontColorToStyleColor = True
          Caption = 'User name:'
        end
        object scLabel17: TscLabel
          Left = 19
          Top = 85
          Width = 50
          Height = 13
          TabOrder = 1
          DragForm = False
          GlowEffect.Enabled = False
          GlowEffect.Color = clBtnShadow
          GlowEffect.AlphaValue = 255
          GlowEffect.GlowSize = 7
          GlowEffect.Offset = 0
          GlowEffect.Intensive = True
          GlowEffect.StyleColors = True
          AutoSize = True
          UseFontColorToStyleColor = True
          Caption = 'Password:'
        end
        object scPasswordEdit1: TscPasswordEdit
          Left = 86
          Top = 81
          Width = 120
          Height = 21
          Cursor = crIBeam
          TabOrder = 2
          FrameColor = clBtnShadow
          FrameActiveColor = clHighlight
          Transparent = True
          BorderKind = sccebNone
          ContentMarginLeft = 0
          ContentMarginRight = 0
          ContentMarginTop = 0
          ContentMarginBottom = 0
          PasswordCharImageIndex = -1
          PasswordCharSelectedImageIndex = -1
          WallpaperIndex = -1
          CustomBackgroundImageNormalIndex = -1
          CustomBackgroundImageHotIndex = -1
          CustomBackgroundImageDisabledIndex = -1
          UseFontColorToStyleColor = True
          Text = 'password'
        end
        object scEdit2: TscEdit
          Left = 88
          Top = 43
          Width = 108
          Height = 21
          UseFontColorToStyleColor = True
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
          Transparent = True
          BorderKind = scebFrame
          FrameColor = clBtnShadow
          FrameActiveColor = clHighlight
          Text = 'Admin'
          BorderStyle = bsNone
          TabOrder = 3
        end
      end
      object scListGroupPanel4: TscListGroupPanel
        Left = 0
        Top = 260
        Width = 249
        Height = 150
        Align = alTop
        TabOrder = 2
        AutoSize = True
        RowCount = 3
        RowLineMargin = 20
        RowHeight = 40
        GroupColor = clWindow
        GroupColorAlpha = 255
        Color = clBtnFace
        Caption = 'scListGroupPanel3'
        HeaderFont.Charset = DEFAULT_CHARSET
        HeaderFont.Color = clBtnText
        HeaderFont.Height = -15
        HeaderFont.Name = 'Tahoma'
        HeaderFont.Style = []
        HeaderHeight = 30
        HeaderMargin = 10
        HeaderAutoColor = True
        TransparentBackground = False
        object scLabel18: TscLabel
          Left = 20
          Top = 47
          Width = 53
          Height = 13
          TabOrder = 0
          DragForm = False
          GlowEffect.Enabled = False
          GlowEffect.Color = clBtnShadow
          GlowEffect.AlphaValue = 255
          GlowEffect.GlowSize = 7
          GlowEffect.Offset = 0
          GlowEffect.Intensive = True
          GlowEffect.StyleColors = True
          AutoSize = True
          UseFontColorToStyleColor = True
          Caption = 'Option #4:'
        end
        object scAdvancedComboBox1: TscAdvancedComboBox
          Left = 106
          Top = 40
          Width = 98
          Height = 25
          TabOrder = 1
          UseFontColorToStyleColor = True
          AutoComplete = False
          Animation = False
          CheckedListMode = False
          CheckedListWrap = True
          Items = <
            item
              Button.Enabled = True
              Button.Visible = False
              Button.StyleKind = sclbsPushButton
              Button.Width = 50
              Button.Height = 25
              Button.ImageIndex = -1
              Button.ImageHotIndex = -1
              Button.ImagePressedIndex = -1
              ProgressBar.Min = 0
              ProgressBar.Max = 100
              ProgressBar.Value = 0
              Header = False
              Enabled = True
              Caption = 'Item1'
              Checked = False
            end
            item
              Button.Enabled = True
              Button.Visible = False
              Button.StyleKind = sclbsPushButton
              Button.Width = 50
              Button.Height = 25
              Button.ImageIndex = -1
              Button.ImageHotIndex = -1
              Button.ImagePressedIndex = -1
              ProgressBar.Min = 0
              ProgressBar.Max = 100
              ProgressBar.Value = 0
              Header = False
              Enabled = True
              Caption = 'Item2'
              Checked = False
            end
            item
              Button.Enabled = True
              Button.Visible = False
              Button.StyleKind = sclbsPushButton
              Button.Width = 50
              Button.Height = 25
              Button.ImageIndex = -1
              Button.ImageHotIndex = -1
              Button.ImagePressedIndex = -1
              ProgressBar.Min = 0
              ProgressBar.Max = 100
              ProgressBar.Value = 0
              Header = False
              Enabled = True
              Caption = 'Item3'
              Checked = False
            end
            item
              Button.Enabled = True
              Button.Visible = False
              Button.StyleKind = sclbsPushButton
              Button.Width = 50
              Button.Height = 25
              Button.ImageIndex = -1
              Button.ImageHotIndex = -1
              Button.ImagePressedIndex = -1
              ProgressBar.Min = 0
              ProgressBar.Max = 100
              ProgressBar.Value = 0
              Header = False
              Enabled = True
              Caption = 'Item4'
              Checked = False
            end>
          ItemIndex = 0
          DropDownCount = 7
          Style = sccbColorCombo
          HideSelection = False
          SelectionStyle = scastStyled
          SelectionColor = clNone
          SelectionTextColor = clHighlightText
          SelectionGlow.Enabled = False
          SelectionGlow.Color = clHighlight
          SelectionGlow.AlphaValue = 255
          SelectionGlow.GlowSize = 7
          SelectionGlow.Offset = 0
          SelectionGlow.Intensive = True
          SelectionGlow.StyleColors = True
          ShowFocusRect = True
          AlternateRow = False
          ShowItemTitle = True
          ShowItemImage = True
          ShowItemText = True
          ShowItemDetail = False
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'Tahoma'
          TitleFont.Style = [fsBold]
          HeaderFont.Charset = DEFAULT_CHARSET
          HeaderFont.Color = clWindowText
          HeaderFont.Height = -11
          HeaderFont.Name = 'Tahoma'
          HeaderFont.Style = [fsBold]
          DetailFont.Charset = DEFAULT_CHARSET
          DetailFont.Color = clGrayText
          DetailFont.Height = -11
          DetailFont.Name = 'Tahoma'
          DetailFont.Style = []
          ColorOptions.NormalColor = clNone
          ColorOptions.HotColor = clNone
          ColorOptions.PressedColor = clNone
          ColorOptions.FocusedColor = clNone
          ColorOptions.DisabledColor = clNone
          ColorOptions.FrameNormalColor = clNone
          ColorOptions.FrameHotColor = clNone
          ColorOptions.FramePressedColor = clNone
          ColorOptions.FrameFocusedColor = clNone
          ColorOptions.FrameDisabledColor = clNone
          ColorOptions.FrameWidth = 1
          ColorOptions.FontNormalColor = clWindowText
          ColorOptions.FontHotColor = clWindowText
          ColorOptions.FontPressedColor = clWindowText
          ColorOptions.FontFocusedColor = clWindowText
          ColorOptions.FontDisabledColor = clGray
          ColorOptions.TitleFontNormalColor = clBtnText
          ColorOptions.TitleFontHotColor = clBtnText
          ColorOptions.TitleFontPressedColor = clBtnText
          ColorOptions.TitleFontFocusedColor = clBtnText
          ColorOptions.TitleFontDisabledColor = clBtnShadow
          ColorOptions.StyleColors = True
          CustomImageNormalIndex = -1
          CustomImageHotIndex = -1
          CustomImagePressedIndex = -1
          CustomImageDisabledIndex = -1
          CustomImageFocusedIndex = -1
          CustomArrowImageNormalIndex = -1
          CustomArrowImageHotIndex = -1
          CustomArrowImagePressedIndex = -1
          CustomArrowImageDisabledIndex = -1
          CustomArrowImageFocusedIndex = -1
          ListBoxHeaderUseStyleColor = True
          ListBoxWallpaperIndex = -1
          ListBoxHeaderStyle = scahsDefault
          ListBoxLineColor = clBtnFace
          ListBoxWidth = 0
          ListBoxHeight = 0
          ListBoxShowItemTitles = True
          ListBoxShowItemDetails = False
          ListBoxShowLines = False
          ListBoxItemHeight = 25
          ListBoxHeaderHeight = 20
          ListBoxSelectionStyle = scastStyled
          ListBoxSelectionColor = clNone
          ListBoxSelectionTextColor = clHighlightText
          ListBoxCustomSelectionImageIndex = -1
          ListBoxCustomHeaderImageIndex = -1
        end
        object scCheckBox5: TscCheckBox
          Left = 20
          Top = 77
          Width = 98
          Height = 29
          TabOrder = 2
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
          UseFontColorToStyleColor = True
          Animation = False
          Caption = 'scCheckBox1'
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
          ImageGlow = True
          Checked = True
          State = cbChecked
          ShowFocusRect = True
        end
        object scCheckBox6: TscCheckBox
          Left = 21
          Top = 118
          Width = 98
          Height = 24
          TabOrder = 3
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
          UseFontColorToStyleColor = True
          Animation = False
          Caption = 'scCheckBox2'
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
          ImageGlow = True
          Checked = False
          ShowFocusRect = True
        end
      end
    end
  end
  object RzStatusBar1: TRzStatusBar
    Left = 0
    Top = 655
    Width = 1026
    Height = 19
    BorderInner = fsNone
    BorderOuter = fsNone
    BorderSides = [sdLeft, sdTop, sdRight, sdBottom]
    BorderWidth = 0
    TabOrder = 33
    Visible = False
    VisualStyle = vsGradient
  end
  object scCheckBox1: TscCheckBox
    Left = 299
    Top = 349
    Width = 140
    Height = 30
    TabOrder = 34
    TabStop = True
    Visible = False
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
    Caption = 'Show maximize button'
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
    ImageGlow = True
    Checked = True
    State = cbChecked
    ShowFocusRect = True
  end
  object scRadioGroup1: TscRadioGroup
    Left = 727
    Top = 469
    Width = 132
    Height = 64
    TabOrder = 35
    Visible = False
    FramePosition = scgfpDefault
    CustomImageIndex = -1
    CustomImageDisabledIndex = -1
    CustomCheckedImageIndex = -1
    CustomCheckedImageHotIndex = -1
    CustomCheckedImagePressedIndex = -1
    CustomCheckedImageDisabledIndex = -1
    CustomUnCheckedImageIndex = -1
    CustomUnCheckedImageHotIndex = -1
    CustomUnCheckedImagePressedIndex = -1
    CustomUnCheckedImageDisabledIndex = -1
    Alignment = taLeftJustify
    Caption = 'Menu type'
    ImageIndex = -1
    ShowCheckBox = False
    Checked = False
    AutoEnabledControls = False
    GlowEffect.Enabled = False
    GlowEffect.Color = clHighlight
    GlowEffect.AlphaValue = 255
    GlowEffect.GlowSize = 7
    GlowEffect.Offset = 0
    GlowEffect.Intensive = True
    GlowEffect.StyleColors = True
    ImageGlow = True
    StorePaintBuffer = True
    ItemIndex = 0
    Items.Strings = (
      'PopupMenu'
      'SplitView')
    ButtonsGlowEffect.Enabled = False
    ButtonsGlowEffect.Color = clHighlight
    ButtonsGlowEffect.AlphaValue = 255
    ButtonsGlowEffect.GlowSize = 7
    ButtonsGlowEffect.Offset = 0
    ButtonsGlowEffect.Intensive = True
    ButtonsGlowEffect.StyleColors = True
    ButtonsGlowEffect.HotColor = clNone
    ButtonsGlowEffect.PressedColor = clNone
    ButtonsGlowEffect.FocusedColor = clNone
    ButtonsGlowEffect.PressedGlowSize = 7
    ButtonsGlowEffect.PressedAlphaValue = 255
    ButtonsGlowEffect.States = [scsHot, scsPressed, scsFocused]
    ButtonsAnimation = False
    ShowFocusRect = True
  end
  object scSplitView2: TscSplitView
    Left = 0
    Top = 34
    Width = 167
    Height = 621
    Anchors = [akLeft, akTop, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNone
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 36
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
    Color = clSilver
    StorePaintBuffer = False
    AnimationStep = 25
    CompactWidth = 0
    CompactHeight = 20
    GripSize = 0
    Opened = True
    OpenedWidth = 167
    OpenedHeight = 50
    OpenedMinWidth = 0
    OpenedMaxWidth = 0
    OpenedMinHeight = 0
    OpenedMaxHeight = 0
    ParentBackground = False
    ParentColor = False
    Placement = scsvpLeft
    HideControls = False
    Animation = True
    DisplayMode = scsvmOverlay
    object RzSizePanel_knihovna_objektu: TscListGroupPanel
      Left = 3
      Top = 109
      Width = 166
      Height = 300
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      AutoSize = False
      RowCount = 1
      RowLineMargin = 20
      RowHeight = 0
      GroupColor = clWindow
      GroupColorAlpha = 255
      Color = 10114859
      Caption = 'Knihovna objekt'#367
      HeaderFont.Charset = DEFAULT_CHARSET
      HeaderFont.Color = clWhite
      HeaderFont.Height = -15
      HeaderFont.Name = 'Tahoma'
      HeaderFont.Style = []
      HeaderHeight = 30
      HeaderMargin = 10
      HeaderAutoColor = False
      TransparentBackground = False
      object DrawGrid_knihovna: TDrawGrid
        Left = 3
        Top = 48
        Width = 203
        Height = 400
        ParentCustomHint = False
        Align = alCustom
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        ColCount = 2
        Ctl3D = False
        DefaultColWidth = 70
        DefaultRowHeight = 50
        FixedColor = clWhite
        FixedCols = 0
        RowCount = 4
        FixedRows = 0
        GridLineWidth = 0
        Options = []
        ParentCtl3D = False
        ScrollBars = ssNone
        TabOrder = 0
        OnDrawCell = DrawGrid_knihovnaDrawCell
        OnKeyDown = DrawGrid_knihovnaKeyDown
        OnKeyUp = DrawGrid_knihovnaKeyUp
        OnMouseDown = DrawGrid_knihovnaMouseDown
        OnMouseLeave = DrawGrid_knihovnaMouseLeave
        OnMouseWheelDown = DrawGrid_knihovnaMouseWheelDown
        OnMouseWheelUp = DrawGrid_knihovnaMouseWheelUp
      end
    end
    object RzSizePanel_parametry_projekt: TscListGroupPanel
      Left = 3
      Top = 6
      Width = 166
      Height = 113
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      AutoSize = False
      RowCount = 1
      RowLineMargin = 20
      RowHeight = 0
      GroupColor = clWindow
      GroupColorAlpha = 255
      Color = 10114859
      Caption = 'Nastaven'#237' projektu'
      HeaderFont.Charset = DEFAULT_CHARSET
      HeaderFont.Color = clWhite
      HeaderFont.Height = -15
      HeaderFont.Name = 'Tahoma'
      HeaderFont.Style = []
      HeaderHeight = 30
      HeaderMargin = 10
      HeaderAutoColor = False
      TransparentBackground = False
      DesignSize = (
        166
        113)
      object Label2: TLabel
        Left = 3
        Top = 83
        Width = 94
        Height = 16
        Caption = 'Definice zak'#225'zek'
        Color = clWindowFrame
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowFrame
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        Transparent = True
      end
      object Label8: TLabel
        Left = 3
        Top = 47
        Width = 88
        Height = 16
        Caption = 'Parametry linky'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowFrame
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        Transparent = True
      end
      object Button_dopravnik_parametry: TscGPGlyphButton
        Left = 121
        Top = 41
        Width = 30
        Height = 30
        Anchors = [akTop, akRight, akBottom]
        TabOrder = 0
        OnClick = Button_dopravnik_parametryClick
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
        Options.ShapeCornerRadius = 10
        Options.ShapeStyle = scgpRoundedRect
        Options.ArrowSize = 9
        Options.StyleColors = True
        GlyphOptions.NormalColor = clBtnText
        GlyphOptions.HotColor = clBtnText
        GlyphOptions.PressedColor = clBtnText
        GlyphOptions.FocusedColor = clBtnText
        GlyphOptions.DisabledColor = clBtnText
        GlyphOptions.NormalColorAlpha = 200
        GlyphOptions.HotColorAlpha = 255
        GlyphOptions.PressedColorAlpha = 255
        GlyphOptions.FocusedColorAlpha = 255
        GlyphOptions.DisabledColorAlpha = 150
        GlyphOptions.Kind = scgpbgkDetailPoints
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
      object Button_vozik_parametry: TscGPGlyphButton
        Left = 121
        Top = 72
        Width = 30
        Height = 30
        Anchors = [akTop, akRight, akBottom]
        TabOrder = 1
        OnClick = Button_vozik_parametryClick
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
        Options.ShapeCornerRadius = 10
        Options.ShapeStyle = scgpRoundedRect
        Options.ArrowSize = 9
        Options.StyleColors = True
        GlyphOptions.NormalColor = clBtnText
        GlyphOptions.HotColor = clBtnText
        GlyphOptions.PressedColor = clBtnText
        GlyphOptions.FocusedColor = clBtnText
        GlyphOptions.DisabledColor = clBtnText
        GlyphOptions.NormalColorAlpha = 200
        GlyphOptions.HotColorAlpha = 255
        GlyphOptions.PressedColorAlpha = 255
        GlyphOptions.FocusedColorAlpha = 255
        GlyphOptions.DisabledColorAlpha = 150
        GlyphOptions.Kind = scgpbgkDetailPoints
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
  object scGPGlyphButton2: TscGPGlyphButton
    Left = 230
    Top = 585
    Width = 30
    Height = 30
    TabOrder = 38
    Visible = False
    OnClick = scGPGlyphButton2Click
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
    Options.NormalColorAlpha = 10
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
    Options.ShapeStyle = scgpRounded
    Options.ArrowSize = 9
    Options.StyleColors = True
    GlyphOptions.NormalColor = clBtnText
    GlyphOptions.HotColor = clBtnText
    GlyphOptions.PressedColor = clBtnText
    GlyphOptions.FocusedColor = clBtnText
    GlyphOptions.DisabledColor = clBtnText
    GlyphOptions.NormalColorAlpha = 200
    GlyphOptions.HotColorAlpha = 255
    GlyphOptions.PressedColorAlpha = 255
    GlyphOptions.FocusedColorAlpha = 255
    GlyphOptions.DisabledColorAlpha = 150
    GlyphOptions.Kind = scgpbgkPriorArrow
    GlyphOptions.Thickness = 2
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
  object scSplitView4: TscSplitView
    Left = 0
    Top = 34
    Width = 180
    Height = 621
    Anchors = [akLeft, akTop, akBottom]
    TabOrder = 39
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
    Alignment = taLeftJustify
    Color = clBtnFace
    StorePaintBuffer = False
    AnimationStep = 25
    CompactWidth = 0
    CompactHeight = 20
    GripSize = 0
    Opened = True
    OpenedWidth = 180
    OpenedHeight = 50
    OpenedMinWidth = 0
    OpenedMaxWidth = 0
    OpenedMinHeight = 0
    OpenedMaxHeight = 0
    ParentBackground = False
    ParentColor = False
    Placement = scsvpLeft
    HideControls = False
    Animation = True
    DisplayMode = scsvmOverlay
    object scButton1: TscButton
      Left = 0
      Top = 571
      Width = 180
      Height = 50
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Animation = False
      Caption = 'Konec'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      Images = scGPVirtualImageList1
      ImageIndex = 4
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = clRed
      ColorOptions.PressedColor = 191
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
      WordWrap = False
    end
    object scButton2: TscButton
      Left = 0
      Top = 0
      Width = 180
      Height = 50
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = NovySouborClick
      Animation = False
      Caption = 'Nov'#253' projekt'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 2
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
      WordWrap = False
    end
    object scButton3: TscButton
      Left = 0
      Top = 50
      Width = 180
      Height = 50
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = OtevritClick
      Animation = False
      Caption = 'Otev'#345#237't'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 2
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
      WordWrap = False
    end
    object scButton4: TscButton
      Left = 0
      Top = 250
      Width = 180
      Height = 64
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Animation = False
      Caption = 'Nastaven'#237
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      Images = scGPVirtualImageList1
      ImageIndex = 3
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
      WordWrap = False
    end
    object scButton5: TscButton
      Left = 0
      Top = 521
      Width = 180
      Height = 50
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = Obnovitzezlohy1Click
      Animation = False
      Caption = 'Obnovit ze z'#225'lohy'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 5
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
      WordWrap = False
    end
    object scButton6: TscButton
      Left = 0
      Top = 150
      Width = 180
      Height = 50
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = html1Click
      Animation = False
      Caption = 'Report'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 2
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
      WordWrap = False
    end
    object scGPSwitch4: TscGPSwitch
      Left = 81
      Top = 336
      Width = 42
      Height = 20
      TabOrder = 6
      OnClick = antialiasing1Click
      Animation = True
      FrameColor = clWhite
      FrameOnColor = clWhite
      FramePressedColor = 5912332
      ParentColor = True
      State = scswOn
      StyleKind = scswsStyled
      ThumbColor = clWhite
      ThumbOnColor = 8538404
      ThumbPressedColor = clWhite
      FrameColorAlpha = 255
      FrameOnColorAlpha = 255
      FramePressedColorAlpha = 255
      ThumbColorAlpha = 255
      ThumbOnColorAlpha = 255
      ThumbPressedColorAlpha = 255
      ThumbShadow = False
      FrameSolid = False
      FrameOnSolid = True
      FrameInside = False
    end
    object scGPSwitch5: TscGPSwitch
      Left = 81
      Top = 362
      Width = 42
      Height = 20
      TabOrder = 7
      OnClick = MrizkaClick
      Animation = True
      FrameColor = clWhite
      FrameOnColor = clWhite
      FramePressedColor = 5912332
      ParentColor = True
      State = scswOn
      StyleKind = scswsStyled
      ThumbColor = clWhite
      ThumbOnColor = 8538404
      ThumbPressedColor = clWhite
      FrameColorAlpha = 255
      FrameOnColorAlpha = 255
      FramePressedColorAlpha = 255
      ThumbColorAlpha = 255
      ThumbOnColorAlpha = 255
      ThumbPressedColorAlpha = 255
      ThumbShadow = False
      FrameSolid = False
      FrameOnSolid = True
      FrameInside = False
    end
    object scLabel11: TscLabel
      Left = 1
      Top = 336
      Width = 65
      Height = 16
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      DragForm = False
      GlowEffect.Enabled = False
      GlowEffect.Color = clBtnShadow
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      AutoSize = True
      UseFontColorToStyleColor = False
      Caption = 'Antialiasing'
    end
    object scLabel16: TscLabel
      Left = 3
      Top = 358
      Width = 37
      Height = 16
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      DragForm = False
      GlowEffect.Enabled = False
      GlowEffect.Color = clBtnShadow
      GlowEffect.AlphaValue = 255
      GlowEffect.GlowSize = 7
      GlowEffect.Offset = 0
      GlowEffect.Intensive = True
      GlowEffect.StyleColors = True
      AutoSize = True
      UseFontColorToStyleColor = False
      Caption = 'M'#345#237#382'ka'
    end
    object scButton7: TscButton
      Left = 0
      Top = 100
      Width = 180
      Height = 50
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 10
      OnClick = UlozitjakoClick
      Animation = False
      Caption = 'Ulo'#382'it jako...'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 2
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
      WordWrap = False
    end
    object scButton8: TscButton
      Left = 0
      Top = 200
      Width = 180
      Height = 50
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
      OnClick = Export1Click
      Animation = False
      Caption = 'Export...'
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 2
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 1
      AllowAllUp = False
      WordWrap = False
    end
    object scButton9: TscButton
      Left = 0
      Top = 471
      Width = 180
      Height = 50
      Align = alBottom
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 12
      OnClick = MagnaClick
      Animation = False
      Caption = 'Naposledy otev'#345'en'#233
      CanFocused = False
      CustomDropDown = False
      Margin = 0
      Spacing = 10
      Layout = blGlyphLeft
      ImageIndex = 5
      ImageMargin = 50
      TransparentBackground = False
      ColorOptions.NormalColor = 8538404
      ColorOptions.HotColor = 7159583
      ColorOptions.PressedColor = 5648664
      ColorOptions.FocusedColor = 657930
      ColorOptions.DisabledColor = 657930
      ColorOptions.FrameNormalColor = clNone
      ColorOptions.FrameHotColor = clNone
      ColorOptions.FramePressedColor = clNone
      ColorOptions.FrameFocusedColor = clNone
      ColorOptions.FrameDisabledColor = clBtnShadow
      ColorOptions.FrameWidth = 0
      ColorOptions.FontNormalColor = clWhite
      ColorOptions.FontHotColor = clWhite
      ColorOptions.FontPressedColor = clWhite
      ColorOptions.FontFocusedColor = clWhite
      ColorOptions.FontDisabledColor = clGray
      ColorOptions.TitleFontNormalColor = clBtnText
      ColorOptions.TitleFontHotColor = clBtnText
      ColorOptions.TitleFontPressedColor = clBtnText
      ColorOptions.TitleFontFocusedColor = clBtnText
      ColorOptions.TitleFontDisabledColor = clBtnShadow
      ColorOptions.StyleColors = True
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = [fsBold]
      HotImageIndex = -1
      FocusedImageIndex = -1
      PressedImageIndex = -1
      StyleKind = scbsColorButton
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
      ImageGlow = True
      ShowGalleryMenuFromTop = False
      ShowGalleryMenuFromRight = False
      ShowMenuArrow = True
      SplitButton = False
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
      WordWrap = False
    end
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 688
    Top = 184
    object Nastvitparametry1: TMenuItem
      Caption = 'Nastavit parametry'
      Visible = False
      OnClick = Nastvitparametry1Click
    end
    object Smazat1: TMenuItem
      Caption = 'Smazat objekt'
      Visible = False
      OnClick = Smazat1Click
    end
    object Zobrazitparametry1: TMenuItem
      Caption = 'Zobrazit parametry'
      Visible = False
      OnClick = Zobrazitparametry1Click
    end
    object Rychlexport1: TMenuItem
      Caption = 'Rychl'#253' export'
      Visible = False
      OnClick = Rychlexport1Click
    end
    object N5: TMenuItem
      Caption = '-'
    end
    object Posouvat2: TMenuItem
      Bitmap.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888880000000
        088888880FFFFFFFF0888880FFFFFFFFF088880FFFFFFFFFFF08880FFFFFFFFF
        FF08880FF00FFFFFFFF0880F0880FFFFFFF0880F0880F0F0F0F088808880F0F0
        F0F000088880F0F0F0F00880880F0F0F0F080880880F0F0F0F08000880F0F0F0
        8088088888080808888808888888888888880008888888888888}
      Caption = 'Posouvat'
      ShortCut = 116
      OnClick = Posouvat2Click
    end
    object Posunout3: TMenuItem
      Bitmap.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        888888880000000088888880FFFFFFFF0888880FFFFFFFFF088880FFFFFFFFFF
        F08880FFFFFFFFFFF08880FF00FFFFFFFF0880F0880FFFFFFF0880F0880F0F0F
        0F088808880F0F0F0F088888880F0F0F0F08888880F0F0F0F088888880F0F0F0
        F08888880F0F0F08088888888080808888888888888888888888}
      Caption = 'Posunout'
      ShortCut = 117
      OnClick = Posunout3Click
    end
    object Priblizit2: TMenuItem
      Bitmap.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        8800888888888888800088888888888800088888888888800088888800008800
        08888800FFFF0000888880FFFFFFFF08888880FFF00FFF0888880FFFF00FFFF0
        88880FF000000FF088880FF000000FF088880FFFF00FFFF0888880FFF00FFF08
        888880FFFFFFFF0888888800FFFF008888888888000088888888}
      Caption = 'P'#345'ibl'#237#382'it'
      ShortCut = 118
      OnClick = Priblizit2Click
    end
    object Oddalit2: TMenuItem
      Bitmap.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        8800888888888888800088888888888800088888888888800088888800008800
        08888800FFFF0000888880FFFFFFFF08888880FFFFFFFF0888880FFFFFFFFFF0
        88880FF000000FF088880FF000000FF088880FFFFFFFFFF0888880FFFFFFFF08
        888880FFFFFFFF0888888800FFFF008888888888000088888888}
      Caption = 'Odd'#225'lit'
      ShortCut = 119
      OnClick = Oddalit2Click
    end
    object Vybratoknem2: TMenuItem
      Bitmap.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        8800877777777777700087FFFFFFFFFF000887FFFF000FF0007887FF00FFF000
        0F7887F0FFFFFFF0FF7887F0FFF0FFF0FF78870FFFF0FFFF0F78870FF00000FF
        0F78870FFFF0FFFF0F7887F0FFF0FFF0FF7887F0FFFFFFF0FF7887FF00FFF00F
        FF7887FFFF000FFFFF7887777777777777788888888888888888}
      Caption = 'Vybrat oknem...'
      ShortCut = 115
      OnClick = Vybratoknem2Click
    end
  end
  object ImageList1: TImageList
    Left = 592
    Top = 96
    Bitmap = {
      494C010118009C02CC0210001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000007000000001002000000000000070
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500FFFFFF00CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC0000000000999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500FFFFFF00CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC0000000000999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600E5E5
      E500CC9966000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999000000
      0000999999000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600E5E5
      E500CC9966000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999000000
      0000999999000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600CC99
      6600000000000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900000000000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600CC99
      6600000000000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC9966000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999000000
      0000000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC9966000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500FFFFFF00CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC0000000000999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500FFFFFF00CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC0000000000999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600E5E5
      E500CC9966000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999000000
      0000999999000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600E5E5
      E500CC9966000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999000000
      0000999999000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600CC99
      6600000000000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900000000000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600CC99
      6600000000000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC9966000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999000000
      0000000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC9966000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCC
      CC00000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00CC99660000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500FFFFFF00CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC0000000000999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500FFFFFF00CC996600CC99
      6600CC996600CC99660000000000000000000000000000000000999999000000
      0000CCCCCC00CCCCCC00CCCCCC00CCCCCC00CCCCCC0000000000999999009999
      9900999999009999990000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600E5E5
      E500CC9966000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999000000
      0000999999000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600E5E5
      E500CC9966000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999000000
      0000999999000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600CC99
      6600000000000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900000000000000000000000000000000000000000000000000CC996600FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CC996600CC99
      6600000000000000000000000000000000000000000000000000999999000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC9966000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999000000
      0000000000000000000000000000000000000000000000000000CC996600CC99
      6600CC996600CC996600CC996600CC996600CC996600CC996600CC9966000000
      0000000000000000000000000000000000000000000000000000999999009999
      9900999999009999990099999900999999009999990099999900999999000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080000000000000000000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000080808000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      000000000000FFFFFF0080808000000000000000000080808000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      000000000000FFFFFF0080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0080808000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0080808000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0080808000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0080808000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000008080800000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF008080800000000000000000008080800000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF0080808000000000000000000000000000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      000000000000000000000000000000000000000000008080800000000000FFFF
      FF00FFFFFF000000000000000000000000000000000000000000FFFFFF00FFFF
      FF0000000000FFFFFF008080800000000000000000008080800000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00808080000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      000000000000000000000000000000000000000000008080800000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF008080800000000000000000008080800000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00808080000000000000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0080808000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00808080000000000000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0080808000000000000000000080808000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00808080000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF0080808000000000000000000080808000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF0080808000000000000000000000000000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0080808000000000000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0080808000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000000000000000000000000000000000000FFFF
      FF0000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000000000000000000000000000000000000FFFF
      FF0000000000000000000000000000000000FFFFFF0000000000FFFFFF000000
      0000FFFFFF0000000000FFFFFF00000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000000000FFFFFF000000
      0000FFFFFF0000000000FFFFFF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000000000FFFFFF000000
      0000FFFFFF0000000000FFFFFF000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF00000000000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008080000080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000080
      8000008080000080800000808000008080000080800000808000008080000080
      8000000000000000000000000000000000000000000000000000008080000080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF000000
      0000008080000080800000808000008080000080800000808000008080000080
      8000008080000000000000000000000000000000000000000000008080000080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF00000000000080800000808000008080000080800000808000008080000080
      8000008080000080800000000000000000000000000000000000008080000080
      8000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF000000000000808000008080000080800000808000008080000080
      8000008080000080800000808000000000000000000000000000008080000080
      8000008080000080800000808000008080000080800000808000008080000080
      8000008080000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008080000080
      8000000000000000000000000000000000000000000000000000000000000000
      0000008080000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000700000000100010000000000800300000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFFFFFFFFC003C003C003C003
      C003DFFBC003DFFBC003D00BC003D00BC003DFFBC003DFFBC003D00BC003D00B
      C003DFFBC003DFFBC003D00BC003D00BC003DFFBC003DFFBC003D00BC003D00B
      C003DFFBC003DFFBC003D043C003D043C007DFD7C007DFD7C00FDFCFC00FDFCF
      C01FC01FC01FC01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC003C003C003C003
      C003DFFBC003DFFBC003D00BC003D00BC003DFFBC003DFFBC003D00BC003D00B
      C003DFFBC003DFFBC003D00BC003D00BC003DFFBC003DFFBC003D00BC003D00B
      C003DFFBC003DFFBC003D043C003D043C007DFD7C007DFD7C00FDFCFC00FDFCF
      C01FC01FC01FC01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC003C003C003C003
      C003DFFBC003DFFBC003D00BC003D00BC003DFFBC003DFFBC003D00BC003D00B
      C003DFFBC003DFFBC003D00BC003D00BC003DFFBC003DFFBC003D00BC003D00B
      C003DFFBC003DFFBC003D043C003D043C007DFD7C007DFD7C00FDFCFC00FDFCF
      C01FC01FC01FC01FFFFFFFFFFFFFFFFFFFFCFFFCFFFCFFFCFFF880008000FFF8
      FFF180018001FFF1FFE380018001FFE3F0C780018001F0C7C00F80018001C00F
      801F80018001801F801F80018001801F000F80018001000F000F80018001000F
      000F80018001000F000F80018001000F801F80018001801F801F80018001801F
      C03F80018001C03FF0FFFFFFFFFFF0FFFFFFFFFFF807FFFCFFFFF00FF003FFF8
      FFFFE007E003FFF1FFFFC007C001FFE3EFFF8003C001F0C7EF838003C000C00F
      DFC38001C600801FDFE38C01C600801FDFD38C01EE00000FEF3BDC011E00000F
      F0FFFC016C01000FFFFFF8036C01000FFFFFF803180B801FFFFFF0177D5F801F
      FFFFFABF7FFFC03FFFFFFFFF1FFFF0FFFFFFFFFFFFFFFFFFFFFFFFFFC001FFFF
      C007800F8031FFFFC00780078031FFFFC00780038031FFF7C00780018001C1F7
      C00780008001C3FBC00780008001C7FBC007800F8FF1CBFBC007800F8FF1DCF7
      C007800F8FF1FF0FC007C7F88FF1FFFFC00FFFFC8FF1FFFFC01FFFBA8FF5FFFF
      C03FFFC78001FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 624
    Top = 96
  end
  object OpenDialog1: TOpenDialog
    Left = 656
    Top = 96
  end
  object Timer_backup: TTimer
    Interval = 120000
    OnTimer = Timer_backupTimer
    Left = 720
    Top = 96
  end
  object SavePictureDialog1: TSavePictureDialog
    DefaultExt = '*.bmp'
    Filter = 
      'All (*.gif;*.png;*.jpg;*.jpeg;*.bmp;*.tif;*.tiff;*.ico;*.emf;*.w' +
      'mf)|*.gif;*.png;*.jpg;*.jpeg;*.bmp;*.tif;*.tiff;*.ico;*.emf;*.wm' +
      'f|GIF Image (*.gif)|*.gif|Portable Network Graphics (*.png)|*.pn' +
      'g|JPEG Image File (*.jpg)|*.jpg|JPEG Image File (*.jpeg)|*.jpeg|' +
      'Bitmaps (*.bmp)|*.bmp|TIFF Images (*.tif)|*.tif|TIFF Images (*.t' +
      'iff)|*.tiff|Icons (*.ico)|*.ico|Enhanced Metafiles (*.emf)|*.emf' +
      '|Metafiles (*.wmf)|*.wmf'
    Left = 688
    Top = 96
  end
  object Timer_simulace: TTimer
    Enabled = False
    Interval = 17
    OnTimer = Timer_simulaceTimer
    Left = 816
    Top = 96
  end
  object Timer_neaktivity: TTimer
    Enabled = False
    OnTimer = Timer_neaktivityTimer
    Left = 784
    Top = 96
  end
  object Timer_animace: TTimer
    Enabled = False
    Interval = 40
    OnTimer = Timer_animaceTimer
    Left = 848
    Top = 96
  end
  object TeeGDIPlus1: TTeeGDIPlus
    Active = True
    AntiAliasText = gpfBest
    TeePanel = Chart1
    Left = 808
    Top = 216
  end
  object TeeGDIPlus2: TTeeGDIPlus
    Active = True
    AntiAliasText = gpfBest
    TeePanel = Chart2
    Left = 936
    Top = 216
  end
  object TeeGDIPlus3: TTeeGDIPlus
    Active = True
    AntiAliasText = gpfBest
    TeePanel = Chart3
    Left = 904
    Top = 216
  end
  object TeeGDIPlus4: TTeeGDIPlus
    Active = True
    AntiAliasText = gpfBest
    TeePanel = Chart4
    Left = 872
    Top = 216
  end
  object TeeGDIPlus5: TTeeGDIPlus
    Active = True
    AntiAliasText = gpfBest
    TeePanel = Chart6
    Left = 840
    Top = 216
  end
  object TimerMouseWheel: TTimer
    Interval = 250
    OnTimer = TimerMouseWheelTimer
    Left = 880
    Top = 96
  end
  object IdTime1: TIdTime
    Timeout = 500
    Host = '129.6.15.28'
    BaseDate = 2.000000000000000000
    Left = 767
    Top = 168
  end
  object IdHTTP1: TIdHTTP
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.ContentRangeEnd = -1
    Request.ContentRangeStart = -1
    Request.ContentRangeInstanceLength = -1
    Request.Accept = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    Request.Ranges.Units = 'bytes'
    Request.Ranges = <>
    HTTPOptions = [hoForceEncodeParams]
    Left = 799
    Top = 168
  end
  object Timer_tr: TTimer
    Enabled = False
    Interval = 130000
    OnTimer = Timer_trTimer
    Left = 752
    Top = 96
  end
  object FDConnection1: TFDConnection
    Params.Strings = (
      'ConnectionDef=Mysql')
    FetchOptions.AssignedValues = [evCursorKind]
    FetchOptions.CursorKind = ckDefault
    ResourceOptions.AssignedValues = [rvSilentMode]
    ResourceOptions.SilentMode = True
    Left = 600
    Top = 360
  end
  object FDQuery1: TFDQuery
    Connection = FDConnection1
    ResourceOptions.AssignedValues = [rvSilentMode]
    ResourceOptions.SilentMode = True
    Left = 648
    Top = 360
  end
  object FDQuery2: TFDQuery
    Connection = FDConnection1
    Left = 696
    Top = 360
  end
  object PopupMenu2: TPopupMenu
    Left = 927
    Top = 424
    object Open1: TMenuItem
      Caption = '&Open...'
    end
    object New1: TMenuItem
      Caption = '&New'
    end
    object Save1: TMenuItem
      Caption = '&Save'
    end
    object SaveAs1: TMenuItem
      Caption = 'Save &As...'
    end
    object MenuItem1: TMenuItem
      Caption = '-'
    end
    object Print1: TMenuItem
      Caption = '&Print...'
    end
    object PrintSetup1: TMenuItem
      Caption = 'P&rint Setup...'
    end
    object MenuItem2: TMenuItem
      Caption = '-'
    end
    object Exit1: TMenuItem
      Caption = 'E&xit'
    end
  end
  object scStyledForm1: TscStyledForm
    DWMClientShadow = False
    DropDownForm = False
    DropDownAnimation = False
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
    Left = 927
    Top = 480
  end
  object ImageList48: TImageList
    ColorDepth = cd32Bit
    Height = 48
    Width = 48
    Left = 503
    Top = 438
    Bitmap = {
      494C010106000800940030003000FFFFFFFF2110FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000C00000006000000001002000000000000020
      0100000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000040404200404042000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000909090C0909090C000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF31313170000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000010101102424
      2460797979B0909090C0A9A9A9D0FFFFFFFFFFFFFFFFA9A9A9D0909090C07979
      79B0242424600101011000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000009090930FFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F0090909300000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000019191950797979B0FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF797979B0101010400000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000A9A9A9D0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF909090C00000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040909090C0FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF909090C010101040000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000031313170FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3131
      3170000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000051515190FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA8A8
      A8CF3030306F0F0F0F3F090909300000000000000000090909300F0F0F3F3030
      306FA8A8A8CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF515151900000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000404
      0420E1E1E1F0FFFFFFFFFFFFFFFF4040408040404080FFFFFFFFFFFFFFFFE1E1
      E1F0040404200000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000101
      0110909090C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3F3F3F7F010101100000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000010101103F3F3F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7979
      79B0010101100000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000009090
      90C0FFFFFFFFFFFFFFFFA8A8A8CF0000000000000000A8A8A8CFFFFFFFFFFFFF
      FFFF909090C00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000001010110A9A9
      A9D0FFFFFFFFFFFFFFFFFFFFFFFF787878AF0404042000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000004040420787878AFFFFFFFFFFFFFFFFFFFFF
      FFFFA9A9A9D00101011000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000024242460FFFF
      FFFFFFFFFFFFFFFFFFFF09090930000000000000000010101040FFFFFFFFFFFF
      FFFFFFFFFFFF1919195000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000909090C0FFFF
      FFFFFFFFFFFFFFFFFFFF3F3F3F7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000003F3F3F7FFFFFFFFFFFFF
      FFFFFFFFFFFF797979B000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000001010110E1E1E1F0FFFF
      FFFFFFFFFFFF515151900000000000000000000000000000000051515190FFFF
      FFFFFFFFFFFFE1E1E1F001010110000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000051515190FFFFFFFFFFFF
      FFFFFFFFFFFF2424246000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000024242460FFFF
      FFFFFFFFFFFFFFFFFFFF51515190000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000051515190FFFFFFFFFFFF
      FFFFE0E0E0EF010101100000000000000000000000000000000001010110E0E0
      E0EFFFFFFFFFFFFFFFFF51515190000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000010101040FFFFFFFFFFFFFFFFFFFF
      FFFF404040800000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000004040
      4080FFFFFFFFFFFFFFFFFFFFFFFF101010400000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000004040420FFFFFFFFFFFFFFFFFFFF
      FFFF242424600000000000000000000000000000000000000000000000002424
      2460FFFFFFFFFFFFFFFFFFFFFFFF040404200000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000909090C0FFFFFFFFFFFFFFFF7878
      78AF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000787878AFFFFFFFFFFFFFFFFF909090C00000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000909090C0FFFFFFFFFFFFFFFF8F8F
      8FBF000000000000000000000000000000000000000000000000000000000000
      00008F8F8FBFFFFFFFFFFFFFFFFF909090C00000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000019191950FFFFFFFFFFFFFFFFFFFFFFFF0404
      0420000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000004040420FFFFFFFFFFFFFFFFFFFFFFFF1919195000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000010101040FFFFFFFFFFFFFFFFFFFFFFFF0909
      0930000000000000000000000000000000000000000000000000000000000000
      000009090930FFFFFFFFFFFFFFFFFFFFFFFF1010104000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000797979B0FFFFFFFFFFFFFFFF404040800000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000040404080FFFFFFFFFFFFFFFF797979B000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000A9A9A9D0FFFFFFFFFFFFFFFF646464A00000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000646464A0FFFFFFFFFFFFFFFFA9A9A9D000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000001010110FFFFFFFFFFFFFFFFFFFFFFFF010101100000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000001010110FFFFFFFFFFFFFFFFFFFFFFFF01010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000024242460FFFFFFFFFFFFFFFFE0E0E0EF040404200000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000004040420E0E0E0EFFFFFFFFFFFFFFFFF24242460000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000024242460FFFFFFFFFFFFFFFFA8A8A8CF000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000A8A8A8CFFFFFFFFFFFFFFFFF24242460000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C4C4C4E0FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFFC4C4C4E0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000797979B0FFFFFFFFFFFFFFFF31313170000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000031313170FFFFFFFFFFFFFFFF797979B0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000024242460FFFFFFFFFFFFFFFFE0E0E0EF01010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000001010110E0E0E0EFFFFFFFFFFFFFFFFF242424600000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000909090C0FFFFFFFFFFFFFFFF10101040000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040FFFFFFFFFFFFFFFF909090C0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C4C4C4E0FFFFFFFFFFFFFFFF4040408000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000040404080FFFFFFFFFFFFFFFFC4C4C4E00000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000A9A9A9D0FFFFFFFFFFFFFFFF09090930000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000009090930FFFFFFFFFFFFFFFFA9A9A9D0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000024242460FFFFFFFFFFFFFFFFE0E0E0EF0101011000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000001010110E0E0E0EFFFFFFFFFFFFFFFFF2424
      2460000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000909090C0FFFFFFFFFFFFFFFF404040800000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000040404080FFFFFFFFFFFFFFFF9090
      90C0000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000909
      0930FFFFFFFFFFFFFFFFFFFFFFFF010101100000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000001010110FFFFFFFFFFFFFFFFFFFF
      FFFF040404200000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E0E0E0EFFFFFFFFFFFFFFFFF01010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000004040420FFFFFFFFFFFFFFFFC3C3C3DF000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000005151
      5190FFFFFFFFFFFFFFFF646464A0000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000646464A0FFFFFFFFFFFF
      FFFF515151900000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000909090C0FFFFFFFFFFFFFFFF10101040000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040FFFFFFFFFFFFFFFF909090C0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C4C4
      C4E0FFFFFFFFFFFFFFFF19191950000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000019191950FFFFFFFFFFFF
      FFFFC4C4C4E00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000646464A0FFFFFFFFFFFFFFFF31313170000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000031313170FFFFFFFFFFFFFFFF646464A0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000004040420FFFF
      FFFFFFFFFFFFE0E0E0EF00000000000000000000000000000000000000000000
      0000000000000000000051515190C4C4C4E0C4C4C4E040404080000000000000
      0000000000000000000000000000000000000000000000000000E0E0E0EFFFFF
      FFFFFFFFFFFF0404042000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000024242460FFFFFFFFFFFFFFFF797979B0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000797979B0FFFFFFFFFFFFFFFF24242460000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000024242460FFFF
      FFFFFFFFFFFF909090C000000000000000000000000000000000000000000000
      00000000000040404080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF404040800000
      0000000000000000000000000000000000000000000000000000909090C0FFFF
      FFFFFFFFFFFF1919195000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000004040420FFFFFFFFFFFFFFFFFFFFFFFF040404200000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000004040420FFFFFFFFFFFFFFFFFFFFFFFF04040420000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000040404080FFFF
      FFFFFFFFFFFF4040408000000000000000000000000000000000000000000000
      000000000000E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4C4C4E00000
      000000000000000000000000000000000000000000000000000040404080FFFF
      FFFFFFFFFFFF4040408000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000008F8F8FBFFFFFFFFFFFFFFFFF404040800000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000040404080FFFFFFFFFFFFFFFF8F8F8FBF00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000040404080FFFF
      FFFFFFFFFFFF4040408000000000000000000000000000000000000000000000
      000000000000E0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3C3C3DF0000
      000000000000000000000000000000000000000000000000000040404080FFFF
      FFFFFFFFFFFF4040408000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000019191950FFFFFFFFFFFFFFFFE1E1E1F00404
      0420000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000004040420E1E1E1F0FFFFFFFFFFFFFFFF1919195000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000019191950FFFF
      FFFFFFFFFFFF797979B000000000000000000000000000000000000000000000
      00000000000051515190FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF515151900000
      0000000000000000000000000000000000000000000000000000797979B0FFFF
      FFFFFFFFFFFF1919195000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000A8A8A8CFFFFFFFFFFFFFFFFF7979
      79B0000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000797979B0FFFFFFFFFFFFFFFFA8A8A8CF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000010101040FFFF
      FFFFFFFFFFFFA9A9A9D000000000000000000000000000000000000000000000
      000000000000010101105050508FC3C3C3DFC3C3C3DF3F3F3F7F000000000000
      0000000000000000000000000000000000000000000000000000A9A9A9D0FFFF
      FFFFFFFFFFFF1010104000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000010101040FFFFFFFFFFFFFFFFFFFF
      FFFF313131700000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000003131
      3170FFFFFFFFFFFFFFFFFFFFFFFF101010400000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000E0E0
      E0EFFFFFFFFFFFFFFFFF04040420000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000004040420FFFFFFFFFFFF
      FFFFE0E0E0EF0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000051515190FFFFFFFFFFFF
      FFFFFFFFFFFF2424246000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      000000000000000000000000000000000000000000000000000024242460FFFF
      FFFFFFFFFFFFFFFFFFFF51515190000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000007979
      79B0FFFFFFFFFFFFFFFF40404080000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000040404080FFFFFFFFFFFF
      FFFF646464A00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000A8A8A8CFFFFF
      FFFFFFFFFFFFFFFFFFFF31313170000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000031313170FFFFFFFFFFFF
      FFFFFFFFFFFF8F8F8FBF00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000001010
      1040FFFFFFFFFFFFFFFFFFFFFFFF040404200000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000004040420FFFFFFFFFFFFFFFFFFFF
      FFFF101010400000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000001010110A8A8
      A8CFFFFFFFFFFFFFFFFFFFFFFFFF797979B00101011000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      000000000000000000000000000001010110797979B0FFFFFFFFFFFFFFFFFFFF
      FFFFA8A8A8CF0101011000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008F8F8FBFFFFFFFFFFFFFFFFFA9A9A9D00000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000A9A9A9D0FFFFFFFFFFFFFFFF8F8F
      8FBF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000101
      0110A8A8A8CFFFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F040404080000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000040404080E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFFA8A8
      A8CF010101100000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000010101040FFFFFFFFFFFFFFFFFFFFFFFF646464A000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000646464A0FFFFFFFFFFFFFFFFFFFFFFFF0909
      0930000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000005050508FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      00000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5050508F0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000024242460FFFFFFFFFFFFFFFFFFFFFFFFA9A9A9D0040404200000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000004040420A9A9A9D0FFFFFFFFFFFFFFFFFFFFFFFF242424600000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040C3C3C3DFFFFFFFFFFFFFFFFF000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      00000000000000000000FFFFFFFFFFFFFFFFA8A8A8CF10101040000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4040
      4080040404200000000000000000000000000000000000000000000000000404
      042040404080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6363639F000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000001818184F8F8F8FBF000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      000000000000000000008F8F8FBF1818184F0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000024242460FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFA9A9A9D0797979B04040408040404080797979B0A9A9A9D0FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2424246000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000040404080FFFFFFFFFFFFFFFF40404080000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000090909308F8F8FBFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF8F8F8FBF090909300000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000101010407878
      78AFE0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0E0
      E0EF787878AF1010104000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000F0F0F3F1818184F3F3F3F7F3F3F3F7F1818184F0F0F0F3F0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000037373777565656940000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000565656941B1B1B54000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFFFFF1F1
      F1F80707072D0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000707
      072DF1F1F1F8FFFFFFFF00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000404
      0420404040804040408040404080404040804040408040404080404040804040
      4080040404200000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000024242460646464A00000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F1F1F1F8FFFF
      FFFFFFFFFFFF757575AD00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000757575ADFFFF
      FFFFFFFFFFFFF1F1F1F800000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000003131
      3170FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF313131700000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000024242460FFFFFFFFFFFFFFFF646464A000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000ADADADD2FFFF
      FFFFFFFFFFFFFFFFFFFFF1F1F1F81B1B1B540000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000001B1B1B54F1F1F1F8FFFFFFFFFFFF
      FFFFFFFFFFFFADADADD200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000006464
      64A0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF646464A00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFF646464A0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000037373777FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFADADADD200000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000ADADADD2FFFFFFFFFFFFFFFFFDFDFDFEFFFF
      FFFFFFFFFFFF3737377700000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000A9A9
      A9D0FFFFFFFFFFFFFFFF6363639F3F3F3F7F3F3F3F7F6363639FFFFFFFFFFFFF
      FFFFA9A9A9D00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFF646464A00000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FDFD
      FDFEFFFFFFFF37373777D4D4D4E9FFFFFFFFFFFFFFFFF7F7F7FB1B1B1B540000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000001B1B1B54F7F7F7FBFFFFFFFFFFFFFFFFD4D4D4E937373777FFFF
      FFFFFDFDFDFE0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFF0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFF6464
      64A0000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000E5E5
      E5F2FFFFFFFF929292C10000000056565694FDFDFDFEFFFFFFFFFFFFFFFFC4C4
      C4E0000000000000000000000000000000000000000000000000000000000000
      0000C4C4C4E0FFFFFFFFFFFFFFFFFDFDFDFE5656569400000000929292C1FFFF
      FFFFE5E5E5F20000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000010101040FFFF
      FFFFFFFFFFFFA8A8A8CF00000000000000000000000000000000A8A8A8CFFFFF
      FFFFFFFFFFFF1010104000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000006363639FFFFFFFFFFFFFFFFFFFFF
      FFFF646464A00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000009292
      92C1FFFFFFFFE5E5E5F200000000000000000707072DC3C3C3DFFFFFFFFFFFFF
      FFFFFBFBFBFD373737770000000000000000000000000000000037373777FBFB
      FBFDFFFFFFFFFFFFFFFFC3C3C3DF0707072D0000000000000000E5E5E5F2FFFF
      FFFF929292C10000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000010101040000000000000
      0000000000000000000000000000000000000000000010101040909090C0FFFF
      FFFFFFFFFFFF646464A000000000000000000000000000000000646464A0FFFF
      FFFFFFFFFFFF909090C010101040000000000000000000000000000000000000
      0000000000000000000010101040000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000006363639FFFFFFFFFFFFF
      FFFFFFFFFFFF646464A000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000001B1B
      1B54FFFFFFFFFFFFFFFF0000000000000000000000000000000037373777FBFB
      FBFDFFFFFFFFFFFFFFFFC4C4C4E00000000000000000C4C4C4E0FFFFFFFFFFFF
      FFFFFBFBFBFD3737377700000000000000000000000000000000FFFFFFFFFFFF
      FFFF1B1B1B540000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000010101040FFFFFFFFE1E1E1F06464
      64A019191950000000000000000004040420909090C0FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF313131700000000000000000000000000000000031313170FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF909090C00404042000000000000000001919
      1950646464A0E1E1E1F0FFFFFFFF101010400000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000006363639FFFFF
      FFFFFFFFFFFFFFFFFFFF646464A0000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FBFBFBFDFFFFFFFF1B1B1B54000000000000000000000000000000000000
      0000C3C3C3DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3C3
      C3DF000000000000000000000000000000000000000037373777FFFFFFFFFBFB
      FBFD000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C4C4C4E0FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFE1E1E1F0646464A0E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFFE0E0
      E0EF5050508F0101011000000000000000000000000000000000010101105050
      508FE0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F0646464A0E1E1E1F0FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFC4C4C4E00000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000006363
      639FFFFFFFFFFFFFFFFFFFFFFFFF646464A00000000000000000000000000000
      0000000000000000000000000000000000000000000009090930101010404040
      4080404040803131317010101040090909300000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000D6D6D6EAFFFFFFFFADADADD2000000000000000000000000000000000000
      0000000000001B1B1B54F7F7F7FBFFFFFFFFFFFFFFFFF7F7F7FB1B1B1B540000
      00000000000000000000000000000000000000000000ADADADD2FFFFFFFFD6D6
      D6EA000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000031313170FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0E0E0EF2323235F0101
      0110000000000000000000000000000000000000000000000000000000000000
      0000010101102323235FE0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3131317000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00006363639FFFFFFFFFFFFFFFFFFFFFFFFF646464A000000000000000000000
      0000000000000000000009090930646464A0C4C4C4E0FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4C4C4E051515190101010400000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000757575ADFFFFFFFFF1F1F1F8000000000000000000000000000000000000
      0000000000000000000000000000ABABABD1ABABABD100000000000000000000
      00000000000000000000000000000000000000000000F1F1F1F8FFFFFFFF7575
      75AD000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000001010110E1E1E1F0FFFFFFFFFFFFFFFF3F3F3F7F5050
      508FC3C3C3DFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8FBF01010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000010101108F8F8FBFFFFFFFFFFFFFFFFFFFFFFFFFC3C3
      C3DF5050508F3F3F3F7FFFFFFFFFFFFFFFFFE1E1E1F001010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFF646464A0000000000000
      000009090930909090C0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7979
      79B0040404200000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000707072DFFFFFFFFFFFFFFFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF0707
      072D000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000646464A0FFFFFFFFFFFFFFFFA8A8A8CF000000000000
      000000000000090909305050508F5050508F0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000005050508F5050508F090909300000
      00000000000000000000A8A8A8CFFFFFFFFFFFFFFFFF646464A0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFF646464A03131
      3170FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0E0E0EF8F8F8FBF5050
      508F3F3F3F7F5050508F8F8F8FBFE0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF2424246000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000F7F7F7FBFFFFFFFF373737770000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000037373777FFFFFFFFF7F7F7FB0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000009090930FFFFFFFFFFFFFFFFFFFFFFFF10101040000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000101011019191950404040804040408019191950010101100000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040FFFFFFFFFFFFFFFFFFFFFFFF090909300000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF6363639F0909093000000000000000000000
      000000000000000000000000000000000000090909306363639FFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF646464A0000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C4C4C4E0FFFFFFFFADADADD20000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C4C4C4E0FFFFFFFFC4C4C4E00000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000909090C0FFFFFFFFFFFFFFFFFFFFFFFF09090930000000000000
      0000000000000000000000000000000000000000000000000000000000000101
      0110646464A0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6464
      64A0010101100000000000000000000000000000000000000000000000000000
      0000000000000000000009090930FFFFFFFFFFFFFFFFFFFFFFFF909090C00000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000006363639FFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFA8A8A8CF04040420000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000004040420A8A8
      A8CFFFFFFFFFFFFFFFFFFFFFFFFF646464A00000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000056565694FFFFFFFFF7F7F7FB0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F7F7F7FBFFFFFFFF565656940000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000024242460FFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F0101010400000
      000000000000000000000000000000000000000000000000000009090930C4C4
      C4E0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFC4C4C4E00909093000000000000000000000000000000000000000000000
      00000000000010101040E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFF242424600000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000031313170FFFFFFFFFFFFFFFFFFFF
      FFFF6363639F0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00006363639FFFFFFFFFFFFFFFFFFFFFFFFF2424246000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000707072DFFFFFFFFFFFFFFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFF0707072D0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000009090930E0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFF2424
      2460000000000000000000000000000000000000000001010110C4C4C4E0FFFF
      FFFFFFFFFFFFFFFFFFFFA8A8A8CF5050508F5050508FA8A8A8CFFFFFFFFFFFFF
      FFFFFFFFFFFFC4C4C4E001010110000000000000000000000000000000000000
      000024242460FFFFFFFFFFFFFFFFFFFFFFFFE0E0E0EF09090930000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000009090930FFFFFFFFFFFFFFFFFFFFFFFF6363
      639F000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000006363639FFFFFFFFFFFFFFFFFFFFFFFFF04040420000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000037373777FBFBFBFDFFFFFFFFFDFDFDFE1B1B1B5400000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000001B1B1B54FDFDFDFEFFFFFFFFFBFBFBFD1B1B
      1B54000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000009090930C3C3C3DFFFFFFFFFFFFFFFFFFFFF
      FFFF2424246000000000000000000000000000000000646464A0FFFFFFFFFFFF
      FFFFE0E0E0EF191919500000000000000000000000000000000019191950E0E0
      E0EFFFFFFFFFFFFFFFFF646464A0000000000000000000000000000000001919
      1950FFFFFFFFFFFFFFFFFFFFFFFFC3C3C3DF0909093000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000909090C0FFFFFFFFFFFFFFFFA8A8A8CF0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000A8A8A8CFFFFFFFFFFFFFFFFF797979B0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000005656
      5694FFFFFFFFFFFFFFFFFBFBFBFD0707072D0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000707072DFBFBFBFDFFFFFFFFFFFF
      FFFF565656940000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000001010110C3C3C3DFFFFFFFFFFFFF
      FFFF1010104000000000000000000000000001010110FFFFFFFFFFFFFFFFFFFF
      FFFF191919500000000000000000000000000000000000000000000000001919
      1950FFFFFFFFFFFFFFFFFFFFFFFF010101100000000000000000000000001010
      1040FFFFFFFFFFFFFFFFC3C3C3DF010101100000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000009090930FFFFFFFFFFFFFFFFFFFFFFFF040404200000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000004040420FFFFFFFFFFFFFFFFFFFFFFFF101010400000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000ADADADD2FFFF
      FFFFFFFFFFFFE3E3E3F100000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000E3E3E3F1FFFF
      FFFFFFFFFFFFADADADD200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C4C4C4E0FFFFFFFFFFFF
      FFFF0101011000000000000000000000000019191950FFFFFFFFFFFFFFFFA8A8
      A8CF000000000000000000000000000000000000000000000000000000000000
      0000A8A8A8CFFFFFFFFFFFFFFFFF191919500000000000000000000000000101
      0110FFFFFFFFFFFFFFFFC4C4C4E0000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000646464A0FFFFFFFFFFFFFFFF646464A0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000646464A0FFFFFFFFFFFFFFFF515151900000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000E5E5E5F2FFFFFFFFFFFF
      FFFF929292C10000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000009292
      92C1FFFFFFFFFFFFFFFFE5E5E5F2000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFF
      FFFF0000000000000000000000000000000040404080FFFFFFFFFFFFFFFF5151
      5190000000000000000000000000000000000000000000000000000000000000
      0000646464A0FFFFFFFFFFFFFFFF404040800000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E1E1E1F0FFFFFFFFFFFFFFFF09090930000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000009090930FFFFFFFFFFFFFFFFC4C4C4E00000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000707072DFBFBFBFDFFFFFFFFFFFFFFFF3737
      3777000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000037373777FFFFFFFFFFFFFFFFFBFBFBFD0707072D00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFF
      FFFF0000000000000000000000000000000040404080FFFFFFFFFFFFFFFF5151
      5190000000000000000000000000000000000000000000000000000000000000
      0000646464A0FFFFFFFFFFFFFFFF404040800000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000009090930FFFFFFFFFFFFFFFFE0E0E0EF00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E0E0E0EFFFFFFFFFFFFFFFFF0909
      0930000000000000000000000000000000000000000000000000000000000000
      0000000000000000000037373777FFFFFFFFFFFFFFFFFBFBFBFD0707072D0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000001B1B1B54FBFBFBFDFFFFFFFFFFFFFFFF37373777000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C3C3C3DFFFFFFFFFFFFF
      FFFF0101011000000000000000000000000019191950FFFFFFFFFFFFFFFFA9A9
      A9D0000000000000000000000000000000000000000000000000000000000000
      0000A9A9A9D0FFFFFFFFFFFFFFFF191919500000000000000000000000000101
      0110FFFFFFFFFFFFFFFFC3C3C3DF000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000010101040FFFFFFFFFFFFFFFF909090C000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000909090C0FFFFFFFFFFFFFFFF1010
      1040000000000000000000000000000000000000000000000000000000000000
      000000000000929292C1FFFFFFFFFFFFFFFFE3E3E3F10707072D000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000707072DE3E3E3F1FFFFFFFFFFFFFFFF929292C10000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000001010110C4C4C4E0FFFFFFFFFFFF
      FFFF1010104000000000000000000000000001010110FFFFFFFFFFFFFFFFFFFF
      FFFF191919500000000000000000000000000000000000000000000000001919
      1950FFFFFFFFFFFFFFFFFFFFFFFF010101100000000000000000000000001010
      1040FFFFFFFFFFFFFFFFC4C4C4E0010101100000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000040404080FFFFFFFFFFFFFFFF5151519000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000051515190FFFFFFFFFFFFFFFF3131
      3170000000000000000000000000000000000000000000000000000000000000
      0000D6D6D6EAFFFFFFFFFFFFFFFFABABABD10000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000ABABABD1FFFFFFFFFFFFFFFFD6D6
      D6EA000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000009090930C4C4C4E0FFFFFFFFFFFFFFFFFFFF
      FFFF2424246000000000000000000000000000000000646464A0FFFFFFFFFFFF
      FFFFE1E1E1F0191919500000000000000000000000000000000019191950E1E1
      E1F0FFFFFFFFFFFFFFFF646464A0000000000000000000000000000000002424
      2460FFFFFFFFFFFFFFFFFFFFFFFFC4C4C4E00909093000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000040404080FFFFFFFFFFFFFFFF4040408000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000040404080FFFFFFFFFFFFFFFF4040
      40800000000000000000000000000000000000000000000000000707072DF7F7
      F7FBFFFFFFFFFFFFFFFFC4C4C4E0373737770000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000037373777C4C4C4E0FFFFFFFFFFFF
      FFFFF7F7F7FB0707072D00000000000000000000000000000000000000000000
      0000000000000000000009090930E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFF2424
      2460000000000000000000000000000000000000000001010110C3C3C3DFFFFF
      FFFFFFFFFFFFFFFFFFFFA9A9A9D05151519051515190A9A9A9D0FFFFFFFFFFFF
      FFFFFFFFFFFFC3C3C3DF01010110000000000000000000000000000000000000
      000024242460FFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F009090930000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000031313170FFFFFFFFFFFFFFFF5151519000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000051515190FFFFFFFFFFFFFFFF3131
      3170000000000000000000000000000000000000000037373777FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFBFBFBFDF7F7F7FBF7F7F7FBF7F7F7FBF7F7F7FBC4C4C4E0C4C4C4E01B1B
      1B54000000000000000000000000000000000000000000000000000000000000
      00001B1B1B54C4C4C4E0C4C4C4E0F7F7F7FBF7F7F7FBF7F7F7FBF7F7F7FBFBFB
      FBFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFBFBFBFD37373777000000000000000000000000000000000000
      00000000000024242460FFFFFFFFFFFFFFFFFFFFFFFFE0E0E0EF101010400000
      000000000000000000000000000000000000000000000000000009090930C3C3
      C3DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFC3C3C3DF0909093000000000000000000000000000000000000000000000
      00000000000010101040E0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFF242424600000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000010101040FFFFFFFFFFFFFFFF909090C000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000909090C0FFFFFFFFFFFFFFFF1010
      1040000000000000000000000000000000000707072DADADADD2C3C3C3DFC3C3
      C3DFD4D4D4E9F7F7F7FBF7F7F7FBF7F7F7FBF7F7F7FBFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE5E5
      E5F2000000000000000000000000000000000000000000000000000000000000
      0000E5E5E5F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7FBF7F7F7FBF7F7F7FBF7F7F7FBD4D4
      D4E9C3C3C3DFC3C3C3DFADADADD20707072D0000000000000000000000000000
      0000000000008F8F8FBFFFFFFFFFFFFFFFFFFFFFFFFF09090930000000000000
      0000000000000000000000000000000000000000000000000000000000000101
      01106363639FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6363
      639F010101100000000000000000000000000000000000000000000000000000
      0000000000000000000009090930FFFFFFFFFFFFFFFFFFFFFFFF8F8F8FBF0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000009090930FFFFFFFFFFFFFFFFE1E1E1F000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E1E1E1F0FFFFFFFFFFFFFFFF0909
      0930000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000707072D53535392535353925353539253535392FFFFFFFFFFFF
      FFFF0707072D0000000000000000000000000000000000000000000000000707
      072DFFFFFFFFFFFFFFFF535353925353539253535392535353920707072D0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000009090930FFFFFFFFFFFFFFFFFFFFFFFF10101040000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000010101101818184F3F3F3F7F3F3F3F7F1818184F010101100000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040FFFFFFFFFFFFFFFFFFFFFFFF090909300000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C3C3C3DFFFFFFFFFFFFFFFFF09090930000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000009090930FFFFFFFFFFFFFFFFC3C3C3DF0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F1F1F1F8FFFF
      FFFF929292C10000000000000000000000000000000000000000000000009292
      92C1FFFFFFFFF1F1F1F800000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000646464A0FFFFFFFFFFFFFFFFA9A9A9D0000000000000
      0000000000000909093051515190515151900000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000005151519051515190090909300000
      00000000000000000000A9A9A9D0FFFFFFFFFFFFFFFF646464A0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000051515190FFFFFFFFFFFFFFFF646464A0000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000646464A0FFFFFFFFFFFFFFFF515151900000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000757575ADFFFF
      FFFFF7F7F7FB000000000000000000000000000000000000000000000000F7F7
      F7FBFFFFFFFF757575AD00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000001010110E0E0E0EFFFFFFFFFFFFFFFFF404040805151
      5190C4C4C4E0FFFFFFFFFFFFFFFFFFFFFFFF909090C001010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000001010110909090C0FFFFFFFFFFFFFFFFFFFFFFFFC4C4
      C4E05151519040404080FFFFFFFFFFFFFFFFE0E0E0EF01010110000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000010101040FFFFFFFFFFFFFFFFFFFFFFFF040404200000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000004040420FFFFFFFFFFFFFFFFFFFFFFFF101010400000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FDFD
      FDFEFFFFFFFF1B1B1B54000000000000000000000000000000001B1B1B54FFFF
      FFFFFDFDFDFE0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000031313170FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F0242424600101
      0110000000000000000000000000000000000000000000000000000000000000
      00000101011024242460E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3131317000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000787878AFFFFFFFFFFFFFFFFF909090C00000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000A9A9A9D0FFFFFFFFFFFFFFFF787878AF000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000D6D6
      D6EAFFFFFFFFD6D6D6EA00000000000000000000000000000000D6D6D6EAFFFF
      FFFFD6D6D6EA0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C3C3C3DFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFE0E0E0EF6363639FE0E0E0EFFFFFFFFFFFFFFFFFFFFFFFFFE1E1
      E1F0515151900101011000000000000000000000000000000000010101105151
      5190E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFFE0E0E0EF6363639FE0E0E0EFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFC3C3C3DF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000004040420FFFFFFFFFFFFFFFFFFFFFFFF6464
      64A0000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000646464A0FFFFFFFFFFFFFFFFFFFFFFFF04040420000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000001B1B
      1B54FFFFFFFFFFFFFFFF00000000000000000000000000000000FFFFFFFFFFFF
      FFFF1B1B1B540000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000010101040FFFFFFFFE0E0E0EF6363
      639F1818184F0000000000000000040404208F8F8FBFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF313131700000000000000000000000000000000031313170FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8FBF0404042000000000000000001818
      184F6363639FE0E0E0EFFFFFFFFF101010400000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000024242460FFFFFFFFFFFFFFFFFFFF
      FFFF646464A00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000646464A0FFFFFFFFFFFFFFFFFFFFFFFF2424246000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F7F7F7FBFFFFFFFF757575AD0000000000000000757575ADFFFFFFFFF7F7
      F7FB000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000F0F0F3F000000000000
      00000000000000000000000000000000000000000000101010408F8F8FBFFFFF
      FFFFFFFFFFFF646464A000000000000000000000000000000000646464A0FFFF
      FFFFFFFFFFFF8F8F8FBF10101040000000000000000000000000000000000000
      000000000000000000000F0F0F3F000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000006363639FFFFFFFFFFFFF
      FFFFFFFFFFFFA9A9A9D004040420000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000004040420A9A9
      A9D0FFFFFFFFFFFFFFFFFFFFFFFF6363639F0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000929292C1FFFFFFFFF1F1F1F80000000000000000F1F1F1F8FFFFFFFF9292
      92C1000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000010101040FFFF
      FFFFFFFFFFFFA9A9A9D000000000000000000000000000000000A9A9A9D0FFFF
      FFFFFFFFFFFF1010104000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000006363639FFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFF646464A00909093000000000000000000000
      00000000000000000000000000000000000009090930646464A0FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF6363639F000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000707072DFFFFFFFFFFFFFFFF0707072D0707072DFFFFFFFFFFFFFFFF0707
      072D000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFF0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000002424
      2460FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE1E1E1F0909090C05151
      51904040408051515190909090C0E1E1E1F0FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF2424246000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E5E5E5F2FFFFFFFFADADADD2ADADADD2FFFFFFFFE5E5E5F20000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000A8A8
      A8CFFFFFFFFFFFFFFFFF646464A04040408040404080646464A0FFFFFFFFFFFF
      FFFFA8A8A8CF0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000004040420787878AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7878
      78AF040404200000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000037373777FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF373737770000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000006464
      64A0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF646464A00000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000101010405050508FC3C3C3DFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3C3C3DF5050508F101010400000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FDFDFDFEFFFFFFFFFFFFFFFFFDFDFDFE000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000003131
      3170FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF313131700000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000090909300F0F0F3F3030
      306F3F3F3F7F3030306F0F0F0F3F090909300000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000ADADADD2FFFFFFFFFFFFFFFFADADADD2000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000404
      04203F3F3F7F3F3F3F7F3F3F3F7F3F3F3F7F3F3F3F7F3F3F3F7F3F3F3F7F3F3F
      3F7F040404200000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000707072DFFFFFFFFFFFFFFFF0707072D000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000F1F1F1F8F1F1F1F800000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000757575AD757575AD00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      28000000C0000000600000000100010000000000000900000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000}
  end
  object scGPVirtualImageList1: TscGPVirtualImageList
    ColorDepth = cdDefault
    PixelOffsetMode = scgppomDefault
    Width = 24
    Height = 24
    DefaultWidth = 24
    DefaultHeight = 24
    SourceImageList = ImageList48
    Left = 575
    Top = 462
    Bitmap = {
      494C010106007400740018001800FFFFFFFF0500FFFFFFFFFFFFFFFF424D7600
      0000000000007600000028000000600000003000000001000400000000000009
      0000000000000000000000000000000000000000000000008000008000000080
      800080000000800080008080000080808000C0C0C0000000FF0000FF000000FF
      FF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000000000
      0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
      000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF424D3E000000000000003E000000
      2800000060000000300000000100010000000000400200000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000}
  end
end
