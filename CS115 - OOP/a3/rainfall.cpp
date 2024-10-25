
#include "rainfall.h"

#include <iostream>

Rainfall::Rainfall(): maxRainfall(0), minRainfall(0), avgRainfall(0)
{}

Rainfall::Rainfall(int maxRain, int minRain, int avgRain) 
{
  maxRainfall = maxRain;
  minRainfall = minRain;
  avgRainfall = avgRain;
}

Rainfall::Rainfall(const Rainfall& Rain)
{
  maxRainfall = Rain.maxRainfall;
  minRainfall = Rain.minRainfall;
  avgRainfall = Rain.avgRainfall;
}

Rainfall::~Rainfall()
{}

void Rainfall::print() const
{ 
  std::cout << "Rainfall(Min): " << minRainfall << std::endl;
  std::cout << "Rainfall(Max): " << maxRainfall << std::endl;
  std::cout << "Rainfall(Avg): " << avgRainfall << std::endl;
}

int Rainfall::getMaxRainfall() const
{
  return maxRainfall;
}

int Rainfall::getMinRainfall() const
{
  return minRainfall;
}

int Rainfall::getAvgRainfall() const
{
  return avgRainfall;
}

void Rainfall::setMaxRainfall(int newMaxRain)
{
  maxRainfall = newMaxRain;
}

void Rainfall::setMinRainfall(int newMinRain)
{
  minRainfall = newMinRain;
}

void Rainfall::setAvgRainfall(int newAvgRain)
{
  avgRainfall = newAvgRain;
}
