#pragma once
#include "log_helper.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_system.h>

/**
 * @brief デバイスの起動ステータスをMQTTでパブリッシュします。
 *
 * リセット理由、ファームウェアバージョン、コアID、ヒープ情報、フラッシュサイズなどを含む
 * JSON形式のペイロードを構築し、MQTT_PUB_STATUSトピックに送信します。
 */
void publishBootStatus();
