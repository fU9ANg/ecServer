
/**
 * @ingroup framework
 * @{
 */

#include <stdio.h>
#include "ThreadPool.h"
#include "AtomicT.h"
#include "PoolT.h"
#include "Buf.h"

#include "ProcessManager.h"

// it's rock
int main(int argc, char* argv[])
{
    ProcessManager* x = new ProcessManager();
    if (x == NULL)
    {
        cout << "ERROR: ProcessManager is NULL" << endl;
        abort();
    }
    x->process_logic(argc, argv);
    delete x;
    return 0;
}
///@}
