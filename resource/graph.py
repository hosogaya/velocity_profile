from matplotlib import pyplot as plt
import numpy as np

## 加速時の動き
t = [0.0]
a_s = -5.0
a = [a_s]
j = 20.0
period = 0.01
a_m = 20.0
v_m = 20.0
v_s = -10.0
v = [v_s]

t_1 = (a_m - a_s) / j
t_c2 = a_m / j
t_f = (v_m - v_s - a_s * t_1 + j/2 * (np.power(t_c2, 2) - np.power(t_1, 2))) / a_m + t_1
t_2 = t_f - t_c2

while t[-1] < t_1:
    next_a = min(a[-1] + j * period, a_m)
    a.append(next_a)
    next_v = min(v[-1] + a[-1] * period, v_m)
    v.append(next_v)
    t.append(t[-1] + period)

while t[-1] < t_2:
    next_a = min(a[-1] + j * period, a_m)
    a.append(next_a)
    next_v = min(v[-1] + a[-1] * period, v_m)
    v.append(next_v)
    t.append(t[-1] + period)

while t[-1] < t_f:
    next_a = max(a[-1] - j * period, 0.0)
    a.append(next_a)
    next_v = min(v[-1] + a[-1] * period, v_m)
    v.append(next_v)
    t.append(t[-1] + period)


fig1 = plt.figure()
plt.plot(t, a)
fig2 = plt.figure()
plt.plot(t, v)
plt.show()    