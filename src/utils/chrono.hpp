#ifndef __CHRONO_HPP__
#define __CHRONO_HPP__

#include <chrono>

namespace RT_CPU
{
	class Chrono
	{
		using Clock = std::chrono::high_resolution_clock;
		using Duration = std::chrono::duration<float>;

	public:
		void  start() { _begin = Clock::now(); }
		void  stop() { _end = Clock::now(); }
		float elapsedTime() const { return (std::chrono::duration_cast<Duration>(_end - _begin)).count(); }

	private:
		Clock::time_point _begin;
		Clock::time_point _end;
	};
}

#endif