clear;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp=fopen('bp_data.txt','r');
RecForm = '%f';
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
##fp=fopen('ecg_data.txt','r');
##RecForm = '%d';
##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
##fp=fopen('pat_ecg_data.txt','r');
##RecForm = '%f';
##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
##fp=fopen('pat_ppg_data.txt','r');
##RecForm = '%f';
##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

RecIncr = 1;
signal_input=fscanf(fp, RecForm);
fclose(fp);
x_val = (1 : RecIncr : size(signal_input, 1)*RecIncr)';

plot(x_val, signal_input, "Linewidth", 1);
grid on;
