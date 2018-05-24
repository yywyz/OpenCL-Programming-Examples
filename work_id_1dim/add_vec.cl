__kernel void add_vec(__global int * data_in, 
					  __global int *mem_global_id, 
					  __global int *mem_local_id, 
					  __global int *data_out, 
					  int length)
{
	int i,j;
	int global_id;
	int local_id;

	global_id = get_global_id(0);
	local_id  = get_local_id(0);

	mem_global_id[global_id] = global_id;
	mem_local_id [global_id] = local_id;

	for(i=0; i<length; i++)
	{
		data_out[i] = data_in[i]*2;
	}
}
