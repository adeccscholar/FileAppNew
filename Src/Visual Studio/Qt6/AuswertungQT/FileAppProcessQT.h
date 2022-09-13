#pragma once

#include <FileAppProcess.h>
#include <MyForm.h>

class TProcessQT : public TProcess {
public:
   TProcessQT(void);
   TProcessQT(TProcessQT const&) = delete;
   virtual ~TProcessQT(void);

   //virtual TMyForm CreateFileDlg(void) override;
   //virtual TMyForm CreateShowFile(void) override;

private:
   void _xx_Check() { delete new TProcessQT(); }
};
