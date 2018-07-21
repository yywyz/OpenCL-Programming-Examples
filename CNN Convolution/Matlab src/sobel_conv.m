%% Function:  sobel filter for image processing
%% Author  :  A warrior
%% Date    :  2018/07/18
%% Reference: https://blog.csdn.net/qq_29540745/article/details/51918004
close all; clc; clear;

%% please down load lena.png first through below link
%% https://pan.baidu.com/s/19SPoDPU2sxi3fioHcjE4PA
lena_rgb = imread('lena.png');
lena_hsv = rgb2hsv(lena_rgb);
image    = lena_hsv(:,:,1);
[image_out_mat,thresh2]=edge(image,'sobel'); 

subplot(1,2,1);
imshow(lena_rgb);

subplot(1,2,2);
imshow(image_out_mat);

%% Sobel filter
Gx = [-1 0 1; -2 0 2; -1 0 1];
Gy = [1 2 1; 0 0 0; -1 -2 -1];

%% Kenerl parameters
H = size(image, 1); %image width
W = size(image, 2); %image height
K = size(Gx,1);     %filter size
stride = 1;         %filter stride

%% Image padding, full padding
Hn = H + K - 1;     
Wn = W + K - 1;
image_padded = [image,zeros(H,K-1)];
image_padded = [image_padded; zeros(K-1,W+K-1)];

%% Reshape memory to one row
image_padded = reshape(image_padded.', 1, []);
image_out_gx = zeros(1,W*H);
gx = reshape(Gx.', 1, []);

%% Horizontal gradient calculation
sum = 0;
for i = 1 : H
    for j = 1 : W
        for ki = 1 : K
            for kj = 1 : K
                sum = sum + image_padded(Wn*(i-1 + ki-1) + j + kj-1) * gx((ki-1)*K + kj);
            end
        end
        image_out_gx((i-1)*W + j) = sum;
        sum = 0;
    end
end

figure 
image_out1 = reshape(image_out_gx, W, []).';
imshow(image_out1)
title('Horizontal gradient')

%% Vertical gradient calculation
image_out_gy = zeros(1,W*H);
gy = reshape(Gy.', 1, []);
sum = 0;
for i = 1 : H
    for j = 1 : W
        for ki = 1 : K
            for kj = 1 : K
                sum = sum + image_padded(Wn*(i-1 + ki-1) + j + kj-1) * gy((ki-1)*K + kj);
            end
        end
        image_out_gy((i-1)*W + j) = sum;
        sum = 0;
    end
end
figure 
image_out2 = reshape(image_out_gy, W, []).';
imshow(image_out2)
title('Vertical gradient')

%% Gradient of image
figure
g = sqrt(image_out1.^2 + image_out2.^2);
imshow(g)
title('Gradient of lena')
