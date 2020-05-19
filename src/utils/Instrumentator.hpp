//
// Basic instrumentation profiler by Cherno
// https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

// Usage: include this header file somewhere in your code (eg. precompiled
// header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like
//     this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session

#pragma once

#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fstream>
#include <memory>
#include <string>
#include <thread>

#ifdef linux
#include <sys/types.h>
#include <unistd.h>
#endif

namespace utils {
struct ProfileResult {
    std::string Name;
    long long   Start, End;
    uint32_t    ThreadID;
    uint32_t    PID;
};

struct InstrumentationSession {
    std::string Name;
};

class Instrumentor {
  private:
    std::unique_ptr<InstrumentationSession> m_CurrentSession;
    std::ofstream                           m_OutputStream;
    int                                     m_ProfileCount;

    Instrumentor() : m_CurrentSession(), m_ProfileCount(0) {}

  public:
    void BeginSession(const std::string &name,
                      const std::string &filepath = "profile.json") {
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = std::make_unique<InstrumentationSession>(
            InstrumentationSession{name});
    }

    void EndSession() {
        WriteFooter();
        m_OutputStream.flush();
        m_OutputStream.close();
        m_CurrentSession.reset();
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult &result) {
        if (m_ProfileCount++ > 0){
            m_OutputStream << ",";
        }

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');
        // auto message = fmt::format(
        //    R"({"cat":"function","dur":{},"name":{},"ph":"X","pid":{},"tid":{},"ts":{}})",
        //    (result.End - result.Start), name, result.PID, result.ThreadID,
        //    result.Start);
        // std::cout<< message <<'\n';

        //        m_OutputStream <<message;

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";
    }

    void WriteHeader() {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
    }

    void WriteFooter() { m_OutputStream << "]}"; }

    static Instrumentor &Get() {
        static Instrumentor instance;
        return instance;
    }
};

class InstrumentationTimer {
  public:
    InstrumentationTimer(const char *name) : m_Name(name), m_Stopped(false) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer() {
        if (!m_Stopped)
            Stop();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(
                m_StartTimepoint)
                .time_since_epoch()
                .count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(
                            endTimepoint)
                            .time_since_epoch()
                            .count();

        auto threadID = static_cast<uint32_t>(
            std::hash<std::thread::id>{}(std::this_thread::get_id()));
        uint32_t pid{0};
#ifdef linux
        pid = static_cast<uint32_t>(getpid());
#endif
        Instrumentor::Get().WriteProfile({m_Name, start, end, threadID, pid});

        m_Stopped = true;
    }

  private:
    const char *m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock>
         m_StartTimepoint;
    bool m_Stopped;
};
} // namespace utils

#define INSTRUMENTATOR_PROFILE 0
#ifdef INSTRUMENTATOR_PROFILE
#define INSTRUMENTATOR_FUNC_SIG __func__
#define INSTRUMENTATOR_PROFILE_BEGIN_SESSION(name, filepath)                   \
    ::utils::Instrumentor::Get().BeginSession(name, filepath)
#define INSTRUMENTATOR_PROFILE_END_SESSION()                                   \
    ::utils::Instrumentor::Get().EndSession()
#define INSTRUMENTATOR_PROFILE_SCOPE(name)                                     \
    ::utils::InstrumentationTimer timer##__LINE__(name);
#define INSTRUMENTATOR_PROFILE_FUNCTION()                                      \
    INSTRUMENTATOR_PROFILE_SCOPE(INSTRUMENTATOR_FUNC_SIG)
#else
#define INSTRUMENTATOR_PROFILE_BEGIN_SESSION(name, filepath)
#define INSTRUMENTATOR_PROFILE_END_SESSION()
#define INSTRUMENTATOR_PROFILE_SCOPE(name)
#define INSTRUMENTATOR_PROFILE_FUNCTION()

#endif
