#include "QuizApp.h"
#include <codecvt>
#include <locale>
#include <sstream>
#include <random>
#include <iostream>



#pragma warning(disable : 4996)  // disables "deprecated function" warning


std::wstring QuizApp::utf8_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(str);
}

size_t QuizApp::write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t real_size = size * nmemb;
    std::string* response = static_cast<std::string*>(userdata);
    response->append(ptr, real_size);
    return real_size;
}

std::wstring QuizApp::fetchUrl(const std::string& url)
{
    CURL* curl = curl_easy_init();
    std::string response_utf8;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_utf8);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "QuizApp/1.0");
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
    }

    return utf8_to_wstring(response_utf8);
}

std::wstring QuizApp::requestQuestionAndAnswers(const std::wstring& url_wide, int* questionIndex)
{
    std::string url_narrow(url_wide.begin(), url_wide.end()); // naive conversion (URL is ASCII-safe)
    std::wstring data = fetchUrl(url_narrow);
    questions.clear();

    std::wstringstream iss(data);
    std::wstring line;

    while (std::getline(iss, line)) {
        std::vector<std::wstring> questionParts;
        std::wstringstream lineStream(line);
        std::wstring part;

        while (std::getline(lineStream, part, L';')) {
            if (!part.empty() && part.back() == L'\r')
                part.pop_back(); // clean \r
            questionParts.push_back(part);
        }

        if (questionParts.size() >= 6) {
            questions.push_back(questionParts);
        }
    }

    if (questions.empty()) {
        *questionIndex = -1;
        return L"No questions available";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, questions.size() - 1);
    int randomIndex = distr(gen);
    auto& question = questions[randomIndex];

    *questionIndex = randomIndex;

    std::wstring result = question[0] + L"\n";
    for (int i = 1; i < 5; i++) {
        result += std::to_wstring(i - 1) + L") " + question[i] + L"\n";
    }

    return result;
}

bool QuizApp::pushAnswerToServer(int questionIndex, int answerIndex)
{
    return std::stoi(questions[questionIndex].back()) == answerIndex;
}
