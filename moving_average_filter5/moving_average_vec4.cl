__kernel void moving_average_vec4(__global int4 *values,
	__global float4 *average,
	int length,
	int width)
{
	int i;
	int4 add_value; /* A vector to hold 4 components */

	/* Compute sum for the first "width" elements for 4 stocks */
	add_value = (int4)0;
	for (i=0; i < width; i++) {
		add_value += values[i];
	}
	average[width-1] = convert_float4(add_value);
	/* Compute sum for the (width)th ～ (length-1)th elements for 4 stocks */
	for (i=width; i < length; i++) {
		add_value = add_value - values[i-width] + values[i];
		average[i] = convert_float4(add_value);
	}

	/* Insert zeros to 0th ～ (width-2)th element for 4 stocks*/
	for (i=0; i < width-1; i++) {
		average[i] = (float4)(0.0f);
	}

	/* Compute average of (width-1) ~ (length-1) elements for 4 stocks */
	for (i=width-1; i < length; i++) {
	average[i] /= (float4)width;
	}
}
