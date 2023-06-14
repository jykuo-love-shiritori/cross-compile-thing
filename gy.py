from collections import namedtuple 

from utils import *

Acc = namedtuple('Acc','x, y, z')

class GY521:
    def get_acc(self):
        acc_x = read_raw_data(ACCEL_XOUT_H)
        acc_y = read_raw_data(ACCEL_YOUT_H)
        acc_z = read_raw_data(ACCEL_ZOUT_H)
        Ax = acc_x/2048.0
        Ay = acc_y/2048.0
        Az = acc_z/2048.0
        return  Acc(Ax,Ay,Az)
    ...
