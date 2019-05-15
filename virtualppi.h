#ifndef VIRTUAL_PPI_H
#define VIRTUAL_PPI_H

#include <stdint.h>

namespace VirtualPPI {

const uint16_t defaultRangeBinCount = 10;
const uint16_t defaultAzimuthAngleCount = 10;

class Image {
    private:
        double** values;

        uint16_t numberRangeBins;
        uint16_t numberAzimuthAngles;

        void allocateValues();
    public:
        
        // Constructors
        Image();
        Image(uint16_t rangeBins, uint16_t azimuthAngles);
        Image(const char * jsonFileName);

        // Destructor
        ~Image();

        // Functions
        uint16_t getAzimuthAngles(uint16_t);
        uint16_t getRangeBins(uint16_t);

        void writePNGImage(const char* filename);
        void writeJSON(const char* filename);

        void setValue(uint16_t azimuth, uint16_t rangeBin, double value);

        void readFromJSON(const char* filename);
};

}
#endif
