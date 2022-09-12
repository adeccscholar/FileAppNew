//---------------------------------------------------------------------------

#ifndef MainFormFMXH
#define MainFormFMXH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Rtti.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Menus.hpp>
//---------------------------------------------------------------------------
#include <FileAppProcessFMX.h>

class TfrmMainFMX : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
   TPanel *Panel1;
   TPanel *Panel2;
   TButton *btnCount;
   TButton *btnShow;
   TListBox *lbValues;
   TButton *btnParse;
   TStatusBar *StatusBar1;
   TLabel *sbMain;
   TMemo *memError;
   TSplitter *Splitter1;
   TStringGrid *lvOutput;
   TPanel *Panel3;
   TLabel *lblDirectory;
   TEdit *edtDirectory;
   TPopupMenu *mnuProjects;
   TPopupMenu *mnuShow;
   TMenuItem *mitemProjectFile;
   TMenuItem *mitemCppFile;
   TMenuItem *mitemHeaderFile;
   TMenuItem *mitemResFile;
   TMenuItem *mitemCntAllFileRows;
   TMenuItem *mitemCntSelectedFileRows;
   TMenuItem *mitemShowFile;
   TButton *btnSelect;
   TButton *btnAddExtention;
   TButton *btnChgExtention;
   TButton *btnDelExtention;
   TButton *btnDelAllExtentions;
   TEdit *edtExtentions;
   TMenuItem *MenuItem1;
   TMenuItem *MenuItem2;
   TButton *btnCloseApp;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall DynActionClick(TObject *Sender);
   void __fastcall lbValuesChange(TObject *Sender);
private:	// Benutzer-Deklarationen
   TProcessFMX proc;
public:		// Benutzer-Deklarationen
   __fastcall TfrmMainFMX(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMainFMX *frmMainFMX;
//---------------------------------------------------------------------------
#endif
