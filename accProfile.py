import numpy as np
from matplotlib import pyplot as plt

class AccProfile:
    def __init__(self, speedUp = True, thres = 0.95):
        self.speedUp = speedUp
        self.thres = thres

    def calPhase(self, acc, maxAcc):
        normAcc = acc / maxAcc  
        if normAcc > self.thres:
            self.speedUp = False
        if -normAcc < self.thres:
            self.speedUp = True

        if self.speedUp:
            return np.sin(normAcc)

    def calTimeF(self, vel, acc, phi):
        if phi == np.pi /2:
            phi = np.pi / 2 + 0.01
        if phi == np.pi /2 * 3:
            phi = np.pi /2 * 3 + 0.01
        return (2 * np.pi)/(1 - np.cos(phi)) * vel / acc

    def calMaxVel(self, dest, timeF, phi):
        return 2 * dest / timeF * (2 * np.pi - phi) /(2 * np.pi - phi + np.sin(phi))

    def calJark(self, dest, phi, timeF, maxAcc):
        return maxAcc * (2 * np.pi - phi) / timeF * np.cos(phi)

    