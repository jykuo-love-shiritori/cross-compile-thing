from enum import Enum

from utils import *

class ChiaDuhState(Enum):
    BO_DAI_JI = 0
    CHIA_DUH = 1

class BuaDuhState(Enum):
    BO_DAI_JI = 0
    BUA_DUH = 1
    TOU_KA = 2 # unused


if __name__ == "__main__":
    try:
        MPU_Init()
        print (" Reading Data of Gyroscope and Accelerometer")
        post("今仔日")
        sleep(0.1)
        post("哇想講")
        sleep(0.1)
        post("欲來給恁講")
        sleep(0.5)
        post("台灣話")

        chiaDuhState = ChiaDuhState.BO_DAI_JI
        prevChiaDuhState = chiaDuhState

        buaDuhState = BuaDuhState.BO_DAI_JI
        prevBuaDuhState = buaDuhState

        while True:

            # Read Accelerometer raw value
            acc_x = read_raw_data(ACCEL_XOUT_H)
            acc_y = read_raw_data(ACCEL_YOUT_H)
            acc_z = read_raw_data(ACCEL_ZOUT_H)

            # Read Gyroscope raw value
            gyro_x = read_raw_data(GYRO_XOUT_H)
            gyro_y = read_raw_data(GYRO_YOUT_H)
            gyro_z = read_raw_data(GYRO_ZOUT_H)

            # Full scale range +/- 250 degree/C as per sensitivity scale factor
            Gx = gyro_x / 131.0 - gx_off
            Gy = gyro_y / 131.0 - gy_off
            Gz = gyro_z / 131.0 - gz_off

            Ax = acc_x/16384.0 - ax_off
            Ay = acc_y/16384.0 - ay_off
            Az = acc_z/16384.0 - az_off

            # Ax = acc_x / 4096.0 - ax_off
            # Ay = acc_y / 4096.0 - ay_off
            # Az = acc_z / 4096.0 - az_off

            gravity = (Ax ** 2 + Ay ** 2 + Az ** 2) ** 0.5

            print(f"{gravity:.2f} {Ax:.2f} {Ay:.2f} {Az:.2f}", end="\r")
            # print(f"{Ax:.2f} {Ay:.2f} {Az:.2f} {gravity:.2f}", end="\n")
            # print(f"{chiaDuhState.name}", end="\r")

            if Ax > 0.9:
                chiaDuhState = ChiaDuhState.BO_DAI_JI
            elif 0.9 < gravity < 1.1 and Ay ** 2 + Az ** 2 > 0.9:
                chiaDuhState = ChiaDuhState.CHIA_DUH

            if 1.1 > gravity > 0.9:
                buaDuhState = BuaDuhState.BO_DAI_JI
            elif gravity < 0.6:
                buaDuhState = BuaDuhState.BUA_DUH
            

            if chiaDuhState != prevChiaDuhState:
                if chiaDuhState == ChiaDuhState.BO_DAI_JI:
                    post("甘溫！")

                if chiaDuhState == ChiaDuhState.CHIA_DUH:
                    post("哇哄恰倒阿")
                
                prevChiaDuhState = chiaDuhState

            if buaDuhState != prevBuaDuhState:
                if buaDuhState == BuaDuhState.BO_DAI_JI:
                    post("我猶未係")

                if buaDuhState == BuaDuhState.BUA_DUH:
                    post("哇摔落去了")

                if buaDuhState == BuaDuhState.TOU_KA:
                    ...

                prevBuaDuhState = buaDuhState


            # system("clear")
            # print(f"Gx={Gx:+03.2f} deg/s \nGy={Gy:+03.2f} deg/s \nGz={Gz:+03.2f} deg/s")
            # print(f"Ax={Ax:+03.2f} g \nAy={Ay:+03.2f} g \nAz={Az:+03.2f} g", end="\n")
            # sleep(0.05)
        #     if gravity > 5:
        #         print(f"gravity: {gravity:.2f}")
    except KeyboardInterrupt:

        exit()
