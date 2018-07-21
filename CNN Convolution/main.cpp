#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<CL/cl.h>
#include<string.h>
#include <iostream>  

#define MAX_SOURCE_SIZE (0x10000)

int main(void)
{
	/*=================================================
	define parameters
	=================================================*/
	cl_platform_id	platform_id = NULL;
	cl_uint			ret_num_platforms;
	cl_device_id	device_id = NULL;
	cl_uint			ret_num_devices;
	cl_context		context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem			data_in = NULL;
	cl_mem			data_out = NULL;
	cl_mem			filter_in = NULL;
	cl_program		program = NULL;
	cl_kernel		kernel = NULL;
	size_t			kernel_code_size;
	char			*kernel_str;
	int				*result;
	cl_int			ret;
	FILE			*fp;
	cl_uint			work_dim;
	size_t			global_item_size[2];
	size_t			local_item_size[2];
	/*=================================================
	define parameters for image, filter, kernels
	=================================================*/
	int const W = 8;			//image width
	int const H = 8;			//image height
	int const K = 3;			//filter kernel size
	int const Wn = (W + K - 1); //padded image width
	int const Hn = (H + K - 1); //padded image height

	int point_num = Wn * Hn;
	int data_vecs[Wn*Hn];
	int filter_coe[K*K] = { -1,0,1,-2,0,2,-1,0,1 }; //sobel filter: horizontal gradient
	int i, j;

	for (i = 0; i < point_num; i++)
	{
		data_vecs[i] = rand() % 20;
	}

	//display input data
	printf("\n");
	printf("Array data_in:\n");
	for (i = 0; i < Hn; i++) {
		printf("row[%d]:\t", i);
		for (j = 0; j < Wn; j++) {
			printf("%d,\t", data_vecs[i*Wn + j]);
		}
		printf("\n");
	}
	printf("\n");


	/*=================================================
	load kernel, opencl environment setup
	create input and output buffer
	set kernel arguments, excute kernels
	get final results
	=================================================*/
	kernel_str = (char *)malloc(MAX_SOURCE_SIZE);
	result = (int *)malloc(W*H * sizeof(int));

	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id,
		&ret_num_devices);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	fp = fopen("Conv2D.cl", "r");
	kernel_code_size = fread(kernel_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	program = clCreateProgramWithSource(context, 1, (const char **)&kernel_str,
		(const size_t *)&kernel_code_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "Conv2D", &ret);
	data_in = clCreateBuffer(context, CL_MEM_READ_WRITE, Wn*Hn * sizeof(int), NULL, &ret);
	data_out = clCreateBuffer(context, CL_MEM_READ_WRITE, W*H * sizeof(int), NULL, &ret);
	filter_in = clCreateBuffer(context, CL_MEM_READ_WRITE, K*K * sizeof(int), NULL, &ret);

	//write image data into data_in buffer
	ret = clEnqueueWriteBuffer(command_queue, data_in, CL_TRUE, 0, Wn*Hn * sizeof(int), data_vecs, 0, NULL, NULL);

	//write filter data into filter_in buffer
	ret = clEnqueueWriteBuffer(command_queue, filter_in, CL_TRUE, 0, K*K * sizeof(int), filter_coe, 0, NULL, NULL);

	//set kernel arguments
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&data_in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&filter_in);
	ret = clSetKernelArg(kernel, 2, sizeof(int), (void *)&K);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&data_out);

	work_dim = 2;
	global_item_size[0] = { W };
	global_item_size[1] = { H };
	local_item_size[0] = { 1 };
	local_item_size[1] = { 1 };

	//execute data parallel kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL,
		global_item_size, local_item_size, 0, NULL, NULL);


	// read data_out to host
	ret = clEnqueueReadBuffer(command_queue, data_out, CL_TRUE, 0,
		W*H * sizeof(int), result, 0, NULL, NULL);

	//display output data
	FILE *f_img_out = fopen("image_out.txt", "w+");
	printf("Array data_out: \n");
	for (i = 0; i < H; i++) {
		printf("row[%d]:\t", i);
		for (j = 0; j < W; j++) {
			printf("%d,\t", result[i*W + j]);
			fprintf(f_img_out, "%d,\t", result[i*W + j]);
		}
		printf("\n");
		fprintf(f_img_out, "\n");
	}
	printf("\n");
	fclose(f_img_out);
	/*=================================================
	release all opencl objects
	=================================================*/
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(data_in);
	ret = clReleaseMemObject(data_out);
	ret = clReleaseMemObject(filter_in);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(result);
	free(kernel_str);

	system("pause");
	return 0;
}
