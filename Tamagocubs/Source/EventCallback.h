#pragma once

template<class... Args>
class IEventCallback
{
public:
	virtual void operator() (Args... args) = 0;
};

template<typename T, class... Args>
class EventCallback : public IEventCallback<Args...>
{
public:
	EventCallback(T* instance, void (T::* function)(Args... args))
		: instance(instance), function(function)
	{
	}
	virtual void operator () (Args... args) override
	{
		(instance->*function)(args...);
	}

private:
	void (T::* function)(Args... a);
	T* instance;
};