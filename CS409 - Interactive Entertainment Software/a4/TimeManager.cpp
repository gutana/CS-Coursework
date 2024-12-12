#include "TimeManager.h"
#include "Sleep.h"
#include <iostream>

TimeManager::TimeManager(int updatesPerSecond, int minSmoothDurationInMs, int maxUpdatesPerFrame)
    : m_UpdatesPerSecond(updatesPerSecond),
    m_DeltaTime(1000000 / updatesPerSecond),
    m_MinSmoothDuration(minSmoothDurationInMs),
    m_MaxUpdatesPerFrame(maxUpdatesPerFrame)
{
    m_FrameCount = 0;
    m_UpdateCount = 0;

    // arbitrary plausible values
    m_SmoothedFps = 60; 
    m_SmoothedUps = 60;

    // initialize timing
    m_StartTime = now();

    m_NextUpdateTime = m_StartTime;
    m_LastUpdateDuration = m_DeltaTime;
    m_LastFrameTime = m_StartTime;

    m_LastFrameTime = m_StartTime;
    m_LastFrameDuration = m_DeltaTime;
}

double TimeManager::getUpdateDeltaTime() const
{
	double a = duration<float>(m_DeltaTime).count();
    return a;
}

unsigned int TimeManager::getMaxUpdatesPerFrame() const
{
    return m_MaxUpdatesPerFrame;
}

bool TimeManager::isUpdateWaiting() const
{
	return (m_NextUpdateTime <= now());
}

double TimeManager::getGameDuration() const
{
    return duration<float>(now() - m_StartTime).count();
}

int TimeManager::getUpdateCount() const
{
    return m_UpdateCount;
}

int TimeManager::getFrameCount() const
{
    return m_FrameCount;
}

double TimeManager::getUpdateRateAverage() const
{
    return m_UpdateCount / getGameDuration();
}

double TimeManager::getFrameRateAverage() const
{
    return m_FrameCount / getGameDuration();
}

double TimeManager::getUpdateRateInstantaneous() const
{
    double instantUps = 1.0f / duration<double>(m_LastUpdateDuration).count();
    if (instantUps > 1000)
    {
        return m_UpdatesPerSecond; // return a normal number when we freak out and update very quickly in sequence
    }
    return 1.0f / duration<float>(m_LastUpdateDuration).count();
}

double TimeManager::getFrameRateInstantaneous() const
{
    return 1.0f / duration<double>(m_LastFrameDuration).count();
}

double TimeManager::getUpdateRateSmoothed() const
{
    return m_SmoothedUps;
}

double TimeManager::getFrameRateSmoothed() const
{
    return m_SmoothedFps;
}

void TimeManager::sleepUntilNextUpdate() const
{	
    tp currentTime = now();
	if (currentTime < m_NextUpdateTime)
	{
		duration<float> sleepTime = m_NextUpdateTime - currentTime;
        std::cout << "Sleeping for " << std::fixed << sleepTime.count() << " seconds\n";
		sleep(duration<float>(sleepTime).count());
	}
}

void TimeManager::markNextUpdate()
{
	m_UpdateCount++;
	m_NextUpdateTime += m_DeltaTime;
	tp currentTime = now();

	m_LastUpdateDuration = currentTime - m_LastUpdateTime;
	if (m_LastUpdateDuration < m_MinSmoothDuration)
		m_LastUpdateDuration = m_MinSmoothDuration;
	m_LastUpdateTime = currentTime;

	double lastUpdateSeconds = duration<float>(m_LastUpdateDuration).count();
	double instantUps = 1.0f / lastUpdateSeconds;
    if (instantUps > 1000.0f)
    {
        return;
    }
	m_SmoothedUps = 0.95f * m_SmoothedUps + 0.05f * instantUps;
}

void TimeManager::markNextFrame()
{	
    m_FrameCount++;
	tp currentTime = now();

	m_LastFrameDuration = currentTime - m_LastFrameTime;
	if (m_LastFrameDuration < m_MinSmoothDuration)
		m_LastFrameDuration = m_MinSmoothDuration;
	m_LastFrameTime = currentTime;

	double lastFrameSeconds = duration<float>(m_LastFrameDuration).count();
	double instantFps = 1.0f / lastFrameSeconds;
	m_SmoothedFps = 0.95f * m_SmoothedFps + 0.05f * instantFps;
}

void TimeManager::adjustNextUpdateTimeToNow()
{
    m_NextUpdateTime = now();
}
