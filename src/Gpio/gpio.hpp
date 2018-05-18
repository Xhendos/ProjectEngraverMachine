#ifndef GPIO_H
#define GPIO_H

#include <string>

extern std::string DIR_IN;
extern std::string DIR_OUT;

extern std::string EDGE_RISE;
extern std::string EDGE_FALL;
extern std::string EDGE_NONE;


class gpio
{
    private:
        gpio() {}                                                   /* Disallow to create an instance */

    public:
    	static unsigned char export_gpio(unsigned char pin);
		static unsigned char set_direction(unsigned char pin, std::string direction);
		static unsigned char set_value(unsigned char pin, unsigned char value);
};

#endif
