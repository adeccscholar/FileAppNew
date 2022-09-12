//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MainFormFMX.h"
#include <FMX.Dialogs.hpp>
#include <regex>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMainFMX *frmMainFMX;

std::map<EShowVariante, TPopupMenu*> mpMenus;
std::map<std::wstring, std::function<void ()>> mpActions;

// specified prefixes are removed from the name. The same applies to up to two-digit 
// numbers that are optional.  Between them can be an underscore
void CallAction(std::wstring const& strComponentName) {
   static const std::wregex parser(L"^((mitem|btn|tbtn))([A-Za-z]+)(_?)([0-9]{0,2})$");
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
     { L"Select",              [this]() { this->proc.SelectWithDirDlg(this->proc.Form(), "edtDirectory"); } },

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
                                          this->proc.Form().Message(EMyMessageType::information, "FileApp - Information", os.str());
                                        } },
     { L"CntSelectedFileRows", [this]() { std::ostringstream os;
                                          this->proc.CountFileRowsForProjects(os, true);
                                          this->proc.Form().Message(EMyMessageType::information, "FileApp - Information", os.str());
                                        } },
     // Show Files
     { L"ShowFile",            [this]() { this->proc.OpenViewFile(); } }
     };


   try {
      proc.Init( { this, false } );   
      }
   catch(std::exception &ex) {
      ShowMessage(ex.what());
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

