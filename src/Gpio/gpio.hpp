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
        struct gpio_header
        {
            unsigned char pin;                                      /* Pin number */
            std::string direction;                                  /* Is 'out' or 'in' */
            unsigned char value;                                    /* If direction is 'out', this can be written '0' or '1' */
            std::string edge;                                       /* Is 'none', 'rising' or 'falling' */
            unsigned char active_low;                               /* Is '0' [means false] or '1' [means true] */
        };

    
        static unsigned char export_gpio(gpio_header header);      
};

#endif
