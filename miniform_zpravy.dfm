object Form_zpravy: TForm_zpravy
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Form_zpravy'
  ClientHeight = 338
  ClientWidth = 360
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
  object scGPPanel_header: TscGPPanel
    Left = 0
    Top = 0
    Width = 360
    Height = 34
    Align = alTop
    FluentUIOpaque = False
    TabOrder = 0
    BlurBackground = False
    BlurBackgroundAmount = 5
    BackgroundStyle = gppbsColor
    ContentMarginLeft = 0
    ContentMarginRight = 0
    ContentMarginTop = 0
    ContentMarginBottom = 0
    CustomImageIndex = -1
    DragForm = False
    DragTopForm = True
    DrawTextMode = scdtmGDIPlus
    FillGradientAngle = 90
    FillGradientBeginAlpha = 255
    FillGradientEndAlpha = 255
    FillGradientBeginColorOffset = 25
    FillGradientEndColorOffset = 25
    FrameWidth = 0
    FillColor = 10708284
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
    Color = 10708284
    Caption = 'scGPPanel_header'
    TransparentBackground = True
    StorePaintBuffer = True
    Sizeable = False
    WallpaperIndex = -1
    object Skryt: TscGPGlyphButton
      AlignWithMargins = True
      Left = 310
      Top = 0
      Width = 50
      Height = 33
      Hint = 'Skr'#253't'
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      ParentShowHint = False
      FluentUIOpaque = False
      ShowHint = True
      TabOrder = 0
      OnClick = SkrytClick
      Animation = True
      Badge.Color = clRed
      Badge.ColorAlpha = 255
      Badge.Font.Charset = DEFAULT_CHARSET
      Badge.Font.Color = clWhite
      Badge.Font.Height = -11
      Badge.Font.Name = 'Tahoma'
      Badge.Font.Style = [fsBold]
      Badge.Visible = False
      Caption = 'Skryt'
      CanFocused = False
      CustomDropDown = False
      DrawTextMode = scdtmGDI
      FluentLightEffect = False
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
      Options.NormalColorAlpha = 200
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
      Options.FrameNormalColorAlpha = 200
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
      Options.ArrowNormalColor = clBtnText
      Options.ArrowHotColor = clBtnText
      Options.ArrowPressedColor = clBtnText
      Options.ArrowFocusedColor = clBtnText
      Options.ArrowDisabledColor = clBtnText
      Options.ArrowNormalColorAlpha = 200
      Options.ArrowHotColorAlpha = 255
      Options.ArrowPressedColorAlpha = 255
      Options.ArrowFocusedColorAlpha = 200
      Options.ArrowDisabledColorAlpha = 125
      Options.StyleColors = True
      Options.PressedHotColors = False
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
      GlyphOptions.Kind = scgpbgkUpArrow
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
    object scLabel_header: TscLabel
      Left = 0
      Top = 0
      Width = 273
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
      FluentUIOpaque = False
      TabOrder = 1
      OnMouseDown = scLabel_headerMouseDown
      OnMouseMove = scLabel_headerMouseMove
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
      Caption = 'Zpr'#225'vy'
    end
    object scGPGlyphButton_pripnout: TscGPGlyphButton
      Left = 273
      Top = 0
      Width = 37
      Height = 34
      Hint = 'P'#345'ipnout k prav'#233' '#269#225'sti '
      Align = alRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      ParentShowHint = False
      FluentUIOpaque = False
      ShowHint = True
      TabOrder = 2
      Visible = False
      OnClick = scGPGlyphButton_pripnoutClick
      OnResize = scGPGlyphButton_pripnoutResize
      Animation = True
      Badge.Color = clRed
      Badge.ColorAlpha = 255
      Badge.Font.Charset = DEFAULT_CHARSET
      Badge.Font.Color = clWhite
      Badge.Font.Height = -11
      Badge.Font.Name = 'Tahoma'
      Badge.Font.Style = [fsBold]
      Badge.Visible = False
      Caption = 'P'#345'ipnout / Odepnout'
      CanFocused = False
      CustomDropDown = False
      DrawTextMode = scdtmGDI
      FluentLightEffect = False
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
      Options.ArrowNormalColor = clBtnText
      Options.ArrowHotColor = clBtnText
      Options.ArrowPressedColor = clBtnText
      Options.ArrowFocusedColor = clBtnText
      Options.ArrowDisabledColor = clBtnText
      Options.ArrowNormalColorAlpha = 200
      Options.ArrowHotColorAlpha = 255
      Options.ArrowPressedColorAlpha = 255
      Options.ArrowFocusedColorAlpha = 200
      Options.ArrowDisabledColorAlpha = 125
      Options.StyleColors = True
      Options.PressedHotColors = False
      GlyphOptions.NormalColor = clWhite
      GlyphOptions.HotColor = 16744448
      GlyphOptions.PressedColor = clWhite
      GlyphOptions.FocusedColor = 16744448
      GlyphOptions.DisabledColor = 16744448
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 150
      GlyphOptions.Kind = scgpbgkPin
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
    object scGPImage_warning: TscGPImage
      Left = 34
      Top = 2
      Width = 30
      Height = 30
      FluentUIOpaque = False
      TabOrder = 3
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
      Caption = 'scGPImage_warning'
      StorePaintBuffer = False
      ImageIndex = -1
      InterpolationMode = scgppimDefault
      ClipFrame = scgpcfNone
      ClipFrameRadius = 10
      ClipFrameFillColor = clWindow
      ClipFrameFillColorAlpha = 255
      ClipFrameColor = clBtnFace
      ClipFrameWidth = 2
      DrawTextMode = scdtmGDI
      RotationAngle = 0
      AnimationAcceleration = False
      RotateAnimation = False
      PngImage.Data = {
        89504E470D0A1A0A0000000D494844520000001E0000001E0802000000B45239
        F5000000097048597300002E2800002E2801F196167E0000086F4944415478DA
        8556095453D716CDCB7B2F1301145189322852994429A3E0841F149149C5918A
        1F145975E02BC52A2820A051B14BA8A22DE8B7A038A0A2604150CB3C08A8D0A2
        1241661904412012C8F45EF24F0845FEFF74F5AE24EBE6BD7BF73D779F73F639
        C8D2DD2994C98694202552992A8B365F4FC35057C34047438589532872A18868
        E91AAC6FEBE7B57EE20B44340CA5E1E8A408C8FF439332B95842E869A93B58EA
        6E5869A83D4355F99C2065F08BA154E5DF0F7D828CA286A2EAF78D1D0338461D
        7FFE97D04231A1C2C0777A2CDCEEB200A5223DFDC34F2B5B9FD776B6F70C49A4
        242CC03194A3C9B631E1AC5E3C57796A46D1BB8BF7AA063E8B580CFC2FA10157
        7FF694606F1BDB05B36A9BFBB24A1B73CA9BE1D628556114828C2D230819DC40
        4D85EE68ADB776898195B1162C3E9B52F1A6A97722FA1834EC1A124A161ACCF8
        E9B033707AF371EDA57B5570122CA55211CA644326938F88093872C7DA05819B
        2D451222282EAFE27527B8473E111AC89DA73D35F6A0E3F429ACB3372AD2F2EB
        193485A194BF1B32B97C44285D63A77FE49F76B0FCD0F9FCEAFA6E261D1F8386
        D724294F0C59B368FE8CC82BA569F97553D80CE4EF61BF0C20DAD96EEEB9038E
        2D5D7CBFE82CF00A0A7601B44028DDBFC96297C7A2D4A7BCE8AB6570268386FE
        0F0FA44C06B1A88C101A46954F78259753800DF8ECF5B2D8BBD1E25169D3F12B
        254C3A86D8FA25EB6AA9DF88728703779FCA31D19F0EF4D6B5F40D89A4E38E03
        5A555519863A5361DEDD3BD4DE2B18E74A4E91A354D4CC60BA8C245FBEED3E1F
        BCCACE6CB63F37A7A6A10759F4CDD56F377C0D9F88C4E2F492A6B4B39B0C67B1
        63AE14A616B7B09963EE160825BEEB2C0F782D82794EC9BB6397CB54FE7C2591
        1206FA5A37C3D7F4F67D760DBEBF78A1F685E055F7F2EAB849CF107BFF94E408
        57F0DE9663199005FBB7DB07AC357E5AF236F472398B4953EE1F1649B9FBFEE1
        62AB073C3434F5F89D7A8C5051E5958685D2AD6E5F1FDE625E54D9F0DDC51226
        83763DD24D9D4DDF16F610F18DCE4A0A774DCBAB3B99F40C485C60A4FD4B8853
        4FCF80774496504601003978998ADEE4AE4349E9A0486EA6ABEA139E51DF354C
        C715E9372226CF1D5AE36036F35462415A592B50B7D37DE1BFB6581D8E2F4062
        AE971FF659FC5D5C5EF11FED741CC568F8F5684F3D0DFAEE139935AD83F004E2
        D278FEECE4A3AB32F3DEF03E4A42B65924DC2E4FCCAE03BA48481C75762AD783
        4121BCC31E760D8A011A042725CA3D25E70DF2A0A07EDD8AF95E471E74F50940
        0A205A8E05386C5C36373AB130A3AC05925E302209D8BA788F9B49D88F4FDEF4
        4A32B8EEB5751DFE6772511A26161336E67A97825656D5B4EE892DA0D1718086
        2C4B3FBB81D7DC87E43E6F5961A1EB1E9C36382482C002EE36AD360DF5B1B9FA
        E0E5C507AFC05D24424D8EF29CA38E6D0C49EF1E26AF457B1A69B17684673474
        0F4BA5C46667B390EDD609775F2466BE663369008D61E81DAE473F5F84E4BE68
        5D6EAEE371680C5A24262CCD74120E39565637079E2F421064DEDC99B78EBB54
        5435075E289292F2833E4B7C9D0D23130A7F7DD602117D64D7B2AD0E0647E3F3
        9EBC6C67D13180C67134F5A4E727FE08925DD60489E4197CBF8F3F02D032998C
        C660A49E5EA74625368664B4F70BBD9C4C8EFBD9C5DFAEFC25BB964A415C5718
        46FBDBDFF8F5F71F52ABD9AACC9493EB75D5A89B42333EF025388A28A1D3CEAC
        EFFC3884FC7CBF1A82DA9F9BFDAAA1974E4347438D88DAE7E86EAB1314939D53
        D51916E0B0DDE9ABEF639F14D47C804C319833E37A844B73F3876FA272F4F539
        B7225D7EAF69DB1B978FD37044514064B334D96931EB334B1A9183B1B971418E
        20B8FF7E5803A2A58827A1D46999614C807D4AFA8B73E9BCF4D8CD1C16056EF0
        698440110A2947AE46799A70589E41772DADE647F858C62697A4E4352AF30B5C
        B5D67EDEC93DCBCF5C2B479CF6DD4EE57AF205E21D915990B7402EC8B18686EA
        DD539E6FEBDAE3D279378FBB14573404FF54CA6028374B02B62CFED6CD24F252
        BE8DADA18BF94CEFB08CE68F2390134AC58F3FB4CACA98B3155206123D7CD712
        AF95867B639E3CE77D005981152289EC72849B069D92F9A273FF3A336E42FEFD
        676D2AA3D01070A646B3934257E557341A1B690FF7F37D4EE4A038066C88A5E4
        573A536F447994BDEA38702E17B1DA910435E5E2F7AB9F56B61CBE50A0AA421B
        150D69D8EEE5AEB6BA5DFD228E3AB6EDD8C3EECF52F092428F64720A8A5D8B72
        E7A8E32C26FD51717DF895674A4981180BF5B5F376363DF673D1E3F26664897F
        0A5423A01B142BE452E16F952D10F340B7BBA369A4AF0D6CA8AEEDD8F3432E4E
        C3C6451624E574A0A3B3B52ECCE3AE95A6E4360034506169A4056CB4757FDECD
        CD067F2AF45A42C834D599B74E784084F81CCF6C6C1F60D0314D4DF5B4D39E74
        2A253DB7363AB992CDA28D0B34E4E74E2FABC0F50B2932624B487ADB27A14442
        EA6AA9DD3CE101F9EC17FDE8DDFB7E285263050C3CEBB6D420326069C3FB81E8
        ABA5BC963E15267DA9B9CE5436ADB6B1E75D271F9FD00B80744CD754B533E508
        04C2E23F3A0422628E965AA8AFBDB50927F6D673500FF6A8647EA9E89F87C5BE
        AE6641DE364323122871E5AF3B810114A38E363154F984BA828CF62422290965
        0F86A9BE665C9093D63495A4AC57F1775EB21834A5DE7E818645B0016C3FB8D5
        1AB8BBF1B836BDF05D73C720344718A62838C85859517CA524D43312FA10F765
        063E2E0BE838F663EA8B3BBFF12015C76BD37FF521601A7F586C6BCA39EA67AF
        3F6B0A6CBEFD9497FFB2AD03FA1B3856A680859A098442CA3958EA6D5B6D0246
        401B743AB91C96A9A980BD5F2AEA248D1914500892A58BB4A181329E334D1955
        03432282544083555044A6A93361DED43978FB09AFB0BA6D7048AC4C88890399
        B49D040347FB1BCCC2486B99B90EB0A93985A5F42458DFC717420C1456B5419D
        05FF43384DDAB1FC07C893208CBAA3C8F10000000049454E44AE426082}
    end
    object scGPImage_error: TscGPImage
      Left = 6
      Top = 2
      Width = 30
      Height = 30
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      FluentUIOpaque = False
      TabOrder = 4
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
      ClipFrameWidth = 0
      DrawTextMode = scdtmGDI
      RotationAngle = 0
      AnimationAcceleration = False
      RotateAnimation = False
      PngImage.Data = {
        89504E470D0A1A0A0000000D494844520000001E0000001E0802000000B45239
        F5000000097048597300002E2800002E2801F196167E000006114944415478DA
        9D566950535714CE23CBCB828130A865532945AB2801A4B80023EAE0B854C0A8
        1D47B0158A5A712C5A815A172C52A83AE352B78A0A2276510B82A8A82D081844
        507610294154761105C9FE96BC9E100A3109DAE99DFC79EFE57EF7DCEF7CE73B
        07F1597781666AE184468D932893EE3076D4877602273B4B944587F73205DEDC
        D1FBA4ADAFA34706FF61B3E80CBA994904C4189A24352A8C74B4B558E637D1E3
        E30FA6385A1B6FD3505455E38BF2C75D19F97FBFEC55B0590C3333E43DD0184E
        7250A668EEA47541420ECAE87A25AF69EA2EAA6A6BEDEE871869148D8D32E054
        1F37FB694E63ACF8ECD7FDCAE397CBFF2C7D466A3406E10F43C3A132256E3BDA
        FCF096F9CEE3AC9ADBFB8E5E2A2B6FE852A8708A46D346850CC64B92143C9973
        59BE42FBAF57798EB6E496D475C41CCB57AA090E4AA7282368B912F716DAC5AC
        99E930969F925D7D3EA74E2AC7386CC620A4D1223594528D8F11F022967B04CE
        71AE7FFA2A3EB948D2DA0B777D0B1A23486B0BEE991D8B20EA8494E2F4FC06A0
        6FA4FCBC452041C225C203855F89DC81FDAD87F35418A1DBA885069A580C7AF2
        AE254EF69609E78A2FFEF5D882874256705203DBDE816B464758743392A2640A
        6CF34ACFF020E1BDEAB62D8772511603AEAA85063D8505B86EFE6CFAB9EB3547
        2F96F1382CF8A05013CBFD5D167A3A00AD7433D3E157D4B59CC8AA052048801A
        231323E62C98E1B8E364E1ADFBCD3C0E1399117ADED6DAFC42DCD2977D8A2FE2
        AE834274D7E957E2D1A13E21F39C6934EA8D0C33261C411071A9644FEA0336CA
        D4496BB480FBCBDE008512FF3CEE9A4245206EC1C9D12133562F74D97A38B7B0
        A2154ED3ED942AF1C835B343174CBA2D6E884F7BC865334D6492D460A0C87F17
        A47DF5C2292084BD678BB20A25DAA8536397580BB8A2980C9DAA0CA06FE43FDA
        995C6A121AAEA2AF1F103E1070E580E8764933D082ACDC9E79293130F7C1B3ED
        270AF4F70F41E714D4EF4E310D6DB0288A2248EAECCEC5136CF8CBB76722DF1E
        CFDFB7C92FF6B4F8BAB869880D7DE8ABB9B5D1A78A8DA1816B1E9B699003A902
        DB2072DF28720FFF210749CAAC5C1FE416167FA3AEB907CCC8185AF2EC657E65
        3B93612812B51A4BBFD3A8C434FAF281829CE739FEC0E6B971678B90B49CDA35
        8BA606C76637B5F6B24C418F4C00B934F2728F0C0A04D187F676B5FB699BFFBE
        B492F743D73474648A9BF53F0DD041C3301C14A526287DCB03E8D9AE7647B7F9
        EF07E8E37F946F5AE1119E70B346D2AD736403E8ECDCBA9824D35C83C318C81D
        BC6CC14CC71F23FC769FBA8B7C7324EF60E4BCC4D4FBE9790D26D3F8DF15A24B
        63E42ACFD04F5DA1FA90C0A8F48C7DA2E2DA36709621D3FABFE2D3EA2FEDFBA5
        023E7BE57799C8276B53C1F026D85804466740B1D28D4AE65A9E96103016632C
        501E18E4D0236C1F6F6371393128ABA0716FF23DC42D24256285C7BA40E19ED3
        E26C7193B951A197563F3F77F331AA07318C851160D360DC3A75031B60ADF083
        5284824466859D1FC5437F8B0F80CFAB7666F7CBD53A09F72BF0982F7D83E77E
        F40E0694725540F41539A6BD2B68035A33D85C4B57FFDAF81BC08CD654A1BF04
        CD99181BEE7D4DDC1477A60845E19F08F88EFB641B97F15623E16A1B9E427DAB
        E4291437383EBC3916E5EF39D966E3FEDB0FEB3B216D5A688DF646C8A12DF3BD
        5C6C92AE549ECCA880BE07F060C1D044DE11354400E925C0FF70725798374C00
        D97725C0B24E0E830D8C20340C8659F2AEC513C7599DC9AA4ACAAC0268C33231
        B5546A82C9A447857889FC261594B740F3053E75F639DC76D518E1642F00EF86
        D903DA049452DB0B298C060CBA89C60B54EA662008252AD8CBCBC5F64ED9F383
        BF96F6BC51422F1C644C7F5880A6C5E7B1E2D6FBFABA39BC91A97FCEA8287DD4
        D1D923874EAAC51F3801404112E079D0A0FDA68FDBB0CC1DCEBE5A2849482D86
        CF607043CDD470C401E2801C28D64D2BA6C36678532DE9BE5FDBFEA4BD0FAE05
        8F207088D44768EFEC2080C7C696D7477E7F58FAA81326348301CAC460064505
        0386159F0323D2CCA9B6B3A6D9F179A8C17FA091DEAD6C2DABEF2CAA6E1B986C
        4CA81E19699C845B438A80036B4BAEFD985130547206DAAB54AE6E79216DEF96
        F64A55DA6A44479C81FE014CAD3724C664CC700000000049454E44AE426082}
    end
  end
  object scGPListBox_zpravy: TscGPListBox
    Left = 0
    Top = 34
    Width = 360
    Height = 276
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    FluentUIOpaque = False
    TabOrder = 1
    TabStop = True
    Touch.InteractiveGestures = [igPan, igPressAndTap]
    Touch.InteractiveGestureOptions = [igoPanSingleFingerHorizontal, igoPanSingleFingerVertical, igoPanInertia, igoParentPassthrough]
    OnMouseLeave = scGPListBox_zpravyMouseLeave
    OnMouseMove = scGPListBox_zpravyMouseMove
    CustomImageIndex = -1
    Options.FillColor = clWindow
    Options.FillColor2 = clWindow
    Options.FillColorAlpha = 255
    Options.FillColor2Alpha = 255
    Options.FillGradientAngle = 90
    Options.FrameColor = clBtnShadow
    Options.FrameColorAlpha = 255
    Options.FrameWidth = 2
    Options.FrameScaleWidth = False
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
    Color = clBtnFace
    TransparentBackground = True
    WallpaperIndex = -1
    AutoComplete = True
    DrawTextMode = scdtmGDI
    SelectionOptions.Color = clScrollBar
    SelectionOptions.ColorAlpha = 220
    SelectionOptions.FillStyle = scgpsfColor
    SelectionOptions.GradientAngle = 90
    SelectionOptions.FocusedColor = clScrollBar
    SelectionOptions.FocusedColorAlpha = 255
    SelectionOptions.FocusedFillStyle = scgpsfColor
    SelectionOptions.FocusedGradientAngle = 90
    SelectionOptions.FontColor = clHighlightText
    SelectionOptions.FocusedFontColor = clSkyBlue
    ShowLines = False
    LineColor = clWindowText
    LineColorAlpha = 20
    MouseMoveChangeIndex = False
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -11
    HeaderFont.Name = 'Tahoma'
    HeaderFont.Style = [fsBold]
    DetailFont.Charset = DEFAULT_CHARSET
    DetailFont.Color = clWindowText
    DetailFont.Height = -16
    DetailFont.Name = 'Tahoma'
    DetailFont.Style = []
    DetailPosition = scgplbdBottom
    DetailWordWrap = False
    OnItemClick = scGPListBox_zpravyItemClick
    Items = <>
    ItemIndex = -1
    ItemMargin = 5
    ItemWordWrap = True
    ItemShowEllipsis = False
    IndentMargin = 10
    CheckBoxOptions.NormalColor = clWindow
    CheckBoxOptions.DisabledColor = clWindow
    CheckBoxOptions.NormalColorAlpha = 200
    CheckBoxOptions.DisabledColorAlpha = 125
    CheckBoxOptions.FrameNormalColor = clWindowText
    CheckBoxOptions.FrameDisabledColor = clWindowText
    CheckBoxOptions.FrameNormalColorAlpha = 100
    CheckBoxOptions.FrameDisabledColorAlpha = 50
    CheckBoxOptions.CheckedNormalColor = clWindow
    CheckBoxOptions.CheckedDisabledColor = clWindow
    CheckBoxOptions.CheckedNormalColorAlpha = 200
    CheckBoxOptions.CheckedDisabledColorAlpha = 125
    CheckBoxOptions.CheckedFrameNormalColor = clWindowText
    CheckBoxOptions.CheckedFrameDisabledColor = clWindowText
    CheckBoxOptions.CheckedFrameNormalColorAlpha = 100
    CheckBoxOptions.CheckedFrameDisabledColorAlpha = 50
    CheckBoxOptions.FrameWidth = 2
    CheckBoxOptions.CheckMarkNormalColor = clWindowText
    CheckBoxOptions.CheckMarkDisabledColor = clWindowText
    CheckBoxOptions.CheckMarkNormalColorAlpha = 255
    CheckBoxOptions.CheckMarkDisabledColorAlpha = 125
    CheckBoxOptions.ShapeSize = 20
    CheckBoxOptions.CheckMarkThickness = 2
    CheckBoxOptions.ScaleCheckMarkThickness = True
    CheckBoxOptions.ScaleFrameWidth = False
    ShowCheckBoxes = False
    Images = Form1.scGPVirtualImageList1
    ShowItemDetails = False
    ItemSpacing = 5
    ItemHeight = 38
    HeaderOptions.Color = clBtnFace
    HeaderOptions.ColorAlpha = 220
    HeaderOptions.FillStyle = scgpsfColor
    HeaderOptions.GradientAngle = 90
    HeaderOptions.Margin = 5
    HeaderHeight = 20
  end
  object scGPPanel_statusbar: TscGPPanel
    Left = 0
    Top = 310
    Width = 360
    Height = 28
    Align = alBottom
    FluentUIOpaque = False
    TabOrder = 2
    BlurBackground = False
    BlurBackgroundAmount = 5
    BackgroundStyle = gppbsColor
    ContentMarginLeft = 0
    ContentMarginRight = 0
    ContentMarginTop = 0
    ContentMarginBottom = 0
    CustomImageIndex = -1
    DragForm = False
    DragTopForm = True
    DrawTextMode = scdtmGDI
    FillGradientAngle = 90
    FillGradientBeginAlpha = 255
    FillGradientEndAlpha = 255
    FillGradientBeginColorOffset = 25
    FillGradientEndColorOffset = 25
    FrameWidth = 0
    FillColor = 15592168
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
    Color = clActiveCaption
    TransparentBackground = False
    StorePaintBuffer = True
    Sizeable = False
    WallpaperIndex = -1
    object scGPSizeBox: TscGPSizeBox
      Left = 334
      Top = 0
      Width = 26
      Height = 28
      Cursor = crSizeNWSE
      Align = alRight
      BiDiMode = bdLeftToRight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      FluentUIOpaque = False
      TabOrder = 0
      Visible = False
      GlyphColor = clBlack
      GlyphColorAlpha = 200
      GlyphThickness = 1
    end
    object scGPGlyphButton2: TscGPGlyphButton
      AlignWithMargins = True
      Left = 0
      Top = 0
      Width = 3
      Height = 28
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      FluentUIOpaque = False
      TabOrder = 1
      TabStop = True
      Animation = False
      Badge.Color = clRed
      Badge.ColorAlpha = 255
      Badge.Font.Charset = DEFAULT_CHARSET
      Badge.Font.Color = clWhite
      Badge.Font.Height = -11
      Badge.Font.Name = 'Tahoma'
      Badge.Font.Style = [fsBold]
      Badge.Visible = False
      Caption = 'scGPGlyphButton1'
      CanFocused = True
      CustomDropDown = False
      DrawTextMode = scdtmGDI
      FluentLightEffect = False
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
      Options.NormalColorAlpha = 10
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
      Options.FrameNormalColorAlpha = 30
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
      Options.ArrowNormalColor = clBtnText
      Options.ArrowHotColor = clBtnText
      Options.ArrowPressedColor = clBtnText
      Options.ArrowFocusedColor = clBtnText
      Options.ArrowDisabledColor = clBtnText
      Options.ArrowNormalColorAlpha = 200
      Options.ArrowHotColorAlpha = 255
      Options.ArrowPressedColorAlpha = 255
      Options.ArrowFocusedColorAlpha = 200
      Options.ArrowDisabledColorAlpha = 125
      Options.StyleColors = True
      Options.PressedHotColors = False
      GlyphOptions.NormalColor = clBtnText
      GlyphOptions.HotColor = clBtnText
      GlyphOptions.PressedColor = clBtnText
      GlyphOptions.FocusedColor = clBtnText
      GlyphOptions.DisabledColor = clBtnText
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkDetailPoints
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
      ShowMenuArrow = True
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
    object scLabel_varovani: TscLabel
      Left = 112
      Top = 6
      Width = 96
      Height = 18
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      FluentUIOpaque = False
      TabOrder = 2
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
      Caption = 'Po'#269'et varov'#225'n'#237
    end
    object scLabel_chyby: TscLabel
      Left = 10
      Top = 6
      Width = 72
      Height = 18
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      Font.Quality = fqClearType
      ParentFont = False
      FluentUIOpaque = False
      TabOrder = 3
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
      Caption = 'Po'#269'et chyb'
    end
    object scGPGlyphButton1: TscGPGlyphButton
      AlignWithMargins = True
      Left = 105
      Top = 0
      Width = 3
      Height = 28
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alCustom
      FluentUIOpaque = False
      TabOrder = 4
      TabStop = True
      Animation = False
      Badge.Color = clRed
      Badge.ColorAlpha = 255
      Badge.Font.Charset = DEFAULT_CHARSET
      Badge.Font.Color = clWhite
      Badge.Font.Height = -11
      Badge.Font.Name = 'Tahoma'
      Badge.Font.Style = [fsBold]
      Badge.Visible = False
      Caption = 'scGPGlyphButton1'
      CanFocused = True
      CustomDropDown = False
      DrawTextMode = scdtmGDI
      FluentLightEffect = False
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
      Options.NormalColorAlpha = 10
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
      Options.FrameNormalColorAlpha = 30
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
      Options.ArrowNormalColor = clBtnText
      Options.ArrowHotColor = clBtnText
      Options.ArrowPressedColor = clBtnText
      Options.ArrowFocusedColor = clBtnText
      Options.ArrowDisabledColor = clBtnText
      Options.ArrowNormalColorAlpha = 200
      Options.ArrowHotColorAlpha = 255
      Options.ArrowPressedColorAlpha = 255
      Options.ArrowFocusedColorAlpha = 200
      Options.ArrowDisabledColorAlpha = 125
      Options.StyleColors = True
      Options.PressedHotColors = False
      GlyphOptions.NormalColor = clBtnText
      GlyphOptions.HotColor = clBtnText
      GlyphOptions.PressedColor = clBtnText
      GlyphOptions.FocusedColor = clBtnText
      GlyphOptions.DisabledColor = clBtnText
      GlyphOptions.NormalColorAlpha = 200
      GlyphOptions.HotColorAlpha = 255
      GlyphOptions.PressedColorAlpha = 255
      GlyphOptions.FocusedColorAlpha = 255
      GlyphOptions.DisabledColorAlpha = 100
      GlyphOptions.Kind = scgpbgkDetailPoints
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
      ShowMenuArrow = True
      ShowFocusRect = True
      Down = False
      GroupIndex = 0
      AllowAllUp = False
    end
  end
end
