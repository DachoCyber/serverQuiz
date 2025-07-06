#include <iostream>
#include <array>

#include <fcntl.h>
#include <io.h>

#include "QuizApp.h"


int main()
{

    _setmode(_fileno(stdout), _O_U16TEXT);

    int numOfQuestions = 10;
    const std::wstring url = L"https://alas.matf.bg.ac.rs/~mr22033/quizApp/questions.txt";

    QuizApp quiz;
    for (int i = 0; i < numOfQuestions; i++) {

        int questionIndex;

        std::wstring questionAndAnswers = quiz.requestQuestionAndAnswers(url, &questionIndex);
        
        
        std::wcout << questionAndAnswers << std::endl;

        int answerIndex;
        std::cin >> answerIndex;

        bool correct = quiz.pushAnswerToServer(questionIndex, answerIndex);

        if (correct)
            std::wcout << L"Answer is correct!" << std::endl;
        else
            std::wcout << L"Answer is not correct!" << std::endl;
    }

    std::cout << "Hello World!\n";
}
