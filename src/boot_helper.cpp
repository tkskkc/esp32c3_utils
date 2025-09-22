#include "boot_helper.h"
#include "mqtt_helper.h"
#include <settings.h>

/**
 * @brief デバイスの起動ステータスをMQTTでパブリッシュします。
 *
 * リセット理由、ファームウェアバージョン、コアID、ヒープ情報、フラッシュサイズなどを含む
 * JSON形式のペイロードを構築し、MQTT_PUB_STATUSトピックに送信します。
 */
void publishBootStatus() {
  // String reason = ESP.getResetReason();
  esp_reset_reason_t reasonCode = esp_reset_reason();

  String reasonStr;
  switch (reasonCode) {
  case ESP_RST_POWERON:
    reasonStr = "POWERON_RESET";
    break;
  case ESP_RST_SW:
    reasonStr = "SOFTWARE_RESET";
    break;
  case ESP_RST_DEEPSLEEP:
    reasonStr = "DEEPSLEEP_RESET";
    break;
  case ESP_RST_BROWNOUT:
    reasonStr = "BROWNOUT_RESET";
    break;
  case ESP_RST_EXT:
    reasonStr = "EXT_RESET";
    break;
  default:
    reasonStr = "UNKNOWN";
    break;
  }

  DynamicJsonDocument doc(128);
  doc["boot"]      = "ok";
  doc["version"]   = DEVICE_VERSION;
  doc["reason"]    = reasonStr;
  doc["core"]      = xPortGetCoreID();
  doc["heapMax"]   = ESP.getMaxAllocHeap();
  doc["flashSize"] = ESP.getFlashChipSize();
  doc["level"]     = LOG_INFO;

  String payload;
  serializeJson(doc, payload);
  publishMQTT(MQTT_PUB_STATUS, payload);
}
