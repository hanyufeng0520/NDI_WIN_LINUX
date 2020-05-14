#include "IConfig.h"
#include "Configurator.h"
#include "../Lib.Base/locker.h"

static IConfig* mSInstance = nullptr;
static Locker	g_lock;
IConfig * IConfig::getInstance()
{
	if (mSInstance != nullptr)
		return mSInstance;

	LockHolder lock(g_lock);
	if (mSInstance == nullptr)
		mSInstance = new Configurator();
	return mSInstance;
}

void IConfig::releaseInstance()
{
	LockHolder lock(g_lock);
	if (mSInstance)
	{
		delete mSInstance;
		mSInstance = nullptr;
	}
}