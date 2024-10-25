
#ifndef rainfall_h
#define rainfall_h

class Rainfall
{
private:
  int maxRainfall;
  int minRainfall;
  int avgRainfall;

public:
  Rainfall(); /* Default Constructor */
  Rainfall(int, int, int); /* Constructor */
  Rainfall(const Rainfall&); /* Copy Constructor */
  ~Rainfall(); /* Destructor */
  void print() const; /* Print the maxRainfall, minRainfall, and avgRainfall values of the object */
  
  /* Getters */
  int getMaxRainfall() const;
  int getMinRainfall() const;
  int getAvgRainfall() const;
  
  /* Setters */
  void setMaxRainfall(int);
  void setMinRainfall(int);
  void setAvgRainfall(int);
};

#endif /* rainfall_h */