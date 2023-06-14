from enum import Enum

from utils import *

class State(Enum):
    NORMAL = 0,
    FALLING = 1,
    ON_GROUND = 2,
    CHIA_DUH = 3


if __name__ == "__main__":
    MPU_Init()
    print (" Reading Data of Gyroscope and Accelerometer")

    state = State.NORMAL

    # Read Accelerometer raw value
    acc_x = read_raw_data(ACCEL_XOUT_H)
    acc_y = read_raw_data(ACCEL_YOUT_H)
    acc_z = read_raw_data(ACCEL_ZOUT_H)

    # Read Gyroscope raw value
    gyro_x = read_raw_data(GYRO_XOUT_H)
    gyro_y = read_raw_data(GYRO_YOUT_H)
    gyro_z = read_raw_data(GYRO_ZOUT_H)

    # Full scale range +/- 250 degree/C as per sensitivity scale factor
    Gx = gyro_x/131.0
    Gy = gyro_y/131.0
    Gz = gyro_z/131.0

    # Ax = acc_x/16384.0
    # Ay = acc_y/16384.0
    # Az = acc_z/16384.0

    Ax = acc_x/2048.0
    Ay = acc_y/2048.0
    Az = acc_z/2048.0

    # gyro
    gx_off = Gx
    gy_off = Gy
    gz_off = Gz

    # accel
    ax_off = 0
    ay_off = 0
    az_off = 0

    while True:
        prev_state = state

        # Read Accelerometer raw value
        acc_x = read_raw_data(ACCEL_XOUT_H)
        acc_y = read_raw_data(ACCEL_YOUT_H)
        acc_z = read_raw_data(ACCEL_ZOUT_H)

        # Read Gyroscope raw value
        gyro_x = read_raw_data(GYRO_XOUT_H)
        gyro_y = read_raw_data(GYRO_YOUT_H)
        gyro_z = read_raw_data(GYRO_ZOUT_H)

        # Full scale range +/- 250 degree/C as per sensitivity scale factor
        Gx = gyro_x/131.0 - gx_off
        Gy = gyro_y/131.0 - gy_off
        Gz = gyro_z/131.0 - gz_off

        # Ax = acc_x/16384.0
        # Ay = acc_y/16384.0
        # Az = acc_z/16384.0

        Ax = acc_x/4096.0 - ax_off
        Ay = acc_y/4096.0 - ay_off
        Az = acc_z/4096.0 - az_off

        gravity = (Ax ** 2 + Ay ** 2 + Az ** 2) ** 0.5

        # print(f"{Ax:.2f} {Ay:.2f} {Az:.2f} {gravity:.2f}", end="\n")
        # print(f"{state.name}", end="\r")

        # if gravity < 0.2:
        #     requests.post(WEBHOOK_URL, json={
        #         "content": "i'm falling ><" + f"g={Ax}cg",
        #         "username": "TX2",
        #         "avatar_url": JENSEN_URL
        #     })

        if Ax > 0.9:
            state = State.NORMAL
        elif 0.9 < Ay ** 2 + Az ** 2 < 1.1:
            state = State.CHIA_DUH

        if state != prev_state:
            content = ""
            if state == State.CHIA_DUH:
                content = "哇哄恰倒阿"
            
            if state == State.NORMAL:
                content = "甘溫！"

            requests.post(WEBHOOK_URL, json={
                "content": content,
                "username": "TX2",
                "avatar_url": JENSEN_URL
            })

            exit()

            


        # system("clear")
        print(f"Gx={Gx:+03.2f} deg/s \nGy={Gy:+03.2f} deg/s \nGz={Gz:+03.2f} deg/s")
        print(f"Ax={Ax:+03.2f} g \nAy={Ay:+03.2f} g \nAz={Az:+03.2f} g", end="\n")
        sleep(0.1)
    #     if gravity > 5:
    #         print(f"gravity: {gravity:.2f}")
