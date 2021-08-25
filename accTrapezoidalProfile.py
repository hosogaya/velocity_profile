import numpy as np
from matplotlib import pyplot as plt

class AccTraProfile:
    def __init__(self, jark, max_acc, max_vel, period):
        self.speedUp = True
        self.jark = jark
        self.max_acc = max_acc
        self.max_vel = max_vel
        self.period = period
        self.time1 = max_acc / jark
        self.time2 = (max_vel - jark * np.power(self.time1, 2) * max_acc * self.time1) / max_acc
        self.A = max_acc * np.power(time1, 2) * 0.5
        self.AB = self.A + max_acc * (self.time2 - self.time1)

    def calAcc(self, vel):
        # 加速度上昇
        if vel < self.A:
            next_acc = np.sqrt(2 * vel/self.jark) * self.jark
        # 定加速度
        elif vel < self.AB:
            next_acc = self.max_acc
        # 加速度低下
        else:
            next_acc = self.max_acc - np.sqrt(2 * (vel - self.AB) / self.jark) * self.jark

        return next_acc


def main():
    dist = 100.0
    max_acc = 30.0
    max_vel = 30.0
    period = 0.01
    vel = 0.0
    acc = 0.0
