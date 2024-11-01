#include"StateStack.h"
#include<cassert>
#include"Foreach.h"

StateStack::StateStack(State::Context context)
	:mStack()
	, mPendingList()
	, mFactories()
	, mContext(context)
{

}


State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());
	return found->second();
}
void::StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++)
	{
		if (!(*itr)->handleEvent(event))
			return;
	}
	applyPendingChanges();
}
void::StateStack::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}
	applyPendingChanges();

}
void::StateStack::draw()
{
	FOREACH(State::Ptr & states, mStack)
		states->draw();
}
void::StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(push, stateID));
}
void::StateStack::popState()
{
	mPendingList.push_back(PendingChange(pop));
}
void::StateStack::clearState()
{
	mPendingList.push_back(PendingChange(clear));
}
bool::StateStack::isEmpty()const
{
	return mStack.empty();
}
void::StateStack::applyPendingChanges()
{
	FOREACH(PendingChange change, mPendingList)
	{
		switch (change.action)
		{
		case push:
			mStack.push_back(createState(change.stateID));
			break;
		case pop:
			mStack.pop_back();
			break;
		case clear:
			mStack.clear();
			break;
		}	
	}
	mPendingList.clear();
}
StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	:action(action)
	,stateID(stateID)
{

}
