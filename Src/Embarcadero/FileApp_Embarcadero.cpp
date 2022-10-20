//---------------------------------------------------------------------------

#pragma hdrstop

#include "FileApp_Embarcadero.h"

#include <exception>
#include <regex>

//---------------------------------------------------------------------------
#pragma package(smart_init)

used_Process proc;
std::map<EShowVariante, used_PopupMenu*> mpMenus;


used_PopupMenu* Find_Popup(EShowVariante mode) {
   if(auto it = mpMenus.find(mode); it != mpMenus.end()) return it->second;
   else return nullptr; 
   }




