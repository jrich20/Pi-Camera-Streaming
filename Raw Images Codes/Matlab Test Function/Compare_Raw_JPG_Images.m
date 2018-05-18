%% Convert a Bayer Pattern Encoded Image To an RGB Image
% Convert a Bayer pattern encoded image that was photographed by a camera
% with a sensor alignment of |'bggr'| .

% Copyright 2015 The MathWorks, Inc.

I = imread('will_convert.ppm');
J = rgb2gray(I);
figure, imshow(J)
A = imread('will_convert.jpg');
B = rgb2gray(A);
figure, imshow(B)