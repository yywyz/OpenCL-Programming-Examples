/**********************************************
function: 2D Convolution of CNN
date    : 2018/07/21
**********************************************/

__kernel void Conv2D( __global int * image_in,  //image input
					  __global int * filter_in, //filter input
					           int K,		    //filter kernel size
					  __global int * image_out) //feature map output
{
	int W;       //work group global size
	int Wn;      //padded image width
	int x;		 //global id x 
	int y;		 //global id y
	int ki, kj;	 //filter coordinate,(kj, ki)

	int sum = 0; //multiply and sum of filter and data

	W = get_global_size(0);
	x = get_global_id(0);
	y = get_global_id(1);
	Wn = W + (K - 1);
	
	for(ki=0; ki<K; ki++)
		for(kj=0; kj<K; kj++)
			{
				sum  = sum + filter_in[ki*K + kj] * image_in[Wn*(y+ki) + x + kj];
			}

	image_out[y*W + x] = sum;

}
