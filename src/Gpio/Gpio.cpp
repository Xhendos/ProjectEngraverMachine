#define SYSFS           "/sys/class/gpio/"
#define EXPORT          SYSFS"export"
#define UNEXPORT        SYSFS"unexport"

#include "Gpio.hpp"

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
		return 0;
	}

	if(fprintf(fp, "%d\n", (int) pin) != std::to_string(pin).size() + 1)
	{
		perror("[GPIO] Failed to write");
		return 0;
	}

	if(fflush(fp))
	{
		perror("[GPIO] Failed to flush");
		perror("[GPIO] Maybe you tried to export a gpio pin that already has been exported?");
		return 0;
	}

	if(fclose(fp))
	{
		perror("[GPIO] Failed to close");
		return 0;
	}

    return pin;
}

unsigned char gpio::unexport_gpio(unsigned char pin)
{
    FILE *fp = fopen(UNEXPORT, "w");

    if(fp == NULL)
    {   
        perror("[GPIO] [unexport_gpio] Cannot open file " EXPORT);
        return 0;
    }   

    if(fprintf(fp, "%d\n", (int) pin) != std::to_string(pin).size() + 1)
    {   
        perror("[GPIO] Failed to write");
        return 0;
    }   

    if(fflush(fp))
    {   
        perror("[GPIO] Failed to flush");
        perror("[GPIO] Maybe you tried to unexport a gpio pin that already has been unexported?");
        return 0;
    }   

    if(fclose(fp))
    {   
        perror("[GPIO] Failed to close");
        return 0;
    }   

    return pin;
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

unsigned char gpio::set_edge(unsigned char pin, std::string edge)
{
    char path[50];
    snprintf(path, 50, SYSFS"gpio%d/edge", pin);

    FILE *fp = fopen(path, "w");

    if(fp == NULL)
    {   
        perror("[GPIO] [set_direction] Cannot open file");
        return 1;
    }   

    if(fprintf(fp, "%s\n", edge.c_str()) != edge.size() + 1)
    {   
        perror("[GPIO] [set_edge] Failed to write");
        return 1;
    }   

    if(fflush(fp))
    {   
        perror("[GPIO] [set_edge] Failed to flush");
        return 1;
    }   

    if(fclose(fp))
    {   
        perror("[GPIO] [set_edge] Failed to close");                                                                                              
        return 1;
    }   

    return 0;
}


unsigned char gpio::set_activelow(unsigned char pin, unsigned char low)
{
    char path[50];
    snprintf(path, 50, SYSFS"gpio%d/active_low", pin);

    FILE *fp = fopen(path, "w");

    if(fp == NULL)
    {   
        perror("[GPIO] [set_value] Cannot open file");
        return 1;
    }

    if(fprintf(fp, "%d\n", low) != std::to_string(low).size() + 1)
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



static std::string get_direction(unsigned char pin)
{
	char path[50];
	std::string dir;

	snprintf(path, 50, SYSFS"gpio%d/direction", pin);


	FILE *fp = fopen(path, "r");

	if(fp == NULL)
	{
		perror("[GPIO] [get_direction] Cannot open file");
		return "none";
	}

	if(fscanf(fp, "%s", dir) == -1)
	{
		perror("[GPIO] [get_direction] Cannot read from file");
		return "none";
	}

	return dir;
	
}

static unsigned char get_value(unsigned char pin)
{
	chat path[50];
	char c;
	
	snprintf(path, 50, SYSFS"gpio%d/value", pin);
	
	FILE *fp = fopen(path, "r");

	if(fp == NULL)
	{
		perror("[GPIO] [get_value] Cannot open file");
		return 2;
	}

	if(fscanf(fp, "%c", c) == -1)
	{
		perror("[GPIO] [get_value] Cannot get value");
		return 2;
	}
	

	return c;
}
