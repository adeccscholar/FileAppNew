object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'frmMain'
  ClientHeight = 1415
  ClientWidth = 2266
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -25
  Font.Name = 'Segoe UI'
  Font.Style = []
  OnCreate = FormCreate
  PixelsPerInch = 200
  TextHeight = 35
  object Panel1: TPanel
    Left = 1734
    Top = 0
    Width = 532
    Height = 1379
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Align = alRight
    TabOrder = 0
    object btnCount: TButton
      Left = 12
      Top = 21
      Width = 499
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnCount'
      TabOrder = 0
      OnClick = DynActionClick
    end
    object btnParse: TButton
      Left = 10
      Top = 703
      Width = 499
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnParse'
      TabOrder = 9
      OnClick = DynActionClick
    end
    object btnShow: TButton
      Left = 12
      Top = 85
      Width = 499
      Height = 53
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnShow'
      TabOrder = 1
      OnClick = DynActionClick
    end
    object lbValues: TListBox
      Left = 12
      Top = 157
      Width = 499
      Height = 304
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      ItemHeight = 35
      MultiSelect = True
      TabOrder = 2
      OnClick = lbValuesClick
    end
    object edtExtentions: TEdit
      Left = 11
      Top = 470
      Width = 499
      Height = 43
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      TabOrder = 3
      Text = 'edtExtentions'
    end
    object btnAddExtention: TButton
      Left = 11
      Top = 517
      Width = 165
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnAddExtention'
      TabOrder = 4
      OnClick = DynActionClick
    end
    object btnChgExtention: TButton
      Left = 178
      Top = 517
      Width = 165
      Height = 53
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnChgExtention'
      TabOrder = 5
      OnClick = DynActionClick
    end
    object btnDelExtention: TButton
      Left = 346
      Top = 517
      Width = 165
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnDelExtention'
      TabOrder = 6
      OnClick = DynActionClick
    end
    object btnDelAllExtentions: TButton
      Left = 12
      Top = 571
      Width = 499
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnDelAllExtentions'
      TabOrder = 7
      OnClick = DynActionClick
    end
    object btnCloseApp: TButton
      Left = 11
      Top = 783
      Width = 499
      Height = 52
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'btnCloseApp'
      TabOrder = 10
      OnClick = DynActionClick
    end
    object chbSubDirs: TCheckBox
      Left = 12
      Top = 648
      Width = 499
      Height = 34
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Caption = 'chbSubDirs'
      TabOrder = 8
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 1734
    Height = 1379
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Align = alClient
    TabOrder = 1
    object Splitter1: TSplitter
      Left = 1
      Top = 1186
      Width = 1732
      Height = 6
      Cursor = crVSplit
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alBottom
      ExplicitLeft = 2
      ExplicitTop = 179
      ExplicitWidth = 2229
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 1732
      Height = 85
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alTop
      TabOrder = 0
      DesignSize = (
        1732
        85)
      object lblDirectory: TLabel
        Left = 33
        Top = 25
        Width = 129
        Height = 35
        Margins.Left = 6
        Margins.Top = 6
        Margins.Right = 6
        Margins.Bottom = 6
        Caption = 'lblDirectory'
      end
      object edtDirectory: TEdit
        Left = 221
        Top = 22
        Width = 1324
        Height = 43
        Margins.Left = 6
        Margins.Top = 6
        Margins.Right = 6
        Margins.Bottom = 6
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        Text = 'edtDirectory'
      end
      object btnSelect: TButton
        Left = 1557
        Top = 18
        Width = 156
        Height = 52
        Margins.Left = 6
        Margins.Top = 6
        Margins.Right = 6
        Margins.Bottom = 6
        Anchors = [akTop, akRight]
        Caption = 'btnSelect'
        TabOrder = 1
        OnClick = DynActionClick
      end
    end
    object memError: TMemo
      Left = 1
      Top = 1192
      Width = 1732
      Height = 186
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alBottom
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -29
      Font.Name = 'Courier New'
      Font.Style = []
      Lines.Strings = (
        'memError')
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 1
    end
    object lvOutput: TListView
      Left = 1
      Top = 86
      Width = 1732
      Height = 1100
      Margins.Left = 6
      Margins.Top = 6
      Margins.Right = 6
      Margins.Bottom = 6
      Align = alClient
      Columns = <>
      TabOrder = 2
    end
  end
  object sbMain: TStatusBar
    Left = 0
    Top = 1379
    Width = 2266
    Height = 36
    Margins.Left = 6
    Margins.Top = 6
    Margins.Right = 6
    Margins.Bottom = 6
    Panels = <>
  end
  object mnuProjects: TPopupMenu
    Left = 1819
    Top = 1083
    object mitemProjectFile: TMenuItem
      Caption = 'open project file'
      OnClick = DynActionClick
    end
    object openprojectfile2: TMenuItem
      Caption = '-'
    end
    object mitemCppFile: TMenuItem
      Caption = 'open cpp file'
      OnClick = DynActionClick
    end
    object mitemHeaderFile: TMenuItem
      Caption = 'open h file'
      OnClick = DynActionClick
    end
    object mitemResFile: TMenuItem
      Caption = 'open res file'
      OnClick = DynActionClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object mitemCntAllFileRows: TMenuItem
      Caption = 'Lines in all Files'
      OnClick = DynActionClick
    end
    object mitemCntSelectedFileRows: TMenuItem
      Caption = 'Lines in selected Files'
      OnClick = DynActionClick
    end
    object mitemSelectFile2: TMenuItem
      Caption = 'select a file to show'
      OnClick = DynActionClick
    end
  end
  object mnuShow: TPopupMenu
    Left = 1955
    Top = 1077
    object mitemShowFile: TMenuItem
      Caption = 'open file'
      OnClick = DynActionClick
    end
    object mitemSelectFile1: TMenuItem
      Caption = 'select file to show'
      OnClick = DynActionClick
    end
  end
end
