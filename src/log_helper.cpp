#include "log_helper.h"
#include "mqtt_helper.h"
#include <Arduino.h>
#include <ArduinoJson.h> // For DynamicJsonDocument
#include <settings.h>

/**
 * @brief LogLevel列挙型に対応する文字列を返します。
 * @param level ログレベル。
 * @return ログレベルの文字列表現。
 */
String getLogLevelString(LogLevel level) {
  switch (level) {
  case LOG_ERROR:
    return "ERROR";
  case LOG_WARN:
    return "WARN";
  case LOG_INFO:
    return "INFO";
  case LOG_DEBUG:
    return "DEBUG";
  default:
    return "UNKNOWN";
  }
}

/**
 * @brief LogLevel列挙型に対応する整数値を返します。
 * @param level ログレベル。
 * @return ログレベルの整数表現。
 */
int getLogLevelInt(LogLevel level) {
  switch (level) {
  case LOG_ERROR:
    return 1;
  case LOG_WARN:
    return 2;
  case LOG_INFO:
    return 3;
  case LOG_DEBUG:
    return 4;
  default:
    return 0;
  }
}

/**
 * @brief 指定されたログレベルとメッセージを処理します。
 *
 * CURRENT_LOG_LEVELに基づいてメッセージをフィルタリングし、
 * MQTTまたはシリアルポートに出力します。
 * @param level ログレベル。
 * @param message 出力するメッセージ。
 */
void logMessage(LogLevel level, const String &message) {
  if (level > CURRENT_LOG_LEVEL)
    return;

  DynamicJsonDocument doc(128);
  doc["level"]   = getLogLevelInt(level);
  doc["message"] = message;

  String payload;
  serializeJson(doc, payload);

#if defined(LOG_TO_MQTT)
  publishMQTT(MQTT_PUB_LOG, payload);
#endif
#if defined(LOG_TO_SERIAL)
  Serial.println(getLogLevelString(level) + message);
#endif
}
