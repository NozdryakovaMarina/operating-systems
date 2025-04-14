// BEIL 

#include <windows.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <queue>


template<typename T>
struct Mailbox {
    std::queue<T> messages;
    HANDLE access_semaphore;
    HANDLE items_semaphore;
};


Mailbox<double> mailboxA;
Mailbox<double> mailboxB;
Mailbox<double> mailboxSquareA;
Mailbox<double> mailboxSquareB;
Mailbox<double> mailboxSum;
Mailbox<double> mailboxResult;

HANDLE log_semaphore;

std::string DoubleToString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void LogWithTimestamp(const char* message) {
    SYSTEMTIME st;
    GetLocalTime(&st);

    WaitForSingleObject(log_semaphore, INFINITE);
    std::cout << "["
        << std::setfill('0') << std::setw(2) << st.wHour << ":"
        << std::setfill('0') << std::setw(2) << st.wMinute << ":"
        << std::setfill('0') << std::setw(2) << st.wSecond << "."
        << std::setfill('0') << std::setw(3) << st.wMilliseconds
        << "][Thread " << GetCurrentThreadId() << "] "
        << message << std::endl;
    ReleaseSemaphore(log_semaphore, 1, NULL);
}

template<typename T>
void SendToMailbox(Mailbox<T>& mailbox, const T& value) {
    WaitForSingleObject(mailbox.access_semaphore, INFINITE);
    mailbox.messages.push(value);
    ReleaseSemaphore(mailbox.access_semaphore, 1, NULL);
    ReleaseSemaphore(mailbox.items_semaphore, 1, NULL);
}

template<typename T>
T ReceiveFromMailbox(Mailbox<T>& mailbox) {
    WaitForSingleObject(mailbox.items_semaphore, INFINITE);
    WaitForSingleObject(mailbox.access_semaphore, INFINITE);
    T value = mailbox.messages.front();
    mailbox.messages.pop();
    ReleaseSemaphore(mailbox.access_semaphore, 1, NULL);
    return value;
}

DWORD WINAPI SquareThread(LPVOID lpParam) {
    int thread_id = (int)lpParam;

    while (true) {
        double value;
        Mailbox<double>* output_mailbox;
        const char* prefix;

        if (thread_id == 0) {
            value = ReceiveFromMailbox(mailboxA);
            output_mailbox = &mailboxSquareA;
            prefix = "A";
        }
        else {
            value = ReceiveFromMailbox(mailboxB);
            output_mailbox = &mailboxSquareB;
            prefix = "B";
        }

        LogWithTimestamp((std::string("Processing ") + prefix + " value: " + DoubleToString(value)).c_str());

        double result = value * value;
        Sleep(1000);
        LogWithTimestamp((std::string(prefix) + " square result: " + DoubleToString(result)).c_str());

        SendToMailbox(*output_mailbox, result);
    }
    return 0;
}

DWORD WINAPI SumThread(LPVOID) {
    while (true) {
        double a = ReceiveFromMailbox(mailboxSquareA);
        double b = ReceiveFromMailbox(mailboxSquareB);
        LogWithTimestamp((std::string("Adding ") + DoubleToString(a) + " and " + DoubleToString(b)).c_str());

        double result = a + b;
        Sleep(1000);
        LogWithTimestamp((std::string("Sum result: ") + DoubleToString(result)).c_str());

        SendToMailbox(mailboxSum, result);
    }
    return 0;
}

DWORD WINAPI SqrtThread(LPVOID) {
    while (true) {
        double value = ReceiveFromMailbox(mailboxSum);
        LogWithTimestamp((std::string("Calculating square root of ") + DoubleToString(value)).c_str());

        double result = sqrt(value);
        Sleep(2000);
        LogWithTimestamp((std::string("Final result: ") + DoubleToString(result)).c_str());

        SendToMailbox(mailboxResult, result);
    }
    return 0;
}

void InitializeMailbox(Mailbox<double>& mailbox) {
    mailbox.access_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
    mailbox.items_semaphore = CreateSemaphore(NULL, 0, 100, NULL);
}

int main() {
    setlocale(LC_ALL, "Russian");

    InitializeMailbox(mailboxA);
    InitializeMailbox(mailboxB);
    InitializeMailbox(mailboxSquareA);
    InitializeMailbox(mailboxSquareB);
    InitializeMailbox(mailboxSum);
    InitializeMailbox(mailboxResult);

    log_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    HANDLE threads[4];
    threads[0] = CreateThread(NULL, 0, SquareThread, (LPVOID)0, 0, NULL); 
    threads[1] = CreateThread(NULL, 0, SquareThread, (LPVOID)1, 0, NULL);
    threads[2] = CreateThread(NULL, 0, SumThread, NULL, 0, NULL);
    threads[3] = CreateThread(NULL, 0, SqrtThread, NULL, 0, NULL);

    int check = 1;
    while (true) {
        double a, b;
        std::cout << "¬ведите катет A: ";
        std::cin >> a;
        std::cout << "¬ведите катет B: ";
        std::cin >> b;

        SendToMailbox(mailboxA, a);
        SendToMailbox(mailboxB, b);

        double result = ReceiveFromMailbox(mailboxResult);
        LogWithTimestamp((std::string("\n√ипотенуза: ") + DoubleToString(result)).c_str());

        std::cout << "\n\n¬ведите 0 дл€ продолжени€ или другое число дл€ выхода: ";
        std::cin >> check;
        if (check != 0) break;
    }

    for (int i = 0; i < 4; i++) {
        TerminateThread(threads[i], 0);
        CloseHandle(threads[i]);
    }

    CloseHandle(mailboxA.access_semaphore);
    CloseHandle(mailboxA.items_semaphore);
    CloseHandle(mailboxB.access_semaphore);
    CloseHandle(mailboxB.items_semaphore);
    CloseHandle(mailboxSquareA.access_semaphore);
    CloseHandle(mailboxSquareA.items_semaphore);
    CloseHandle(mailboxSquareB.access_semaphore);
    CloseHandle(mailboxSquareB.items_semaphore);
    CloseHandle(mailboxSum.access_semaphore);
    CloseHandle(mailboxSum.items_semaphore);
    CloseHandle(mailboxResult.access_semaphore);
    CloseHandle(mailboxResult.items_semaphore);
    CloseHandle(log_semaphore);

    return 0;
}