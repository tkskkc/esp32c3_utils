#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <settings.h>
#include "log_helper.h"
#include "mqtt_helper.h"

// Global client instances (declared extern in mqtt_helper.h)
WiFiClient espClient;
PubSubClient mqttClient(espClient);

/**
 * @brief MQTTブローカーへの接続を試行します。
 * 
 * 接続が確立されていない場合にのみ再接続を試み、
 * 成功時にはMQTTサブスクリプションを設定します。
 */
void connectMQTT() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

  if (!mqttClient.connected()) {
    Serial.println("[INFO] MQTT再接続を試行中...");
    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      logMessage(LOG_INFO, "MQTT接続成功");
      mqttClient.subscribe(MQTT_SUB_COMMAND);
      logMessage(LOG_DEBUG, "サブスク登録: " + String(MQTT_SUB_COMMAND));
    } else {
      Serial.println("[ERROR] MQTT接続失敗 RC=" + String(mqttClient.state()));
      // logMessage(LOG_ERROR, ...) は使わない（未接続だから！）
    }
  }
}

/**
 * @brief 指定されたトピックとペイロードでMQTTメッセージをパブリッシュします。
 * @param topic パブリッシュするMQTTトピック。
 * @param payload パブリッシュするメッセージペイロード。
 */
void publishMQTT(const String& topic, const String& payload) {
  static bool inLogging = false; // logMessage() を呼ぶ前にループ保護。
  if (inLogging) return;
  inLogging = true;

  mqttClient.publish(topic.c_str(), payload.c_str());
  logMessage(LOG_DEBUG, "MQTT送信: " + topic + " → " + payload);

  inLogging = false;
}

/**
 * @brief MQTTクライアントのループ処理を実行します。
 * 
 * 受信メッセージの処理や接続維持のために定期的に呼び出す必要があります。
 */
void handleMQTT() {
  mqttClient.loop();
}

/**
 * @brief 受信したMQTTメッセージを処理します。
 * 
 * 現在は "smart_speaker/speak" トピックのメッセージを処理します。
 * @param topic 受信したメッセージのトピック。
 * @param payload 受信したメッセージのペイロード。
 */
void handleReceivedMessage(const char* topic, const char* payload) {
  if (strcmp(topic, "smart_speaker/speak") == 0) {
    logMessage(LOG_INFO, String("音声受信指令: ") + payload);
    // → 今後 handleSpeechCommand(payload) に分離予定
  }
}

/**
 * @brief デバイスのステータス情報をMQTTでパブリッシュします。
 * 
 * RSSI、稼働時間、ヒープメモリ情報などを含むJSON形式のペイロードを構築し、
 * MQTT_PUB_LOGトピックに送信します。
 */
void sendDeviceStatus() {
  DynamicJsonDocument doc(256);
  doc["level"]   = LOG_INFO;
  doc["rssi"]    = WiFi.RSSI();
  doc["uptime"]  = millis() / 1000;  // 秒単位
  doc["heap"]    = ESP.getFreeHeap();

  String payload;
  serializeJson(doc, payload);
  publishMQTT(MQTT_PUB_LOG, payload);  // 例：smart_speaker/esp32c3_node01/log
}
