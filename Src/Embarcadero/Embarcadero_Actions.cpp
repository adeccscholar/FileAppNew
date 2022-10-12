//---------------------------------------------------------------------------

#pragma hdrstop

#include "Embarcadero_Actions.h"

#include <exception>
#include <regex>

//---------------------------------------------------------------------------
#pragma package(smart_init)

std::map<std::wstring, std::function<void ()>> mpActions;

// specified prefixes are removed from the name. The same applies to up to two-digit 
// numbers that are optional.  Between them can be an underscore
void CallAction(std::wstring const& strComponentName) {
   static const std::wregex parser(L"^((act|mitem|btn|tbtn))([A-Za-z]+)(_?)([0-9]{0,2})$");
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

