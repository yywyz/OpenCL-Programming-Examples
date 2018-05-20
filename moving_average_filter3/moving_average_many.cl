__kernel void moving_average_many(__global int4 *values,
	__global float4 *average,
	int length,
	int name_num,
	int width)
	{
	int i, j;
	int loop_num = name_num / 4; /* compute the number of times to loop */
	int4 add_value;
	
	for (j=0; j < loop_num; j++) {
		add_value = (int4)0;
		for (i=0; i < width; i++) {
			add_value += values[i*loop_num+j];
		}
		average[(width-1)*loop_num+j] = convert_float4(add_value);

		for (i=width; i < length; i++) {
			add_value = add_value - values[(i-width)*loop_num+j] + values[i*loop_num+j];
			average[i*loop_num+j] = convert_float4(add_value);
		}

		for (i=0; i < width-1; i++) {
			average[i*loop_num+j] = (float4)(0.0f);
		}

		for (i=width-1; i < length; i++) {
			average[i*loop_num+j] /= (float4)width;
		}
	}
}
