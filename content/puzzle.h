#ifndef PUZZLE_H_
#define PUZZLE_H_
#include <time.h>
class CPuzzle{
    public:
        CPuzzle();
        ~CPuzzle();
        void start();
        void end();
        time_t get_time();
    private:
        time_t m_start_time;
        time_t m_end_time;
};
#endif
