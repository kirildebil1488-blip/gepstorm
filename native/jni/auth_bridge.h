#pragma once
#include <jni.h>
#include <string>
std::string auth_read_token(JNIEnv*, jobject);
bool auth_verify_token(const std::string&, const std::string&, const std::string&, const std::string&);
