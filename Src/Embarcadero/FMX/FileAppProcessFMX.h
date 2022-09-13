#ifndef FileAppProcessFMXH
#define FileAppProcessFMXH

#include "FileAppProcess.h"

class TProcessFMX : public TProcess {
   public:
      TProcessFMX(void);
      TProcessFMX(TProcessFMX const&) = delete;
      virtual ~TProcessFMX(void);

   private:
      void _xx_Check() { delete new TProcessFMX(); }
   };


#endif