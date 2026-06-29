#pragma once

namespace Vex
{
    class Main
    {
      public:
        static void Run(int argc, char* argv[]);

      private:
        static void InitMainContext();
        static void DestroyMainContext();
    };
} // namespace Vex
