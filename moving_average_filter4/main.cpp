#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<CL/cl.h>
#include<string.h>
#include <iostream>  

#define NAME_NUM (8) /* Number of stocks */
#define DATA_NUM (21) /* Number of data to process for each stock */

/* Read Stock data */
int stock_array_vec4_par[NAME_NUM*DATA_NUM] = {
#include "stock_array_vec4_par.txt"
};

/* Moving average width */
#define WINDOW_SIZE (13)

#define MAX_SOURCE_SIZE (0x100000)

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

	int window_num = (int)WINDOW_SIZE;
	int point_num = NAME_NUM * DATA_NUM;
	int data_num = (int)DATA_NUM;
	int name_num = (int)NAME_NUM;

	int i, j;

	for (i = 0; i < point_num; i++) {
		printf("stock_array_vec4_par[%d] = %d\n", i, stock_array_vec4_par[i]);
	}
	/* Allocate space to read in kernel code */
	kernel_src_str = (char *)malloc(MAX_SOURCE_SIZE);

	/* Allocate space for the result on the host side */
	result = (float *)malloc(point_num * sizeof(float));

	/* Get Platform*/
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	/* Get Device */
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id,
		&ret_num_devices);

	/* Create Context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Read kernel source code */
	fp = fopen("moving_average_vec4_para.cl", "r");
	kernel_code_size = fread(kernel_src_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Create Program Object */
	program = clCreateProgramWithSource(context, 1, (const char **)&kernel_src_str,
		(const size_t *)&kernel_code_size, &ret);

	/* Compile kernel */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Create kernel */
	kernel = clCreateKernel(program, "moving_average_many", &ret);

	/* Create buffer for the input data on the device */
	memobj_in = clCreateBuffer(context, CL_MEM_READ_WRITE,
		point_num * sizeof(int), NULL, &ret);

	/* Create buffer for the result on the device */
	memobj_out = clCreateBuffer(context, CL_MEM_READ_WRITE,
		point_num * sizeof(float), NULL, &ret);

	/* Copy input data to the global memory on the device*/
	ret = clEnqueueWriteBuffer(command_queue, memobj_in, CL_TRUE, 0,
		point_num * sizeof(int),
		stock_array_vec4_par, 0, NULL, NULL);

	/* Set Kernel Arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj_in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memobj_out);
	ret = clSetKernelArg(kernel, 2, sizeof(int), (void *)&data_num);
	ret = clSetKernelArg(kernel, 3, sizeof(int), (void *)&name_num);
	ret = clSetKernelArg(kernel, 4, sizeof(int), (void *)&window_num);

	
	/* Set parameters for data parallel processing (work item) */
	cl_uint work_dim = 1;
	size_t global_item_size[3];
	size_t local_item_size[3];
	
	global_item_size[0] = 2; /* Global number of work items */
	local_item_size[0] = 1; /* Number of work items per work group */
	/* --> global_item_size[0] / local_item_size[0] becomes 2, which indirectly sets the
		 number of workgroups to 2*/
	
	/* Execute Data Parallel Kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL,
								global_item_size, local_item_size,
								0, NULL, NULL);

	/* Copy result from device to host */
	ret = clEnqueueReadBuffer(command_queue, memobj_out, CL_TRUE, 0,
		point_num * sizeof(float),
		result, 0, NULL, NULL);

	/* OpenCL Object Finalization */
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj_in);
	ret = clReleaseMemObject(memobj_out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	/* Print results */
	for (i = 0; i < data_num; i++) {
		printf("result[%d]:", i);
		for (j = 0; j < name_num; j++) {
			printf("%f, ", result[i*NAME_NUM + j]);
		}
		printf("\n");
	}

	/* Deallocate memory on the host */
	free(result);
	free(kernel_src_str);
	system("pause");
	return 0;
}
