//==========================================================================
// Tests structures class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 5/16/2016
// Last modified: 5/16/2016
//==========================================================================

#ifndef DISPLAYSTRUCTURES_H
#define DISPLAYSTRUCTURES_H

// C/C++ Libs
#include <iostream>
#include <string.h>
 
class DisplayStructures
{
  public:
    // Contructors and destructor
     DisplayStructures(std::string deviceType, std::string testType);
    ~DisplayStructures();

     void SetOnlineMonitoringNumberOfPoints(unsigned int numberOfPoints);

    struct Legend {
      char  *entryName;
      double coordinates[4];
    };

    struct Axis {
      char *label;
      double maximum;
    };

    struct GraphSettings {
      char *graphName; 
      Legend *legend;
      Axis *xAxis, *yAxis;
    };
    
    GraphSettings **graphSettings;
    
    unsigned int numberOfGraphsToPlot;
    unsigned int numberOfPointsPerGraph;

  private:
    void Initialize(std::string deviceType, std::string testType);
    void Destroy();

    void BuildDeviceStructures(unsigned int width);

    void InitializeDifferentialAmplifiers(std::string testType);
    void InitializeDacs(std::string testType);
    void InitializeDcdcConverters(std::string testType);
    void InitializeAdcs(std::string testType);
    void InitializeDifferentialToSingleEndedDriver(std::string testType);
    void InitializeComparator(std::string testType);
    void InitializeResistanceToDigital(std::string testType);
    void InitializeI2cDriver(std::string testType);
    void InitializeOnlineMonitoring(std::string testType);
    void InitializeIoExpander(std::string testType);

};
 
#endif
