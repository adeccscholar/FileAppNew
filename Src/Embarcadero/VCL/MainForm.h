//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "FileAppProcessVCL.h"
#include <Vcl.Menus.hpp>

class TfrmMain : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *lblDirectory;
    TEdit *edtDirectory;
    TButton *btnCount;
   TButton *btnParse;
    TStatusBar *sbMain;
    TMemo *memError;
    TListView *lvOutput;
    TSplitter *Splitter1;
    TButton *btnShow;
    TListBox *lbValues;
   TPopupMenu *mnuProjects;
   TPopupMenu *mnuShow;
   TMenuItem *mitemProjectFile;
   TMenuItem *openprojectfile2;
   TMenuItem *mitemCppFile;
   TMenuItem *mitemHeaderFile;
   TMenuItem *mitemResFile;
   TMenuItem *N1;
   TMenuItem *mitemCntAllFileRows;
   TMenuItem *mitemCntSelectedFileRows;
   TMenuItem *mitemShowFile;
   TButton *btnSelect;
   TEdit *edtExtentions;
   TButton *btnAddExtention;
   TButton *btnChgExtention;
   TButton *btnDelExtention;
   TButton *btnDelAllExtentions;
   TButton *btnCloseApp;
   TCheckBox *chbSubDirs;
   TMenuItem *mitemSelectFile1;
   TMenuItem *mitemSelectFile2;
   TButton *btnTest;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall DynActionClick(TObject *Sender);
   void __fastcall lbValuesClick(TObject *Sender);
          
private:	// Benutzer-Deklarationen
    TProcessVCL proc;
public:		// Benutzer-Deklarationen
    __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
