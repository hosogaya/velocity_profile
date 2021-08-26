import numpy as np
from matplotlib import pyplot as plt

class AccTraProfile:
    def __init__(self, jark, max_acc, max_vel, period):
        self.mode = 1
        self.jark = jark
        self.max_acc = max_acc
        self.max_vel = max_vel
        self.period = period
        self.time1 = max_acc / jark
        self.time2 = (max_vel - jark * np.power(self.time1, 2) * max_acc * self.time1) / max_acc
        self.A = max_acc * np.power(time1, 2) * 0.5
        self.AB = self.A + max_acc * (self.time2 - self.time1)
        self.D = max_vel * self.time1 + max_acc * (np.power(self.time2,2) - np.power(self.time1, 2)) /2

    def calAcc(self, vel):
        # 加速領域
        if self.mode == 1:
            # 加速度上昇
            if vel < self.A:
                next_acc = np.sqrt(2 * vel/self.jark) * self.jark
            # 定加速度
            elif vel < self.AB:
                next_acc = self.max_acc
            # 加速度低下
            else:
                next_acc = self.max_acc - np.sqrt(2 * (vel - self.AB) / self.jark) * self.jark
        # 定速領域
        elif self.mode == 2:
            next_acc = 0.0
        elif self.mode == 3:
            if abs(vel) < self.A:
                next_acc = -np.sqrt(2 * vel/self.jark) * self.jark
            elif abs(vel) < self.AB:
                next_acc = -self.max_acc
            else:
                next_acc = -self.max_acc + np.sqrt(2 * (abs(vel) - self.AB) / self.jark) * self.jark

        return next_acc

    def calVel(self, dest, vel):
        acc = self.calAcc(vel)
        next_vel = min(vel + acc * self.period, self.max_vel)
        next_vel = max(next_vel, -self.max_vel)
        if self.mode == 1:
            # 減速判定
            if dest < self.D:
                self.mode = 3
            else:
                if vel < self.A:
                    t = np.sqrt(2 * vel / self.jark)
                    C = self.D - self.jark * np.power(t, 3) / 6
                # 定加速度
                elif vel < self.AB:
                    t = (vel - self.A) / self.max_acc + self.time1
                    C = self.D - self.A - self.max_acc * (np.power(t, 2) - np.power(self.time1, 2)) * 0.5
                # 加速度低下
                else:
                    t = np.sqrt(2 * vel / self.jark)
                    C = max_vel * t - self.jark * np.power(t, 3) / 6

        elif self.mode == 2:
            if dest< self.D:
                self.mode = 3
        elif self.mode == 3:
            if vel < self.A:
                t = np.sqrt(2 * vel / self.jark)
                C = self.D - self.jark * np.power(t, 3) / 6
            # 定加速度
            elif vel < self.AB:
                t = (vel - self.A) / self.max_acc + self.time1
                C = self.D - self.A - self.max_acc * (np.power(t, 2) - np.power(self.time1, 2)) * 0.5
            # 加速度低下
            else:
                t = np.sqrt(2 * vel / self.jark)
                C = max_vel * t - self.jark * np.power(t, 3) / 6


def main():
    dist = 100.0
    max_acc = 30.0
    max_vel = 30.0
    period = 0.01
    vel = 0.0
    acc = 0.0
