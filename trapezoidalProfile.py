import numpy as np
from matplotlib import pyplot as plt

class TrapezoidalProfile:
    def __init__(self, acc = 0.5, speedUp = True):
        self.speedUp = speedUp
        self.acc = acc

    def calVel(self, dest, vel, period, maxVel):
        # 定速域
        if vel == maxVel:
            # 減速し始めるかの判定
            if dest < np.power(vel, 2) / self.acc / 2:
                return vel - self.acc * period
            else:
                return vel
        # 加速域
        elif self.speedUp:
            # maxVelに達しない場合
            if 2 * self.acc * dest < 2 * np.power(maxVel, 2) - np.power(vel, 2):
                peakVel = np.sqrt((2*self.acc*dest + np.power(vel, 2))/2)
                if vel + self.acc * period < peakVel:
                    return vel + self.acc * period
                else:
                    self.speedUp = False
                    return peakVel
            else:
                if vel + self.acc * period <  maxVel:
                    return vel + self.acc * period
                else:
                    print("speed down")
                    self.speedUp = False
                    return maxVel
        # 減速域
        elif self.speedUp == False:
            if vel - self.acc * period > 0:
                return vel - self.acc * period
            else:
                return 0.0

def main():
    dest = 100.0
    maxVel = 20.0
    vel = 0.0
    pos = 0.0
    period = 0.05
    list_vel = [vel]
    list_pos = [pos]
    list_acc = [0.0]
    list_time = [0.0]
    i = 0
    pro = TrapezoidalProfile(acc = 10)
    while dest - pos > 1:
        i = i + 1
        vel = pro.calVel(dest = dest - pos, vel = vel, period = period, maxVel = maxVel)
        if vel == 0.0 and pro.speedUp == False:
            pos = dest
        else:
            pos = min(pos + vel * period, dest)
        list_vel.append(vel)
        list_pos.append(pos)
        list_time.append(i *period)

    plt.plot(list_time, list_pos)
    plt.plot(list_time, list_vel)
    plt.show()

if __name__ == "__main__":
    main()
            