#include "FileAppProcessQT.h"
#include "FileDlgClass.h"

TProcessQT::TProcessQT(void) : TProcess() {
}


TProcessQT::~TProcessQT(void) {
}

TMyForm TProcessQT::CreateFileDlg(void) {
   TMyForm frm(new FileDlgClass(FileDlgProcess(), nullptr), true);
   return frm;
}
