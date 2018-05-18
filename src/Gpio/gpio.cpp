#define SYSFS           "/sys/class/gpio/"
#define EXPORT          SYSFS"export"
#define UNEXPORT        SYSFS"unexport"

#define DIRECTION(s)    SYSFS"gpio"#s"/direction"
#define VALUE(s)        SYSFS"gpio"#s"/value"
#define EDGE(s)         SYSFS"gpio"#s"/edge"
#define ACTIVE_LOW(s)   SYSFS"gpio"#s"/active_low"

#include "gpio.hpp"

#include <stdio.h>                                              /* fopen(), fwrite() */
#include <stdlib.h>                                             /* exit() */

//#include <string>                                               
#include <cstring>                                              /* strerror() */

std::string DIR_IN      = "in";
std::string DIR_OUT     = "out";

std::string EDGE_RISE   = "rising";
std::string EDGE_FALL   = "falling";
std::string EDGE_NONE   = "none";


unsigned char gpio::export_gpio(gpio::gpio_header header)
{
    FILE *fp = fopen(EXPORT, "w");                              /* Gets a file descriptor for write-only to the export file */
    
    if(fp == NULL)                                              /* If we could not open the export file, we should exit the program */ 
    {
        perror("Failed to open /sys/class/gpio/export");
        exit(1);                                                
    }

                                                                
    if(fprintf(fp, "%d", header.pin) != std::to_string(header.pin).size())
    {   
        perror("Cannot write to export");
        exit(1);
    }

    
    if(fclose(fp))
    {
        perror("Cannot close file pointer (1)");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    
    return 0;
}
