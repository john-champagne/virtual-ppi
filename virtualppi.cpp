#include "virtualppi.h"
#include "cJSON.h"

#include <iostream>
#include <cairo/cairo.h>
#include <math.h>

namespace VirtualPPI{

Image::Image() {
    numberAzimuthAngles = defaultAzimuthAngleCount;
    numberRangeBins = defaultRangeBinCount;
    allocateValues();
}

Image::Image(uint16_t nAzAngles, uint16_t nRangeBins) {
    numberAzimuthAngles = nAzAngles;
    numberRangeBins = nRangeBins;
    allocateValues();
}

void Image::allocateValues() {
    values = new double* [numberAzimuthAngles];
    for (int i = 0; i < numberAzimuthAngles; i++)
        values[i] = new double [numberRangeBins]; 
}

Image::~Image() {
    for (int i = 0; i < numberAzimuthAngles; i++)
        delete[] values[i];
    delete[] values;
}

void Image::writeJSON(const char* filename) {
    cJSON* root = cJSON_CreateObject();
    cJSON* vArray = cJSON_CreateArray();
    for (int i = 0; i < numberAzimuthAngles; i++) {
        cJSON* rangeBins = cJSON_CreateArray();
        for (int j = 0; j < numberRangeBins; j++)
           cJSON_AddItemToArray(rangeBins, cJSON_CreateNumber(values[i][j])); 
        cJSON_AddItemToArray(vArray, rangeBins);
    }
    cJSON_AddNumberToObject(root, "azimuthangles", numberAzimuthAngles);
    cJSON_AddNumberToObject(root, "rangebins", numberAzimuthAngles);
    cJSON_AddItemToObject(root, "values", vArray);
    std::cout << cJSON_Print(root);
    cJSON_Delete(root);
}


void Image::setValue(uint16_t azimuth, uint16_t rangeBin, double value){
    values[azimuth][rangeBin] = value;
}


void Image::writePNGImage(const char* filename) {
    int width = 5000;
    int height = 5000;
    int ox = width/2;
    int oy = height/2;
    double radius = width*0.5*0.75;
    double dr = radius / numberRangeBins;
    double dt = 2*M_PI / numberAzimuthAngles;

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width,height);
    cairo_t* cr = cairo_create(surface);

    for (int i = 0; i < numberAzimuthAngles; i++) {
        for (int j = 0; j < numberRangeBins; j++) {
            cairo_set_source_rgb (cr, pow(1-(double(j)/double(numberRangeBins)), 4), 0, 0);
            cairo_move_to (cr, ox + cos(i*dt)*(j*dr), oy+sin(i*dt)*(j*dr));
            cairo_line_to (cr, ox + cos(i*dt+dt)*(j*dr), oy+sin(i*dt+dt)*(j*dr));
            cairo_line_to (cr, ox + cos(i*dt+dt)*(j*dr+dr), oy+sin(i*dt+dt)*(j*dr+dr));
            cairo_line_to (cr, ox + cos(i*dt)*(j*dr+dr), oy+sin(i*dt)*(j*dr+dr));
            cairo_set_line_width (cr, 0);
            cairo_fill(cr);
        }
    }
    cairo_surface_write_to_png(surface, filename);

}

}

int main() {
    VirtualPPI::Image i = VirtualPPI::Image(400,500);
    i.setValue(0,1, 90);
    i.writePNGImage("test.png");
}
