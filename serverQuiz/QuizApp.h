#pragma once

#include <curl/curl.h>

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <random>

class QuizApp {
public:
    std::wstring fetchUrl(const std::string& url);
    std::wstring requestQuestionAndAnswers(const std::wstring& url, int* questionIndex);
    bool pushAnswerToServer(int questionIndex, int answerIndex);

private:
    static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
    std::vector<std::vector<std::wstring>> questions;

    std::wstring utf8_to_wstring(const std::string& str);
};
