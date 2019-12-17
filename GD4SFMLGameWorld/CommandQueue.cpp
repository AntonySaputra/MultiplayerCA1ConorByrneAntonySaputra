//Antony Saputra D00197615
//Conor Byrne D00185566

#include "CommandQueue.hpp"

void CommandQueue::push(const Command& command)
{
	mQueue.push(command);
}

Command CommandQueue::pop()
{
	Command command = mQueue.front();
	mQueue.pop();
	return Command(command);
}

bool CommandQueue::isEmpty() const
{
	return mQueue.empty();
}
