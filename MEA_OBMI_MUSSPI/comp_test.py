import serial
import matplotlib.pyplot as plt

ser = serial.Serial('COM7', 115200, timeout=5)


def plot_adc(x_in, y1_in, y2_in):
    fig = plt.figure(figsize=(10, 7))
    fig.subplots_adjust(bottom=0.1, left=0.1)
    plot_list = []
    plot_list.append(plt.subplot(211))
    plot_list.append(plt.subplot(212, sharex=plot_list[0]))

    plot_list[0].clear()
    plot_list[0].plot(x_in, y1_in, 'b')
    plot_list[0].set_title('Input signal')
    plot_list[0].grid()

    plot_list[1].clear()
    plot_list[1].plot(x_in, y2_in, 'g')
    plot_list[1].set_title('Computed signal')
    plot_list[1].grid()
    plot_list[1].set_xlabel('time [ms]')

    plt.show()


if __name__ == "__main__":
    data_in = []
    data_serial = []
    data_calk = []
    data_time = []
    timing = 0
    mid_dat = [0, 0, 0, 0, 0]
    data_out = [0x53, 0x00, 0x00]
    data_zero = [0x4F, 0x4F, 0x4F]
    file = open('front_end_med_2.txt', 'r')
    lines = file.readlines()
    print('Reading data form file...')
    for x in lines:
        data_in.append(int(x.split('\t')[1]))
        data_time.append(timing)
        timing += 4
    file.close()
    print('... data ready\nStart computation...')
    for i in range(data_in.__len__()):
        data_out[1] = (data_in[i] >> 8) & 0xFF
        data_out[2] = data_in[i] & 0xFF
        ser.write(data_out)
        data_serial.append(float(ser.readline().decode()))
    # for i in range(data_in.__len__()):
    #     mid_dat.pop(0)
    #     mid_dat.append(data_in[i])
    #     data_serial.append(sum(mid_dat)/5.0)
    print('... computation ready')
    ser.write(data_zero)
    plot_adc(data_time, data_in, data_serial)
    data_serial.clear()
    data_time.clear()

