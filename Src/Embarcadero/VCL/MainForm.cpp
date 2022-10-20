//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "Embarcadero_Actions.h"
#include "FileApp_Embarcadero.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner) {
   }

void __fastcall TfrmMain::FormCreate(   TObject *Sender) {
   /*
   auto find_menu = [this](EShowVariante mode) {
      if(auto it = mpMenus.find(mode); it != mpMenus.end()) this->lvOutput->PopupMenu = it->second;
      else this->lvOutput->PopupMenu = nullptr; 
      };
   */
   mpMenus   = {
     { EShowVariante::Projects, mnuProjects },
     { EShowVariante::Files,    mnuShow },
     { EShowVariante::Count,    nullptr }
     };

   SetCallActions(mpActions, this);

   try {
      proc.Init( { this, false });
      }
   catch(std::exception& ex) {
      TMyFileDlg::Message(EMyMessageType::error, "FileApp - Information", ex.what());
      }
   }
   
//-------------------------------------------------------------------------

void __fastcall TfrmMain::DynActionClick(TObject *Sender) {
	TComponent *comp = dynamic_cast<TComponent*>(Sender);
   if(comp) {
      CallAction(comp->Name.c_str());
      }
   else {
      std::cerr << "the sender of the action isn't a component, name isn't available" << std::endl;
      }
	}

void __fastcall TfrmMain::lbValuesClick(TObject *Sender) {
   proc.SelectedExtentionsChanged();      
   }
//---------------------------------------------------------------------------



