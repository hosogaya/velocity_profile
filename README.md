# velocity_profile
<span style = "color : red">まだ目的地が正方向にある場合のみにしか対応していません</span>
# 台形プロファイル
初期加速度，速度をそれぞれ $a_s, v_s$とする．最大加速度 $a_m$ まで一定躍度 $j$ で変化する．その後一定加速度 $a_m$ で加速した後，一定躍度 $j$ で加速度が0になるように加速度を変化させ，最大速度 $v_m$に達する．このとき $a_m$ に達するまでの時間を $t_1$, 加速度が $a_m$に達した後再び変化し始める時間を $t_2$, 最大速度に達する時間を $t_f$とする．
## 加速時
![加速度の台形プロファイル](resource/trapezoidal_acc_edit.png)
![速度の変化](resource/trapezoidal_vel_edit.png)
### 加速時の速度
$$
\begin{align}
    t_1 &= \frac{a_m - a_s}{j} \\ 
    t_f - t_2 &= \frac{a_m}{j} \\
    t_{c1} &= \frac{a_m - a_s}{j} \\
    t_{c2} &= \frac{a_m}{j} \\
    v_{s1} &= \int_0^t (a_s + jt)\ dt + v_s =a_st + \frac{1}{2}jt^2 + v_s \\
    v_{12} &= \int_{t_1}^{t} (a_m)dt + v_{s1} = a_m(t - t_1) + v_{s1} \\
    v_{2f} &= \int_{t_2}^{t} (a_m - j(t - t_2))dt + v_{12} = a_m(t - t_2) - \frac{j}{2}(t - t_2)^2 + v_{12} \\
    v_{c1} &= v_s + \int_0^{t} \left(a_s + jt\right)dt \\ &
    = v_s + a_st + \frac{j}{2}{t^2} \\
    v_{cc} &= v_s + a_st_{c1} + \frac{j}{2}t_{c1}^2 + \int_{t_{c1}}^t a_m dt\\ 
    &= v_s + a_st_{c1} + \frac{j}{2}t_{c1}^2 + a_m(t_2 - t_1) \\
    v_{c2} &= v_s + a_st_{c1} + \frac{j}{2}t_{c1}^2 + a_m(t - t_1) + \int_{t_2}^t \left(a_m - j(t - t_2)\right)dt \\
    &= v_s + a_st_{c1} + \frac{j}{2}t_{c1}^2 + a_m(t_2 - t_1) + a_m(t - t_2) - \frac{j}{2}(t - t_2)^2
\end{align}
$$

### 加速時の速度の拘束条件
$$
\begin{aligned}
v_m &=  v_s + a_st_{c1} + \frac{j}{2}t_{c1}^2 + a_m(t_2 - t_1) + a_m(t_f - t_2) - \frac{j}{2}(t_f - t_2)^2\\
\Rightarrow v_m &= v_s + a_st_{c1} + \frac{j}{2}t_{c1}^2 + a_m(t_f - t_1) - \frac{j}{2}t_{c2}^2 \\
\Rightarrow t_f &= \frac{1}{a_m}\left(v_m - v_s - a_st_{c1} + \frac{j}{2}\left(t_{c2}^2 - t_{c1}^2\right)\right) + t_{c1} \\
&= \frac{v_m}{a_m} + t_{c1} 
\end{aligned}
$$
したがって
$$
\begin{align}
    t_2 &= t_f - t_{c2} \\
    &= \frac{1}{a_m}\left(v_m - v_s - a_st_{c1} + \frac{j}{2}\left(t_{c2}^2 - t_{c1}^2\right)\right) + t_{c1} - t_{c2} \\
    &= \frac{v_m}{a_m} + t_{c1} - t_{c2}
