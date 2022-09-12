#include "FileAppProcessQT.h"
#include "FileDlgClass.h"
#include "FileShowClass.h"

TProcessQT::TProcessQT(void) : TProcess() {
}


TProcessQT::~TProcessQT(void) {
}

TMyForm TProcessQT::CreateFileDlg(void) {
   TMyForm frm(new FileDlgClass(FileDlgProcess(), nullptr), true);
   return frm;
}

TMyForm TProcessQT::CreateShowFile(void) {
   TMyForm frm(new FileShowClass(nullptr), true);
   return frm;
}

// CreateShowFile