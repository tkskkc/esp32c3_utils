#pragma once
#include <Arduino.h>
#include <settings.h>

enum LogLevel {
  LOG_ERROR,
  LOG_WARN,
  LOG_INFO,
  LOG_DEBUG
};

/**
 * @brief LogLevel列挙型に対応する文字列を返します。
 * @param level ログレベル。
 * @return ログレベルの文字列表現。
 */
String getLogLevelString(LogLevel level);

/**
 * @brief LogLevel列挙型に対応する整数値を返します。
 * @param level ログレベル。
 * @return ログレベルの整数表現。
 */
int getLogLevelInt(LogLevel level);

/**
 * @brief 指定されたログレベルとメッセージを処理します。
 * 
 * CURRENT_LOG_LEVELに基づいてメッセージをフィルタリングし、
 * MQTTまたはシリアルポートに出力します。
 * @param level ログレベル。
 * @param message 出力するメッセージ。
 */
void logMessage(LogLevel level, const String& message);