\end{align}
$$
また，
$$
\begin{align}
    t_f - t_1 &= \frac{1}{a_m}\left(v_m - v_s - a_st_{c1} + \frac{j}{2}\left(t_{c2}^2 - t_{c1}^2\right)\right) \\
    &= \frac{1}{a_m}\left\{v_m - v_s + \frac{a_s(a_m - a_s)}{j} + \frac{a_m^2 - (a_m - a_s)^2}{2j}\right\} \\
    &= \frac{1}{a_m}\left\{v_m - v_s + \frac{a_s^2}{2j}\right\} \\
    &= \frac{v_m}{a_m}
\end{align}
$$

### 加速時の走行距離
$$
\begin{align}
d_{s1} &= \int_0^t \left(a_st + \frac{j}{2}t^2 + v_s\right) dt = \frac{a_s}{2}t^2 + \frac{j}{6}t^3 + v_st \\
d_{12} &= \int_{t_1}^t \left(a_m(t - t_1) + v_{s1}\right) dt = \frac{a_m}{2}(t - t_1)^2 + v_{s1}(t - t_1) \\
d_{2f} &= \int_{t_2}^{t} \left(a_m(t - t_2) - \frac{j}{2}(t - t_2)^2 + v_{12}\right)dt \\
 &= \frac{a_m}{2}(t - t_2)^2 - \frac{j}{6}(t - t_2)^3  + v_{12}(t - t_2) \\
d_a &= d_{s1} + d_{12} + d_{2f}\\
&= v_st_f + a_st_1\left(t_f - \frac{t_1}{2} \right) + \frac{a_m}{2}(t_f - t_1)^2 - \frac{j}{6}\left\{t_1^2(2t_1 - 3t_f) + (t_f - t_2)^3 \right\}
\end{align}
$$

## 減速時
### 減速時の速度
$$
\begin{align}
t_1 &= \frac{a_m}{j} \\
t_f - t_2 &= \frac{a_m}{j} = t_1 \\
v_{s1} &= v_m - \int_0^t \left(jt\right)dt = v_m - \frac{j}{2}t^2 \\
v_{12} &= v_{s1} - \int_{t_1}^t \left(a_m\right)dt = v_{s1} - a_m(t - t_1) \\
v_{2f} &= v_{12} - \int_{t_2}^t\left(a_m - j(t - t_2)\right)dt = v_{12} - a_m(t - t_2) + \frac{j}{2}(t - t_2)^2  
\end{align}
$$
### 減速時の速度の拘束条件
$$
\begin{align}
0 &= v_m - a_m(t_f - t_1) + \frac{j}{2}\left((t_f - t_2)^2 - t_1^2\right) \\
\Rightarrow v_m &= a_m(t_f - t_1) \\
\Rightarrow t_f &= \frac{v_m}{a_m} + t_1
\end{align}
$$
### 減速時の走行距離
$$
\begin{align}
d_{s1} &= \int_0^t \left(v_m - \frac{j}{2}t^2\right) dt = v_mt - \frac{j}{6}t^3\\
d_{12} &= \int_{t_1}^t \left(v_{s1} - a_m(t - t_1)\right) dt = v_{s1}(t - t_1) - \frac{a_m}{2}(t - t_1)^2  \\
d_{2f} &= \int_{t_2}^t \left(v_{12} - a_m(t - t_2) + \frac{j}{2}(t - t_2)^2\right) dt\\
&= v_{12}(t - t_2) - \frac{a_m}{2}(t - t_2)^2 + \frac{j}{6}(t - t_2)^3 \\
d_d &= d_{s1} + d_{12} + d_{2f} \\
&= v_mt_f - \frac{a_m}{2}(t_f - t_1)^2 + \frac{j}{6}\left\{t_1^2(2t_1 - 3t_f) + (t_f - t_2)^3\right\}
\end{align}
$$

