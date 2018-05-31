# GPIO 

General Purpose Input/Output library using the [sysfs](https://www.kernel.org/doc/Documentation/gpio/sysfs.txt) method.
This library works with the Linux kernel and can be used for any project that requires input and/or output through the general purpose pins.

### How it works

Before using a GPIO pin, this pin must be mapped to the pseudo filesystem. There is a file called __/sys/class/gpio/export__. Any number written to this file (and has not been mapped already) will result in a directory called __/sys/class/gpio/gpioX/__ where X is the number that has been written to the __export__ file.

Once a GPIO pin has been exported and the directory __gpioX__ has been created, there will be a couple of interesting files inside this directory

1. *diretion*  This is "out" or "in"
2. *value* This is "1" or "0"
3. *edge* This could be "rising", "falling" or "none"
4. *active_low* This could be "1" or "0"

The implementation of this library simply writes the correct information to the correct file.
The implementation does error checks aswell to see if it has succesfully written to a file or something went wrong.

### Functions

| Name                | Description                                  | Return success             | Return failure |
|---------------------|----------------------------------------------|----------------------------|----------------|
| export_gpio(n)      | Export pin number *n* to the export file     | The pin number is returned | 0 is returned  |
| unexport_gpio(n)    | Unexport pin number *n* to the unexport file | The pin number is returned | 0 is returned  |
| set_direction(n, d) | Set pin *n* to the direction *d*             | 0 is returned              | 1 is returned  |
| set_value(n, v)     | Set pin *n* its logical value                | 0 is returned              | 1 is returned  |
| set_edge(n, e)      | Set pin *n* its *edge*                       | 0 is returned              | 1 is returned  |
| set_activelow(n, l) | Set pin *n* its active_low level             | 0 is returned              | 1 is returned  |



