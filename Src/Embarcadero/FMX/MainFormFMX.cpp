//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MainFormFMX.h"
#include "Embarcadero_Actions.h"
#include "MyFileDlg.h"
#include <FMX.Dialogs.hpp>
#include <regex>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMainFMX *frmMainFMX;

std::map<EShowVariante, TPopupMenu*> mpMenus;

//---------------------------------------------------------------------------
__fastcall TfrmMainFMX::TfrmMainFMX(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMainFMX::FormCreate(TObject *Sender)
{

   auto find_menu = [this](EShowVariante mode) {
      if(auto it = mpMenus.find(mode); it != mpMenus.end()) this->lvOutput->PopupMenu = it->second;
      else this->lvOutput->PopupMenu = nullptr; 
      };

   mpMenus   = {
     { EShowVariante::Projects, mnuProjects },
     { EShowVariante::Files,    mnuShow },
     { EShowVariante::Count,    nullptr }
     };

   mpActions = {
     // Main- Actions
     { L"Select",              [this]() { this->proc.SelectWithDirDlg(); } },
     //{ L"Select",              [this]() { this->proc.Test2(); } },

     { L"Count",               [this, find_menu]() { this->proc.CountAction();
                                                     find_menu(this->proc.GetShowMode()); 
                                                   } },
     { L"Parse",               [this, find_menu]() { this->proc.ParseAction();
                                                   find_menu(this->proc.GetShowMode());  
                                                   } },
     { L"Show",                [this, find_menu]() { this->proc.ShowAction(); 
                                                     find_menu(this->proc.GetShowMode()); 
                                                   } },

     { L"CloseApp",            [this]() { this->Close(); } },

     { L"Test",                [this]() { this->proc.Test4(); } },

     { L"AddExtention",        [this]() { this->proc.AddExtention(); } },
     { L"ChgExtention",        [this]() { this->proc.ChangeSelectedExtentions(); } },
     { L"DelExtention",        [this]() { this->proc.DeleteExtentions(true); } },
     { L"DelAllExtentions",    [this]() { this->proc.DeleteExtentions(false); } },

     // Project parse 
     { L"ProjectFile",         [this]() { this->proc.OpenProjectFile(); } },
     { L"CppFile",             [this]() { this->proc.OpenSourceFile(); } },
     { L"HeaderFile",          [this]() { this->proc.OpenHeaderFile(); } },
     { L"ResFile",             [this]() { this->proc.OpenResFile(); 
                                        } },
     { L"CntAllFileRows",      [this]() { std::ostringstream os;
                                          this->proc.CountFileRowsForProjects(os, false);
                                          TMyFileDlg::Message(EMyMessageType::information, "FileApp - Information", os.str());
                                          //this->proc.Form().Message(EMyMessageType::information, "FileApp - Information", os.str());
                                        } },
     { L"CntSelectedFileRows", [this]() { std::ostringstream os;
                                          this->proc.CountFileRowsForProjects(os, true);
                                          TMyFileDlg::Message(EMyMessageType::information, "FileApp - Information", os.str());
                                          //this->proc.Form().Message(EMyMessageType::information, "FileApp - Information", os.str());
                                        } },
     // Show Files
     { L"ShowFile",            [this]() { this->proc.OpenViewFile(); } }
     };


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

