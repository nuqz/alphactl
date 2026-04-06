#include "Controller.hpp"

Controller ctl;

extern "C" void app_main()
{
    ctl.run();
}