##　加減速時に走破する距離
$$
\begin{align}
    d_a + d_d &= v_st_{af} + a_st_{a1}\left(t_{af} - \frac{t_{a1}}{2}\right) + \frac{j}{6}\left(t_{a1}^2t_{af} - t_{d1}^2t_{df}\right) + v_mt_{df} \\
    &= \frac{v_m}{a_m}\left\{v_s + \frac{a_s}{j}(a_m - a_s) + \frac{(a_m - a_s)^2}{6j} - \frac{a_m^2}{6j} + v_m \right\} \\
    &\frac{a_m - a_s}{2j}v_s - \frac{a_s}{2j^2}(a_m - a_s)^2 + \frac{(a_m - a_s)^3}{6j^2} - \frac{a_m^3}{6j^2} + \frac{a_m}{j}v_m \\
    &= \frac{v_m}{a_m}\left\{v_m - v_s + \frac{a_s^2}{6j} + \frac{2a_sa_m}{3j} \right\} - \frac{a_s}{12j^2}(11a_s^2 - 21a_sa_m + 12a_m^2) + \frac{a_m}{j}v_m \\
    \Rightarrow 0 &= v_m^2 + a_m\left\{\frac{a_m}{j} - v_s + \frac{a_s^2}{6j} + \frac{2a_sa_m}{3j}\right\}v_m \\
    &- \frac{a_sa_m}{12j^2}(11a_s^2 - 21a_sa_m + 12a_m^2) - d_a - d_d
\end{align}
$$
$k = a_m\left\{\frac{a_m}{j} - v_s + \frac{a_s^2}{6j} + \frac{2a_sa_m}{3j}\right\}$，$l = \frac{a_sa_m}{12j^2}(11a_s^2 - 21a_sa_m + 12a_m^2) + d_a + d_d$とすると
$$
\begin{align}
    &v_m^2 + kv_m - l = 0 \\
    \Rightarrow &v_m = \frac{-k}{2} \pm\frac{\sqrt{k^2 + 4l}}{2} 
\end{align}
$$

## 直ちに終端速度に向かう場合
直ちに終端速度に向かって加減速する場合に走破する距離を$d_i$とする
### 減速する場合
$$
\begin{align}
    t_1 &= \frac{a_m + a_s}{j} \\
    t_f - t_2 &= \frac{a_m}{j} \\
    v_{s1} &= v_s - \int_0^{t_1} \left(a_s - jt\right)dt \\
    &= v_s - a_st_1 + \frac{j}{2}t_1^2 \\
    v_{12} &= v_{s1} - \int_{t_1}^t a_m dt \\
    &= v_{s1} - a_m(t - t_1) \\
    v_{2f} &= v_{12} - \int_{t_2}^t \left(a_m - j(t - t_2)\right) \\
    &= v_{12} - a_m(t - t_2) + \frac{j}{2}(t - t_2)^2
\end{align}
$$

$$
\begin{align}
    d_{s1} &= \int_0^t \left(v_s - a_st + \frac{j}{2}t^2\right) \\
    &= v_st - \frac{a_s}{2}t^2  + \frac{j}{6}t^3 \\
    d_{12} &= \int_{t_1}^t \left(v_{s1} - a_m(t - t_1)\right)dt \\
    &= v_{s1}(t - t_1) - \frac{a_m}{2}(t - t_1)^2 \\
    d_{2f} &= \int_{t_2}^t \left(v_{12} - a_m(t - t_2) + \frac{j}{2}(t - t_2)^2\right) \\
    &= v_{12}(t - t_2) - \frac{a_m}{2}(t - t_2)^2 + \frac{j}{6}(t - t_2)^3
\end{align}
$$

## 条件分岐
### 符号に関して
1. $d \ge 0$であれば$v_m > 0$
2. $d \ge 0 \cap v_s < v_m$ならば$a_m > 0$
2. $d \ge 0 \cap v_s \ge v_m$ならば$a_m \le 0$
### 設計に関して
1. $d_a + d_d \le d$であれば$v_m, a_m$そのまま
2. $d_a + d_d > d$であれば$v_m$を再設計する必要あり
3. $d_i >= d$すぐに終端速度に向けて減速する必要あり