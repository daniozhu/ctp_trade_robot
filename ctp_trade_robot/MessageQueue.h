#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

template<class T>
class MessageQueue
{
public:
	void Push(const T& msg);
	T Pop();

	MessageQueue(const MessageQueue&) = delete;
	MessageQueue& operator = (const MessageQueue&) = delete;

	static MessageQueue* Get();

private:
	MessageQueue() {}

	std::queue<T>				m_queue;
	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;
};

template<class T>
MessageQueue<T>* MessageQueue<T>::Get()
{
	static MessageQueue<T> s_msgQueue;
	return &s_msgQueue;
}

template<class T>
void MessageQueue<T>::Push(const T& msg)
{
	std::unique_lock<std::mutex> lk(m_mutex);
	m_queue.push(msg);

	// notify all waiting threads that a new message has been added to the queue.
	m_cond.notify_all();
}

template<class T>
T MessageQueue<T>::Pop()
{
	std::unique_lock<std::mutex> lk(m_mutex);
	m_cond.wait(lk, [this]() {return !this->m_queue.empty(); });
	
	T msg = m_queue.front();
	m_queue.pop();
	
	return msg;  // move?
}