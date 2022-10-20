//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MainFormFMX.h"
#include "Embarcadero_Actions.h"
#include "FileApp_Embarcadero.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMainFMX *frmMainFMX;

//---------------------------------------------------------------------------
__fastcall TfrmMainFMX::TfrmMainFMX(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainFMX::FormCreate(TObject *Sender) {

   mpMenus   = {
     { EShowVariante::Projects, mnuProjects },
     { EShowVariante::Files,    mnuShow },
     { EShowVariante::Count,    nullptr }
     };

   SetCallActions(mpActions, this);

   try {
      proc.Init( { this, false } );   
      }
   catch(std::exception &ex) {
      TMyFileDlg::Message(EMyMessageType::error, "FileApp - Information", ex.what());
      }
   }

//---------------------------------------------------------------------------

void __fastcall TfrmMainFMX::DynActionClick(TObject *Sender) {
	TComponent *comp = dynamic_cast<TComponent*>(Sender);
   if(comp) {
      CallAction(comp->Name.c_str());
      }
   else {
      std::cerr << "the sender of the action isn't a component, name isn't available" << std::endl;
      }
	}


void __fastcall TfrmMainFMX::lbValuesChange(TObject *Sender) {
   proc.SelectedExtentionsChanged();   
   }
//---------------------------------------------------------------------------

