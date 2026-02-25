[signal, fs] = audioread('idioteque_clip.wav');
myWAV = mean(signal,2);
sweep = sweeptone();
t = 0:1/fs:1-1/fs; % Time vector for 1 second
sine = sin(2 * pi * 1000 * t);
signal = {myWAV, sweep, sine'};
word = {' My .WAV', ' ESS', ' 1k Sine Tone'};
fc = 1000;
Q = .5;
gain = 12;
filter_type = "Constant";
obj = BiquadFilter(fc, fs, Q, gain);
obj = obj.filterSelect(filter_type);
for i = 1:length(signal)
    audio = obj.process(signal{i});
    % Spectrogram as suggested analysis
    window_size = 256;
    overlap = 128;
    nfft = 512; % number of fft points
    [S, F, T, P] = spectrogram(audio, window_size, overlap, nfft, fs);
    subplot(3,1,i);
    imagesc(T, F, 10*log10(P));
    axis xy;
    xlabel('Time (s)');
    ylabel('Frequency (Hz)');
    title(strcat('Spectrogram for',word{i}));
    colorbar;
end

audio = obj.process(signal{1});
sound(audio,fs);

