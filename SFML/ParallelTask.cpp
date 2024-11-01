#include "ParallelTask.h"
ParallelTask::ParallelTask()
	:mThread(&ParallelTask::runTask, this)
	,mFinished(false)
{

}
void::ParallelTask::execute()
{
	mFinished = false;
	mElapsedTime.restart();
	mThread.launch();
}
bool::ParallelTask::isFinished()
{
	sf::Lock Lock(mMutex);
	return mFinished;
}
float::ParallelTask::getCompletion()
{
	sf::Lock Lock(mMutex);
	return mElapsedTime.getElapsedTime().asSeconds() / 5.f;
}
void::ParallelTask::runTask()
{
	bool ended = false;
	while (!ended)
	{
		sf::Lock lock(mMutex);
		if (mElapsedTime.getElapsedTime().asSeconds() >= 5.f)
			ended = true;
	}
	{
		sf::Lock lock(mMutex);
		mFinished = true;
	}
}