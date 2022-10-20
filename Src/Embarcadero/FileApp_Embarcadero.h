//---------------------------------------------------------------------------

#ifndef FileApp_EmbarcaderoH
#define FileApp_EmbarcaderoH
//---------------------------------------------------------------------------

#include "Embarcadero_Actions.h"

#if defined BUILD_WITH_VCL 
   #include "FileAppProcessVCL.h"
   using used_Process   = TProcessVCL;
#elif defined BUILD_WITH_FMX
   #include "FileAppProcessFMX.h"
   using used_Process   = TProcessFMX;
#else
   #error wrong framework for use this file
#endif

#include "MyForm.h"
#include "MyFileDlg.h"

extern used_Process proc;
extern std::map<EShowVariante, used_PopupMenu*> mpMenus;

used_PopupMenu* Find_Popup(EShowVariante mode);

template <typename used_type>
void SetCallActions(mapActions& actions, used_type* frm) {
   actions = {
     // Main- Actions
     { L"Select",              [frm]() { proc.SelectWithDirDlg(); 
                                        } },
     { L"Count",               [frm]() { proc.CountAction();
                                          frm->lvOutput->PopupMenu = Find_Popup(proc.GetShowMode()); 
                                        } },
     { L"Parse",               [frm]() { proc.ParseAction();
                                          frm->lvOutput->PopupMenu = Find_Popup(proc.GetShowMode()); 
                                        } },
     { L"Show",                [frm]() { proc.ShowAction(); 
                                          frm->lvOutput->PopupMenu = Find_Popup(proc.GetShowMode()); 
                                        } },
     { L"CloseApp",            [frm]() { frm->Close(); 
                                        } },
     { L"Test",                [frm]() { proc.Test3(); 
                                        } },

     { L"AddExtention",        [frm]() { proc.AddExtention(); } },
     { L"ChgExtention",        [frm]() { proc.ChangeSelectedExtentions(); } },
     { L"DelExtention",        [frm]() { proc.DeleteExtentions(true); } },
     { L"DelAllExtentions",    [frm]() { proc.DeleteExtentions(false); } },

     // Project parse 
     { L"ProjectFile",         [frm]() { proc.OpenProjectFile(); } },
     { L"CppFile",             [frm]() { proc.OpenSourceFile(); } },
     { L"HeaderFile",          [frm]() { proc.OpenHeaderFile(); } },
     { L"ResFile",             [frm]() { proc.OpenResFile(); 
                                        } },
     { L"CntAllFileRows",      [frm]() { std::ostringstream os;
                                          proc.CountFileRowsForProjects(os, false);
                                          TMyFileDlg::Message(EMyMessageType::information, "FileApp - Information", os.str());
                                        } },
     { L"CntSelectedFileRows", [frm]() { std::ostringstream os;
                                          proc.CountFileRowsForProjects(os, true);
                                          TMyFileDlg::Message(EMyMessageType::information, "FileApp - Information", os.str());
                                        } },
     // Show Files
     { L"ShowFile",            [frm]() { proc.OpenViewFile(); } }
     };
   }

#endif

