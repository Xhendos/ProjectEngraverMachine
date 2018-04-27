#include <linux/videodev2.h>

#include <iostream>

/* Support rgb24 (8 bit colour) pixel format  https://linuxtv.org/downloads/v4l-dvb-apis/uapi/v4l/pixfmt-packed-rgb.html */
/* Usefull quick search http://v4l.videotechnology.com/dwg/v4l2.html */


/*
 * ioctl() requests used in this software
 *
 * VIDIOC_QUERYCAP	-> Query the device capabilities. Result will go in a v4l2_capability struct
 * VIDIOC_S_FMT		-> Set the data format which is described in a v4l2_format struct
 * VIDIOC_QUERYBUF	-> Query the status of a v4l2_buffer struct
 * VIDIOC_QBUF		-> Enqueue an empty buffer to the driver's incoming queue. Takes a struct v4l2_buffer as parameter which got important info
 * VIDIOC_STREAMON	-> Start streaming the I/O depending on the struct v4l2_buffer type member
 * VIDIOC_DQBUF		-> Dequeue a filled buffer from the driver's outgoing queue. Result is stored in the struct v4l2_buffer
 * VIDIOC_STREAMOFF	-> Stop the streaming I/O which got the same struct v4l2_buffer type as parameter as STREAMON.
 */
class Camera
{		/*
		struct buffer
		{
			void *start;
			size_t length;
		};*/

	private:
		char ready = 0;
		std::string filename;

		int file_des;

		unsigned char capture_f = 0;							/* If 1 the device can cupture image/videos. Otherwise not */
		unsigned char readwrite_f = 0;							/* If 1 the device can understand read() and write(). Otherwise not */
		unsigned char ioctl_f = 0;								/* If 1 the device can understand ioctl(). Otherwise not */



		void initialise();
		int xioctl(int fd, int request, void *arg);				/* ioctl() wrapper to keep requesting untill an error occured */



	public:
		Camera(std::string fname);

		void *capture(int width, int height);
};
