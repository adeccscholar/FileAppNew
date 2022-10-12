//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "MyFileDlg.h"
#include "Embarcadero_Actions.h"
#include <regex>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;


std::map<EShowVariante, TPopupMenu*> mpMenus;
//std::map<std::wstring, std::function<void ()>> mpActions;

/*
// specified prefixes are removed from the name. The same applies to up to two-digit 
// numbers that are optional.  Between them can be an underscore
void CallAction(std::wstring const& strComponentName) {
   static const std::wregex parser(L"^((ac|mitem|btn|tbtn))([A-Za-z]+)(_?)([0-9]{0,2})$");
   static const std::wstring strFmt = L"$3";
   try {
      std::wstring strName = std::regex_replace(strComponentName, parser, strFmt);
      if(auto it = mpActions.find(strName); it != mpActions.end()) {
         it->second();
         }
      else {
         std::cerr << "the sender of the action isn't a component, name isn't available" << std::endl;
         }
      }
   catch(std::exception &ex) {
      std::cerr << "error with action call: " << ex.what() << std::endl;
      }
   }
*/

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner) {
   }

void __fastcall TfrmMain::FormCreate(   TObject *Sender) {

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
     //{ L"Select",              [this]() { this->proc.Test3(); } },

     { L"SelectFile",          [this, find_menu]() { this->proc.OpenFileInDirectory(); } },

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
     { L"ResFile",             [this]() { this->proc.OpenResFile(); } },
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



