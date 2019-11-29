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
      ShowHint = True
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
      ExplicitHeight = 40
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
      ExplicitHeight = 17
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
      ShowHint = True
      TabOrder = 2
      Visible = False
      OnClick = scGPGlyphButton_pripnoutClick
      OnResize = scGPGlyphButton_pripnoutResize
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
      Top = 1
      Width = 32
      Height = 32
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
        89504E470D0A1A0A0000000D4948445200000020000000200802000000FC18ED
        A3000000097048597300002E2800002E2801F196167E000009644944415478DA
        95567958CDE91EFFAD67EF4425DAA313250E290965228FC65589EC97B196B175
        8DB9858436A4B828C6FE18152AAE29FB3673534A298AF693A469A5CEA9D3D9CF
        6FBBEF1159AE793CF7FDEB77CEF77D3F9FF7FDAE1FD82B240DFADA62205847E1
        300CD9F03B9D858D2241CB507E0B1BA5808962905695C96B8D758DDCAEAED75E
        4BE11C5487C2F45771E0AF12E8681C98C60EAC9F6F73CFCBF42997AD821008FA
        1401315C0122A0B29ED1D7DBA6FEE7ADBB82E07051DDB709C0C55524C749D8B2
        66D865DFC105100650E03A856355AFA85A0E2E8BC1108320903DFFADB3B0412C
        ACE57195E054A3DCEE74C3FCDBED1339880E43A8BF24006F27687CA6C5E37F3A
        9D13F27ADAE443723B3DEEBF995CAFB056936CC36E98E93B4533300B21AD785D
        3EE6A55EA625E34C2BC1BFE98D41A94D81329D8087EA98FF2500E808CCFC34FC
        62B0ED2DF033AD71EEF9D7FE529D310E932C84F800FDD95B491AD5D3380653D3
        863C0D1F71D6942F95C844D155EBC085B8A89EF99400ECD652AC48E75FE7DADF
        8228E8406D687A931FF029380C7D6B81B34A92271EF0F288EBFE815C69ABC22A
        A474B74C678423E4470225C90DB07C142B3E0CB61F9284A435FAF1302D0C31DF
        44EF5F1A8A3D5CD87A409C68296CCF7EED175B1DC2430D0806029033C3041DA7
        DCA385AC9E0335A1179A66F0810DFE3FD0FB1688D33041FB61D7442B41EB91DA
        95E71B67F1318D81404BB313C547A65A175C7839677FF55236A404C160E1E817
        E7298A2629E65DA8A1AF586960A5D514DF6B8824D9350E24CB0FC5FB5AD566B0
        FBAA4C5713C949F7DD9D1A93E0BC3D5E935CE77BDBD4D67724FFBB1CC7B18F95
        A1274739596D0C1A0DBE1BFEECFA57C6531443E14FAC231D2DC282C7C8BA15DB
        CF54EC17274DB329CA6C08D85FB31C10642588537CADF32F3504C43E5FEAE366
        7E7C8B8FAA57352F32BB474B63C87B109586085F3D65B18F03F8EEEE56CE8FCC
        511334FAC1AA54EBD72E9AF8A3BF735E71FDFAE4123FDBCAA4B1492A82B7AC38
        019EB3E9D8458F7016A65D5DB2B7A2DB8EC781D2E266DB9972C3F6DD2CACEBE2
        B20CAE60188686E073BB02441682CE5EDDE0019C35B1D72A9AE46CBCCF0AE969
        E678E42C3707939DC9F76F94B671D89C33EEB12E66D587AA56C351513FC78D39
        28E916AD2A8901401A1D11BD615AE004DB931945276ED608B8789FF7F902CE95
        84B9B237D23BE56FD605897FB95878FA765D9F9524E9812646997B66B3196251
        64769B5CA763F86B45D93F8E48CF6FF7847F4D58B0DC31EB6AE3CCF8AA357C4C
        ADD2E8037C47C5ACF0282869D89C92C7E11820743A72F4489BB35B7DAFDD7F91
        9EDF9C153BABACA269ED813F586C1CF848A32526BA3BA4844D7951D51C9AF43B
        C6C27414CBCD447262FCCE97727BF861CAE4291605F1CFD7FFD63A15642E4152
        83CD0764C405906AF5C2A86B3D1A0A385AA9217E5AEEF5C374C7AD07EFDC7BF1
        F67262F05013D6921DD94D5D1A1C4380F5E715DE4B7D4547D30BCEDE95806791
        0C6AC65664788693140417FD3261C2A0E21DCFC2EEBCF1060434CD60189A1A1B
        68672ED8B8F7469144CAC1111A4653E382AC8DE079DB00A83A62F577CB7C4509
        A773B3F21A791C8C62E033D1812E568215BB72AA5B146C1C015DC788A5CFF08C
        C069E57B82A8B2B0DB1D0602E0102D4127477C3FD1D9FCF8C5C253B7EB581862
        6F3B2823E66FA5E58D1B0EE5824AF099203AB8C1FB8FC2BAF0E305A020CCCD07
        64ED0954F7AA16445D53E90C2F060442962EC3732B4AABE047C7264D362FDC5D
        BEE946FB943E029091ABE6796C9CED5250F272F3D17C505C8BFC5DB72E723D92
        5670FE9E04F8C4C1D6F4428C7F8FAC77F1CEEB1D72DD0C6FA7C4B5932A25EDA1
        09F71014056508088C599A4CCF0898D6C15949FEF387DD382759902C5928C0D4
        8040AF271D4516A9517ECA1E0508C31B2575226A9687C87475ECF517AF64E041
        389B95161B6863CADD76E86EF6E3E63DFF981E3CD93EF5B7D2C3575FF0B92C03
        028D0F17B6A48E0F6F565BC289316BC35D4E1677B86D2A8B60237A6006614071
        3C356EB6BD1927343AA7514EE524CE4129626164F61BB90E431195968CD9E01B
        30C126E3C6B384CC8AACC4E0E11682B07D370AEBA47D750346569075DE2E714A
        59E72878E5967D67C7476A48DED227896D6A93BE1E0B2076AF9F36DBD336F14C
        6E93123EB6F9BBAADA9690C4070806261AF0A1DE7FDAA8D8951E205963D2CB2E
        EF0B52CB0D6FEDD5BDAF6D15C94D1A73C4D726FFE2CB39B0E79A4BA7DCE2470F
        AA4AAE5E79AED1BFCF4B20B5A74D1E013C9B79A792C03920054F5C7A7CEA562D
        FF7D655166838C2FC506122A6546EEAB7573C7DECBAFD97EBA88F7AE68F434E6
        60D471C63D8A836A434BE360D79557E6D83CDC294EA9EC745A5D1A8383E10531
        0062A089F06A4250757D075F281861C95FB92BA7AAA597FDA1896AF474F2B699
        934698D537CB1C6D4DA38F3DB856DCC2E7189A632FC9DFE694BA58945DD2E1B6
        F159043C69CD050CA1533D76D809FFDC5E1E7EBBCDD30857839EA1A7A06391B3
        C6D819A328D2DE2E5D1A738B30CC54A8BFBB2D9FEBBE79AE18B4688A20FE1E95
        D3D2ADC551048457806BD23CB65B0A3AC29EEE2A043100F3404D71660C79B277
        CC41A9D6644B7904500F60582A35FA90F913D607BA00B89CFB1531A9257D19D2
        DF9F9D1C2D5277F881EFCA1A4378501C541C48523ADAE5D474EBBC072D53222B
        361886F98791C90B774A5DE29023539B6E7C1655D76B8DD14A07FBC1F12193C0
        F0493AF7E8092869D6C721039EC82068FC7A1F87C1828C9BE5990F1B586C0E03
        A1F1A38E4EB7CEAF930DDFF86CBB823014F57B02A00F042CEDF171F1A201AF5A
        15169BCBB7BD5458F1500D7835989D0449C370FF74F9C8812008B083F645302C
        A06276BB9C9C6E990FD1E8BAA7BB8BBB9CF998F633D9F26E6A37278A0F591BB7
        B4C92D636A36147739210CC941F528F2F5214C1B4285E9198E195BBE7DE4B9E9
        56790C891E91AC487FEDD7AFF23E135E40B99873E4AB865E0DB6BB4391F8EF9D
        9E77DBBD0BBBC440D001FD02245BBFCE00EE266843CE381937CFB4C89F3AA8C8
        5AD8562D1B9152BFA454EACC46F5FD3BBF948EE01898D7CBEC6F868AAEF0380A
        A03ECBBB4715485D2BE4C35BD4E6C0159041975266EC1E4741D344D3E75E66A5
        5CAE1A48A97B6D3E49B5CBA53A23A0243E05FC8AF805424A4D7244466D73AC1F
        F85BE41A717BDFDD1952EA8D01010805E866C6980C32E8637023A448E696D1FC
        FD6390911085235F0A35F8AFE4BBFEDD53EC05EDDE83CAC60DACB560B70F6649
        11C8D0481818EE264CA4FA01D50AC7DCB7EE95F261048D703F71CBA7EBBFD4B5
        9DF8431147810000000049454E44AE426082}
    end
    object scGPImage_error: TscGPImage
      Left = 6
      Top = 1
      Width = 32
      Height = 32
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
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
      RotationAngle = 0
      AnimationAcceleration = False
      RotateAnimation = False
      PngImage.Data = {
        89504E470D0A1A0A0000000D4948445200000020000000200802000000FC18ED
        A3000000097048597300002E2800002E2801F196167E000006B94944415478DA
        AD9679505357148773DF9A40088B4020028A100D2828D4D6820B20AE54C4568A
        D5B1B575C12AA365468BA35651A80AAE436BB1681D45AA025A151714518BA222
        0A8882560143C2120804084AC8F292BCDE80832926683BBD7F65CE7B79DF3DBF
        B38209CB3218A68E0E202A8CB4A294DE32E198E6E743E4120F9918D56A180CA0
        475191BD9BD886F7883BFCB123FF256E416A3518AD33F91D6012D08D33399432
        54FC20B2324F20A962983F2F1CDD2F0842AE7806B632AD59940ABC13A003A80A
        2742C5252B1F64B94B6BA1A58D6D57CEF3AA7016BCB073D56238B4A03AAD5B87
        C4475A35B6A1D2E1652BB4486D9DD23E9C778E3F91D052B85E6B1A00E16A1467
        EAA82F2B2F2D2DCA849672579F1B1EE3F2DDC7B5B26C740C80D27A40D3D04E03
        860E41119AB657BD0C1697040BEF07D49642FB49BFD9E97E1132D2CA42ABA6DF
        066850DCA9BB23A12075745D8582C54E9EB434CF23900288415FBD697D0DEE62
        380020A8A17C7DC14187CE162177D8E69098E7B62E4C43B48C00F0559CD6EDBF
        92EC535FD9C562C7CDF8FECE601F2B4D77EF95073E340328089657BB28E55212
        54AC7E90CBB2D95BE48465AF56AF01F08D98D2534BEE652998ECB56171C54EDE
        969412DA952A4AAB7F07C39289A308E8C648CFCEC694DC64AEBCF9D4984FB68F
        5F6CA95102282704287166487D6972EEEE6E92153763ED3DC3D7550C1AE61D98
        3591EF6CCB1AF0FEF4F99BCFA572158A224A9CF4EC68DC7739D959DEBC73F2F2
        4CC154784B10187D1C63E8D3CF6D1ADA22DA3C3D36873F89A356F4FE534D83F4
        84082F9ED5C01EAC48C82911CA99040A7F7711AC09928ABDE7B741251644EDEC
        20D8C06FE5E9B9D53737E6FF5CC9132C9E93406835AF75EF01FCBA21CC7F98DD
        E3AA6691F4158622263EAFD71F395FDED0A6EC7BAA202CF65FDE11282CF92D60
        FE2FFE916062F4EF872F6CE53755EF0B5A92316A2614AECFF73EC0D6D4EB270B
        5E40AD4D390098240CC19B0A83451A262C4ACCDBDB62E5B0303209ACF8746BEA
        D9F84E0BEBF951BB3A48F69B8C3402EC385470AAB0D60CE02D97000235CF38B3
        D14D56173F3D16A405CC8F2E3A7963F8F8B829B1B0D68DA3F7DF00BD39B9EE5E
        C6BCD29CF3BED3409E2068DAB39B29418BD34785BDD1E79F80F8FDD74E989188
        C4519240FB550B5469AAE87E52EEAE72375F50E5E0CE6FAD5DF5D99622476F96
        4E6D1270BB4CF4442C273013417EF8ACA9F8693381A3C6460AC5DCE59213996B
        1BED784064C383ADF8DBA8EDA5761E4C9DC6246000354EE555241EBB6F6D49F6
        03B8CA9BB232D7482100B67537B96479D4F632F30073690ABB5061A928BFA48E
        4960660135838678B489A144F71CBDCC010648531C43091CE9170308182A6F3A
        91B9460201D7864F08ADBABD33243AD37B9AB920FFDB2C526264487DD9AE8B49
        152E23C1E171518B8BB3F3BC433604C75850FF0FA08BB0585D92F54D71F6E551
        53C0EA884D2939896D1CFB0591C9AF7026AAD79B046417D6B2DF0F4003006BED
        68CEE6E1CD353BA6C480A065C7D2CFFE3044569714BA227B44A82565A255241E
        B891795368CE031405C6F187FA04437D7277BE647116462603BF98D38B2A2E7D
        77EB4899ABCFF2F04D24A5814DBC1F402C91B7C895B0E39804DC2E1166E45731
        49AC4F9FBD57F78454DFCDF20F4FFE781108587E9C4DA932FE58EFF84AB6367C
        C375173F38C8FA0047B6CC1EE9C2195893ABB79EAE3B58C4B630948202677AB7
        8B0F9DF98106C85791490D560E8681A3C059113585F157535A6CB8B133E3AA6D
        5C995A4349EB6846A0AF8B3D871C606E0204881ADA1ED5B46118DCA308E7EEF6
        3D5776F3A52FD2C62F4C1B3DC73070FA4666DCDDA35F3CBCD8CA7158356B438D
        CD60166560A8D45ADDBB46268E21B023293182ABEAFC2937699854785D306963
        C84AB8DDC002790DA010D49A52A65D4C8473ADD9D62976E6BAE7B6AE3D43F5BD
        0E54C64521DB7765978754D86E65B72422A1916D4FEA2886F1DA02A32FE8A8FF
        F15ACA50595DB33577DBE4157778A3E07483722166348219A942093D827AB5D5
        6EFD33D5532A94711CE243638A9DBD7B0560F45BBCA09B7097FABA3C67DEC38B
        0C042918F6D1357EE02D57FF57380BA1F57016F52618DC53B40081DF6569D581
        8D8FA755DD0EAE7D4050EAAB5E41073EF8BC9EC3359E2BFD57470AC134183EEF
        AFFCE8FBD9B65D1DD02274F22876F17DE4E829E6386B0DD760A00CDAADABC54B
        561B202E1334551BBC2759E963E71EF50D87F1228D1B9AC9E5175E102E208315
        B2F0EAC2D94FAF73E5D2BE47BA9ED96B3C5FA0E279FC096704A1421B9E493181
        B9F51DBAA2C670AE523E4EF2C45FF2744867D3A0AE768CD6F760900E0BEB3A1B
        E7474E238A783E8D96F67089237A42FAF6F91B020D7B5FF99D17D80000000049
        454E44AE426082}
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
    object RzStatusPane__chyby_caption: TRzStatusPane
      AlignWithMargins = True
      Left = 3
      Top = 0
      Height = 28
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      BorderWidth = 0
      FlatColor = 10114859
      FrameStyle = fsNone
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -15
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearTypeNatural
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      BlinkColor = 10114859
      Caption = 'Po'#269'et chyb'
      CaptionOffset = 0
    end
    object RzStatusPane_var_header: TRzStatusPane
      Left = 106
      Top = 0
      Width = 130
      Height = 28
      BorderWidth = 0
      FlatColor = 10114859
      FrameStyle = fsNone
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -15
      Font.Name = 'Roboto'
      Font.Style = []
      Font.Quality = fqClearTypeNatural
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      BlinkColor = 10114859
      Caption = 'Po'#269'et varov'#225'n'#237
    end
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
      TabOrder = 0
      Visible = False
      GlyphColor = clBlack
      GlyphColorAlpha = 200
      GlyphThickness = 1
    end
    object scGPGlyphButton1: TscGPGlyphButton
      AlignWithMargins = True
      Left = 236
      Top = 0
      Width = 3
      Height = 28
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 1
      TabStop = True
      Animation = False
      Caption = 'scGPGlyphButton1'
      CanFocused = True
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
      ExplicitLeft = 223
    end
    object scGPGlyphButton2: TscGPGlyphButton
      AlignWithMargins = True
      Left = 103
      Top = 0
      Width = 3
      Height = 28
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 2
      TabStop = True
      Animation = False
      Caption = 'scGPGlyphButton1'
      CanFocused = True
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
