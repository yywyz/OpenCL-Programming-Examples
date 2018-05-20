__kernel void moving_average(__global int *values,
	__global float *average,
	int length,
	int width)
{
	int	 i;
	int add_value;

	/* Compute sum for the first "width" elements */
	add_value = 0;
	for (i=0; i < width; i++) {
		add_value += values[i];
	}
	average[width-1] = (float)add_value;

	/* Compute sum for the (width)th ～ (length-1)th elements */
	for (i=width; i < length; i++) {
		add_value = add_value - values[i-width] + values[i];
		average[i] = (float)(add_value);
	}

	/* Insert zeros to 0th ~ (width-2)th elements */
	for (i=0; i < width-1; i++) {
		average[i] = 0.0f;
	}

	/* Compute average of (width-1) ~ (length-1) elements */
	for (i=width-1; i < length; i++) {
		average[i] /= (float)width;
	}
}
