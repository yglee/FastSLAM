#include <stdio.h>
#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
// MyTimer
////////////////////////////////////////////////////////////////////////////////

// little helper class to time stuff
// this should be in a utilities header file
class MyTimer
{
public:
  MyTimer()
  {
  }
  
  void Start()
  {
    gettimeofday(&tv1, NULL);
  }
  double Stop()
  {
    gettimeofday(&tv2, NULL);
    int sec = tv2.tv_sec - tv1.tv_sec;
    int usec = tv2.tv_usec - tv1.tv_usec;
    
    if (usec < 0)
    {
      sec--;
      usec = 1000000 + usec;
    }
    
    return (double)sec + (double)usec / 1000000.0;
  }
  void Print(const char *label)
  {
    int sec = tv2.tv_sec - tv1.tv_sec;
    int usec = tv2.tv_usec - tv1.tv_usec;
    
    if (usec < 0)
    {
      sec--;
      usec = 1000000 + usec;
    }
    printf("%s took %d sec %d usec\n", label, sec, usec);
  }
  
private:
  struct timeval tv1, tv2;
};

