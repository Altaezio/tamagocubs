#include "Event.h"
#include "EventCallback.h"

Event::Event() { }
Event::~Event() { }

void Event::addListener(IEventCallback* action)
{
	CallbackArray::iterator position = find(actions.begin(), actions.end(), action);

	if (position != actions.end())
	{
		return;
	}

	actions.push_back(action);
}

void Event::removeListener(IEventCallback* action)
{
	CallbackArray::iterator position = find(actions.begin(), actions.end(), action);

	if (position == actions.end())
	{
		return;
	}

	actions.erase(position);
}

void Event::fire()
{
	for (IEventCallback* action : actions)
	{
		if (action)
		{
			(*action)();
		}
	}
}