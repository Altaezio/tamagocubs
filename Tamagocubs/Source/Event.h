#pragma once
#include <vector>
#include "EventCallback.h"


template<class... Args>
class Event
{
public:
	Event() {};
	~Event() {};

	void addListener(IEventCallback<Args...>* action)
	{
		typename CallbackArray::iterator position = find(actions.begin(), actions.end(), action);
		if (position != actions.end())
		{
			return;
		}

		actions.push_back(action);
	}

	void removeListener(IEventCallback<Args...>* action)
	{
		typename CallbackArray::iterator position = find(actions.begin(), actions.end(), action);
		if (position == actions.end())
		{
			return;
		}

		actions.erase(position);
	}

	void fire(Args... args)
	{
		for (auto action : actions)
		{
			if (action)
			{
				(*action)(args...);
			}
		}
	}

private:
	typedef std::vector<IEventCallback<Args...>*> CallbackArray;
	CallbackArray actions;
};
