#pragma once
#include<logSys.hpp>
#include<spdlog/sinks/basic_file_sink.h>
#include<spdlog/async.h>
#include<spdlog/sinks/stdout_color_sinks.h>

#define LOG_RELATIVE_PATH "logs/runtime_log.txt"


using namespace myEngine;
LogSystem::LogSystem()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%^%l%$] %v");
    auto file_out = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_RELATIVE_PATH);
    file_out->set_level(spdlog::level::err);

    const spdlog::sinks_init_list sink_list = { console_sink,file_out };

    spdlog::init_thread_pool(8192, 3);

    m_logger = std::make_shared<spdlog::async_logger>("muggle_logger",
        sink_list.begin(),
        sink_list.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);
    m_logger->set_level(spdlog::level::trace);

    spdlog::register_logger(m_logger);
}

LogSystem::~LogSystem()
{
    m_logger->flush();
    spdlog::drop_all();
}