//---------------------------------------------------------------------------

#ifndef Embarcadero_ActionsH
#define Embarcadero_ActionsH
//---------------------------------------------------------------------------

#include "MyForm.h"

#include <string>
#include <map>
#include <functional>



extern std::map<std::wstring, std::function<void ()>> mpActions;

void CallAction(std::wstring const& strComponentName);

//fw_Form


#endif
