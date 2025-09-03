#pragma once

#include <ArduinoOTA.h>
#include "log_helper.h"

/**
 * @brief OTA (Over-The-Air) アップデート機能をセットアップします。
 * 
 * ホスト名の設定、OTA開始/終了/進捗/エラー時のコールバック関数を登録し、
 * OTAサービスを開始します。
 */
void setupOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);

  ArduinoOTA.onStart([]() {
    logMessage(LOG_INFO, "OTA開始");
  });

  ArduinoOTA.onEnd([]() {
    logMessage(LOG_INFO, "OTA完了");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // logMessage(LOG_DEBUG, "OTA進捗: " + String((progress * 100) / total) + "%");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    logMessage(LOG_ERROR, "OTAエラー: " + String(error));
  });

  ArduinoOTA.begin();
  logMessage(LOG_INFO, "OTA準備完了");
}

/**
 * @brief OTAアップデートの処理を実行します。
 * 
 * ArduinoOTAライブラリのハンドル関数を呼び出し、
 * OTA通信を処理します。loop関数内で定期的に呼び出す必要があります。
 */
void handleOTA() {
  ArduinoOTA.handle();
}

