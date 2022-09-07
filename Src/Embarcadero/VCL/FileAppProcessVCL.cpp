#include "FileAppProcessVCL.h"
#include "FileDlgFormVCL.h"

TProcessVCL::TProcessVCL(void) : TProcess() {
   }


TProcessVCL::~TProcessVCL(void) {
   }

TMyForm TProcessVCL::CreateFileDlg(void) {
   TMyForm frm(new TfrmFileDlg(FileDlgProcess(), nullptr), true);
   return frm;
   }
