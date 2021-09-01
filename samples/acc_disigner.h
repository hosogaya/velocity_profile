/**
 * @file accel_designer.h
 * @author Ryotaro Onuki (kerikun11+github@gmail.com)
 * @ref https://www.kerislab.jp/posts/2018-04-29-accel-designer4/
 * @brief 霍晞屬縺ｮ諡俶據繧呈ｺ縺溘☆蜉�貂幃溯ｵｰ陦瑚ｻ碁％繧堤函謌舌☆繧九け繝ｩ繧ｹ繧剃ｿ晄戟縺吶ｋ繝輔ぃ繧､繝ｫ
 * @date 2020-04-19
 */
#pragma once

#include "accel_curve.h"

#include <algorithm> //< for std::max, std::min
#include <array>
#include <iostream> //< for std::cout
#include <ostream>

/**
 * @brief 蛻ｶ蠕｡髢｢菫ゅ�蜷榊燕遨ｺ髢�
 */
namespace ctrl {

/**
 * @brief 諡俶據譚｡莉ｶ繧呈ｺ縺溘☆譖ｲ邱壼刈貂幃溘�霆碁％繧堤函謌舌☆繧九け繝ｩ繧ｹ
 *
 * - 遘ｻ蜍戊ｷ晞屬縺ｮ諡俶據譚｡莉ｶ繧呈ｺ縺溘☆譖ｲ邱壼刈騾溯ｻ碁％繧堤函謌舌☆繧�
 * - 蜷�凾蛻ｻ $t$ 縺ｫ縺翫￠繧玖ｺ榊ｺｦ $j(t)$縲∝刈騾溷ｺｦ $a(t)$縲�溷ｺｦ $v(t)$縲∽ｽ咲ｽｮ $x(t)$
 * 繧呈署萓帙☆繧�
 * - 譛螟ｧ蜉�騾溷ｺｦ $a_{\max}$ 縺ｨ蟋狗せ騾溷ｺｦ $v_s$
 * 縺ｪ縺ｩ諡俶據谺｡隨ｬ縺ｧ縺ｯ逶ｮ讓咎溷ｺｦ縺ｫ驕斐☆繧九％縺ｨ縺後〒縺阪↑縺��ｴ蜷医′縺ゅｋ縺ｮ縺ｧ豕ｨ諢上☆繧�
 */
class AccelDesigner {
public:
  /**
   * @brief 蛻晄悄蛹紋ｻ倥″繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
   *
   * @param j_max     譛螟ｧ霄榊ｺｦ縺ｮ螟ｧ縺阪＆ [m/s/s/s]縲∵ｭ｣縺ｧ縺ゅｋ縺薙→
   * @param a_max     譛螟ｧ蜉�騾溷ｺｦ縺ｮ螟ｧ縺阪＆ [m/s/s], 豁｣縺ｧ縺ゅｋ縺薙→
   * @param v_sat     鬟ｽ蜥碁溷ｺｦ縺ｮ螟ｧ縺阪＆ [m/s]縲∵ｭ｣縺ｧ縺ゅｋ縺薙→
   * @param v_start   蟋狗せ騾溷ｺｦ [m/s]
   * @param v_target  逶ｮ讓咎溷ｺｦ [m/s]
   * @param v_end     邨らせ騾溷ｺｦ [m/s]
   * @param dist      遘ｻ蜍戊ｷ晞屬 [m]
   * @param x_start   蟋狗せ菴咲ｽｮ [m] (繧ｪ繝励す繝ｧ繝ｳ)
   * @param t_start   蟋狗せ譎ょ綾 [s] (繧ｪ繝励す繝ｧ繝ｳ)
   */
  AccelDesigner(const float j_max, const float a_max, const float v_sat,
                const float v_start, const float v_target, const float dist,
                const float x_start = 0, const float t_start = 0) {
    reset(j_max, a_max, v_sat, v_start, v_target, dist, x_start, t_start);
  }
  /**
   * @brief 遨ｺ縺ｮ繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ縲ゅ≠縺ｨ縺ｧ reset() 縺ｫ繧医ｊ蛻晄悄蛹悶☆繧九％縺ｨ縲�
   */
  AccelDesigner() { t0 = t1 = t2 = t3 = x0 = x3 = 0; }
  /**
   * @brief 蠑墓焚縺ｮ諡俶據譚｡莉ｶ縺九ｉ譖ｲ邱壹ｒ逕滓�縺吶ｋ縲�
   * 縺薙�髢｢謨ｰ縺ｫ繧医▲縺ｦ縲√☆縺ｹ縺ｦ縺ｮ螟画焚縺悟�譛溷喧縺輔ｌ繧九�(貍上ｌ縺ｯ縺ｪ縺�)
   *
   * @param j_max     譛螟ｧ霄榊ｺｦ縺ｮ螟ｧ縺阪＆ [m/s/s/s]縲∵ｭ｣縺ｧ縺ゅｋ縺薙→
   * @param a_max     譛螟ｧ蜉�騾溷ｺｦ縺ｮ螟ｧ縺阪＆ [m/s/s], 豁｣縺ｧ縺ゅｋ縺薙→
   * @param v_sat     鬟ｽ蜥碁溷ｺｦ縺ｮ螟ｧ縺阪＆ [m/s]縲∵ｭ｣縺ｧ縺ゅｋ縺薙→
   * @param v_start   蟋狗せ騾溷ｺｦ [m/s]
   * @param v_target  逶ｮ讓咎溷ｺｦ [m/s]
   * @param v_end     邨らせ騾溷ｺｦ [m/s]
   * @param dist      遘ｻ蜍戊ｷ晞屬 [m]
   * @param x_start   蟋狗せ菴咲ｽｮ [m] (繧ｪ繝励す繝ｧ繝ｳ)
   * @param t_start   蟋狗せ譎ょ綾 [s] (繧ｪ繝励す繝ｧ繝ｳ)
   */
  void reset(const float j_max, const float a_max, const float v_sat,
             const float v_start, const float v_target, const float dist,
             const float x_start = 0, const float t_start = 0) {
    /* 譛螟ｧ騾溷ｺｦ縺ｮ莉ｮ鄂ｮ縺� */
    float v_max = dist > 0 ? std::max({v_start, v_sat, v_target})
                           : std::min({v_start, -v_sat, v_target});
    /* 襍ｰ陦瑚ｷ晞屬縺九ｉ邨らせ騾溷ｺｦ$v_e$繧堤ｮ怜� */
    float v_end = v_target;
    const auto dist_min =
        AccelCurve::calcMinDistance(j_max, a_max, v_start, v_end);
    // logd << "dist_min: " << dist_min << std::endl;
    if (std::abs(dist) < std::abs(dist_min)) {
      logd << "vs -> ve != vt" << std::endl;
      /* 逶ｮ讓咎溷ｺｦ$v_t$縺ｫ蜷代°縺�∬ｵｰ陦瑚ｷ晞屬$d$縺ｧ蛻ｰ驕斐＠蠕励ｋ邨らせ騾溷ｺｦ$v_e$繧堤ｮ怜� */
      v_end =
          AccelCurve::calcVelocityEnd(j_max, a_max, v_start, v_target, dist);
      v_max = v_end; //< 襍ｰ陦瑚ｷ晞屬縺ｮ諡俶據繧呈ｺ縺溘☆縺溘ａ縲�｣ｽ蜥碁溷ｺｦ縺ｾ縺ｧ蜉�騾溘〒縺阪↑縺�
      // logd << "ve: " << v_end << std::endl;
    }
    /* 譖ｲ邱壹ｒ逕滓� */
    ac.reset(j_max, a_max, v_start, v_max); //< 蜉�騾�
    dc.reset(j_max, a_max, v_max, v_end);   //< 貂幃�
    /* 鬟ｽ蜥碁溷ｺｦ縺ｾ縺ｧ蜉�騾溘☆繧九→襍ｰ陦瑚ｷ晞屬縺ｮ諡俶據繧呈ｺ縺溘＆縺ｪ縺��ｴ蜷医�蜃ｦ逅� */
    const auto d_sum = ac.x_end() + dc.x_end();
    if (std::abs(dist) < std::abs(d_sum)) {
      logd << "vs -> vm -> ve" << std::endl;
      /* 襍ｰ陦瑚ｷ晞屬縺九ｉ譛螟ｧ騾溷ｺｦ$v_m$繧堤ｮ怜�; 荳玖ｨ�v_max縺ｯ荳願ｨ�v_max莉･荳九↓縺ｪ繧� */
      v_max = AccelCurve::calcVelocityMax(j_max, a_max, v_start, v_end, dist);
      /* 辟｡鬧�↑貂幃溘ｒ蝗樣∩ */
      v_max = dist > 0 ? std::max({v_start, v_max, v_end})
                       : std::min({v_start, v_max, v_end});
      ac.reset(j_max, a_max, v_start, v_max); //< 蜉�騾�
      dc.reset(j_max, a_max, v_max, v_end);   //< 貂幃�
    }
    /* 蜷�ｮ壽焚縺ｮ邂怜� */
    x0 = x_start;
    x3 = x_start + dist;
    t0 = t_start;
    t1 = t0 + ac.t_end(); //< 譖ｲ邱壼刈騾溽ｵゆｺ��譎ょ綾
    t2 = t0 + ac.t_end() +
         (dist - ac.x_end() - dc.x_end()) / v_max; //< 遲蛾溯ｵｰ陦檎ｵゆｺ��譎ょ綾
    t3 = t0 + ac.t_end() + (dist - ac.x_end() - dc.x_end()) / v_max +
         dc.t_end(); //< 譖ｲ邱壽ｸ幃溽ｵゆｺ��譎ょ綾
    /* 蜃ｺ蜉帙�繝√ぉ繝�け */
    const float e = 0.01f; //< 謨ｰ蛟､隱､蟾ｮ蛻�
    bool show_info = false;
    /* 邨らせ騾溷ｺｦ */
    if (std::abs(v_start - v_end) > e + std::abs(v_start - v_target)) {
      std::cerr << "Error: Velocity Target!" << std::endl;
      show_info = true;
    }
    /* 譛螟ｧ騾溷ｺｦ */
    if (std::abs(v_max) >
        e + std::max({v_sat, std::abs(v_start), std::abs(v_end)})) {
      std::cerr << "Error: Velocity Saturation!" << std::endl;
      show_info = true;
    }
    /* 繧ｿ繧､繝�繧ｹ繧ｿ繝ｳ繝� */
    if (!(t0 <= t1 + e && t1 <= t2 + e && t2 <= t3 + e)) {
      loge << "Error: Time Point Relationship!" << std::endl;
      show_info = true;
    }
    /* 蜈･蜉帶ュ蝣ｱ縺ｮ陦ｨ遉ｺ */
    if (show_info) {
      loge << "Constraints:"
           << "\tj_max: " << j_max << "\ta_max: " << a_max
           << "\tv_start: " << v_start << "\tv_sat: " << v_sat
           << "\tv_target: " << v_target << "\tdist: " << dist << std::endl;
      loge << "ad.reset(" << j_max << ", " << a_max << ", " << v_sat << ", "
           << v_start << ", " << v_target << ", " << dist << ");" << std::endl;
      /* 陦ｨ遉ｺ */
      loge << "Time Stamp: "
           << "\tt0: " << t0 << "\tt1: " << t1 << "\tt2: " << t2
           << "\tt3: " << t3 << std::endl;
      loge << "Position:   "
           << "\tx0: " << x0 << "\tx1: " << x0 + ac.x_end()
           << "\tx2: " << x0 + (dist - dc.x_end()) << "\tx3: " << x3
           << std::endl;
      loge << "Velocity:   "
           << "\tv0: " << v_start << "\tv1: " << v(t1) << "\tv2: " << v(t2)
           << "\tv3: " << v_end << std::endl;
    }
  }
  /**
   * @brief 譎ょ綾 $t$ 縺ｫ縺翫￠繧玖ｺ榊ｺｦ $j$
   * @param t 譎ょ綾[s]
   * @return j 霄榊ｺｦ[m/s/s/s]
   */
  float j(const float t) const {
    if (t < t2)
      return ac.j(t - t0);
    else
      return dc.j(t - t2);
  }
  /**
   * @brief 譎ょ綾 $t$ 縺ｫ縺翫￠繧句刈騾溷ｺｦ $a$
   * @param t 譎ょ綾 [s]
   * @return a 蜉�騾溷ｺｦ [m/s/s]
   */
  float a(const float t) const {
    if (t < t2)
      return ac.a(t - t0);
    else
      return dc.a(t - t2);
  }
  /**
   * @brief 譎ょ綾 $t$ 縺ｫ縺翫￠繧矩溷ｺｦ $v$
   * @param t 譎ょ綾 [s]
   * @return v 騾溷ｺｦ [m/s]
   */
  float v(const float t) const {
    if (t < t2)
      return ac.v(t - t0);
    else
      return dc.v(t - t2);
  }
  /**
   * @brief 譎ょ綾 $t$ 縺ｫ縺翫￠繧倶ｽ咲ｽｮ $x$
   * @param t 譎ょ綾 [s]
   * @return x 菴咲ｽｮ [m]
   */
  float x(const float t) const {
    if (t < t2)
      return x0 + ac.x(t - t0);
    else
      return x3 - dc.x_end() + dc.x(t - t2);
  }
  /**
   * @brief 邨らせ譎ょ綾 [s]
   */
  float t_end() const { return t3; }
  /**
   * @brief 邨らせ騾溷ｺｦ [m/s]
   */
  float v_end() const { return dc.v_end(); }
  /**
   * @brief 邨らせ菴咲ｽｮ [m]
   */
  float x_end() const { return x3; }
  /**
   * @brief 蠅�阜縺ｮ譎ょ綾
   */
  float t_0() const { return t0; }
  float t_1() const { return t1; }
  float t_2() const { return t2; }
  float t_3() const { return t3; }
  /**
   * @brief stdout 縺ｫ霆碁％縺ｮcsv繧貞�蜉帙☆繧矩未謨ｰ縲�
   */
  void printCsv(const float t_interval = 0.001f) const {
    printCsv(std::cout, t_interval);
  }
  /**
   * @brief std::ostream 縺ｫ霆碁％縺ｮcsv繧貞�蜉帙☆繧矩未謨ｰ縲�
   */
  void printCsv(std::ostream &os, const float t_interval = 0.001f) const {
    for (float t = t0; t < t_end(); t += t_interval)
      os << t << "," << j(t) << "," << a(t) << "," << v(t) << "," << x(t)
         << std::endl;
  }
  /**
   * @brief 諠��ｱ縺ｮ陦ｨ遉ｺ
   */
  friend std::ostream &operator<<(std::ostream &os, const AccelDesigner &obj) {
    os << "AccelDesigner:";
    os << "\td: " << obj.x3 - obj.x0;
    os << "\tvs: " << obj.ac.v(0);
    os << "\tvm: " << obj.ac.v_end();
    os << "\tve: " << obj.dc.v_end();
    os << "\tt0: " << obj.t0;
    os << "\tt1: " << obj.t1;
    os << "\tt2: " << obj.t2;
    os << "\tt3: " << obj.t3;
    return os;
  }
  /**
   * @brief 蠅�阜縺ｮ繧ｿ繧､繝�繧ｹ繧ｿ繝ｳ繝励ｒ蜿門ｾ�
   * @return std::array<float, 8> 蠅�阜縺ｮ繧ｿ繧､繝�繧ｹ繧ｿ繝ｳ繝励�驟榊�
   */
  const std::array<float, 8> getTimeStamp() const {
    return {{
        t0 + ac.t_0(),
        t0 + ac.t_1(),
        t0 + ac.t_2(),
        t0 + ac.t_3(),
        t2 + dc.t_0(),
        t2 + dc.t_1(),
        t2 + dc.t_2(),
        t2 + dc.t_3(),
    }};
  }

protected:
  float t0, t1, t2, t3; /**< @brief 蠅�阜轤ｹ縺ｮ譎ょ綾 [s] */
  float x0, x3;         /**< @brief 蠅�阜轤ｹ縺ｮ菴咲ｽｮ [m] */
  AccelCurve ac, dc; /**< @brief 譖ｲ邱壼刈騾溘∵峇邱壽ｸ幃溘が繝悶ず繧ｧ繧ｯ繝� */
};

} // namespace ctrl