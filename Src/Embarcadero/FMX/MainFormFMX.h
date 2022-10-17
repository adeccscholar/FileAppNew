//---------------------------------------------------------------------------

#ifndef MainFormFMXH
#define MainFormFMXH
//---------------------------------------------------------------------------
#include <FileAppProcessFMX.h>
#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.Menus.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>
#include <System.Rtti.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.FMXUI.Wait.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>

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
   TCheckBox *chbSubDirs;
   TButton *btnTest;
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
