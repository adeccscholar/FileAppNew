//---------------------------------------------------------------------------

#ifndef Embarcadero_ActionsH
#define Embarcadero_ActionsH
//---------------------------------------------------------------------------

#if defined BUILD_WITH_VCL 
   #include <Vcl.Forms.hpp>
   #include <Vcl.Menus.hpp>
   using TForm          = Vcl::Forms::TForm;
   using used_Form      = TForm;
   using used_PopupMenu = Vcl::Menus::TPopupMenu;
   using used_MenuItem  = Vcl::Menus::TMenuItem;
#elif defined BUILD_WITH_FMX
   #include <Fmx.Forms.hpp>
   #include <Fmx.Menus.hpp>
   using TForm          = Fmx::Forms::TForm;
   using used_Form      = TForm;
   using used_PopupMenu = Fmx::Menus::TPopupMenu;
   using used_MenuItem  = Fmx::Menus::TMenuItem;
#else
   #error wrong framework for use this file
#endif

#include <string>
#include <map>
#include <functional>

using mapActions = std::map<std::wstring, std::function<void ()>>;
extern mapActions mpActions;

void CallAction(std::wstring const& strComponentName);

#endif
