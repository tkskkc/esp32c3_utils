#pragma once
#include <Arduino.h>
#include <esp_system.h>
#include <ArduinoJson.h>
#include "log_helper.h"

/**
 * @brief デバイスの起動ステータスをMQTTでパブリッシュします。
 * 
 * リセット理由、ファームウェアバージョン、コアID、ヒープ情報、フラッシュサイズなどを含む
 * JSON形式のペイロードを構築し、MQTT_PUB_STATUSトピックに送信します。
 */
void publishBootStatus();
