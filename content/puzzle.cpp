#include "puzzle.h"
CPuzzle::CPuzzle():
m_start_time(0),m_end_time(0){
}

CPuzzle::~CPuzzle(){
}

void CPuzzle::start() {
    if (0 == m_start_time) {
        m_start_time = time(NULL);
    }
}

void CPuzzle::end() {
    if (0 == m_end_time) {
        m_end_time = time(NULL);
    }
}

time_t CPuzzle::get_time() {
    return m_end_time - m_start_time;
}
