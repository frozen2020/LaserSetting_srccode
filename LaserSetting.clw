; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLaserSettingDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LaserSetting.h"

ClassCount=5
Class1=CLaserSettingApp
Class2=CLaserSettingDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_COM (Chinese (P.R.C.))
Class4=CComConfigDlg
Resource4=IDD_LASERSETTING_DIALOG
Resource5=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource6=IDD_LASERSETTING_DIALOG (Chinese (P.R.C.))
Resource7=IDD_DIALOG_COM
Class5=CDlgPass
Resource8=IDD_DIALOG1

[CLS:CLaserSettingApp]
Type=0
HeaderFile=LaserSetting.h
ImplementationFile=LaserSetting.cpp
Filter=N
LastObject=CLaserSettingApp

[CLS:CLaserSettingDlg]
Type=0
HeaderFile=LaserSettingDlg.h
ImplementationFile=LaserSettingDlg.cpp
Filter=D
LastObject=IDC_BUTTON_CANCEL
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=LaserSettingDlg.h
ImplementationFile=LaserSettingDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LASERSETTING_DIALOG]
Type=1
Class=CLaserSettingDlg
ControlCount=48
Control1=IDC_STATIC_COM,static,1342308352
Control2=IDC_COMBO_COM,combobox,1344340227
Control3=IDC_BUTTON_SEND,button,1342242816
Control4=IDC_BUTTON_CONNECT,button,1342242816
Control5=IDC_BUTTON_CANCEL,button,1476460544
Control6=IDC_STATIC,static,1342177296
Control7=IDC_EDIT_WIDI,edit,1350631552
Control8=IDC_EDIT_WPRF,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_WPWM,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_TEV,edit,1350633600
Control18=IDC_EDIT_TLA,edit,1350633600
Control19=IDC_EDIT_RTDI,edit,1350633600
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_RSHG,edit,1350633600
Control22=IDC_EDIT_RTHG,edit,1350633600
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342177297
Control27=IDC_STATIC,static,1342308352
Control28=IDC_COMBO_DIO,combobox,1342243075
Control29=IDC_STATIC,static,1342308865
Control30=IDC_STATIC,static,1342308352
Control31=IDC_COMBO_QS,combobox,1342243075
Control32=IDC_COMBO_ESH,combobox,1342243075
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT_WSHG,edit,1082196096
Control36=IDC_EDIT_RIDI,edit,1350633600
Control37=IDC_EDIT_RPRF,edit,1350633600
Control38=IDC_EDIT_RPWM,edit,1350633600
Control39=IDC_EDIT_WTHG,edit,1082196096
Control40=IDC_STATIC,static,1342308352
Control41=IDC_STATIC,static,1342308352
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_STATIC,static,1342308352
Control45=IDC_EDIT_WTDI,edit,1082196096
Control46=IDC_STATIC_DIO,static,1342177283
Control47=IDC_STATIC_QS,static,1342177283
Control48=IDC_STATIC_ESH,static,1342177283

[DLG:IDD_DIALOG_COM]
Type=1
Class=CComConfigDlg
ControlCount=12
Control1=IDC_BUTTON_APPLY,button,1342242816
Control2=IDC_BUTTON_CANCEL,button,1342242816
Control3=IDC_STATIC_BAUDRATE,static,1342308354
Control4=IDC_STATIC_PARITY,static,1342308354
Control5=IDC_STATIC_DATABITS,static,1342308354
Control6=IDC_STATIC_STOPBITS,static,1342308354
Control7=IDC_STATIC_DATACONTROL,static,1342308354
Control8=IDC_COMBO_BAUDRATE,combobox,1344339970
Control9=IDC_COMBO_DATABITS,combobox,1344339970
Control10=IDC_COMBO_PARITY,combobox,1344339970
Control11=IDC_COMBO_STOPBITS,combobox,1344339970
Control12=IDC_COMBO_DATACONTROL,combobox,1344339970

[CLS:CComConfigDlg]
Type=0
HeaderFile=ComConfigDlg.h
ImplementationFile=ComConfigDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CComConfigDlg
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIALOG_COM (Chinese (P.R.C.))]
Type=1
Class=CComConfigDlg
ControlCount=12
Control1=IDC_BUTTON_APPLY,button,1342242816
Control2=IDC_BUTTON_CANCEL,button,1342242816
Control3=IDC_STATIC_BAUDRATE,static,1342308354
Control4=IDC_STATIC_PARITY,static,1342308354
Control5=IDC_STATIC_DATABITS,static,1342308354
Control6=IDC_STATIC_STOPBITS,static,1342308354
Control7=IDC_STATIC_DATACONTROL,static,1342308354
Control8=IDC_COMBO_BAUDRATE,combobox,1344339970
Control9=IDC_COMBO_DATABITS,combobox,1344339970
Control10=IDC_COMBO_PARITY,combobox,1344339970
Control11=IDC_COMBO_STOPBITS,combobox,1344339970
Control12=IDC_COMBO_DATACONTROL,combobox,1344339970

[DLG:IDD_LASERSETTING_DIALOG (Chinese (P.R.C.))]
Type=1
Class=CLaserSettingDlg
ControlCount=18
Control1=IDC_CHECK_DIO,button,1342242819
Control2=IDC_STATIC_COM,static,1342308352
Control3=IDC_COMBO_COM,combobox,1344340227
Control4=IDC_BUTTON_COMSET,button,1342242816
Control5=IDC_BUTTON_CONNECT,button,1342242816
Control6=IDC_BUTTON_CANCEL,button,1476460544
Control7=IDC_STATIC,static,1342177296
Control8=IDC_EDIT_IDI,edit,1350631552
Control9=IDC_EDIT_PRF,edit,1350631552
Control10=IDC_CHECK_QS,button,1342242819
Control11=IDC_CHECK_EXT,button,1342242819
Control12=IDC_CHECK_SHT,button,1342242819
Control13=IDC_BUTTON_SEND,button,1476460544
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_BUTTON_GET,button,1476460544

[DLG:IDD_DIALOG1]
Type=1
Class=CDlgPass
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_BUTTON1,button,1342242816

[CLS:CDlgPass]
Type=0
HeaderFile=DlgPass.h
ImplementationFile=DlgPass.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

