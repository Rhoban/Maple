#include <stdlib.h>
#include <wirish/wirish.h>
#include <servos.h>
#include <terminal.h>
#include <main.h>
#if defined(DXL_AVAILABLE)
#include <dxl.h>
#endif
#if defined(RHOCK)
#include <rhock/vm.h>
#endif
 
// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain()
{
    init();
}

int main(void)
{
    setup();

#if defined(DXL_AVAILABLE)
    // Enabling asychronous dynamixel
    dxl_async(true);
#endif

#if defined(RHOCK)
    // Initializing rhock
    rhock_vm_init();
#endif

    while (true) {
        loop();
    }

    return 0;
}

