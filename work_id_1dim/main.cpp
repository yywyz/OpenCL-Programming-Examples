#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<CL/cl.h>
#include<string.h>
#include <iostream>  

#define MAX_SOURCE_SIZE (0x100000)
//in order to print for the gloab id of the kernels and local id of the kernels

#define width 8
#define height 8

int main(void)
{
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_platforms;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem data_in = NULL;
	cl_mem data_out = NULL;
	cl_mem mem_global_id = NULL;
	cl_mem mem_local_id = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	size_t kernel_code_size;
	char *kernel_src_str;
	int *result;
	cl_int ret;
	FILE *fp;


	int point_width = (int)width;
	int point_height = (int)height;
	int point_num = point_width * point_height;
	int data_vecs[width*height];
	int i, j;

	for (i = 0; i < point_num; i++)
	{
		data_vecs[i] = i;
	}
	printf("\n");
	printf("data_in array:\n");
	for (i = 0; i < height; i++) {
		printf("row[%d]:\t", i);
		for (j = 0; j < width; j++) {
			printf("%d,\t", data_vecs[i*width + j]);
		}
		printf("\n");
	}
	printf("\n");

	kernel_src_str = (char *)malloc(MAX_SOURCE_SIZE);
	result = (int *) malloc(point_num * sizeof(int));


	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id,
		&ret_num_devices);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	fp = fopen("add_vec.cl", "r");
	kernel_code_size = fread(kernel_src_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	program = clCreateProgramWithSource(context, 1, (const char **)&kernel_src_str,
		(const size_t *)&kernel_code_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "add_vec", &ret);
	data_in = clCreateBuffer(context, CL_MEM_READ_WRITE,
		point_num * sizeof(int), NULL, &ret);
	data_out = clCreateBuffer(context, CL_MEM_READ_WRITE,
		point_num * sizeof(int), NULL, &ret);
	mem_global_id = clCreateBuffer(context, CL_MEM_READ_WRITE,
		point_num * sizeof(int), NULL, &ret);
	mem_local_id  = clCreateBuffer(context, CL_MEM_READ_WRITE,
		point_num * sizeof(int), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue, data_in, CL_TRUE, 0,
		point_num  * sizeof(int),
		data_vecs, 0, NULL, NULL);

	/* Set Kernel Arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&data_in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&mem_global_id);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&mem_local_id);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&data_out);
	ret = clSetKernelArg(kernel, 4, sizeof(int), (void *)&point_num);

	cl_uint work_dim = 1;
	size_t global_item_size;
	size_t local_item_size;

	global_item_size = width*height; /* Global number of work items */
	local_item_size = 4; /* Number of work items per work group */
							/* --> global_item_size / local_item_size which indirectly sets the
							number of workgroups*/

	/* Execute Data Parallel Kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL,
		&global_item_size, &local_item_size,
		0, NULL, NULL);

	/* Copy result from device to host */
	ret = clEnqueueReadBuffer(command_queue, mem_global_id, CL_TRUE, 0,
		point_num * sizeof(int),
		result, 0, NULL, NULL);

	/* print global id */
	printf("Global id array:\n");
	for (i = 0; i < height; i++) {
		printf("row[%d]:\t", i);
		for (j = 0; j < width; j++) {
			printf("%d,\t", result[i*width + j]);
		}
		printf("\n");
	}

	printf("\n");
	/* print local id */
	ret = clEnqueueReadBuffer(command_queue, mem_local_id, CL_TRUE, 0,
		point_num * sizeof(int),
		result, 0, NULL, NULL);

	printf("Local id array:\n");
	for (i = 0; i < height; i++) {
		printf("row[%d]:\t", i);
		for (j = 0; j < width; j++) {
			printf("%d,\t", result[i*width + j]);
		}
		printf("\n");
	}

	printf("\n");
	/* print local id */
	ret = clEnqueueReadBuffer(command_queue, data_out, CL_TRUE, 0,
		point_num * sizeof(int),
		result, 0, NULL, NULL);

	printf("data_out array:\n");
	for (i = 0; i < height; i++) {
		printf("row[%d]:\t", i);
		for (j = 0; j < width; j++) {
			printf("%d,\t", result[i*width + j]);
		}
		printf("\n");
	}
	/* OpenCL Object Finalization */
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(data_in);
	ret = clReleaseMemObject(data_out);
	ret = clReleaseMemObject(mem_global_id);
	ret = clReleaseMemObject(mem_local_id);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	/* Deallocate memory on the host */
	free(result);
	free(kernel_src_str);

	system("pause");
	return 0;
}
