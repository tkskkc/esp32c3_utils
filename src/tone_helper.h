#pragma once
#include <Arduino.h>
#include <driver/ledc.h>

#define BUZZER_PIN 4
#define BZ_CHANNEL LEDC_CHANNEL_0
#define BZ_TIMER   LEDC_TIMER_0
#define BZ_MODE    LEDC_LOW_SPEED_MODE  // High-Speed はC3で未定義
#define BZ_RES     LEDC_TIMER_8_BIT     // 8bit分解能

// freq[Hz] を duration_ms[ms] だけ鳴らす
/**
 * @brief 指定された周波数と期間でトーンを生成します。
 *
 * LEDCタイマーとチャンネルを設定し、指定された周波数でデューティサイクル50%のPWM信号を出力します。
 * @param freq トーンの周波数（Hz）。
 * @param duration_ms トーンを鳴らす期間（ミリ秒）。
 */
void toneESP(uint32_t freq, uint32_t duration_ms) {
  // 1) タイマー設定
  ledc_timer_config_t timer_conf = {};
  timer_conf.speed_mode      = BZ_MODE;
  timer_conf.duty_resolution = BZ_RES;
  timer_conf.timer_num       = BZ_TIMER;
  timer_conf.freq_hz         = freq;
  timer_conf.clk_cfg         = LEDC_AUTO_CLK;
  ledc_timer_config(&timer_conf);

  // 2) チャンネル設定（最初は duty=0 でセット）
  ledc_channel_config_t chan_conf = {};
  chan_conf.gpio_num   = BUZZER_PIN;
  chan_conf.speed_mode = BZ_MODE;
  chan_conf.channel    = BZ_CHANNEL;
  chan_conf.intr_type  = LEDC_INTR_DISABLE;
  chan_conf.timer_sel  = BZ_TIMER;
  chan_conf.duty       = 0;
  chan_conf.hpoint     = 0;
  ledc_channel_config(&chan_conf);

  // 3) 出力ON  (50%デューティ)
  uint32_t duty = (1 << BZ_RES) / 2;
  ledc_set_duty(BZ_MODE, BZ_CHANNEL, duty);
  ledc_update_duty(BZ_MODE, BZ_CHANNEL);

  delay(duration_ms);

  // 4) 出力OFF
  ledc_set_duty(BZ_MODE, BZ_CHANNEL, 0);
  ledc_update_duty(BZ_MODE, BZ_CHANNEL);
}

/**
 * @brief 成功を示す短いビープ音を鳴らします。
 */
inline void beepOK() {
  toneESP(1500, 80);
  delay(50);
  toneESP(2000, 80);
}

/**
 * @brief エラーを示すビープ音を鳴らします。
 */
inline void beepError() { toneESP(400, 300); }
