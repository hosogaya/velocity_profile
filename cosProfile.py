import numpy as np
from matplotlib import pyplot as plt

class CosProfile:
    def __init__(self, speedUp = True, thres = 0.95):
        self.speedUp = speedUp
        self.thres = thres

    def calPhase(self, vel, maxVel):
        normVel = abs(vel / maxVel)
        phi = np.arccos(1 - 2 * normVel)
        if abs(phi - np.pi) / np.pi  * 180 < 0.5:
            return np.pi

        elif abs(phi) / np.pi * 180 < 0.5:
            return 0.0
        elif vel < 0.0:
            phi = -phi

        return phi

    def calTimeF(self, dest, phi, maxVel):
        return (2 * dest / maxVel) * (2 * np.pi - phi)/ (2 * np.pi - phi + np.sin(phi))

    # calcurate time when reach max Vel form now
    def calTime1(self, phi, vel, maxVel, maxAcc):
        return (maxVel - vel) * (np.pi - phi) / (maxAcc * (1 + np.cos(phi)))

    # calcurate time when start to decelate
    def calTime2(self, maxVel, maxAcc):
        return (maxVel * np.pi) / (2 * maxAcc)

    def calAcc(self, dest, vel, maxVel, acc_2,  maxAcc, period ):
        phi = self.calPhase(vel = vel, maxVel = maxVel)
        if phi == np.pi:
            time2 = self.calTime2(maxVel = maxVel, maxAcc = maxAcc)
            B = abs(maxVel * time2 / 2)
            if B < dest:
                acc = 0.0
            else:
                self.speedUp = False
                acc = -0.1
        elif np.pi == 0.0:
            if self.speedUp:
                return 0.1
            else:
                return -0.1
        else:
            if self.speedUp == False:
                phi = 2 * np.pi - phi
                timeF = self.calTimeF(dest, phi, maxVel)
                acc = (maxVel / 2) * (2 * np.pi - phi) * np.sin(phi) / timeF
                return acc, phi

            time1 = self.calTime1(phi = phi, vel = vel, maxVel = maxVel, maxAcc = maxAcc)
            time2 = self.calTime2(maxVel = maxVel, maxAcc = maxAcc)
            A = abs(maxVel * (time1 + np.sin(phi)) / 2)
            B = abs(maxVel * time2 / 2)

            # 減速域
            if B - A >= dest * 1.0:
                self.speedUp = False
                if phi == np.pi:
                    print("slow down start")
                    acc = -0.1
                else:
                    print("slow down")
                    acc = -maxAcc * np.sin(phi)
            # 定速域
            elif abs(phi - np.pi) / np.pi  * 180 < 1:
                acc = 0.0
            # 加速域
            elif A + B < dest:
                if phi == 0.0:
                    acc = 0.01
                else:
                    acc = abs(maxAcc * np.sin(phi))
            # 減速しても届かない →　加速または定速域
            # 加速しても良いかを判断する
            else:# B - A < dest: # <= A + B
                # print("median")
                acc = maxAcc * np.sin(abs(phi))
                vel_2 = vel * acc * period
                # time1 = self.calTime1(phi = np.pi, vel = vel_2, maxVel = vel_2, maxAcc = maxAcc)
                time2 = self.calTime2(maxVel = vel_2, maxAcc = maxAcc)
                # A = abs(vel_2 * (time1 * np.sin(np.pi)) /2 )
                B = abs(vel_2 *time2 / 2)
                if B < dest: # 加速しても減速が間に合う
                    pass
                else: # 減速が間に合わない
                    acc = 0.0

        return acc, phi



def main():
    dest = 100.0
    maxAcc = 30.0
    maxVel = 30.0
    vel = 0.0
    pos = 0.0
    period = 0.01
    list_vel = [vel]
    list_pos = [pos]
    list_acc = [0.0]
    list_phi = [0.0]
    list_time = [0.0]
    i = 0
    pro = CosProfile(speedUp = True, thres = 0.9999)
    while dest - pos > 0.01:
        i = i + 1
        acc, phi = pro.calAcc(dest = dest - pos, vel = vel, maxVel = maxVel, acc_2 = list_acc[-1], maxAcc = maxAcc, period = period)
        vel = min(vel + acc * period, maxVel)
        print(dest - pos)
        pos = pos + vel * period
        list_acc.append(acc)
        list_vel.append(vel)
        list_pos.append(pos)
        list_phi.append(phi)
        list_time.append(i * period)

    fig1 = plt.figure()
    plt.plot(list_time, list_pos)
    plt.plot(list_time, list_vel)
    plt.plot(list_time, list_acc)
    fig2 = plt.figure()
    plt.plot(list_time, list_phi)
    plt.show()

if __name__ == "__main__":
    main()