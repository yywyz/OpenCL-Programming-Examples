__kernel void pooling(__global int * data_in, 
					  __global int *mem_global_id, 
					  __global int *mem_local_id, 
					  __global int *data_out,
					  int width)
{
	int i,j;
	int global_id_x, global_id_y;
	int local_id_x, local_id_y;

	global_id_x = get_global_id(0);
	global_id_y = get_global_id(1);
	local_id_x  = get_local_id(0);
	local_id_y  = get_local_id(1);

	//(x,y)
	mem_global_id[global_id_y * width + global_id_x] = global_id_x * 10 + global_id_y;
	mem_local_id [global_id_y * width + global_id_x] = local_id_x * 10  + local_id_y;

	if((global_id_x % 2 == 0 )&& (global_id_y % 2 == 0))
	{
		int index1 = global_id_y * width + global_id_x;
		int index2 = global_id_y * width + global_id_x + width;
		int tmp1 = max(data_in[index1],data_in[index1 + 1]);
		int tmp2 = max(data_in[index2],data_in[index2 + 1]);
		int tmp  = max(tmp1,tmp2);
		data_out[global_id_y / 2 * width / 2 + global_id_x/2] = tmp ;
	}
}
