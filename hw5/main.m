%% 8903 Audio software engineering 
% HW5 matlab test file

clear all; clc; close all;

%==== Read CPP results for comparison
cppResults = textread('sweepFeatures.txt');

%==== Read audio file
[y, fs] = wavread('sweep.wav');
yLeft = y(:, 1);
yRight = y(:, 2);

%==== Define parameters
windowSize = 1024;
hopSize = 1024;
noverlap = windowSize - hopSize;

yTimeL = x2mat(yLeft, windowSize, hopSize);
yTimeR = x2mat(yRight, windowSize, hopSize);

% 
% SL = spectrogram(yLeft, windowSize, noverlap, windowSize, fs);
% SR = spectrogram(yRight, windowSize, noverlap, windowSize, fs);


[m, n] = size(yTimeL);

matlabResults = [];

tmpResult = zeros(4, 2);
for i = 1:n
    currentFrameL = yTimeL(:, i);
    currentFrameR = yTimeR(:, i);
    
    %order: SC, SF, ZC, SR
    currentSpecL = fft(currentFrameL);
    currentSpecR = fft(currentFrameR);
%     currentSpecL = SL(:, i);
%     currentSpecR = SR(:, i);
    
    %extract features
    [vscL] = FeatureSpectralCentroid(abs(currentSpecL), fs);
    [vscR] = FeatureSpectralCentroid(abs(currentSpecR), fs);
    
    [vsfL] = FeatureSpectralFlux(abs(currentSpecL), fs);
    [vsfR] = FeatureSpectralFlux(abs(currentSpecR), fs);
    
    [vzcL] = FeatureTimeZeroCrossingRate(currentFrameL, windowSize, hopSize, fs);
    [vzcR] = FeatureTimeZeroCrossingRate(currentFrameR, windowSize, hopSize, fs);
    
    [vsrL] = FeatureSpectralRolloff(abs(currentSpecL), fs, 0.85);
    [vsrR] = FeatureSpectralRolloff(abs(currentSpecR), fs, 0.85);
    
    tmpResult(1, 1) = vscL;
    tmpResult(1, 2) = vscR;
    tmpResult(2, 1) = vsfL;
    tmpResult(2, 2) = vsfR;
    tmpResult(3, 1) = vzcL;
    tmpResult(3, 2) = vzcR;
    tmpResult(4, 1) = vsrL;
    tmpResult(4, 2) = vsrR;
    
    matlabResults = [matlabResults; tmpResult];
   
    
end
