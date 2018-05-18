#define SYSFS           "/sys/class/gpio/"
#define EXPORT          SYSFS"export"
#define UNEXPORT        SYSFS"unexport"

#define DIRECTION(s)    SYSFS"gpio"#s"/direction"
#define VALUE(s)        SYSFS"gpio"#s"/value"
#define EDGE(s)         SYSFS"gpio"#s"/edge"
#define ACTIVE_LOW(s)   SYSFS"gpio"#s"/active_low"

#include "gpio.hpp"

#include <stdio.h>
#include <stdlib.h>                                             /* exit() */

#include <fcntl.h>												/* O_WRONLY */
#include <cstring>                                              /* strerror() */

#include <unistd.h>												/* write(), close() */
std::string DIR_IN      = "in";
std::string DIR_OUT     = "out";

std::string EDGE_RISE   = "rising";
std::string EDGE_FALL   = "falling";
std::string EDGE_NONE   = "none";


unsigned char gpio::export_gpio(unsigned char pin)
{
	FILE *fp = fopen(EXPORT, "w");

	printf("%s\n", EXPORT);
	if(fp == NULL)
	{
		perror("Cannot open " EXPORT);
		return 1;
	}

	if(fprintf(fp, "%d\n", (int) pin) != std::to_string(pin).size() + 1)
	{
		perror("[GPIO] Failed to write");
		return 1;
	}

	if(fflush(fp))
	{
		perror("[GPIO] Failed to flush");
		perror("[GPIO] Maybe you tried to export a gpio pin that already has been exported?");
		return 1;
	}

	if(fclose(fp))
	{
		perror("[GPIO] Failed to close");
		return 1;
	}

    return 0;
}


unsigned char gpio::set_direction(unsigned char pin, std::string direction)
{
	char path[50];
	snprintf(path, 50, SYSFS"gpio%d/direction", pin);

	FILE *fp = fopen(path, "w");

	if(fp == NULL)
	{
		perror("[GPIO] [set_direction] Cannot open file");
		return 1;
	}

	if(fprintf(fp, "%s\n", direction.c_str()) != direction.size() + 1)
	{
		perror("[GPIO] [set_direction] Failed to write");
		return 1;
	}

	if(fflush(fp))
	{
		perror("[GPIO] [set_direction] Failed to flush");
		return 1;
	}

	if(fclose(fp))
	{
		perror("[GPIO] [set_direction] Failed to close");
		return 1;
	}

	return 0;
}

unsigned char gpio::set_value(unsigned char pin, unsigned char value)
{
    char path[50];
    snprintf(path, 50, SYSFS"gpio%d/value", pin);

    FILE *fp = fopen(path, "w");

    if(fp == NULL)
    {
        perror("[GPIO] [set_value] Cannot open file");
        return 1;
    }

    if(fprintf(fp, "%d\n", value) != std::to_string(value).size() + 1)
    {
        perror("[GPIO] [set_value] Failed to write");
        return 1;
    }

    if(fflush(fp))
    {
        perror("[GPIO] [set_value] Failed to flush");
        return 1;
    }

    if(fclose(fp))
	{
        perror("[GPIO] [set_value] Failed to close");
        return 1;
    }

    return 0;

}
