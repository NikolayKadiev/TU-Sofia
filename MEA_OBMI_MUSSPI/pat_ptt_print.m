clear all

signal_in = csvread('pat_ptt_2.csv');

ecg = signal_in(:,2);
ppg_wrist = signal_in(:,3);
ppg_finger = signal_in(:,4);

ecg = ecg*2+11500;
ppg_wrist = ppg_wrist+7500;
ppg_finger = ppg_finger+2000;

x = 1:4:(length(ecg)*4);

plot(x, ecg, "r", "Linewidth", 3, x, ppg_wrist, "g", x, ppg_finger, "b");
h = legend ({"ECG", "Wrist PPG", "Finger PPG"}, "location", "southwest");
ylabel('Signals');
xlabel('time, ms');
grid on;
