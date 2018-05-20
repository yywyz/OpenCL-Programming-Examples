#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<CL/cl.h>
#include<string.h>
#include <iostream>  

int stock_array1[] = {
#include "stock_array1.txt"
};

#define WINDOW_SIZE (13)

#define MAX_SOURC_SIZE (0x100000)

int main(void)
{
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_platforms;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj_in = NULL;
	cl_mem memobj_out = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	size_t kernel_code_size;
	char *kernel_src_str;
	float *result;
	cl_int ret;
	FILE *fp;

	int data_num = sizeof(stock_array1) / sizeof(stock_array1[0]);

	int window_num = (int)WINDOW_SIZE;
	int i;
	printf("data_num = %d\n", data_num);
	for (i = 0; i < data_num; i++) {
		printf("stock_array1[%d]=%d\t", i, stock_array1[i]);
	}
	printf("\n");
	kernel_src_str = (char *)malloc(MAX_SOURC_SIZE);
	result = (float *)malloc(data_num * sizeof(float));
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	fp = fopen("moving_average.cl", "r");
	kernel_code_size = fread(kernel_src_str, 1, MAX_SOURC_SIZE, fp);
	fclose(fp);

	program = clCreateProgramWithSource(context, 1, (const char **)&kernel_src_str,
		(const size_t *)&kernel_code_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "moving_average", &ret);
	memobj_in = clCreateBuffer(context, CL_MEM_READ_WRITE, data_num * sizeof(int), NULL, &ret);
	memobj_out = clCreateBuffer(context, CL_MEM_READ_WRITE, data_num * sizeof(float), NULL, &ret);
	ret = clEnqueueWriteBuffer(command_queue, memobj_in, CL_TRUE, 0, data_num * sizeof(int),
		stock_array1, 0, NULL, NULL);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj_in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memobj_out);
	ret = clSetKernelArg(kernel, 2, sizeof(int), (void *)&data_num);
	ret = clSetKernelArg(kernel, 3, sizeof(int), (void *)&window_num);
	ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, memobj_out, CL_TRUE, 0, data_num * sizeof(float),
		result, 0, NULL, NULL);

	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj_in);
	ret = clReleaseMemObject(memobj_out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	for (i = 0; i<data_num; i++) {
		printf("result[%d] = %f\t", i, result[i]);
	}
	printf("\n");

	free(result);
	free(kernel_src_str);
	system("pause");
	return 0;
}
