#pragma once
#include <chrono>

#define ms std::chrono::microseconds
#define tp std::chrono::system_clock::time_point
#define now std::chrono::system_clock::now

using std::chrono::duration;

class TimeManager
{
public:
	TimeManager(int updatesPerSecond, int minSmoothDurationInMs, int maxUpdatesPerFrame);
	double getUpdateDeltaTime() const;
	unsigned int getMaxUpdatesPerFrame() const;
	bool isUpdateWaiting() const;
	double getGameDuration() const;
	int getUpdateCount() const;
	int getFrameCount() const;
	double getUpdateRateAverage() const;
	double getFrameRateAverage() const;
	double getUpdateRateInstantaneous() const;
	double getFrameRateInstantaneous() const;
	double getUpdateRateSmoothed() const;
	double getFrameRateSmoothed() const;
	void sleepUntilNextUpdate() const;
	void markNextUpdate();
	void markNextFrame();

	void adjustNextUpdateTimeToNow();

private:
	uint32_t m_FrameCount = 0;
	uint32_t m_UpdateCount = 0;
	const ms m_DeltaTime;

	double m_SmoothedFps = 60;
	double m_SmoothedUps = 60; 

	uint32_t m_UpdatesPerSecond = 60;
	const ms m_MinSmoothDuration;
	const uint32_t m_MaxUpdatesPerFrame = 10;
	tp m_StartTime;
	tp m_LastFrameTime;
	tp m_LastUpdateTime;
	tp m_NextUpdateTime;

	duration<double> m_LastFrameDuration;
	duration<double> m_LastUpdateDuration;
};

