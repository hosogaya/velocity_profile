# 速度をcosでプロファイル
# 終端速度域あり
# 終端速度と最大加速度は設定される

import numpy as np
from matplotlib import pyplot as plt
 
class Train:
  def __init__(self, velocity, destination, maxVelocity, maxAcc):
    self.vel = []
    self.vel.append(velocity)
    self.pos = [0.0]
    self.dest = destination
    self.maxVel = maxVelocity
    self.maxAcc = maxAcc
    self.speedUp = True
    self.mode = True
    self.thres = 0.9999
    self.period = 0.05

  def calPhase(self, vel):
    normVel = vel / self.maxVel
    if self.thres < normVel:
      # print("speed down")
      self.speedUp = False
    if self.speedUp:
      return np.arccos(1 - 2 * normVel)
    else:
      return 2 * np.pi - np.arccos(1 - 2 * normVel)

  def calTerminalTime(self, phi):
    return 2 * (self.dest - self.pos[-1])/self.maxVel * (2 * np.pi - phi) / (2*np.pi - phi + np.sin(phi))
  
  def calTime1(self, phi, maxVel):
    return (maxVel - self.vel[-1]) * (np.pi - phi) / self.maxAcc / (1 + np.cos(phi))

  def calTime2(self, maxVel):
    return (maxVel * np.pi) / (2 * self.maxAcc)

  def calAccel(self, timeF, phi):
    return (self.maxVel / 2) * (2 * np.pi - phi)/timeF * np.sin(phi)

  def calAcc1(self, phi):
    return self.maxAcc * np.sin(phi)

  def calVel(self):
    phi = self.calPhase(self.vel[-1])
    time1 = self.calTime1(phi, self.maxVel)
    time2 = self.calTime2(self.maxVel)
    A = self.maxVel/2 * (time1 + np.sin(phi))
    B = self.maxVel * time2/ 2
    if self.speedUp and A + B > self.dest - self.pos[-1] or self.mode == False:
      self.mode = False
      timeF = self.calTerminalTime(phi)
      acc = self.calAccel(timeF, phi)
    else:
      if self.speedUp:
        acc = self.calAcc1(phi)
      else:
        if B < (self.dest - self.pos[-1] + self.maxVel * self.period) * 0.90:
          return self.maxVel
        else:
          acc = self.calAcc1(phi)

    # acc = self.calAccel(timeF, phi)
    if abs(acc) < 0.2:
      acc = 0.2*(2 * self.speedUp - 1)
    # 次ステップでたどり着く場合
    if self.dest - self.pos[-1] < self.vel[-1] * self.period + acc / 2 * np.power(self.period, 2):
      val = self.dest - self.pos[-1] / self.period
    else: 
      val = min(self.vel[-1] + acc * self.period, self.maxVel)
    return val
  
  def run(self):
    while self.dest - self.pos[-1] > 0.1:
      new_vel = self.calVel()
      self.vel.append(new_vel)
      self.pos.append(self.pos[-1] + new_vel * self.period)
    
if __name__ == "__main__":
  train = Train(0, 100.0, 20.0, maxAcc = 10.0)
  train.run()
  plt.plot(train.pos)
  plt.plot(train.vel)
  plt.show()