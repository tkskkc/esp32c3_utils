#pragma once

#include <WiFi.h>
#include <settings.h>
#include "log_helper.h"

/**
 * @brief WiFi接続を維持します。
 * 
 * WiFiが未接続の場合、再接続を試行します。接続成功時にはIPアドレス、SSID、MACアドレスをログに出力します。
 * 接続に失敗した場合でも、ループを維持し再起動は行いません。
 */
void maintainWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    logMessage(LOG_WARN, "WiFi未接続。再接続を試行中...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long timeout = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - timeout < 10000) {
      delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
      logMessage(LOG_INFO, "WiFi接続成功 IP: " + WiFi.localIP().toString() + 
                           " SSID: " + WiFi.SSID() + 
                           " MAC: " + WiFi.macAddress());

    } else {
      logMessage(LOG_WARN, "WiFi未接続継続。再起動せずループ維持。");
      // ESP.restart(); ← コメントアウト or 削除！
    }
  }
}
