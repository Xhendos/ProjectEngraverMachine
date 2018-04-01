#include "Camera.hpp"

#include <fcntl.h>													/* defines O_ constants, open() */

#include <string.h>													/* strerror() */
#include <errno.h>													/* global variable errno */

#include <sys/ioctl.h>												/* ioctl() */
#include <stdio.h>

#include <vector>													/* std::vector */
		
#include <sys/mman.h>												/* mmap() */

Camera::Camera(std::string fname)
{
	filename = fname;
	initialise();
}

int Camera::xioctl(int fd, int request, void *arg)
{
	int r;
	do r = ioctl(fd, request, arg);
	while (r == -1 && errno == EINTR);

	return r;
}

void Camera::initialise()
{
	struct v4l2_capability	cap = {0};								/* Describes capabilities of the camera */

	/* This is step 1 in the activity diagram */
	file_des = open(filename.c_str(), O_RDWR);						/* File descriptor for the camera device */
	if(file_des < 0)
	{
		fprintf(stderr, strerror(errno));
		exit(4);
	}

	/* This is step 2 in the activity diagram */
	if(xioctl(file_des, VIDIOC_QUERYCAP, &cap) == -1)				/* Try to receive device capabilities */
	{
		fprintf(stderr, strerror(errno));							/* In case we cannot receive capabilities, print and exit */
		exit(5);
	}
																	/* Check for the important capabilities in this application */
	if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)					/* Can we capture image/videos with the hardware device? */
		capture_f = 1;

	if(cap.capabilities & V4L2_CAP_READWRITE)						/* Can we use the read() and write() from unistd.h on this device? */
		readwrite_f = 1;

	if(cap.capabilities & V4L2_CAP_STREAMING)						/* Can we use the ioctl() from sys/ioctl.h on this device? */
		ioctl_f = 1;


	printf("[DRIVER] [INFO] Driver is %s\n", cap.driver);
	printf("[DRIVER] [INFO] Card is %s\n", cap.card);
	printf("[DRIVER] [INFO] Version is %d\n", cap.version);
	printf("[DRIVER] [INFO] bus info %s\n", cap.bus_info);

}

void Camera::capture(int width, int height)
{
	struct v4l2_format			fmt = {0};							/* Describes the stream data format */
	struct v4l2_requestbuffers 	rbf = {0};							/* */

	struct buffer_info
	{
		struct v4l2_buffer buffer;
		void *start;
	};


	/* This is step 3 in the activity diagram */
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;							/* Tell the camera we only want to make a picture */
																	/* Give the camera some parameters so it knows our wishes */
	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;

	if(xioctl(file_des, VIDIOC_S_FMT, &fmt) == -1)					/* If failed to send or accept our wishes, exit the program */
	{
		fprintf(stderr, strerror(errno));
		exit(6);
	}

	/* This is step 4 in the activity diagram */
	if(fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24)				/* If VIDIOC_S_FMT changed the format because it is not supported tell the user. */
		perror("[DRIVER] Error pixelformat.\n");


	rbf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rbf.memory = V4L2_MEMORY_MMAP;									/* Retrieve the information by a shared memory object. */
	rbf.count = 1;													/* 1 buffer available, but can be changed after VIDIOC_REQBUFS */

	if(xioctl(file_des, VIDIOC_REQBUFS, &rbf) == -1)				/* Try to initialise memory mapped memory */
	{
		if(errno == EINVAL)
		{
			fprintf(stdout, "[DRIVER] Error user pointer.\n");
			exit(8);
		} else
		{
			fprintf(stderr, "[DRIVER] Cannot request buffer.\n");	
			exit(9);
		}
	}
																	/* Keep in mind that at this point the rbf.count could have been changed. */

	/* This is step 5 in the activity diagram */
	std::vector<struct buffer_info> buffers;						/* Data structure to hold all buffers and its shared memory object */

	for(int index = 0; index < rbf.count; index++)
	{
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));								/* Set all the bits to 0 so VIDIOC_QUERYBUF does not give false positives. */

		buf.index = index;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		if(xioctl(file_des, VIDIOC_QUERYBUF, &buf) < 0)				/* Get the status of buf and exit if its not okay. */
		{
			perror("[DRIVER] Error status of buffer.\n");
			exit(13);
		}

																	/* Map between the user address and the kernel address space. */
		void *start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
							MAP_SHARED, file_des, buf.m.offset);

		if(start == MAP_FAILED)
		{
			perror("[DRIVER] Error to map a shared memory object.\n");
			exit(10);
		}

		if(xioctl(file_des, VIDIOC_QBUF, &buf) < 0)					/* */
		{
			perror("[DRIVER] Failed to enqueue buffer.\n");
			exit(11);
		}


		buffers.push_back(buffer_info{buf, start});
	}

	/* This is step 6 in the activity diagram */
	if(xioctl(file_des, VIDIOC_STREAMON, &buffers[0].buffer.type) < 0)
	{
		perror("[DRIVER] Error in VIDIOC_STREAMON.\n");
	}


	for(int index = 0; index < rbf.count; index++)
	{
		if(xioctl(file_des, VIDIOC_DQBUF, &buffers[index].buffer) != 0)
		{
			perror("[DRIVER] Error dequeue a filled buffer.\n");
			exit(12);
		}

		printf("%d\n", buffers[index].buffer.length);
		printf("%d\n", buffers[index].buffer.bytesused);
	}



	if(xioctl(file_des, VIDIOC_STREAMOFF, &buffers[0].buffer.type) < 0)
	{
		perror("[DRIVER] Error in VIDIOC_STREAMOFF");
	}
}
