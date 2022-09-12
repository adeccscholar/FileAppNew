#ifndef FileAppProcessFMXH
#define FileAppProcessFMXH

#include "FileAppProcess.h"

class TProcessFMX : public TProcess {
   public:
      TProcessFMX(void);
      TProcessFMX(TProcessFMX const&) = delete;
      virtual ~TProcessFMX(void);

      virtual TMyForm CreateFileDlg(void) override;
      virtual TMyForm CreateShowFile(void) override;
   private:
      void _xx_Check() { delete new TProcessFMX(); }
   };


#endif