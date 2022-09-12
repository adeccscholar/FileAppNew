#include "FileAppProcessVCL.h"
#include "FileDlgFormVCL.h"
#include "FileShowFormVCL.h"


TProcessVCL::TProcessVCL(void) : TProcess() {
   }


TProcessVCL::~TProcessVCL(void) {
   }

TMyForm TProcessVCL::CreateFileDlg(void) {
   TMyForm frm(new TfrmFileDlg(FileDlgProcess(), nullptr), true);
   return frm;
   }

TMyForm TProcessVCL::CreateShowFile(void) {
   TfrmFileShowVCL *fwform = new TfrmFileShowVCL(nullptr);
   TMyForm frm(fwform, true);
   return frm;
   }
