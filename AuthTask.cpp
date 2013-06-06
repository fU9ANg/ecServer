
#include "AuthTask.h"
#include "confirm.hpp"

AuthTask::AuthTask ()
{
    m_oldtime = time (NULL);
}

AuthTask::~AuthTask ()
{
}

int AuthTask::work ()
{
    while (true)
    {
        time_t now_time = time (NULL);
        if ((now_time - m_oldtime) > INTERVAL_TIME)
        {
            m_oldtime = now_time;
            if (Confirm().confirm())
                cout << "auth success" << endl;
            else {
                cout << "authorize failed" << endl;
                exit (1);
            }
        }
        sleep (SLEEP_TIME);
    }

    return (0);
}
