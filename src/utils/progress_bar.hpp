#ifndef __PROGRESS_BAR_HPP__
#define __PROGRESS_BAR_HPP__

#include "utils/defines.hpp"

#include <mutex>
#include <string>
#include <iostream>

namespace RT_CPU
{
	class ProgressBar
	{
	public:
		ProgressBar() {}
		~ProgressBar() {}

		void start(const int p_nbTasks, const int p_barWidth)
		{
			_barWidth = glm::max<int>(2, p_barWidth - 2);
			_nbTasks = glm::max<int>(1, p_nbTasks - 1);

			std::cout << "Warning ! Console progress bar impacts performance !" << std::endl;
			_nbTasksDone = 0;

			std::cout << "\r[";
			for(int i=0; i <_barWidth ;i++) std::cout << " ";
			std::cout << "] 0.00 %";
		}

		void next(const int n = 1)
		{
			_mutex.lock();
			_nbTasksDone += n;
			int			nbToDisplay = _nbTasksDone * _barWidth / _nbTasks;
			std::string strPercent = std::to_string(_nbTasksDone * 100.f / _nbTasks);
			std::string percent(strPercent.begin(), strPercent.begin() + 4);
			
			std::cout << "\r[";
			for (int i=0; i<_barWidth ;i++) std::cout << (i < nbToDisplay ? "-" : " ");
			std::cout << "] " << percent << " % ";

			std::cout.flush();
			_mutex.unlock();
		}

		void stop()
		{
			_nbTasksDone = 0;
			std::cout << std::endl;
		}

	private:
		int _barWidth = 50;
		int _nbTasksDone = 0;
		int _nbTasks = 1;

		std::mutex _mutex;
	};
}

#endif