#ifndef FileAppProcessVCLH
#define FileAppProcessVCLH

#include "FileAppProcess.h"

class TProcessVCL : public TProcess {
   public:
      TProcessVCL(void);
      TProcessVCL(TProcessVCL const&) = delete;
      virtual ~TProcessVCL(void);

   private:
      void _xx_Check() { delete new TProcessVCL(); }
   };


#endif