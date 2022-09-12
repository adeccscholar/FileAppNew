#include "FileAppProcessFMX.h"
#include "FileDlgFormFMX.h"
#include "FileShowDlgFMX.h"

TProcessFMX::TProcessFMX(void) : TProcess() {
   }


TProcessFMX::~TProcessFMX(void) {
   }

TMyForm TProcessFMX::CreateFileDlg(void) {
   TMyForm frm(new TfrmFileDlgFMX(FileDlgProcess(), nullptr), true);
   return frm;
   }

TMyForm TProcessFMX::CreateShowFile(void) {
   TMyForm frm(new TfrmFileShowDlg(nullptr), true);
   return frm;
   }

