# OpenCL-Programming-Examples
These are OpenCL Programming Examples for learning.

The codes are running with Visual Stdio 2017,  
you need to setup your tool to run OpenCL porgrams firstly. 

Codes are copied from "The OpenCL Programming Book" which are marked with "TOPB".  
If this makes you unhappy, please contact me to delete.  

CNN Convolution (20180721)
###
  >function: for convoluational operation in CNN of Deep Learning    
    main.cpp                 : host program  
    Conv2D.cl                : 2 dimension convolution
 
work_id_2dim_pooling(20180524)
###
  >function: output global id and local id of work-items for 2 dimensions data    
    main.cpp                 : host program  
    pooling.cl               : opencl kernel, deep learning max pooling , pooling size 2*2

work_id_1dim(20180524) 
###
  >function: output global id and local id of work-items for 1 dimesion data    
    main.cpp                 : host program  
    add_vec.cl               : opencl kernel  

moving_average_filter5 ("TOPB") 
###
  >function: process 4 channels data of stock price based on kernel based on task parallel model    
    main.cpp                 : host program  
    moving_average_vec4.cl   : opencl kernel  
    stock_array_4.txt        : test file  

moving_average_filter4 ("TOPB") 
###
  >function: process 8 channels data of stock price based on kernel based on data parallel model    
    main.cpp                 : host program  
    moving_average_vec4_para.cl : opencl kernel  
    stock_array_vec4_par.txt : test file 

moving_average_filter3 ("TOPB") 
###
  >function: process 8 channels data of stock price based on kernel "moving_average_vec4.cl"  
    main.cpp                 : host program  
    moving_average_many.cl   : opencl kernel  
    stock_array_many.txt     : test file  
    
moving_average_filter2 ("TOPB") 
###
  > function: process 4 channels data of stock price at one time   
    main.cpp                 : host program  
    moving_average_vec4.cl   : opencl kernel  
    stock_array_4.txt        : test file  

moving_average_filter1 ("TOPB")
###
>function: process one channel data of stock price   
    main.cpp                 : host program  
    moving_average.cl        : opencl kernel  
    stock_array1.txt         : test file 


