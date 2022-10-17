//---------------------------------------------------------------------------

#ifndef Embarcadero_ActionsH
#define Embarcadero_ActionsH
//---------------------------------------------------------------------------

#if defined BUILD_WITH_VCL 
   #include <Vcl.Menus.hpp>
#elif defined BUILD_WITH_FMX
   #include <FMX.Menus.hpp>
#else
   #error wrong framework for use this file
#endif

#include "MyForm.h"
#include "FileAppProcess.h"

#include <string>
#include <map>
#include <functional>



extern std::map<std::wstring, std::function<void ()>> mpActions;
extern std::map<EShowVariante, TPopupMenu*> mpMenus;


void CallAction(std::wstring const& strComponentName);
TPopupMenu* Find_Popup(EShowVariante mode);



#endif
