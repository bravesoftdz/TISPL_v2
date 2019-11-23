object Form_zpravy: TForm_zpravy
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Form_zpravy'
  ClientHeight = 338
  ClientWidth = 361
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
    Width = 361
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
    Color = 10114859
    Caption = 'scGPPanel_header'
    TransparentBackground = True
    StorePaintBuffer = True
    Sizeable = False
    WallpaperIndex = -1
    object Skryt: TscGPGlyphButton
      AlignWithMargins = True
      Left = 311
      Top = 0
      Width = 50
      Height = 33
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 1
      Align = alRight
      TabOrder = 0
      OnClick = SkrytClick
      Animation = True
      Caption = 'Skryt'
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
      Width = 274
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
      Caption = 'Zpr'#225'vy'
    end
    object scGPGlyphButton_info: TscGPGlyphButton
      Left = 274
      Top = 0
      Width = 37
      Height = 34
      Hint = 'P'#345'ipnout k prav'#233' '#269#225'sti '
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
      TabOrder = 2
      OnClick = scGPGlyphButton_infoClick
      OnResize = scGPGlyphButton_infoResize
      Animation = True
      Caption = 'P'#345'ipnout / Odepnout'
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
      Left = 23
      Top = 5
      Width = 24
      Height = 24
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
      RotationAngle = 0
      AnimationAcceleration = False
      RotateAnimation = False
      PngImage.Data = {
        89504E470D0A1A0A0000000D494844520000001800000018080200000118129A
        39000000097048597300002E2800002E2801F196167E0000064D4944415478DA
        4D547B389459189F331733C6A8C9BD9B1A145DD4BA855229955C7249A9166350
        D276DF8A5D9550D1BA75D33E6E3BCC52C2AAE45A11135B3C53B68D14615D6250
        0663C65CBFF9F67CA51EEF1FDF73CE77DEF3BEEF797FBFDF0B1C0EE4E270B829
        A90238ECCFC501B8C6010B7FF61F319E6A043C80A73E9B4D8B9FB683FB8D7DB6
        A67ACC8802F0F5C6844886AD2AAEFABA9E2C847B1487BB796A2BB876979753FA
        46AE541188043C1E4F5523605ED77FDE1274B17C9606F9D3B864A5910EF64B89
        A8DC1D4CFE1B1C6FEF156039AD98D97205329B46861BDD39D4A1513188C9AC6F
        7D37C058CE6868688B39E361CFD004D68139BC1C262257C82452994CD9322006
        2FDFF12B5E0ED6705BF778DBB20B9E6747B98335AC1CDB95F3FE691F4651342F
        D68379A174BA68A94CC94DF74BC86DAAE1F548E5CA057AB32259F61F4726AF70
        5EA89108C02E88539FE17FE04A5547CF281E0F505895993EAF8D0F1B41D3A038
        9A1BB40F08413CE7795943978ECEEC232E261738BC7BD7FC8E243EB19A4B2C7E
        D1CFE3045930B3D3C29D41C8A50AC184646854F43493E91D59EA69AEC5A9E9E4
        6605B2E29E1851E475ED8282386FACA633FEB639E52D4363D2FAB47D362C8E06
        858800D2B3D49D56417965C93E3E11F7A70B4754A8E922ADE4E34E1D7D02D826
        C63CBA4028098DAB249308B0F1D34ED0C452C54A235D4B337D991CA97CD12D91
        2AE0BBBE1E614E08A20A7433DF6869783EFD590F7F428D4858BB6AC1C5B0F55B
        8EDCC57F8110ACDBFFE79900BB12EE87F6BE513C00B86F3639252F49F4F18B7A
        08A0B99E28747358C26BE9ED1C9C8467A646BA43FCB1098952A54297317432CF
        BA400000FFF3E4868377DA0B82ED42F31185B291CDECEAE68724D5491468D32D
        9FD014EEB040043E8E08778617BFC8665907B069B368275D8DBDDDAC2C9939D6
        AB1996FAF8CCC79DC7F7DA80D6EE4F61F155217BEC1F3F695EB77ED5A3AA5721
        819B32D8D5E74EBA9F4D2C9D90221181F6003278D3A13B742DCD30A7453B5C2C
        AD03B2E7CDD70EB433F0F55A6315C8A192092E6B8D4159C387F369F5F0A57FA7
        EF1389245E672BE0B5BA54DFCE2EFE89B4C663BED649B79BC00FFEECE75901B0
        119CCBBB8A4A9A6A5B208D5469313E49A995BD63F2AB279D20DE583329646251
        9C97C3813C4D0D329180B54A2C512C67E85C39ECE8155EAC4E264EC322912993
        8E6F3635D4EAE81F2311F0CB8D74A233EA1B5B0749443C6E2676988850744AAA
        9CAB4BDBB365D926AB45744DF2F8A44C28964120A8641294C58458F6B28D9F5B
        D1DA35300EF343627EBFFB8D29088AE2D008A6BDC706936BF9BCA29AF7900A90
        04B042F0054F984389A00A2502D7DBED8DA100DE767F3E9BC6158A645FB98205
        12491487765A04ED30674697BFFE30A24BA7C0A8DF4D06F58DE2A0F7F7FC30B2
        502CD7A153F32F79BEEE18FA25954BA510B13972CA6F8DF53283E0F8EAE2A45D
        4038B62BAA4205A0B4704A25A2AD432FBAE2A98EC7E51637A53C68A3A993B016
        4EC977B95944EEB3387AF1A1F3E615741AF9444A3508882E4D8B7066C594BFEF
        F9ECB675757CB04D7246F5ED868F502FB0D2D863DB56EB934A9A478E7A98F986
        DFEB1D931170A8265DF37E82F7FB37DDA12975341AA5E0B2E7ADBF9AC1F5BB3C
        279BC5ACD832281C058ACF8CF69C4790EE8E2A9F94AB56982DE4FCBA2532B9B2
        AA75B4F2E6DEAEB7DD47531B543870C87F5DA8F3D2A073F75A07442A0481F80F
        09A640426EE36E27B31FCF3F842291C915361626BF9F581F7BE3517EE3E09D78
        1FF5A931D66FB57244E5ECB8223EC43632A5A2BE4B5475C3F779C3DB88AC260D
        753548BB9B67B6750F8C03C7B0DB85715E8979D848A2524862B92A21DCDD4035
        59FC72F802CBE660F483E6DE494825050AD8B1DEF889D1DA2E71F0E6C55EA78B
        857214CA79C942AD8CC8EDCC98326CE8191ACC86B1A2D29F95D677AA930973E7
        EBE645BB5209B8B2C7FF46E5BE826931EC64CA55E68B324E6F82E44B6173F36A
        BB212D9633B4D9E7DD22526BB9CDFD18FC50C8F02F7CAA8136EDA784477D4342
        E3855A1A6402FF93500A2BF9463AE8A6A74DD320E3DFF508E0708F3FEC68A8AF
        B9FF72A5402885E49E2624749629101291702E782DEC7D5D735F61F5BBCEFE31
        48EBEF84820F37D49FB563BD89E7C6A52D9D9FA2B3EA4704535432119DC9EC99
        068737FCEACDD1B03233309A4F57A710E1160EC65EBEF075C770FF8810CA80A2
        469C31C030FB1FC18A0F12479764DE0000000049454E44AE426082}
    end
    object scGPImage_error: TscGPImage
      Left = 1
      Top = 5
      Width = 24
      Height = 24
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
      ClipFrameWidth = 2
      RotationAngle = 0
      AnimationAcceleration = False
      RotateAnimation = False
      PngImage.Data = {
        89504E470D0A1A0A0000000D494844520000001800000018080200000118129A
        39000000097048597300002E2800002E2801F196167E000005514944415478DA
        65556B50135714CECD2621D944DAB462B488154B44B1A3820FD2AA108BA9155A
        B5D8D15A4001AB059956ADEF0730427D8C7654C62245CB4304E3306A2D200105
        1418AD0AE888444B950046200AF208843C369BEDD96C48637B7FEDDEFBED39DF
        F79D73EEA2F9EBCF19CD56D246A18F62CFDECE5A235B978702A2721AF2A2E30E
        A9109CCA3EF4BCD3D481181C8BC5A29F664F1D5755DF463FD56446C862F358F4
        3EDA17F3313AAEBC77A9AAB9F674644199BA50D5D86B2469544E52D8EA7D7FF0
        791CBDC132433A86DEA228CACFDBE3F9CB018391A0BF9E119165A328018F032F
        0051B7F6A09FF3EF9E2F570F9BAC420157E0C6B97C241CCD599B4BDA6CDFAF9C
        CDE3621997EE1F880F46B50FB49B8E5D9F3BCD93CFC3EA9E7465EE5C8CE644E7
        4EF31EFDEC451F2439B367C97787CAE88C1078689850A5ADBCF5509B5FF6D86C
        21BD24A3F66F585050AEBE58F9170763D30E5C3DB1521E779E83A186BC18D6C8
        2A2A7E70B4581D1430A1ADB31FED3F535B59DF6EB351C32602407362F2847C9A
        B40D3CB4A38F6D0E41F187CB5E740FF6E94D0C28604D8E90CF75C6035AE7F67F
        4173DA161978B6E4D1AB3EC3244F31EC33C70441B6740E285397452415398893
        A46DCAC4D127B7299A5ABACD04F9C178719FDE189B5A0A9A29A624CCA790EE1D
        77C14C5F09B8DAF0A40B44B1D98839A241E0B03C60C2F628595266CD336D1F97
        832D0BF2592EF75DB2A990C7653B40AB17FB75760F5DBFDBCA7259507B688E85
        F1055044B42841F9F5A77EB9258DC1813EA9B132E4C8C09A15994DB25875B96B
        E7ADCF479A8EBE2F775C1E85F3E43229D73C547447CBB22BC4DD68B7A64BC73C
        7DDE8BBA7A0657ECFA9D8D108044944955DFC14402BD8C9A8D5F0520B5A61B4A
        08EF000A9B35AEA56B908E64B5A65F69E4B0118076AC912183D11292A004C100
        C2496369DD0B57FA50AEA5415254FEA766E72F3745381740D3C6F2EF36F7C019
        64AF7BDC09CC362C9F79EAE27DE41F955D75EA9B908DCAD1625C221638DC43A8
        B5A31FC3D8477F58B8F95805ED9308E765ED0D0DDB52880BB86CBB0716821C2F
        71CF490C5D187F1EE7731D6581064EDBBA48EA256E6E7FCDE16033A563F6FD5A
        73BBB183CB1971DC9529C8018B1581DEE1F2C9EF79887078C1106557603411AF
        FA8755B73545354FFB07CD30154EE7FF0D042526ACB6F815FE319F4FCF57359D
        BDDA04DD05D3C261B359236892A4A089DC85BC159FF86E5A35BBE25E5B4AD62D
        B803C03D472060B174813479FDFCDDA7AA4B6A9FB98B7822DC4D22C69D3DEA5C
        D05F0303C66EBD69C84840517393C2724B1E9D2C6C10013BFFC8ECB8707F45E0
        C4C8A462060A7143E57EA9B181174A1F563F7A09EDEB0C846148F752AFD1E931
        3B8BC1614BDA8F0AC24AEE4ABF8956EDB992B9FBB3B8C3659ACE7EF083B18909
        74F0F48DC29A56D7E96232399F612CE1B42075295C27F4F5A498EB1D9D52C2B7
        5F34AE810CC316A385743A0A8363351AB71C2D7FDA6BE161C80986E9D6F51AD0
        917377A04091C9C53061CC37CE40C9E91505552DA0DF851012D23DF24695D3B6
        2ADA7503481E5700DC4E28EB2AEBDA70BB0A67A0948C4AE50D8D50F0A63484DC
        465C837EF3F112A76F5FFCED8152FAD29BEC25CE4E0CDB93510D1575C7790613
        B12478EA4FEB6414C572ED1466B569BBA353CB080A992DD6499E6F672786A6FC
        760B48D0E507CF20CFF12D211E623CE1C8B5AED74370315BED33F9FF05A30147
        40F37082DC4BE2BEE1A0AA576F82E676342424361124F89DB86E9E62EEC4AAFA
        F60BD79EFCADEDD51BCC4E4E20FCFDB16F2D0B962E0F9EACD6F4ECCDA8068F85
        6E1CCAB5B35D97C9620551927785B3A68CF5192FE6DBEF14B0E3B94E7FBF59A7
        D5E9AD2405BF8CFFA8FE075A9B9E6CB4E856220000000049454E44AE426082}
    end
  end
  object scGPListBox_zpravy: TscGPListBox
    Left = 0
    Top = 34
    Width = 361
    Height = 276
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
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
    SelectionOptions.Color = clHighlight
    SelectionOptions.ColorAlpha = 220
    SelectionOptions.FillStyle = scgpsfColor
    SelectionOptions.GradientAngle = 90
    SelectionOptions.FocusedColor = clHighlight
    SelectionOptions.FocusedColorAlpha = 255
    SelectionOptions.FocusedFillStyle = scgpsfColor
    SelectionOptions.FocusedGradientAngle = 90
    SelectionOptions.FontColor = clHighlightText
    SelectionOptions.FocusedFontColor = clHighlightText
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
    ItemHeight = 30
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
    Width = 361
    Height = 28
    Align = alBottom
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
    TransparentBackground = False
    StorePaintBuffer = True
    Sizeable = False
    WallpaperIndex = -1
    object RzStatusPane_pocet_chyb_value: TRzStatusPane
      Left = 73
      Top = 0
      Width = 26
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
      Caption = '0'
      ExplicitLeft = 81
    end
    object RzStatusPane__chyby_caption: TRzStatusPane
      Left = 0
      Top = 0
      Width = 73
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
      Alignment = taCenter
      BlinkColor = 10114859
      Caption = 'Po'#269'et chyb'
    end
    object RzStatusPane_var_header: TRzStatusPane
      Left = 99
      Top = 0
      Width = 88
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
      Alignment = taCenter
      BlinkColor = 10114859
      Caption = 'Po'#269'et varov'#225'n'#237
      ExplicitLeft = 113
    end
    object RzStatusPane_pocet_var_value: TRzStatusPane
      Left = 187
      Top = 0
      Width = 26
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
      Caption = '0'
    end
    object scGPSizeBox: TscGPSizeBox
      Left = 335
      Top = 0
      Width = 26
      Height = 28
      Cursor = crSizeNWSE
      Align = alRight
      TabOrder = 0
      GlyphColor = clWhite
      GlyphColorAlpha = 200
      GlyphThickness = 1
    end
  end
end
