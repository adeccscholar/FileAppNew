//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("MainForm.cpp", frmMain);
USEFORM("..\..\..\adecc_FileDlg\Src\Embarcadero\VCL\FileShowFormVCL.cpp", frmFileShowVCL);
USEFORM("..\..\..\adecc_FileDlg\Src\Embarcadero\VCL\FileDlgFormVCL.cpp", frmFileDlg);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TfrmMain), &frmMain);
       Application->CreateForm(__classid(TfrmFileShowVCL), &frmFileShowVCL);
       Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
