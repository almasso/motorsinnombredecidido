#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <chrono>

class TimeManager {
private:
	std::chrono::system_clock::time_point previous;

public:
    void init();
	void update();
};

#endif